/********************************************************************************
 * Flux
 *
 * File: X3DMFFloat.cpp
 * Description: Implementation of CX3DMFFloat
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

// X3DMFFloat.cpp : Implementation of CX3DMFFloat
#include "stdafx.h"
#include "Anima.h"
#include "X3DMFFloat.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DMFFloat

STDMETHODIMP CX3DMFFloat::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_mffloatvalue);

	if (m_mffloatvalue)
		m_mffloatvalue->Ref();
	else
		m_mffloatvalue = new FloatArray;

	return S_OK;
}

STDMETHODIMP CX3DMFFloat::getValue(int cnt, float *value)
{
	FloatArray *pfa = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &pfa);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	else if (setval == ANMFIELDBASE_DIY)
		pfa = m_mffloatvalue;

	for (int i = 0; i < pfa->size(); i++)
	{
		if (i >= cnt)
			break;

		value[i] = (*pfa)[i];
	}
	
	return S_OK;
}

STDMETHODIMP CX3DMFFloat::setValue(int cnt, float *value)
{
	SafeUnRef(m_mffloatvalue);
	m_mffloatvalue = new FloatArray;

	for (int i = 0; i < cnt; i++)
	{
		m_mffloatvalue->push_back(value[i]);
	}

	if (m_mffloatvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mffloatvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CX3DMFFloat::set1Value(int index, float value)
{
	assert(m_mffloatvalue);

	int sz = m_mffloatvalue->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			m_mffloatvalue->push_back(0.f);
			sz++;
		}
		m_mffloatvalue->push_back(value);
	}
	else
		(*m_mffloatvalue)[index] = value;

	if (m_mffloatvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mffloatvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}


STDMETHODIMP CX3DMFFloat::get1Value(int index, float *value)
{
	FloatArray *pfa = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &pfa);

	if (setval == ANMFIELDBASE_CANTDO)
		return E_FAIL;
	else if (setval == ANMFIELDBASE_DIY)
		pfa = m_mffloatvalue;

	if (index < 0 || index >= pfa->size())
		return E_FAIL;

	float f = (*pfa)[index];

	*value = f;

	return S_OK;
}

STDMETHODIMP CX3DMFFloat::get_Count(long *pVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		*pVal = m_mffloatvalue->size();
		return S_OK;
	}

	FloatArray *pfa = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &pfa);
	assert(pfa != NULL);

	*pVal = pfa->size();

	return S_OK;
}

STDMETHODIMP CX3DMFFloat::Item(VARIANT index, VARIANT *pItem)
{
	if (index.vt == VT_I2 || index.vt == VT_I4)
	{
		float f = 0.f;

		int ind = (index.vt == VT_I2) ? index.iVal : index.lVal;

		HRESULT hr = get1Value(ind, &f);

		if (SUCCEEDED(hr))
		{
			pItem->vt = VT_R4;
			pItem->fltVal = f;
			return S_OK;
		}
		else
			return hr;
	}

	return E_INVALIDARG;
}


void CX3DMFFloat::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	FloatArray *pfa = *(FloatArray **) callData;

	assert(pfa != NULL);

	SafeUnRef(m_mffloatvalue);
	m_mffloatvalue = pfa;
	m_mffloatvalue->Ref();

	X3DFieldBase::callCallbacks(this);
}


STDMETHODIMP CX3DMFFloat::get_length(long *pVal)
{
	return get_Count(pVal);
}

STDMETHODIMP CX3DMFFloat::put_length(long newVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		m_mffloatvalue->resize(newVal);
		return S_OK;
	}

	FloatArray *pfa = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &pfa);
	assert(pfa != NULL);

	pfa->resize(newVal);

	return S_OK;
}

// IDispatch overrides
#define IDOFFSET 32        

STDMETHODIMP CX3DMFFloat::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DMFFloat, &IID_X3DMFFloat, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DMFFloat::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DMFFloat, &IID_X3DMFFloat, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}
        
STDMETHODIMP CX3DMFFloat::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DMFFloat, &IID_X3DMFFloat, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

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
        
STDMETHODIMP CX3DMFFloat::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DMFFloat, &IID_X3DMFFloat, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
		wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	if (!SUCCEEDED(hr))
	{
		int index = dispIdMember - IDOFFSET;

		if (index >= 0)
		{
			float f;

			if (wFlags == DISPATCH_PROPERTYGET)
			{
				hr = get1Value(index, &f);

				if (SUCCEEDED(hr))
				{
					assert(pVarResult);
					pVarResult->vt = VT_R4;
					pVarResult->fltVal = f;

					return S_OK;
				}
				else
					return S_FALSE;
			}
			else if (wFlags == DISPATCH_PROPERTYPUT)
			{
				assert(pDispParams);

				if (pDispParams->cArgs > 0)
				{
					if (pDispParams->rgvarg[0].vt == VT_R4)
						f = pDispParams->rgvarg[0].fltVal;
					else if (pDispParams->rgvarg[0].vt == VT_R8)
						f = (float) pDispParams->rgvarg[0].dblVal;
					else if (pDispParams->rgvarg[0].vt == VT_I4)
						f = (float) pDispParams->rgvarg[0].lVal;
					else if (pDispParams->rgvarg[0].vt == VT_I2)
						f = (float) pDispParams->rgvarg[0].iVal;
					else
						return E_FAIL;

					return set1Value(index, f);
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
