/********************************************************************************
 * Flux
 *
 * File: anmbrowser.cpp
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

#include "stdafx.h"
#include "anmbrowser.h"
#include "anmevent.h"
#include "anmexecutioncontext.h"
#include "anmfilemanager.h"
#include "anmfileloader.h"
#include "anmnavigationinfo.h"
#include "anmnode.h"
#include "anmproto.h"
#include "anmroute.h"
#include "anmsainode.h"
#include "anmsaiscene.h"
#include "anmscene.h"
#include "anmscript.h"
#include "anmsymbol.h"
#include "anmurlobject.h"
#include "anmviewer.h"
#include "anmviewpoint.h"
#include "anmworld.h"

CAnmBrowser::CAnmBrowser(cApplication *pApp)
: m_app(pApp),
m_baseUrl("")
{
	m_containingNode = NULL;
}

CAnmBrowser::~CAnmBrowser()
{
	SafeUnRef(m_containingNode);
}

void CAnmBrowser::SetBaseUrl(const char *baseUrl)
{
	m_baseUrl = baseUrl;
}

const char *CAnmBrowser::GetBaseUrl()
{
	return m_baseUrl;
}

void CAnmBrowser::SetContainingNode(CAnmScript *pContainingNode)
{
	SafeUnRef(m_containingNode);
	m_containingNode = pContainingNode;
	m_containingNode->Ref();
}

void CAnmBrowser::createVrmlFromUrl(StringArray *pUrl, CAnmNode *pNode, char *eventname,
									CAnmFileLoadedCallback usercb, void *usercbdata)
{
	loadUrl(pUrl, pNode, eventname, NULL, false, usercb, usercbdata);
}

static void finishNode(CAnmNode *pNode, void *data)
{
	char *baseUrl = (char *) data;
	if (baseUrl)
		pNode->SetBaseUrl(new CAnmString((const char *) baseUrl));

	// N.B.: each node should already have at least two refs from this load; ->
	// one from the node creation and another as a child of the loaded scene proto. ->
	// so take one ref off, otherwise it will never get cleaned up -- TP 1/23/03
	// Or maybe not? -- TP 3/24/03
	// pNode->UnRef();
}

#define BADSCENEPOINTER ((CAnmScene *) 0xFFFFFFFF)

CAnmSAIScene *CAnmBrowser::createVrmlFromUrl(StringArray *pUrl, CAnmSAINodeEventSink *pSink)
{
	CAnmScene *pScene = NULL;

	loadUrl(pUrl, NULL, NULL, &pScene, false, NULL, NULL);

	// Poll our scene; when it's non-NULL we're done
	bool loaded = false;
	while (!loaded)
	{
		Lock();
		loaded = pScene != NULL;
		Unlock();
	}

	if (pScene != BADSCENEPOINTER)
	{
		return new CAnmSAIScene(m_app, pScene, pSink);
	}
	else
		return NULL;
}


CAnmSAIScene *CAnmBrowser::createVrmlFromString(String s, CAnmSAINodeEventSink *pSink)
{
	CAnmScene *pScene = NULL;

	CAnmSymbolTable *pNodeSyms = NULL;
	NodeArray *pNodesReturn = NULL;
	CAnmProto *pProto = NULL;

	CAnmWorld *pWorld = m_app->GetWorld();
	assert(pWorld);

	String baseUrlString = new CAnmString(m_baseUrl);

	CAnmScene *pNewScene = CAnmFileLoader::CreateScene(s,
		baseUrlString, m_app);
	
	baseUrlString->UnRef();

	if (pNewScene)
	{
		// Finish initialization
		NodeArray *pChildren = NULL;
		pNewScene->GetChildren(&pChildren);

		pChildren->Map(finishNode, (char *) m_baseUrl);

		return new CAnmSAIScene(m_app, pNewScene, pSink);
	}
	else
	{
		return NULL;
	}
}

void CAnmBrowser::loadUrl(StringArray *pUrl, CAnmFileLoadedCallback usercb, void *usercbdata)
{
	loadUrl(pUrl, NULL, NULL, NULL, true, usercb, usercbdata);
}

void CAnmBrowser::replaceWorld(CAnmSAIScene *pSAIScene)
{
	CAnmScene *pScene = NULL;
	
	if (pSAIScene)
		pScene = pSAIScene->GetScene();

	m_app->ReplaceWorld(pScene);
}

typedef struct sAnmBrowserLoadUrlRequest {

	CAnmBrowser *pBrowser;
	char *baseUrl;
	bool replaceworld;
	CAnmNode *pNode;
	CLASSMEMBERID mid;
	CAnmScene **ppScene;
	CAnmFileLoadedCallback usercb;
	void *usercbdata;
} sAnmBrowserLoadUrlRequest;


#define MAXBUFLEN 2048

void CAnmBrowser::loadUrl(StringArray *pUrl, CAnmNode *pNode, char *eventname, CAnmScene **ppScene,
						  bool replaceworld, CAnmFileLoadedCallback usercb, void *usercbdata)
{
	CLASSMEMBERID mid = NULL;
	if (pNode)
	{
		// Get the node's event from the name
		CAnmClass *pClass = pNode->GetClass();
		assert(pClass);

		char eventbuf[MAXBUFLEN];		// N.B.: length
		mid = pClass->FindMethod(eventname);
		if (mid == NULL)
		{
			// try automatic set_ thing
			sprintf(eventbuf, "set_%s", eventname);
			mid = pClass->FindMethod(eventbuf);
		}

		if (mid == NULL)
			// N.B.: should probably throw something
			return;
	}

	// Set up base Url for when children are loaded
	char fullnamebuf[MAXBUFLEN];
	char *fullname = NULL;

	if (m_baseUrl && strlen(m_baseUrl))
	{

		if (CAnmUrlObject::IsRootedPath((*pUrl)[0]->GetBuf()) &&
			CAnmUrlObject::IsWebPath( m_baseUrl ) ) {
			char RootUrl[ANMMAXSTRBUF];
			CAnmUrlObject::ComputeRootedUrl( m_baseUrl, RootUrl, ANMMAXSTRBUF );
			strcpy(fullnamebuf, RootUrl);
			strcat(fullnamebuf, (*pUrl)[0]->GetBuf());
		}
		else if (CAnmUrlObject::IsRelativePath((*pUrl)[0]->GetBuf()))
		{
			strcpy(fullnamebuf, m_baseUrl);
			strcat(fullnamebuf, (*pUrl)[0]->GetBuf());
		}
		else
			strcpy(fullnamebuf, (*pUrl)[0]->GetBuf());
	
		fullname = fullnamebuf;
	}
	else
		fullname = (char *) (*pUrl)[0]->GetBuf();

	if (pUrl->size() > 0 && (*pUrl)[0]->length() > 0)
	{
		sAnmBrowserLoadUrlRequest *pBrowserRequest = new sAnmBrowserLoadUrlRequest;
		pBrowserRequest->pBrowser = this;
		// N.B.: need to delete this, but after callback is processed
		pBrowserRequest->baseUrl = new char[MAXBUFLEN];
		CAnmUrlObject::ComputeBaseUrl(fullname, pBrowserRequest->baseUrl, MAXBUFLEN);
		pBrowserRequest->replaceworld = replaceworld;
		pBrowserRequest->pNode = pNode;
		// N.B.: need to delete this somehow, but after event is consumed. put on list:
		pBrowserRequest->mid = mid;
		pBrowserRequest->ppScene = ppScene;
		pBrowserRequest->usercb = usercb;
		pBrowserRequest->usercbdata = usercbdata;
		m_app->GetFileManager()->RequestLoadUrl(pUrl, m_baseUrl, urlLoadedCallback, NULL,
			NULL, NULL, pBrowserRequest);
	}
}


class CAnmAsset *CAnmBrowser::urlLoadedCallback(String pRequestUrl, String pCacheFileName, void *userData)
{
	sAnmBrowserLoadUrlRequest *pBrowserRequest = (sAnmBrowserLoadUrlRequest *) userData;

	assert(pBrowserRequest != NULL);
	assert(pBrowserRequest->pBrowser != NULL);
	pBrowserRequest->pBrowser->handleUrlLoaded(pRequestUrl, pCacheFileName, pBrowserRequest->baseUrl,
		pBrowserRequest->pNode, pBrowserRequest->mid, pBrowserRequest->ppScene,
		pBrowserRequest->replaceworld, pBrowserRequest->usercb, pBrowserRequest->usercbdata);

	delete pBrowserRequest;

	return NULL;
}

void CAnmBrowser::handleUrlLoaded(String pRequestUrl, String pCacheFileName, char *baseUrl, 
								  CAnmNode *pNode, CLASSMEMBERID mid, class CAnmScene **ppScene,
								  bool replaceworld, CAnmFileLoadedCallback usercb, void *usercbdata)
{
	CAnmSymbolTable *pNodeSyms = NULL;
	NodeArray *pNodesReturn = NULL;
	CAnmProto *pProto = NULL;

	CAnmWorld *pWorld = m_app->GetWorld();
	assert(pWorld);

	String baseUrlString = new CAnmString(baseUrl);

	CAnmScene *pNewScene = CAnmFileLoader::CreateScene(pCacheFileName->GetBuf(),
		baseUrlString, m_app);
	
	baseUrlString->UnRef();

	if (pNewScene)
	{
		// Finish initialization
		NodeArray *pChildren = NULL;
		pNewScene->GetChildren(&pChildren);

		pChildren->Map(finishNode, baseUrl);

		if (replaceworld)
		{
			m_app->ReplaceWorld(pNewScene);
		}
		else if (ppScene)
		{
			Lock();
			*ppScene = pNewScene;
			Unlock();
		}
		else
		{
			pWorld->Lock();

			// N.B.: this needs to get reworked 
			m_app->AddEventReceiver(pNode);

			// Clear out this scene; children will be added via CallMethod below
			pNewScene->RemoveAllChildren();

			// call the method straight away; it's funky with deadlocks if we post an event instead
			pNode->CallMethod(mid, &pChildren);

			// N.B.: this is just to update the polycount, duh
			pWorld->SetStateDirty();
			
			pWorld->Unlock();

			// N.B.: can we get rid of this scene?!
		}
	}
	else
	{
		if (ppScene)
			*ppScene = BADSCENEPOINTER;
		return;
	}

	if (usercb)
		(*usercb)(pRequestUrl, pCacheFileName, usercbdata);

}

// N.B.: string resources
const char *CAnmBrowser::getName()
{
	return "Flux Player";
}

const char *CAnmBrowser::getVersion()
{
	return "Version 2.0";
}

float CAnmBrowser::getCurrentFrameRate()
{
	return m_app->GetFrameRate();
}

float CAnmBrowser::getCurrentSpeed()
{
	CAnmWorld *pWorld = m_app->GetWorld();

	// get defined speed of World's currently bound navigation info
	if (pWorld)
	{
		CAnmNavigationInfo *pNavInfo = pWorld->GetNavigationInfo();
		if (pNavInfo)
		{
			return pNavInfo->GetSpeed();
		}
	}

	// unknown
	return 0.f;
}

void CAnmBrowser::setDescription(char *description)
{
	assert(m_app);

	m_app->SetStatusText(description);
}

void CAnmBrowser::print(char *str)
{
	assert(m_app);

	m_app->WriteToConsole(str);
}

CAnmNode *CAnmBrowser::getNode(char *nodename)
{
	CAnmWorld *pWorld = m_app->GetWorld();

	if (pWorld == NULL)
		return NULL;

	CAnmScene *pScene = pWorld->GetScene();

	if (pScene)
		return pScene->GetNode(nodename);
	else
		return NULL;
}

	
CAnmNode *CAnmBrowser::createNode(char *nodetypename)
{
	CAnmSymbolTable *pClassSymbols = CAnmFileLoader::GetClassSymbols();

	if (pClassSymbols == NULL)
		return NULL;

	CAnmClassSymbol *pClassSymbol = (CAnmClassSymbol *) pClassSymbols->FindSymbol(nodetypename);

	if (pClassSymbol == NULL)
		return NULL;

	CAnmClass *pClass = pClassSymbol->GetClass();
	assert(pClass);

	CAnmNode *pNode = (CAnmNode *) (*pClass->GetCreateObject()) ();

	return pNode;
}

CAnmRoute *CAnmBrowser::addRoute(CAnmNode *pFromNode, char *fromField, CAnmNode *pToNode, char *toField)
{
	CAnmProto *pProto = getProtoContext();
	assert(pProto);

	return pProto->AddRoute(m_app, pFromNode, fromField, pToNode, toField);
}

void CAnmBrowser::deleteRoute(CAnmNode *pFromNode, char *fromField, CAnmNode *pToNode, char *toField)
{
	// All good; we found the nodes and their fields. Wire them up
	CAnmProto *pProto = getProtoContext();
	assert(pProto);

	pProto->RemoveRoute(pFromNode, fromField, pToNode, toField);
}

CAnmProto *CAnmBrowser::getProtoContext()
{
	assert(m_containingNode);

	return m_containingNode->GetProtoContext();
}

CAnmExecutionContext *CAnmBrowser::getExecutionContext()
{
	CAnmProto *pProto = getProtoContext();

	assert(pProto);

	if (pProto->GetClass()->IsA(GETCLASS(CAnmScene)))
	{
		CAnmScene *pScene = (CAnmScene *) pProto;
		return new CAnmSAIScene(m_app, pScene, m_containingNode);
	}
	else
		return new CAnmExecutionContext(m_app, pProto, m_containingNode);
}

void CAnmBrowser::moveView(eAnmBrowserNavAxis axis, int direction)
{
	CAnmViewer *pViewer = m_app->GetViewer();

	pViewer->Move((eAnmViewerNavAxis) axis, direction);
}

void CAnmBrowser::rotateView(eAnmBrowserNavAxis axis, int direction)
{
	CAnmViewer *pViewer = m_app->GetViewer();

	pViewer->Rotate((eAnmViewerNavAxis) axis, direction);
}

void CAnmBrowser::orbitView(eAnmBrowserNavAxis axis, int direction)
{
	CAnmViewer *pViewer = m_app->GetViewer();

	pViewer->Orbit((eAnmViewerNavAxis) axis, direction);
}

void CAnmBrowser::firstViewpoint()
{
	CAnmWorld *pWorld = m_app->GetWorld();

	if (pWorld)
	{
		pWorld->FirstViewpoint();
	}
}

void CAnmBrowser::lastViewpoint()
{
	CAnmWorld *pWorld = m_app->GetWorld();

	if (pWorld)
	{
		pWorld->LastViewpoint();
	}
}

void CAnmBrowser::nextViewpoint()
{
	CAnmWorld *pWorld = m_app->GetWorld();

	if (pWorld)
	{
		pWorld->NextViewpoint();
	}
}

void CAnmBrowser::previousViewpoint()
{
	CAnmWorld *pWorld = m_app->GetWorld();

	if (pWorld)
	{
		pWorld->PreviousViewpoint();
	}
}

std::vector<class CAnmSAINode *> *CAnmBrowser::getViewpoints()
{
	CAnmWorld *pWorld = m_app->GetWorld();

	if (pWorld)
	{
		std::list<class CAnmViewpoint *> *pViewpoints = pWorld->GetViewpoints();
		std::vector<class CAnmSAINode *> *pSAINodes = new
			std::vector<class CAnmSAINode *> ;

		int sz = pViewpoints->size();

		if (sz > 0)
		{
			std::list<class CAnmViewpoint *>::iterator iter;

			int i;

			for (i = 0, iter = pViewpoints->begin(); i < sz; i++, iter++)
			{
				CAnmViewpoint *pViewpoint = *iter;

				CAnmSAINode *pSAINode = new CAnmSAINode(pViewpoint);
				pSAINode->SetSink(m_containingNode);
				pSAINodes->push_back(pSAINode);
			}
		}

		return pSAINodes;
	}

	return NULL;
}

void CAnmBrowser::showConsole()
{
	m_app->ShowConsole();
}

void CAnmBrowser::AddEventCallback(CAnmBrowserEventCallback cb, void *userdata)
{
	sAnmBrowserEventCallback ecb;
	ecb.eventCallback = cb;
	ecb.userData = userdata;
	m_eventcallbacks.push_back(ecb);
}

void CAnmBrowser::RemoveEventCallback(CAnmBrowserEventCallback cb, void *userdata)
{
	sAnmBrowserEventCallback ecb;
	ecb.eventCallback = cb;
	ecb.userData = userdata;
	m_eventcallbacks.remove(ecb);
}

void CAnmBrowser::CallEventCallbacks(eAnmBrowserEvent event)
{
	std::list<sAnmBrowserEventCallback>::iterator iter;

	for (iter = m_eventcallbacks.begin();
			iter != m_eventcallbacks.end();
			iter++)
		{
			sAnmBrowserEventCallback ecb = *iter;
			CAnmBrowserEventCallback cb = ecb.eventCallback;
			(*cb) (event, ecb.userData);
		}
}

void CAnmBrowser::HandleWorldLoaded()
{
	CallEventCallbacks(eAnmBrowserInitialized);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_ABSTRACT_EMPTY_CLASS(CAnmBrowser, CAnmObject)
