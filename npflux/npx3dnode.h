/********************************************************************************
 * Flux
 *
 * File: npx3dnode.h
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

#ifndef _npx3dnode_h
#define _npx3dnode_h

#include "scriptablepluginobject.h"
#include "npx3dfield.h"
 
class CNPX3DNode : public ScriptablePluginObject
{
protected :

	class CAnmSAINode					*m_node;
	class CNPX3DExecutionContext		*m_context;

	// NP Glue
	static bool m_classInitialized;

	DECLARE_NPMETHODID(getFieldDefinitions);
	DECLARE_NPMETHODID(getFieldInfo);
	DECLARE_NPMETHODID(getNumFields);
	DECLARE_NPMETHODID(getName);
	DECLARE_NPMETHODID(getField);
	DECLARE_NPMETHODID(getTypeName);

	void initClass();

	bool getFieldAsVariant(char *fieldname, NPVariant *result);
	bool setFieldFromVariant(char *fieldname, const NPVariant *value);

public:

	CNPX3DNode(NPP npp)

		: ScriptablePluginObject(npp)
		{
		  m_node = NULL;
		  m_context = NULL;

		  initClass();
		}

	virtual ~CNPX3DNode();

	// New methods
	virtual bool getFieldDefinitions(/*[out, retval]*/ class CNPX3DFieldDefinitionArray **fieldDefinitions);
	virtual bool getFieldInfo(/*[in]*/ int fieldIndex, /*[out]*/ NPX3DFieldAccess *accessType, 
		/*[out]*/ NPX3DFieldType *fieldType, /*[out]*/ char **fieldName);

	virtual bool getNumFields(/*[out, retval]*/ int *result);
	virtual bool getName(/*[out, retval]*/ const char **name);
	virtual bool getField(/*[in]*/ const char *fieldname, /*[out, retval]*/ class CNPX3DField **result);
	virtual bool getTypeName(/*[out, retval]*/ const char **typenm);

	// Accessors
	virtual void SetNode(class CAnmSAINode *pNode);
	virtual class CAnmSAINode *GetNode();
	virtual void SetContext(class CNPX3DExecutionContext *context);
	virtual class CNPX3DExecutionContext *GetContext();


	
	// NP Glue
	static CNPX3DNode *CNPX3DNode::Create(NPP npp, class CAnmSAINode *pSAINode, class CNPX3DExecutionContext *pContext);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

DECLARE_NPOBJECT_CLASS(CNPX3DNode);

#endif // _npx3dnode_h
