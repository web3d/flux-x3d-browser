/********************************************************************************
 * Flux
 *
 * File: anmclass.cpp
 * Description: Class metadata for object system
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
#include "anmclass.h"
#include "anmsymbol.h"

CAnmObject *CAnmClass::Create()
{
	assert(m_createObject != NULL);
	
	CAnmObject *pObject = (*m_createObject)();
	if (m_classSymbol)
		m_classSymbol->InitObject(pObject);

	return pObject;
}

CAnmClassMember *CAnmClass::FindCallback(char *name)
{
	return FindClassMember(name, eAnmCallbackMember);
}


CAnmClassMember *CAnmClass::FindMethod(char *name)
{
	return FindClassMember(name, eAnmMethodMember);
}

CAnmClassMember *CAnmClass::FindValue(char *name)
{
	return FindClassMember(name, eAnmValueMember);
}

CAnmClassMember *CAnmClass::FindInit(char *name)
{
	return FindClassMember(name, eAnmInitMember);
}

CAnmClassMember *CAnmClass::FindClassMember(char *name)
{
	CAnmClassMember *pEntry, *pFoundEntry = NULL;
	
	for (int i = 0; i < m_numMembers; i++)
	{
		pEntry = m_memberTable[i];
		if (!strcmp(name, pEntry->name))
		{
			pFoundEntry = pEntry;
			break;
		}
	}

	if (pFoundEntry)
		return pFoundEntry;
	else
	{
		if (m_superclass)
			return m_superclass->FindClassMember(name);
	}

	return NULL;
}

CAnmClassMember *CAnmClass::FindClassMember(char *name, eAnmClassMemberType memberType)
{
	CAnmClassMember *pEntry, *pFoundEntry = NULL;
	
	for (int i = 0; i < m_numMembers; i++)
	{
		pEntry = m_memberTable[i];
		if (!strcmp(name, pEntry->name) && memberType == pEntry->memberType)
		{
			pFoundEntry = pEntry;
			break;
		}
	}

	if (pFoundEntry)
		return pFoundEntry;
	else
	{
		if (m_superclass)
			return m_superclass->FindClassMember(name, memberType);
	}

	return NULL;
}

void CAnmClass::FixupInherited()
{
	if (m_superclass && m_superclass->m_fixupinherited)
	{
		m_superclass->FixupInherited();
	}

	CAnmClassMember *pEntry;
	
	for (int i = 0; i < m_numMembers; i++)
	{
		pEntry = m_memberTable[i];
		if (pEntry->memberType == eAnmInheritedMember)
		{
			CLASSMEMBERID mid = (CLASSMEMBERID) pEntry->pInherited;
			*pEntry = *mid;
		}
	}

	m_fixupinherited = false;
}
