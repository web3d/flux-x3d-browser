/********************************************************************************
 * Flux
 *
 * File: npx3dfielddefinition.h
 * Description: Mozilla X3DFieldDefinition
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

#ifndef _npx3dfielddefinition_h
#define _npx3dfielddefinition_h

#include "scriptablepluginobject.h"

class CNPX3DFieldDefinition : public ScriptablePluginObject
{
protected :

	static bool m_classInitialized;

	DECLARE_NPPROPID(name);
	DECLARE_NPPROPID(accessType);
	DECLARE_NPPROPID(dataType);

	CLASSMEMBERID m_mid;

	void initClass();

public:
	CNPX3DFieldDefinition(NPP npp)
	: ScriptablePluginObject(npp)
	{
		m_mid = NULL;

		initClass();
	}

	virtual ~CNPX3DFieldDefinition();

	void SetClassMember(CLASSMEMBERID mid);
	CLASSMEMBERID GetClassMember();

	virtual bool getName(const char **name);
	virtual bool getAccessType(int *accessType);
	virtual bool getDataType(int *dataType);

	// NP Glue
	static CNPX3DFieldDefinition *Create(NPP npp, CLASSMEMBERID mid);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

#endif // _npx3dfielddefinition_h
