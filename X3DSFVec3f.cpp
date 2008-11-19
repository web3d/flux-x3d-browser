/********************************************************************************
 * Flux
 *
 * File: X3DSFVec3f.cpp
 * Description: Implementation of CX3DSFVec3f
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

// X3DSFVec3f.cpp : Implementation of CX3DSFVec3f
#include "stdafx.h"
#include "Anima.h"
#include "X3DSFVec3f.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFVec3f

STDMETHODIMP CX3DSFVec3f::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_point3value);

	return S_OK;
}

STDMETHODIMP CX3DSFVec3f::get_x(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Point3 p3;
	p3 = m_point3value; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &p3);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = p3.x;
	return S_OK;
}

STDMETHODIMP CX3DSFVec3f::put_x(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Point3 p3;
//	setval = X3DFieldBase::getValue((void **) &p3);

//	if (setval != ANMFIELDBASE_DID)
		p3 = m_point3value;

	p3.x = newVal;
	m_point3value = p3;

	if (X3DFieldBase::setValue((void *) &p3) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFVec3f::get_y(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Point3 p3;
	p3 = m_point3value; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &p3);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = p3.y;
	return S_OK;
}

STDMETHODIMP CX3DSFVec3f::put_y(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Point3 p3;
//	setval = X3DFieldBase::getValue((void **) &p3);

//	if (setval != ANMFIELDBASE_DID)
		p3 = m_point3value;

	p3.y = newVal;
	m_point3value = p3;

	if (X3DFieldBase::setValue((void *) &p3) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFVec3f::get_z(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Point3 p3;
	p3 = m_point3value; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &p3);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = p3.z;
	return S_OK;
}

STDMETHODIMP CX3DSFVec3f::put_z(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Point3 p3;
//	setval = X3DFieldBase::getValue((void **) &p3);

//	if (setval != ANMFIELDBASE_DID)
		p3 = m_point3value;

	p3.z = newVal;
	m_point3value = p3;

	if (X3DFieldBase::setValue((void *) &p3) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFVec3f::getValue(float *value)
{
	eAnmFieldBaseSetValue setval;

	if (value == NULL)
		return S_FALSE;

	Point3 p3;
	p3 = m_point3value; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &p3);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	value[0] = p3.x;
	value[1] = p3.y;
	value[2] = p3.z;

	return S_OK;
}

STDMETHODIMP CX3DSFVec3f::setValue(float *value)
{
	if (value == NULL)
		return S_FALSE;
	
	m_point3value.x = value[0];
	m_point3value.y = value[1];
	m_point3value.z = value[2];

	if (X3DFieldBase::setValue((void *) value) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

void CX3DSFVec3f::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	m_point3value = *(Point3 *) callData;

	X3DFieldBase::callCallbacks(this);
}

X3DSFVec3f *CX3DSFVec3f::Create(X3DExecutionContext *pX3DContext, const Point3 &p3)
{
	X3DSFVec3f *pX3DSFVec3f = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CoX3DSFVec3f, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFVec3f, (LPVOID*)&pX3DSFVec3f);

	if (FAILED(hr))
		return NULL;

	pX3DSFVec3f->attach(pX3DContext, NULL, NULL, NULL, eValuePoint3);

	pX3DSFVec3f->put_x(p3.x);
	pX3DSFVec3f->put_y(p3.y);
	pX3DSFVec3f->put_z(p3.z);

	return pX3DSFVec3f;
}
