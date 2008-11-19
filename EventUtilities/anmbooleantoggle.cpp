/********************************************************************************
 * Flux
 *
 * File: anmbooleantoggle.cpp
 * Description: BooleanToggle class
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
#include "anmbooleantoggle.h"


CAnmBooleanToggle::CAnmBooleanToggle()
: CAnmNode()
{
	m_toggle = ANMBOOLEANTOGGLE_DEFAULTTOGGLE;
}


CAnmBooleanToggle::~CAnmBooleanToggle()
{
}

// CAnmNode overrides
void CAnmBooleanToggle::Realize()
{
	CAnmNode::Realize();
}


void CAnmBooleanToggle::SetBoolean(Boolean b)
{
	if (b)
	{
		m_toggle = !m_toggle;
		CallCallbacks(CALLBACKID(CAnmBooleanToggle, toggle_changed), &m_toggle);
	}
}

void CAnmBooleanToggle::SetToggle(Boolean toggle)
{
	m_toggle = toggle;
	CallCallbacks(CALLBACKID(CAnmBooleanToggle, toggle_changed), &toggle);	
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmBooleanToggle, toggle, eValueBoolean, Boolean, GetToggle, SetToggle);

DEFINE_METHOD(CAnmBooleanToggle, set_boolean, eValueBoolean, Boolean, SetBoolean);
DEFINE_METHOD(CAnmBooleanToggle, set_toggle, eValueBoolean, Boolean, SetToggle);

DEFINE_CALLBACK(CAnmBooleanToggle, toggle_changed, eValueBoolean);


BEGIN_DEFINE_NODE_MEMBERS(CAnmBooleanToggle)
VALUEID(CAnmBooleanToggle, toggle),

METHODID(CAnmBooleanToggle, set_boolean),
METHODID(CAnmBooleanToggle, set_toggle),

CALLBACKID(CAnmBooleanToggle, toggle_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmBooleanToggle, CAnmNode);


