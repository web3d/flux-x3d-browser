/********************************************************************************
 * Flux
 *
 * File: X3DMFRotation.cpp
 * Description: Implementation of CX3DMFRotation
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

// X3DMFRotation.cpp : Implementation of CX3DMFRotation
#include "stdafx.h"
#include "Anima.h"
#include "X3DMFRotation.h"
#include "X3DSFRotation.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DMFRotation

STDMETHODIMP CX3DMFRotation::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_mfrotationvalue);

	if (m_mfrotationvalue)
		m_mfrotationvalue->Ref();
	else
		m_mfrotationvalue = new RotationArray;

	return S_OK;
}

STDMETHODIMP CX3DMFRotation::getValue(int cnt, float *value)
{
	Rotation r;
	
	RotationArray *pra = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &pra);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	else if (setval == ANMFIELDBASE_DIY)
		pra = m_mfrotationvalue;

	for (int i = 0; i < pra->size(); i++)
	{
		// krv:
		// cnt should be the number of Rotations, no the humber of floats.
		// if (i * 4 >= cnt)
		if (i >= cnt)
			break;

		r = (*pra)[i];

		value[i * 4] = r.vec.x;
		value[i * 4 + 1] = r.vec.y;
		value[i * 4 + 2] = r.vec.z;
		value[i * 4 + 3] = r.theta;
	}
	
	return S_OK;
}

STDMETHODIMP CX3DMFRotation::setValue(int cnt, float *value)
{
	float f;
	Rotation r;

	SafeUnRef(m_mfrotationvalue);
	m_mfrotationvalue = new RotationArray;

	// krv:
	// cnt should be the number of Rotations, no the humber of floats.
	cnt *= 4;

	for (int i = 0; i < cnt; i++)
	{
		int mod = i % 4;
		f = value[i];

		if (mod == 0)
			r.vec.x = f;
		else if (mod == 1)
			r.vec.y = f;
		else if (mod == 2)
			r.vec.z = f;
		else if (mod == 3)
			r.theta = f;

		if (mod == 3)
		{
			m_mfrotationvalue->push_back(r);
			r = Rotation(0, 1, 0, 0);
		}
	}

	if (m_mfrotationvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfrotationvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CX3DMFRotation::set1Value(int index, X3DSFRotation *value)
{
	if (value == NULL)
		return E_INVALIDARG;

	Rotation val;
	value->getValue((float *) &val);

	assert(m_mfrotationvalue);

	int sz = m_mfrotationvalue->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			static const Rotation r(0, 1, 0, 0);
			m_mfrotationvalue->push_back(r);
			sz++;
		}
		m_mfrotationvalue->push_back(val);
	}
	else
		(*m_mfrotationvalue)[index] = val;

	if (m_mfrotationvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfrotationvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}


STDMETHODIMP CX3DMFRotation::get1Value(int index, X3DSFRotation **value)
{
	RotationArray *pra = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &pra);

	if (setval == ANMFIELDBASE_CANTDO)
		return E_FAIL;
	else if (setval == ANMFIELDBASE_DIY)
		pra = m_mfrotationvalue;

	if (index < 0 || index >= pra->size())
		return E_FAIL;

	Rotation r = (*pra)[index];

	X3DSFRotation *pX3DSFRotation = CX3DSFRotation::Create(m_context, r);
	if (pX3DSFRotation)
	{
		*value = pX3DSFRotation;
		return S_OK;
	}
	else
		return E_FAIL;
}

STDMETHODIMP CX3DMFRotation::get_Count(long *pVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
	// krv:
	// count should be the number of Rotations, no the humber of floats.
		*pVal = m_mfrotationvalue->size();
		return S_OK;
	}

	RotationArray *pra = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &pra);
	assert(pra != NULL);

	*pVal = pra->size();

	return S_OK;
}

STDMETHODIMP CX3DMFRotation::Item(VARIANT index, VARIANT *pItem)
{
	if (index.vt == VT_I2 || index.vt == VT_I4)
	{
		X3DSFRotation *pX3DSFRotation = NULL;

		int ind = (index.vt == VT_I2) ? index.iVal : index.lVal;

		HRESULT hr = get1Value(ind, &pX3DSFRotation);

		if (SUCCEEDED(hr))
		{
			pItem->vt = VT_DISPATCH;
			pItem->pdispVal = pX3DSFRotation;
			return S_OK;
		}
		else
			return hr;
	}

	return E_INVALIDARG;
}


void CX3DMFRotation::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	RotationArray *pra = *(RotationArray **) callData;

	assert(pra != NULL);

	SafeUnRef(m_mfrotationvalue);
	m_mfrotationvalue = pra;
	m_mfrotationvalue->Ref();

	X3DFieldBase::callCallbacks(this);
}


STDMETHODIMP CX3DMFRotation::get_length(long *pVal)
{
	return get_Count(pVal);
}

STDMETHODIMP CX3DMFRotation::put_length(long newVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		m_mfrotationvalue->resize(newVal);
		return S_OK;
	}

	RotationArray *pra = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &pra);
	assert(pra != NULL);

	pra->resize(newVal);

	return S_OK;
}

// IDispatch overrides
#define IDOFFSET 32        

STDMETHODIMP CX3DMFRotation::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DMFRotation, &IID_X3DMFRotation, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DMFRotation::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DMFRotation, &IID_X3DMFRotation, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}
        
STDMETHODIMP CX3DMFRotation::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DMFRotation, &IID_X3DMFRotation, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

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
        
STDMETHODIMP CX3DMFRotation::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DMFRotation, &IID_X3DMFRotation, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
		wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	if (!SUCCEEDED(hr))
	{
		int index = dispIdMember - IDOFFSET;

		if (index >= 0)
		{
			X3DSFRotation *pSFRotation;

			if (wFlags == DISPATCH_PROPERTYGET)
			{
				hr = get1Value(index, &pSFRotation);

				if (SUCCEEDED(hr))
				{
					assert(pVarResult);
					pVarResult->vt = VT_DISPATCH;
					pVarResult->pdispVal = pSFRotation;

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

					hr = pDispatch->QueryInterface(IID_X3DSFRotation, (void **) &pSFRotation);
					if (SUCCEEDED(hr))
					{
						hr = set1Value(index, pSFRotation);

						pSFRotation->Release();

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
