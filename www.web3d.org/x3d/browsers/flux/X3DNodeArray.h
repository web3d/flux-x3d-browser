/********************************************************************************
 * Flux
 *
 * File: X3DNodeArray.h
 * Description: Declaration of the CX3DNodeArray
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

// X3DNodeArray.h : Declaration of the CX3DNodeArray

#ifndef __X3DNODEARRAY_H_
#define __X3DNODEARRAY_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CX3DNodeArray
class ATL_NO_VTABLE CX3DNodeArray : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DNodeArray, &CLSID_CoX3DNodeArray>,
	public IDispatchImpl<X3DNodeArray, &IID_X3DNodeArray, &LIBID_ANIMALib>
{
protected :

	std::vector<interface X3DNode *> m_nodes;

public:
	CX3DNodeArray()
	{
	}

	~CX3DNodeArray();

DECLARE_REGISTRY_RESOURCEID(IDR_X3DNODEARRAY)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DNodeArray)
	COM_INTERFACE_ENTRY(X3DNodeArray)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// X3DNodeArray
public:
	STDMETHOD(get_length)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_length)(/*[in]*/ long newVal);
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Count)(/*[int]*/ long val);
	STDMETHOD(Item)(/*[in]*/ VARIANT index, /*[out, retval]*/ VARIANT *pItem);
	STDMETHOD(get1Value)(/*[in]*/ int index, /*[out]*/ X3DNode **value);
	STDMETHOD(set1Value)(/*[in]*/ int index, /*[in]*/ X3DNode *value);
	static X3DNodeArray *CreateX3DNodeArray(std::vector<class CAnmSAINode *> *pSAINodes, X3DExecutionContext *pX3DContext);

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
	
};

#endif //__X3DNODEARRAY_H_
