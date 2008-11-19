/********************************************************************************
 * Flux
 *
 * File: X3DProtoDeclaration.cpp
 * Description: Implementation of CX3DProtoDeclaration
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

// X3DProtoDeclaration.cpp : Implementation of CX3DProtoDeclaration
#include "stdafx.h"
#include "Anima.h"
#include "X3DProtoDeclaration.h"
#include "x3dfieldbase.h"
#include "x3dfielddefinition.h"
#include "x3dfielddefinitionarray.h"

#include "anmsymbol.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DProtoDeclaration

X3DProtoDeclaration *CX3DProtoDeclaration::Create(class CAnmProtoSymbol *pProtoSym)
{
	assert(pProtoSym);

	X3DProtoDeclaration *pProtoDeclaration = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CoX3DProtoDeclaration, NULL, CLSCTX_INPROC_SERVER, IID_X3DProtoDeclaration, 
		(LPVOID*)&pProtoDeclaration);

	if (FAILED(hr))
		return NULL;

	pProtoDeclaration->setNativeProtoDeclaration(pProtoSym);

	return pProtoDeclaration;
}

STDMETHODIMP CX3DProtoDeclaration::get_name(/*[out, retval]*/ BSTR *pVal)
{
	assert (m_protosym);

	String s = m_protosym->GetName();

	if (s)
	{
		CComBSTR bstr = s->GetBuf();

		*pVal = bstr.Copy();

		return S_OK;
	}

	return E_FAIL;
}

STDMETHODIMP CX3DProtoDeclaration::get_isExternProto(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	assert (m_protosym);

	if (m_protosym->IsExternProto())
		*pVal = VARIANT_TRUE;
	else
		*pVal = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CX3DProtoDeclaration::get_fields(/*[out, retval]*/ X3DFieldDefinitionArray **pVal)
{
	assert (m_protosym);

	CAnmClass *pClass = m_protosym->GetClass();
	assert(pClass);

	int numClassMembers = pClass->GetNumMembers();

	if (numClassMembers <= 0)
		return S_OK;

	X3DFieldDefinitionArray *pFieldDefs = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CoX3DFieldDefinitionArray, NULL, CLSCTX_INPROC_SERVER, 
		IID_X3DFieldDefinitionArray, (LPVOID*)&pFieldDefs);

	if (FAILED(hr))
		return E_FAIL;

	pFieldDefs->put_Count(numClassMembers);

	CAnmClassMember **members = pClass->GetMemberTable();
	for (int i = 0; i < numClassMembers; i++)
	{
		CLASSMEMBERID mid = members[i];

		X3DFieldType fieldType;
		X3DFieldAccess accessType; 
		BSTR fieldName;


		accessType = X3DFieldBase::getAccessType(mid->memberType);
		fieldType = X3DFieldBase::getFieldType(mid->valueType);
		CComBSTR str(mid->name);
		fieldName = str.Copy();

		X3DFieldDefinition *pFieldDef = CX3DFieldDefinition::Create(fieldType, accessType, fieldName);

		pFieldDefs->set1Value(i, pFieldDef);
	}

	*pVal = pFieldDefs;

	return S_OK;
}
