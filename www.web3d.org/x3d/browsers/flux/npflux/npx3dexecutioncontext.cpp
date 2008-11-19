/********************************************************************************
 * Flux
 *
 * File: npx3dexecutioncontext.cpp
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
 

#include "stdafx.h"
#include "npx3dexecutioncontext.h"
#include "npx3dnode.h"
#include "npx3dprotodeclaration.h"
#include "npx3droute.h"
 
#include "anmexecutioncontext.h"
#include "anmroute.h"
#include "anmsainode.h"

#include "anmsymbol.h"

/////////////////////////////////////////////////////////////////////////////
// CNPX3DExecutionContext

CNPX3DExecutionContext::~CNPX3DExecutionContext()
{
	SafeUnRef(m_context);
}

void CNPX3DExecutionContext::SetContext(CAnmExecutionContext *pContext)
{
	SafeUnRef(m_context);

	m_context = pContext;
	m_context->Ref();
}

void CNPX3DExecutionContext::GetContext(CAnmExecutionContext **ppContext)
{
	*ppContext = m_context;
}

bool CNPX3DExecutionContext::getSpecificationVersion(const char **value)
{
	*value = m_context->GetSpecificationVersion();

	return true;
}

bool CNPX3DExecutionContext::getEncoding(eAnmX3DEncoding *encodingType)
{
	*encodingType = m_context->GetEncoding();

	return true;
}


bool CNPX3DExecutionContext::getNode(const char *name, class CNPX3DNode **value)
{
	CAnmSAINode *pNode = m_context->GetNode(name);

	if (pNode)
	{
		CNPX3DNode *pX3DNode = CNPX3DNode::Create(mNpp, pNode, this);

		if (pX3DNode)
		{
			NPN_RetainObject(pX3DNode);

			*value = pX3DNode;

			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool CNPX3DExecutionContext::createNode(const char *type, class CNPX3DNode **value)
{
	CAnmSAINode *pNode = m_context->CreateNode(type);

	if (pNode)
	{
		CNPX3DNode *pX3DNode = CNPX3DNode::Create(mNpp, pNode, this);

		if (pX3DNode)
		{
			NPN_RetainObject(pX3DNode);

			*value = pX3DNode;

			return true;
		}
		else
			return false;
	}
	else
		return false;
}


bool CNPX3DExecutionContext::addRoute(class CNPX3DNode *fromNode, const char *fromEventOut, 
									  class CNPX3DNode *toNode, const char *toEventIn, 
									  class CNPX3DRoute **ppRoute)
{
	CAnmSAINode *pFromNode = NULL, *pToNode = NULL;
	CAnmRoute *pRoute = NULL;

	if (fromNode == NULL || toNode == NULL)
		return false;

	pFromNode = fromNode->GetNode();
	if (pFromNode)
	{
		pToNode = toNode->GetNode();
		if (pToNode)
		{
			CAnmSAIRoute *pSAIRoute = m_context->AddRoute(pFromNode, fromEventOut, pToNode, toEventIn);
			if (pSAIRoute)
			{
				*ppRoute = CNPX3DRoute::Create(mNpp, pSAIRoute, this);
				return true;
			}
			else
				return false;
		}

		return false;
	}

	return false;
}

bool CNPX3DExecutionContext::deleteRoute(CNPX3DRoute *pX3DRoute)
{
	CAnmSAIRoute *pSAIRoute = pX3DRoute->GetRoute();

	if (pSAIRoute)
	{
		m_context->RemoveRoute(pSAIRoute);
		return true;
	}
	else
		return false;

}


bool CNPX3DExecutionContext::getRootNodes(class NPArray **rootnodes)
{
	if (rootnodes == NULL)
		return false;

	std::vector<class CAnmSAINode *> *pSAIRootNodes = m_context->GetRootNodes();

	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	int sz = pSAIRootNodes->size();

	if (sz <= 0)
		return true;

	*rootnodes = pArray;

	for (int i = 0; i < sz; i++)
	{
		CAnmSAINode *pSAINode = (*pSAIRootNodes)[i];

		CNPX3DNode *pX3DNode = CNPX3DNode::Create(mNpp, pSAINode, this);

		NPN_RetainObject(pX3DNode);

		NPVariant v;

		OBJECT_TO_NPVARIANT(pX3DNode, v);

		pArray->set1Value(i, v);
	}

	delete pSAIRootNodes;

	return true;

}

bool CNPX3DExecutionContext::getProtoDeclarations(NPArray **protoDeclarations)
{
	int nProtoDecls;
	CAnmProtoSymbol **ppProtoSymbols = NULL;

	m_context->GetProtoDeclarations(&nProtoDecls, &ppProtoSymbols);

	if (nProtoDecls <= 0)
		return true;

	NPArray *pArray = NPArray::Create(mNpp);
	NPN_RetainObject(pArray);

	*protoDeclarations = pArray;
	
	for (int i = 0; i < nProtoDecls; i++)
	{
		CAnmProtoSymbol *pProtoSym = ppProtoSymbols[i];

		CNPX3DProtoDeclaration *pProtoDeclaration = CNPX3DProtoDeclaration::Create(mNpp, pProtoSym);

		NPN_RetainObject(pProtoDeclaration);

		NPVariant v;

		OBJECT_TO_NPVARIANT(pProtoDeclaration, v);

		pArray->set1Value(i, v);
	}

	if (ppProtoSymbols)
		delete ppProtoSymbols;

	return true;
}

// NP Glue

DEFINE_NPMETHODID(CNPX3DExecutionContext,getSpecificationVersion);
DEFINE_NPMETHODID(CNPX3DExecutionContext,getEncoding);
DEFINE_NPMETHODID(CNPX3DExecutionContext,getNode);
DEFINE_NPMETHODID(CNPX3DExecutionContext,createNode);
DEFINE_NPMETHODID(CNPX3DExecutionContext,addRoute);
DEFINE_NPMETHODID(CNPX3DExecutionContext,deleteRoute);
DEFINE_NPMETHODID(CNPX3DExecutionContext,getRootNodes);
DEFINE_NPMETHODID(CNPX3DExecutionContext,getProtoDeclarations);

bool CNPX3DExecutionContext::m_classInitialized = false;

static NPObject *
AllocateCNPX3DExecutionContext(NPP npp, NPClass *aClass)
{
  return new CNPX3DExecutionContext(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DExecutionContext,
                                 AllocateCNPX3DExecutionContext);

CNPX3DExecutionContext *CNPX3DExecutionContext::Create(NPP npp, CAnmExecutionContext *pContext)
{
	CNPX3DExecutionContext *pX3DContext = (CNPX3DExecutionContext *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DExecutionContext));

	if (pX3DContext)
		pX3DContext->SetContext(pContext);

	return pX3DContext;
}


void CNPX3DExecutionContext::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPMETHODID(getSpecificationVersion);
	INIT_NPMETHODID(getEncoding);
	INIT_NPMETHODID(getNode);
	INIT_NPMETHODID(createNode);
	INIT_NPMETHODID(addRoute);
	INIT_NPMETHODID(deleteRoute);
	INIT_NPMETHODID(getRootNodes);
	INIT_NPMETHODID(getProtoDeclarations);

	m_classInitialized = true;
}

bool
CNPX3DExecutionContext::HasMethod(NPIdentifier name)
{
  return 
	  TEST_NPMETHODID(name, getSpecificationVersion) ||
	  TEST_NPMETHODID(name, getEncoding) ||
	  TEST_NPMETHODID(name, getNode) ||
	  TEST_NPMETHODID(name, createNode) ||
	  TEST_NPMETHODID(name, addRoute) ||
	  TEST_NPMETHODID(name, deleteRoute) ||
	  TEST_NPMETHODID(name, getRootNodes) ||
	  TEST_NPMETHODID(name, getProtoDeclarations) ||
	  false	  
	  ;

}

bool
CNPX3DExecutionContext::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	const char *cpret = NULL;
	String s0 = NULL;
	String s1 = NULL;
	NPObject *pObj = NULL;
	bool retval = false;

	if (TEST_NPMETHODID(name,getSpecificationVersion)) {

		if (getSpecificationVersion(&cpret))
		{
			STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
		}

		return PR_TRUE;
	}
	else if (TEST_NPMETHODID(name,getEncoding)) {
		eAnmX3DEncoding encoding;
		if (getEncoding(&encoding))
		{
			INT32_TO_NPVARIANT((int32) encoding, *result);
		}

		return PR_TRUE;
	}
	else if (TEST_NPMETHODID(name,getNode)) {

		if (argCount > 0)
		{
			s0 = NPVToString(args[0]);

			if (s0)
			{
				CNPX3DNode *pNode = NULL;
				if (getNode(s0->GetBuf(), &pNode))
				{
					OBJECT_TO_NPVARIANT(pNode, *result);
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
	else if (TEST_NPMETHODID(name,createNode)) {

		if (argCount > 0)
		{
			s0 = NPVToString(args[0]);

			if (s0)
			{
				CNPX3DNode *pNode = NULL;
				if (createNode(s0->GetBuf(), &pNode))
				{
					OBJECT_TO_NPVARIANT(pNode, *result);
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
	else if (TEST_NPMETHODID(name,getRootNodes)) {

		NPArray *pArray = NULL;

		if (getRootNodes(&pArray))
		{
			OBJECT_TO_NPVARIANT(pArray, *result);
			return PR_TRUE;
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPMETHODID(name,getProtoDeclarations)) {

		NPArray *pArray = NULL;

		if (getProtoDeclarations(&pArray))
		{
			OBJECT_TO_NPVARIANT(pArray, *result);
			return PR_TRUE;
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPMETHODID(name,addRoute)) {

		if (argCount > 3)
		{
			CNPX3DNode *pFromX3DNode = NULL;
			CNPX3DNode *pToX3DNode = NULL;

			retval = PR_TRUE;

			s0 = NPVToString(args[0]);

			if (s0)
			{
				pObj = NPVToObject(args[1]);

				if (pObj)
				{
					if (NPOBJECT_ISA(pObj, CNPX3DNode))
					{
						// N.B.: type safety; is there a better way?
						pFromX3DNode = (CNPX3DNode *) pObj;

						s1 = NPVToString(args[2]);

						if (s1)
						{
							pObj = NPVToObject(args[3]);

							if (pObj)
							{
								if (NPOBJECT_ISA(pObj, CNPX3DNode))
								{
									// N.B.: type safety; is there a better way?
									pToX3DNode = (CNPX3DNode *) pObj;

									CNPX3DRoute *pRoute = NULL;
									if (addRoute(pFromX3DNode, s0->GetBuf(), pToX3DNode, s1->GetBuf(), &pRoute))
									{
										NPN_RetainObject(pRoute);

										OBJECT_TO_NPVARIANT(pRoute, *result);
									}
									else
										retval = PR_FALSE;
								}
								else
									retval = PR_FALSE;
							}
							else retval = PR_FALSE;
						}
						else
							retval = PR_FALSE;
					}
					else
						retval = PR_FALSE;
				}
				else
					retval = PR_FALSE;
			}
			else
				retval = PR_FALSE;

			SafeUnRef(s0);
			SafeUnRef(s1);
			return retval;

		}
		else
			return PR_FALSE;
	}

  return PR_FALSE;
}

bool
CNPX3DExecutionContext::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  STRINGZ_TO_NPVARIANT(NPstrdup("[X3DExecutionContext]"), *result);

  return PR_TRUE;
}

bool
CNPX3DExecutionContext::HasProperty(NPIdentifier name)
{
  return false;
}

bool
CNPX3DExecutionContext::GetProperty(NPIdentifier name, NPVariant *result)
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
CNPX3DExecutionContext::SetProperty(NPIdentifier name,
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
