/********************************************************************************
 * Flux
 *
 * File: X3DSFString.cpp
 * Description: Implementation of CX3DSFString
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

// X3DSFString.cpp : Implementation of CX3DSFString
#include "stdafx.h"
#include "Anima.h"
#include "X3DSFString.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFString


STDMETHODIMP CX3DSFString::getValue(BSTR *value)
{
	eAnmFieldBaseSetValue setval;

	if (value == NULL)
		return S_FALSE;

	String s;

	s = m_stringvalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &s);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;

	CComBSTR bstr = s->GetBuf();
	*value = bstr.Copy();

	return S_OK;
}

STDMETHODIMP CX3DSFString::setValue(BSTR value)
{
	int len = wcslen(value);
	String s = new CAnmString;
	s->alloc(len + 1);
	wcstombs(s->GetBuf(), value, len + 1);

	SafeUnRef(m_stringvalue);
	m_stringvalue = s;

	bool success = (X3DFieldBase::setValue((void *) &s) != ANMFIELDBASE_CANTDO);

	if (success)
		return S_OK;
	else
		return S_FALSE;


}

void CX3DSFString::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	m_stringvalue = *(String *) callData;

	X3DFieldBase::callCallbacks(this);
}
