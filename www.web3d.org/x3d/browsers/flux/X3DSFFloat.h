/********************************************************************************
 * Flux
 *
 * File: X3DSFFloat.h
 * Description: Declaration of the CX3DSFFloat
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

// X3DSFFloat.h : Declaration of the CX3DSFFloat

#ifndef __X3DSFFLOAT_H_
#define __X3DSFFLOAT_H_

#include "resource.h"       // main symbols
#include "x3dfieldbase.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFFloat
class ATL_NO_VTABLE CX3DSFFloat : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DSFFloat, &CLSID_CoX3DSFFloat>,
	public IDispatchImpl<X3DSFFloat, &IID_X3DSFFloat, &LIBID_ANIMALib>,
	public X3DFieldBase
{
public:
	CX3DSFFloat()
	{
		m_floatvalue = 0.f;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DSFFLOAT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DSFFloat)
	COM_INTERFACE_ENTRY(X3DSFFloat)
	COM_INTERFACE_ENTRY(X3DField)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

protected :

	Float	m_floatvalue;

// X3DSFFloat
public:
	STDMETHOD(setValue)(/*[in]*/ float value);
	STDMETHOD(getValue)(/*[in,out]*/ float *value);

	STDMETHOD(removeFieldEventListener)(/*[in]*/ IDispatch * listener)
	{ X3DFieldBase::removeFieldEventListener(listener); return S_OK; }

	STDMETHOD(addFieldEventListener)(/*[in]*/ IDispatch * listener)
	{ X3DFieldBase::addFieldEventListener(listener); return S_OK; }

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

// X3DFieldBase overrides

	virtual void handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData);	
};

#endif //__X3DSFFLOAT_H_
