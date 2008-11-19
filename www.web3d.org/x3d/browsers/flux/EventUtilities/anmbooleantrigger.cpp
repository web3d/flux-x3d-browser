/********************************************************************************
 * Flux
 *
 * File: anmbooleantrigger.cpp
 * Description: BooleanTrigger class
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
#include "anmbooleantrigger.h"

CAnmBooleanTrigger::CAnmBooleanTrigger()
: CAnmNode()
{
}


CAnmBooleanTrigger::~CAnmBooleanTrigger()
{
}

// CAnmNode overrides
void CAnmBooleanTrigger::Realize()
{
	CAnmNode::Realize();
}


void CAnmBooleanTrigger::SetTriggerTime(Time t)
{
	Boolean b = true;

	CallCallbacks(CALLBACKID(CAnmBooleanTrigger, triggerTrue), &b);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_METHOD(CAnmBooleanTrigger, set_triggerTime, eValueTime, Time, SetTriggerTime);

DEFINE_CALLBACK(CAnmBooleanTrigger, triggerTrue, eValueBoolean);

BEGIN_DEFINE_NODE_MEMBERS(CAnmBooleanTrigger)

METHODID(CAnmBooleanTrigger, set_triggerTime),

CALLBACKID(CAnmBooleanTrigger, triggerTrue),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmBooleanTrigger, CAnmNode);


