/********************************************************************************
 * Flux
 *
 * File: npx3dscene.h
 * Description: Mozilla X3DScene
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

#ifndef _npx3dscene_h
#define _npx3dscene_h

#include "npx3dexecutioncontext.h"

class CNPX3DScene : public CNPX3DExecutionContext
{
protected :

	// NP Glue
	static bool m_classInitialized;

	DECLARE_NPMETHODID(getMetaData);
	DECLARE_NPMETHODID(setMetaData);
	DECLARE_NPMETHODID(addRootNode);
	DECLARE_NPMETHODID(removeRootNode);

	void initClass();

public:

	// Constructor/destructor
	CNPX3DScene(NPP npp)
	: CNPX3DExecutionContext(npp)
	{
		initClass();
	}

	virtual ~CNPX3DScene();

	// New methods
 	virtual bool getMetaData(/*[in]*/ String key, /*[out,retval]*/ const char **value);
 	virtual bool setMetaData(/*[in]*/ String key, /*[in]*/ String value);
 	virtual bool addRootNode(/*[in]*/ class CNPX3DNode *node);
 	virtual bool removeRootNode(/*[in]*/ class CNPX3DNode *node);

	// NP Glue
	static CNPX3DScene *CNPX3DScene::Create(NPP npp, class CAnmExecutionContext *pContext);

	virtual bool HasMethod(NPIdentifier name);
	virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
	virtual bool HasProperty(NPIdentifier name);
	virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};

#endif // _npx3dscene_h
