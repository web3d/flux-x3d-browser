/********************************************************************************
 * Flux
 *
 * File: X3DEventOutObserver.h
 * Description: Declaration of the CX3DEventOutObserver
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

// X3DEventOutObserver.h : Declaration of the CX3DEventOutObserver

#ifndef __X3DEVENTOUTOBSERVER_H_
#define __X3DEVENTOUTOBSERVER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CX3DEventOutObserver
class ATL_NO_VTABLE CX3DEventOutObserver : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DEventOutObserver, &CLSID_CoX3DEventOutObserver>,
	public IDispatchImpl<X3DEventOutObserver, &IID_X3DEventOutObserver, &LIBID_ANIMALib>
{
public:
	CX3DEventOutObserver()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DEVENTOUTOBSERVER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DEventOutObserver)
	COM_INTERFACE_ENTRY(X3DEventOutObserver)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// X3DEventOutObserver
public:
	STDMETHOD(callbackFunction)(/*[in]*/ X3DField *value, /*[in]*/ double timeStamp, /*[in]*/ IDispatch *userData);

};

#endif //__X3DEVENTOUTOBSERVER_H_
