/********************************************************************************
 * Flux
 *
 * File: anmbrowser.h
 * Description: Browser object (for SAI)
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

#ifndef _anmbrowser_h
#define _anmbrowser_h

#include "anmobject.h"
#include "anmfilemanager.h"

typedef enum eAnmBrowserEvent {
	eAnmBrowserInitialized			= 0,
	eAnmBrowserShutdown				= 1,
	eAnmBrowserConnectionError		= 2,
	eAnmBrowserUrlError				= 3,
} eAnmBrowserEvent ;

typedef void (*CAnmBrowserEventCallback) (eAnmBrowserEvent event, void *userData);

typedef struct sAnmBrowserEventCallback {
	CAnmBrowserEventCallback eventCallback;
	void *userData;

	operator ==(const sAnmBrowserEventCallback &cb)
	{
		return cb.eventCallback == eventCallback &&
			cb.userData == userData;
	}

} sAnmBrowserEventCallback ;

typedef enum eAnmBrowserNavAxis {
	eAnmBrowserXAxis			= 0,
	eAnmBrowserYAxis			= 1,
	eAnmBrowserZAxis			= 2,
} eAnmBrowserNavAxis ;

class EXPORT CAnmBrowser : public CAnmObject
{
protected :

	class cApplication					*m_app;
	const char							*m_baseUrl;
	class CAnmScript					*m_containingNode;
	std::list<sAnmBrowserEventCallback>
										 m_eventcallbacks;

	virtual void loadUrl(StringArray *pUrl, class CAnmNode *pNode, char *eventname, class CAnmScene **ppScene,
		bool replaceworld, CAnmFileLoadedCallback usercb, void *usercbdata);
	virtual void handleUrlLoaded(String pRequestUrl, String pCacheFileName, char *baseUrl,
		class CAnmNode *pNode, CLASSMEMBERID mid, class CAnmScene **ppScene,
		bool replaceworld, CAnmFileLoadedCallback usercb, void *usercbdata);
	static class CAnmAsset *urlLoadedCallback(String pRequestUrl, String pCacheFileName, void *userData);

	virtual class CAnmProto *getProtoContext();

	virtual void CallEventCallbacks(eAnmBrowserEvent event);

public :

	// Constructor/destructor
	CAnmBrowser(class cApplication *pApp);
	virtual ~CAnmBrowser();


	// New methods
	virtual void createVrmlFromUrl(StringArray *pUrl, class CAnmNode *pNode, char *eventname,
		CAnmFileLoadedCallback usercb, void *usercbdata);
	virtual class CAnmSAIScene *createVrmlFromUrl(StringArray *pUrl, class CAnmSAINodeEventSink *pSink);
	virtual class CAnmSAIScene *createVrmlFromString(String s, class CAnmSAINodeEventSink *pSink);
	virtual void loadUrl(StringArray *pUrl, CAnmFileLoadedCallback usercb, void *usercbdata);
	virtual void replaceWorld(class CAnmSAIScene *pSAIScene);
	virtual const char *getName();
	virtual const char *getVersion();
	virtual float getCurrentFrameRate();
	virtual float getCurrentSpeed();

	virtual void setDescription(char *description);
	virtual void print(char *str);

	virtual class CAnmNode *getNode(char *nodename);
	virtual class CAnmNode *createNode(char *nodetypename);

	virtual class CAnmRoute *addRoute(CAnmNode *pFromNode, char *fromField, CAnmNode *pToNode, char *toField);
	virtual void deleteRoute(CAnmNode *pFromNode, char *fromField, CAnmNode *pToNode, char *toField);

	virtual class CAnmExecutionContext *getExecutionContext();

	virtual void moveView(eAnmBrowserNavAxis axis, int direction);
	virtual void rotateView(eAnmBrowserNavAxis axis, int direction);
	virtual void orbitView(eAnmBrowserNavAxis axis, int direction);

	virtual void firstViewpoint();
	virtual void lastViewpoint();
	virtual void nextViewpoint();
	virtual void previousViewpoint();
	virtual std::vector<class CAnmSAINode *> *getViewpoints();

	virtual void showConsole();

	virtual void AddEventCallback(CAnmBrowserEventCallback ecb, void *userdata);
	virtual void RemoveEventCallback(CAnmBrowserEventCallback ecb, void *userdata);

	virtual void HandleWorldLoaded();

	// Accessors
	virtual void SetBaseUrl(const char *baseUrl);
	virtual const char *GetBaseUrl();

	virtual class cApplication *GetApplication()
	{
		return m_app;
	}

	virtual void SetContainingNode(class CAnmScript *pContainingNode);

	virtual class CAnmScript *GetContainingNode()
	{
		return m_containingNode;
	}

	DECLARE_CLASS(CAnmBrowser);

};

#endif // _anmbrowser_h