/********************************************************************************
 * Flux
 *
 * File: X3DFieldDefinition.cpp
 * Description: Implementation of CX3DFieldDefinition
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

// X3DFieldDefinition.cpp : Implementation of CX3DFieldDefinition
#include "stdafx.h"
#include "Anima.h"
#include "X3DFieldDefinition.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DFieldDefinition


STDMETHODIMP CX3DFieldDefinition::get_name(BSTR *pVal)
{
	if (pVal == NULL)
		return E_INVALIDARG;

	*pVal = m_name.Copy();

	return S_OK;
}

STDMETHODIMP CX3DFieldDefinition::put_name(BSTR newVal)
{
	m_name = newVal;

	return S_OK;
}

STDMETHODIMP CX3DFieldDefinition::get_dataType(X3DFieldType *pVal)
{
	if (pVal == NULL)
		return E_INVALIDARG;

	*pVal = m_fieldType;

	return S_OK;
}

STDMETHODIMP CX3DFieldDefinition::put_dataType(X3DFieldType newVal)
{
	m_fieldType = newVal;

	return S_OK;
}

STDMETHODIMP CX3DFieldDefinition::get_accessType(X3DFieldAccess *pVal)
{
	if (pVal == NULL)
		return E_INVALIDARG;

	*pVal = m_fieldAccessType;

	return S_OK;
}

STDMETHODIMP CX3DFieldDefinition::put_accessType(X3DFieldAccess newVal)
{
	m_fieldAccessType = newVal;

	return S_OK;
}

X3DFieldDefinition *CX3DFieldDefinition::Create(X3DFieldType fieldType, X3DFieldAccess accessType,
	BSTR name)
{
	X3DFieldDefinition *pFD = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CoX3DFieldDefinition, NULL, CLSCTX_INPROC_SERVER, IID_X3DFieldDefinition,
		(LPVOID*)&pFD);

	if (FAILED(hr))
		return NULL;

	pFD->put_name(name);
	pFD->put_dataType(fieldType);
	pFD->put_accessType(accessType);

	return pFD;
}
