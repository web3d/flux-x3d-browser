/********************************************************************************
 * Flux
 *
 * File: npx3droute.cpp
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
 

#include "stdafx.h"
#include "npx3droute.h"
#include "npx3dnode.h"
#include "npx3dexecutioncontext.h"

#include "anmsairoute.h"
#include "anmroute.h"

/////////////////////////////////////////////////////////////////////////////
// CNPX3DRoute

CNPX3DRoute::~CNPX3DRoute()
{
	if (m_context)
		NPN_ReleaseObject(m_context);
}

void CNPX3DRoute::SetRoute(CAnmSAIRoute *pSAIRoute)
{
	m_route = pSAIRoute;
}

CAnmSAIRoute *CNPX3DRoute::GetRoute()
{
	return m_route;
}

	
void CNPX3DRoute::SetContext(CNPX3DExecutionContext *pContext)
{
	if (m_context)
		NPN_ReleaseObject(m_context);

	m_context = pContext;

	if (m_context)
		NPN_RetainObject(m_context);
}

CNPX3DExecutionContext *CNPX3DRoute::GetContext()
{
	if (m_context)
		NPN_RetainObject(m_context);

	return m_context;
}


bool CNPX3DRoute::getSourceNode(class CNPX3DNode **value)
{
	CAnmSAINode *pNode = m_route->GetFromNode();

	if (pNode)
	{
		CNPX3DNode *pX3DNode = CNPX3DNode::Create(mNpp, pNode, m_context);

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

bool CNPX3DRoute::getSourceField(const char **fieldname)
{
	assert (m_route);

	CLASSMEMBERID frommid = m_route->GetRoute()->GetFromCallback();
	assert(frommid != NULL);

	*fieldname = frommid->name;

	return true;
}


bool CNPX3DRoute::getDestinationNode(class CNPX3DNode **value)
{
	CAnmSAINode *pNode = m_route->GetToNode();

	if (pNode)
	{
		CNPX3DNode *pX3DNode = CNPX3DNode::Create(mNpp, pNode, m_context);

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

bool CNPX3DRoute::getDestinationField(const char **fieldname)
{
	assert (m_route);

	CLASSMEMBERID tomid = m_route->GetRoute()->GetToMethod();
	assert(tomid != NULL);

	*fieldname = tomid->name;

	return true;
}


// NP Glue

DEFINE_NPPROPID(CNPX3DRoute,sourceNode);
DEFINE_NPPROPID(CNPX3DRoute,sourceField);
DEFINE_NPPROPID(CNPX3DRoute,destinationNode);
DEFINE_NPPROPID(CNPX3DRoute,destinationField);
DEFINE_NPMETHODID(CNPX3DRoute,getSourceNode);
DEFINE_NPMETHODID(CNPX3DRoute,getSourceField);
DEFINE_NPMETHODID(CNPX3DRoute,getDestinationNode);
DEFINE_NPMETHODID(CNPX3DRoute,getDestinationField);


bool CNPX3DRoute::m_classInitialized = false;

static NPObject *
AllocateCNPX3DRoute(NPP npp, NPClass *aClass)
{
  return new CNPX3DRoute(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(CNPX3DRoute,
                                 AllocateCNPX3DRoute);

CNPX3DRoute *CNPX3DRoute::Create(NPP npp, CAnmSAIRoute *pSAIRoute, CNPX3DExecutionContext *context)
{
	CNPX3DRoute *pX3DRoute = (CNPX3DRoute *) NPN_CreateObject(npp,
                       GET_NPOBJECT_CLASS(CNPX3DRoute));

	if (pX3DRoute)
	{
		pX3DRoute->SetRoute(pSAIRoute);
		pX3DRoute->SetContext(context);
	}

	return pX3DRoute;
}


void CNPX3DRoute::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPPROPID(sourceNode);
	INIT_NPPROPID(sourceField);
	INIT_NPPROPID(destinationNode);
	INIT_NPPROPID(destinationField);
	INIT_NPMETHODID(getSourceNode);
	INIT_NPMETHODID(getSourceField);
	INIT_NPMETHODID(getDestinationNode);
	INIT_NPMETHODID(getDestinationField);

	m_classInitialized = true;
}

bool
CNPX3DRoute::HasMethod(NPIdentifier name)
{
  return 
	  TEST_NPMETHODID(name, getSourceNode) ||
	  TEST_NPMETHODID(name, getSourceField) ||
	  TEST_NPMETHODID(name, getDestinationNode) ||
	  TEST_NPMETHODID(name, getDestinationField) ||
	  false	  
	  ;

}

bool
CNPX3DRoute::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
	const char *cpret = NULL;
	CNPX3DNode *pNode = NULL;

	if (TEST_NPMETHODID(name,getSourceNode)) {

		if (getSourceNode(&pNode))
		{
			OBJECT_TO_NPVARIANT(pNode, *result);
			return PR_TRUE;
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPMETHODID(name,getSourceField)) {

		if (getSourceField(&cpret))
		{
			STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
		}

		return PR_TRUE;
	}
	else if (TEST_NPMETHODID(name,getDestinationNode)) {

		if (getDestinationNode(&pNode))
		{
			OBJECT_TO_NPVARIANT(pNode, *result);
			return PR_TRUE;
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPMETHODID(name,getDestinationField)) {

		if (getDestinationField(&cpret))
		{
			STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
		}

		return PR_TRUE;
	}

	return PR_FALSE;
}

bool
CNPX3DRoute::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  STRINGZ_TO_NPVARIANT(NPstrdup("[X3DProtoDeclaration]"), *result);

  return PR_TRUE;
}

bool
CNPX3DRoute::HasProperty(NPIdentifier name)
{
	return 
	  TEST_NPPROPID(name, sourceNode) ||
	  TEST_NPPROPID(name, sourceField) ||
	  TEST_NPPROPID(name, destinationNode) ||
	  TEST_NPPROPID(name, destinationField) ||

	  false ;
}

bool
CNPX3DRoute::GetProperty(NPIdentifier name, NPVariant *result)
{
	const char *cpret = NULL;
	CNPX3DNode *pNode = NULL;

	if (TEST_NPPROPID(name, sourceNode))
	{
		if (getSourceNode(&pNode))
		{
			OBJECT_TO_NPVARIANT(pNode, *result);
			return PR_TRUE;
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPPROPID(name, sourceField))
	{
		if (getSourceField(&cpret))
		{
			STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
		}

		return PR_TRUE;
	}
	else if (TEST_NPPROPID(name, destinationNode))
	{
		if (getDestinationNode(&pNode))
		{
			OBJECT_TO_NPVARIANT(pNode, *result);
			return PR_TRUE;
		}
		else
			return PR_FALSE;
	}
	else if (TEST_NPPROPID(name, destinationField))
	{
		if (getDestinationField(&cpret))
		{
			STRINGZ_TO_NPVARIANT(NPstrdup(cpret), *result);
		}

		return PR_TRUE;
	}

	return PR_FALSE;
}

bool
CNPX3DRoute::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return PR_FALSE;
}
