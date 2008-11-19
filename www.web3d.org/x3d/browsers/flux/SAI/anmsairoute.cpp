/********************************************************************************
 * Flux
 *
 * File: anmsairoute.cpp
 * Description: Route wrapper class for SAI
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
#include "anmsairoute.h"
#include "anmroute.h"
#include "anmsainode.h"

CAnmSAIRoute::CAnmSAIRoute(CAnmRoute *pRoute, CAnmSAINode *pFromSAINode, CAnmSAINode *pToSAINode)
{
	m_route = pRoute;
	m_fromNode = pFromSAINode;
	m_toNode = pToSAINode;

	m_route->Ref();
	m_fromNode->Ref();
	m_toNode->Ref();
}

CAnmSAIRoute::~CAnmSAIRoute()
{
	SafeUnRef(m_route);
	SafeUnRef(m_fromNode);
	SafeUnRef(m_toNode);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_ABSTRACT_EMPTY_CLASS(CAnmSAIRoute, CAnmObject)
