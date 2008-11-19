/********************************************************************************
 * Flux
 *
 * File: X3DSFFloat.cpp
 * Description: Implementation of CX3DSFFloat
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

// X3DSFFloat.cpp : Implementation of CX3DSFFloat
#include "stdafx.h"
#include "Anima.h"
#include "X3DSFFloat.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFFloat


STDMETHODIMP CX3DSFFloat::getValue(float *value)
{
	eAnmFieldBaseSetValue setval;

	if (value == NULL)
		return S_FALSE;
	
	*value = m_floatvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) value);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	else
		return S_OK;

}

STDMETHODIMP CX3DSFFloat::setValue(float value)
{
	m_floatvalue = value;
	if (X3DFieldBase::setValue((void *) &value) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

void CX3DSFFloat::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	m_floatvalue = *(Float *) callData;

	X3DFieldBase::callCallbacks(this);
}
