/********************************************************************************
 * Flux
 *
 * File: X3DMFString.cpp
 * Description: Implementation of CX3DMFString
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

// X3DMFString.cpp : Implementation of CX3DMFString
#include "stdafx.h"
#include "Anima.h"
#include "X3DMFString.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DMFString

STDMETHODIMP CX3DMFString::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_mfstringvalue);

	if (m_mfstringvalue)
		m_mfstringvalue->Ref();
	else
		m_mfstringvalue = new StringArray;

	return S_OK;
}

STDMETHODIMP CX3DMFString::getValue(int cnt, BSTR *value)
{
	StringArray *psa = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &psa);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	else if (setval == ANMFIELDBASE_DIY)
		psa = m_mfstringvalue;

	for (int i = 0; i < psa->size(); i++)
	{
		if (i >= cnt)
			break;

		CComBSTR bstr = (*psa)[i]->GetBuf();		//N.B.: whose heap?
		value[i] = bstr.Copy();
	}
	
	return S_OK;
}

STDMETHODIMP CX3DMFString::setValue(int cnt, BSTR *value)
{
	SafeUnRef(m_mfstringvalue);
	m_mfstringvalue = new StringArray;

	for (int i = 0; i < cnt; i++)
	{
		int len = wcslen(value[i]);
		String s = new CAnmString;
		s->alloc(len + 1);
		wcstombs(s->GetBuf(), value[i], len + 1);

		m_mfstringvalue->push_back(s);
	}

	if (m_mfstringvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfstringvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}


STDMETHODIMP CX3DMFString::set1Value(int index, BSTR value)
{
	assert(m_mfstringvalue);

	String s;

	if (value != NULL)
	{
		int len = wcslen(value);
		s = new CAnmString;
		s->alloc(len + 1);
		wcstombs(s->GetBuf(), value, len + 1);
	}
	else
		s = new CAnmString("");

	int sz = m_mfstringvalue->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			m_mfstringvalue->push_back(new CAnmString(""));
			sz++;
		}
		m_mfstringvalue->push_back(s);
	}
	else
		(*m_mfstringvalue)[index] = s;

	if (m_mfstringvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfstringvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}


STDMETHODIMP CX3DMFString::get1Value(int index, BSTR *value)
{
	StringArray *psa = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &psa);

	if (setval == ANMFIELDBASE_CANTDO)
		return E_FAIL;
	else if (setval == ANMFIELDBASE_DIY)
		psa = m_mfstringvalue;

	if (index < 0 || index >= psa->size())
		return E_FAIL;

	String s = (*psa)[index];
	CComBSTR bstr = s->GetBuf();
	*value = bstr.Copy();

	return S_OK;
}

STDMETHODIMP CX3DMFString::get_Count(long *pVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		*pVal = m_mfstringvalue->size();
		return S_OK;
	}

	StringArray *psa = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &psa);
	assert(psa != NULL);

	*pVal = psa->size();

	return S_OK;
}

STDMETHODIMP CX3DMFString::Item(VARIANT index, VARIANT *pItem)
{
	if (index.vt == VT_I2 || index.vt == VT_I4)
	{
		BSTR strval = NULL;

		int ind = (index.vt == VT_I2) ? index.iVal : index.lVal;

		HRESULT hr = get1Value(ind, &strval);

		if (SUCCEEDED(hr))
		{
			pItem->vt = VT_BSTR;
			pItem->bstrVal = strval;
			return S_OK;
		}
		else
			return hr;
	}

	return E_INVALIDARG;
}


void CX3DMFString::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	StringArray *psa = *(StringArray **) callData;

	assert(psa != NULL);

	SafeUnRef(m_mfstringvalue);
	m_mfstringvalue = psa;
	m_mfstringvalue->Ref();

	X3DFieldBase::callCallbacks(this);
}


STDMETHODIMP CX3DMFString::get_length(long *pVal)
{
	return get_Count(pVal);
}

STDMETHODIMP CX3DMFString::put_length(long newVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		m_mfstringvalue->resize(newVal);
		return S_OK;
	}

	StringArray *psa = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &psa);
	assert(psa != NULL);

	psa->resize(newVal);

	return S_OK;
}

STDMETHODIMP CX3DMFString::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DMFString, &IID_X3DMFString, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DMFString::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DMFString, &IID_X3DMFString, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}

// IDispatch overrides
#define IDOFFSET 32        
        
STDMETHODIMP CX3DMFString::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DMFString, &IID_X3DMFString, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

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
        
STDMETHODIMP CX3DMFString::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DMFString, &IID_X3DMFString, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
		wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	if (!SUCCEEDED(hr))
	{
		int index = dispIdMember - IDOFFSET;

		if (index >= 0)
		{
			BSTR bstr;

			if (wFlags == DISPATCH_PROPERTYGET)
			{
				hr = get1Value(index, &bstr);

				if (SUCCEEDED(hr))
				{
					assert(pVarResult);
					pVarResult->vt = VT_BSTR;
					pVarResult->bstrVal = bstr;

					return S_OK;
				}
				else
					return S_FALSE;
			}
			else if (wFlags == DISPATCH_PROPERTYPUT)
			{
				assert(pDispParams);

				if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
				{
					bstr = pDispParams->rgvarg[0].bstrVal;
					return set1Value(index, bstr);
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

