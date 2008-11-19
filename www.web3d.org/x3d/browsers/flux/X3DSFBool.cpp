/********************************************************************************
 * Flux
 *
 * File: X3DSFBool.cpp
 * Description: Implementation of CX3DSFBool
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

// X3DSFBool.cpp : Implementation of CX3DSFBool
#include "stdafx.h"
#include "Anima.h"
#include "X3DSFBool.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFBool


STDMETHODIMP CX3DSFBool::setValue (VARIANT_BOOL value)
{
	Boolean val;
	if (value == VARIANT_TRUE)
		val = TRUE;
	else
		val = FALSE;

	m_boolval = val;
	if (X3DFieldBase::setValue((void *) &val) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFBool::getValue(VARIANT_BOOL *value)
{
	Boolean val;
	eAnmFieldBaseSetValue setval;

	val	= m_boolval; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &val);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	if (val)
		*value = VARIANT_TRUE;
	else
		*value = VARIANT_FALSE;

	return S_OK;
}

void CX3DSFBool::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	m_boolval = *(Boolean *) callData;

	X3DFieldBase::callCallbacks(this);
}
