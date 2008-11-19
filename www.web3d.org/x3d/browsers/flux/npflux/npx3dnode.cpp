/********************************************************************************
 * Flux
 *
 * File: npx3dnode.cpp
 * Description: Mozilla X3DNode
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
#include "npx3dnode.h"
#include "npx3dexecutioncontext.h"
#include "npx3dfield.h"
 
#include "anmnode.h"
#include "anmsainode.h"
#include "anmfields.h"

/////////////////////////////////////////////////////////////////////////////
// CNPX3DNode

CNPX3DNode::~CNPX3DNode()
{
	SafeUnRef(m_node);

	if (m_context)
		NPN_ReleaseObject(m_context);
}

void CNPX3DNode::SetNode(CAnmSAINode *pNode)
{
	SafeUnRef(m_node);

	m_node = pNode;

	if (m_node)
		m_node->Ref();
}

CAnmSAINode *CNPX3DNode::GetNode()
{
	return m_node;
}

	
void CNPX3DNode::SetContext(CNPX3DExecutionContext *pContext)
{
	if (m_context)
		NPN_ReleaseObject(m_context);

	m_context = pContext;

	if (m_context)
		NPN_RetainObject(m_context);
}

CNPX3DExecutionContext *CNPX3DNode::GetContext()
{
	if (m_context)
		NPN_RetainObject(m_context);

	return m_context;
}


bool CNPX3DNode::getFieldDefinitions(class CNPX3DFieldDefinitionArray **fieldDefinitions)
{
	return false;
}

bool CNPX3DNode::getFieldInfo(int fieldIndex, NPX3DFieldAccess *accessType, NPX3DFieldType *fieldType, char **fieldName)
{
	return false;
}

bool CNPX3DNode::getNumFields(int *result)
{
	return false;
}

bool CNPX3DNode::getName(const char **name)
{
	if (m_node == NULL)
		return false;

	CAnmNode *pNode = m_node->GetNode();
	const char *nm = pNode->GetName()->GetBuf();

	if (nm && strlen(nm))
	{
		*name = nm;

		return true;
	}
	else
		return false;

}

static CNPX3DField *createMember(NPP pNpp, eValueType valueType)
{
	CNPX3DField *pX3DField = NULL;

	switch (valueType)
	{
		case eValueBoolean :
			pX3DField = CNPX3DSFBool::Create(pNpp);
			break;

		case eValueColor :
			pX3DField = CNPX3DSFColor::Create(pNpp);
			break;

		case eValueFloat :
			pX3DField = CNPX3DSFFloat::Create(pNpp);
			break;

		case eValueInteger :
			pX3DField = CNPX3DSFInt32::Create(pNpp);
			break;

		case eValueNode :
			pX3DField = CNPX3DSFNode::Create(pNpp);
			break;

		case eValuePoint2 :
			pX3DField = CNPX3DSFVec2f::Create(pNpp);
			break;

		case eValuePoint3 :
			pX3DField = CNPX3DSFVec3f::Create(pNpp);
			break;

		case eValueRotation :
			pX3DField = CNPX3DSFRotation::Create(pNpp);
			break;

		case eValueString :
			pX3DField = CNPX3DSFString::Create(pNpp);
			break;

		case eValueTime :
			pX3DField = CNPX3DSFTime::Create(pNpp);
			break;

		case eValueColorArray :
			pX3DField = CNPX3DMFColor::Create(pNpp);
			break;

		case eValueFloatArray :
			pX3DField = CNPX3DMFFloat::Create(pNpp);
			break;

		case eValueIntegerArray :
			pX3DField = CNPX3DMFInt32::Create(pNpp);
			break;

		case eValueNodeArray :
			pX3DField = CNPX3DMFNode::Create(pNpp);
			break;

		case eValueRotationArray :
			pX3DField = CNPX3DMFRotation::Create(pNpp);
			break;

		case eValueStringArray :
			pX3DField = CNPX3DMFString::Create(pNpp);
			break;

		case eValuePoint2Array :
			pX3DField = CNPX3DMFVec2f::Create(pNpp);
			break;
		
		case eValuePoint3Array :
			pX3DField = CNPX3DMFVec3f::Create(pNpp);
			break;

		default :
			assert(FALSE);
			pX3DField = NULL;
			break;
	}


	return pX3DField;
}

bool CNPX3DNode::getField(const char *fieldname, class CNPX3DField **result)
{
	if (m_node == NULL)
		return false;

	if (!fieldname || !strlen(fieldname))
		return false;

	CAnmField *pField = m_node->GetField(fieldname);

	if (pField == NULL)
		return false;
	else
	{
		CNPX3DField *pX3DField = createMember(mNpp, pField->GetType());

		if (pX3DField)
		{
			NPN_RetainObject(pX3DField);
			*result = pX3DField;

			CAnmNode *pNode = m_node->GetNode();
			assert(pNode);

			CAnmClass *pClass = pNode->GetClass();
			assert(pClass);

			CLASSMEMBERID mid = pClass->FindClassMember((char *)fieldname);

			(*result)->attach(m_context, m_node, pField, mid, mid->valueType);
		}
		else
			return false;

	}

	return true;
}

bool CNPX3DNode::getTypeName(const char **typenm)
{
	const char *nm = NULL;

	if (m_node == NULL)
		nm = "UNKNOWN NODE TYPE";
	else
	{
		CAnmNode *pNode = m_node->GetNode();
		if (pNode)
			nm = pNode->GetClass()->m_className;
	}

	if (nm && strlen(nm))
	{
		*typenm = nm;

		return true;
	}
	else
		return false;


}

// NP Glue

DEFINE_NPMETHODID(CNPX3DNode,getFieldDefinitions);
DEFINE_NPMETHODID(CNPX3DNode,getFieldInfo);
DEFINE_NPMETHODID(CNPX3DNode,getNumFields);
DEFINE_NPMETHODID(CNPX3DNode,getName);
DEFINE_NPMETHODID(CNPX3DNode,getField);
DEFINE_NPMETHODID(CNPX3DNode,getTypeName);

bool CNPX3DNode::m_classInitialized = false;

static NPObject *
AllocateCNPX3DNode(NPP npp, NPClass *aClass)
{
  return new CNPX3DNode(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DNode,
                                 AllocateCNPX3DNode);

CNPX3DNode *CNPX3DNode::Create(NPP npp, CAnmSAINode *pSAINode, CNPX3DExecutionContext *pContext)
{
	CNPX3DNode *pX3DNode = (CNPX3DNode *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DNode));

	if (pX3DNode)
	{
		pX3DNode->SetNode(pSAINode);
		pX3DNode->SetContext(pContext);
	}

	return pX3DNode;
}

void CNPX3DNode::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPMETHODID(getFieldDefinitions);
	INIT_NPMETHODID(getFieldInfo);
	INIT_NPMETHODID(getNumFields);
	INIT_NPMETHODID(getName);
	INIT_NPMETHODID(getField);
	INIT_NPMETHODID(getTypeName);

	m_classInitialized = true;
}

bool
CNPX3DNode::HasMethod(NPIdentifier name)
{
  return 
	  TEST_NPMETHODID(name, getFieldDefinitions) ||
	  TEST_NPMETHODID(name, getFieldInfo) ||
	  TEST_NPMETHODID(name, getNumFields) ||
	  TEST_NPMETHODID(name, getName) ||
	  TEST_NPMETHODID(name, getField) ||
	  TEST_NPMETHODID(name, getTypeName) ||
		false	  
	  ;

}

bool
CNPX3DNode::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	const char *cpret = NULL;
	String s0 = NULL;

	if (TEST_NPMETHODID(name,getName)) {

		if (getName(&cpret))
		{
			STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
		}

		return PR_TRUE;
	}
	else if (TEST_NPMETHODID(name,getTypeName)) {
		if (getTypeName(&cpret))
		{
			STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
		}

		return PR_TRUE;
	}
	else if (TEST_NPMETHODID(name,getField)) {

		if (argCount > 0)
		{
			s0 = NPVToString(args[0]);

			if (s0)
			{
				CNPX3DField *pField = NULL;
				if (getField(s0->GetBuf(), &pField))
				{
					OBJECT_TO_NPVARIANT(pField, *result);
				}

				SafeUnRef(s0);

				return PR_TRUE;
			}
			else
			  return PR_FALSE;
		}
		else
		  return PR_FALSE;
	}

  return PR_FALSE;
}

bool
CNPX3DNode::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  STRINGZ_TO_NPVARIANT(NPstrdup("[NPX3DNode]"), *result);

  return PR_TRUE;
}

bool
CNPX3DNode::HasProperty(NPIdentifier name)
{
	if (m_node == NULL)
		return PR_FALSE;

	if (NPN_IdentifierIsString(name))
	{
		char *fieldname = NPN_UTF8FromIdentifier(name);

		CAnmField *pField = m_node->GetField(fieldname);

		if (pField)
			return PR_TRUE;
		else
			return PR_FALSE;
	}
	else
		return PR_FALSE;

  return PR_FALSE;
}

bool
CNPX3DNode::GetProperty(NPIdentifier name, NPVariant *result)
{
	if (m_node == NULL)
		return PR_FALSE;

	if (NPN_IdentifierIsString(name))
	{
		char *fieldname = NPN_UTF8FromIdentifier(name);

		return getFieldAsVariant(fieldname, result);
	}
	else
		return PR_FALSE;

  return PR_FALSE;
}

bool
CNPX3DNode::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
	if (m_node == NULL)
		return PR_FALSE;

	if (NPN_IdentifierIsString(name))
	{
		char *fieldname = NPN_UTF8FromIdentifier(name);

		return setFieldFromVariant(fieldname, value);
	}
	else
		return PR_FALSE;

  return PR_FALSE;
}

bool CNPX3DNode::getFieldAsVariant(char *fieldname, NPVariant *result)
{
	if (!fieldname || !strlen(fieldname) || !result)
		return PR_FALSE;

	CAnmField *pField = m_node->GetField(fieldname);

	if (pField)
	{
		switch (pField->GetType())
		{
			case eValueBoolean :
				{
					bool *pB = ((CAnmSFBool *)pField)->GetValue();
					assert(pB);

					BOOLEAN_TO_NPVARIANT(*pB, *result);
				}
				break;

			case eValueDouble :
				{
					double *pD = ((CAnmSFDouble *)pField)->GetValue();
					assert(pD);

					DOUBLE_TO_NPVARIANT(*pD, *result);
				}
				break;

			case eValueFloat :
				{
					float *pF = ((CAnmSFFloat *)pField)->GetValue();
					assert(pF);

					double d = (double) *pF;

					DOUBLE_TO_NPVARIANT(d, *result);
				}
				break;

			case eValueInteger :
				{
					Integer *pI = ((CAnmSFInt32 *)pField)->GetValue();
					assert(pI);

					INT32_TO_NPVARIANT(*pI, *result);
				}
				break;

			case eValueString :
				{
					String *pS = ((CAnmSFString *)pField)->GetValue();
					assert(pS);

					char *cp = (*pS)->GetBuf();

					STRINGZ_TO_NPVARIANT(NPstrdup(cp), *result);
				}
				break;

			case eValueTime :
				{
					Time *pT = ((CAnmSFTime *)pField)->GetValue();
					assert(pT);

					double d = *pT;

					DOUBLE_TO_NPVARIANT(d, *result);
				}
				break;

			default :
				{
					CNPX3DField *pX3DField = NULL;
					if(getField(fieldname, &pX3DField))
					{
						OBJECT_TO_NPVARIANT(pX3DField, *result);
					}
				}
				break;
		}
		return PR_TRUE;
	}
	else
		return PR_FALSE;
}

bool CNPX3DNode::setFieldFromVariant(char *fieldname, const NPVariant *value)
{
	if (!fieldname || !strlen(fieldname) || !value)
		return PR_FALSE;

	CAnmField *pField = m_node->GetField(fieldname);

	if (pField)
	{
		eValueType valueType = pField->GetType();

		switch (valueType)
		{
			case eValueBoolean :
				{
					bool b = NPVToBoolean(*value);

					((CAnmSFBool *)pField)->SetValue(b);
				}
				break;

			case eValueDouble :
				{
					double d = NPVToDouble(*value);
					
					((CAnmSFDouble *)pField)->SetValue(d);
				}
				break;

			case eValueFloat :
				{
					double d = NPVToDouble(*value);
					
					((CAnmSFFloat *)pField)->SetValue((float) d);
				}
				break;

			case eValueInteger :
				{
					Integer i32 = NPVToInteger(*value);

					((CAnmSFInt32 *)pField)->SetValue(i32);
				}
				break;

			case eValueString :
				{
					String s = NPVToString(*value);

					((CAnmSFString *)pField)->SetValue(s);

					s->UnRef();
				}
				break;

			case eValueTime :
				{
					double d = NPVToDouble(*value);

					((CAnmSFTime *)pField)->SetValue(d);
				}
				break;

			default :
				{
					NPObject *pObject = NPVToObject(*value);

					// N.B.: how far can I get without this?!
					if (pObject)
					{						
						if (NPOBJECT_ISA(pObject, CNPX3DSFColor) && valueType == eValueColor)
						{
						}
					}
				}
				break;
		}


		return PR_TRUE;
	}
	else
		return PR_FALSE;
}