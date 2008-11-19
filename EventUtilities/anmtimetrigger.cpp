/********************************************************************************
 * Flux
 *
 * File: anmtimetrigger.cpp
 * Description: TimeTrigger class
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
#include "anmtimetrigger.h"
#include "anmtime.h"
#include "anmworld.h"

CAnmTimeTrigger::CAnmTimeTrigger()
: CAnmNode()
{
}


CAnmTimeTrigger::~CAnmTimeTrigger()
{
}

// CAnmNode overrides
void CAnmTimeTrigger::Realize()
{
	CAnmNode::Realize();
}


void CAnmTimeTrigger::SetBoolean(Boolean b)
{
	CAnmWorld *pWorld = GetWorld();
	if (pWorld == NULL)
		return;

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmTime *pClock = pApp->GetClock();
	assert(pClock);

	Time now = pClock->GetCurrentTime();

	CallCallbacks(CALLBACKID(CAnmTimeTrigger, triggerTime), &now);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_METHOD(CAnmTimeTrigger, set_boolean, eValueBoolean, Boolean, SetBoolean);

DEFINE_CALLBACK(CAnmTimeTrigger, triggerTime, eValueTime);


BEGIN_DEFINE_NODE_MEMBERS(CAnmTimeTrigger)

METHODID(CAnmTimeTrigger, set_boolean),

CALLBACKID(CAnmTimeTrigger, triggerTime),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmTimeTrigger, CAnmNode);


