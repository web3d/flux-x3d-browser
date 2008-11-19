/********************************************************************************
 * Flux
 *
 * File: X3DMFVec2f.cpp
 * Description: Implementation of CX3DMFVec2f
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

// X3DMFVec2f.cpp : Implementation of CX3DMFVec2f
#include "stdafx.h"
#include "Anima.h"
#include "X3DMFVec2f.h"
#include "X3DSFVec2f.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DMFVec2f
STDMETHODIMP CX3DMFVec2f::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_mfvec2fvalue);

	if (m_mfvec2fvalue)
		m_mfvec2fvalue->Ref();
	else
		m_mfvec2fvalue = new Point2Array;

	return S_OK;
}

STDMETHODIMP CX3DMFVec2f::getValue(int cnt, float *value)
{
	Point2 p2;

	Point2Array *p2a = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &p2a);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	else if (setval == ANMFIELDBASE_DIY)
		p2a = m_mfvec2fvalue;

	for (int i = 0; i < p2a->size(); i++)
	{
		// krv:
		// cnt should be the number of Rotations, no the humber of floats.
		if (i >= cnt)
			break;

		p2 = (*p2a)[i];

		value[i * 2] = p2.x;
		value[i * 2 + 1] = p2.y;
	}
	
	return S_OK;
}

STDMETHODIMP CX3DMFVec2f::setValue(int cnt, float *value)
{
	float f;
	Point2 p2;

	// krv:
	// cnt should be the number of Rotations, no the humber of floats.
	cnt *= 2;

	SafeUnRef(m_mfvec2fvalue);
	m_mfvec2fvalue = new Point2Array;

	for (int i = 0; i < cnt; i++)
	{
		int mod = i % 2;
		f = value[i];

		if (mod == 0)
			p2.x = f;
		else if (mod == 1)
			p2.y = f;

		if (mod == 1)
		{
			m_mfvec2fvalue->push_back(p2);
			p2 = Point2(0, 0);
		}
	}

	if (m_mfvec2fvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfvec2fvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CX3DMFVec2f::set1Value(int index, X3DSFVec2f *value)
{
	if (value == NULL)
		return E_INVALIDARG;

	Point2 val;
	value->getValue((float *) &val);

	assert(m_mfvec2fvalue);

	int sz = m_mfvec2fvalue->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			static const Point2 p2(0, 0);
			m_mfvec2fvalue->push_back(p2);
			sz++;
		}
		m_mfvec2fvalue->push_back(val);
	}
	else
		(*m_mfvec2fvalue)[index] = val;

	if (m_mfvec2fvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfvec2fvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CX3DMFVec2f::get1Value(int index, X3DSFVec2f **value)
{
	Point2Array *p2a = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &p2a);

	if (setval == ANMFIELDBASE_CANTDO)
		return E_FAIL;
	else if (setval == ANMFIELDBASE_DIY)
		p2a = m_mfvec2fvalue;

	if (index < 0 || index >= p2a->size())
		return E_FAIL;

	Point2 p2 = (*p2a)[index];

	X3DSFVec2f *pX3DSFVec2f = CX3DSFVec2f::Create(m_context, p2);
	if (pX3DSFVec2f)
	{
		*value = pX3DSFVec2f;
		return S_OK;
	}
	else
		return E_FAIL;
}

STDMETHODIMP CX3DMFVec2f::get_Count(long *pVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{

		// krv:
		// Count should be the number of Rotations, no the humber of floats.

		*pVal = m_mfvec2fvalue->size();
		return S_OK;
	}

	Point2Array *p2a = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &p2a);
	assert(p2a != NULL);

	*pVal = p2a->size();

	return S_OK;
}

STDMETHODIMP CX3DMFVec2f::Item(VARIANT index, VARIANT *pItem)
{
	if (index.vt == VT_I2 || index.vt == VT_I4)
	{
		X3DSFVec2f *pX3DSFVec2f = NULL;

		int ind = (index.vt == VT_I2) ? index.iVal : index.lVal;

		HRESULT hr = get1Value(ind, &pX3DSFVec2f);

		if (SUCCEEDED(hr))
		{
			pItem->vt = VT_DISPATCH;
			pItem->pdispVal = pX3DSFVec2f;
			return S_OK;
		}
		else
			return hr;
	}

	return E_INVALIDARG;
}


void CX3DMFVec2f::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	Point2Array *p2a = *(Point2Array **) callData;

	assert(p2a != NULL);

	SafeUnRef(m_mfvec2fvalue);
	m_mfvec2fvalue = p2a;
	m_mfvec2fvalue->Ref();

	X3DFieldBase::callCallbacks(this);
}

STDMETHODIMP CX3DMFVec2f::get_length(long *pVal)
{
	return get_Count(pVal);
}

STDMETHODIMP CX3DMFVec2f::put_length(long newVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		m_mfvec2fvalue->resize(newVal);
		return S_OK;
	}

	Point2Array *p2a = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &p2a);
	assert(p2a != NULL);

	p2a->resize(newVal);

	return S_OK;
}

// IDispatch overrides
#define IDOFFSET 32        

STDMETHODIMP CX3DMFVec2f::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DMFVec2f, &IID_X3DMFVec2f, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DMFVec2f::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DMFVec2f, &IID_X3DMFVec2f, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}

#define IDOFFSET 32
        
STDMETHODIMP CX3DMFVec2f::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DMFVec2f, &IID_X3DMFVec2f, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

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
        
STDMETHODIMP CX3DMFVec2f::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DMFVec2f, &IID_X3DMFVec2f, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
		wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	if (!SUCCEEDED(hr))
	{
		int index = dispIdMember - IDOFFSET;

		if (index >= 0)
		{
			X3DSFVec2f *pSFVec2f;

			if (wFlags == DISPATCH_PROPERTYGET)
			{
				hr = get1Value(index, &pSFVec2f);

				if (SUCCEEDED(hr))
				{
					assert(pVarResult);
					pVarResult->vt = VT_DISPATCH;
					pVarResult->pdispVal = pSFVec2f;

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

					hr = pDispatch->QueryInterface(IID_X3DSFVec2f, (void **) &pSFVec2f);
					if (SUCCEEDED(hr))
					{
						hr = set1Value(index, pSFVec2f);

						pSFVec2f->Release();

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
