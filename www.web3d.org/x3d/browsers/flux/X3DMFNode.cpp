/********************************************************************************
 * Flux
 *
 * File: X3DMFNode.cpp
 * Description: Implementation of CX3DMFNode
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

// X3DMFNode.cpp : Implementation of CX3DMFNode
#include "stdafx.h"
#include "Anima.h"
#include "X3DMFNode.h"
#include "x3dnode.h"
#include "x3dnodearray.h"
#include "anmexecutioncontext.h"
#include "anmbrowser.h"
#include "anmnode.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DMFNode

STDMETHODIMP CX3DMFNode::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_mfnodevalue);

	if (m_mfnodevalue)
		m_mfnodevalue->Ref();
	else
		m_mfnodevalue = new NodeArray;

	return S_OK;
}


STDMETHODIMP CX3DMFNode::getValue(X3DNodeArray **value)
{
	NodeArray *pna = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &pna);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	else if (setval == ANMFIELDBASE_DIY)
		pna = m_mfnodevalue;

	int sz = pna->size();

	std::vector<CAnmSAINode *> saiNodes;
	if (sz > 0)
		saiNodes.reserve(sz);

	for (int i = 0; i < sz; i++)
	{
		CAnmNode *pNode = (*pna)[i];
		CAnmSAINode *pSAINode = new CAnmSAINode(pNode);
		pSAINode->SetSink(m_node->GetSink());
		saiNodes.push_back(pSAINode);
	}

	*value = CX3DNodeArray::CreateX3DNodeArray(&saiNodes, m_context);
	return S_OK;
}

STDMETHODIMP CX3DMFNode::setValue(X3DNodeArray *value)
{
	HRESULT hr;

	X3DNodeArray *pX3DNodeArray = NULL;

	// let's keep this as a sanity check for automation clients? -- TP 10/6/03
	hr = value->QueryInterface(IID_X3DNodeArray, (void **) &pX3DNodeArray);
	if (SUCCEEDED(hr))
	{
		// remove the extra ref
		pX3DNodeArray->Release();

		long count;
		hr = pX3DNodeArray->get_Count(&count);
		if (FAILED(hr)) {
			return E_FAIL;
		}

		SafeUnRef(m_mfnodevalue);

		m_mfnodevalue = new NodeArray;

		for (int i = 0; i < count; i++)
		{
			X3DNode *pX3DNode = NULL;

			hr = pX3DNodeArray->get1Value(i, &pX3DNode);
			if (SUCCEEDED(hr))
			{
				X3DNode *pSaneX3DNode = NULL;
				// let's keep this as a sanity check for automation clients? -- TP 10/6/03
				hr = pX3DNode->QueryInterface(IID_X3DNode, (void **) &pSaneX3DNode);
				if (SUCCEEDED(hr))
				{
					// remove the extra ref
					pSaneX3DNode->Release();

					CAnmSAINode *pSAINode = NULL;

					hr = pX3DNode->getNativeNode(&pSAINode);

					if (SUCCEEDED(hr) && pSAINode != NULL)
					{
						CAnmNode *pNode = pSAINode->GetNode();
						m_mfnodevalue->push_back(pNode);

						// N.B.: make sure this has a reasonable execution context!
						pX3DNode->setContext(m_context);
					
					}
				}
				else
					m_mfnodevalue->push_back(NULL);
			}
			else
			{
				m_mfnodevalue->push_back(NULL);
			}
		}

		if (m_mfnodevalue->size() >= 0)
		{
			if (X3DFieldBase::setValue((void *) &m_mfnodevalue) != ANMFIELDBASE_CANTDO)
				return S_OK;
			else
				return S_FALSE;
		}

	}
	else
		return E_INVALIDARG;

	return S_OK;
}

#if 0
// Old, kept around for historical reasons
STDMETHODIMP CX3DMFNode::setValue(int cnt, LPDISPATCH *value)
{
	HRESULT hr;

	SafeUnRef(m_mfnodevalue);
	m_mfnodevalue = new NodeArray;

	for (int i = 0; i < cnt; i++)
	{
		X3DNode *pX3DNode = NULL;

		hr = value[i]->QueryInterface(IID_X3DNode, (void **) &pX3DNode);
		if (SUCCEEDED(hr))
		{
			CAnmSAINode *pSAINode = NULL;

			hr = pX3DNode->getNativeNode(&pSAINode);

			if (SUCCEEDED(hr) && pSAINode != NULL)
			{
				m_mfnodevalue->push_back(pSAINode->GetNode());
			}
		}
	}

	if (m_mfnodevalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfnodevalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}
#endif // 0

STDMETHODIMP CX3DMFNode::set1Value(int index, X3DNode *value)
{
	assert(m_mfnodevalue);

	X3DNode *pX3DNode = NULL;
	CAnmNode *pNode = NULL;

	if (value)
	{
		// let's keep this as a sanity check for automation clients? -- TP 10/6/03
		HRESULT hr = value->QueryInterface(IID_X3DNode, (void **) &pX3DNode);
		if (SUCCEEDED(hr))
		{
			// remove the extra ref
			pX3DNode->Release();

			CAnmSAINode *pSAINode = NULL;

			hr = pX3DNode->getNativeNode(&pSAINode);

			if (SUCCEEDED(hr) && pSAINode != NULL)
			{
				pNode = pSAINode->GetNode();

				// N.B.: make sure this has a reasonable execution context!
				pX3DNode->setContext(m_context);
			}
			else if (FAILED(hr))
				return E_FAIL;
		}
		else
			return E_INVALIDARG;
	}

	int sz = m_mfnodevalue->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			m_mfnodevalue->push_back(NULL);
			sz++;
		}
		m_mfnodevalue->push_back(pNode);
	}
	else
		(*m_mfnodevalue)[index] = pNode;

	if (m_mfnodevalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfnodevalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CX3DMFNode::get_Count(long *pVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		*pVal = m_mfnodevalue->size();
		return S_OK;
	}

	NodeArray *pna = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &pna);
	assert(pna != NULL);

	*pVal = pna->size();

	return S_OK;
}


STDMETHODIMP CX3DMFNode::get1Value(int index, X3DNode **value)
{
	NodeArray *pna = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &pna);

	if (setval == ANMFIELDBASE_CANTDO)
		return E_FAIL;
	else if (setval == ANMFIELDBASE_DIY)
		pna = m_mfnodevalue;

	if (index < 0 || index >= pna->size())
		return E_FAIL;

	CAnmNode *pNode = (*pna)[index];
	CAnmSAINode *pSAINode = new CAnmSAINode(pNode);
	pSAINode->SetSink(m_node->GetSink());

	X3DNode *pX3DNode = CX3DNode::CreateX3DNode(pSAINode, m_context);
	if (pX3DNode)
	{
		*value = pX3DNode;
		return S_OK;
	}
	else
		return E_FAIL;
}

STDMETHODIMP CX3DMFNode::get1ValueV(int index, VARIANT *value)
{
	X3DNode *pX3DNode;

	HRESULT hr = get1Value(index, &pX3DNode);

	if (SUCCEEDED(hr))
	{
		value->vt = VT_DISPATCH;
		value->pdispVal = pX3DNode;
		return S_OK;
	}
	else
		return hr;
}


STDMETHODIMP CX3DMFNode::Item(VARIANT index, VARIANT *pItem)
{
	if (index.vt == VT_I2 || index.vt == VT_I4)
	{
		X3DNode *pX3DNode = NULL;

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



void CX3DMFNode::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	NodeArray *pna = *(NodeArray **) callData;

	assert(pna != NULL);

	SafeUnRef(m_mfnodevalue);
	m_mfnodevalue = pna;
	m_mfnodevalue->Ref();

	X3DFieldBase::callCallbacks(this);
}

STDMETHODIMP CX3DMFNode::get_length(long *pVal)
{
	return get_Count(pVal);
}

STDMETHODIMP CX3DMFNode::put_length(long newVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		m_mfnodevalue->resize(newVal);
		return S_OK;
	}

	NodeArray *pna = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &pna);
	assert(pna != NULL);

	pna->resize(newVal);

	return S_OK;
}

// IDispatch overrides
#define IDOFFSET 32        

STDMETHODIMP CX3DMFNode::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DMFNode, &IID_X3DMFNode, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DMFNode::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DMFNode, &IID_X3DMFNode, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}
        
STDMETHODIMP CX3DMFNode::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DMFNode, &IID_X3DMFNode, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

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
        
STDMETHODIMP CX3DMFNode::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DMFNode, &IID_X3DMFNode, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
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

