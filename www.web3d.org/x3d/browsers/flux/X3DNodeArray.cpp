/********************************************************************************
 * Flux
 *
 * File: X3DNodeArray.cpp
 * Description: Implementation of CX3DNodeArray
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

// X3DNodeArray.cpp : Implementation of CX3DNodeArray
#include "stdafx.h"
#include "Anima.h"
#include "X3DNodeArray.h"
#include "X3DNode.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DNodeArray
CX3DNodeArray::~CX3DNodeArray()
{
	int sz = m_nodes.size();
	for (int i = 0; i < sz; i++)
		SafeRelease(m_nodes[i]);
}

X3DNodeArray *CX3DNodeArray::CreateX3DNodeArray(std::vector<CAnmSAINode *> *pSAINodes, X3DExecutionContext *pX3DContext)
{
	assert(pSAINodes);

	X3DNodeArray *pX3DNodeArray = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CoX3DNodeArray, NULL, CLSCTX_INPROC_SERVER, IID_X3DNodeArray, (LPVOID*)&pX3DNodeArray);

	if (FAILED(hr))
		return NULL;

	int sz = pSAINodes->size();
	pX3DNodeArray->put_Count(sz);

	for (int i = 0; i < sz; i++)
	{
		CAnmSAINode *pSAINode = (*pSAINodes)[i];
		X3DNode *pX3DNode = CX3DNode::CreateX3DNode(pSAINode, pX3DContext);
		if (pX3DNode == NULL)
			return NULL;

		pX3DNodeArray->set1Value(i, pX3DNode);
	}

	return pX3DNodeArray;
}

STDMETHODIMP CX3DNodeArray::get_Count(long *pVal)
{
	if (pVal == NULL)
		return E_INVALIDARG;

	*pVal = m_nodes.size();

	return S_OK;
}

STDMETHODIMP CX3DNodeArray::put_Count(long val)
{
	if (val <= 0)
		return E_INVALIDARG;

	m_nodes.reserve(val);
	int sz = m_nodes.size();

	for (int i = sz; i < val; i++)
		m_nodes.push_back(NULL);

	return S_OK;
}

STDMETHODIMP CX3DNodeArray::Item(VARIANT index, VARIANT *pItem)
{
	if (index.vt == VT_I2 || index.vt == VT_I4)
	{
		X3DNode *pX3DNode;

		int ind = (index.vt == VT_I2) ? index.iVal : index.lVal;

		HRESULT hr = get1Value(ind, &pX3DNode);

		if (SUCCEEDED(hr))
		{
			pItem->vt = VT_DISPATCH;
			pItem->pdispVal = pX3DNode;
			return S_OK;
		}
		else
			return hr;
	}

	return E_INVALIDARG;
}

STDMETHODIMP CX3DNodeArray::get1Value(int index, X3DNode **value)
{
	int sz = m_nodes.size();
	if (index < 0 || index >= sz)
		return E_INVALIDARG;

	*value = m_nodes[index];
	(*value)->AddRef();

	return S_OK;
}


STDMETHODIMP CX3DNodeArray::set1Value(int index, X3DNode *value)
{
	// Check boundary conditions
	if (value == NULL)
		return E_INVALIDARG;

	if (index < 0)
		return E_INVALIDARG;

	// Valid X3DNode?
	X3DNode *pX3DNode = NULL;

	// let's keep this as a sanity check for automation clients? -- TP 10/6/03
	HRESULT hr = value->QueryInterface(IID_X3DNode, (void **) &pX3DNode);

	if (!SUCCEEDED(hr))
		return E_INVALIDARG;

	// Ok, stuff it in. First, pad the array if needed
	int sz = m_nodes.size();
	for (int i = sz; i <= index; i++)
		m_nodes.push_back(NULL);

	// Release any old pointer
	if (m_nodes[index] != NULL)
		m_nodes[index]->Release();

	m_nodes[index] = pX3DNode;

	// N.B.: don't remove the extra ref, because we should add one anyway ->
	// (done in value->QueryInterface(IID_X3DNode,...) call above)

	// pX3DNode->Release();

	return S_OK;
}


STDMETHODIMP CX3DNodeArray::get_length(long *pVal)
{
	return get_Count(pVal);
}

STDMETHODIMP CX3DNodeArray::put_length(long newVal)
{
	return (put_Count(newVal));
}

// IDispatch overrides
#define IDOFFSET 32
        
STDMETHODIMP CX3DNodeArray::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DNodeArray, &IID_X3DNodeArray, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DNodeArray::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DNodeArray, &IID_X3DNodeArray, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}

STDMETHODIMP CX3DNodeArray::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DNodeArray, &IID_X3DNodeArray, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

	if (!SUCCEEDED(hr))
	{
		char indexbuf[ANMMAXSTRBUF];

		for (int nameindex = 0; nameindex < cNames; nameindex++)
		{
			wcstombs(indexbuf, rgszNames[nameindex], ANMMAXSTRBUF);

			if (isdigit(indexbuf[0]))
			{
				int index = atoi(indexbuf);

				rgDispId[nameindex] = index + IDOFFSET;
			}
			else
				return E_FAIL;
		}

		return S_OK;
	}

	return hr;
}
        
STDMETHODIMP CX3DNodeArray::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DNodeArray, &IID_X3DNodeArray, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
		wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	if (!SUCCEEDED(hr))
	{
		int index = dispIdMember - IDOFFSET;

		if (index >= 0)
		{
			X3DNode *pSFNode;

			if (wFlags == DISPATCH_PROPERTYGET)
			{
				hr = get1Value(index, &pSFNode);

				if (SUCCEEDED(hr))
				{
					assert(pVarResult);
					pVarResult->vt = VT_DISPATCH;
					pVarResult->pdispVal = pSFNode;

					return S_OK;
				}
				else
					return S_FALSE;
			}
			else if (wFlags & DISPATCH_PROPERTYPUT)
			{
				assert(pDispParams);

				if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_DISPATCH)
				{
					LPDISPATCH pDispatch = pDispParams->rgvarg[0].pdispVal;

					hr = pDispatch->QueryInterface(IID_X3DNode, (void **) &pSFNode);
					if (SUCCEEDED(hr))
					{
						hr = set1Value(index, pSFNode);

						pSFNode->Release();

						return hr;
					}
				}
				else
					return E_FAIL;
			}
		}
		else
			return E_FAIL;
	}

	return hr;
}
