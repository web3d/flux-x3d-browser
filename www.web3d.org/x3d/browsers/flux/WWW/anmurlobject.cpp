/********************************************************************************
 * Flux
 *
 * File: anmurlobject.cpp
 * Description: Base object for WWW resources
 *
 * (C) 2001-2006 by Media Machines, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *********************************************************************************/

#include "stdafx.h"
#include "anmurlobject.h"
#include "anmassetcache.h"
#include "anmfilemanager.h"
#include "anmloadsensor.h"
#include <wininet.h>

CAnmUrlObject::CAnmUrlObject()
{		
	m_url = new StringArray;
	m_baseUrl = NULL;
}

CAnmUrlObject::~CAnmUrlObject()
{
	SafeUnRef(m_url);
	SafeUnRef(m_baseUrl);

	// Unref any watchers
	std::list<CAnmLoadSensor *>::iterator iter;

	for (iter = m_loadSensors.begin(); iter != m_loadSensors.end(); iter++)
	{
		CAnmLoadSensor *pLoadSensor = *iter;
		pLoadSensor->UnRef();
	}

}

void CAnmUrlObject::Load(cApplication *pApp, bool bAllowStreaming )
{
	assert(m_url);

	if (m_url->size() > 0)
	{
		pApp->GetFileManager()->RequestLoadUrl(m_url, m_baseUrl ? m_baseUrl->GetBuf() : "",
			urlLoadedCallback, urlAssetLoadedCallback, NULL, urlProgressCallback, this, bAllowStreaming );
	}
}

CAnmAsset *CAnmUrlObject::urlLoadedCallback(String pRequestUrl, String pCacheFileName, void *userData)
{
	CAnmUrlObject *pUrlObject = (CAnmUrlObject *) userData;

	assert(pUrlObject != NULL);

	return pUrlObject->urlLoadedHandler(pRequestUrl, pCacheFileName);
}

void CAnmUrlObject::urlAssetLoadedCallback(String pRequestUrl, String pCacheFileName, CAnmAsset *pAsset, void *userData)
{
	CAnmUrlObject *pUrlObject = (CAnmUrlObject *) userData;

	assert(pUrlObject != NULL);

	pUrlObject->urlLoadedHandler(pRequestUrl, pAsset);
}

CAnmAsset *CAnmUrlObject::urlLoadedHandler(String pRequestUrl, String pCacheFileName)
{
	// Add a ref-- not yet; only if we add this name to some kind of cache
	// pCacheFileName->Ref();

	// This is the thing the subclasses override
	CAnmAsset *pAsset = handleUrlLoaded(pRequestUrl, pCacheFileName);

	// Notify anyone watching me
	std::list<CAnmLoadSensor *>::iterator iter;

	for (iter = m_loadSensors.begin(); iter != m_loadSensors.end(); iter++)
	{
		CAnmLoadSensor *pLoadSensor = *iter;
		pLoadSensor->NotifyLoaded(this);
	}

	return pAsset;
}

void CAnmUrlObject::urlLoadedHandler(String pRequestUrl, CAnmAsset *pAsset)
{
	// This is the thing the subclasses override
	handleUrlLoaded(pRequestUrl, pAsset);

	// Notify anyone watching me
	std::list<CAnmLoadSensor *>::iterator iter;

	for (iter = m_loadSensors.begin(); iter != m_loadSensors.end(); iter++)
	{
		CAnmLoadSensor *pLoadSensor = *iter;
		pLoadSensor->NotifyLoaded(this);
	}


}

void CAnmUrlObject::urlProgressCallback(float progress, void *userData)
{
	CAnmUrlObject *pUrlObject = (CAnmUrlObject *) userData;

	assert(pUrlObject != NULL);

	pUrlObject->urlProgressHandler(progress);
}

void CAnmUrlObject::urlProgressHandler(float progress)
{
	// Notify anyone watching me
	std::list<CAnmLoadSensor *>::iterator iter;

	for (iter = m_loadSensors.begin(); iter != m_loadSensors.end(); iter++)
	{
		CAnmLoadSensor *pLoadSensor = *iter;
		pLoadSensor->NotifyProgress(this, progress);
	}
}


void CAnmUrlObject::ComputeRootedUrl(const char *url, char *outBuf, int maxlen)
{
	assert(maxlen > 0);
	outBuf[0] = '\0';

	int len = strlen(url);
	for (int i = 0; i < len + 1; i++)
	{
		outBuf[i] = url[i];

		// Break at first slash that is not:   {://}
		if( i>0 &&
			( outBuf[i] == '/' || outBuf[i] == '\\' ) &&
			outBuf[i-1] != '/' &&
			outBuf[i-1] != '\\' &&
			outBuf[i-1] != ':' ) {
			outBuf[i] = '\0';
			break;
		}
	}
}


void CAnmUrlObject::ComputeBaseUrl(const char *url, char *outBuf, int maxlen)
{
	assert(maxlen > 0);

	char *cp;

	// N.B.: length
	char tmp[ANMMAXSTRBUF];

	// copy to tmp buffer, substituting '/' for '\'
	int len = strlen(url);
	for (int i = 0; i < len + 1; i++)
	{
		if (url[i] == '\\')
			tmp[i] = '/';
		else
			tmp[i] = url[i];
	}

	cp = strrchr(tmp, '/');

	if (!cp)
	{
		outBuf[0] = '\0';
		return;
	}

	len = cp - tmp + 1;

	if (len > maxlen - 1)
		len = maxlen - 1;

	strncpy(outBuf, tmp, len);
	outBuf[len] = '\0';
}


void CAnmUrlObject::ComputeFullName(const char *url, const char *baseUrl, char *fullname)
{
	if (baseUrl && strlen(baseUrl))
	{
#if 1		// Let's try Dave's improvements for a while...
		
		if (IsRootedPath(url) &&
			IsWebPath( baseUrl ) ) {
			char RootUrl[ANMMAXSTRBUF];
			CAnmUrlObject::ComputeRootedUrl( baseUrl, RootUrl, ANMMAXSTRBUF );
			strcpy(fullname, RootUrl);
			strcat(fullname, url);
		}
		else if (CAnmUrlObject::IsRelativePath(url))
		{
			strcpy(fullname, baseUrl);
			strcat(fullname, url);
		}

#else
		if (CAnmUrlObject::IsRelativePath(url) || PathIsRelative (url))
		{
			char szCombined[INTERNET_MAX_URL_LENGTH+2];
			DWORD cchCombined = INTERNET_MAX_URL_LENGTH;
			DWORD dwFlags = ICU_DECODE;
			BOOL success = InternetCombineUrl (baseUrl, url, szCombined, &cchCombined, dwFlags);
			if (success)
				strcpy (fullname, szCombined); // hope fullname is long enough
		}
#endif

		else
			strcpy(fullname, url);

	}
	else
	{
		strcpy(fullname, url);
	}

	// Look for '#' url silliness and remove it before trying to load
	char *hashchr = strchr(fullname, '#');

	if (hashchr)
		*hashchr = '\0';

}



// Uses Windows based calls to determine if file exists on web.
//
//
BOOL CAnmUrlObject::DoesWebFileExist(const char *fname)
{
	BOOL bFound = FALSE;
	if( IsWebPath( fname ) ) {


bFound = TRUE;

/*****
		CInternetSession* session = new CInternetSession( ( "Flux" ), 
							1, INTERNET_OPEN_TYPE_PRECONFIG );
		if( session ) {

			CStdioFile* fin = session->OpenURL( fname, 1, INTERNET_FLAG_TRANSFER_BINARY );
			if( fin ) {
				bFound = TRUE;
				fin->Close();
			}
			session->Close();
		}

****/
	}
	return bFound;
}


BOOL CAnmUrlObject::IsNonStreamable(const char *fname)
{
	int len = strlen( fname );
	if( len > 4 &&
		((stricmp( fname + len - 4, ".gif" ) == 0 ) 
			|| (stricmp( fname + len - 4, ".wav" ) == 0 ))) {
		return TRUE;
	}
	return FALSE;
}


BOOL CAnmUrlObject::IsWebPath(const char *fname)
{
	if (!strnicmp(fname, "http://", 7))
		return TRUE;

	if (!strnicmp(fname, "ftp://", 6))
		return TRUE;

	return FALSE;
}

BOOL CAnmUrlObject::IsRootedPath(const char *fname)
{
	if ( fname[0] == '/' || fname[0] == '\\'  )
		return TRUE;

	return FALSE;
}

BOOL CAnmUrlObject::IsRelativePath(const char *fname)
{
	if( IsWebPath(fname) ) {
		return FALSE;
	}
	return !IsLocalFile(fname);
}

BOOL CAnmUrlObject::IsLocalFile(const char *fname)
{
	if (!strnicmp(fname, "file:", 5))
		return TRUE;

	if (fname[1] == ':')
		return TRUE;

	return FALSE;
}


char *CAnmUrlObject::GetViewpointAnchor(String url)
{
	const char *cp = url->GetBuf();

	char *vp = strrchr(cp, '#');
	if (vp && *(++vp))
		return vp;
	else
		return NULL;
}

void CAnmUrlObject::AddLoadSensor(class CAnmLoadSensor *pLoadSensor)
{
	assert(pLoadSensor != NULL);
	m_loadSensors.push_back(pLoadSensor);
	pLoadSensor->Ref();
}

void CAnmUrlObject::RemoveLoadSensor(class CAnmLoadSensor *pLoadSensor)
{
	assert(pLoadSensor != NULL);
	m_loadSensors.remove(pLoadSensor);
	pLoadSensor->UnRef();
}

// Accessors
void CAnmUrlObject::SetUrl(StringArray *pUrl)
{
	assert(pUrl != NULL);

	SafeUnRef(m_url);
	m_url = pUrl;
	m_url->Ref();
}

void CAnmUrlObject::SetBaseUrl(String baseUrl)
{
	SafeUnRef(m_baseUrl);
	m_baseUrl = baseUrl;
	if (m_baseUrl)
		m_baseUrl->Ref();

}

// Interface glue
ANMDEFINE_INTERFACE(CAnmUrlObject)