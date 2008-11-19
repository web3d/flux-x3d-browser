/********************************************************************************
 * Flux
 *
 * File: npx3dscene.cpp
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
 

#include "stdafx.h"
#include "npx3dscene.h"
#include "npx3dnode.h"
 
#include "anmsaiscene.h"
#include "anmroute.h"
#include "anmsainode.h"

/////////////////////////////////////////////////////////////////////////////
// CNPX3DScene

CNPX3DScene::~CNPX3DScene()
{
}

bool CNPX3DScene::getMetaData(/*[in]*/ String key, /*[out,retval]*/ const char **value)
{
	CAnmSAIScene *pSAIScene = (CAnmSAIScene *) m_context;
	assert(pSAIScene);

	if (key == NULL)
		return false;

	String s = pSAIScene->GetMetaData(key->GetBuf());

	if (s)
		*value = s->GetBuf();
	else
		*value = "";

	return true;
}

bool CNPX3DScene::setMetaData(/*[in]*/ String key, /*[in]*/ String value)
{
	CAnmSAIScene *pSAIScene = (CAnmSAIScene *) m_context;
	assert(pSAIScene);

	if (key == NULL)
		return false;

	if (value == NULL)
		return false;

	pSAIScene->SetMetaData(key, value);

	return true;
}


bool CNPX3DScene::addRootNode(/*[in]*/ class CNPX3DNode *node)
{
	CAnmSAINode *pSAINode = node->GetNode();
	if (pSAINode)
	{
		CAnmSAIScene *pSAIScene = (CAnmSAIScene *) m_context;
		assert(pSAIScene);

		pSAIScene->AddRootNode(pSAINode);

		node->SetContext(this);

		return true;
	}
	else
		return false;
}

bool CNPX3DScene::removeRootNode(/*[in]*/ class CNPX3DNode *node)
{
	CAnmSAINode *pSAINode = node->GetNode();
	if (pSAINode)
	{
		CAnmSAIScene *pSAIScene = (CAnmSAIScene *) m_context;
		assert(pSAIScene);

		pSAIScene->RemoveRootNode(pSAINode);

		return true;
	}
	else
		return false;

	return false;
}


// NP Glue
DEFINE_NPMETHODID(CNPX3DScene,getMetaData);
DEFINE_NPMETHODID(CNPX3DScene,setMetaData);
DEFINE_NPMETHODID(CNPX3DScene,addRootNode);
DEFINE_NPMETHODID(CNPX3DScene,removeRootNode);

bool CNPX3DScene::m_classInitialized = false;

static NPObject *
AllocateCNPX3DScene(NPP npp, NPClass *aClass)
{
  return new CNPX3DScene(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DScene,
                                 AllocateCNPX3DScene);

CNPX3DScene *CNPX3DScene::Create(NPP npp, CAnmExecutionContext *pContext)
{

	CNPX3DScene *pX3DScene = (CNPX3DScene *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DScene));

	if (pX3DScene)
		pX3DScene->SetContext(pContext);

	return pX3DScene;
}


void CNPX3DScene::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPMETHODID(getMetaData);
	INIT_NPMETHODID(setMetaData);
	INIT_NPMETHODID(addRootNode);
	INIT_NPMETHODID(removeRootNode);

	m_classInitialized = true;
}

bool
CNPX3DScene::HasMethod(NPIdentifier name)
{
  return 
	  TEST_NPMETHODID(name, getMetaData) ||
	  TEST_NPMETHODID(name, setMetaData) ||
	  TEST_NPMETHODID(name, addRootNode) ||
	  TEST_NPMETHODID(name, removeRootNode) ||
	  CNPX3DExecutionContext::HasMethod(name);
	  ;

}

bool
CNPX3DScene::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	const char *cpret = NULL;
	String s0 = NULL;
	String s1 = NULL;
	NPObject *pObj = NULL;
	bool retval;

	if (TEST_NPMETHODID(name,getMetaData))
	{
		if (argCount > 0)
		{
			s0 = NPVToString(args[0]);

			if (s0)
			{
				if (getMetaData(s0, &cpret))
				{
					STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
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
	else if (TEST_NPMETHODID(name,setMetaData)) 
	{
		if (argCount > 1)
		{
			s0 = NPVToString(args[0]);
			s1 = NPVToString(args[1]);

			if (s0 && s1)
			{
				retval = setMetaData(s0, s1);
			}
			else
				retval = false;

			SafeUnRef(s0);
			SafeUnRef(s1);

			return retval;

		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPMETHODID(name,addRootNode))
	{
		if (argCount > 0)
		{
			pObj = NPVToObject(args[0]);

			if (pObj)
			{
				if (NPOBJECT_ISA(pObj, CNPX3DNode))
				{
					// N.B.: type safety; is there a better way?
					CNPX3DNode *pX3DNode = (CNPX3DNode *) pObj;

					return addRootNode(pX3DNode);
				}
				else
					return PR_FALSE;
			}
			else
				return PR_FALSE;
		}
		else return PR_FALSE;
	}
	else if (TEST_NPMETHODID(name,removeRootNode))
	{
		if (argCount > 0)
		{
			pObj = NPVToObject(args[0]);

			if (pObj)
			{
				if (NPOBJECT_ISA(pObj, CNPX3DNode))
				{
					// N.B.: type safety; is there a better way?
					CNPX3DNode *pX3DNode = (CNPX3DNode *) pObj;

					return removeRootNode(pX3DNode);
				}
				else
					return PR_FALSE;
			}
			else
				return PR_FALSE;
		}
		else return PR_FALSE;
	}
	else
		return CNPX3DExecutionContext::Invoke(name, args, argCount, result);

  return PR_FALSE;
}

bool
CNPX3DScene::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  STRINGZ_TO_NPVARIANT(NPstrdup("[X3DExecutionContext]"), *result);

  return PR_TRUE;
}

bool
CNPX3DScene::HasProperty(NPIdentifier name)
{
  return false;
}

bool
CNPX3DScene::GetProperty(NPIdentifier name, NPVariant *result)
{
/*
  if (name == sBar_id) {
    static int a = 17;

    INT32_TO_NPVARIANT(a, *result);

    a += 5;

    return true;
  }

  VOID_TO_NPVARIANT(*result);
*/

  return false;
}

bool
CNPX3DScene::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
/*
  if (name == sBar_id) {
    printf ("bar set\n");

    int a = 5;

    a = NPVToInteger(*value);

    return true;
  }

*/
  return false;
}
