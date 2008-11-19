/********************************************************************************
 * Flux
 *
 * File: X3DFieldDefinitionArray.cpp
 * Description: Implementation of CX3DFieldDefinitionArray
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

// X3DFieldDefinitionArray.cpp : Implementation of CX3DFieldDefinitionArray
#include "stdafx.h"
#include "Anima.h"
#include "X3DFieldDefinitionArray.h"
#include "X3DFieldDefinition.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DFieldDefinitionArray

CX3DFieldDefinitionArray::~CX3DFieldDefinitionArray()
{
	int sz = m_definitions.size();

	for (int i = 0; i < sz; i++)
		SafeRelease(m_definitions[i]);
}

STDMETHODIMP CX3DFieldDefinitionArray::get_Count(long *pVal)
{
	if (pVal == NULL)
		return E_INVALIDARG;

	*pVal = m_definitions.size();
	return S_OK;
}

STDMETHODIMP CX3DFieldDefinitionArray::put_Count(long val)
{
	if (val <= 0)
		return E_INVALIDARG;

	m_definitions.reserve(val);
	int sz = m_definitions.size();

	for (int i = sz; i < val; i++)
		m_definitions.push_back(NULL);

	return S_OK;
}

STDMETHODIMP CX3DFieldDefinitionArray::Item(VARIANT index, VARIANT *pItem)
{
	if (index.vt == VT_I2 || index.vt == VT_I4)
	{
		X3DFieldDefinition *pFD;

		int ind = (index.vt == VT_I2) ? index.iVal : index.lVal;

		HRESULT hr = get1Value(ind, &pFD);

		if (SUCCEEDED(hr))
		{
			pItem->vt = VT_DISPATCH;
			pItem->pdispVal = pFD;
			return S_OK;
		}
		else
			return hr;

		return S_OK;
	}

	return E_INVALIDARG;
}

STDMETHODIMP CX3DFieldDefinitionArray::get1Value(int index, X3DFieldDefinition **value)
{
	int sz = m_definitions.size();
	if (index < 0 || index >= sz)
		return E_INVALIDARG;

	*value = m_definitions[index];
	(*value)->AddRef();

	return S_OK;
}

STDMETHODIMP CX3DFieldDefinitionArray::set1Value(long index, X3DFieldDefinition *value)
{
	int sz = m_definitions.size();

	if (index < 0 || !value)
		return E_INVALIDARG;

	// let's keep this as a sanity check for automation clients - also adds a ref! :-)
	X3DFieldDefinition *pFieldDef = NULL;
	HRESULT hr = value->QueryInterface(IID_X3DFieldDefinition, (void **) &pFieldDef);
	if (SUCCEEDED(hr))
	{
		if (index >= sz)
		{
			while (index > sz)
			{
				m_definitions.push_back(NULL);
				sz++;
			}
			
			m_definitions.push_back(value);
		}
		else
		{
			SafeRelease(m_definitions[index]);
			m_definitions[index] = value;
		}
	}

	return S_OK;
}

STDMETHODIMP CX3DFieldDefinitionArray::get_length(long *pVal)
{
	return get_Count(pVal);
}

STDMETHODIMP CX3DFieldDefinitionArray::put_length(long newVal)
{
	return put_Count(newVal);
}

// IDispatch overrides
#define IDOFFSET 32
        
STDMETHODIMP CX3DFieldDefinitionArray::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DFieldDefinitionArray, &IID_X3DFieldDefinitionArray, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DFieldDefinitionArray::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DFieldDefinitionArray, &IID_X3DFieldDefinitionArray, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}

STDMETHODIMP CX3DFieldDefinitionArray::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DFieldDefinitionArray, &IID_X3DFieldDefinitionArray, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

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
        
STDMETHODIMP CX3DFieldDefinitionArray::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DFieldDefinitionArray, &IID_X3DFieldDefinitionArray, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
		wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	if (!SUCCEEDED(hr))
	{
		int index = dispIdMember - IDOFFSET;

		if (index >= 0)
		{
			X3DFieldDefinition *pFieldDef;

			if (wFlags == DISPATCH_PROPERTYGET)
			{
				hr = get1Value(index, &pFieldDef);

				if (SUCCEEDED(hr))
				{
					assert(pVarResult);
					pVarResult->vt = VT_DISPATCH;
					pVarResult->pdispVal = pFieldDef;

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

					hr = pDispatch->QueryInterface(IID_X3DFieldDefinition, (void **) &pFieldDef);
					if (SUCCEEDED(hr))
					{
						hr = set1Value(index, pFieldDef);

						pFieldDef->Release();

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