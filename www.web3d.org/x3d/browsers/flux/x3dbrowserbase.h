/********************************************************************************
 * Flux
 *
 * File: x3dbrowserbase.h
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

#ifndef _x3dbrowserbase_h
#define _x3dbrowserbase_h

#include "anmbrowser.h"

struct sAnmBrowserListenerCallback {

	IDispatch *m_listener;

	sAnmBrowserListenerCallback(IDispatch *pListener)
	{
		m_listener = pListener;
	}
};

struct sAnmThreadedBrowserCallback {
	class X3DBrowserBase *pBrowserBase;
	X3DBrowser *pBrowser;
	eAnmBrowserEvent event;
};

class X3DBrowserBase
{
protected :

	class CAnmBrowser	 *m_browser;
	interface X3DBrowser *m_x3dbrowser;
	std::list<sAnmBrowserListenerCallback> m_listeners;

	void SetX3DBrowser(interface X3DBrowser *pX3DBrowser)
	{
		m_x3dbrowser = pX3DBrowser;
	}

	static void browserEventCallback(eAnmBrowserEvent event, void *userData);
	void handleBrowserEvent(eAnmBrowserEvent event);
	void callThreadedCallbacks(eAnmBrowserEvent event);

public :

	X3DBrowserBase()
	{
		m_browser = NULL;
	}

	~X3DBrowserBase();

	STDMETHOD(getViewpoints)(/*[out,retval]*/ X3DNodeArray **pVal);
	STDMETHOD(previousViewpoint)();
	STDMETHOD(nextViewpoint)();
	STDMETHOD(lastViewpoint)();
	STDMETHOD(firstViewpoint)();

	// old-style VRML; keep for reference
	// STDMETHOD(createX3DFromURL)(/*[in]*/ BSTR url, /*[in]*/ X3DNode *node, /*[in]*/ BSTR event);
	STDMETHOD(createX3DFromURL)(/*[in]*/ BSTR url, /*[out, retval]*/ X3DScene **scenereturn);
	STDMETHOD(createX3DFromString)(/*[in]*/ BSTR x3dSource, /*[out, retval]*/ X3DScene **scenereturn);
	STDMETHOD(setDescription)(/*[in]*/ BSTR description);
	STDMETHOD(loadURL)(/*[in]*/ int nUrls, /*[in, size_is(nUrls)]*/ BSTR *url, /*[in]*/ int nParams, /*[in,size_is(nParams)]*/ BSTR *params);
	STDMETHOD(replaceWorld)(/*[in]*/ X3DScene *value);
	STDMETHOD(getWorldURL)(/*[out, retval]*/ BSTR *value);
	STDMETHOD(getCurrentFrameRate)(/*[out, retval]*/ float *value);
	STDMETHOD(getCurrentSpeed)(/*[out, retval]*/ float *value);
	STDMETHOD(getVersion)(/*[out, retval]*/ BSTR *value);
	STDMETHOD(getName)(/*[out, retval]*/ BSTR *value);
	STDMETHOD(getClosure)(/*[out, retval]*/ void *closure);
	STDMETHOD(setClosure)(/*[in]*/ void *closure);
	STDMETHOD(print)(/*[in]*/ BSTR str);
	STDMETHOD(loadUrlS)(/*[in]*/ BSTR url);
	STDMETHOD(getExecutionContext)(/*[out, retval]*/ X3DExecutionContext **ppContext);
	STDMETHOD(addBrowserListener)(/*[in]*/ IDispatch  *pListener);
	STDMETHOD(removeBrowserListener)(/*[in]*/ IDispatch  *pListener);
	STDMETHOD(addBrowserListenerV)(/*[in]*/ VARIANT listener);

	static void CallX3DCallback(sAnmThreadedBrowserCallback *pTCB);
};


#endif // _x3dbrowserbase_h
