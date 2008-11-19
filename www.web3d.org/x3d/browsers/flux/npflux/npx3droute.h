/********************************************************************************
 * Flux
 *
 * File: npx3droute.h
 * Description: Mozilla X3DRoute
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

#ifndef _npx3droute_h
#define _npx3droute_h

#include "scriptablepluginobject.h"

class CNPX3DRoute : public ScriptablePluginObject
{
protected :

	static bool m_classInitialized;

	DECLARE_NPPROPID(sourceNode);
	DECLARE_NPPROPID(sourceField);
	DECLARE_NPPROPID(destinationNode);
	DECLARE_NPPROPID(destinationField);

	DECLARE_NPMETHODID(getSourceNode);
	DECLARE_NPMETHODID(getSourceField);
	DECLARE_NPMETHODID(getDestinationNode);
	DECLARE_NPMETHODID(getDestinationField);

	class CAnmSAIRoute *m_route;
	class CNPX3DExecutionContext		*m_context;

	void initClass();

public:
	CNPX3DRoute(NPP npp)
	: ScriptablePluginObject(npp)
	{
		m_route = NULL;
		m_context = NULL;
		initClass();
	}

	virtual ~CNPX3DRoute();

	void SetRoute(class CAnmSAIRoute *pSAIRoute);
	class CAnmSAIRoute *GetRoute();
	virtual void SetContext(class CNPX3DExecutionContext *context);
	virtual class CNPX3DExecutionContext *GetContext();

	virtual bool getSourceNode(class CNPX3DNode **value);
	virtual bool getSourceField(const char **fieldname);
	virtual bool getDestinationNode(class CNPX3DNode **value);
	virtual bool getDestinationField(const char **fieldname);

	// NP Glue
	static CNPX3DRoute *Create(NPP npp, class CAnmSAIRoute *pSAIRoute, class CNPX3DExecutionContext *context);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

#endif // _npx3droute_h
