/********************************************************************************
 * Flux
 *
 * File: X3DMFColor.cpp
 * Description: Implementation of CX3DMFColor
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

// X3DMFColor.cpp : Implementation of CX3DMFColor
#include "stdafx.h"
#include "Anima.h"
#include "X3DMFColor.h"
#include "X3DSFColor.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DMFColor

STDMETHODIMP CX3DMFColor::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_mfcolorvalue);

	if (m_mfcolorvalue)
		m_mfcolorvalue->Ref();
	else
		m_mfcolorvalue = new ColorArray;

	return S_OK;
}


STDMETHODIMP CX3DMFColor::getValue(int cnt, float *value)
{
	ColorArray *pca = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &pca);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	else if (setval == ANMFIELDBASE_DIY)
		pca = m_mfcolorvalue;

	Color c;
	for (int i = 0; i < pca->size(); i++)
	{
		// krv: cnt is the Color Count, not the float count
		// if (i * 3 >= cnt)
		if (i >= cnt)
			break;

		c = (*pca)[i];

		value[i * 3] = c.r;
		value[i * 3 + 1] = c.g;
		value[i * 3 + 2] = c.b;
	}
	
	return S_OK;
}

STDMETHODIMP CX3DMFColor::setValue(int cnt, float *value)
{
	float f;

	Color c;
	SafeUnRef(m_mfcolorvalue);
	m_mfcolorvalue = new ColorArray;

	// krv: cnt is the Color Count, not the float count
	cnt *= 3;

	for (int i = 0; i < cnt; i++)
	{
		int mod = i % 3;
		f = value[i];

		if (mod == 0)
			c.r = f;
		else if (mod == 1)
			c.g = f;
		else if (mod == 2)
			c.b = f;

		if (mod == 2)
		{
			m_mfcolorvalue->push_back(c);
			c = Color(0, 0, 0);
		}
	}

	if (m_mfcolorvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfcolorvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CX3DMFColor::set1Value(int index, X3DSFColor *value)
{
	if (value == NULL)
		return E_INVALIDARG;

	Color val;
	value->getValue((float *) &val);

	assert(m_mfcolorvalue);

	int sz = m_mfcolorvalue->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			static const Color c(0, 0, 0);
			m_mfcolorvalue->push_back(c);
			sz++;
		}
		m_mfcolorvalue->push_back(val);
	}
	else
		(*m_mfcolorvalue)[index] = val;

	if (m_mfcolorvalue->size() >= 0)
	{
		if (X3DFieldBase::setValue((void *) &m_mfcolorvalue) != ANMFIELDBASE_CANTDO)
			return S_OK;
		else
			return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CX3DMFColor::get1Value(int index, X3DSFColor **value)
{
	ColorArray *pca = NULL;

	eAnmFieldBaseSetValue setval = X3DFieldBase::getValue((void **) &pca);

	if (setval == ANMFIELDBASE_CANTDO)
		return E_FAIL;
	else if (setval == ANMFIELDBASE_DIY)
		pca = m_mfcolorvalue;

	if (index < 0 || index >= pca->size())
		return E_FAIL;

	Color c = (*pca)[index];

	X3DSFColor *pX3DSFColor = CX3DSFColor::Create(m_context, c);
	if (pX3DSFColor)
	{
		*value = pX3DSFColor;
		return S_OK;
	}
	else
		return E_FAIL;
}

STDMETHODIMP CX3DMFColor::get_Count(long *pVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		*pVal = m_mfcolorvalue->size();
		return S_OK;
	}

	ColorArray *pca = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &pca);
	assert(pca != NULL);

	*pVal = pca->size();

	return S_OK;
}

STDMETHODIMP CX3DMFColor::Item(VARIANT index, VARIANT *pItem)
{
	if (index.vt == VT_I2 || index.vt == VT_I4)
	{
		X3DSFColor *pX3DSFColor = NULL;

		int ind = (index.vt == VT_I2) ? index.iVal : index.lVal;

		HRESULT hr = get1Value(ind, &pX3DSFColor);

		if (SUCCEEDED(hr))
		{
			pItem->vt = VT_DISPATCH;
			pItem->pdispVal = pX3DSFColor;
			return S_OK;
		}
		else
			return hr;
	}

	return E_INVALIDARG;
}


void CX3DMFColor::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	ColorArray *pca = *(ColorArray **) callData;

	assert(pca != NULL);

	SafeUnRef(m_mfcolorvalue);
	m_mfcolorvalue = pca;
	m_mfcolorvalue->Ref();

	X3DFieldBase::callCallbacks(this);
}


STDMETHODIMP CX3DMFColor::get_length(long *pVal)
{
	return get_Count(pVal);
}

STDMETHODIMP CX3DMFColor::put_length(long newVal)
{
	if (m_mid->memberType == eAnmCallbackMember)
	{
		m_mfcolorvalue->resize(newVal);
		return S_OK;
	}

	ColorArray *pca = NULL;
	CAnmNode *pNode = m_node->GetNode();
	pNode->GetValue(m_mid, (void **) &pca);
	assert(pca != NULL);

	pca->resize(newVal);

	return S_OK;
}

// IDispatch overrides
#define IDOFFSET 32        

STDMETHODIMP CX3DMFColor::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DMFColor, &IID_X3DMFColor, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DMFColor::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DMFColor, &IID_X3DMFColor, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}

STDMETHODIMP CX3DMFColor::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DMFColor, &IID_X3DMFColor, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

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
        
STDMETHODIMP CX3DMFColor::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DMFColor, &IID_X3DMFColor, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
		wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	if (!SUCCEEDED(hr))
	{
		int index = dispIdMember - IDOFFSET;

		if (index >= 0)
		{
			X3DSFColor *pSFColor;

			if (wFlags == DISPATCH_PROPERTYGET)
			{
				hr = get1Value(index, &pSFColor);

				if (SUCCEEDED(hr))
				{
					assert(pVarResult);
					pVarResult->vt = VT_DISPATCH;
					pVarResult->pdispVal = pSFColor;

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

					hr = pDispatch->QueryInterface(IID_X3DSFColor, (void **) &pSFColor);
					if (SUCCEEDED(hr))
					{
						hr = set1Value(index, pSFColor);

						pSFColor->Release();

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

#if 0

// These are for reference only; we figured out how to deal with VARIANT SafeArrays and IDispatch
// arrays for JScript. We'll use this code if we decide to surface VARIANT versions of get/setValue for
// MF- fields

STDMETHODIMP CX3DMFColor::setVValue(VARIANT v)
{
	// TODO: Add your implementation code here

	if (v.vt == VT_DISPATCH)
	{
		setDispatchValue(v);
	}
	else if (v.vt & VT_ARRAY)
	{
		setSafeArrayValue(v);
	}

	return S_OK;
}

void CX3DMFColor::setDispatchValue(VARIANT v)
{
	IDispatch *pDisp = v.pdispVal;

	assert(pDisp);

	DISPID lengthdisp = -1;
	LPOLESTR lengthname = L"length";
	DISPID indexdisp;
	char buf[1024];
	WCHAR wbuf[1024];
	LPOLESTR indexname = wbuf;

	HRESULT hr;
	
	hr = pDisp->GetIDsOfNames(IID_NULL, &lengthname, 1, LANG_NEUTRAL, &lengthdisp);

	VARIANT vl;
	DISPPARAMS params = {0, 0, 0, 0};

	hr = pDisp->Invoke(lengthdisp, IID_NULL, LANG_NEUTRAL, DISPATCH_PROPERTYGET, &params, &vl,
		NULL, NULL);

	ColorArray ca;
	ColorArray *pca = &ca;
	int arraysize = vl.intVal;
	Color c(0, 0, 0);
	for (int i = 0; i < arraysize; i++)
	{
		int mod = i % 3;

		sprintf(buf, "%d", i);
		mbstowcs(wbuf, buf, 1024);
		hr = pDisp->GetIDsOfNames(IID_NULL, &indexname, 1, LANG_NEUTRAL, &indexdisp);

		if (SUCCEEDED(hr))
		{
			hr = pDisp->Invoke(indexdisp, IID_NULL, LANG_NEUTRAL, DISPATCH_PROPERTYGET, &params, &vl,
				NULL, NULL);

			double d;

			if (vl.vt == VT_R8)
				d = vl.dblVal;
			else if (vl.vt == VT_R4)
				d = vl.fltVal;
			else if (vl.vt == VT_I4)
				d = vl.intVal;
			else
				d = 0.f;

			if (mod == 0)
				c.r = d;
			else if (mod == 1)
				c.g = d;
			else if (mod == 2)
				c.b = d;

		}

		if (mod == 2)
		{
			ca.push_back(c);
			c = Color(0, 0, 0);
		}
	}

	m_node->SetValue(m_mid, &pca);
}

void CX3DMFColor::setSafeArrayValue(VARIANT v)
{
	SAFEARRAY *pSA;

	pSA = v.parray;

	long lBound;
	long uBound;
	SafeArrayGetUBound(pSA, 1, &uBound);
	SafeArrayGetLBound(pSA, 1, &lBound);

	ColorArray ca;
	ColorArray *pca = &ca;
	Color c(0, 0, 0);
	for (long i = lBound; i <= uBound; i++)
	{
		int mod = i % 3;

		float f;
		double d;
		void *pV;

		if (v.vt & VT_R4)
			pV = &f;
		else if (v.vt & VT_R8)
			pV = &d;

		SafeArrayGetElement(pSA, &i, pV);

		if (v.vt & VT_R8 && !(v.vt & VT_R4))
			f = d;

		if (mod == 0)
			c.r = f;
		else if (mod == 1)
			c.g = f;
		else if (mod == 2)
			c.b = f;

		if (mod == 2)
		{
			ca.push_back(c);
			c = Color(0, 0, 0);
		}
	}

	m_node->SetValue(m_mid, &pca);
}

#endif
