/********************************************************************************
 * Flux
 *
 * File: X3DSFVec2f.cpp
 * Description: Implementation of CX3DSFVec2f
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

// X3DSFVec2f.cpp : Implementation of CX3DSFVec2f
#include "stdafx.h"
#include "Anima.h"
#include "X3DSFVec2f.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFVec2f

STDMETHODIMP CX3DSFVec2f::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_point2value);

	return S_OK;
}


STDMETHODIMP CX3DSFVec2f::get_x(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Point2 p2;
	p2 = m_point2value; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &p2);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = p2.x;
	return S_OK;
}

STDMETHODIMP CX3DSFVec2f::put_x(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Point2 p2;
//	setval = X3DFieldBase::getValue((void **) &p3);

//	if (setval != ANMFIELDBASE_DID)
		p2 = m_point2value;

	p2.x = newVal;
	m_point2value = p2;

	if (X3DFieldBase::setValue((void *) &p2) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFVec2f::get_y(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Point2 p2;
	p2 = m_point2value; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &p2);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = p2.y;
	return S_OK;
}

STDMETHODIMP CX3DSFVec2f::put_y(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Point2 p2;
//	setval = X3DFieldBase::getValue((void **) &p3);

//	if (setval != ANMFIELDBASE_DID)
		p2 = m_point2value;

	p2.y = newVal;
	m_point2value = p2;

	if (X3DFieldBase::setValue((void *) &p2) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFVec2f::getValue(float *value)
{
	eAnmFieldBaseSetValue setval;

	if (value == NULL)
		return S_FALSE;

	Point2 p2;
	p2 = m_point2value; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &p2);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	value[0] = p2.x;
	value[1] = p2.y;

	return S_OK;
}

STDMETHODIMP CX3DSFVec2f::setValue(float *value)
{
	if (value == NULL)
		return S_FALSE;
	
	m_point2value.x = value[0];
	m_point2value.y = value[1];

	if (X3DFieldBase::setValue((void *) value) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

void CX3DSFVec2f::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	m_point2value = *(Point2 *) callData;

	X3DFieldBase::callCallbacks(this);
}

X3DSFVec2f *CX3DSFVec2f::Create(X3DExecutionContext *pX3DContext, const Point2 &p2)
{
	X3DSFVec2f *pX3DSFVec2f = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CoX3DSFVec2f, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFVec2f, (LPVOID*)&pX3DSFVec2f);

	if (FAILED(hr))
		return NULL;

	pX3DSFVec2f->attach(pX3DContext, NULL, NULL, NULL, eValuePoint2);

	pX3DSFVec2f->put_x(p2.x);
	pX3DSFVec2f->put_y(p2.y);

	return pX3DSFVec2f;
}
