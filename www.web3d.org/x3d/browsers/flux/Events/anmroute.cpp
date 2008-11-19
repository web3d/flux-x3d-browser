/********************************************************************************
 * Flux
 *
 * File: anmroute.cpp
 * Description: Route class
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
#include "anmroute.h"
#include "anmevent.h"

static void routeCallbackFunc(CAnmNode *pNode, CLASSMEMBERID reason, void *pData, void *pClosure)
{
	CAnmRoute *pRoute = (CAnmRoute *) pClosure;

	pRoute->Fire(pData);
}

CAnmRoute::CAnmRoute(cApplication *pApp,
					 CAnmNode				*pFromNode,
					 CLASSMEMBERID				 fromCallback,
					 CAnmNode					*pToNode,
					 CLASSMEMBERID				 toMethod,
					 Boolean						 eventFlag)
:
m_app(pApp),
m_fromNode(pFromNode),
m_fromCallback(fromCallback),
m_toNode(pToNode),
m_toMethod(toMethod),
m_eventFlag(eventFlag)
{
	// make sure in/out types are compatible. for now that means equal
	assert(fromCallback->valueType == toMethod->valueType);
	
	m_callbackPtr = 
		m_fromNode->AddCallback(fromCallback, (CAnmCallbackFunction) routeCallbackFunc, this);

	assert(m_callbackPtr != NULL);

	if (eventFlag)
	{
		m_app->AddEventReceiver(pToNode);
		m_event = new CAnmEvent(pToNode, toMethod);
	}
	else
		m_event = NULL;

	m_fromNode->Ref();
	m_toNode->Ref();
}

CAnmRoute::~CAnmRoute()
{
	// N.B.: don't want me getting called back at a time like this... it could happen -- TP
	m_fromNode->RemoveCallback(m_fromCallback, m_callbackPtr);
	m_callbackPtr = NULL;	// this thing's dead

	m_fromNode->UnRef();
	m_toNode->UnRef();

	SafeDelete(m_event);
}

void CAnmRoute::Fire(void *pData)
{

	if (m_eventFlag)
	{
		// m_toNode->PostEvent(m_toNode, m_toMethod, pData);
		m_event->SetData(pData);
		m_toNode->PostEvent(m_event);
	}
	else
	{
		m_toNode->CallMethod(m_toMethod, pData);
	}
}


void CAnmRoute::SetFromNode(CAnmNode *pFromNode)
{
	m_fromNode = pFromNode;
}

void CAnmRoute::SetFromCallback(CLASSMEMBERID fromCallback)
{
	m_fromCallback = fromCallback;
}


void CAnmRoute::SetToNode(CAnmNode *pToNode)
{
	m_toNode = pToNode;
}


void CAnmRoute::SetToMethod(CLASSMEMBERID toMethod)
{
	m_toMethod = toMethod;
}


