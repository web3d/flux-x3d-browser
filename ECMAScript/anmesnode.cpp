/********************************************************************************
 * Flux
 *
 * File: anmesnode.cpp
 * Description: ECMAScript node object
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
#include "anmesnode.h"
#include "anmeslanguagebinding.h"
#include "anmesmessage.h"
#include "anmesstring.h"
#include "anmfields.h"
#include "anmnode.h"
#include "anmsainode.h"
#include "anmscript.h"

// Node object
CAnmESNodeObject::CAnmESNodeObject(CAnmSAINode *pNode, CAnmECMAScriptLanguageBinding *pOwner) : 
	CAnmESBuiltinObject(pNode, pOwner, eNodeObject)
{
}

CAnmESNodeObject::~CAnmESNodeObject()
{
}


void CAnmESNodeObject::PutProperty(char *propName, AnmESVar propValue)
{
	CAnmSAINode *pSAINode = GetNode();
	assert(pSAINode);

	CAnmNode *pNode = pSAINode->GetNode();

	if (pNode == NULL)
		// ANMMSG_ESATTEMPTSETPROPNULLNODE : "Attempt to set property of NULL node"
		throw CAnmError(ANMMSG_ESATTEMPTSETPROPNULLNODE);

	CAnmClass *pClass = pNode->GetClass();
	assert(pClass != NULL);

	CLASSMEMBERID mid = pClass->FindClassMember(propName);
	if (mid == NULL)
	{
		// ANMMSG_ESATTEMPTSETUNKNOWNPROP : "Attempt to set value of unknown property '%s'"
		char *errmsg = (char *) malloc(strlen(ANMMSG_ESATTEMPTSETUNKNOWNPROP) + 
			strlen(propName) +
			1);

		sprintf(errmsg, ANMMSG_ESATTEMPTSETUNKNOWNPROP, propName);

		CAnmError err(errmsg);

		delete errmsg;

		throw err;
	}

	void *val = NULL;

	int ival;
	Boolean bval;
	double dval;
	float fval;
	CAnmNode *pNodeVal = NULL;
	CAnmESString *pStr;
	String s = NULL;

	if (mid->valueType == eValueNode)
	{
		pNodeVal = (CAnmNode *) propValue;
		val = &pNodeVal;
	}
	else if (mid->valueType == eValueInteger)
	{
		ival = (int) (double) propValue;
		val = &ival;
	}
	else if (mid->valueType == eValueBoolean)
	{
		bval = (bool) (double) propValue;
		val = &bval;
	}
	else if (mid->valueType == eValueFloat)
	{
		fval = (float) (double) propValue; 
		val = &fval;
	}
	else if (mid->valueType == eValueTime)
	{
		dval = (double) propValue; 
		val = &dval;
	}
	else if (mid->valueType == eValueString)
	{
		pStr = propValue;
		if (pStr == NULL)
			pStr = new CAnmESString(new CAnmString(""), m_owner);
		s = pStr->GetString();
		val = &s;
	}
	else
	{
		CAnmField *pField = AnmESVarToField(propValue);
		if (pField)
		{
			if (pField->GetType() == mid->valueType)
				val = pField->GetData();
			else
				// ANMMSG_ESINCOMPATIBLEFIELDTYPES : "Incompatible field types"
				throw CAnmError(ANMMSG_ESINCOMPATIBLEFIELDTYPES);
		}
		else
			// ANMMSG_ESEXPECTEDFIELDOBJECT : "Expected field object"
			throw CAnmError("Expected field object");
	}

	CAnmScript *pOwner = m_owner->GetScriptNode();
	if (mid->memberType == eAnmMethodMember && pNode == pOwner)
	{
		// N.B.: Prevent recursion of the buffered changed list. If I'm in here
		// it's because some script triggered a call to another one of
		// its methods; time to flush the change buffer and directly
		// call the method
		pOwner->FlushBuffer();
		pNode->CallMethod(mid, val);
	}
	else
	{
		CAnmField *pF = pSAINode->GetField(mid);
		assert(pF != NULL);

		pF->SetData(val);
	}
}


AnmESVar CAnmESNodeObject::GetProperty(char *propName)
{
	CAnmSAINode *pSAINode = GetNode();
	assert(pSAINode);

	CAnmNode *pNode = pSAINode->GetNode();

	if (pNode == NULL)
		// ANMMSG_ESATTEMPTGETPROPNULLNODE : "Attempt to get property of NULL node"
		throw CAnmError(ANMMSG_ESATTEMPTGETPROPNULLNODE);

	CAnmClass *pClass = pNode->GetClass();
	assert(pClass != NULL);

	CLASSMEMBERID mid = pClass->FindClassMember(propName);
	if (mid == NULL)
	{
		// ANMMSG_ESATTEMPTGETUNKNOWNPROP : "Attempt to get value of unknown property '%s'"
		char *errmsg = (char *) malloc(strlen(ANMMSG_ESATTEMPTGETUNKNOWNPROP) + 
			strlen(propName) +
			1);

		sprintf(errmsg, ANMMSG_ESATTEMPTGETUNKNOWNPROP, propName);

		CAnmError err(errmsg);

		delete errmsg;

		throw err;
	}

	AnmESVar v;

	CAnmField *pF = pSAINode->GetField(mid);
	assert(pF != NULL);
	v = AnmESVarFromField(pF, m_owner);

	
	return v;
}

AnmESVar CAnmESNodeObject::CallMethod(char *methodName, int nArgs, AnmESVar *args)
{
	// Just return 0-valued var
	AnmESVar retv;
	if (nArgs < 1)
	{
		if (!strcmp(methodName, "getBoundingBoxCenter"))
		{
			CAnmSAINode *pSAINode = GetNode();
			assert(pSAINode);

			CAnmNode *pNode = pSAINode->GetNode();

			if (pNode == NULL)
				// ANMMSG_ESATTEMPTGETPROPNULLNODE : "Attempt to get property of NULL node"
				throw CAnmError(ANMMSG_ESATTEMPTGETPROPNULLNODE);

			CAnmBoundingBox bbox;
			pNode->GetBoundingBox(&bbox);

			Point3 center, size;
			bbox.GetCenterSize(&center, &size);

			CAnmSFVec3f *pSFVec3f = new CAnmSFVec3f();
			pSFVec3f->SetValue(center);

			retv = AnmESVarFromField(pSFVec3f, m_owner);
		}
		else if (!strcmp(methodName, "getBoundingBoxSize"))
		{
			CAnmSAINode *pSAINode = GetNode();
			assert(pSAINode);

			CAnmNode *pNode = pSAINode->GetNode();

			if (pNode == NULL)
				// ANMMSG_ESATTEMPTGETPROPNULLNODE : "Attempt to get property of NULL node"
				throw CAnmError(ANMMSG_ESATTEMPTGETPROPNULLNODE);

			CAnmBoundingBox bbox;
			pNode->GetBoundingBox(&bbox);

			Point3 center, size;
			bbox.GetCenterSize(&center, &size);

			CAnmSFVec3f *pSFVec3f = new CAnmSFVec3f();
			pSFVec3f->SetValue(size);

			retv = AnmESVarFromField(pSFVec3f, m_owner);
		}

		// N.B.: should issue warning here (or error?)
		return retv;
	}

	if (nArgs > 1)
	{
		// N.B.: should issue warning here, too
	}

	PutProperty(methodName, args[0]);

	return retv;
}

CAnmSAINode *CAnmESNodeObject::GetNode()
{
	return (CAnmSAINode *) m_object;
}

// Tables for Callbacks, Methods and Put/GetProperties
