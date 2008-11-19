/********************************************************************************
 * Flux
 *
 * File: X3DMFFloat.h
 * Description: Declaration of the CX3DMFFloat
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

// X3DMFFloat.h : Declaration of the CX3DMFFloat

#ifndef __X3DMFFLOAT_H_
#define __X3DMFFLOAT_H_

#include "resource.h"       // main symbols
#include "x3dfieldbase.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DMFFloat
class ATL_NO_VTABLE CX3DMFFloat : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DMFFloat, &CLSID_CoX3DMFFloat>,
	public IDispatchImpl<X3DMFFloat, &IID_X3DMFFloat, &LIBID_ANIMALib>,
	X3DFieldBase
{
public:
	CX3DMFFloat()
	{
//		m_mffloatvalue = new FloatArray;
		m_mffloatvalue = NULL;
	}
	~CX3DMFFloat()
	{
		SafeUnRef(m_mffloatvalue);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DMFFLOAT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DMFFloat)
	COM_INTERFACE_ENTRY(X3DMFFloat)
	COM_INTERFACE_ENTRY(X3DMField)
	COM_INTERFACE_ENTRY(X3DField)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

protected :

	FloatArray		*m_mffloatvalue;

		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

// X3DMFFloat
public:
	STDMETHOD(get_length)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_length)(/*[in]*/ long newVal);
	STDMETHOD(setValue)(/*[in]*/ int cnt, /*[in, out,  size_is(cnt)]*/ float *value);
	STDMETHOD(set1Value)(/*[in]*/ int index, /*[in]*/ float value);
	STDMETHOD(getValue)(/*[in]*/ int cnt, /*[in, out, size_is(cnt)]*/ float *value);
	STDMETHOD(get1Value)(/*[in]*/ int index, /*[out]*/ float *value);

	STDMETHOD(removeFieldEventListener)(/*[in]*/ IDispatch * listener)
	{ X3DFieldBase::removeFieldEventListener(listener); return S_OK; }

	STDMETHOD(addFieldEventListener)(/*[in]*/ IDispatch * listener)
	{ X3DFieldBase::addFieldEventListener(listener); return S_OK; }

	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
	STDMETHOD(Item)(/*[in]*/ VARIANT index, /*[out, retval]*/ VARIANT *pItem);

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
};

#endif //__X3DMFFLOAT_H_
