/********************************************************************************
 * Flux
 *
 * File: scriptablepluginobject.cpp
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

#include "stdafx.h"

#include "scriptablepluginobject.h"

void
ScriptablePluginObject::Invalidate()
{
}

bool
ScriptablePluginObject::HasMethod(NPIdentifier name)
{
  return false;
}

bool
ScriptablePluginObject::Invoke(NPIdentifier name, const NPVariant *args,
                                   uint32_t argCount, NPVariant *result)
{
  return false;
}

bool
ScriptablePluginObject::InvokeDefault(const NPVariant *args,
                                          uint32_t argCount, NPVariant *result)
{
  return false;
}

bool
ScriptablePluginObject::HasProperty(NPIdentifier name)
{
  return false;
}

bool
ScriptablePluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
	return false;
}

bool
ScriptablePluginObject::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return false;
}

bool
ScriptablePluginObject::RemoveProperty(NPIdentifier name)
{
  return false;
}

// static
void
ScriptablePluginObject::_Deallocate(NPObject *npobj)
{
  // Call the virtual destructor.
  delete (ScriptablePluginObject *)npobj;
}

// static
void
ScriptablePluginObject::_Invalidate(NPObject *npobj)
{
  ((ScriptablePluginObject *)npobj)->Invalidate();
}

// static
bool
ScriptablePluginObject::_HasMethod(NPObject *npobj, NPIdentifier name)
{
  return ((ScriptablePluginObject *)npobj)->HasMethod(name);
}

// static
bool
ScriptablePluginObject::_Invoke(NPObject *npobj, NPIdentifier name,
                                    const NPVariant *args, uint32_t argCount,
                                    NPVariant *result)
{
  return ((ScriptablePluginObject *)npobj)->Invoke(name, args, argCount,
                                                       result);
}

// static
bool
ScriptablePluginObject::_InvokeDefault(NPObject *npobj,
                                           const NPVariant *args,
                                           uint32_t argCount,
                                           NPVariant *result)
{
  return ((ScriptablePluginObject *)npobj)->InvokeDefault(args, argCount,
                                                              result);
}

// static
bool
ScriptablePluginObject::_HasProperty(NPObject * npobj, NPIdentifier name)
{
  return ((ScriptablePluginObject *)npobj)->HasProperty(name);
}

// static
bool
ScriptablePluginObject::_GetProperty(NPObject *npobj, NPIdentifier name,
                                         NPVariant *result)
{
  return ((ScriptablePluginObject *)npobj)->GetProperty(name, result);
}

// static
bool
ScriptablePluginObject::_SetProperty(NPObject *npobj, NPIdentifier name,
                                         const NPVariant *value)
{
  return ((ScriptablePluginObject *)npobj)->SetProperty(name, value);
}

// static
bool
ScriptablePluginObject::_RemoveProperty(NPObject *npobj, NPIdentifier name)
{
  return ((ScriptablePluginObject *)npobj)->RemoveProperty(name);
}

// NPArray

NPArray::~NPArray()
{
	int sz = m_objects.size();

	for (int i = 0; i < sz; i++)
	{
#if 0
		NPObject *pObj = m_objects[i];

		if (pObj)
			NPN_ReleaseObject(pObj);
#endif
	}

	m_objects.clear();
}

int NPArray::getLength()
{
	return m_objects.size();
}

bool NPArray::set1Value(int index, NPVariant v)
{
	if (index < 0)
		return false;

	// First, pad the array if needed
	NPVariant vvoid;
	VOID_TO_NPVARIANT(vvoid);
	int sz = m_objects.size();
	for (int i = sz; i <= index; i++)
		m_objects.push_back(vvoid);

	// Release any old pointer
#if 0
	if (m_objects[index] != NULL)
	{
		NPN_ReleaseObject(m_objects[index]);
	}
#endif
	// Ok, stuff it in. 
	m_objects[index] = v;

	// Add a reference
	// NPN_RetainObject(pObj);

	return true;
}

bool NPArray::get1Value(int index, NPVariant *pV)
{
	int sz = m_objects.size();
	if (index < 0 || index >= sz)
		return false;

	*pV = m_objects[index];

	if (NPVARIANT_IS_OBJECT(*pV))
	{
		NPObject *pObj = NPVARIANT_TO_OBJECT(*pV);
		if (pObj)
			NPN_RetainObject(pObj);
	}

	return true;
}

// NPGlue (NPArray)

DEFINE_NPPROPID(NPArray,length);

bool NPArray::m_classInitialized = false;
static NPObject *
AllocateNPArray(NPP npp, NPClass *aClass)
{
  return new NPArray(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(NPArray,
                                 AllocateNPArray);

NPArray *NPArray::Create(NPP npp)
{
	return (NPArray *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(NPArray));
}


void NPArray::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPPROPID(length);

	m_classInitialized = true;
}


bool
NPArray::HasMethod(NPIdentifier name)
{
  return false;
}

bool
NPArray::Invoke(NPIdentifier name, const NPVariant *args,
                                   uint32_t argCount, NPVariant *result)
{
  return false;
}

bool
NPArray::InvokeDefault(const NPVariant *args,
                                          uint32_t argCount, NPVariant *result)
{
  return false;
}

bool
NPArray::HasProperty(NPIdentifier name)
{
	if (!NPN_IdentifierIsString(name))
		return true;
	else
		return
		TEST_NPPROPID(name, length) ||
		false;
}

bool
NPArray::GetProperty(NPIdentifier name, NPVariant *result)
{
	if (!NPN_IdentifierIsString(name))
	{
		int id = NPN_IntFromIdentifier(name);

		return get1Value(id, result);

	}
	if (TEST_NPPROPID(name, length))
	{
		int sz = m_objects.size();
		INT32_TO_NPVARIANT(sz, *result);
		return true;
	}

	return false;
}

bool
NPArray::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return false;
}

bool
NPArray::RemoveProperty(NPIdentifier name)
{
  return false;
}
