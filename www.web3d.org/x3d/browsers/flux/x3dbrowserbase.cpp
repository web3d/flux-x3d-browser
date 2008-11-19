/********************************************************************************
 * Flux
 *
 * File: x3dbrowserbase.cpp
 * Description: Base class for COM browser interfaces
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
#include "Anima.h"
#include "x3dbrowserbase.h"

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
#include "x3dexecutioncontext.h"
#include "x3dnode.h"
#include "x3dnodearray.h"
#include "x3dscene.h"

/////////////////////////////////////////////////////////////////////////////
// X3DBrowserBase

X3DBrowserBase::~X3DBrowserBase()
{
	std::list< sAnmBrowserListenerCallback >::iterator iter;
	for( 
		iter = m_listeners.begin(); 
		iter != m_listeners.end();
		iter++ )
	{
		IDispatch *pListener = (*iter).m_listener;
		SafeRelease(pListener);
	}
}

STDMETHODIMP X3DBrowserBase::getName(BSTR *value)
{
	// TODO: Add your implementation code here
	assert(m_browser);

	CComBSTR str = m_browser->getName();
	*value = str.Copy();
	return S_OK;
}

STDMETHODIMP X3DBrowserBase::getVersion(BSTR *value)
{
	// TODO: Add your implementation code here
	CComBSTR str = m_browser->getVersion();
	*value = str.Copy();
	return S_OK;
}

STDMETHODIMP X3DBrowserBase::getCurrentSpeed(float *value)
{
	// TODO: Add your implementation code here

	*value = m_browser->getCurrentSpeed();
	return S_OK;
}

STDMETHODIMP X3DBrowserBase::getCurrentFrameRate(float *value)
{
	// TODO: Add your implementation code here

	*value = m_browser->getCurrentFrameRate();
	return S_OK;
}

STDMETHODIMP X3DBrowserBase::getWorldURL(BSTR *value)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP X3DBrowserBase::replaceWorld(X3DScene *value)
{
	CAnmSAIScene *pSAIScene = NULL;
	if (value != NULL)
	{
		HRESULT hr = value->getClosure((void **) &pSAIScene);
		if (pSAIScene == NULL)
			return E_FAIL;
	}

	m_browser->replaceWorld(pSAIScene);

	return S_OK;
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


STDMETHODIMP X3DBrowserBase::loadURL(int nUrls, BSTR *url, int nParams, BSTR *params)
{
	// TODO: Add your implementation code here
	StringArray *pStrArray = new StringArray;

	for (int i = 0; i < nUrls; i++)
	{
		BSTR bstr = url[i];

		int len = wcslen(bstr);
		if (len <= 0)
			continue;

		String s = new CAnmString;
		s->alloc(len + 1);
		wcstombs(s->GetBuf(), bstr, len + 1);
		pStrArray->push_back(s);
	}

	if (pStrArray->size() > 0)
		m_browser->loadUrl(pStrArray, urlLoadedCallback, pStrArray);

	return S_OK;
}

STDMETHODIMP X3DBrowserBase::setDescription(BSTR description)
{
	int len = wcslen(description);
	if (len <= 0)
		return E_INVALIDARG;

	char *cp = new char[len + 1];
	wcstombs(cp, description, len + 1);

	m_browser->setDescription(cp);

	delete cp;

	return S_OK;
}

STDMETHODIMP X3DBrowserBase::createX3DFromString(BSTR x3dSource, X3DScene **scenereturn)
{
	int len = wcslen(x3dSource);
	String s = new CAnmString;
	s->alloc(len + 1);
	wcstombs(s->GetBuf(), x3dSource, len + 1);

	HRESULT retval = E_FAIL;

	CAnmExecutionContext *pContext = m_browser->getExecutionContext();
	if (pContext == NULL)
		return E_FAIL;

	CAnmSAIScene *pSAIScene = m_browser->createVrmlFromString(s, pContext->GetSink());
	if (pSAIScene)
	{
		X3DScene *pX3DScene = NULL;
		HRESULT hr = CoCreateInstance(CLSID_CoX3DScene, NULL, CLSCTX_INPROC_SERVER,
			IID_X3DScene, (LPVOID*)&pX3DScene);

		if (SUCCEEDED(hr))
		{
			pX3DScene->setClosure(pSAIScene);
			*scenereturn = pX3DScene;
			retval = S_OK;
		}
		else
			retval = E_FAIL;
	}
	else
		retval = E_FAIL;

	return retval;
}

STDMETHODIMP X3DBrowserBase::createX3DFromURL(BSTR url, X3DScene **scenereturn)
{
	// create a string array to hold the URL
	int len = wcslen(url);
	String s = new CAnmString;
	s->alloc(len + 1);
	wcstombs(s->GetBuf(), url, len + 1);
	StringArray *pStrArray = new StringArray;
	pStrArray->push_back(s);


	CAnmExecutionContext *pContext = m_browser->getExecutionContext();
	if (pContext == NULL)
		return E_FAIL;

	HRESULT retval = E_FAIL;

	CAnmSAIScene *pSAIScene = m_browser->createVrmlFromUrl(pStrArray, pContext->GetSink());
	if (pSAIScene)
	{
		X3DScene *pX3DScene = NULL;
		HRESULT hr = CoCreateInstance(CLSID_CoX3DScene, NULL, CLSCTX_INPROC_SERVER,
			IID_X3DScene, (LPVOID*)&pX3DScene);

		if (SUCCEEDED(hr))
		{
			pX3DScene->setClosure(pSAIScene);
			*scenereturn = pX3DScene;
			retval = S_OK;
		}
		else
			retval = E_FAIL;
	}
	else
		retval = E_FAIL;

	// Clean up
	delete pStrArray;
	delete s;

	return retval;
}

#if 0
// N.B.: old VRML-style version
STDMETHODIMP X3DBrowserBase::createX3DFromURL(BSTR url, X3DNode *node, BSTR event)
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
				return S_OK;
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


STDMETHODIMP X3DBrowserBase::setClosure(void *closure)
{
	m_browser = (CAnmBrowser *) closure;
	m_browser->AddEventCallback(browserEventCallback, this);
	return S_OK;
}

STDMETHODIMP X3DBrowserBase::getClosure(void *closure)
{
	CAnmBrowser **ppBrowser = (CAnmBrowser **) closure;
	*ppBrowser = m_browser;
	return S_OK;
}

STDMETHODIMP X3DBrowserBase::print(BSTR str)
{
	int len = wcslen(str);
	if (len <= 0)
		return E_INVALIDARG;

	char *cp = new char[len + 1];
	wcstombs(cp, str, len + 1);

	m_browser->print(cp);

	delete cp;

	return S_OK;
}

STDMETHODIMP X3DBrowserBase::loadUrlS(BSTR url)
{
	int len = wcslen(url);
	String s = new CAnmString;
	s->alloc(len + 1);
	wcstombs(s->GetBuf(), url, len + 1);

	StringArray *pStrArray = new StringArray;
	pStrArray->push_back(s);

	m_browser->loadUrl(pStrArray, urlLoadedCallback, pStrArray);

	return S_OK;
}

STDMETHODIMP X3DBrowserBase::getExecutionContext(X3DExecutionContext **ppContext)
{
	*ppContext = NULL;

	CAnmExecutionContext *pContext = m_browser->getExecutionContext();
	if (pContext == NULL)
		return E_FAIL;

	X3DExecutionContext *pX3DContext = NULL;
	HRESULT hr = S_OK;

	if (pContext->GetClass()->IsA(GETCLASS(CAnmSAIScene)))
	{
		X3DScene *pScene = NULL;
		hr = CoCreateInstance(CLSID_CoX3DScene, NULL, CLSCTX_INPROC_SERVER,
			IID_X3DScene, (LPVOID*)&pScene);

		if (SUCCEEDED(hr))
		{
			pX3DContext = pScene;
		}
	}
	else
	{
		X3DExecutionContext *pContext = NULL;
		hr = CoCreateInstance(CLSID_CoX3DExecutionContext, NULL, CLSCTX_INPROC_SERVER,
			IID_X3DExecutionContext, (void **)&pContext);

		if (SUCCEEDED(hr))
		{
			pX3DContext = pContext;
		}
	
	}

	if (pX3DContext)
		pX3DContext->setClosure(pContext);

	*ppContext = pX3DContext;

	return hr;
}



STDMETHODIMP X3DBrowserBase::firstViewpoint()
{
	m_browser->firstViewpoint();

	return S_OK;
}

STDMETHODIMP X3DBrowserBase::lastViewpoint()
{
	m_browser->lastViewpoint();

	return S_OK;
}

STDMETHODIMP X3DBrowserBase::nextViewpoint()
{
	m_browser->nextViewpoint();

	return S_OK;
}

STDMETHODIMP X3DBrowserBase::previousViewpoint()
{
	m_browser->previousViewpoint();

	return S_OK;
}

STDMETHODIMP X3DBrowserBase::getViewpoints(X3DNodeArray **pVal)
{
	if (pVal == NULL)
		return E_INVALIDARG;

	std::vector<class CAnmSAINode *> *pSAIRootNodes = m_browser->getViewpoints();

	X3DExecutionContext *pX3DContext = NULL;

	HRESULT hr = getExecutionContext(&pX3DContext);
	
	if (SUCCEEDED(hr))
		*pVal = CX3DNodeArray::CreateX3DNodeArray(pSAIRootNodes, pX3DContext);
	else
		*pVal = NULL;

	delete pSAIRootNodes;

	if (*pVal == NULL)
		return E_FAIL;

	return S_OK;
}


STDMETHODIMP X3DBrowserBase::addBrowserListener(IDispatch *pListener)
{
	if (pListener)
	{
		m_listeners.push_back(sAnmBrowserListenerCallback(pListener));
		pListener->AddRef();
	}
	return S_OK;
}

STDMETHODIMP X3DBrowserBase::removeBrowserListener(IDispatch *pListener)
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
			pListener->Release();
		}
	}
	return S_OK;
}

STDMETHODIMP X3DBrowserBase::addBrowserListenerV(VARIANT listener)
{
	if (listener.vt == VT_DISPATCH)
	{
		IDispatch *pDisp = listener.pdispVal;

		if (pDisp)
		{
			addBrowserListener(pDisp);
		}
	}
	return S_OK;
}

void X3DBrowserBase::browserEventCallback(eAnmBrowserEvent event,
										  void *userData)
{
	X3DBrowserBase *pBrowserBase = (X3DBrowserBase *) userData;
	pBrowserBase->handleBrowserEvent(event);
}

void X3DBrowserBase::handleBrowserEvent(eAnmBrowserEvent event)
{
	cApplication *pApp = m_browser->GetApplication();

	assert(pApp);

	CAnmTime *pClock = pApp->GetClock();
	assert(pClock);

	Time timestamp = pClock->GetCurrentTime();

	sAnmThreadedBrowserCallback *pTCB = new sAnmThreadedBrowserCallback;
	pTCB->pBrowserBase = this;
	pTCB->event = event;
	pApp->PostMessage(eAppMsgCallBrowserFunction, pTCB);
}

void X3DBrowserBase::callThreadedCallbacks(eAnmBrowserEvent event)
{
	std::list< sAnmBrowserListenerCallback >::iterator iter;
	for( 
		iter = m_listeners.begin(); 
		iter != m_listeners.end();
		iter++ )
	{
		HRESULT hr;
		IDispatch *pListener = (*iter).m_listener;

		// try to use the raw COM interface if it's there
		X3DBrowserListener *pRawListener = NULL;
		hr = pListener->QueryInterface(IID_X3DBrowserListener, (void **) &pRawListener);

		if (SUCCEEDED(hr))
		{
			pRawListener->browserChanged((X3DBrowserEvent) event);

			int refcount = pRawListener->Release();
		}
		else
		{
			// Otherwise, try Dispatch method

			DISPID callbackdisp = -1;
			BSTR callbackname = SysAllocString(L"browserChanged");


			ITypeInfo *pTInfo = NULL;

			hr = pListener->GetTypeInfo( 
				0, GetUserDefaultLCID(), &pTInfo);

			if (SUCCEEDED(hr))
			{
				// got type info? let's try to dispatch that way
				FUNCDESC *pFD;
				hr = pTInfo->GetFuncDesc(0, &pFD);

				hr = pListener->GetIDsOfNames(IID_NULL, &callbackname, 1, GetUserDefaultLCID(), &callbackdisp);

				if (SUCCEEDED(hr))
				{
					// N.B.: load VARIANT args in reverse order for Invoke?
					VARIANT args[1];
					args[0].vt = VT_I4;
					args[0].intVal = event;

					VARIANT retval = { VT_EMPTY, 0 };

					DISPPARAMS params = {args, 0, 1, 0};

					hr = pListener->Invoke(callbackdisp,
						IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &params, NULL,
						NULL, NULL);
				}
			}
		}
	}
}

void X3DBrowserBase::CallX3DCallback(sAnmThreadedBrowserCallback *pTCB)
{
	if (pTCB)
	{
		pTCB->pBrowserBase->callThreadedCallbacks(pTCB->event);
		delete pTCB;
	}
}