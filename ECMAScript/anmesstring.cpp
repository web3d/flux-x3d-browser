/********************************************************************************
 * Flux
 *
 * File: anmesstring.cpp
 * Description: ECMAScript String object
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
#include "anmesstring.h"

// Browser object
CAnmESString::CAnmESString(String s, CAnmECMAScriptLanguageBinding *pOwner)
: CAnmESBuiltinObject(s, pOwner)
{
}


CAnmESString::~CAnmESString()
{
}

CAnmESObject *CAnmESString::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	CAnmESString *pStr = NULL;

	if (nargs >= 1)
	{
		pStr = args[0];
		if (pStr)
			return pStr->Clone();
	}

	return new CAnmESString(new CAnmString(""), pOwner);
}

CAnmESString *CAnmESString::Clone()
{
	String s = (String) m_object;

	return new CAnmESString(s->Clone(), m_owner);
}

AnmESVar CAnmESString::toString(int nArgs, AnmESVar *args)
{
	return (AnmESVar) this;
}

AnmESVar CAnmESString::getLength()
{
	String s = (String) m_object;

	return (AnmESVar) s->length();
}

void CAnmESString::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

#if 0
	// don't really know how to do this yet
	char *cp;
	String s = (String) propValue;
	char c = '\0';
	if (s->length() > 0)
	{
		cp = s->GetBuf();
		c = cp[0];
	}

	s->UnRef();

	s = (String) m_object;
	cp = s->GetBuf();
	if (index < s->length())
	{
		cp[index] = c;
	}
	else
	{
		String newstr = new CAnmString;
		newstr->alloc(index + 2);
		char *cp2 = newstr->GetBuf();
		strcpy(cp2, cp);
		cp2[index] = c;
		cp2[index+1] = '\0';

		SafeUnRef(m_object);
		m_object = newstr;
	}
#endif
}

AnmESVar CAnmESString::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;

	char c = '\0';
	String s = (String) m_object;
	if (index < s->length())
	{
		char *cp = s->GetBuf();
		c = cp[index];
	}

	CAnmESString *pStr = new CAnmESString(new CAnmString(&c), m_owner);

	return (AnmESVar) pStr;
}

// Tables for Callbacks, Methods and Put/GetProperties

// Browser object
DEFINE_ESMETHOD(CAnmESString, toString);
DEFINE_ESREADONLYPROPERTY(CAnmESString, length, getLength);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESString)
ESMETHODID(CAnmESString, toString),
ESPROPID(CAnmESString, length),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESString, CAnmESBuiltinObject);

