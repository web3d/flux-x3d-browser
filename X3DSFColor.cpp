/********************************************************************************
 * Flux
 *
 * File: X3DSFColor.cpp
 * Description: Implementation of CX3DSFColor
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

// X3DSFColor.cpp : Implementation of CX3DSFColor
#include "stdafx.h"
#include "Anima.h"
#include "X3DSFColor.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFColor

STDMETHODIMP CX3DSFColor::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_colorvalue);

	return S_OK;
}

STDMETHODIMP CX3DSFColor::get_r(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Color c;
	c = m_colorvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &c);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = c.r;
	return S_OK;
}

STDMETHODIMP CX3DSFColor::put_r(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Color c;
//	setval = X3DFieldBase::getValue((void **) &c);

//	if (setval != ANMFIELDBASE_DID)
		c = m_colorvalue;

	c.r = newVal;
	m_colorvalue = c;

	if (X3DFieldBase::setValue((void *) &c) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFColor::get_g(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Color c;
	c = m_colorvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &c);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = c.g;
	return S_OK;
}

STDMETHODIMP CX3DSFColor::put_g(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Color c;
//	setval = X3DFieldBase::getValue((void **) &c);

//	if (setval != ANMFIELDBASE_DID)
		c = m_colorvalue;

	c.g = newVal;
	m_colorvalue = c;

	if (X3DFieldBase::setValue((void *) &c) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFColor::get_b(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Color c;
	c = m_colorvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &c);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = c.b;
	return S_OK;
}

STDMETHODIMP CX3DSFColor::put_b(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Color c;
//	setval = X3DFieldBase::getValue((void **) &c);

//	if (setval != ANMFIELDBASE_DID)
		c = m_colorvalue;

	c.b = newVal;
	m_colorvalue = c;

	if (X3DFieldBase::setValue((void *) &c) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFColor::getValue(float *value)
{
	eAnmFieldBaseSetValue setval;

	if (value == NULL)
		return S_FALSE;

	Color c;
	c = m_colorvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &c);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	value[0] = c.r;
	value[1] = c.g;
	value[2] = c.b;

	return S_OK;
}

STDMETHODIMP CX3DSFColor::setValue(float *value)
{
	if (value == NULL)
		return S_FALSE;
	
	m_colorvalue.r = value[0];
	m_colorvalue.g = value[1];
	m_colorvalue.b = value[2];

	if (X3DFieldBase::setValue((void *) value) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

void CX3DSFColor::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	m_colorvalue = *(Color *) callData;

	X3DFieldBase::callCallbacks(this);
}

X3DSFColor *CX3DSFColor::Create(X3DExecutionContext *pX3DContext, const Color &c)
{
	X3DSFColor *pX3DSFColor = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CoX3DSFColor, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFColor, (LPVOID*)&pX3DSFColor);

	if (FAILED(hr))
		return NULL;

	pX3DSFColor->attach(pX3DContext, NULL, NULL, NULL, eValueColor);

	pX3DSFColor->put_r(c.r);
	pX3DSFColor->put_g(c.g);
	pX3DSFColor->put_b(c.b);

	return pX3DSFColor;
}
