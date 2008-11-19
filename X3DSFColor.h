/********************************************************************************
 * Flux
 *
 * File: X3DSFColor.h
 * Description: Declaration of the CX3DSFColor
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

// X3DSFColor.h : Declaration of the CX3DSFColor

#ifndef __X3DSFCOLOR_H_
#define __X3DSFCOLOR_H_

#include "resource.h"       // main symbols
#include "x3dfieldbase.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFColor
class ATL_NO_VTABLE CX3DSFColor : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DSFColor, &CLSID_CoX3DSFColor>,
	public IDispatchImpl<X3DSFColor, &IID_X3DSFColor, &LIBID_ANIMALib>,
	public X3DFieldBase
{
public:
	CX3DSFColor()
	{
		m_colorvalue = Color(0, 0, 0);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DSFCOLOR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DSFColor)
	COM_INTERFACE_ENTRY(X3DSFColor)
	COM_INTERFACE_ENTRY(X3DField)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

protected :

	Color		m_colorvalue;

// X3DSFColor
public:
	STDMETHOD(get_r)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_r)(/*[in]*/ float newVal);

	STDMETHOD(get_g)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_g)(/*[in]*/ float newVal);

	STDMETHOD(get_b)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_b)(/*[in]*/ float newVal);
	
	STDMETHOD(setValue)(/*[in, size_is(3)]*/ float *value);
	STDMETHOD(getValue)(/*[in, out, size_is(3)]*/ float *value);

	STDMETHOD(removeFieldEventListener)(/*[in]*/ IDispatch * listener)
	{ X3DFieldBase::removeFieldEventListener(listener); return S_OK; }

	STDMETHOD(addFieldEventListener)(/*[in]*/ IDispatch * listener)
	{ X3DFieldBase::addFieldEventListener(listener); return S_OK; }

	STDMETHOD(attach)(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType);

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

	static X3DSFColor *Create(X3DExecutionContext *pX3DContext, const Color &c);

};

#endif //__X3DSFCOLOR_H_
