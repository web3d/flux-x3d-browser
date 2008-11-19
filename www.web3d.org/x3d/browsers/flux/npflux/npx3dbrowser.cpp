/********************************************************************************
 * Flux
 *
 * File: npx3dbrowser.cpp
 * Description: Mozilla X3DBrowser 
 *							Scriptable Plugin Object
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
#include "npx3dbrowser.h"
#include "npx3dexecutioncontext.h"
#include "npx3dnode.h"
#include "npx3dscene.h"
 
#include "anmbrowser.h"
#include "animaapp.h"
#include "anmsymbol.h"
#include "anmroute.h"
#include "anmfileloader.h"
#include "anmproto.h"
#include "anmexecutioncontext.h"
#include "anmsainode.h"
#include "anmsaiscene.h"
#include "anmscene.h"
#include "anmworld.h"


/////////////////////////////////////////////////////////////////////////////
// CNPX3DBrowser

CNPX3DBrowser::~CNPX3DBrowser()
{
	std::list< sAnmBrowserListenerCallback >::iterator iter;
	for( 
		iter = m_listeners.begin(); 
		iter != m_listeners.end();
		iter++ )
	{
		NPObject *pListener = (*iter).m_listener;
		if (pListener)
			NPN_ReleaseObject(pListener);
	}
}

bool CNPX3DBrowser::getName(const char **value)
{
	assert(m_browser);

	*value = m_browser->getName();
	return true;
}

bool CNPX3DBrowser::getVersion(const char **value)
{
	assert(m_browser);

	*value = m_browser->getVersion();
	return true;
}

bool CNPX3DBrowser::getCurrentSpeed(float *value)
{
	assert(m_browser);

	*value = m_browser->getCurrentSpeed();
	return true;
}

bool CNPX3DBrowser::getCurrentFrameRate(float *value)
{
	assert(m_browser);

	*value = m_browser->getCurrentFrameRate();
	return true;
}

bool CNPX3DBrowser::getWorldURL(const char **value)
{
	// TODO: Add your implementation code here

	return true;
}

bool CNPX3DBrowser::replaceWorld(class CNPX3DScene *value)
{
#if 0
	CAnmSAIScene *pSAIScene = NULL;
	if (value != NULL)
	{
		HRESULT hr = value->getClosure((void **) &pSAIScene);
		if (pSAIScene == NULL)
			return false;
	}

	m_browser->replaceWorld(pSAIScene);
#endif

	return true;
}

static class CAnmAsset *urlLoadedCallback(String pRequestUrl, String pCacheFileName, void *userData)
{
	StringArray *pStrArray = (StringArray *) userData;
	
	int sz = pStrArray->size();
	
	for (int i = 0; i < sz; i++)
		delete (char *) (*pStrArray)[i]->GetBuf();

	SafeUnRef(pStrArray);

	return NULL;
}


bool CNPX3DBrowser::loadURL(int nUrls, char **url, int nParams, char **params)
{
	// TODO: Add your implementation code here
	StringArray *pStrArray = new StringArray;

	for (int i = 0; i < nUrls; i++)
	{
		char *cp = url[i];

		int len = strlen(cp);
		if (len <= 0)
			continue;

		String s = new CAnmString(cp);
		pStrArray->push_back(s);
	}

	if (pStrArray->size() > 0)
		m_browser->loadUrl(pStrArray, urlLoadedCallback, pStrArray);

	return true;
}

bool CNPX3DBrowser::setDescription(char *description)
{
	int len = strlen(description);
	if (len <= 0)
		return false;

	m_browser->setDescription(description);

	return true;
}

bool CNPX3DBrowser::createX3DFromString(char *x3dSource, class CNPX3DScene **scenereturn)
{
	int len = strlen(x3dSource);
	String s = new CAnmString(x3dSource);

	bool retval = false;

	CAnmExecutionContext *pContext = m_browser->getExecutionContext();
	if (pContext == NULL)
		return false;

	CAnmSAIScene *pSAIScene = m_browser->createVrmlFromString(s, pContext->GetSink());
	if (pSAIScene)
	{
		CNPX3DScene *pX3DScene = CNPX3DScene::Create(mNpp, pSAIScene);

		if (pX3DScene)
		{
			NPN_RetainObject(pX3DScene);

			*scenereturn = pX3DScene;

			return true;
		}
		else
			return false;
	}
	else
		retval = false;

	return retval;
}

bool CNPX3DBrowser::createX3DFromURL(char *url, class CNPX3DScene **scenereturn)
{
	// create a string array to hold the URL
	int len = strlen(url);
	if (len <= 0)
		return false;

	String s = new CAnmString(url);
	StringArray *pStrArray = new StringArray;
	pStrArray->push_back(s);

	bool retval = false;

	CAnmExecutionContext *pContext = m_browser->getExecutionContext();
	if (pContext == NULL)
		return false;

	CAnmSAIScene *pSAIScene = m_browser->createVrmlFromUrl(pStrArray, pContext->GetSink());
	if (pSAIScene)
	{
		CNPX3DScene *pX3DScene = CNPX3DScene::Create(mNpp, pSAIScene);

		if (pX3DScene)
		{
			NPN_RetainObject(pX3DScene);

			*scenereturn = pX3DScene;

			return true;
		}
		else
			return false;
	}
	else
		retval = false;

	// Clean up
	delete pStrArray;
	delete s;

	return retval;
}

bool CNPX3DBrowser::firstViewpoint()
{
	assert(m_browser);

	m_browser->firstViewpoint();

	return PR_TRUE;
}

bool CNPX3DBrowser::lastViewpoint()
{
	assert(m_browser);

	m_browser->lastViewpoint();

	return PR_TRUE;
}

bool CNPX3DBrowser::nextViewpoint()
{
	assert(m_browser);

	m_browser->nextViewpoint();

	return PR_TRUE;
}

bool CNPX3DBrowser::previousViewpoint()
{
	assert(m_browser);

	m_browser->previousViewpoint();

	return PR_TRUE;
}

bool CNPX3DBrowser::getViewpoints(NPArray **viewpoints)
{
	if (viewpoints == NULL)
		return false;

	std::vector<class CAnmSAINode *> *pSAIRootNodes = m_browser->getViewpoints();

	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	*viewpoints = pArray;

	int sz = pSAIRootNodes->size();

	if (sz > 0)
	{
		CNPX3DExecutionContext *pContext = NULL;
		if (getExecutionContext(&pContext))
		{
			for (int i = 0; i < sz; i++)
			{
				CAnmSAINode *pSAINode = (*pSAIRootNodes)[i];

				CNPX3DNode *pX3DNode = CNPX3DNode::Create(mNpp, pSAINode, pContext);

				NPN_RetainObject(pX3DNode);

				NPVariant v;

				OBJECT_TO_NPVARIANT(pX3DNode, v);

				pArray->set1Value(i, v);
			}
		}
	}

	delete pSAIRootNodes;

	return true;

}


#if 0
// N.B.: old VRML-style version
bool CNPX3DBrowser::createX3DFromURL(char *url, X3DNode *node, char *event)
{
	if (node)
	{
		CAnmSAINode *pSAINode = NULL;
		HRESULT hr = node->getNativeNode(&pSAINode);

		if (SUCCEEDED(hr))
		{
			CAnmNode *pNode = pSAINode->GetNode();
			if (pNode)
			{
				int len = wcslen(url);
				String s = new CAnmString;
				s->alloc(len + 1);
				wcstombs(s->GetBuf(), url, len + 1);
				StringArray *pStrArray = new StringArray;
				pStrArray->push_back(s);

				// N.B.: length
				char methodname[ANMMAXSTRBUF];

				wcstombs(methodname, event, wcslen(event) + 1);

				m_browser->createVrmlFromUrl(pStrArray, pNode, methodname,
					urlLoadedCallback, pStrArray);
				return true;
			}
			else
				return E_INVALIDARG;
		}
		else
			return E_INVALIDARG;
	}

	return E_INVALIDARG;
}
#endif // 0


bool CNPX3DBrowser::setClosure(void *closure)
{
	m_browser = (CAnmBrowser *) closure;
	m_browser->AddEventCallback(browserEventCallback, this);
	return true;
}

bool CNPX3DBrowser::getClosure(void *closure)
{
	CAnmBrowser **ppBrowser = (CAnmBrowser **) closure;
	*ppBrowser = m_browser;
	return true;
}

bool CNPX3DBrowser::print(const char *str)
{
	int len = strlen(str);
	if (len <= 0)
		return false;

	m_browser->print((char *) str);

	return true;
}

bool CNPX3DBrowser::loadUrlS(const char *url)
{
	int len = strlen(url);
	if (len <= 0)
		return false;

	String s = new CAnmString(url);
	StringArray *pStrArray = new StringArray;
	pStrArray->push_back(s);

	m_browser->loadUrl(pStrArray, urlLoadedCallback, pStrArray);

	return true;
}

bool CNPX3DBrowser::getExecutionContext(CNPX3DExecutionContext **ppContext)
{
	*ppContext = NULL;

	CAnmExecutionContext *pContext = m_browser->getExecutionContext();
	if (pContext == NULL)
		return false;

	if (pContext->GetClass()->IsA(GETCLASS(CAnmSAIScene)))
	{
		CNPX3DScene *pX3DScene = CNPX3DScene::Create(mNpp, pContext);

		if (pX3DScene)
		{
			NPN_RetainObject(pX3DScene);

			*ppContext = pX3DScene;

			return true;
		}
		else
			return false;
	}
	else
	{
		CNPX3DExecutionContext *pX3DContext = CNPX3DExecutionContext::Create(mNpp, pContext);

		if (pX3DContext)
		{
			NPN_RetainObject(pX3DContext);

			*ppContext = pX3DContext;

			return true;
		}
		else
			return false;
	}

	return false;
}

bool CNPX3DBrowser::addBrowserListener(NPObject *pListener)
{
	if (pListener)
	{
		m_listeners.push_back(sAnmBrowserListenerCallback(pListener));
		NPN_RetainObject(pListener);
	}
	return true;
}

bool CNPX3DBrowser::removeBrowserListener(NPObject *pListener)
{
	if (pListener)
	{
		BOOL found = FALSE;

		std::list< sAnmBrowserListenerCallback >::iterator iter;
		for( 
			iter = m_listeners.begin(); 
			iter != m_listeners.end();
			iter++ )
		{
			if ((*iter).m_listener == pListener)
			{
				found = TRUE;
				break;
			}

		}

		if (found)
		{
			m_listeners.erase(iter);
			NPN_ReleaseObject(pListener);
		}
	}
	return true;
}

void CNPX3DBrowser::browserEventCallback(eAnmBrowserEvent event,
										  void *userData)
{
	CNPX3DBrowser *pBrowserBase = (CNPX3DBrowser *) userData;
	pBrowserBase->handleBrowserEvent(event);
}

void CNPX3DBrowser::handleBrowserEvent(eAnmBrowserEvent event)
{
	cApplication *pApp = m_browser->GetApplication();

	assert(pApp);

	CAnmTime *pClock = pApp->GetClock();
	assert(pClock);

	Time timestamp = pClock->GetCurrentTime();

	sAnmThreadedBrowserCallback *pTCB = new sAnmThreadedBrowserCallback;
	pTCB->pBrowser = this;
	pTCB->event = event;
	pApp->PostMessage(eAppMsgCallBrowserFunction, pTCB);
}

static NPIdentifier sBrowserChanged = 0;

void CNPX3DBrowser::callThreadedCallbacks(eAnmBrowserEvent event)
{
	std::list< sAnmBrowserListenerCallback >::iterator iter;
	for( 
		iter = m_listeners.begin(); 
		iter != m_listeners.end();
		iter++ )
	{
		NPObject *pObserver = (*iter).m_listener;

		if (!sBrowserChanged)
			sBrowserChanged = NPN_GetStringIdentifier("browserChanged");

		bool stat = NPN_HasMethod(mNpp, pObserver,sBrowserChanged);

		if (stat)
		{
			NPVariant args[1];
			INT32_TO_NPVARIANT(event, args[0]);
			NPVariant result;

			NPN_RetainObject(pObserver);

			stat = NPN_Invoke(mNpp,pObserver,sBrowserChanged, args, 1, &result);
			NPN_ReleaseVariantValue(&result);
		}
	}
}

void CNPX3DBrowser::CallX3DCallback(sAnmThreadedBrowserCallback *pTCB)
{
	if (pTCB)
	{
		pTCB->pBrowser->callThreadedCallbacks(pTCB->event);
		delete pTCB;
	}
}


// NP Glue
DEFINE_NPMETHODID(CNPX3DBrowser,getName);
DEFINE_NPMETHODID(CNPX3DBrowser,getVersion);
DEFINE_NPMETHODID(CNPX3DBrowser,getExecutionContext);
DEFINE_NPMETHODID(CNPX3DBrowser,addBrowserListener);
DEFINE_NPMETHODID(CNPX3DBrowser,loadUrlS);
DEFINE_NPMETHODID(CNPX3DBrowser,createX3DFromString);
DEFINE_NPMETHODID(CNPX3DBrowser,createX3DFromURL);
DEFINE_NPMETHODID(CNPX3DBrowser,firstViewpoint);
DEFINE_NPMETHODID(CNPX3DBrowser,lastViewpoint);
DEFINE_NPMETHODID(CNPX3DBrowser,nextViewpoint);
DEFINE_NPMETHODID(CNPX3DBrowser,previousViewpoint);

DEFINE_NPPROPID(CNPX3DBrowser,viewpoints);

bool CNPX3DBrowser::m_classInitialized = false;

static NPObject *
AllocateCNPX3DBrowser(NPP npp, NPClass *aClass)
{
  return new CNPX3DBrowser(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DBrowser,
                                 AllocateCNPX3DBrowser);

CNPX3DBrowser *CNPX3DBrowser::Create(NPP npp)
{
	return (CNPX3DBrowser *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DBrowser));
}


void CNPX3DBrowser::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPMETHODID(getName);
	INIT_NPMETHODID(getVersion);
	INIT_NPMETHODID(getExecutionContext);
	INIT_NPMETHODID(addBrowserListener);
	INIT_NPMETHODID(loadUrlS);
	INIT_NPMETHODID(createX3DFromString);
	INIT_NPMETHODID(createX3DFromURL);
	INIT_NPMETHODID(firstViewpoint);
	INIT_NPMETHODID(lastViewpoint);
	INIT_NPMETHODID(nextViewpoint);
	INIT_NPMETHODID(previousViewpoint);

	INIT_NPPROPID(viewpoints);

	m_classInitialized = true;
}

bool
CNPX3DBrowser::HasMethod(NPIdentifier name)
{
  return 
	  TEST_NPMETHODID(name, getName) ||
	  TEST_NPMETHODID(name, getVersion) ||
	  TEST_NPMETHODID(name, getExecutionContext) ||
	  TEST_NPMETHODID(name, addBrowserListener) ||
	  TEST_NPMETHODID(name, loadUrlS) ||
	  TEST_NPMETHODID(name, createX3DFromString) ||
	  TEST_NPMETHODID(name, createX3DFromURL) ||
	  TEST_NPMETHODID(name, firstViewpoint) ||
	  TEST_NPMETHODID(name, lastViewpoint) ||
	  TEST_NPMETHODID(name, nextViewpoint) ||
	  TEST_NPMETHODID(name, previousViewpoint) ||
		false	  
	  ;

}

bool
CNPX3DBrowser::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	const char *cpret = NULL;
	String s0 = NULL;
	bool retval = false;
	CNPX3DExecutionContext *pExecutionContext = NULL;
	CNPX3DScene *pScene = NULL;

	if (TEST_NPMETHODID(name, getName)) {

		if (getName(&cpret))
		{
			STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
		}

		return PR_TRUE;
	}
	else if (TEST_NPMETHODID(name, getVersion)) {
		if (getVersion(&cpret))
		{
			STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
		}

		return PR_TRUE;
	}
	else if (TEST_NPMETHODID(name, getExecutionContext)) {
		if (getExecutionContext(&pExecutionContext))
		{
			OBJECT_TO_NPVARIANT(pExecutionContext, *result);
		}

		return PR_TRUE;
	}
	else if (TEST_NPMETHODID(name, addBrowserListener)) {

		if (argCount > 0)
		{
			NPObject *pObj = NPVToObject(args[0]);
			if (pObj)
				return (addBrowserListener(pObj));
			else
				return PR_FALSE;
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPMETHODID(name, loadUrlS)) {

		if (argCount > 0)
		{
			s0 = NPVToString(args[0]);

			if (s0)
			{
				retval = loadUrlS(s0->GetBuf());
				SafeUnRef(s0);

				return retval;
			}
			else
				return PR_FALSE;
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPMETHODID(name, createX3DFromString)) {

		if (argCount > 0)
		{
			s0 = NPVToString(args[0]);

			if (s0)
			{
				retval = createX3DFromString(s0->GetBuf(), &pScene);

				if (retval)
					OBJECT_TO_NPVARIANT(pScene, *result);

				SafeUnRef(s0);

				return retval;
			}
			else
				return PR_FALSE;
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPMETHODID(name, createX3DFromURL)) {

		if (argCount > 0)
		{
			s0 = NPVToString(args[0]);

			if (s0)
			{
				retval = createX3DFromURL(s0->GetBuf(), &pScene);

				if (retval)
					OBJECT_TO_NPVARIANT(pScene, *result);

				SafeUnRef(s0);

				return retval;
			}
			else
			{
				NPObject *pObj = NPVToObject(args[0]);
				if (pObj)
				{
					// N.B.: try this later
					return PR_FALSE;
				}
				else
					return PR_FALSE;
			}
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPMETHODID(name, firstViewpoint)) {

		return firstViewpoint();
	}
	else if (TEST_NPMETHODID(name, lastViewpoint)) {

		return lastViewpoint();
	}
	else if (TEST_NPMETHODID(name, nextViewpoint)) {

		return nextViewpoint();
	}
	else if (TEST_NPMETHODID(name, previousViewpoint)) {

		return previousViewpoint();
	}

  return PR_FALSE;
}

bool
CNPX3DBrowser::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  STRINGZ_TO_NPVARIANT(NPstrdup("[X3DBrowser]"), *result);

  return PR_TRUE;
}

bool
CNPX3DBrowser::HasProperty(NPIdentifier name)
{
  return 
	  TEST_NPPROPID(name, viewpoints) ||
	  false;
}

bool
CNPX3DBrowser::GetProperty(NPIdentifier name, NPVariant *result)
{
	  if (TEST_NPPROPID(name, viewpoints))
	  {
		NPArray *pArray = NULL;

		if (getViewpoints(&pArray))
		{
			OBJECT_TO_NPVARIANT(pArray, *result);
			return PR_TRUE;
		}
		else
			return PR_FALSE;

	  }

	  return PR_TRUE;
}

bool
CNPX3DBrowser::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return false;
}
