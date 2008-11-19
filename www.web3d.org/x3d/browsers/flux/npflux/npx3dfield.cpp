/********************************************************************************
 * Flux
 *
 * File: npx3dfield.cpp
 * Description: Mozilla X3DField
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
#include "npx3dfield.h"
#include "npx3dnode.h"
#include "npx3dexecutioncontext.h"
 
#include "anmnode.h"
#include "anmsainode.h"
#include "anmfields.h"
#include "animaapp.h"
#include "anmexecutioncontext.h"
#include "anmbrowser.h"
#include "anmtime.h"
#include "anmworld.h"

/////////////////////////////////////////////////////////////////////////////
// CNPX3DNode

CNPX3DField::~CNPX3DField()
{
	SafeUnRef(m_node);
	SafeUnRef(m_field);
	SafeUnRef(m_savedfield);

	if (m_mid && m_mid->memberType == eAnmCallbackMember && m_callback
		&& m_node)
	{
		CAnmNode *pNode = m_node->GetNode();
		pNode->RemoveCallback(m_mid, m_callback);
	}

	if (m_context)
		NPN_ReleaseObject(m_context);

	std::list< sAnmObserverCallback >::iterator iter;
	for( 
		iter = m_observers.begin(); 
		iter != m_observers.end();
		iter++ )
	{
		NPObject *pListener = (*iter).m_observer;
		// N.B. this doesn't seem like a good idea; Mozilla apparently blows
		// these away before we even destroy the field. Is there a way to
		// check for an invalid object pointer?
		if (false) // pListener)
			NPN_ReleaseObject(pListener);
	}

}

void CNPX3DField::attach(class CNPX3DExecutionContext *pContext, class CAnmSAINode *pNode, class CAnmField *pField,
		CLASSMEMBERID mid, eValueType valueType)
{

	// Attach the context
	if (m_context)
		NPN_ReleaseObject(m_context);

	m_context = pContext;

	if (m_context)
		NPN_RetainObject(m_context);

	// Attach the node
	SafeUnRef(m_node);

	m_node = pNode;

	if (m_node)
		m_node->Ref();

	// Attach the field
	SafeUnRef(m_field);

	m_field = pField;

	if (m_field)
		m_field->Ref();

	m_fieldtype = valueType;
	m_mid = mid;

	if (pNode && mid)
	{
		if (mid->memberType == eAnmCallbackMember)
		{
			CAnmNode *pNode = m_node->GetNode();
			m_callback = pNode->AddCallback(mid, fieldBaseCallback, this);
		}

		if (mid->memberType == eAnmCallbackMember || mid->memberType == eAnmMethodMember)
			m_savedfield = CAnmField::CreateField(valueType, m_field->GetData());

	}
	else
	{
		m_savedfield = CAnmField::CreateField(valueType, NULL);
	}
}

void CNPX3DField::fieldBaseCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData, void *userData)
{
	CNPX3DField *pfb = (CNPX3DField *) userData;
	pfb->handleCallback(sourceObject, reason, callData);
}

void CNPX3DField::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData)
{
	assert(m_savedfield);

	m_savedfield->SetData(callData);

	callCallbacks(this);
}

void CNPX3DField::callCallbacks(CNPX3DField *pField)
{
	CAnmExecutionContext *pContext = NULL;
	m_context->GetContext(&pContext);

	if (pContext)
	{
		cApplication *pApp = pContext->GetApp();

		assert(pApp);

		CAnmTime *pClock = pApp->GetClock();
		assert(pClock);

		Time timestamp = pClock->GetCurrentTime();

		sAnmThreadedCallback *pTCB = new sAnmThreadedCallback;
		pTCB->pField = pField;
		pTCB->timestamp = timestamp;
		pApp->PostMessage( eAppMsgCallFunction, pTCB );
	}
}

void CNPX3DField::CallX3DCallback(sAnmThreadedCallback *pTCB)
{
	if (pTCB)
	{
		pTCB->pField->callThreadedCallbacks(pTCB->pField, pTCB->timestamp);
		delete pTCB;
	}
}

static NPIdentifier sReadableFieldChanged = 0;

void CNPX3DField::callThreadedCallbacks(CNPX3DField *pField, double timestamp)
{
	int sz = m_observers.size();

	std::list< sAnmObserverCallback >::iterator iter;
	for( 
		iter = m_observers.begin(); 
		iter != m_observers.end();
		iter++ )
	{
		NPObject *pObserver = (*iter).m_observer;

		if (!sReadableFieldChanged)
			sReadableFieldChanged = NPN_GetStringIdentifier("readableFieldChanged");

		bool stat = NPN_HasMethod(mNpp,pObserver,sReadableFieldChanged);

		if (stat)
		{
			NPVariant args[2];
			OBJECT_TO_NPVARIANT(pField, args[0]);
			DOUBLE_TO_NPVARIANT(timestamp, args[1]);
			NPVariant result;

			// Don't think we want this; 
			NPN_RetainObject(pObserver);
			stat = NPN_Invoke(mNpp, pObserver,sReadableFieldChanged, args, 2, &result);
			NPN_ReleaseVariantValue(&result);
		}
	}
}

eAnmFieldBaseSetValue CNPX3DField::getValue(void **pVal)
{
	if (m_mid == NULL || m_node == NULL || pVal == NULL)
	{
		if (m_savedfield)
			m_savedfield->Copy(pVal);

		return ANMFIELDBASE_DIY;
	}

	if (m_mid->memberType == eAnmValueMember || m_mid->memberType == eAnmInitMember)
	{
		CAnmNode *pNode = m_node->GetNode();
		assert(pNode);

		// If the node is in the scene graph already, we need to
		// make sure we're synced up with the latest simulation state
		CAnmWorld *pWorld = pNode->GetWorld();
		if (pWorld)
		{
			cAnimaApp *pApp = (cAnimaApp *) pWorld->GetApp();
			assert(pApp);

			pApp->WaitForExternalEvents();

			pApp->Lock();

			pNode->GetValue(m_mid, pVal);

			pApp->Unlock();
		}
		else
		{
			pNode->GetValue(m_mid, pVal);
		}
	}
	else if (m_mid->memberType == eAnmCallbackMember || m_mid->memberType == eAnmMethodMember )
	{
		if (m_savedfield)
		{
			m_savedfield->Copy(pVal);
			return ANMFIELDBASE_DID;
		}
		else
			return ANMFIELDBASE_CANTDO;
	}
	else
		return ANMFIELDBASE_CANTDO;

	return ANMFIELDBASE_DID;
}

eAnmFieldBaseSetValue CNPX3DField::setValue(void *pVal)
{
	if (m_savedfield)
		m_savedfield->SetData(pVal);

	if (m_field)
	{
		CAnmNode *pNode = m_node->GetNode();
		assert(pNode);

		// If the node is in the scene graph already, we need to
		// make sure we're synced up with the latest simulation state
		CAnmWorld *pWorld = pNode->GetWorld();
		if (pWorld)
		{
			cAnimaApp *pApp = (cAnimaApp *) pWorld->GetApp();
			assert(pApp);

			pApp->Lock();

			m_field->SetData(pVal);

			pApp->Unlock();
		}
		else
		{
			m_field->SetData(pVal);
		}

		return ANMFIELDBASE_DID;
	}
	else
	{
		return ANMFIELDBASE_DIY;
	}
}


bool CNPX3DField::getValue(NPVariant *value)
{
	assert(m_field);

	void *pVal = NULL;
	Boolean *pB = NULL;
	Color *pC = NULL;
	Double *pD = NULL;
	Float *pF = NULL;
	Double d = 0.f;
	// Image img;
	Integer *pI32 = NULL;
	Node *pNode = NULL;
	Rotation *pR = NULL;
	String *pS = NULL;
	char *cp = NULL;
	Time *pT = NULL;
	Point2 *pP2 = NULL;
	Point3 *pP3 = NULL;
	void **pPtr = NULL;

	CAnmField *pField = NULL;

	assert(m_field);
	assert(m_mid);
	if (m_savedfield)
		pField = m_savedfield;
	else
		pField = m_field;

	switch (m_fieldtype)
	{
		case eValueBoolean :
			pB = (Boolean *) pField->GetData();
			BOOLEAN_TO_NPVARIANT(*pB, *value);
			break;

		case eValueDouble :
			pD = (Double *) pField->GetData();
			DOUBLE_TO_NPVARIANT(*pD, *value);
			break;

		case eValueFloat :
			pF = (Float *) pField->GetData();
			d = *pF;
			DOUBLE_TO_NPVARIANT(d, *value);
			break;

		case eValueInteger :
			pI32 = (Integer *) pField->GetData();
			INT32_TO_NPVARIANT(*pI32, *value);
			break;

		case eValueNode :
			{
				pNode = (Node *) pField->GetData();

				CAnmSAINode *pSAINode = new CAnmSAINode(*pNode);
				pSAINode->SetSink(m_node->GetSink());

				CNPX3DNode *pX3DNode = CNPX3DNode::Create(mNpp, pSAINode, m_context);

				if (pX3DNode)
				{
					NPN_RetainObject(pX3DNode);

					OBJECT_TO_NPVARIANT(pX3DNode, *value);
				}
			}
			break;

		case eValueString :
			pS = (String *) pField->GetData();
			assert(pS);
			assert(*pS);
			cp = (*pS)->GetBuf();
			STRINGZ_TO_NPVARIANT(NPstrdup(cp), *value);
			break;

		case eValueTime :
			pT = (Time *) pField->GetData();
			DOUBLE_TO_NPVARIANT(*pT, *value);
			break;

		case eValueColor :
			assert(FALSE);
			// pVal = &c;
			break;

		case eValuePoint2 :
			assert(FALSE);
			// pVal = &p2;
			break;

		case eValuePoint3 :
			assert(FALSE);
			// pVal = &p3;
			break;

		case eValueRotation :
			assert(FALSE);
			// pVal = &r;
			break;

		case eValueBooleanArray :
		case eValueColorArray :
		case eValueDoubleArray :
		case eValueIntegerArray :
		case eValueFloatArray :
		case eValueNodeArray :
		case eValuePoint2Array :
		case eValuePoint3Array :
		case eValueRotationArray :
		case eValueStringArray :
		case eValueTimeArray :

			assert(FALSE);
			// don't know what to do yet
			break;

		default :
			assert(FALSE);	// what the hell?
			break;
	}

	return true;
}

bool CNPX3DField::setValue(NPVariant value)
{
	void *pVal = NULL;
	Boolean b;
	Color c;
	Double d;
	Float f;
	// Image img;
	Integer i32;
	Rotation r;
	String s;
	Time t;
	Point2 p2;
	Point3 p3;
	void *ptr = NULL;

	assert(m_field);
	assert(m_mid);

	switch (m_fieldtype)
	{
		case eValueBoolean :
			b = NPVToBoolean(value);
			pVal = &b;
			break;

		case eValueDouble :
			d = NPVToDouble(value);
			pVal = &d;
			break;

		case eValueInteger :
			i32 = NPVToInteger(value);
			pVal = &i32;
			break;

		case eValueFloat :
			f = (float) NPVToDouble(value);
			pVal = &f;
			break;

		case eValueString :
			s = NPVToString(value);
			pVal = &s;
			break;

		case eValueTime :
			t = NPVToDouble(value);
			pVal = &t;
			break;

		case eValueColor :
			// pVal = &c;
			break;

		case eValuePoint2 :
			// pVal = &p2;
			break;

		case eValuePoint3 :
			// pVal = &p3;
			break;

		case eValueRotation :
			// pVal = &r;
			break;

		case eValueBooleanArray :
		case eValueColorArray :
		case eValueDoubleArray :
		case eValueIntegerArray :
		case eValueFloatArray :
		case eValueNode :
		case eValueNodeArray :
		case eValuePoint2Array :
		case eValuePoint3Array :
		case eValueRotationArray :
		case eValueStringArray :
		case eValueTimeArray :

			// don't know what to do yet
			break;

		default :
			assert(FALSE);	// what the hell?
			break;
	}

	if (pVal)
		m_field->SetData(pVal);

	return true;
}

bool CNPX3DField::getAccessType(NPX3DFieldAccess *accesstype)
{
	if (m_mid)
		*accesstype = getAccessType(m_mid->memberType);
	else
		*accesstype = NPX3DInputOutput;

	return true;
}

bool CNPX3DField::getType(NPX3DFieldType *fieldtype)
{
	*fieldtype = getFieldType(m_fieldtype);

	return true;
}

bool CNPX3DField::getName(const char **name)
{
	assert(name);

	if (m_mid)
		*name = m_mid->name;
	else
		*name = "";

	return true;
}

bool CNPX3DField::dispose()
{
	// N.B.: later
	return false;
}


void CNPX3DField::addFieldEventListener(NPObject *listener)
{
	if (listener)
	{
		m_observers.push_back(sAnmObserverCallback(listener, NULL));

		NPN_RetainObject(listener);
	}
}

void CNPX3DField::removeFieldEventListener(NPObject *listener)
{
	if (listener)
	{
		BOOL found = FALSE;

		std::list< sAnmObserverCallback >::iterator iter;
		for( 
			iter = m_observers.begin(); 
			iter != m_observers.end();
			iter++ )
		{
			if ((*iter).m_observer == listener)
			{
				found = TRUE;
				break;
			}

		}

		if (found)
		{
			m_observers.erase(iter);
			NPN_ReleaseObject(listener);
		}
	}
}


// NP Glue

// CNPX3DField
DEFINE_NPMETHODID(CNPX3DField,addFieldEventListener);
DEFINE_NPMETHODID(CNPX3DField,removeFieldEventListener);
DEFINE_NPMETHODID(CNPX3DField,getValue);
DEFINE_NPMETHODID(CNPX3DField,setValue);
DEFINE_NPMETHODID(CNPX3DField,getAccessType);
DEFINE_NPMETHODID(CNPX3DField,getType);
DEFINE_NPMETHODID(CNPX3DField,getName);
DEFINE_NPMETHODID(CNPX3DField,dispose);

bool CNPX3DField::m_classInitialized = false;

static NPObject *
AllocateCNPX3DField(NPP npp, NPClass *aClass)
{
  return new CNPX3DField(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DField,
                                 AllocateCNPX3DField);


void CNPX3DField::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPMETHODID(addFieldEventListener);
	INIT_NPMETHODID(removeFieldEventListener);
	INIT_NPMETHODID(getValue);
	INIT_NPMETHODID(setValue);
	INIT_NPMETHODID(getAccessType);
	INIT_NPMETHODID(getType);
	INIT_NPMETHODID(getName);
	INIT_NPMETHODID(dispose);

	m_classInitialized = true;
}

bool
CNPX3DField::HasMethod(NPIdentifier name)
{
  return 
	  TEST_NPMETHODID(name, getValue) ||
	  TEST_NPMETHODID(name, setValue) ||
	  TEST_NPMETHODID(name, getAccessType) ||
	  TEST_NPMETHODID(name, getType) ||
	  TEST_NPMETHODID(name, getName) ||
	  TEST_NPMETHODID(name, dispose) ||
	  TEST_NPMETHODID(name, addFieldEventListener) ||
	  TEST_NPMETHODID(name, removeFieldEventListener) ||
		false	  
	  ;

}

bool
CNPX3DField::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	const char *cpret = NULL;
	String s0 = NULL;
	NPObject *pObject = NULL;

	if (TEST_NPMETHODID(name,getValue)) {
		return getValue(result);
	}
	if (TEST_NPMETHODID(name,setValue)) {

		if (argCount > 0)
		{
			return setValue(args[0]);
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPMETHODID(name,getType)) {

		NPX3DFieldType fieldType;

		getType(&fieldType);
		INT32_TO_NPVARIANT(fieldType, *result);

		return PR_TRUE;
	}
	else if (TEST_NPMETHODID(name,getAccessType)) {

		NPX3DFieldAccess fieldAccessType;

		getAccessType(&fieldAccessType);
		INT32_TO_NPVARIANT(fieldAccessType, *result);
	
		return PR_TRUE;
	}
	else if (TEST_NPMETHODID(name,getName)) {

		if (getName(&cpret))
		{
			STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
		}
	
		return PR_TRUE;
	}
	else if (TEST_NPMETHODID(name,addFieldEventListener)) {

		if (argCount > 0)
		{
			pObject = NPVToObject(args[0]);
			addFieldEventListener(pObject);

			return PR_TRUE;
		}
		else
			return PR_FALSE;

	}
	else if (TEST_NPMETHODID(name,removeFieldEventListener)) {

		if (argCount > 0)
		{
			pObject = NPVToObject(args[0]);
			removeFieldEventListener(pObject);

			return PR_TRUE;
		}
		else
			return PR_FALSE;
	}

	return PR_FALSE;
}

bool
CNPX3DField::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  STRINGZ_TO_NPVARIANT(NPstrdup("[X3DField]"), *result);

  return PR_TRUE;
}

bool
CNPX3DField::HasProperty(NPIdentifier name)
{
  return false;
}

bool
CNPX3DField::GetProperty(NPIdentifier name, NPVariant *result)
{
  return false;
}

bool
CNPX3DField::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return false;
}

// CNPX3DSFBool
CNPX3DSFBool::~CNPX3DSFBool()
{
}

bool CNPX3DSFBool::m_classInitialized = false;
static NPObject *
AllocateCNPX3DSFBool(NPP npp, NPClass *aClass)
{
  return new CNPX3DSFBool(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DSFBool,
                                 AllocateCNPX3DSFBool);

CNPX3DSFBool *CNPX3DSFBool::Create(NPP npp)
{
	return (CNPX3DSFBool *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DSFBool));
}


void CNPX3DSFBool::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DSFBool::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DField::HasMethod(name)
	  ;

}

bool
CNPX3DSFBool::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFBool::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFBool::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DField::HasProperty(name);

}

bool
CNPX3DSFBool::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DField::GetProperty(name, result);
}

bool
CNPX3DSFBool::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DField::SetProperty(name, value);
}

// CNPX3DSFColor
CNPX3DSFColor::~CNPX3DSFColor()
{
}

float CNPX3DSFColor::get_r()
{
	Color c;
	getValue((void **) &c);

	return c.r;
}

float CNPX3DSFColor::get_g()
{
	Color c;
	getValue((void **) &c);

	return c.g;
}

float CNPX3DSFColor::get_b()
{
	Color c;
	getValue((void **) &c);

	return c.b;
}

void CNPX3DSFColor::set_r(float f)
{
	Color c;
	getValue((void **) &c);

	c.r = f;

	setValue((void *) &c);
}

void CNPX3DSFColor::set_g(float f)
{
	Color c;
	getValue((void **) &c);

	c.g = f;

	setValue((void *) &c);
}

void CNPX3DSFColor::set_b(float f)
{
	Color c;
	getValue((void **) &c);

	c.b = f;

	setValue((void *) &c);
}


// NP Glue (CNPX3DSFColor)
DEFINE_NPPROPID(CNPX3DSFColor,r);
DEFINE_NPPROPID(CNPX3DSFColor,g);
DEFINE_NPPROPID(CNPX3DSFColor,b);

bool CNPX3DSFColor::m_classInitialized = false;
static NPObject *
AllocateCNPX3DSFColor(NPP npp, NPClass *aClass)
{
  return new CNPX3DSFColor(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DSFColor,
                                 AllocateCNPX3DSFColor);

CNPX3DSFColor *CNPX3DSFColor::Create(NPP npp)
{
	return (CNPX3DSFColor *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DSFColor));
}


void CNPX3DSFColor::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPPROPID(r);
	INIT_NPPROPID(g);
	INIT_NPPROPID(b);

	m_classInitialized = true;
}

bool
CNPX3DSFColor::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DField::HasMethod(name)
	  ;

}

bool
CNPX3DSFColor::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFColor::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFColor::HasProperty(NPIdentifier name)
{
  return 
	  TEST_NPPROPID(name, r) ||
	  TEST_NPPROPID(name, g) ||
	  TEST_NPPROPID(name, b) ||
		CNPX3DField::HasProperty(name);

}

bool
CNPX3DSFColor::GetProperty(NPIdentifier name, NPVariant *result)
{
	double d;

	  if (TEST_NPPROPID(name, r))
	  {
		  d = get_r();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else if (TEST_NPPROPID(name, g))
	  {
		  d = get_g();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else if (TEST_NPPROPID(name, b))
	  {
		  d = get_b();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else
		  return CNPX3DField::GetProperty(name, result);

	  return PR_TRUE;
}

bool
CNPX3DSFColor::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
	double d;

	if (!value)
		return PR_FALSE;

	  if (TEST_NPPROPID(name, r))
	  {
		  d = NPVToDouble(*value);

		  set_r((float) d);
	  }
	  else if (TEST_NPPROPID(name, g))
	  {
		  d = NPVToDouble(*value);

		  set_g((float) d);
	  }
	  else if (TEST_NPPROPID(name, b))
	  {
		  d = NPVToDouble(*value);

		  set_b((float) d);
	  }
	  else
		  return CNPX3DField::SetProperty(name, value);

	  return PR_TRUE;

}

// CNPX3DSFFloat
CNPX3DSFFloat::~CNPX3DSFFloat()
{
}

bool CNPX3DSFFloat::m_classInitialized = false;
static NPObject *
AllocateCNPX3DSFFloat(NPP npp, NPClass *aClass)
{
  return new CNPX3DSFFloat(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DSFFloat,
                                 AllocateCNPX3DSFFloat);

CNPX3DSFFloat *CNPX3DSFFloat::Create(NPP npp)
{
	return (CNPX3DSFFloat *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DSFFloat));
}


void CNPX3DSFFloat::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DSFFloat::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DField::HasMethod(name)
	  ;

}

bool
CNPX3DSFFloat::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFFloat::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFFloat::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DField::HasProperty(name);

}

bool
CNPX3DSFFloat::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DField::GetProperty(name, result);
}

bool
CNPX3DSFFloat::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DField::SetProperty(name, value);
}

// CNPX3DSFInt32
CNPX3DSFInt32::~CNPX3DSFInt32()
{
}

bool CNPX3DSFInt32::m_classInitialized = false;
static NPObject *
AllocateCNPX3DSFInt32(NPP npp, NPClass *aClass)
{
  return new CNPX3DSFInt32(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DSFInt32,
                                 AllocateCNPX3DSFInt32);

CNPX3DSFInt32 *CNPX3DSFInt32::Create(NPP npp)
{
	return (CNPX3DSFInt32 *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DSFInt32));
}


void CNPX3DSFInt32::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DSFInt32::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DField::HasMethod(name)
	  ;

}

bool
CNPX3DSFInt32::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFInt32::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFInt32::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DField::HasProperty(name);

}

bool
CNPX3DSFInt32::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DField::GetProperty(name, result);
}

bool
CNPX3DSFInt32::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DField::SetProperty(name, value);
}

// CNPX3DSFNode
CNPX3DSFNode::~CNPX3DSFNode()
{
}

bool CNPX3DSFNode::m_classInitialized = false;
static NPObject *
AllocateCNPX3DSFNode(NPP npp, NPClass *aClass)
{
  return new CNPX3DSFNode(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DSFNode,
                                 AllocateCNPX3DSFNode);

CNPX3DSFNode *CNPX3DSFNode::Create(NPP npp)
{
	return (CNPX3DSFNode *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DSFNode));
}


void CNPX3DSFNode::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DSFNode::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DField::HasMethod(name)
	  ;

}

bool
CNPX3DSFNode::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFNode::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFNode::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DField::HasProperty(name);

}

bool
CNPX3DSFNode::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DField::GetProperty(name, result);
}

bool
CNPX3DSFNode::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DField::SetProperty(name, value);
}

// CNPX3DSFRotation
CNPX3DSFRotation::~CNPX3DSFRotation()
{
}

float CNPX3DSFRotation::get_x()
{
	Rotation r;
	getValue((void **) &r);

	return r.vec.x;
}

float CNPX3DSFRotation::get_y()
{
	Rotation r;
	getValue((void **) &r);

	return r.vec.y;
}

float CNPX3DSFRotation::get_z()
{
	Rotation r;
	getValue((void **) &r);

	return r.vec.z;
}

float CNPX3DSFRotation::get_angle()
{
	Rotation r;
	getValue((void **) &r);

	return r.theta;
}

void CNPX3DSFRotation::set_x(float f)
{
	Rotation r;
	getValue((void **) &r);

	r.vec.x = f;

	setValue((void *) &r);
}

void CNPX3DSFRotation::set_y(float f)
{
	Rotation r;
	getValue((void **) &r);

	r.vec.y = f;

	setValue((void *) &r);
}

void CNPX3DSFRotation::set_z(float f)
{
	Rotation r;
	getValue((void **) &r);

	r.vec.z = f;

	setValue((void *) &r);
}

void CNPX3DSFRotation::set_angle(float f)
{
	Rotation r;
	getValue((void **) &r);

	r.theta = f;

	setValue((void *) &r);
}

// NP Glue (CNPX3DSFRotation)
DEFINE_NPPROPID(CNPX3DSFRotation,x);
DEFINE_NPPROPID(CNPX3DSFRotation,y);
DEFINE_NPPROPID(CNPX3DSFRotation,z);
DEFINE_NPPROPID(CNPX3DSFRotation,angle);

bool CNPX3DSFRotation::m_classInitialized = false;
static NPObject *
AllocateCNPX3DSFRotation(NPP npp, NPClass *aClass)
{
  return new CNPX3DSFRotation(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DSFRotation,
                                 AllocateCNPX3DSFRotation);

CNPX3DSFRotation *CNPX3DSFRotation::Create(NPP npp)
{
	return (CNPX3DSFRotation *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DSFRotation));
}


void CNPX3DSFRotation::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPPROPID(x);
	INIT_NPPROPID(y);
	INIT_NPPROPID(z);
	INIT_NPPROPID(angle);

	m_classInitialized = true;
}

bool
CNPX3DSFRotation::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DField::HasMethod(name)
	  ;

}

bool
CNPX3DSFRotation::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFRotation::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFRotation::HasProperty(NPIdentifier name)
{
  return 
	  TEST_NPPROPID(name, x) ||
	  TEST_NPPROPID(name, y) ||
	  TEST_NPPROPID(name, z) ||
	  TEST_NPPROPID(name, angle) ||
		CNPX3DField::HasProperty(name);

}

bool
CNPX3DSFRotation::GetProperty(NPIdentifier name, NPVariant *result)
{
	double d;

	  if (TEST_NPPROPID(name, x))
	  {
		  d = get_x();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else if (TEST_NPPROPID(name, y))
	  {
		  d = get_y();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else if (TEST_NPPROPID(name, z))
	  {
		  d = get_z();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else if (TEST_NPPROPID(name, angle))
	  {
		  d = get_angle();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else
		return CNPX3DField::GetProperty(name, result);

	  return PR_TRUE;
}

bool
CNPX3DSFRotation::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
	double d;

	if (!value)
		return PR_FALSE;

	if (TEST_NPPROPID(name, x))
	{
		d = NPVToDouble(*value);

		set_x((float) d);
	}
	else if (TEST_NPPROPID(name, y))
	{
		d = NPVToDouble(*value);

		set_y((float) d);
	}
	else if (TEST_NPPROPID(name, z))
	{
		d = NPVToDouble(*value);

		set_z((float) d);
	}
	else if (TEST_NPPROPID(name, angle))
	{
		d = NPVToDouble(*value);

		set_angle((float) d);
	}
	else
	  return CNPX3DField::SetProperty(name, value);

	return PR_TRUE;
}

// CNPX3DSFString
CNPX3DSFString::~CNPX3DSFString()
{
}

bool CNPX3DSFString::m_classInitialized = false;
static NPObject *
AllocateCNPX3DSFString(NPP npp, NPClass *aClass)
{
  return new CNPX3DSFString(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DSFString,
                                 AllocateCNPX3DSFString);

CNPX3DSFString *CNPX3DSFString::Create(NPP npp)
{
	return (CNPX3DSFString *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DSFString));
}


void CNPX3DSFString::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DSFString::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DField::HasMethod(name)
	  ;

}

bool
CNPX3DSFString::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFString::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFString::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DField::HasProperty(name);

}

bool
CNPX3DSFString::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DField::GetProperty(name, result);
}

bool
CNPX3DSFString::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DField::SetProperty(name, value);
}

// CNPX3DSFTime
CNPX3DSFTime::~CNPX3DSFTime()
{
}

bool CNPX3DSFTime::m_classInitialized = false;
static NPObject *
AllocateCNPX3DSFTime(NPP npp, NPClass *aClass)
{
  return new CNPX3DSFTime(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DSFTime,
                                 AllocateCNPX3DSFTime);

CNPX3DSFTime *CNPX3DSFTime::Create(NPP npp)
{
	return (CNPX3DSFTime *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DSFTime));
}


void CNPX3DSFTime::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DSFTime::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DField::HasMethod(name)
	  ;

}

bool
CNPX3DSFTime::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFTime::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFTime::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DField::HasProperty(name);

}

bool
CNPX3DSFTime::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DField::GetProperty(name, result);
}

bool
CNPX3DSFTime::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DField::SetProperty(name, value);
}

// CNPX3DSFVec2f
CNPX3DSFVec2f::~CNPX3DSFVec2f()
{
}

float CNPX3DSFVec2f::get_x()
{
	Point2 p2;
	getValue((void **) &p2);

	return p2.x;
}

float CNPX3DSFVec2f::get_y()
{
	Point2 p2;
	getValue((void **) &p2);

	return p2.y;
}

void CNPX3DSFVec2f::set_x(float f)
{
	Point2 p2;
	getValue((void **) &p2);

	p2.x = f;

	setValue((void *) &p2);
}

void CNPX3DSFVec2f::set_y(float f)
{
	Point2 p2;
	getValue((void **) &p2);

	p2.y = f;

	setValue((void *) &p2);
}

// NP Glue (CNPX3DSFVec2f)
DEFINE_NPPROPID(CNPX3DSFVec2f,x);
DEFINE_NPPROPID(CNPX3DSFVec2f,y);

bool CNPX3DSFVec2f::m_classInitialized = false;
static NPObject *
AllocateCNPX3DSFVec2f(NPP npp, NPClass *aClass)
{
  return new CNPX3DSFVec2f(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DSFVec2f,
                                 AllocateCNPX3DSFVec2f);

CNPX3DSFVec2f *CNPX3DSFVec2f::Create(NPP npp)
{
	return (CNPX3DSFVec2f *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DSFVec2f));
}


void CNPX3DSFVec2f::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPPROPID(x);
	INIT_NPPROPID(y);

	m_classInitialized = true;
}

bool
CNPX3DSFVec2f::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DField::HasMethod(name)
	  ;

}

bool
CNPX3DSFVec2f::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFVec2f::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFVec2f::HasProperty(NPIdentifier name)
{
  return 
	  TEST_NPPROPID(name, x) ||
	  TEST_NPPROPID(name, y) ||
		CNPX3DField::HasProperty(name);

}

bool
CNPX3DSFVec2f::GetProperty(NPIdentifier name, NPVariant *result)
{
	double d;

	  if (TEST_NPPROPID(name, x))
	  {
		  d = get_x();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else if (TEST_NPPROPID(name, y))
	  {
		  d = get_y();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else
		return CNPX3DField::GetProperty(name, result);

	  return PR_TRUE;
}

bool
CNPX3DSFVec2f::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
	double d;

	if (!value)
		return PR_FALSE;

	if (TEST_NPPROPID(name, x))
	{
		d = NPVToDouble(*value);

		set_x((float) d);
	}
	else if (TEST_NPPROPID(name, y))
	{
		d = NPVToDouble(*value);

		set_y((float) d);
	}
	else
	  return CNPX3DField::SetProperty(name, value);

	return PR_TRUE;
}

// CNPX3DSFVec3f
CNPX3DSFVec3f::~CNPX3DSFVec3f()
{
}

float CNPX3DSFVec3f::get_x()
{
	Point3 p3;
	getValue((void **) &p3);

	return p3.x;
}

float CNPX3DSFVec3f::get_y()
{
	Point3 p3;
	getValue((void **) &p3);

	return p3.y;
}

float CNPX3DSFVec3f::get_z()
{
	Point3 p3;
	getValue((void **) &p3);

	return p3.z;
}

void CNPX3DSFVec3f::set_x(float f)
{
	Point3 p3;
	getValue((void **) &p3);

	p3.x = f;

	setValue((void *) &p3);
}

void CNPX3DSFVec3f::set_y(float f)
{
	Point3 p3;
	getValue((void **) &p3);

	p3.y = f;

	setValue((void *) &p3);
}

void CNPX3DSFVec3f::set_z(float f)
{
	Point3 p3;
	getValue((void **) &p3);

	p3.z = f;

	setValue((void *) &p3);
}

// NP Glue (CNPX3DSFVec3f)
DEFINE_NPPROPID(CNPX3DSFVec3f,x);
DEFINE_NPPROPID(CNPX3DSFVec3f,y);
DEFINE_NPPROPID(CNPX3DSFVec3f,z);

bool CNPX3DSFVec3f::m_classInitialized = false;
static NPObject *
AllocateCNPX3DSFVec3f(NPP npp, NPClass *aClass)
{
  return new CNPX3DSFVec3f(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DSFVec3f,
                                 AllocateCNPX3DSFVec3f);

CNPX3DSFVec3f *CNPX3DSFVec3f::Create(NPP npp)
{
	return (CNPX3DSFVec3f *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DSFVec3f));
}


void CNPX3DSFVec3f::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPPROPID(x);
	INIT_NPPROPID(y);
	INIT_NPPROPID(z);

	m_classInitialized = true;
}

bool
CNPX3DSFVec3f::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DField::HasMethod(name)
	  ;

}

bool
CNPX3DSFVec3f::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFVec3f::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DSFVec3f::HasProperty(NPIdentifier name)
{
  return 
	  TEST_NPPROPID(name, x) ||
	  TEST_NPPROPID(name, y) ||
	  TEST_NPPROPID(name, z) ||
		CNPX3DField::HasProperty(name);

}

bool
CNPX3DSFVec3f::GetProperty(NPIdentifier name, NPVariant *result)
{
	double d;

	  if (TEST_NPPROPID(name, x))
	  {
		  d = get_x();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else if (TEST_NPPROPID(name, y))
	  {
		  d = get_y();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else if (TEST_NPPROPID(name, z))
	  {
		  d = get_z();
		  DOUBLE_TO_NPVARIANT(d, *result);
	  }
	  else
		return CNPX3DField::GetProperty(name, result);

	  return PR_TRUE;
}

bool
CNPX3DSFVec3f::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
	double d;

	if (!value)
		return PR_FALSE;

	if (TEST_NPPROPID(name, x))
	{
		d = NPVToDouble(*value);

		set_x((float) d);
	}
	else if (TEST_NPPROPID(name, y))
	{
		d = NPVToDouble(*value);

		set_y((float) d);
	}
	else if (TEST_NPPROPID(name, z))
	{
		d = NPVToDouble(*value);

		set_z((float) d);
	}
	else
	  return CNPX3DField::SetProperty(name, value);

	return PR_TRUE;
}

// M-Fields
CNPX3DMField::~CNPX3DMField()
{
}

bool CNPX3DMField::getValue(NPVariant *result)
{
	NPArray *pArray = NULL;

	if (CNPX3DField::getValue((void **) &pArray))
	{
		OBJECT_TO_NPVARIANT(pArray, *result);
		return PR_TRUE;
	}
	else
		return PR_FALSE;
}

bool CNPX3DMField::setValue(NPVariant value)
{
	return false;
}

bool CNPX3DMField::get1Value(int index, NPVariant *result)
{
	return false;
}

bool CNPX3DMField::set1Value(int index, NPVariant value)
{
	return false;
}

DEFINE_NPMETHODID(CNPX3DMField,get1Value);
DEFINE_NPMETHODID(CNPX3DMField,set1Value);

bool CNPX3DMField::m_classInitialized = false;
static NPObject *
AllocateCNPX3DMField(NPP npp, NPClass *aClass)
{
  return new CNPX3DMField(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DMField,
                                 AllocateCNPX3DMField);


void CNPX3DMField::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPMETHODID(get1Value);
	INIT_NPMETHODID(set1Value);

	m_classInitialized = true;
}

bool
CNPX3DMField::HasMethod(NPIdentifier name)
{
  return 
	  TEST_NPMETHODID(name, get1Value) ||
	  TEST_NPMETHODID(name, set1Value) ||
	  CNPX3DField::HasMethod(name)
	  ;

}

bool
CNPX3DMField::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (TEST_NPMETHODID(name, get1Value))
	{
		if (argCount > 0)
		{
			int index = NPVToInteger(args[0]);
			return get1Value(index, result);
		}
	}
	else if (TEST_NPMETHODID(name, set1Value))
	{
		if (argCount > 1)
		{
			int index = NPVToInteger(args[0]);
			return set1Value(index, args[1]);
		}
	}
	else
	{
		return CNPX3DField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMField::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMField::HasProperty(NPIdentifier name)
{
	if (!NPN_IdentifierIsString(name))
	{
		return true;
	}
	else 
		return CNPX3DField::HasProperty(name);

}

bool
CNPX3DMField::GetProperty(NPIdentifier name, NPVariant *result)
{
	if (!NPN_IdentifierIsString(name))
	{
		int id = NPN_IntFromIdentifier(name);

		return get1Value(id, result);

		return true;
	}
	else
		return CNPX3DField::GetProperty(name, result);
}

bool
CNPX3DMField::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
	if (value && !NPN_IdentifierIsString(name))
	{
		int id = NPN_IntFromIdentifier(name);

		return set1Value(id, *value);

		return true;
	}
	else
		return CNPX3DField::SetProperty(name, value);
}


// CNPX3DMFColor
CNPX3DMFColor::~CNPX3DMFColor()
{
}

bool CNPX3DMFColor::getValue(NPArray **ppArray)
{
	ColorArray *pCA = NULL;
	CNPX3DField::getValue((void **) &pCA);

	assert(pCA );

	assert(ppArray);
	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	int sz = pCA->size();
	for (int i = 0; i < sz; i++)
	{
		Color c = (*pCA)[i];

		CNPX3DSFColor *pSFColor = CNPX3DSFColor::Create(mNpp);

		pSFColor->attach(m_context, NULL, NULL, NULL, eValueColor);

		NPN_RetainObject(pSFColor);

		NPVariant v;

		OBJECT_TO_NPVARIANT(pSFColor, v);

		pArray->set1Value(i, v);
	}

	*ppArray = pArray;

	return true;
}

bool CNPX3DMFColor::get1Value(int index, NPVariant *result)
{
	ColorArray *pCA = NULL;
	CNPX3DField::getValue((void **) &pCA);

	assert(pCA);

	if (index < 0 || index >= pCA->size())
	{
		NULL_TO_NPVARIANT(*result);
		return true;
	}

	Color c = (*pCA)[index];

	CNPX3DSFColor *pSFColor = CNPX3DSFColor::Create(mNpp);
	pSFColor->attach(m_context, NULL, NULL, NULL, eValueColor);

	pSFColor->set_r(c.r);
	pSFColor->set_g(c.g);
	pSFColor->set_b(c.b);

	NPN_RetainObject(pSFColor);

	OBJECT_TO_NPVARIANT(pSFColor, *result);

	return true;
}

bool CNPX3DMFColor::set1Value(int index, NPVariant value)
{
	ColorArray *pCA = NULL;
	CNPX3DField::getValue((void **) &pCA);

	assert(pCA);

	Color c;
	NPObject *pObj = NPVToObject(value);
	if (pObj)
	{
		if (NPOBJECT_ISA(pObj, CNPX3DSFColor))
		{
			CNPX3DSFColor *pSFColor = (CNPX3DSFColor *) pObj;

			c.r = pSFColor->get_r();
			c.g = pSFColor->get_g();
			c.b = pSFColor->get_b();
		}
	}

	int sz = pCA->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			pCA->push_back(c);
			sz++;
		}
		pCA->push_back(c);
	}
	else
		(*pCA)[index] = c;

	if (pCA->size() >= 0)
		CNPX3DField::setValue((void *) &pCA);

	return true;
}

bool CNPX3DMFColor::m_classInitialized = false;
static NPObject *
AllocateCNPX3DMFColor(NPP npp, NPClass *aClass)
{
  return new CNPX3DMFColor(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DMFColor,
                                 AllocateCNPX3DMFColor);

CNPX3DMFColor *CNPX3DMFColor::Create(NPP npp)
{
	return (CNPX3DMFColor *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DMFColor));
}


void CNPX3DMFColor::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DMFColor::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DMField::HasMethod(name)
	  ;

}

bool
CNPX3DMFColor::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFColor::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFColor::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DMField::HasProperty(name);

}

bool
CNPX3DMFColor::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DMField::GetProperty(name, result);
}

bool
CNPX3DMFColor::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DMField::SetProperty(name, value);
}

// CNPX3DMFFloat
CNPX3DMFFloat::~CNPX3DMFFloat()
{
}

bool CNPX3DMFFloat::getValue(NPArray **ppArray)
{
	FloatArray *pFA = NULL;
	CNPX3DField::getValue((void **) &pFA);

	assert(pFA);

	assert(ppArray);
	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	int sz = pFA->size();
	for (int i = 0; i < sz; i++)
	{
		Double d = (*pFA)[i];

		NPVariant v;
		DOUBLE_TO_NPVARIANT(d, v);

		pArray->set1Value(i, v);
	}

	*ppArray = pArray;

	return true;
}

bool CNPX3DMFFloat::get1Value(int index, NPVariant *result)
{
	FloatArray *pFA = NULL;
	CNPX3DField::getValue((void **) &pFA);

	assert(pFA);

	if (index < 0 || index >= pFA->size())
	{
		NULL_TO_NPVARIANT(*result);
		return true;
	}

	Double d = (*pFA)[index];

	DOUBLE_TO_NPVARIANT(d, *result);

	return true;
}

bool CNPX3DMFFloat::set1Value(int index, NPVariant value)
{
	FloatArray *pFA = NULL;
	CNPX3DField::getValue((void **) &pFA);

	assert(pFA);

	Double d = NPVToDouble(value);

	int sz = pFA->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			pFA->push_back((float)d);
			sz++;
		}
		pFA->push_back((float)d);
	}
	else
		(*pFA)[index] = (float)d;

	if (pFA->size() >= 0)
		CNPX3DField::setValue((void *) &pFA);

	return true;
}

bool CNPX3DMFFloat::m_classInitialized = false;
static NPObject *
AllocateCNPX3DMFFloat(NPP npp, NPClass *aClass)
{
  return new CNPX3DMFFloat(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DMFFloat,
                                 AllocateCNPX3DMFFloat);

CNPX3DMFFloat *CNPX3DMFFloat::Create(NPP npp)
{
	return (CNPX3DMFFloat *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DMFFloat));
}


void CNPX3DMFFloat::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DMFFloat::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DMField::HasMethod(name)
	  ;

}

bool
CNPX3DMFFloat::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFFloat::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFFloat::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DMField::HasProperty(name);

}

bool
CNPX3DMFFloat::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DMField::GetProperty(name, result);
}

bool
CNPX3DMFFloat::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DMField::SetProperty(name, value);
}

// CNPX3DMFInt32
CNPX3DMFInt32::~CNPX3DMFInt32()
{
}


bool CNPX3DMFInt32::getValue(NPArray **ppArray)
{
	IntegerArray *pIA = NULL;
	CNPX3DField::getValue((void **) &pIA);

	assert(pIA);

	assert(ppArray);
	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	int sz = pIA->size();
	for (int i = 0; i < sz; i++)
	{
		Integer i32 = (*pIA)[i];

		NPVariant v;
		INT32_TO_NPVARIANT(i32, v);

		pArray->set1Value(i, v);
	}

	*ppArray = pArray;

	return true;
}

bool CNPX3DMFInt32::get1Value(int index, NPVariant *result)
{
	IntegerArray *pIA = NULL;
	CNPX3DField::getValue((void **) &pIA);

	assert(pIA);

	if (index < 0 || index >= pIA->size())
	{
		NULL_TO_NPVARIANT(*result);
		return true;
	}

	Integer i32 = (*pIA)[index];

	INT32_TO_NPVARIANT(i32, *result);

	return true;
}

bool CNPX3DMFInt32::set1Value(int index, NPVariant value)
{
	IntegerArray *pIA = NULL;
	CNPX3DField::getValue((void **) &pIA);

	assert(pIA);

	Integer i32 = NPVToInteger(value);

	int sz = pIA->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			pIA->push_back(i32);
			sz++;
		}
		pIA->push_back(i32);
	}
	else
		(*pIA)[index] = i32;

	if (pIA->size() >= 0)
		CNPX3DField::setValue((void *) &pIA);

	return true;
}


bool CNPX3DMFInt32::m_classInitialized = false;
static NPObject *
AllocateCNPX3DMFInt32(NPP npp, NPClass *aClass)
{
  return new CNPX3DMFInt32(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DMFInt32,
                                 AllocateCNPX3DMFInt32);

CNPX3DMFInt32 *CNPX3DMFInt32::Create(NPP npp)
{
	return (CNPX3DMFInt32 *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DMFInt32));
}


void CNPX3DMFInt32::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DMFInt32::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DMField::HasMethod(name)
	  ;

}

bool
CNPX3DMFInt32::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFInt32::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFInt32::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DMField::HasProperty(name);

}

bool
CNPX3DMFInt32::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DMField::GetProperty(name, result);
}

bool
CNPX3DMFInt32::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DMField::SetProperty(name, value);
}

// CNPX3DMFNode
CNPX3DMFNode::~CNPX3DMFNode()
{
}

bool CNPX3DMFNode::getValue(NPArray **ppArray)
{
	NodeArray *pNA = NULL;
	CNPX3DField::getValue((void **) &pNA);

	assert(pNA);

	assert(ppArray);
	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	int sz = pNA->size();
	for (int i = 0; i < sz; i++)
	{
		CAnmNode *pNode = (*pNA)[i];

		CAnmSAINode *pSAINode = new CAnmSAINode(pNode);
		pSAINode->SetSink(m_node->GetSink());

		CNPX3DNode *pX3DNode = CNPX3DNode::Create(mNpp, pSAINode, m_context);

		NPN_RetainObject(pX3DNode);

		NPVariant v;

		OBJECT_TO_NPVARIANT(pX3DNode, v);

		pArray->set1Value(i, v);
	}

	*ppArray = pArray;

	return true;
}

bool CNPX3DMFNode::get1Value(int index, NPVariant *result)
{
	NodeArray *pNA = NULL;
	CNPX3DField::getValue((void **) &pNA);

	assert(pNA);

	if (index < 0 || index >= pNA->size())
	{
		NULL_TO_NPVARIANT(*result);
		return true;
	}

	CAnmNode *pNode = (*pNA)[index];

	CAnmSAINode *pSAINode = new CAnmSAINode(pNode);
	pSAINode->SetSink(m_node->GetSink());

	CNPX3DNode *pX3DNode = CNPX3DNode::Create(mNpp, pSAINode, m_context);

	NPN_RetainObject(pX3DNode);

	OBJECT_TO_NPVARIANT(pX3DNode, *result);

	return true;
}

extern NPClass NPOBJECT_CLASS_NAME(CNPX3DNode);

bool CNPX3DMFNode::set1Value(int index, NPVariant value)
{
	NodeArray *pNA = NULL;
	CNPX3DField::getValue((void **) &pNA);

	assert(pNA);

	CAnmNode *pNode = NULL;

	NPObject *pObj = NPVToObject(value);
	if (pObj)
	{
		if (NPOBJECT_ISA(pObj, CNPX3DNode))
		{
			CNPX3DNode *pX3DNode = (CNPX3DNode *) pObj;

			CAnmSAINode *pSAINode = pX3DNode->GetNode();

			if (pSAINode)
				pNode = pSAINode->GetNode();
		}
	}

	int sz = pNA->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			pNA->push_back(pNode);
			sz++;
		}
		pNA->push_back(pNode);
	}
	else
		(*pNA)[index] = pNode;

	if (pNA->size() >= 0)
		CNPX3DField::setValue((void *) &pNA);

	return true;
}

bool CNPX3DMFNode::m_classInitialized = false;
static NPObject *
AllocateCNPX3DMFNode(NPP npp, NPClass *aClass)
{
  return new CNPX3DMFNode(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DMFNode,
                                 AllocateCNPX3DMFNode);

CNPX3DMFNode *CNPX3DMFNode::Create(NPP npp)
{
	return (CNPX3DMFNode *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DMFNode));
}


void CNPX3DMFNode::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DMFNode::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DMField::HasMethod(name)
	  ;

}

bool
CNPX3DMFNode::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFNode::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFNode::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DMField::HasProperty(name);

}

bool
CNPX3DMFNode::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DMField::GetProperty(name, result);
}

bool
CNPX3DMFNode::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DMField::SetProperty(name, value);
}

// CNPX3DMFRotation
CNPX3DMFRotation::~CNPX3DMFRotation()
{
}

bool CNPX3DMFRotation::getValue(NPArray **ppArray)
{
	RotationArray *pRA = NULL;
	CNPX3DField::getValue((void **) &pRA);

	assert(pRA);

	assert(ppArray);
	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	int sz = pRA->size();
	for (int i = 0; i < sz; i++)
	{
		Rotation r = (*pRA)[i];

		CNPX3DSFRotation *pSFRotation = CNPX3DSFRotation::Create(mNpp);

		pSFRotation->attach(m_context, NULL, NULL, NULL, eValueRotation);

		NPN_RetainObject(pSFRotation);

		NPVariant v;

		OBJECT_TO_NPVARIANT(pSFRotation, v);

		pArray->set1Value(i, v);
	}

	*ppArray = pArray;

	return true;
}

bool CNPX3DMFRotation::get1Value(int index, NPVariant *result)
{
	RotationArray *pRA = NULL;
	CNPX3DField::getValue((void **) &pRA);

	assert(pRA);

	if (index < 0 || index >= pRA->size())
	{
		NULL_TO_NPVARIANT(*result);
		return true;
	}

	CNPX3DSFRotation *pSFRotation = CNPX3DSFRotation::Create(mNpp);
	pSFRotation->attach(m_context, NULL, NULL, NULL, eValueRotation);

	NPN_RetainObject(pSFRotation);

	OBJECT_TO_NPVARIANT(pSFRotation, *result);

	return true;
}

bool CNPX3DMFRotation::set1Value(int index, NPVariant value)
{
	RotationArray *pRA = NULL;
	CNPX3DField::getValue((void **) &pRA);

	assert(pRA);

	Rotation r;
	NPObject *pObj = NPVToObject(value);
	if (pObj)
	{
		if (NPOBJECT_ISA(pObj, CNPX3DSFRotation))
		{
			CNPX3DSFRotation *pSFRotation = (CNPX3DSFRotation *) pObj;

			r.vec.x = pSFRotation->get_x();
			r.vec.y = pSFRotation->get_y();
			r.vec.z = pSFRotation->get_z();
			r.theta = pSFRotation->get_angle();
		}
	}

	int sz = pRA->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			pRA->push_back(r);
			sz++;
		}
		pRA->push_back(r);
	}
	else
		(*pRA)[index] = r;

	if (pRA->size() >= 0)
		CNPX3DField::setValue((void *) &pRA);

	return true;
}

bool CNPX3DMFRotation::m_classInitialized = false;
static NPObject *
AllocateCNPX3DMFRotation(NPP npp, NPClass *aClass)
{
  return new CNPX3DMFRotation(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DMFRotation,
                                 AllocateCNPX3DMFRotation);

CNPX3DMFRotation *CNPX3DMFRotation::Create(NPP npp)
{
	return (CNPX3DMFRotation *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DMFRotation));
}


void CNPX3DMFRotation::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DMFRotation::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DMField::HasMethod(name)
	  ;

}

bool
CNPX3DMFRotation::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFRotation::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFRotation::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DMField::HasProperty(name);

}

bool
CNPX3DMFRotation::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DMField::GetProperty(name, result);
}

bool
CNPX3DMFRotation::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DMField::SetProperty(name, value);
}

// CNPX3DMFString
CNPX3DMFString::~CNPX3DMFString()
{
}

bool CNPX3DMFString::getValue(NPArray **ppArray)
{

	StringArray *pSA = NULL;
	CNPX3DField::getValue((void **) &pSA);

	assert(pSA);

	assert(ppArray);
	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	int sz = pSA->size();
	for (int i = 0; i < sz; i++)
	{
		String s = (*pSA)[i];

		char *cp = NULL;

		if (s)
		{
			cp = s->GetBuf();
			assert(cp);
		}
		else
			cp = "";

		char *cp1 = NPstrdup(cp);

		NPVariant v;
		STRINGZ_TO_NPVARIANT(cp1, v);

		pArray->set1Value(i, v);
	}

	*ppArray = pArray;

	return true;
}

bool CNPX3DMFString::get1Value(int index, NPVariant *result)
{
	StringArray *pSA = NULL;
	CNPX3DField::getValue((void **) &pSA);

	assert(pSA);

	if (index < 0 || index >= pSA->size())
	{
		STRINGZ_TO_NPVARIANT(NPstrdup(""), *result);
		return true;
	}

	String s = (*pSA)[index];

	char *cp = NULL;

	if (s)
	{
		cp = s->GetBuf();
		assert(cp);
	}
	else
		cp = "";

	char *cp1 = NPstrdup(cp);
	STRINGZ_TO_NPVARIANT(cp1, *result);

	return true;
}

bool CNPX3DMFString::set1Value(int index, NPVariant value)
{
	StringArray *pSA = NULL;
	CNPX3DField::getValue((void **) &pSA);

	assert(pSA);

	String s = NPVToString(value);
	if (!s)
		s = new CAnmString("");

	int sz = pSA->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			pSA->push_back(new CAnmString(""));
			sz++;
		}
		pSA->push_back(s);
	}
	else
		(*pSA)[index] = s;

	if (pSA->size() >= 0)
		CNPX3DField::setValue((void *) &pSA);

	return true;
}


// NP Glue (CNPX3DMFString)
bool CNPX3DMFString::m_classInitialized = false;
static NPObject *
AllocateCNPX3DMFString(NPP npp, NPClass *aClass)
{
  return new CNPX3DMFString(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DMFString,
                                 AllocateCNPX3DMFString);

CNPX3DMFString *CNPX3DMFString::Create(NPP npp)
{
	return (CNPX3DMFString *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DMFString));
}


void CNPX3DMFString::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DMFString::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DMField::HasMethod(name)
	  ;

}

bool
CNPX3DMFString::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFString::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFString::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DMField::HasProperty(name);

}

bool
CNPX3DMFString::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DMField::GetProperty(name, result);
}

bool
CNPX3DMFString::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DMField::SetProperty(name, value);
}

// CNPX3DMFTime
CNPX3DMFTime::~CNPX3DMFTime()
{
}

bool CNPX3DMFTime::getValue(NPArray **ppArray)
{
	TimeArray *pTA = NULL;
	CNPX3DField::getValue((void **) &pTA);

	assert(pTA);

	assert(ppArray);
	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	int sz = pTA->size();
	for (int i = 0; i < sz; i++)
	{
		Time t = (*pTA)[i];

		NPVariant v;
		DOUBLE_TO_NPVARIANT(t, v);

		pArray->set1Value(i, v);
	}

	*ppArray = pArray;

	return true;
}

bool CNPX3DMFTime::get1Value(int index, NPVariant *result)
{
	TimeArray *pTA = NULL;
	CNPX3DField::getValue((void **) &pTA);

	assert(pTA);

	if (index < 0 || index >= pTA->size())
	{
		NULL_TO_NPVARIANT(*result);
		return true;
	}

	Time t = (*pTA)[index];

	DOUBLE_TO_NPVARIANT(t, *result);

	return true;
}

bool CNPX3DMFTime::set1Value(int index, NPVariant value)
{
	TimeArray *pTA = NULL;
	CNPX3DField::getValue((void **) &pTA);

	assert(pTA);

	Double d = NPVToDouble(value);

	int sz = pTA->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			pTA->push_back(d);
			sz++;
		}
		pTA->push_back(d);
	}
	else
		(*pTA)[index] = d;

	if (pTA->size() >= 0)
		CNPX3DField::setValue((void *) &pTA);

	return true;
}

bool CNPX3DMFTime::m_classInitialized = false;
static NPObject *
AllocateCNPX3DMFTime(NPP npp, NPClass *aClass)
{
  return new CNPX3DMFTime(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DMFTime,
                                 AllocateCNPX3DMFTime);

CNPX3DMFTime *CNPX3DMFTime::Create(NPP npp)
{
	return (CNPX3DMFTime *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DMFTime));
}


void CNPX3DMFTime::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DMFTime::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DMField::HasMethod(name)
	  ;

}

bool
CNPX3DMFTime::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFTime::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFTime::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DMField::HasProperty(name);

}

bool
CNPX3DMFTime::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DMField::GetProperty(name, result);
}

bool
CNPX3DMFTime::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DMField::SetProperty(name, value);
}

// CNPX3DMFVec2f
CNPX3DMFVec2f::~CNPX3DMFVec2f()
{
}

bool CNPX3DMFVec2f::getValue(NPArray **ppArray)
{
	Point2Array *pPA = NULL;
	CNPX3DField::getValue((void **) &pPA);

	assert(pPA);

	assert(ppArray);
	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	int sz = pPA->size();
	for (int i = 0; i < sz; i++)
	{
		Point2 p2 = (*pPA)[i];

		CNPX3DSFVec2f *pSFVec2f = CNPX3DSFVec2f::Create(mNpp);

		pSFVec2f->attach(m_context, NULL, NULL, NULL, eValuePoint2);

		NPN_RetainObject(pSFVec2f);

		NPVariant v;

		OBJECT_TO_NPVARIANT(pSFVec2f, v);

		pArray->set1Value(i, v);
	}

	*ppArray = pArray;

	return true;
}

bool CNPX3DMFVec2f::get1Value(int index, NPVariant *result)
{
	Point2Array *pPA = NULL;
	CNPX3DField::getValue((void **) &pPA);

	assert(pPA);

	if (index < 0 || index >= pPA->size())
	{
		NULL_TO_NPVARIANT(*result);
		return true;
	}

	CNPX3DSFVec2f *pSFVec2f = CNPX3DSFVec2f::Create(mNpp);
	pSFVec2f->attach(m_context, NULL, NULL, NULL, eValuePoint2);

	NPN_RetainObject(pSFVec2f);

	OBJECT_TO_NPVARIANT(pSFVec2f, *result);

	return true;
}

bool CNPX3DMFVec2f::set1Value(int index, NPVariant value)
{
	Point2Array *pPA = NULL;
	CNPX3DField::getValue((void **) &pPA);

	assert(pPA);

	Point2 p2;
	NPObject *pObj = NPVToObject(value);
	if (pObj)
	{
		if (NPOBJECT_ISA(pObj, CNPX3DSFVec2f))
		{
			CNPX3DSFVec2f *pSFVec2f = (CNPX3DSFVec2f *) pObj;

			p2.x = pSFVec2f->get_x();
			p2.y = pSFVec2f->get_y();
		}
	}

	int sz = pPA->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			pPA->push_back(p2);
			sz++;
		}
		pPA->push_back(p2);
	}
	else
		(*pPA)[index] = p2;

	if (pPA->size() >= 0)
		CNPX3DField::setValue((void *) &pPA);

	return true;
}

bool CNPX3DMFVec2f::m_classInitialized = false;
static NPObject *
AllocateCNPX3DMFVec2f(NPP npp, NPClass *aClass)
{
  return new CNPX3DMFVec2f(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DMFVec2f,
                                 AllocateCNPX3DMFVec2f);

CNPX3DMFVec2f *CNPX3DMFVec2f::Create(NPP npp)
{
	return (CNPX3DMFVec2f *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DMFVec2f));
}


void CNPX3DMFVec2f::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DMFVec2f::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DMField::HasMethod(name)
	  ;

}

bool
CNPX3DMFVec2f::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFVec2f::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFVec2f::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DMField::HasProperty(name);

}

bool
CNPX3DMFVec2f::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DMField::GetProperty(name, result);
}

bool
CNPX3DMFVec2f::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DMField::SetProperty(name, value);
}

// CNPX3DMFVec3f
CNPX3DMFVec3f::~CNPX3DMFVec3f()
{
}

bool CNPX3DMFVec3f::getValue(NPArray **ppArray)
{
	Point3Array *pPA = NULL;
	CNPX3DField::getValue((void **) &pPA);

	assert(pPA);

	assert(ppArray);
	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	int sz = pPA->size();
	for (int i = 0; i < sz; i++)
	{
		Point3 p3 = (*pPA)[i];

		CNPX3DSFVec3f *pSFVec3f = CNPX3DSFVec3f::Create(mNpp);

		pSFVec3f->attach(m_context, NULL, NULL, NULL, eValuePoint3);

		NPN_RetainObject(pSFVec3f);

		NPVariant v;

		OBJECT_TO_NPVARIANT(pSFVec3f, v);

		pArray->set1Value(i, v);
	}

	*ppArray = pArray;

	return true;
}

bool CNPX3DMFVec3f::get1Value(int index, NPVariant *result)
{
	Point3Array *pPA = NULL;
	CNPX3DField::getValue((void **) &pPA);

	assert(pPA);

	if (index < 0 || index >= pPA->size())
	{
		NULL_TO_NPVARIANT(*result);
		return true;
	}

	CNPX3DSFVec3f *pSFVec3f = CNPX3DSFVec3f::Create(mNpp);
	pSFVec3f->attach(m_context, NULL, NULL, NULL, eValuePoint3);

	NPN_RetainObject(pSFVec3f);

	OBJECT_TO_NPVARIANT(pSFVec3f, *result);

	return true;
}

bool CNPX3DMFVec3f::set1Value(int index, NPVariant value)
{
	Point3Array *pPA = NULL;
	CNPX3DField::getValue((void **) &pPA);

	assert(pPA);

	Point3 p3;
	NPObject *pObj = NPVToObject(value);
	if (pObj)
	{
		if (NPOBJECT_ISA(pObj, CNPX3DSFVec3f))
		{
			CNPX3DSFVec3f *pSFVec3f = (CNPX3DSFVec3f *) pObj;

			p3.x = pSFVec3f->get_x();
			p3.y = pSFVec3f->get_y();
			p3.z = pSFVec3f->get_z();
		}
	}

	int sz = pPA->size();
	if (index >= sz)
	{
		while (index > sz)
		{
			pPA->push_back(p3);
			sz++;
		}
		pPA->push_back(p3);
	}
	else
		(*pPA)[index] = p3;

	if (pPA->size() >= 0)
		CNPX3DField::setValue((void *) &pPA);

	return true;
}


bool CNPX3DMFVec3f::m_classInitialized = false;
static NPObject *
AllocateCNPX3DMFVec3f(NPP npp, NPClass *aClass)
{
  return new CNPX3DMFVec3f(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DMFVec3f,
                                 AllocateCNPX3DMFVec3f);

CNPX3DMFVec3f *CNPX3DMFVec3f::Create(NPP npp)
{
	return (CNPX3DMFVec3f *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DMFVec3f));
}


void CNPX3DMFVec3f::initClass()
{
	if (m_classInitialized)
		return;

	m_classInitialized = true;
}

bool
CNPX3DMFVec3f::HasMethod(NPIdentifier name)
{
  return 
	  CNPX3DMField::HasMethod(name)
	  ;

}

bool
CNPX3DMFVec3f::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::Invoke(name, args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFVec3f::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
	if (false)
	{
	}
	else
	{
		return CNPX3DMField::InvokeDefault(args, argCount, result);
	}
	return PR_FALSE;
}

bool
CNPX3DMFVec3f::HasProperty(NPIdentifier name)
{
  return 
		CNPX3DMField::HasProperty(name);

}

bool
CNPX3DMFVec3f::GetProperty(NPIdentifier name, NPVariant *result)
{
  return 
		CNPX3DMField::GetProperty(name, result);
}

bool
CNPX3DMFVec3f::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return 
		CNPX3DMField::SetProperty(name, value);
}
