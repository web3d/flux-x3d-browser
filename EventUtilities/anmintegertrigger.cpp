/********************************************************************************
 * Flux
 *
 * File: anmintegertrigger.cpp
 * Description: IntegerTrigger class
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
#include "anmintegertrigger.h"


CAnmIntegerTrigger::CAnmIntegerTrigger()
: CAnmNode()
{
	m_integerKey = ANMINTEGERTRIGGER_DEFAULTINTEGERKEY;
}


CAnmIntegerTrigger::~CAnmIntegerTrigger()
{
}

// CAnmNode overrides
void CAnmIntegerTrigger::Realize()
{
	CAnmNode::Realize();
}



void CAnmIntegerTrigger::SetBoolean(Boolean b)
{
	CallCallbacks(CALLBACKID(CAnmIntegerTrigger, triggerValue), &m_integerKey);
}

void CAnmIntegerTrigger::SetIntegerKey(Integer keyval)
{
	m_integerKey = keyval;
	CallCallbacks(CALLBACKID(CAnmIntegerTrigger, integerKey_changed), &keyval);	
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmIntegerTrigger, integerKey, eValueInteger, Integer, GetIntegerKey, SetIntegerKey);

DEFINE_METHOD(CAnmIntegerTrigger, set_boolean, eValueBoolean, Boolean, SetBoolean);
DEFINE_METHOD(CAnmIntegerTrigger, set_integerKey, eValueInteger, Integer, SetIntegerKey);

DEFINE_CALLBACK(CAnmIntegerTrigger, integerKey_changed, eValueInteger);
DEFINE_CALLBACK(CAnmIntegerTrigger, triggerValue, eValueInteger);


BEGIN_DEFINE_NODE_MEMBERS(CAnmIntegerTrigger)
VALUEID(CAnmIntegerTrigger, integerKey),

METHODID(CAnmIntegerTrigger, set_boolean),
METHODID(CAnmIntegerTrigger, set_integerKey),

CALLBACKID(CAnmIntegerTrigger, integerKey_changed),
CALLBACKID(CAnmIntegerTrigger, triggerValue),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmIntegerTrigger, CAnmNode);


