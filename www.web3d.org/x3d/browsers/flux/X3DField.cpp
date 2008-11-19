/********************************************************************************
 * Flux
 *
 * File: X3DField.cpp
 * Description: Implementation of CX3DField
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

// X3DField.cpp : Implementation of CX3DField
#include "stdafx.h"
#include "Anima.h"
#include "X3DField.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DField




STDMETHODIMP CX3DField::attach(X3DExecutionContext *context, void *pNode, void *pField, void *mid, long valueType)
{
	assert(FALSE);
	return S_OK;
}

STDMETHODIMP CX3DField::addFieldEventListener(IDispatch *listener)
{
	assert(FALSE);
	return S_OK;
}

STDMETHODIMP CX3DField::removeFieldEventListener(IDispatch *listener)
{
	assert(FALSE);
	return S_OK;
}

STDMETHODIMP CX3DField::getAccessType(X3DFieldAccess *accesstype)
{
	assert(FALSE);
	return S_OK;
}

STDMETHODIMP CX3DField::getType(X3DFieldType *fieldtype)
{
	assert(FALSE);
	return S_OK;
}

STDMETHODIMP CX3DField::getName(BSTR *name)
{
	assert(FALSE);
	return S_OK;
}

STDMETHODIMP CX3DField::dispose()
{
	assert(FALSE);
	return S_OK;
}