/********************************************************************************
 * Flux
 *
 * File: scriptablepluginobject.h
 * Description: Scriptable Plugin Object
 *							Framework for Flux
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

#ifndef _scriptablepluginobject_h
#define _scriptablepluginobject_h

// Helper class that can be used to map calls to the NPObject hooks
// into virtual methods on instances of classes that derive from this
// class.

#include "npapi.h"
#include "npruntime.h"


class ScriptablePluginObject : public NPObject
{
public:
  ScriptablePluginObject(NPP npp)
    : mNpp(npp)
  {
  }

  virtual ~ScriptablePluginObject()
  {
	  // just a test; failed miserably :-(
	  // assert(referenceCount == 1);
  }

  // Virtual NPObject hooks called through this base class. Override
  // as you see fit.
  virtual void Invalidate();
  virtual bool HasMethod(NPIdentifier name);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name, NPVariant *result);
  virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
  virtual bool RemoveProperty(NPIdentifier name);

public:
  static void _Deallocate(NPObject *npobj);
  static void _Invalidate(NPObject *npobj);
  static bool _HasMethod(NPObject *npobj, NPIdentifier name);
  static bool _Invoke(NPObject *npobj, NPIdentifier name,
                      const NPVariant *args, uint32_t argCount,
                      NPVariant *result);
  static bool _InvokeDefault(NPObject *npobj, const NPVariant *args,
                             uint32_t argCount, NPVariant *result);
  static bool _HasProperty(NPObject * npobj, NPIdentifier name);
  static bool _GetProperty(NPObject *npobj, NPIdentifier name,
                           NPVariant *result);
  static bool _SetProperty(NPObject *npobj, NPIdentifier name,
                           const NPVariant *value);
  static bool _RemoveProperty(NPObject *npobj, NPIdentifier name);

protected:
  NPP mNpp;
};

#define NPOBJECT_CLASS_NAME(_class) gNPClass_##_class##_NPClass 

#define DECLARE_NPOBJECT_CLASS(_class) extern NPClass NPOBJECT_CLASS_NAME(_class)

#define DEFINE_NPOBJECT_CLASS_WITH_BASE(_class, ctor)                        \
NPClass NPOBJECT_CLASS_NAME(_class) = {                                        \
  NP_CLASS_STRUCT_VERSION,                                                    \
  ctor,                                                                       \
  ScriptablePluginObject::_Deallocate,                                    \
  ScriptablePluginObject::_Invalidate,                                    \
  ScriptablePluginObject::_HasMethod,                                     \
  ScriptablePluginObject::_Invoke,                                        \
  ScriptablePluginObject::_InvokeDefault,                                 \
  ScriptablePluginObject::_HasProperty,                                   \
  ScriptablePluginObject::_GetProperty,                                   \
  ScriptablePluginObject::_SetProperty,                                   \
  ScriptablePluginObject::_RemoveProperty                                 \
}

#define GET_NPOBJECT_CLASS(_class) &NPOBJECT_CLASS_NAME(_class)

#define NPOBJECT_ISA(_p,_cls) (_p->_class == GET_NPOBJECT_CLASS(_cls))

#define NPMETHODID(_nm_) m_method##_nm_
#define DECLARE_NPMETHODID(_nm_) static NPIdentifier NPMETHODID(_nm_);
#define DEFINE_NPMETHODID(_cls_,_nm_) NPIdentifier _cls_::NPMETHODID(_nm_) = 0;
#define INIT_NPMETHODID(_nm_) NPMETHODID(_nm_) = NPN_GetStringIdentifier(#_nm_);
#define TEST_NPMETHODID(str,_nm_) (str == NPMETHODID(_nm_))

#define NPPROPID(_nm_) m_property##_nm_
#define DECLARE_NPPROPID(_nm_) static NPIdentifier NPPROPID(_nm_);
#define DEFINE_NPPROPID(_cls_,_nm_) NPIdentifier _cls_::NPPROPID(_nm_) = 0;
#define INIT_NPPROPID(_nm_) NPPROPID(_nm_) = NPN_GetStringIdentifier(#_nm_);
#define TEST_NPPROPID(str,_nm_) (str == NPPROPID(_nm_))

// Array class

class NPArray : public ScriptablePluginObject
{
protected :

	std::vector <NPVariant>			m_objects;

	// NP Glue
	static bool m_classInitialized;
	DECLARE_NPPROPID(length);

	void initClass();

public:

	NPArray(NPP npp)
	: ScriptablePluginObject(npp)
	{
		initClass();
	}

	virtual ~NPArray();

	// New Methods
	virtual int getLength();
	virtual bool set1Value(int index, NPVariant v);
	virtual bool get1Value(int index, NPVariant *pV);


	// NP Glue
	static NPArray *Create(NPP npp);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
					  uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
							 NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
	virtual bool RemoveProperty(NPIdentifier name);
};

// Various helpers

// Convert NPVARIANT data to Flux types
inline Boolean NPVToBoolean(const NPVariant &v)
{
	return NPVARIANT_TO_BOOLEAN(v);
}

inline String NPVToString(const NPVariant &v)
{
	NPString str = NPVARIANT_TO_STRING(v);

	int len = str.utf8length;
	if (!len)
		return NULL;

	String s = new CAnmString;
	s->alloc(len + 1);
	char *cp = s->GetBuf();
	strncpy(cp, str.utf8characters, len);
	cp[len] = '\0';

	return s;
}

inline Double NPVToDouble(const NPVariant &v)
{
	double d;

	if (NPVARIANT_IS_DOUBLE(v))
		return NPVARIANT_TO_DOUBLE(v);
	else if (NPVARIANT_IS_INT32(v))
		return (double) NPVARIANT_TO_INT32(v);
	else if (NPVARIANT_IS_BOOLEAN(v))
		return (double) NPVARIANT_TO_BOOLEAN(v);
	else if (NPVARIANT_IS_STRING(v))
	{
		String s = NPVToString(v);
		if (s)
		{
			sscanf(s->GetBuf(), "%lg", &d);

			SafeDelete(s);

			return d;
		}
		else
			return 0.f;
	}
	else
		return 0.f;
	
	return 0.f;
}

inline Integer NPVToInteger(const NPVariant &v)
{
	if (NPVARIANT_IS_INT32(v))
		return NPVARIANT_TO_INT32(v);
	else if (NPVARIANT_IS_DOUBLE(v))
		return (int) NPVARIANT_TO_DOUBLE(v);
	else if (NPVARIANT_IS_BOOLEAN(v))
		return (int) NPVARIANT_TO_BOOLEAN(v);
	else if (NPVARIANT_IS_STRING(v))
	{
		String s = NPVToString(v);
		if (s)
		{
			int i32;
			sscanf(s->GetBuf(), "%d", &i32);

			SafeDelete(s);

			return i32;
		}
		else
			return 0L;
	}
	else
		return 0L;
	
	return 0L;
}


inline NPObject *NPVToObject(const NPVariant &v)
{
	if (NPVARIANT_IS_OBJECT(v))
		return NPVARIANT_TO_OBJECT(v);
	else
		return NULL;
}

inline char *NPstrdup(const char *cp)
{
	int len;

	if (!cp)
		len = 0;
	else
		len = strlen(cp);

	char *dup = (char *) NPN_MemAlloc(len + 1);

	if (len)
		strcpy(dup, cp);
	else
		strcpy(dup, "");

	return dup;
}

#endif // _scriptablepluginobject_h
