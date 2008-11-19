/********************************************************************************
 * Flux
 *
 * File: X3DField.h
 * Description: Declaration of the CX3DField
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

// X3DField.h : Declaration of the CX3DField

#ifndef __X3DFIELD_H_
#define __X3DFIELD_H_

#include "resource.h"       // main symbols

#include "x3dfieldbase.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DField
class ATL_NO_VTABLE CX3DField : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DField, &CLSID_CoX3DField>,
	public IDispatchImpl<X3DField, &IID_X3DField, &LIBID_ANIMALib>,
	public X3DFieldBase
{
public:
	CX3DField()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DFIELD)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DField)
	COM_INTERFACE_ENTRY(X3DField)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// X3DField
public:
	STDMETHOD(attach)(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType);
	STDMETHOD(addFieldEventListener)(/*[in]*/ IDispatch *listener);
	STDMETHOD(removeFieldEventListener)(/*[in]*/ IDispatch *listener);
	STDMETHOD(getAccessType)(/*[out, retval]*/ X3DFieldAccess *accesstype);
	STDMETHOD(getType)(/*[out, retval]*/ X3DFieldType *fieldtype);
	STDMETHOD(getName)(/*[out, retval]*/ BSTR *name);
	STDMETHOD(dispose)();
};

#endif //__X3DFIELD_H_
