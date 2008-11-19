/********************************************************************************
 * Flux
 *
 * File: anmintegersequencer.cpp
 * Description: IntegerSequencer class
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
#include "anmintegersequencer.h"


CAnmIntegerSequencer::CAnmIntegerSequencer()
: CAnmNode()
{
}


CAnmIntegerSequencer::~CAnmIntegerSequencer()
{
}

// CAnmNode overrides
void CAnmIntegerSequencer::Realize()
{
	CAnmNode::Realize();
}



void CAnmIntegerSequencer::SetFraction(Float fraction)
{
	// this is where the interpolation gets done
	Integer ival = 0;

	m_sequencer.Evaluate(fraction, &ival);
	CallCallbacks(CALLBACKID(CAnmIntegerSequencer, value_changed), &ival);
}


void CAnmIntegerSequencer::SetNext(Boolean b)
{
	Integer ival = 0;
	if( m_sequencer.Next(&ival) ) {
		CallCallbacks(CALLBACKID(CAnmIntegerSequencer, value_changed), &ival);
	}
}

void CAnmIntegerSequencer::SetPrevious(Boolean b)
{
	Integer ival = 0;
	if( m_sequencer.Previous(&ival) ) {
		CallCallbacks(CALLBACKID(CAnmIntegerSequencer, value_changed), &ival);
	}
}


void CAnmIntegerSequencer::SetKey(FloatArray *pKey)
{
	m_sequencer.SetKey(pKey);
}

void CAnmIntegerSequencer::SetKeyValue(IntegerArray *pKeyValue)
{
	m_sequencer.SetKeyValue(pKeyValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmIntegerSequencer, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmIntegerSequencer, keyValue, eValueIntegerArray, IntegerArray *, GetKeyValue, SetKeyValue);

DEFINE_METHOD(CAnmIntegerSequencer, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmIntegerSequencer, next, eValueBoolean, Boolean, SetNext);
DEFINE_METHOD(CAnmIntegerSequencer, previous, eValueBoolean, Boolean, SetPrevious);
DEFINE_METHOD(CAnmIntegerSequencer, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmIntegerSequencer, set_keyValue, eValueIntegerArray, IntegerArray *, SetKeyValue);

DEFINE_CALLBACK(CAnmIntegerSequencer, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmIntegerSequencer, keyValue_changed, eValueIntegerArray);
DEFINE_CALLBACK(CAnmIntegerSequencer, value_changed, eValueInteger);


BEGIN_DEFINE_NODE_MEMBERS(CAnmIntegerSequencer)
VALUEID(CAnmIntegerSequencer, key),
VALUEID(CAnmIntegerSequencer, keyValue),
METHODID(CAnmIntegerSequencer, set_fraction),
METHODID(CAnmIntegerSequencer, next),
METHODID(CAnmIntegerSequencer, previous),
METHODID(CAnmIntegerSequencer, set_key),
METHODID(CAnmIntegerSequencer, set_keyValue),
CALLBACKID(CAnmIntegerSequencer, key_changed),
CALLBACKID(CAnmIntegerSequencer, keyValue_changed),
CALLBACKID(CAnmIntegerSequencer, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmIntegerSequencer, CAnmNode);


