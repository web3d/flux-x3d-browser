/********************************************************************************
 * Flux
 *
 * File: X3DSFRotation.cpp
 * Description: Implementation of CX3DSFRotation
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

// X3DSFRotation.cpp : Implementation of CX3DSFRotation
#include "stdafx.h"
#include "Anima.h"
#include "X3DSFRotation.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFRotation

STDMETHODIMP CX3DSFRotation::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{ 
	X3DFieldBase::attach(context, (CAnmSAINode *) pNode, (CAnmField *) pField, 
		(CLASSMEMBERID) mid, (eValueType) valueType);

	X3DFieldBase::getValue((void **) &m_rotationvalue);

	return S_OK;
}


STDMETHODIMP CX3DSFRotation::get_x(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Rotation r;
	r = m_rotationvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &r);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = r.vec.x;
	return S_OK;
}

STDMETHODIMP CX3DSFRotation::put_x(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Rotation r;
//	setval = X3DFieldBase::getValue((void **) &r);

//	if (setval != ANMFIELDBASE_DID)
		r = m_rotationvalue;

	r.vec.x = newVal;
	m_rotationvalue = r;

	if (X3DFieldBase::setValue((void *) &r) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFRotation::get_y(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Rotation r;
	r = m_rotationvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &r);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = r.vec.y;
	return S_OK;
}

STDMETHODIMP CX3DSFRotation::put_y(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Rotation r;
//	setval = X3DFieldBase::getValue((void **) &r);

//	if (setval != ANMFIELDBASE_DID)
		r = m_rotationvalue;

	r.vec.y = newVal;
	m_rotationvalue = r;

	if (X3DFieldBase::setValue((void *) &r) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFRotation::get_z(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Rotation r;
	r = m_rotationvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &r);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = r.vec.z;
	return S_OK;
}

STDMETHODIMP CX3DSFRotation::put_z(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Rotation r;
//	setval = X3DFieldBase::getValue((void **) &r);

//	if (setval != ANMFIELDBASE_DID)
		r = m_rotationvalue;

	r.vec.z = newVal;
	m_rotationvalue = r;

	if (X3DFieldBase::setValue((void *) &r) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFRotation::get_angle(float *pVal)
{
	eAnmFieldBaseSetValue setval;

	if (pVal == NULL)
		return E_INVALIDARG;

	Rotation r;
	r = m_rotationvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &r);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	*pVal = r.theta;
	return S_OK;
}

STDMETHODIMP CX3DSFRotation::put_angle(float newVal)
{	
	eAnmFieldBaseSetValue setval;

	Rotation r;
//	setval = X3DFieldBase::getValue((void **) &r);

//	if (setval != ANMFIELDBASE_DID)
		r = m_rotationvalue;

	r.theta = newVal;
	m_rotationvalue = r;

	if (X3DFieldBase::setValue((void *) &r) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFRotation::getValue(float *value)
{
	eAnmFieldBaseSetValue setval;

	if (value == NULL)
		return S_FALSE;

	Rotation r;
	r = m_rotationvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &r);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	value[0] = r.vec.x;
	value[1] = r.vec.y;
	value[2] = r.vec.z;
	value[3] = r.theta;

	return S_OK;
}

STDMETHODIMP CX3DSFRotation::setValue(float *value)
{
	if (value == NULL)
		return S_FALSE;
	
	m_rotationvalue.vec.x = value[0];
	m_rotationvalue.vec.y = value[1];
	m_rotationvalue.vec.z = value[2];
	m_rotationvalue.theta = value[3];

	if (X3DFieldBase::setValue((void *) value) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

void CX3DSFRotation::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	m_rotationvalue = *(Rotation *) callData;

	X3DFieldBase::callCallbacks(this);
}


X3DSFRotation *CX3DSFRotation::Create(X3DExecutionContext *pX3DContext, const Rotation &r)
{
	X3DSFRotation *pX3DSFRotation = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CoX3DSFRotation, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFRotation, (LPVOID*)&pX3DSFRotation);

	if (FAILED(hr))
		return NULL;

	pX3DSFRotation->attach(pX3DContext, NULL, NULL, NULL, eValueRotation);
	
	pX3DSFRotation->put_x(r.vec.x);
	pX3DSFRotation->put_y(r.vec.y);
	pX3DSFRotation->put_z(r.vec.z);
	pX3DSFRotation->put_angle(r.theta);

	return pX3DSFRotation;
}
