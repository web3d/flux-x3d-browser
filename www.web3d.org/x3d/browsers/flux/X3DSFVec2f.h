/********************************************************************************
 * Flux
 *
 * File: X3DSFVec2f.h
 * Description: Declaration of the CX3DSFVec2f
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

// X3DSFVec2f.h : Declaration of the CX3DSFVec2f

#ifndef __X3DSFVEC2F_H_
#define __X3DSFVEC2F_H_

#include "resource.h"       // main symbols
#include "x3dfieldbase.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFVec2f
class ATL_NO_VTABLE CX3DSFVec2f : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DSFVec2f, &CLSID_CoX3DSFVec2f>,
	public IDispatchImpl<X3DSFVec2f, &IID_X3DSFVec2f, &LIBID_ANIMALib>,
	public X3DFieldBase
{
public:
	CX3DSFVec2f()
	{
		m_point2value = Point2(0, 0);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DSFVEC2F)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DSFVec2f)
	COM_INTERFACE_ENTRY(X3DSFVec2f)
	COM_INTERFACE_ENTRY(X3DField)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

protected:

	Point2			m_point2value;

// X3DSFVec2f
public:
	STDMETHOD(get_x)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_x)(/*[in]*/ float newVal);

	STDMETHOD(get_y)(/*[out, retval]*/ float *pVal);
	STDMETHOD(put_y)(/*[in]*/ float newVal);

	STDMETHOD(setValue)(/*[in,size_is(2)]*/ float *value);
	STDMETHOD(getValue)(/*[in,out,size_is(2)]*/ float *value);

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

	static X3DSFVec2f *Create(X3DExecutionContext *pX3DContext, const Point2 &p2);
	
	
};

#endif //__X3DSFVEC2F_H_
