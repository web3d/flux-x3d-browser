/********************************************************************************
 * Flux
 *
 * File: anmesobject.cpp
 * Description: ECMAScript base object
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
#include "anmesobject.h"
#include "anmeslanguagebinding.h"
#include "anmesmessage.h"
#include "anmesstring.h"

CAnmESObject::CAnmESObject(class CAnmECMAScriptLanguageBinding *pOwner)
{
	m_objType = eBuiltinObject;
	assert(pOwner);
	m_owner = pOwner;
	m_marked = FALSE;
	pOwner->AddObject(this);
}

CAnmESObject::CAnmESObject(eESObjectType objType, class CAnmECMAScriptLanguageBinding *pOwner)
{
	m_objType = objType;
	m_owner = pOwner;
	m_marked = FALSE;
	pOwner->AddObject(this);
}

// Builtin object
CAnmESBuiltinObject::CAnmESBuiltinObject(CAnmObject *pObj, CAnmECMAScriptLanguageBinding *pOwner,
										 eESBuiltinObjectType builtinType)
										 : CAnmESObject(pOwner)
{
	m_object = pObj;
	if (m_object)
		m_object->Ref();
	m_builtinType = builtinType;
}

CAnmESBuiltinObject::~CAnmESBuiltinObject()
{
	SafeUnRef(m_object);
}

void CAnmESBuiltinObject::PutProperty(char *propName, AnmESVar propValue)
{
	CAnmClass *pClass = GetClass();
	assert(pClass);

	CLASSMEMBERID mid = pClass->FindClassMember(propName);
	assert(mid);

	assert(mid->memberType == eAnmValueMember);
	ESPUTPROPPOINTER pPP = (ESPUTPROPPOINTER) mid->pSetValue;

	assert(pPP);

	(*pPP) (this, propValue);
}

AnmESVar CAnmESBuiltinObject::GetProperty(char *propName)
{
	CAnmClass *pClass = GetClass();
	assert(pClass);

	CLASSMEMBERID mid = pClass->FindClassMember(propName);
	if (mid == NULL)
	{
		// ANMMSG_ESUNKNOWNPROPERTY : "Unknown property: %s"
		char *errmsg = (char *) malloc(strlen(ANMMSG_ESUNKNOWNPROPERTY) + 
			strlen(propName) +
			1);

		sprintf(errmsg, ANMMSG_ESUNKNOWNPROPERTY, propName);

		CAnmError err(errmsg);

		delete errmsg;

		throw err;
	}

	assert(mid->memberType == eAnmValueMember);
	ESGETPROPPOINTER pGP = (ESGETPROPPOINTER) mid->pGetValue;

	assert(pGP);

	return ((*pGP) (this));
}

void CAnmESBuiltinObject::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	// ANMMSG_ESOBJDOESNTSUPPORTARRAY : "Object type does not support array properties"
	throw CAnmError(ANMMSG_ESOBJDOESNTSUPPORTARRAY);
}

AnmESVar CAnmESBuiltinObject::GetArrayProperty(AnmESVar vindex)
{
	// ANMMSG_ESOBJDOESNTSUPPORTARRAY : "Object type does not support array properties"
	throw CAnmError(ANMMSG_ESOBJDOESNTSUPPORTARRAY);

	AnmESVar v = NULL;
	return v;
}

AnmESVar CAnmESBuiltinObject::CallMethod(char *methodName, int nArgs, AnmESVar *args)
{
	CAnmClass *pClass = GetClass();
	assert(pClass);

	CLASSMEMBERID mid = pClass->FindClassMember(methodName);
	if (mid == NULL)
	{
		// ANMMSG_ESUNKNOWNMETHOD : "Unknown method call: %s"
		char *errmsg = (char *) malloc(strlen(ANMMSG_ESUNKNOWNMETHOD) + 
			strlen(methodName) +
			1);

		sprintf(errmsg, ANMMSG_ESUNKNOWNMETHOD, methodName);

		CAnmError err(errmsg);

		delete errmsg;

		throw err;
	}

	assert(mid->memberType == eAnmMethodMember);
	ESMETHODPOINTER pMth = (ESMETHODPOINTER) mid->pCallMethod;

	assert(pMth);

	return ((*pMth) (this, nArgs, args));

}

// User object
#define MAXITOABUF 20

CAnmESUserObject::CAnmESUserObject(CAnmECMAScriptLanguageBinding *pOwner)
	: CAnmESObject(pOwner)
{
	m_objType = eUserObject;
}

CAnmESUserObject::CAnmESUserObject(class CAnmECMAScriptLanguageBinding *pOwner, int nArrayElements, 
	CAnmESObject **ppArrayElements)
	: CAnmESObject(pOwner)

{
	assert(ppArrayElements != NULL);

	char propName[MAXITOABUF];
	for (int i = 0; i < nArrayElements; i++)
	{
		itoa(i, propName, 10);

		String s = new CAnmString(propName);
		CAnmESString *pESStr = new CAnmESString(s, pOwner);
		// drop second ref
		s->UnRef();

		m_map[s->GetBuf()] = ppArrayElements[i];
	}
}

CAnmESUserObject::~CAnmESUserObject()
{
}

CAnmESObject *CAnmESUserObject::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	return new CAnmESUserObject(pOwner);
}

void CAnmESUserObject::PutProperty(char *propName, AnmESVar propValue)
{
	if (!strcmp(propName, "length"))
	{
		setLength(propValue);
		return;
	}

	m_map[propName] = propValue;
}

AnmESVar CAnmESUserObject::GetProperty(char *propName)
{
	if (!strcmp(propName, "length"))
		return getLength();

	tAnmESAssocMap::iterator iter = m_map.find(propName);
	if (iter != m_map.end())
		return m_map[propName];
	else
		return (CAnmESObject *) NULL;
}

void CAnmESUserObject::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	CAnmESString *pIndexStr = vindex.toString(m_owner);

	String s = pIndexStr->GetString();
	char *propName = s->GetBuf();

	if (propName && strlen(propName))
	{
		if (!strcmp(propName, "length"))
		{
			setLength(propValue);
			return;
		}

		m_map[propName] = propValue;
	}
}

AnmESVar CAnmESUserObject::GetArrayProperty(AnmESVar vindex)
{
	CAnmESString *pIndexStr = vindex.toString(m_owner);

	String s = pIndexStr->GetString();
	char *propName = s->GetBuf();

	if (propName && strlen(propName))
	{
		if (!strcmp(propName, "length"))
		{
			return getLength();
		}

		tAnmESAssocMap::iterator iter = m_map.find(propName);
		if (iter != m_map.end())
			return m_map[propName];
		else
			return (CAnmESObject *) NULL;
	}
	else
		return NULL;
}

AnmESVar CAnmESUserObject::CallMethod(char *methodName, int nArgs, AnmESVar *args)
{
	if (!strcmp(methodName, "toString"))
	{
		return toString(nArgs, args);
	}

	// NIY for right this second
	return (AnmESVar) NULL;

}

AnmESVar CAnmESUserObject::getLength()
{
	int sz = m_map.size();

	return (AnmESVar) (double) sz;
}

void CAnmESUserObject::setLength(AnmESVar v)
{
	// N.B.: do this later
}

AnmESVar CAnmESUserObject::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString("(Object)"), m_owner);
}

// Tables for Callbacks, Methods and Put/GetProperties
// Builtin object
DEFINE_ABSTRACT_EMPTY_ROOT_CLASS(CAnmESBuiltinObject);

// User object
BEGIN_DEFINE_MEMBERS(CAnmESUserObject)
ESPROPID(CAnmESUserObject, length),
ESMETHODID(CAnmESUserObject, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_ROOT_CLASS(CAnmESUserObject);
DEFINE_ESPROPERTY(CAnmESUserObject, length, getLength, setLength);
DEFINE_ESMETHOD(CAnmESUserObject, toString);



