/********************************************************************************
 * Flux
 *
 * File:  X3DNode.h
 * Description: Declaration of the CX3DNode
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

// X3DNode.h : Declaration of the CX3DNode

#ifndef __X3DNODE_H_
#define __X3DNODE_H_

#include "resource.h"       // main symbols
#include "x3dnodebase.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DNode
class ATL_NO_VTABLE CX3DNode : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DNode, &CLSID_CoX3DNode>,
	public IDispatchImpl<X3DNode, &IID_X3DNode, &LIBID_ANIMALib>,
	X3DNodeBase
{
public:
	CX3DNode()
	{
		m_context = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DNODE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DNode)
	COM_INTERFACE_ENTRY(X3DNode)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// X3DNode
public:
	STDMETHOD(getFieldDefinitions)(/*[out, retval]*/ X3DFieldDefinitionArray **fieldDefinitions);
	STDMETHOD(get_Text)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(getFieldInfo)(/*[in]*/ int fieldIndex, /*[out]*/ X3DFieldAccess *accessType, /*[out]*/ X3DFieldType *fieldType, /*[out]*/ BSTR *fieldName);
	STDMETHOD(getNumFields)(/*[out, retval]*/ int *result);
	STDMETHOD(getName)(/*[out, retval]*/ BSTR *name);
	STDMETHOD(setContext)(/*[in]*/ X3DExecutionContext *context);
	STDMETHOD(getContext)(/*[out, retval]*/ X3DExecutionContext **result);
	STDMETHOD(getField)(/*[in]*/ BSTR name, /*[out, retval]*/ X3DField **result);
	STDMETHOD(getTypeName)(/*[out, retval]*/ BSTR *typenm);
	STDMETHOD(getNativeNode)(/*[out, retval]*/ void *pNativeNode)
	{
		*((CAnmSAINode **) pNativeNode) = X3DNodeBase::getNativeNode();
		return S_OK;
	}

	STDMETHOD(setNativeNode)(/*[out, retval]*/ void *pNativeNode)
	{
		X3DNodeBase::setNativeNode((CAnmSAINode *) pNativeNode);
		return S_OK;
	}

	static X3DNode *CreateX3DNode(class CAnmSAINode *pSAINode, X3DExecutionContext *pX3DContext);

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


protected:

	interface X3DExecutionContext *m_context;

	HRESULT STDMETHODCALLTYPE getField(char *fieldname, X3DField **result);
	HRESULT STDMETHODCALLTYPE getFieldAsVariant(char *fieldname, VARIANT *result);
	HRESULT STDMETHODCALLTYPE setFieldFromVariant(char *fieldname, const VARIANT *value);
	
};

#endif //__X3DNODE_H_
