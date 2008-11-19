/********************************************************************************
 * Flux
 *
 * File: npx3dexecutioncontext.h
 * Description: Mozilla X3DExecutionContext
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

#ifndef _npx3dexecutioncontext_h
#define _npx3dexecutioncontext_h

#include "scriptablepluginobject.h"

class CNPX3DExecutionContext : public ScriptablePluginObject
{
protected :

	static bool m_classInitialized;

	DECLARE_NPMETHODID(getSpecificationVersion);
	DECLARE_NPMETHODID(getEncoding);
	DECLARE_NPMETHODID(getNode);
	DECLARE_NPMETHODID(createNode);
	DECLARE_NPMETHODID(addRoute);
	DECLARE_NPMETHODID(deleteRoute);
	DECLARE_NPMETHODID(getRootNodes);
	DECLARE_NPMETHODID(getProtoDeclarations);

	class CAnmExecutionContext *m_context;

	void initClass();

public:
	CNPX3DExecutionContext(NPP npp)
	: ScriptablePluginObject(npp)
	{
		m_context = NULL;

		initClass();
	}

	virtual ~CNPX3DExecutionContext();

	void SetContext(class CAnmExecutionContext *pContext);
	void GetContext(class CAnmExecutionContext **ppContext);

	virtual bool getSpecificationVersion(/*[out,retval]*/ const char **value);
	virtual bool getEncoding(/*[out,retval]*/ eAnmX3DEncoding *encodingType);
	virtual bool getNode(/*[in]*/ const char *name, /*[out, retval]*/ class CNPX3DNode **node);
	virtual bool createNode(const char *type, /*[out, retval]*/ class CNPX3DNode **node);
	virtual bool addRoute(/*[in]*/ class CNPX3DNode * fromNode, /*[in]*/ const char *fromEventOut, /*[in]*/ class CNPX3DNode *toNode, 
		/*[in]*/ const char *toEventIn, class CNPX3DRoute **ppRoute);
	virtual bool deleteRoute(/*[in]*/ class CNPX3DRoute *pX3DRoute);
	virtual bool getRootNodes(/*[out,retval]*/ class NPArray **rootnodes);
	virtual bool getProtoDeclarations(/*[out,retval]*/ class NPArray **protoDeclarations);


	// NP Glue
	static CNPX3DExecutionContext *Create(NPP npp, class CAnmExecutionContext *pContext);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

#endif // _npx3dexecutioncontext_h
