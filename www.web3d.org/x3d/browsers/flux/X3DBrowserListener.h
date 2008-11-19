/********************************************************************************
 * Flux
 *
 * File: X3DBrowserListener.h
 * Description: Declaration of the CX3DBrowserListener
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

// X3DBrowserListener.h : Declaration of the CX3DBrowserListener

#ifndef __X3DBROWSERLISTENER_H_
#define __X3DBROWSERLISTENER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CX3DBrowserListener
class ATL_NO_VTABLE CX3DBrowserListener : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DBrowserListener, &CLSID_CoX3DBrowserListener>,
	public IDispatchImpl<X3DBrowserListener, &IID_X3DBrowserListener, &LIBID_ANIMALib>
{
public:
	CX3DBrowserListener()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DBROWSERLISTENER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DBrowserListener)
	COM_INTERFACE_ENTRY(X3DBrowserListener)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// X3DBrowserListener
public:
	STDMETHOD(browserChanged)(/*[in]*/ X3DBrowserEvent event);
};

#endif //__X3DBROWSERLISTENER_H_
