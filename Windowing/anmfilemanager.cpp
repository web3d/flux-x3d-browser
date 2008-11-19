/********************************************************************************
 * Flux
 *
 * File: anmfilemgr.cpp
 * Description: File management class
 *						Here's where we handle caching and
 *							async loading
 *                      N.B.: completely Windows-specific
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
#include "../AnimaApp.h"
#include "anmfilemanager.h"
#include "anmassetcache.h"
#include "anmmessage.h"
#include "anmnetworkthread.h"
#include "anmurlobject.h"
#include "anmbindstatuscallback.h"
#include "application.h"
#include "window.h"
#include <string>

#include "wininet.h"
#include "urlmon.h"
#include <io.h>

CAnmFileManager::CAnmFileManager(class cApplication *pApp)
{
	m_app = pApp;
	m_assetCache = NULL;
}

CAnmFileManager::~CAnmFileManager()
{
	SafeDelete(m_assetCache);
}

bool CAnmFileManager::UrlInCache(const char *url, String *pCacheFileName)
{
	// N.B.: let's go with this for a while. Using this method gives us back
	// all cache entries, including stale ones. Let's see if URLDownloadToCacheFile()
	// will do all the modified-date-checking for us...

	// return false;

	INTERNET_CACHE_ENTRY_INFO *pIci = NULL;
	DWORD bufsize = 0L;
	BOOL stat = GetUrlCacheEntryInfo(url, pIci, &bufsize);

	if (!stat)
	{
		DWORD err = GetLastError();
		if (err == ERROR_FILE_NOT_FOUND)
		{
			return false;
		}
		else if (err == ERROR_INSUFFICIENT_BUFFER)
		{
			pIci = (INTERNET_CACHE_ENTRY_INFO *) new char[bufsize];
			memset(pIci, 0, bufsize);
			pIci->dwStructSize = bufsize;

			stat = GetUrlCacheEntryInfo(url, pIci, &bufsize);

			if (stat)
			{
				LONG filetimecompare = CompareFileTime(&pIci->LastModifiedTime, &pIci->LastAccessTime);
				if (filetimecompare < 1)

				{
					*pCacheFileName = new CAnmString(pIci->lpszLocalFileName);
					return true;
				}
				else
					return false;
			}
		}
		else
		{
			// ???
		}
	}

	return false;
}

bool CAnmFileManager::LockUrlInCache(String url)
{
	assert(url);

	INTERNET_CACHE_ENTRY_INFO *pIci = NULL;
	DWORD bufsize = 0L;
	BOOL stat = RetrieveUrlCacheEntryFile(url->GetBuf(), pIci, &bufsize, 0L);

	if (!stat)
	{
		DWORD err = GetLastError();
		if (err == ERROR_FILE_NOT_FOUND)
		{
			return false;
		}
		else if (err == ERROR_INSUFFICIENT_BUFFER)
		{
			pIci = (INTERNET_CACHE_ENTRY_INFO *) new char[bufsize];
			memset(pIci, 0, bufsize);
			pIci->dwStructSize = bufsize;

			stat = RetrieveUrlCacheEntryFile(url->GetBuf(), pIci, &bufsize, 0L);

			if (stat)
			{
				return true;
			}
		}
		else
		{
			// ???
		}
	}

	return false;
}

bool CAnmFileManager::UnlockUrlInCache(String url)
{
	assert(url);

	INTERNET_CACHE_ENTRY_INFO *pIci = NULL;
	DWORD bufsize = 0L;
	return (bool) UnlockUrlCacheEntryFile(url->GetBuf(), 0L);

}

static char *CanonicalizeUrl(const char *url)
{
	DWORD buflen = 1;

	DWORD flags = ICU_DECODE | ICU_NO_ENCODE;

	char dummy;
	BOOL stat = InternetCanonicalizeUrl(url, &dummy, &buflen, flags);

	char *buf = new char[buflen+1];

	stat = InternetCanonicalizeUrl(url, buf, &buflen, flags);

	if (stat)
		return buf;
	else
	{
		delete buf;
		return NULL;
	}
}

bool CAnmFileManager::UrlLoaded(const char *url, String *pCacheFileName, String *pCacheUrl)
{
	String cachedFileName = NULL;

	if (CAnmUrlObject::IsLocalFile(url))
	{
		char *canonical = CanonicalizeUrl(url);

		if (canonical == NULL)
			return false;

		char *fname = canonical;
		if (!strncmp(fname, "file://", 7))
		{
			fname = fname + 7;
			if (*fname == '/')
				fname++;
		}

		// Check for existence of the local file.
		if (_access(fname, 0) == -1)
		{
			return false;
		}
		else
		{
			cachedFileName = new CAnmString(fname);
		}

		delete canonical;
	}
	else if (UrlInCache(url, &cachedFileName))
	{
		if (pCacheUrl)
			*pCacheUrl = new CAnmString(url);
	}
	else
	{
		return false;
	}
	
	*pCacheFileName = cachedFileName;

	return true;
}

bool CAnmFileManager::UrlLoaded(StringArray *pUrl, const char *baseUrl, String *pRequestUrl, String *pCacheFileName)
{
	String url;
	String cachedFileName = NULL;

	char fullname[ANMMAX_PATH];
	for (int i = 0; i < pUrl->size(); i++)
	{
		url = (*pUrl)[i];
		CAnmUrlObject::ComputeFullName(url->GetBuf(), baseUrl, fullname);
		if (UrlLoaded(fullname, pCacheFileName, NULL))
		{
			*pRequestUrl = url;
			return true;
		}

	}

	return false;
}




eAnmReturnStatus CAnmFileManager::IsStreamableFile(String url, const char *baseUrl,
												 String *pCacheFileName, String *pCacheUrl, 
												 CAnmAsset **ppAsset,
												 sAnmLoadRequest *pRequest)
{
	eAnmReturnStatus stat = eAnmReturnAllBad;

	if( pRequest->requestAllowStreaming ) {

		// Compute fully qualified url
		char fullname[ANMMAX_PATH];
		CAnmUrlObject::ComputeFullName(url->GetBuf(), baseUrl, fullname);

		if( !CAnmUrlObject::IsNonStreamable( fullname ) &&
			 CAnmUrlObject::IsWebPath( fullname ) &&
			 CAnmUrlObject::DoesWebFileExist( fullname ) ) {

			// krv need to verify that the file exists.
			//
			
			*pCacheFileName = new CAnmString( fullname );
			if (pCacheUrl) {
				*pCacheUrl = new CAnmString(fullname);
			}
			if (ppAsset) {
				*ppAsset = NULL;
			}

			// Start status message
			// ANMMSG_STREAMINGURL : "Streaming %s..."
			char *statusbuf = (char *) malloc(strlen(fullname) + strlen(ANMMSG_STREAMINGURL) + 1);
			sprintf(statusbuf, ANMMSG_STREAMINGURL, fullname );
			m_app->SetStatusText(statusbuf);
			m_app->WriteToConsole(statusbuf);

			delete statusbuf;

			stat = eAnmReturnAllGood;
		}
	}
	return stat;
}

eAnmReturnStatus CAnmFileManager::LoadCachedFile(String url, const char *baseUrl,
												 String *pCacheFileName, String *pCacheUrl, 
												 CAnmAsset **ppAsset,
												 sAnmLoadRequest *pRequest)
{
	// Compute fully qualified url
	char *fullname = (char *) malloc(url->length() + strlen(baseUrl) + 1);
	CAnmUrlObject::ComputeFullName(url->GetBuf(), baseUrl, fullname);

	// Start status message
	// ANMMSG_LOADINGURL : "Loading %s..."
	char *statusbuf = (char *) malloc(strlen(fullname) + strlen(ANMMSG_LOADINGURL) + 1);

	sprintf(statusbuf, ANMMSG_LOADINGURL, fullname);

	assert(m_app);

	// See if the file is already on the local machine
	// N.B.: this tells us whether it's local or in the cache; the file still
	// might be newer on the server, so unless it's in our asset cache
	// we're going to call URLDownloadToCacheFile() anyway. So right now,
	// this is mostly for the side-effect of computing pCacheFileName and pCacheUrl

	bool urlLoaded = UrlLoaded(fullname, pCacheFileName, pCacheUrl);
	
	CAnmAsset *pAsset = NULL;
	if (urlLoaded)
		pAsset = LookupAssetInCache(*pCacheFileName);

	if (pAsset)
	{
		if (ppAsset)
			*ppAsset = pAsset;
	}
	else if (urlLoaded && CAnmUrlObject::IsLocalFile(fullname))
	{
		m_app->SetStatusText(statusbuf);
		m_app->WriteToConsole(statusbuf);
		// ANMMSG_DONELOADINGURL : "Done"
		m_app->SetStatusText(ANMMSG_DONELOADINGURL);
		m_app->WriteToConsole(ANMMSG_DONELOADINGURL);
	}
	else
	{
		// If not, download it
		CAnmBindStatusCallback bsc(m_app, this, pRequest);
		String cachedFileName = new CAnmString();
		cachedFileName->alloc(ANMMAX_PATH);

		// Start loading message
		m_app->SetStatusText(statusbuf);
		m_app->WriteToConsole(statusbuf);

		if ( FAILED(URLDownloadToCacheFile(
		  NULL,										// control's IUnknown
		  fullname,			                        // URL to download
		  cachedFileName->GetBuf(),					// buffer to fill with name
		  ANMMAX_PATH-1,							// buffer length
		  0,										// reserved
		  &bsc )) )									// async status update
		{
			// ANMMSG_URLNOTFOUND : "not found."
			m_app->SetStatusText(ANMMSG_URLNOTFOUND);

			// ANMMSG_URLCOULDNOTBELOCATED : "the URL could not be located."
			m_app->WriteToConsole(ANMMSG_URLCOULDNOTBELOCATED);

			delete fullname;
			delete statusbuf;
			return eAnmReturnAllBad;
		}

		*pCacheFileName = cachedFileName;
		if (pCacheUrl)
			*pCacheUrl = new CAnmString(fullname);

		// End loading message
		// ANMMSG_DONELOADINGURL : "Done"
		m_app->SetStatusText(ANMMSG_DONELOADINGURL);
		m_app->WriteToConsole(ANMMSG_DONELOADINGURL);
	}


	delete fullname;
	delete statusbuf;
	return eAnmReturnAllGood;
}


eAnmReturnStatus CAnmFileManager::LinkToUrl(const char *url, const char *baseUrl,
											const char *target)
{
	char *fullname = (char *) malloc(strlen(url) + strlen(baseUrl) + 1);
	CAnmUrlObject::ComputeFullName(url, baseUrl, fullname);

	assert(m_app);
	cAnimaApp *pAnmApp = (cAnimaApp *) m_app;

	pAnmApp->LinkToUrl(fullname, target);
	
	delete fullname;

	return eAnmReturnAllGood;

}

eAnmReturnStatus CAnmFileManager::LinkToUrl(StringArray *pUrl, const char *baseUrl,
											StringArray *pParam)
{

	const char *url;
	const char *param;
	char *frame;

	if (pUrl->size() <= 0)
		return eAnmReturnAllBad;

	url = (*pUrl)[0]->GetBuf();
	if (pParam->size())
		param = (*pParam)[0]->GetBuf();
	else
		param = "";

	if (!strncmp(param, "target", 6))
	{
		char tbuf[1024];
		strcpy(tbuf, param+7);
		frame = tbuf;
	}
	else
		frame = "_self";

	return LinkToUrl(url, baseUrl, (const char *) frame);
}



eAnmReturnStatus CAnmFileManager::RequestLinkToUrl(StringArray *pUrls, const char *baseUrl, 
												   StringArray *pParams)
{
	sAnmLinkRequest *pRequest = new sAnmLinkRequest(m_app, pUrls, baseUrl, pParams);

	// N.B.: use the Windows message queue for linking to other URLs; IE seems
	// happiest when you request a link from the main (UI) thread instead of a
	// background thread
	assert(m_app);

	CWnd *pWnd = m_app->GetControlWindow();
	
	if (pWnd)
	{
		pWnd->PostMessage(WM_USERANMLINKTOURL, 0, (LPARAM) pRequest);
	}

	return eAnmReturnAllGood;
}


eAnmReturnStatus CAnmFileManager::ProcessLinkRequest(sAnmLinkRequest *pRequest)
{
	assert(pRequest != NULL);

	eAnmReturnStatus stat;
	stat = LinkToUrl(pRequest->requestUrls, pRequest->requestBaseUrl, pRequest->requestParams);

	delete pRequest;
	return stat;
}

eAnmReturnStatus CAnmFileManager::RequestLoadUrl(StringArray *pUrl, const char *baseUrl,
												 CAnmFileLoadedCallback lcb, 
												 CAnmFileAssetLoadedCallback alcb, 
												 CAnmFileActiveCallback acb,
												 CAnmFileProgressCallback pcb, void *userData,
												 bool bAllowStreaming)
{
	sAnmLoadRequest *pRequest = new sAnmLoadRequest(m_app, pUrl, baseUrl, lcb, alcb,
		acb, pcb, userData, bAllowStreaming);

	Lock();
	
	if (bAllowStreaming)
		m_streamLoadRequests.push_back(pRequest);
	else
		m_loadRequests.push_back(pRequest);
	
	// cAnimaApp *pAnmApp = (cAnimaApp *) m_app;
	// CAnmNetworkThread *pnt = pAnmApp->GetNetworkThread();
	// if (pnt)
	//	pnt->Resume();

	Unlock();

	return eAnmReturnAllGood;
}

#define USE_GROUP_PRIORITY 1

eAnmReturnStatus CAnmFileManager::ProcessLoadRequest(sAnmLoadRequest *pRequest)
{
	assert(pRequest != NULL);

	String requestUrl;
	String cachedFileName = NULL;
	String cachedUrl = NULL;
	CAnmAsset *pAsset = NULL;

	eAnmReturnStatus stat = eAnmReturnAllBad;
	bool bStreaming = false;

	for (int i = 0; i < (*pRequest->requestUrls).size(); i++)
	{
		requestUrl = (*pRequest->requestUrls)[i];
		if (requestUrl->length() > 0)
		{
			stat = IsStreamableFile(requestUrl, pRequest->requestBaseUrl, &cachedFileName, &cachedUrl,
				&pAsset, pRequest);
			if( stat == eAnmReturnAllGood ) {
				bStreaming = true;
				break;
			}
			else {
				stat = LoadCachedFile(requestUrl, pRequest->requestBaseUrl, &cachedFileName, &cachedUrl,
					&pAsset, pRequest);
				if (stat == eAnmReturnAllGood) {
					break;
				}
			}
		}
	}

	if (stat == eAnmReturnAllGood)
	{
		// Lock the file itself so it isn't thrown out of the cache
		FILE *cachefilefp = NULL;
		if( !bStreaming ) {
			cachefilefp = fopen(cachedFileName->GetBuf(), "r");
			if (cachefilefp == NULL)
			{
				// N.B.: we probably need to panic or warn. What to do?

				// Clean up and get out
				delete pRequest;
				return eAnmReturnAllBad;
			}
		}

#if (!USE_GROUP_PRIORITY)
		// Bump network thread priority way up when handling the load; this is
		// where real work gets done like loading an X3D file or creating a texture surface
		cAnimaApp *pAnmApp = (cAnimaApp *) m_app;
		eAnmThreadPriority oldprio = eAnmThreadPriorityNormal;
		CAnmNetworkThread *pnt = pAnmApp->GetNetworkThread();
		if (pnt)
		{
			oldprio = pnt->GetPriority();
			pnt->SetPriority(eAnmThreadPriorityHighest);
		}
#endif

		// Lock the cache entry
		if (cachedUrl)
			LockUrlInCache(cachedUrl);

		// Do the callback; if the asset's in the cache use that otherwise this is
		// getting loaded for the first time
		if (pAsset && pRequest->requestAssetLoadedCallback)
			(*pRequest->requestAssetLoadedCallback)(requestUrl, cachedFileName, pAsset, pRequest->requestUserData);
		else if (pRequest->requestLoadedCallback)
		{
			pAsset = (*pRequest->requestLoadedCallback) (requestUrl, cachedFileName, pRequest->requestUserData);
			if (pAsset)
				AddAssetToCache(pAsset);
		}


		// Unlock the cache entry
		if (cachedUrl)
			UnlockUrlInCache(cachedUrl);

		// Unref the strings; we don't need them anymore
		cachedFileName->UnRef();
		SafeUnRef(cachedUrl);

#if (!USE_GROUP_PRIORITY)
		// Restore old thread priority
		if (pnt)
			pnt->SetPriority(oldprio);
#endif

		if( cachefilefp ) {
			fclose(cachefilefp);
		}
	}

	delete pRequest;
	return stat;
}

void CAnmFileManager::ProcessLoadRequests()
{
	std::vector<sAnmLoadRequest *> requests;

	Lock();

	int sz = m_loadRequests.size();
	int strmsz = m_streamLoadRequests.size();
	int totalsz = sz + strmsz;
	requests.reserve(sz + strmsz);

	while (!m_loadRequests.empty())
	{
		sAnmLoadRequest *pRequest = m_loadRequests.front();
		requests.push_back(pRequest);
		m_loadRequests.pop_front();
	}

	while (!m_streamLoadRequests.empty())
	{
		sAnmLoadRequest *pRequest = m_streamLoadRequests.front();
		requests.push_back(pRequest);
		m_streamLoadRequests.pop_front();
	}

	Unlock();
	
	if (totalsz > 0)
	{
#if USE_GROUP_PRIORITY
		// Bump network thread priority way up when handling the load; this is
		// where real work gets done like loading an X3D file or creating a texture surface
		cAnimaApp *pAnmApp = (cAnimaApp *) m_app;
		eAnmThreadPriority oldprio = eAnmThreadPriorityNormal;
		CAnmNetworkThread *pnt = pAnmApp->GetNetworkThread();
		if (pnt)
		{
			oldprio = pnt->GetPriority();
			pnt->SetPriority(eAnmThreadPriorityHighest);
		}
#endif

		for (int i = 0; i < totalsz; i ++)
		{
			// N.B.: don't do this if we're shutting down
//			if (!m_app->Paused())
			if (pnt->IsRunning())
				ProcessLoadRequest(requests[i]);
		}

#if USE_GROUP_PRIORITY
		// Restore old thread priority
		if (pnt)
			pnt->SetPriority(oldprio);
#endif
	}
}

bool CAnmFileManager::LoadRequestsPending()
{
	Lock();

	bool retval = !(m_loadRequests.empty() && m_streamLoadRequests.empty());

	Unlock();

	return retval;
}

void CAnmFileManager::NotifyProgress(float progress, void *userData)
{
	sAnmLoadRequest *pLoadRequest = (sAnmLoadRequest *) userData;

	if (pLoadRequest->requestProgressCallback)
			(*pLoadRequest->requestProgressCallback) (progress, pLoadRequest->requestUserData);

}

void CAnmFileManager::AddAssetToCache(CAnmAsset *pAsset)
{
	if (m_assetCache == NULL)
		m_assetCache = CreateAssetCache();

	assert(m_assetCache);

	m_assetCache->AddAsset(pAsset);
}

CAnmAsset *CAnmFileManager::LookupAssetInCache(String str)
{
	assert(str);

	if (m_assetCache == NULL)
		m_assetCache = CreateAssetCache();

	assert(m_assetCache);

	CAnmAsset *pAsset = m_assetCache->FindAsset(str->GetBuf());

	return pAsset;
}

CAnmAssetCache *CAnmFileManager::CreateAssetCache()
{
	return new CAnmAssetCache;
}

