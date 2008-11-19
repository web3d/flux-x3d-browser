/********************************************************************************
 * Flux
 *
 * File: X3DBrowser.h
 * Description: Declaration of the CX3DBrowser
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

// X3DBrowser.h : Declaration of the CX3DBrowser

#ifndef __X3DBROWSER_H_
#define __X3DBROWSER_H_

#include "resource.h"       // main symbols
#include "x3dbrowserbase.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DBrowser
class ATL_NO_VTABLE CX3DBrowser : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DBrowser, &CLSID_CoX3DBrowser>,
	public IDispatchImpl<X3DBrowser, &IID_X3DBrowser, &LIBID_ANIMALib>,
	public X3DBrowserBase
{
public:
	CX3DBrowser()
	{
		SetX3DBrowser(this);
	}

	~CX3DBrowser()
	{
		int foo = 1;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DBROWSER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DBrowser)
	COM_INTERFACE_ENTRY(X3DBrowser)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// X3DBrowser
public:
	STDMETHOD(get_viewpoints)(/*[out, retval]*/ X3DNodeArray **pVal)
		{ return X3DBrowserBase::getViewpoints(pVal); }
	
	STDMETHOD(previousViewpoint)()
		{ return X3DBrowserBase::previousViewpoint(); }
	STDMETHOD(nextViewpoint)()
		{ return X3DBrowserBase::nextViewpoint(); }
	STDMETHOD(lastViewpoint)()
		{ return X3DBrowserBase::lastViewpoint(); }
	STDMETHOD(firstViewpoint)()
		{ return X3DBrowserBase::firstViewpoint(); }

	// old-style VRML; keep for reference
	// STDMETHOD(createX3DFromURL)(/*[in]*/ BSTR url, /*[in]*/ X3DNode *node, /*[in]*/ BSTR event)
	//	{ return X3DBrowserBase::createX3DFromURL(url, node, event); }
	STDMETHOD(createX3DFromURL)(/*[in]*/ BSTR url, /*[out,retval]*/ X3DScene **scenereturn)
		{ return X3DBrowserBase::createX3DFromURL(url, scenereturn); }
	STDMETHOD(createX3DFromString)(/*[in]*/ BSTR x3dSource, /*[out, retval]*/ X3DScene **scenereturn)
		{ return X3DBrowserBase::createX3DFromString(x3dSource, scenereturn); }
	STDMETHOD(setDescription)(/*[in]*/ BSTR description)
		{ return X3DBrowserBase::setDescription(description); }
	STDMETHOD(loadURL)(/*[in]*/ int nUrls, /*[in, size_is(nUrls)]*/ BSTR *url, /*[in]*/ int nParams, /*[in,size_is(nParams)]*/ BSTR *params)
		{ return X3DBrowserBase::loadURL(nUrls, url, nParams, params); }
	STDMETHOD(replaceWorld)(/*[in]*/ X3DScene *value)
		{ return X3DBrowserBase::replaceWorld(value); }
	STDMETHOD(getWorldURL)(/*[out, retval]*/ BSTR *value)
		{ return X3DBrowserBase::getWorldURL(value); }
	STDMETHOD(getCurrentFrameRate)(/*[out, retval]*/ float *value)
		{ return X3DBrowserBase::getCurrentFrameRate(value); }
	STDMETHOD(getCurrentSpeed)(/*[out, retval]*/ float *value)
		{ return X3DBrowserBase::getCurrentSpeed(value); }
	STDMETHOD(getVersion)(/*[out, retval]*/ BSTR *value)
		{ return X3DBrowserBase::getVersion(value); }
	STDMETHOD(getName)(/*[out, retval]*/ BSTR *value)
		{ return X3DBrowserBase::getName(value); }
	STDMETHOD(getClosure)(/*[out, retval]*/ void *closure)
		{ return X3DBrowserBase::getClosure(closure); }
	STDMETHOD(setClosure)(/*[in]*/ void *closure)
		{ return X3DBrowserBase::setClosure(closure); }
	STDMETHOD(print)(/*[in]*/ BSTR str)
		{ return X3DBrowserBase::print(str); }
	STDMETHOD(loadUrlS)(/*[in]*/ BSTR url)
		{ return X3DBrowserBase::loadUrlS(url); }
	STDMETHOD(getExecutionContext)(/*[out, retval]*/ X3DExecutionContext **ppContext)
		{ return X3DBrowserBase::getExecutionContext(ppContext); }
	STDMETHOD(addBrowserListener)(/*[in]*/ IDispatch *pListener)
		{ return X3DBrowserBase::addBrowserListener(pListener); }
	STDMETHOD(removeBrowserListener)(/*[in]*/ IDispatch  *pListener)
		{ return X3DBrowserBase::removeBrowserListener(pListener); }
	STDMETHOD(addBrowserListenerV)(/*[in]*/ VARIANT listener)
		{ return X3DBrowserBase::addBrowserListenerV(listener); }
};

#endif //__X3DBROWSER_H_
