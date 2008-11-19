/********************************************************************************
 * Flux
 *
 * File: anmniynodes.cpp
 * Description: Not-implemented node classes
 *						Used to support loading but not realizing
 *						unknown types
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

#include "anmniynodes.h"

#if 0

// We're implementing everything now

BEGIN_NIYCLASS(MovieTexture)

  NIYMEMBER(set_loop, eValueBoolean, eAnmMethodMember),
  NIYMEMBER(set_speed, eValueFloat, eAnmMethodMember),
  NIYMEMBER(set_startTime, eValueTime, eAnmMethodMember),
  NIYMEMBER(set_stopTime, eValueTime, eAnmMethodMember),
  NIYMEMBER(set_url, eValueStringArray, eAnmMethodMember),

  NIYMEMBER(loop, eValueBoolean, eAnmValueMember),
  NIYMEMBER(speed, eValueFloat, eAnmValueMember),
  NIYMEMBER(startTime, eValueTime, eAnmValueMember),
  NIYMEMBER(stopTime, eValueTime, eAnmValueMember),
  NIYMEMBER(url, eValueStringArray, eAnmValueMember),

  NIYMEMBER(repeatS, eValueBoolean, eAnmValueMember),
  NIYMEMBER(repeatT, eValueBoolean, eAnmValueMember),

  NIYMEMBER(loop_changed, eValueBoolean, eAnmCallbackMember),
  NIYMEMBER(speed_changed, eValueFloat, eAnmCallbackMember),
  NIYMEMBER(startTime_changed, eValueTime, eAnmCallbackMember),
  NIYMEMBER(stopTime_changed, eValueTime, eAnmCallbackMember),
  NIYMEMBER(url_changed, eValueStringArray, eAnmCallbackMember),
  NIYMEMBER(duration_changed, eValueTime, eAnmCallbackMember),
  NIYMEMBER(isActive, eValueBoolean, eAnmCallbackMember),

END_NIYCLASS


sAnmNIYClass CAnmNIYNodes::m_NIYClasses[] =
{
	NIYCLASS(MovieTexture),

};

CAnmClass *CAnmNIYNodes::createClass(sAnmNIYClass *pNIYClass)
{
	CAnmClass *pClass = new CAnmClass();

	int nMembers = pNIYClass->nMembers;

	pClass->m_className = pNIYClass->className;
	pClass->m_createObject = NULL;
	pClass->m_superclass = NULL;
	pClass->m_numMembers = nMembers;
	if (nMembers)
		pClass->m_memberTable = new CAnmClassMember *[nMembers];
	else
		pClass->m_memberTable = NULL;

	for (int i = 0; i < nMembers; i++)
	{
		pClass->m_memberTable[i] = &pNIYClass->members[i];
	}

	return pClass;
}


#endif // 0

void CAnmNIYNodes::InitNIYClassSymbols(CAnmSymbolTable *pClassSymbols)
{
#if 0
	
	int numNIYClasses = sizeof(m_NIYClasses) / sizeof(sAnmNIYClass);

	for (int i = 0; i < numNIYClasses; i++)
	{
		CAnmClass *pClass = createClass(&m_NIYClasses[i]);

		// create a new class symbol and list of member symbols
		String s = new CAnmString(pClass->m_className);
		CAnmClassSymbol *pClassSym = new CAnmClassSymbol(s, pClass);
		s->UnRef();
		CAnmClassMember **pMemberTable = pClass->GetMemberTable();

		for (int i = 0; i < pClass->GetNumMembers(); i++)
		{
			CAnmClassMember *pMember = pMemberTable[i];
			pClassSym->AddAttribute(pMember->name, pMember);
		}

		// add it to the symbol table
		pClassSymbols->AddSymbol(pClassSym);
	}
#endif // 0

}



