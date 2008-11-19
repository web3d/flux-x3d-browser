/********************************************************************************
 * Flux
 *
 * File: X3DRoute.h
 * Description: Declaration of the CX3DRoute
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

// X3DRoute.h : Declaration of the CX3DRoute

#ifndef __X3DROUTE_H_
#define __X3DROUTE_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CX3DRoute
class ATL_NO_VTABLE CX3DRoute : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DRoute, &CLSID_CoX3DRoute>,
	public IDispatchImpl<X3DRoute, &IID_X3DRoute, &LIBID_ANIMALib>
{
protected :

	class CAnmSAIRoute	*m_route;
	interface X3DExecutionContext *m_context;
	
public:
	CX3DRoute()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DROUTE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DRoute)
	COM_INTERFACE_ENTRY(X3DRoute)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// X3DRoute
public:
	STDMETHOD(setClosure)(/*[in]*/ void *closure);
	STDMETHOD(getClosure)(/*[in]*/ void **closure);
	STDMETHOD(setContext)(/*[in]*/ X3DExecutionContext *context);
	STDMETHOD(getContext)(/*[out, retval]*/ X3DExecutionContext **result);
	STDMETHOD(getSourceNode)(/*[out, retval]*/ X3DNode **pSourceNode);
	STDMETHOD(getSourceField)(/*[out, retval]*/ BSTR *sourcefield);
	STDMETHOD(getDestinationNode)(/*[out, retval]*/ X3DNode **pDestinationNode);
	STDMETHOD(getDestinationField)(/*[out, retval]*/ BSTR *destinationfield);

	static X3DRoute *CreateX3DRoute(class CAnmSAIRoute *pSAIRoute, X3DExecutionContext *context);
};

#endif //__X3DROUTE_H_
