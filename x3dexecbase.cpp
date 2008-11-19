/********************************************************************************
 * Flux
 *
 * File: x3dexecbase.cpp
 * Description: Base class for COM execution context
 *								 interfaces
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
#include "Anima.h"
#include "animaapp.h"
#include "x3dexecbase.h"
#include "anmexecutioncontext.h"
#include "anmroute.h"
#include "anmsainode.h"
#include "x3dnode.h"
#include "x3droute.h"
#include "x3dnodearray.h"
#include "x3dprotodeclarationarray.h"

void X3DExecBase::SetContext(CAnmExecutionContext *pContext)
{
	m_context = pContext;
	m_context->Ref();
}

void X3DExecBase::GetContext(CAnmExecutionContext **ppContext)
{
	*ppContext = m_context;
}

X3DExecBase::~X3DExecBase()
{
	SafeUnRef(m_context);
}


STDMETHODIMP X3DExecBase::getSpecificationVersion(BSTR *value)
{
	CComBSTR bstr;

	char *version = m_context->GetSpecificationVersion();

	bstr = version;

	*value = bstr.Copy();

	return S_OK;
}

STDMETHODIMP X3DExecBase::getEncoding(X3DEncodingType *encodingType)
{
	eAnmX3DEncoding enctype = m_context->GetEncoding();
	*encodingType = (X3DEncodingType) enctype;
	return S_OK;
}


STDMETHODIMP X3DExecBase::getNode(BSTR name, X3DNode **value)
{
	char nodename[ANMMAXSTRBUF];
	wcstombs(nodename, name, ANMMAXSTRBUF);

	CAnmSAINode *pNode = m_context->GetNode(nodename);
	if (pNode)
	{
		*value = CX3DNode::CreateX3DNode(pNode, m_x3dcontext);
		return S_OK;
	}
	else
		return S_FALSE;
}

STDMETHODIMP X3DExecBase::createNode(BSTR type, X3DNode **node)
{
	char nodetypename[ANMMAXSTRBUF];

	wcstombs(nodetypename, type, ANMMAXSTRBUF);

	*node = NULL;

	CAnmSAINode *pNode = m_context->CreateNode(nodetypename);
	if (pNode)
	{
		*node = CX3DNode::CreateX3DNode(pNode, m_x3dcontext);
		return S_OK;
	}
	else
		return S_FALSE;
}


STDMETHODIMP X3DExecBase::addRoute(X3DNode *fromNode, BSTR fromEventOut, X3DNode *toNode, BSTR toEventIn, X3DRoute **ppRoute)
{
	CAnmSAINode *pFromNode = NULL, *pToNode = NULL;
	CAnmRoute *pRoute = NULL;
	// N.B.: length
	char fromfield[ANMMAXSTRBUF], tofield[ANMMAXSTRBUF];
	HRESULT hr;

	if (fromNode == NULL || toNode == NULL)
		return S_FALSE;

	hr = fromNode->getNativeNode(&pFromNode);
	if (SUCCEEDED(hr))
	{
		hr = toNode->getNativeNode(&pToNode);
		if (SUCCEEDED(hr))
		{
			wcstombs(fromfield, fromEventOut, wcslen(fromEventOut) + 1);
			wcstombs(tofield, toEventIn, wcslen(toEventIn) + 1);

			CAnmSAIRoute *pSAIRoute = m_context->AddRoute(pFromNode, fromfield, pToNode, tofield);
			if (pSAIRoute)
			{
				*ppRoute = CX3DRoute::CreateX3DRoute(pSAIRoute, m_x3dcontext);
				return S_OK;
			}
			else
				return S_FALSE;
		}

		return S_FALSE;
	}

	return S_FALSE;
}

STDMETHODIMP X3DExecBase::deleteRoute(X3DRoute *pX3DRoute)
{

	CAnmSAIRoute *pSAIRoute = NULL;

	HRESULT hr = pX3DRoute->getClosure((void **) &pSAIRoute);
	if (SUCCEEDED(hr))
	{
		m_context->RemoveRoute(pSAIRoute);
		return S_OK;
	}
	else
		return E_INVALIDARG;
}


STDMETHODIMP X3DExecBase::getRootNodes(X3DNodeArray **rootnodes)
{
	if (rootnodes == NULL)
		return E_INVALIDARG;

	std::vector<class CAnmSAINode *> *pSAIRootNodes = m_context->GetRootNodes();

	*rootnodes = CX3DNodeArray::CreateX3DNodeArray(pSAIRootNodes, m_x3dcontext);

	delete pSAIRootNodes;

	if (*rootnodes == NULL)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP X3DExecBase::getProtoDeclarations(X3DProtoDeclarationArray **protodeclarations)
{
	if (protodeclarations == NULL)
		return E_INVALIDARG;

	int nProtoDecls;
	CAnmProtoSymbol **ppProtoSymbols = NULL;

	m_context->GetProtoDeclarations(&nProtoDecls, &ppProtoSymbols);

	if (nProtoDecls <= 0)
		return true;

	*protodeclarations = CX3DProtoDeclarationArray::Create(nProtoDecls, ppProtoSymbols);

	if (*protodeclarations == NULL)
		return E_FAIL;

	return S_OK;
}