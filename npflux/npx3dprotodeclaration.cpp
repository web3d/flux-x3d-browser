/********************************************************************************
 * Flux
 *
 * File: npx3dprotodeclaration.cpp
 * Description: Mozilla X3DProtoDeclaration
 *							Scriptable Plugin Object
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
 

#include "stdafx.h"
#include "npx3dprotodeclaration.h"
#include "npx3dfielddefinition.h"
#include "anmsymbol.h"

/////////////////////////////////////////////////////////////////////////////
// CNPX3DProtoDeclaration

CNPX3DProtoDeclaration::~CNPX3DProtoDeclaration()
{
}

void CNPX3DProtoDeclaration::SetProtoSymbol(CAnmProtoSymbol *pProtoSym)
{
	m_protosym = pProtoSym;
}

CAnmProtoSymbol * CNPX3DProtoDeclaration::GetProtoSymbol()
{
	return m_protosym;
}

bool CNPX3DProtoDeclaration::getName(const char **name)
{
	assert (m_protosym);

	String s = m_protosym->GetName();

	if (s)
	{
		*name = s->GetBuf();
		return true;
	}
	else
		return false;
}

bool CNPX3DProtoDeclaration::getFields(NPArray **protoDeclarations)
{
	CAnmClass *pClass = m_protosym->GetClass();
	assert(pClass);

	int numClassMembers = pClass->GetNumMembers();

	if (numClassMembers <= 0)
		return true;

	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	*protoDeclarations = pArray;

	CAnmClassMember **members = pClass->GetMemberTable();
	for (int i = 0; i < numClassMembers; i++)
	{
		CLASSMEMBERID mid = members[i];

		CNPX3DFieldDefinition *pFieldDefinition = CNPX3DFieldDefinition::Create(mNpp, mid);

		NPN_RetainObject(pFieldDefinition);

		NPVariant v;

		OBJECT_TO_NPVARIANT(pFieldDefinition, v);

		pArray->set1Value(i, v);

	}

	return true;
}

// NP Glue

DEFINE_NPPROPID(CNPX3DProtoDeclaration,name);
DEFINE_NPPROPID(CNPX3DProtoDeclaration,fields);
DEFINE_NPPROPID(CNPX3DProtoDeclaration,isExternProto);


bool CNPX3DProtoDeclaration::m_classInitialized = false;

static NPObject *
AllocateCNPX3DProtoDeclaration(NPP npp, NPClass *aClass)
{
  return new CNPX3DProtoDeclaration(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DProtoDeclaration,
                                 AllocateCNPX3DProtoDeclaration);

CNPX3DProtoDeclaration *CNPX3DProtoDeclaration::Create(NPP npp, CAnmProtoSymbol *pProtoSym)
{
	CNPX3DProtoDeclaration *pProtoDeclaration = (CNPX3DProtoDeclaration *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DProtoDeclaration));

	if (pProtoSym)
		pProtoDeclaration->SetProtoSymbol(pProtoSym);

	return pProtoDeclaration;
}


void CNPX3DProtoDeclaration::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPPROPID(name);
	INIT_NPPROPID(fields);
	INIT_NPPROPID(isExternProto);

	m_classInitialized = true;
}

bool
CNPX3DProtoDeclaration::HasMethod(NPIdentifier name)
{
  return 
	  false	  
	  ;

}

bool
CNPX3DProtoDeclaration::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
  return PR_FALSE;
}

bool
CNPX3DProtoDeclaration::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  STRINGZ_TO_NPVARIANT(NPstrdup("[X3DProtoDeclaration]"), *result);

  return PR_TRUE;
}

bool
CNPX3DProtoDeclaration::HasProperty(NPIdentifier name)
{
	return 
	  TEST_NPPROPID(name, name) ||
	  TEST_NPPROPID(name, fields) ||
	  TEST_NPPROPID(name, isExternProto) ||

	  false ;
}

bool
CNPX3DProtoDeclaration::GetProperty(NPIdentifier name, NPVariant *result)
{
	const char *cpret = NULL;
	String s0 = NULL;

	if (TEST_NPPROPID(name, name))
	{
		if (getName(&cpret))
		{
			STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
		}

		return PR_TRUE;
	}
	else if (TEST_NPPROPID(name, fields))
	{
		NPArray *pArray = NULL;

		if (getFields(&pArray))
		{
			OBJECT_TO_NPVARIANT(pArray, *result);
			return PR_TRUE;
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPPROPID(name, isExternProto))
	{
		bool isExternProto = m_protosym->IsExternProto();
		BOOLEAN_TO_NPVARIANT(isExternProto, *result);

		return PR_TRUE;
	}

  return PR_FALSE;
}

bool
CNPX3DProtoDeclaration::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return PR_FALSE;
}
