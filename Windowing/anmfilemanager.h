/********************************************************************************
 * Flux
 *
 * File: anmfilemgr.h
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

#ifndef _anmfilemgr_h
#define _anmfilemgr_h

#include "anmcriticalsection.h"

// supporting structures
// Info for requesting a hyperlink to url
struct sAnmLinkRequest
{
	class cApplication *requestApp;
	StringArray *requestUrls;
	const char *requestBaseUrl;
	StringArray *requestParams;

	sAnmLinkRequest(class cApplication *pApp, StringArray *pUrls,
		const char *baseUrl, StringArray *pParams)
	{
		requestApp = pApp;
		requestUrls = pUrls;
		requestParams = pParams;
		requestBaseUrl = baseUrl;
	}
};

// Info for requesting an url load
typedef class CAnmAsset * (*CAnmFileLoadedCallback) (String pRequestUrl, String pCacheFileName, void *userData);
typedef void (*CAnmFileAssetLoadedCallback) (String pRequestUrl, String pCacheFileName, class CAnmAsset *pAsset, void *userData);
typedef void (*CAnmFileActiveCallback) (String pRequestUrl, String pCacheFileName, void *userData);
typedef void (*CAnmFileProgressCallback) (float progress, void *userData);

struct sAnmLoadRequest
{
	class cApplication *requestApp;
	StringArray *requestUrls;
	const char *requestBaseUrl;
	CAnmFileLoadedCallback requestLoadedCallback;
	CAnmFileAssetLoadedCallback requestAssetLoadedCallback;
	CAnmFileActiveCallback requestActiveCallback;
	CAnmFileProgressCallback requestProgressCallback;
	void *requestUserData;
	bool requestAllowStreaming;

	sAnmLoadRequest(class cApplication *pApp, StringArray *pUrls, const char *baseUrl,
		CAnmFileLoadedCallback lcb, CAnmFileAssetLoadedCallback alcb, CAnmFileActiveCallback acb,
		CAnmFileProgressCallback pcb, void *userData, bool bAllowStreaming = false )
	{
		requestApp = pApp;
		requestUrls = pUrls;
		requestBaseUrl = baseUrl;
		requestLoadedCallback = lcb;
		requestAssetLoadedCallback = alcb;
		requestActiveCallback = acb;
		requestProgressCallback = pcb;
		requestUserData = userData;
		requestAllowStreaming = bAllowStreaming;
	}
};

class EXPORT CAnmFileManager
{
protected :

	class cApplication					*m_app;
	CAnmCriticalSection					 m_criticalSection;
	std::deque<struct sAnmLoadRequest *> m_loadRequests;
	std::deque<struct sAnmLoadRequest *> m_streamLoadRequests;
	class CAnmAssetCache				*m_assetCache;

	virtual bool UrlInCache(const char *url, String *pCacheFileName);
	virtual bool LockUrlInCache(String url);
	virtual bool UnlockUrlInCache(String url);
	virtual bool UrlLoaded(const char *url, String *pCacheFileName, String *pCacheUrl);
	virtual eAnmReturnStatus LoadCachedFile(String url, const char *baseUrl,
		String *pCacheFileName, String *pCacheUrl,
		class CAnmAsset **ppAsset, sAnmLoadRequest *pRequest);
	virtual eAnmReturnStatus IsStreamableFile(String url, const char *baseUrl,
		String *pCacheFileName, String *pCacheUrl,
		class CAnmAsset **ppAsset, sAnmLoadRequest *pRequest);


	virtual void NotifyProgress(float progress, void *userData);
	friend class CAnmBindStatusCallback;

	// asset cache helpers
	void AddAssetToCache(class CAnmAsset *pAsset);
	class CAnmAssetCache *CreateAssetCache();

public :

	CAnmFileManager(class cApplication *pApp);
	~CAnmFileManager();

	eAnmReturnStatus LinkToUrl(StringArray *pUrl, const char *baseUrl, StringArray *pParam);
	eAnmReturnStatus LinkToUrl(const char *url, const char *baseUrl, const char *target);
	eAnmReturnStatus RequestLinkToUrl(StringArray *pUrls, const char *baseUrl, StringArray *pParams);
	eAnmReturnStatus ProcessLinkRequest(struct sAnmLinkRequest *pRequest);
	bool UrlLoaded(StringArray *pUrl, const char *baseUrl, String *pRequestUrl, String *pCacheFileName);
	eAnmReturnStatus RequestLoadUrl(StringArray *pUrl, const char *baseUrl,
		CAnmFileLoadedCallback lcb, CAnmFileAssetLoadedCallback alcb, CAnmFileActiveCallback acb,
		CAnmFileProgressCallback pcb, void *userData, bool bAllowStreaming = false );
	eAnmReturnStatus ProcessLoadRequest(struct sAnmLoadRequest *pRequest);
	void ProcessLoadRequests();
	bool LoadRequestsPending();
	class CAnmAsset *LookupAssetInCache(String str);

	// Thread safety
	void Lock()
	{
		m_criticalSection.Lock();
	}

	void Unlock()
	{
		m_criticalSection.Unlock();
	}
};

// this way we globally turn on/off async loading until all the kinks are worked out
#define ANMFILEMANAGER_ASYNC_LOAD 1

#define ANMMAX_PATH 2048

// Windows crap
#ifdef WIN32
#define WM_USERANMLINKTOURL			(WM_USER+1)
#define WM_USERANMCALLBROWSERFUNC	(WM_USER+5)
#define WM_USERANMRESETDEVICE		(WM_USER+6)
#endif

#endif // _anmfilemgr_h