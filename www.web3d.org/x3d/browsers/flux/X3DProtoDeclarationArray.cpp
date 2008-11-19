/********************************************************************************
 * Flux
 *
 * File: X3DProtoDeclarationArray.cpp
 * Description: Implementation of CX3DProtoDeclarationArray
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

// X3DProtoDeclarationArray.cpp : Implementation of CX3DProtoDeclarationArray
#include "stdafx.h"
#include "Anima.h"
#include "X3DProtoDeclarationArray.h"
#include "X3DProtoDeclaration.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DProtoDeclarationArray


X3DProtoDeclarationArray *CX3DProtoDeclarationArray::Create(int nProtoDecls, class CAnmProtoSymbol **ppProtoSymbols)
{
	assert(nProtoDecls > 0);
	assert(ppProtoSymbols);

	X3DProtoDeclarationArray *pProtoDeclarationArray = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CoX3DProtoDeclarationArray, NULL, CLSCTX_INPROC_SERVER, IID_X3DProtoDeclarationArray,
		(LPVOID*)&pProtoDeclarationArray);

	if (FAILED(hr))
		return NULL;

	pProtoDeclarationArray->put_Count(nProtoDecls);

	for (int i = 0; i < nProtoDecls; i++)
	{
		CAnmProtoSymbol *pProtoSym = ppProtoSymbols[i];
		X3DProtoDeclaration *pProtoDeclaration = CX3DProtoDeclaration::Create(pProtoSym);
		if (pProtoDeclaration == NULL)
			return NULL;

		pProtoDeclarationArray->set1Value(i, pProtoDeclaration);
	}

	return pProtoDeclarationArray;
}

CX3DProtoDeclarationArray::~CX3DProtoDeclarationArray()
{
	int sz = m_declarations.size();

	for (int i = 0; i < sz; i++)
		SafeRelease(m_declarations[i]);
}

STDMETHODIMP CX3DProtoDeclarationArray::get_Count(long *pVal)
{
	if (pVal == NULL)
		return E_INVALIDARG;

	*pVal = m_declarations.size();
	return S_OK;
}

STDMETHODIMP CX3DProtoDeclarationArray::put_Count(long val)
{
	if (val <= 0)
		return E_INVALIDARG;

	m_declarations.reserve(val);
	int sz = m_declarations.size();

	for (int i = sz; i < val; i++)
		m_declarations.push_back(NULL);

	return S_OK;
}

STDMETHODIMP CX3DProtoDeclarationArray::Item(VARIANT index, VARIANT *pItem)
{
	if (index.vt == VT_I2 || index.vt == VT_I4)
	{
		X3DProtoDeclaration *pFD;

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

STDMETHODIMP CX3DProtoDeclarationArray::get1Value(int index, X3DProtoDeclaration **value)
{
	int sz = m_declarations.size();
	if (index < 0 || index >= sz)
		return E_INVALIDARG;

	*value = m_declarations[index];
	(*value)->AddRef();

	return S_OK;
}

STDMETHODIMP CX3DProtoDeclarationArray::set1Value(long index, X3DProtoDeclaration *value)
{
	int sz = m_declarations.size();

	if (index < 0 || !value)
		return E_INVALIDARG;

	// let's keep this as a sanity check for automation clients - also adds a ref! :-)
	X3DProtoDeclaration *pProtoDecl = NULL;
	HRESULT hr = value->QueryInterface(IID_X3DProtoDeclaration, (void **) &pProtoDecl);
	if (SUCCEEDED(hr))
	{
		if (index >= sz)
		{
			while (index > sz)
			{
				m_declarations.push_back(NULL);
				sz++;
			}
			
			m_declarations.push_back(value);
		}
		else
		{
			SafeRelease(m_declarations[index]);
			m_declarations[index] = value;
		}
	}

	return S_OK;
}

STDMETHODIMP CX3DProtoDeclarationArray::get_length(long *pVal)
{
	return get_Count(pVal);
}

STDMETHODIMP CX3DProtoDeclarationArray::put_length(long newVal)
{
	return put_Count(newVal);
}


// IDispatch overrides
#define IDOFFSET 32
        
STDMETHODIMP CX3DProtoDeclarationArray::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DProtoDeclarationArray, &IID_X3DProtoDeclarationArray, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DProtoDeclarationArray::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DProtoDeclarationArray, &IID_X3DProtoDeclarationArray, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}

STDMETHODIMP CX3DProtoDeclarationArray::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DProtoDeclarationArray, &IID_X3DProtoDeclarationArray, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

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
        
STDMETHODIMP CX3DProtoDeclarationArray::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DProtoDeclarationArray, &IID_X3DProtoDeclarationArray, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
		wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	if (!SUCCEEDED(hr))
	{
		int index = dispIdMember - IDOFFSET;

		if (index >= 0)
		{
			X3DProtoDeclaration *pProtoDecl;

			if (wFlags == DISPATCH_PROPERTYGET)
			{
				hr = get1Value(index, &pProtoDecl);

				if (SUCCEEDED(hr))
				{
					assert(pVarResult);
					pVarResult->vt = VT_DISPATCH;
					pVarResult->pdispVal = pProtoDecl;

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

					hr = pDispatch->QueryInterface(IID_X3DProtoDeclaration, (void **) &pProtoDecl);
					if (SUCCEEDED(hr))
					{
						hr = set1Value(index, pProtoDecl);

						pProtoDecl->Release();

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
