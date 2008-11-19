/********************************************************************************
 * Flux
 *
 * File: anmbindablenode.cpp
 * Description: Bindable node abstract class
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
#include "anmbindablenode.h"
#include "anmlayer.h"
#include "anmworld.h"

CAnmBindableNode::CAnmBindableNode() : 
	CAnmNode()
{
	m_isBound = FALSE;
}

CAnmBindableNode::~CAnmBindableNode()
{
	if (m_isBound)
	{
		SetBind(FALSE);
	}
}

void CAnmBindableNode::Realize()
{
	CAnmLayer *pLayer = GetLayer();
	if (pLayer != NULL)
	{
		CAnmBindableNode *pBoundNode = pLayer->GetBoundNode(GetClass());
		if (!pBoundNode)
			SetBind(true);
	}
	
	CAnmNode::Realize();
}


void CAnmBindableNode::SetBind(Boolean bindFlag)
{
	if (m_isBound == bindFlag)
		return;

	CAnmLayer *pLayer = GetLayer();
	if (pLayer != NULL)
	{
		if (bindFlag)
			pLayer->BindNode(this, GetClass());
		else
			pLayer->TryUnbindNode(this, GetClass());
	}

	if (m_isBound != bindFlag)
	{
		m_isBound = bindFlag;

		// generate our isBound and bindTime events
		CallCallbacks(CALLBACKID(CAnmBindableNode, isBound), &bindFlag);

		if (m_isBound)
		{
			CAnmWorld *pWorld = GetWorld();
//			assert(pWorld);
			ABORT_IF_WORLD_IS_NULL

			cApplication *pApp = pWorld->GetApp();
			assert(pApp);

			CAnmTime *pClock = pApp->GetClock();
			assert(pClock);

			Time now = pClock->GetCurrentTime();

			CallCallbacks(CALLBACKID(CAnmBindableNode, bindTime), &now);
		}

		SetStateDirty();
	}

}

void CAnmBindableNode::NotifyBound(Boolean bindFlag)
{
	if (m_isBound == bindFlag)
		return;

	m_isBound = bindFlag;

	// generate our isBound and bindTime events
	CallCallbacks(CALLBACKID(CAnmBindableNode, isBound), &bindFlag);

	if (m_isBound)
	{
		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL

		cApplication *pApp = pWorld->GetApp();
		assert(pApp);

		CAnmTime *pClock = pApp->GetClock();
		assert(pClock);

		Time now = pClock->GetCurrentTime();

		CallCallbacks(CALLBACKID(CAnmBindableNode, bindTime), &now);
	}

	SetStateDirty();
}


// Accessors


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_METHOD(CAnmBindableNode, set_bind, eValueBoolean, Boolean, SetBind);

DEFINE_CALLBACK(CAnmBindableNode, bindTime, eValueTime);
DEFINE_CALLBACK(CAnmBindableNode, isBound, eValueBoolean);

// member table
BEGIN_DEFINE_MEMBERS(CAnmBindableNode)
METHODID(CAnmBindableNode, set_bind),

CALLBACKID(CAnmBindableNode, bindTime),
CALLBACKID(CAnmBindableNode, isBound),

END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmBindableNode, CAnmNode);

