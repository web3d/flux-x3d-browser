/********************************************************************************
 * Flux
 *
 * File: X3DSFTime.cpp
 * Description: Implementation of CX3DSFTime
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

// X3DSFTime.cpp : Implementation of CX3DSFTime
#include "stdafx.h"
#include "Anima.h"
#include "X3DSFTime.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFTime

STDMETHODIMP CX3DSFTime::setValue(double *value)
{
	if (value == NULL)
		return S_FALSE;

	m_timeval = *value;
	if (X3DFieldBase::setValue((void *) value) != ANMFIELDBASE_CANTDO)
		return S_OK;
	else
		return S_FALSE;
}

STDMETHODIMP CX3DSFTime::getValue(double *value)
{
	eAnmFieldBaseSetValue setval;

	if (value == NULL)
		return S_FALSE;
	
	*value = m_timeval; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) value);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	else
		return S_OK;

}


void CX3DSFTime::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	m_timeval = *(double *) callData;

	X3DFieldBase::callCallbacks(this);
}
