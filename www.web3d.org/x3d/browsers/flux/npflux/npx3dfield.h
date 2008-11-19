/********************************************************************************
 * Flux
 *
 * File: npx3dfield.h
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

#ifndef _npx3dfield_h
#define _npx3dfield_h

#include "scriptablepluginobject.h"

typedef enum NPX3DFieldAccess {
	NPX3DInitializeOnly,
	NPX3DInputOnly,
	NPX3DOutputOnly,
	NPX3DInputOutput,
} NPX3DFieldAccess;

typedef enum NPX3DFieldType {
	
	NPX3DUnknownFieldType = 0,

	// Ordered for easy compatibility with Java language binding since
	// that came first...
	NPX3DSFBoolType,
	NPX3DMFBoolType,
	NPX3DSFColorType,
	NPX3DMFColorType,
	NPX3DSFColorRGBAType,
	NPX3DMFColorRGBAType,
	NPX3DSFDoubleType,
	NPX3DMFDoubleType,
	NPX3DSFFloatType,
	NPX3DMFFloatType,
	NPX3DSFImageType,
	NPX3DMFImageType,
	NPX3DSFInt32Type,
	NPX3DMFInt32Type,
	NPX3DSFNodeType,
	NPX3DMFNodeType,
	NPX3DSFRotationType,
	NPX3DMFRotationType,
	NPX3DSFStringType,
	NPX3DMFStringType,
	NPX3DSFTimeType,
	NPX3DMFTimeType,
	NPX3DSFVec2fType,
	NPX3DMFVec2fType,
	NPX3DSFVec2dType,
	NPX3DMFVec2dType,
	NPX3DSFVec3fType,
	NPX3DMFVec3fType,
	NPX3DSFVec3dType,
	NPX3DMFVec3dType,
} NPX3DFieldType;

struct sAnmObserverCallback {

	NPObject *m_observer;
	NPObject *m_userData;

	sAnmObserverCallback(NPObject *pObserver, NPObject *userData)
	{
		m_observer = pObserver;
		m_userData = userData;
	}
};

struct sAnmThreadedCallback {
	class CNPX3DField *pField;
	double timestamp;
};

enum eAnmFieldBaseSetValue {
	ANMFIELDBASE_CANTDO,
	ANMFIELDBASE_DID,
	ANMFIELDBASE_DIY,
};


#define ANMMINFIELDCALLBACKDELTA .2f	// /secs
 
class CNPX3DField : public ScriptablePluginObject
{
protected :

	std::list<sAnmObserverCallback> m_observers;
	class CNPX3DExecutionContext *m_context;
	class CAnmSAINode *m_node;
	class CAnmField *m_field;
	class CAnmField *m_savedfield;
	class CAnmCallback *m_callback;
	CLASSMEMBERID m_mid;
	eValueType m_fieldtype;
	Time m_lastcbtime; 
	
	static void fieldBaseCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData, void *userData);
	virtual void handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData);
	void callCallbacks(CNPX3DField *pField);
	void callThreadedCallbacks(CNPX3DField *pField, double timestamp);

	eAnmFieldBaseSetValue getValue(void **pVal);
	eAnmFieldBaseSetValue setValue(void *pVal);

	void addFieldEventListener(NPObject *listener);
	void removeFieldEventListener(NPObject *listener);

	// NP Glue
	static bool m_classInitialized;

	DECLARE_NPMETHODID(addFieldEventListener);
	DECLARE_NPMETHODID(removeFieldEventListener);
	DECLARE_NPMETHODID(getValue);
	DECLARE_NPMETHODID(setValue);
	DECLARE_NPMETHODID(getAccessType);
	DECLARE_NPMETHODID(getType);
	DECLARE_NPMETHODID(getName);
	DECLARE_NPMETHODID(dispose);

	void initClass();

public:

	CNPX3DField(NPP npp)
		: ScriptablePluginObject(npp)
	{
		m_context = NULL;
		m_node = NULL;
		m_field = NULL;
		m_savedfield = NULL;
		m_callback = NULL;
		m_mid = NULL;
		m_lastcbtime = ANMTIME_BADTIMEVALUE;

		initClass();
	}

	virtual ~CNPX3DField();

	// Methods
	void attach(class CNPX3DExecutionContext *context, class CAnmSAINode *pNode, class CAnmField *pField,
		CLASSMEMBERID mid, eValueType fieldType);

	virtual bool getValue(/*[out, retval]*/ NPVariant *value);
	virtual bool setValue(/*[in]*/ NPVariant value);
	virtual bool getAccessType(/*[out, retval]*/ NPX3DFieldAccess *accesstype);
	virtual bool getType(/*[out, retval]*/ NPX3DFieldType *fieldtype);
	virtual bool getName(/*[out, retval]*/ const char **name);
	virtual bool dispose();

	static NPX3DFieldAccess getAccessType(eAnmClassMemberType memberType);
	static NPX3DFieldType getFieldType(eValueType valueType);

	static void CallX3DCallback(sAnmThreadedCallback *pTCB);

	virtual CAnmField *GetField()
	{
		return m_field;
	}
	
	// NP Glue

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

inline NPX3DFieldAccess CNPX3DField::getAccessType(eAnmClassMemberType memberType)
{
	NPX3DFieldAccess val;

	switch (memberType)
	{
		case eAnmCallbackMember :
			val = NPX3DOutputOnly;
			break;

		case eAnmMethodMember :
			val = NPX3DInputOnly;
			break;

		case eAnmValueMember :
			val = NPX3DInputOutput;
			break;

		case eAnmInitMember :
			val = NPX3DInitializeOnly;
			break;

		default :
			assert(FALSE);	// what the hell?
			break;
	}

	return val;
}

inline NPX3DFieldType CNPX3DField::getFieldType(eValueType valueType)
{
	NPX3DFieldType val = NPX3DUnknownFieldType;

	switch (valueType)
	{
		case eValueBoolean :
			val = NPX3DSFBoolType;
			break;

		case eValueBooleanArray :
			val = NPX3DMFBoolType;
			break;

		case eValueColor :
			val = NPX3DSFColorType;
			break;

		case eValueColorArray :
			val = NPX3DMFColorType;
			break;

		case eValueDouble :
			val = NPX3DSFDoubleType;
			break;

		case eValueDoubleArray :
			val = NPX3DMFDoubleType;
			break;

		case eValueInteger :
			val = NPX3DSFInt32Type;
			break;

		case eValueIntegerArray :
			val = NPX3DMFInt32Type;
			break;

		case eValueFloat :
			val = NPX3DSFFloatType;
			break;

		case eValueFloatArray :
			val = NPX3DMFFloatType;
			break;

		case eValueNode :
			val = NPX3DSFNodeType;
			break;

		case eValueNodeArray :
			val = NPX3DMFNodeType;
			break;

		case eValuePoint2 :
			val = NPX3DSFVec2fType;
			break;

		case eValuePoint2Array :
			val = NPX3DMFVec2fType;
			break;

		case eValuePoint3 :
			val = NPX3DSFVec3fType;
			break;

		case eValuePoint3Array :
			val = NPX3DMFVec3fType;
			break;

		case eValueRotation :
			val = NPX3DSFRotationType;
			break;

		case eValueRotationArray :
			val = NPX3DMFRotationType;
			break;

		case eValueString :
			val = NPX3DSFStringType;
			break;

		case eValueStringArray :
			val = NPX3DMFStringType;
			break;

		case eValueTime :
			val = NPX3DSFTimeType;
			break;

		case eValueTimeArray :
			val = NPX3DMFTimeType;
			break;

		default :
			assert(FALSE);	// what the hell?
			break;
	}

	return val;

}

class CNPX3DSFBool : public CNPX3DField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();


public:

	CNPX3DSFBool(NPP npp)
		: CNPX3DField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DSFBool();

	// NP Glue
	static CNPX3DSFBool *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DSFColor : public CNPX3DField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;

	DECLARE_NPPROPID(r);
	DECLARE_NPPROPID(g);
	DECLARE_NPPROPID(b);

	void initClass();

public:

	CNPX3DSFColor(NPP npp)
		: CNPX3DField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DSFColor();

	// New methods
	float get_r();
	float get_g();
	float get_b();
	void set_r(float f);
	void set_g(float f);
	void set_b(float f);


	// NP Glue
	static CNPX3DSFColor *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DSFFloat : public CNPX3DField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DSFFloat(NPP npp)
		: CNPX3DField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DSFFloat();

	// NP Glue
	static CNPX3DSFFloat *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DSFInt32 : public CNPX3DField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DSFInt32(NPP npp)
		: CNPX3DField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DSFInt32();

	// NP Glue
	static CNPX3DSFInt32 *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DSFNode : public CNPX3DField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DSFNode(NPP npp)
		: CNPX3DField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DSFNode();

	// NP Glue
	static CNPX3DSFNode *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DSFRotation : public CNPX3DField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;

	DECLARE_NPPROPID(x);
	DECLARE_NPPROPID(y);
	DECLARE_NPPROPID(z);
	DECLARE_NPPROPID(angle);

	void initClass();

public:

	CNPX3DSFRotation(NPP npp)
		: CNPX3DField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DSFRotation();

	// New Methods
	float get_x();
	float get_y();
	float get_z();
	float get_angle();
	void set_x(float f);
	void set_y(float f);
	void set_z(float f);
	void set_angle(float f);

	// NP Glue
	static CNPX3DSFRotation *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DSFString : public CNPX3DField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DSFString(NPP npp)
		: CNPX3DField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DSFString();

	// NP Glue
	static CNPX3DSFString *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DSFTime : public CNPX3DField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DSFTime(NPP npp)
		: CNPX3DField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DSFTime();

	// NP Glue
	static CNPX3DSFTime *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DSFVec2f : public CNPX3DField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;

	DECLARE_NPPROPID(x);
	DECLARE_NPPROPID(y);

	void initClass();

public:

	CNPX3DSFVec2f(NPP npp)
		: CNPX3DField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DSFVec2f();

	// New Methods
	float get_x();
	float get_y();
	void set_x(float f);
	void set_y(float f);

	// NP Glue
	static CNPX3DSFVec2f *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DSFVec3f : public CNPX3DField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;

	DECLARE_NPPROPID(x);
	DECLARE_NPPROPID(y);
	DECLARE_NPPROPID(z);

	void initClass();

public:

	CNPX3DSFVec3f(NPP npp)
		: CNPX3DField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DSFVec3f();

	// New Methods
	float get_x();
	float get_y();
	float get_z();
	void set_x(float f);
	void set_y(float f);
	void set_z(float f);

	// NP Glue
	static CNPX3DSFVec3f *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DMField : public CNPX3DField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;

	DECLARE_NPMETHODID(get1Value);
	DECLARE_NPMETHODID(set1Value);

	void initClass();

public:

	CNPX3DMField(NPP npp)
		: CNPX3DField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DMField();

	// New methods
	virtual bool getValue(NPVariant *result);
	virtual bool setValue(NPVariant value);
	virtual bool get1Value(int index, NPVariant *result);
	virtual bool set1Value(int index, NPVariant value);

	// NP Glue

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DMFColor : public CNPX3DMField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DMFColor(NPP npp)
		: CNPX3DMField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DMFColor();

	// CNPX3DMField overrides
	virtual bool getValue(NPArray **ppArray);
	virtual bool get1Value(int index, NPVariant *result);
	virtual bool set1Value(int index, NPVariant value);

	// NP Glue
	static CNPX3DMFColor *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DMFFloat : public CNPX3DMField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DMFFloat(NPP npp)
		: CNPX3DMField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DMFFloat();

	// CNPX3DMField overrides
	virtual bool getValue(NPArray **ppArray);
	virtual bool get1Value(int index, NPVariant *result);
	virtual bool set1Value(int index, NPVariant value);

	// NP Glue
	static CNPX3DMFFloat *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DMFInt32 : public CNPX3DMField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DMFInt32(NPP npp)
		: CNPX3DMField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DMFInt32();

	// CNPX3DMField overrides
	virtual bool getValue(NPArray **ppArray);
	virtual bool get1Value(int index, NPVariant *result);
	virtual bool set1Value(int index, NPVariant value);

	// NP Glue
	static CNPX3DMFInt32 *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DMFNode : public CNPX3DMField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DMFNode(NPP npp)
		: CNPX3DMField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DMFNode();

	// CNPX3DMField overrides
	virtual bool getValue(NPArray **ppArray);
	virtual bool get1Value(int index, NPVariant *result);
	virtual bool set1Value(int index, NPVariant value);

	// NP Glue
	static CNPX3DMFNode *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DMFRotation : public CNPX3DMField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DMFRotation(NPP npp)
		: CNPX3DMField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DMFRotation();

	// CNPX3DMField overrides
	virtual bool getValue(NPArray **ppArray);
	virtual bool get1Value(int index, NPVariant *result);
	virtual bool set1Value(int index, NPVariant value);

	// NP Glue
	static CNPX3DMFRotation *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DMFString : public CNPX3DMField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DMFString(NPP npp)
		: CNPX3DMField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DMFString();

	// CNPX3DMField overrides
	virtual bool getValue(NPArray **ppArray);
	virtual bool get1Value(int index, NPVariant *result);
	virtual bool set1Value(int index, NPVariant value);

	// NP Glue
	static CNPX3DMFString *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DMFTime : public CNPX3DMField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DMFTime(NPP npp)
		: CNPX3DMField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DMFTime();

	// CNPX3DMField overrides
	virtual bool getValue(NPArray **ppArray);
	virtual bool get1Value(int index, NPVariant *result);
	virtual bool set1Value(int index, NPVariant value);

	// NP Glue
	static CNPX3DMFTime *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DMFVec2f : public CNPX3DMField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DMFVec2f(NPP npp)
		: CNPX3DMField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DMFVec2f();

	// CNPX3DMField overrides
	virtual bool getValue(NPArray **ppArray);
	virtual bool get1Value(int index, NPVariant *result);
	virtual bool set1Value(int index, NPVariant value);

	// NP Glue
	static CNPX3DMFVec2f *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

class CNPX3DMFVec3f : public CNPX3DMField
{
protected :
	
	// NP Glue
	static bool m_classInitialized;


	void initClass();

public:

	CNPX3DMFVec3f(NPP npp)
		: CNPX3DMField(npp)
	{
		initClass();
	}

	virtual ~CNPX3DMFVec3f();

	// CNPX3DMField overrides
	virtual bool getValue(NPArray **ppArray);
	virtual bool get1Value(int index, NPVariant *result);
	virtual bool set1Value(int index, NPVariant value);

	// NP Glue
	static CNPX3DMFVec3f *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

DECLARE_NPOBJECT_CLASS(CNPX3DSFBool);
DECLARE_NPOBJECT_CLASS(CNPX3DSFColor);
DECLARE_NPOBJECT_CLASS(CNPX3DMFColor);
DECLARE_NPOBJECT_CLASS(CNPX3DSFDouble);
DECLARE_NPOBJECT_CLASS(CNPX3DSFFloat);
DECLARE_NPOBJECT_CLASS(CNPX3DMFFloat);
DECLARE_NPOBJECT_CLASS(CNPX3DSFImage);
DECLARE_NPOBJECT_CLASS(CNPX3DSFInt32);
DECLARE_NPOBJECT_CLASS(CNPX3DMFInt32);
DECLARE_NPOBJECT_CLASS(CNPX3DSFNode);
DECLARE_NPOBJECT_CLASS(CNPX3DMFNode);
DECLARE_NPOBJECT_CLASS(CNPX3DSFRotation);
DECLARE_NPOBJECT_CLASS(CNPX3DMFRotation);
DECLARE_NPOBJECT_CLASS(CNPX3DSFString);
DECLARE_NPOBJECT_CLASS(CNPX3DMFString);
DECLARE_NPOBJECT_CLASS(CNPX3DSFTime);
DECLARE_NPOBJECT_CLASS(CNPX3DMFTime);
DECLARE_NPOBJECT_CLASS(CNPX3DSFVec2f);
DECLARE_NPOBJECT_CLASS(CNPX3DMFVec2f);
DECLARE_NPOBJECT_CLASS(CNPX3DSFVec3f);
DECLARE_NPOBJECT_CLASS(CNPX3DMFVec3f);

#endif // _npx3dfield_h
