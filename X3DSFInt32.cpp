/********************************************************************************
 * Flux
 *
 * File: X3DSFInt32.cpp
 * Description: Implementation of CX3DSFInt32
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

// X3DSFInt32.cpp : Implementation of CX3DSFInt32
#include "stdafx.h"
#include "Anima.h"
#include "X3DSFInt32.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFInt32


STDMETHODIMP CX3DSFInt32::getValue(long *value)
{
	eAnmFieldBaseSetValue setval;

	if (value == NULL)
		return S_FALSE;
	
	*value = m_intvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) value);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	else
		return S_OK;
}

STDMETHODIMP CX3DSFInt32::setValue(long value)
{
	m_intvalue = value;
	if (X3DFieldBase::setValue((void *) &value) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

void CX3DSFInt32::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	m_intvalue = *(Integer *) callData;

	X3DFieldBase::callCallbacks(this);
}
