/********************************************************************************
 * Flux
 *
 * File: X3DFieldEventListener.h
 * Description: Declaration of the CX3DFieldEventListener
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

// X3DFieldEventListener.h : Declaration of the CX3DFieldEventListener

#ifndef __X3DFIELDEVENTLISTENER_H_
#define __X3DFIELDEVENTLISTENER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CX3DFieldEventListener
class ATL_NO_VTABLE CX3DFieldEventListener : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DFieldEventListener, &CLSID_CoX3DFieldEventListener>,
	public IDispatchImpl<X3DFieldEventListener, &IID_X3DFieldEventListener, &LIBID_ANIMALib>
{
public:
	CX3DFieldEventListener()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DFIELDEVENTLISTENER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DFieldEventListener)
	COM_INTERFACE_ENTRY(X3DFieldEventListener)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// X3DFieldEventListener
public:

	STDMETHOD(readableFieldChanged)(/*[in]*/ X3DField *pField, double timestamp);
	
};

#endif //__X3DFIELDEVENTLISTENER_H_
