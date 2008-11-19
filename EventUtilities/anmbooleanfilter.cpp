/********************************************************************************
 * Flux
 *
 * File: anmbooleanfilter.cpp
 * Description: BooleanFilter class
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
#include "anmbooleanfilter.h"

CAnmBooleanFilter::CAnmBooleanFilter()
: CAnmNode()
{
}


CAnmBooleanFilter::~CAnmBooleanFilter()
{
}

// CAnmNode overrides
void CAnmBooleanFilter::Realize()
{
	CAnmNode::Realize();
}

void CAnmBooleanFilter::SetBoolean(Boolean b)
{
	Boolean negate = !b;

	CLASSMEMBERID cbid = b ? CALLBACKID(CAnmBooleanFilter, inputTrue) :
		CALLBACKID(CAnmBooleanFilter, inputFalse);

	CallCallbacks(cbid, &b);
	CallCallbacks(CALLBACKID(CAnmBooleanFilter, inputNegate), &negate);
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_METHOD(CAnmBooleanFilter, set_boolean, eValueBoolean, Boolean, SetBoolean);

DEFINE_CALLBACK(CAnmBooleanFilter, inputFalse, eValueBoolean);
DEFINE_CALLBACK(CAnmBooleanFilter, inputNegate, eValueBoolean);
DEFINE_CALLBACK(CAnmBooleanFilter, inputTrue, eValueBoolean);

BEGIN_DEFINE_NODE_MEMBERS(CAnmBooleanFilter)

METHODID(CAnmBooleanFilter, set_boolean),

CALLBACKID(CAnmBooleanFilter, inputFalse),
CALLBACKID(CAnmBooleanFilter, inputNegate),
CALLBACKID(CAnmBooleanFilter, inputTrue),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmBooleanFilter, CAnmNode);


