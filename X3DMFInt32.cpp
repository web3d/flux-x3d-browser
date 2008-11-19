/********************************************************************************
 * Flux
 *
 * File: X3DMFInt32.cpp
 * Description: Implementation of CX3DMFInt32
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

// X3DMFInt32.cpp : Implementation of CX3DMFInt32
#include "stdafx.h"
#include "Anima.h"
#include "X3DMFInt32.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DMFInt32

STDMETHODIMP CX3DMFInt32::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_mfint32value);

	if (m_mfint32value)
		m_mfint32value->Ref();
	else
		m_mfint32value = new IntegerArray;

	return S_OK;
}


STDMETHODIMP CX3DMFInt32::getValue(int cnt, long *value)
{
	IntegerArray *pia = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &pia);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	else if (setval == ANMFIELDBASE_DIY)
		pia = m_mfint32value;

	for (int i = 0; i < pia->size(); i++)
	{
		if (i >= cnt)
			break;

		value[i] = (*pia)[i];
	}
	
	return S_OK;
}

STDMETHODIMP CX3DMFInt32::setValue(int cnt, long *value)
{
	SafeUnRef(m_mfint32value);
	m_mfint32value = new IntegerArray;

	for (int i = 0; i < cnt; i++)
	{
		m_mfint32value->push_back(value[i]);
	}

	if (m_mfint32value->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfint32value) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CX3DMFInt32::set1Value(int index, long value)
{
	assert(m_mfint32value);

	int sz = m_mfint32value->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			m_mfint32value->push_back(0);
			sz++;
		}
		m_mfint32value->push_back(value);
	}
	else
		(*m_mfint32value)[index] = value;

	if (m_mfint32value->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfint32value) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}


STDMETHODIMP CX3DMFInt32::get1Value(int index, long *value)
{
	IntegerArray *pia = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &pia);

	if (setval == ANMFIELDBASE_CANTDO)
		return E_FAIL;
	else if (setval == ANMFIELDBASE_DIY)
		pia = m_mfint32value;

	if (index < 0 || index >= pia->size())
		return E_FAIL;

	long lval = (*pia)[index];

	*value = lval;

	return S_OK;
}

STDMETHODIMP CX3DMFInt32::get_Count(long *pVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		*pVal = m_mfint32value->size();
		return S_OK;
	}

	IntegerArray *pia = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &pia);
	assert(pia != NULL);

	*pVal = pia->size();

	return S_OK;
}

STDMETHODIMP CX3DMFInt32::Item(VARIANT index, VARIANT *pItem)
{
	if (index.vt == VT_I2 || index.vt == VT_I4)
	{
		long lval = 0L;

		int ind = (index.vt == VT_I2) ? index.iVal : index.lVal;

		HRESULT hr = get1Value(ind, &lval);

		if (SUCCEEDED(hr))
		{
			pItem->vt = VT_I4;
			pItem->lVal = lval;
			return S_OK;
		}
		else
			return hr;
	}

	return E_INVALIDARG;
}

void CX3DMFInt32::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	IntegerArray *pia = *(IntegerArray **) callData;

	assert(pia != NULL);

	SafeUnRef(m_mfint32value);
	m_mfint32value = pia;
	m_mfint32value->Ref();

	X3DFieldBase::callCallbacks(this);
}


STDMETHODIMP CX3DMFInt32::get_length(long *pVal)
{
	return get_Count(pVal);
}

STDMETHODIMP CX3DMFInt32::put_length(long newVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		m_mfint32value->resize(newVal);
		return S_OK;
	}

	IntegerArray *pia = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &pia);
	assert(pia != NULL);

	pia->resize(newVal);

	return S_OK;
}

// IDispatch overrides
#define IDOFFSET 32        

STDMETHODIMP CX3DMFInt32::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DMFInt32, &IID_X3DMFInt32, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DMFInt32::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DMFInt32, &IID_X3DMFInt32, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}

STDMETHODIMP CX3DMFInt32::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DMFInt32, &IID_X3DMFInt32, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

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
        
STDMETHODIMP CX3DMFInt32::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DMFInt32, &IID_X3DMFInt32, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
		wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	if (!SUCCEEDED(hr))
	{
		int index = dispIdMember - IDOFFSET;

		if (index >= 0)
		{
			long i32;

			if (wFlags == DISPATCH_PROPERTYGET)
			{
				hr = get1Value(index, &i32);

				if (SUCCEEDED(hr))
				{
					assert(pVarResult);
					pVarResult->vt = VT_I4;
					pVarResult->intVal = i32;

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
						i32 = (int) pDispParams->rgvarg[0].fltVal;
					else if (pDispParams->rgvarg[0].vt == VT_R8)
						i32 = (int) pDispParams->rgvarg[0].dblVal;
					else if (pDispParams->rgvarg[0].vt == VT_I4)
						i32 = pDispParams->rgvarg[0].lVal;
					else if (pDispParams->rgvarg[0].vt == VT_I2)
						i32 = pDispParams->rgvarg[0].iVal;
					else
						return E_FAIL;

					return set1Value(index, i32);
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
