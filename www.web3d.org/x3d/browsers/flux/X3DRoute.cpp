/********************************************************************************
 * Flux
 *
 * File: X3DRoute.cpp
 * Description: Implementation of CX3DRoute
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

// X3DRoute.cpp : Implementation of CX3DRoute
#include "stdafx.h"
#include "Anima.h"
#include "X3DRoute.h"
#include "X3DNode.h"
#include "anmnode.h"
#include "anmroute.h"
#include "anmsainode.h"
#include "anmsairoute.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DRoute

STDMETHODIMP CX3DRoute::setClosure(void *closure)
{
	m_route = (CAnmSAIRoute *) closure;
	return S_OK;
}

STDMETHODIMP CX3DRoute::getClosure(void **closure)
{
	*((CAnmSAIRoute **) closure) = m_route;
	return S_OK;
}

STDMETHODIMP CX3DRoute::getContext(X3DExecutionContext **result)
{

	*result = m_context;
	return S_OK;
}

STDMETHODIMP CX3DRoute::setContext(X3DExecutionContext *context)
{
	m_context = context;
	return S_OK;
}

STDMETHODIMP CX3DRoute::getSourceNode(X3DNode **pSourceNode)
{
	assert(m_route != NULL);

	if (pSourceNode == NULL)
		return E_INVALIDARG;

	*pSourceNode = CX3DNode::CreateX3DNode(m_route->GetFromNode(), m_context);

	return S_OK;
}

STDMETHODIMP CX3DRoute::getSourceField(BSTR *sourcefield)
{
	assert(m_route != NULL);

	CLASSMEMBERID frommid = m_route->GetRoute()->GetFromCallback();

	assert(frommid != NULL);

	CComBSTR bstr = frommid->name;

	*sourcefield = bstr.Copy();
	return S_OK;
}

STDMETHODIMP CX3DRoute::getDestinationNode(X3DNode **pDestinationNode)
{
	assert(m_route != NULL);

	if (pDestinationNode == NULL)
		return E_INVALIDARG;

	*pDestinationNode = CX3DNode::CreateX3DNode(m_route->GetToNode(), m_context);

	return S_OK;
}

STDMETHODIMP CX3DRoute::getDestinationField(BSTR *destinationfield)
{
	assert(m_route != NULL);

	CLASSMEMBERID frommid = m_route->GetRoute()->GetToMethod();

	assert(frommid != NULL);

	CComBSTR bstr = frommid->name;

	*destinationfield = bstr.Copy();
	return S_OK;
}

X3DRoute *CX3DRoute::CreateX3DRoute(CAnmSAIRoute *pSAIRoute, X3DExecutionContext *context)
{
	assert(pSAIRoute);

	X3DRoute *pX3DRoute = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CoX3DRoute, NULL, CLSCTX_INPROC_SERVER, 
		IID_X3DRoute, (LPVOID*)&pX3DRoute);

	if (FAILED(hr))
		return NULL;

	pX3DRoute->setClosure(pSAIRoute);
	pX3DRoute->setContext(context);

	return pX3DRoute;
}
