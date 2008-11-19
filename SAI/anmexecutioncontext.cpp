/********************************************************************************
 * Flux
 *
 * File: anmexecutioncontext.cpp
 * Description: Execution Context object
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
#include "anmexecutioncontext.h"
#include "anmnode.h"
#include "anmproto.h"
#include "anmroute.h"
#include "anmsainode.h"
#include "anmsairoute.h"
#include "anmsymbol.h"

CAnmExecutionContext::CAnmExecutionContext(cApplication *pApp, CAnmProto *pProto,
										   CAnmSAINodeEventSink *pSink)
{
	assert(pApp);
	assert(pProto);

	m_app = pApp;
	m_proto = pProto;
	m_sink = pSink;
}

CAnmExecutionContext::~CAnmExecutionContext()
{
	// N.B.: do I need to ref/unref anything else?
}

char *CAnmExecutionContext::GetSpecificationVersion()
{
	return m_proto->GetSpecificationVersion();
}

eAnmX3DEncoding CAnmExecutionContext::GetEncoding()
{
	return m_proto->GetEncoding();
}

class CAnmProfileInfo *CAnmExecutionContext::GetProfile()
{
	return m_proto->GetProfile();
}

std::vector<class CAnmComponentInfo *> *CAnmExecutionContext::GetComponents()
{
	return m_proto->GetComponents();
}

const char *CAnmExecutionContext::GetWorldURL()
{
	return m_proto->GetWorldURL();
}

CAnmSAINode *CAnmExecutionContext::GetNode(const char *nodename)
{
	CAnmNode *pNode = m_proto->GetNode(nodename);

	if (pNode)
	{
		CAnmSAINode *pSAINode = CreateSAINode(pNode);
		return pSAINode;
	}
	else
		return NULL;
}

CAnmSAINode *CAnmExecutionContext::GetImportedNode(const char *nodename)
{
#if IMPLEMENT_LATER
	
	CAnmNode *pNode = m_proto->GetImportedNode(nodename);

	if (pNode)
	{
		CAnmSAINode *pSAINode = CreateSAINode(pNode);
		return pSAINode;
	}
	else
		return NULL;

#endif // IMPLEMENT_LATER

	return NULL;
}

CAnmSAINode *CAnmExecutionContext::CreateNode(const char *nodetypename)
{
	CAnmNode *pNode = m_proto->CreateNode(nodetypename);

	if (pNode)
	{
		CAnmSAINode *pSAINode = CreateSAINode(pNode);
		return pSAINode;
	}
	else
		return NULL;

	return NULL;
}

CAnmSAINode *CAnmExecutionContext::CreateProto(const char *prototypename)
{
	CAnmProto *pProto = m_proto->CreateProto(prototypename);

	if (pProto)
	{
		CAnmSAINode *pSAINode = CreateSAINode(pProto);
		return pSAINode;
	}
	else
		return NULL;

	return NULL;
}

CAnmProtoSymbol *CAnmExecutionContext::GetProtoDeclaration(const char *prototypename)
{
	CAnmProtoSymbol *pProtoSym = m_proto->GetProtoSymbol();
	assert(pProtoSym);

	// N.B.: not really type safe; should query this class sym for isProto
	return (CAnmProtoSymbol *) pProtoSym->FindClassSymbol((char *) prototypename);
}

void CAnmExecutionContext::GetProtoDeclarations(int *pNProtoDecls, CAnmProtoSymbol ***pppProtoSymbols)
{
	assert(pNProtoDecls);
	assert(pppProtoSymbols);

	CAnmProtoSymbol *pProtoSym = m_proto->GetProtoSymbol();
	assert(pProtoSym);

	CAnmSymbolTable *pSymbolTable = pProtoSym->GetClassSymbols();
	assert(pSymbolTable);

	std::list<CAnmSymbol *> *pSymbols = pSymbolTable->GetSymbols();
	assert (pSymbols);

	*pNProtoDecls = pSymbols->size();
	*pppProtoSymbols = new CAnmProtoSymbol *[*pNProtoDecls];

	std::list<CAnmSymbol *>::iterator iter;
	int i = 0;

	for (iter = pSymbols->begin(); iter != pSymbols->end(); iter++)
	{
		CAnmClassSymbol *pClassSym = (CAnmClassSymbol *) *iter;

		// N.B.: not really type safe; should query this class sym for isProto
		(*pppProtoSymbols)[i++] = (CAnmProtoSymbol *) pClassSym;
	}

}

std::vector<class CAnmSAINode *> *CAnmExecutionContext::GetRootNodes()
{
	std::vector<class CAnmSAINode *> *pSAINodes = new
		std::vector<class CAnmSAINode *> ;

	NodeArray *pRootNodes = m_proto->GetRootNodes();
	
	int sz = pRootNodes->size();
	for (int i = 0; i < sz; i++)
	{
		CAnmNode *pNode = (*pRootNodes)[i];

		CAnmSAINode *pSAINode = CreateSAINode(pNode);
		pSAINodes->push_back(pSAINode);
	}

	return pSAINodes;
}

CAnmSAIRoute *CAnmExecutionContext::AddRoute(CAnmSAINode *pFromSAINode, 
												const char *fromfield,
												CAnmSAINode *pToSAINode, const char *tofield)
{
	CAnmNode *pFromNode = pFromSAINode->GetNode();
	CAnmNode *pToNode = pToSAINode->GetNode();

	CAnmRoute *pRoute = m_proto->AddRoute(m_app, pFromNode, fromfield, pToNode, tofield);
	if (pRoute)
		return new CAnmSAIRoute(pRoute, pFromSAINode, pToSAINode);
	else
		return NULL;
}

void CAnmExecutionContext::RemoveRoute(CAnmSAIRoute *pSAIRoute)
{
	CAnmRoute *pRoute = pSAIRoute->GetRoute();
	assert(pRoute != NULL);

	m_proto->RemoveRoute(pRoute);
}

std::vector<CAnmSAIRoute *> *CAnmExecutionContext::GetRoutes()
{
	// N.B.: later
	return NULL;
}

CAnmSAINode *CAnmExecutionContext::CreateSAINode(CAnmNode *pNode)
{
	CAnmSAINode *pSAINode = new CAnmSAINode(pNode);
	pSAINode->SetSink(m_sink);

	return pSAINode;
}

DEFINE_ABSTRACT_EMPTY_CLASS(CAnmExecutionContext, CAnmObject)
