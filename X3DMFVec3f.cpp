/********************************************************************************
 * Flux
 *
 * File: X3DMFVec3f.cpp
 * Description: Implementation of CX3DMFVec3f
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

// X3DMFVec3f.cpp : Implementation of CX3DMFVec3f
#include "stdafx.h"
#include "Anima.h"
#include "X3DMFVec3f.h"
#include "X3DSFVec3f.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DMFVec3f
STDMETHODIMP CX3DMFVec3f::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_mfvec3fvalue);

	if (m_mfvec3fvalue)
		m_mfvec3fvalue->Ref();
	else
		m_mfvec3fvalue = new Point3Array;

	return S_OK;
}

STDMETHODIMP CX3DMFVec3f::getValue(int cnt, float *value)
{
	Point3 p3;

	Point3Array *p3a = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &p3a);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	else if (setval == ANMFIELDBASE_DIY)
		p3a = m_mfvec3fvalue;

	for (int i = 0; i < p3a->size(); i++)
	{
		// krv:  Should be Vec3 count, not float count
		//
//		if (i * 3 >= cnt)
		if (i >= cnt)
			break;

		p3 = (*p3a)[i];

		value[i * 3] = p3.x;
		value[i * 3 + 1] = p3.y;
		value[i * 3 + 2] = p3.z;
	}
	
	return S_OK;
}

STDMETHODIMP CX3DMFVec3f::setValue(int cnt, float *value)
{
	float f;
	Point3 p3;

	SafeUnRef(m_mfvec3fvalue);
	m_mfvec3fvalue = new Point3Array;

	// krv:  Should be Vec3 count, not float count
	//
	cnt *= 3;

	for (int i = 0; i < cnt; i++)
	{
		int mod = i % 3;
		f = value[i];

		if (mod == 0)
			p3.x = f;
		else if (mod == 1)
			p3.y = f;
		else if (mod == 2)
			p3.z = f;

		if (mod == 2)
		{
			m_mfvec3fvalue->push_back(p3);
			p3 = Point3(0, 0, 0);
		}
	}

	if (m_mfvec3fvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfvec3fvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}


STDMETHODIMP CX3DMFVec3f::set1Value(int index, X3DSFVec3f *value)
{
	if (value == NULL)
		return E_INVALIDARG;

	Point3 val;
	value->getValue((float *) &val);

	assert(m_mfvec3fvalue);

	int sz = m_mfvec3fvalue->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			static const Point3 p3(0, 0, 0);
			m_mfvec3fvalue->push_back(p3);
			sz++;
		}
		m_mfvec3fvalue->push_back(val);
	}
	else
		(*m_mfvec3fvalue)[index] = val;

	if (m_mfvec3fvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfvec3fvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CX3DMFVec3f::get1Value(int index, X3DSFVec3f **value)
{
	Point3Array *p3a = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &p3a);

	if (setval == ANMFIELDBASE_CANTDO)
		return E_FAIL;
	else if (setval == ANMFIELDBASE_DIY)
		p3a = m_mfvec3fvalue;

	if (index < 0 || index >= p3a->size())
		return E_FAIL;

	Point3 p3 = (*p3a)[index];

	X3DSFVec3f *pX3DSFVec3f = CX3DSFVec3f::Create(m_context, p3);
	if (pX3DSFVec3f)
	{
		*value = pX3DSFVec3f;
		return S_OK;
	}
	else
		return E_FAIL;
}

STDMETHODIMP CX3DMFVec3f::get_Count(long *pVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		// krv:
		//THis should NOT return 3 times the number of floats!
		//*pVal = m_mfvec3fvalue->size() * 3;
		*pVal = m_mfvec3fvalue->size();
		return S_OK;
	}

	Point3Array *p3a = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &p3a);
	assert(p3a != NULL);

	// krv:
	//THis should NOT return 3 times the number of floats!
//	*pVal = p3a->size() * 3;
	*pVal = p3a->size();

	return S_OK;
}

STDMETHODIMP CX3DMFVec3f::Item(VARIANT index, VARIANT *pItem)
{
	if (index.vt == VT_I2 || index.vt == VT_I4)
	{
		X3DSFVec3f *pX3DSFVec3f = NULL;

		int ind = (index.vt == VT_I2) ? index.iVal : index.lVal;

		HRESULT hr = get1Value(ind, &pX3DSFVec3f);

		if (SUCCEEDED(hr))
		{
			pItem->vt = VT_DISPATCH;
			pItem->pdispVal = pX3DSFVec3f;
			return S_OK;
		}
		else
			return hr;
	}

	return E_INVALIDARG;
}


void CX3DMFVec3f::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	Point3Array *p3a = *(Point3Array **) callData;

	assert(p3a != NULL);

	SafeUnRef(m_mfvec3fvalue);
	m_mfvec3fvalue = p3a;
	m_mfvec3fvalue->Ref();

	X3DFieldBase::callCallbacks(this);
}

STDMETHODIMP CX3DMFVec3f::get_length(long *pVal)
{
	return get_Count(pVal);
}

STDMETHODIMP CX3DMFVec3f::put_length(long newVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		m_mfvec3fvalue->resize(newVal);
		return S_OK;
	}

	Point3Array *p3a = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &p3a);
	assert(p3a != NULL);

	p3a->resize(newVal);

	return S_OK;
}

// IDispatch overrides
#define IDOFFSET 32        

STDMETHODIMP CX3DMFVec3f::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DMFVec3f, &IID_X3DMFVec3f, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DMFVec3f::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DMFVec3f, &IID_X3DMFVec3f, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}
        
STDMETHODIMP CX3DMFVec3f::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DMFVec3f, &IID_X3DMFVec3f, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

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
        
STDMETHODIMP CX3DMFVec3f::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DMFVec3f, &IID_X3DMFVec3f, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
		wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	if (!SUCCEEDED(hr))
	{
		int index = dispIdMember - IDOFFSET;

		if (index >= 0)
		{
			X3DSFVec3f *pSFVec3f;

			if (wFlags == DISPATCH_PROPERTYGET)
			{
				hr = get1Value(index, &pSFVec3f);

				if (SUCCEEDED(hr))
				{
					assert(pVarResult);
					pVarResult->vt = VT_DISPATCH;
					pVarResult->pdispVal = pSFVec3f;

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

					hr = pDispatch->QueryInterface(IID_X3DSFVec3f, (void **) &pSFVec3f);
					if (SUCCEEDED(hr))
					{
						hr = set1Value(index, pSFVec3f);

						pSFVec3f->Release();

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
