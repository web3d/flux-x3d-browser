/********************************************************************************
 * Flux
 *
 * File: npx3dfielddefinition.cpp
 * Description: Mozilla X3DFieldDefinition
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
#include "npx3dfielddefinition.h"
#include "npx3dfield.h"

/////////////////////////////////////////////////////////////////////////////
// CNPX3DFieldDefinition

CNPX3DFieldDefinition::~CNPX3DFieldDefinition()
{
}

void CNPX3DFieldDefinition::SetClassMember(CLASSMEMBERID mid)
{
	m_mid = mid;
}

CLASSMEMBERID CNPX3DFieldDefinition::GetClassMember()
{
	return m_mid;
}

bool CNPX3DFieldDefinition::getName(const char **name)
{
	assert (m_mid);

	*name = m_mid->name;
	return true;
}


bool CNPX3DFieldDefinition::getAccessType(int *accessType)
{
	assert (m_mid);

	*accessType = CNPX3DField::getAccessType(m_mid->memberType);

	return true;
}

bool CNPX3DFieldDefinition::getDataType(int *dataType)
{
	assert (m_mid);

	*dataType = CNPX3DField::getFieldType(m_mid->valueType);

	return true;
}

// NP Glue

DEFINE_NPPROPID(CNPX3DFieldDefinition,name);
DEFINE_NPPROPID(CNPX3DFieldDefinition,accessType);
DEFINE_NPPROPID(CNPX3DFieldDefinition,dataType);


bool CNPX3DFieldDefinition::m_classInitialized = false;

static NPObject *
AllocateCNPX3DFieldDefinition(NPP npp, NPClass *aClass)
{
  return new CNPX3DFieldDefinition(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DFieldDefinition,
                                 AllocateCNPX3DFieldDefinition);

CNPX3DFieldDefinition *CNPX3DFieldDefinition::Create(NPP npp, CLASSMEMBERID mid)
{
	CNPX3DFieldDefinition *pFieldDefinition = (CNPX3DFieldDefinition *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DFieldDefinition));

	if (mid)
		pFieldDefinition->SetClassMember(mid);

	return pFieldDefinition;
}


void CNPX3DFieldDefinition::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPPROPID(name);
	INIT_NPPROPID(accessType);
	INIT_NPPROPID(dataType);

	m_classInitialized = true;
}

bool
CNPX3DFieldDefinition::HasMethod(NPIdentifier name)
{
  return 
	  false	  
	  ;

}

bool
CNPX3DFieldDefinition::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
  return PR_FALSE;
}

bool
CNPX3DFieldDefinition::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  STRINGZ_TO_NPVARIANT(NPstrdup("[X3DProtoDeclaration]"), *result);

  return PR_TRUE;
}

bool
CNPX3DFieldDefinition::HasProperty(NPIdentifier name)
{
	return 
	  TEST_NPPROPID(name, name) ||
	  TEST_NPPROPID(name, accessType) ||
	  TEST_NPPROPID(name, dataType) ||

	  false ;
}

bool
CNPX3DFieldDefinition::GetProperty(NPIdentifier name, NPVariant *result)
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
	else if (TEST_NPPROPID(name, accessType))
	{
		int accessType = 0;

		if (getAccessType(&accessType))
		{
			INT32_TO_NPVARIANT(accessType, *result);
			return PR_TRUE;
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPPROPID(name, dataType))
	{
		int dataType = 0;

		if (getDataType(&dataType))
		{
			INT32_TO_NPVARIANT(dataType, *result);
			return PR_TRUE;
		}
		else
			return PR_FALSE;
	}

  return PR_FALSE;
}

bool
CNPX3DFieldDefinition::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return PR_FALSE;
}
