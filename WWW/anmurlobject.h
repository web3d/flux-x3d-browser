/********************************************************************************
 * Flux
 *
 * File: anmurlobject.h
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

#ifndef _anmurlobject_h
#define _anmurlobject_h

class EXPORT CAnmUrlObject
{

protected:
	
	StringArray							 *m_url;
	String								  m_baseUrl;

	std::list<class CAnmLoadSensor *>	  m_loadSensors;

	// helpers for async file loading
	virtual void Load(class cApplication *pApp, bool bAllowStreaming = false );
	static class CAnmAsset *urlLoadedCallback(String pRequestUrl, String pCacheFileName, void *userData);
	static void urlAssetLoadedCallback(String pRequestUrl, String pCacheFileName, class CAnmAsset *pAsset, void *userData);
	virtual class CAnmAsset *urlLoadedHandler(String pRequestUrl, String pCacheFileName);
	virtual void urlLoadedHandler(String pRequestUrl, class CAnmAsset *pAsset);
	static void urlProgressCallback(float progress, void *userData);
	virtual void urlProgressHandler(float progress);

	// All subclasses must override this
	virtual class CAnmAsset *handleUrlLoaded(String pRequestUrl, String pCacheFileName) PURE;
	virtual void handleUrlLoaded(String pRequestUrl, class CAnmAsset *pAsset) PURE;

	// notifications with LoadSensor
	friend class CAnmLoadSensor;

	virtual void AddLoadSensor(class CAnmLoadSensor *pLoadSensor);
	virtual void RemoveLoadSensor(class CAnmLoadSensor *pLoadSensor);

public:

	// constructor/destructor
	CAnmUrlObject();
	virtual ~CAnmUrlObject();

	// New methods
	static void ComputeBaseUrl(const char *url, char *outBuf, int maxlen);
	static void ComputeRootedUrl(const char *url, char *outBuf, int maxlen);
	static void ComputeFullName(const char *url, const char *baseUrl, char *fullname);
	static BOOL IsRootedPath(const char *url);
	static BOOL IsRelativePath(const char *url);
	static BOOL IsWebPath(const char *fname);
	static BOOL IsNonStreamable(const char *fname);

	static BOOL DoesWebFileExist(const char *fname);

	static BOOL IsLocalFile(const char *url);
	static char *GetViewpointAnchor(String url);

	// Accessors
	virtual void SetUrl(StringArray *pUrl);
	virtual StringArray *GetUrl() { return m_url; }
	virtual void GetUrl(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_url;
	}

	virtual void SetBaseUrl(String baseUrl);
	virtual String GetBaseUrl()
	{
		return m_baseUrl;
	}

	// Interface glue
	ANMDECLARE_INTERFACE(CAnmUrlObject);
};


#endif // _anmurlobject_h
