/********************************************************************************
 * Flux
 *
 * File: anmbooleansequencer.cpp
 * Description: BooleanSequencer class
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
#include "anmbooleansequencer.h"


CAnmBooleanSequencer::CAnmBooleanSequencer()
: CAnmNode()
{
}


CAnmBooleanSequencer::~CAnmBooleanSequencer()
{
}

// CAnmNode overrides
void CAnmBooleanSequencer::Realize()
{
	CAnmNode::Realize();
}



void CAnmBooleanSequencer::SetFraction(Float fraction)
{
	// this is where the interpolation gets done
	Boolean b = false;

	m_sequencer.Evaluate(fraction, &b);
	CallCallbacks(CALLBACKID(CAnmBooleanSequencer, value_changed), &b);

}

void CAnmBooleanSequencer::SetNext(Boolean bDummy)
{
	Boolean b = false;
	if( m_sequencer.Next(&b) ) {
		CallCallbacks(CALLBACKID(CAnmBooleanSequencer, value_changed), &b);
	}
}

void CAnmBooleanSequencer::SetPrevious(Boolean bDummy)
{
	Boolean b = false;
	if( m_sequencer.Previous(&b) ) {
		CallCallbacks(CALLBACKID(CAnmBooleanSequencer, value_changed), &b);
	}
}



void CAnmBooleanSequencer::SetKey(FloatArray *pKey)
{
	m_sequencer.SetKey(pKey);
}

void CAnmBooleanSequencer::SetKeyValue(BooleanArray *pKeyValue)
{
	m_sequencer.SetKeyValue(pKeyValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmBooleanSequencer, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmBooleanSequencer, keyValue, eValueBooleanArray, BooleanArray *, GetKeyValue, SetKeyValue);

DEFINE_METHOD(CAnmBooleanSequencer, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmBooleanSequencer, next, eValueBoolean, Boolean, SetNext);
DEFINE_METHOD(CAnmBooleanSequencer, previous, eValueBoolean, Boolean, SetPrevious);
DEFINE_METHOD(CAnmBooleanSequencer, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmBooleanSequencer, set_keyValue, eValueBooleanArray, BooleanArray *, SetKeyValue);

DEFINE_CALLBACK(CAnmBooleanSequencer, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmBooleanSequencer, keyValue_changed, eValueBooleanArray);
DEFINE_CALLBACK(CAnmBooleanSequencer, value_changed, eValueBoolean);


BEGIN_DEFINE_NODE_MEMBERS(CAnmBooleanSequencer)
VALUEID(CAnmBooleanSequencer, key),
VALUEID(CAnmBooleanSequencer, keyValue),
METHODID(CAnmBooleanSequencer, set_fraction),
METHODID(CAnmBooleanSequencer, next),
METHODID(CAnmBooleanSequencer, previous),
METHODID(CAnmBooleanSequencer, set_key),
METHODID(CAnmBooleanSequencer, set_keyValue),
CALLBACKID(CAnmBooleanSequencer, key_changed),
CALLBACKID(CAnmBooleanSequencer, keyValue_changed),
CALLBACKID(CAnmBooleanSequencer, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmBooleanSequencer, CAnmNode);


