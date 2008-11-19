/********************************************************************************
 * Flux
 *
 * File: X3DMField.h
 * Description: Declaration of the CX3DMField
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

// X3DMField.h : Declaration of the CX3DMField

#ifndef __X3DMFIELD_H_
#define __X3DMFIELD_H_

#include "resource.h"       // main symbols
#include "x3dfieldbase.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DMField
class ATL_NO_VTABLE CX3DMField : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DMField, &CLSID_CoX3DMField>,
	public IDispatchImpl<X3DMField, &IID_X3DMField, &LIBID_ANIMALib>,
	X3DFieldBase
{
public:
	CX3DMField()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DMFIELD)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DMField)
	COM_INTERFACE_ENTRY(X3DMField)
	COM_INTERFACE_ENTRY(X3DField)
	COM_INTERFACE_ENTRY2(IDispatch, X3DField)
	COM_INTERFACE_ENTRY2(IDispatch, X3DMField)
END_COM_MAP()

// X3DMField
public:
	STDMETHOD(get_length)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_length)(/*[in]*/ long newVal);

	STDMETHOD(removeFieldEventListener)(/*[in]*/ IDispatch * listener)
	{ X3DFieldBase::removeFieldEventListener(listener); return S_OK; }

	STDMETHOD(addFieldEventListener)(/*[in]*/ IDispatch * listener)
	{ X3DFieldBase::addFieldEventListener(listener); return S_OK; }

	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
	STDMETHOD(Item)(/*[in]*/ VARIANT index, /*[out, retval]*/ VARIANT *pItem);

	STDMETHOD(attach)(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
	{ X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType); return S_OK; }

	STDMETHOD(getAccessType)(/*[out, retval]*/ X3DFieldAccess *accesstype)
	{ return X3DFieldBase::getAccessType(accesstype); }

	STDMETHOD(getType)(/*[out, retval]*/ X3DFieldType *fieldtype)
	{ return X3DFieldBase::getType(fieldtype); }

	STDMETHOD(getName)(/*[out, retval]*/ BSTR *name)
	{ return X3DFieldBase::getName(name); }

	STDMETHOD(dispose)()
	{ return X3DFieldBase::dispose(); }


};

#endif //__X3DMFIELD_H_
