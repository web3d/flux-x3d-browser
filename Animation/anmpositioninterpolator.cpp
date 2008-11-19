/********************************************************************************
 * Flux
 *
 * File: anmpositioninterpolator.cpp
 * Description: Position Interpolator class
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
#include "anmpositioninterpolator.h"


CAnmPositionInterpolator::CAnmPositionInterpolator()
: CAnmNode()
{
}


CAnmPositionInterpolator::~CAnmPositionInterpolator()
{
}

// CAnmNode overrides
void CAnmPositionInterpolator::Realize()
{
	CAnmNode::Realize();
}



void CAnmPositionInterpolator::SetFraction(Float fraction)
{
	// this is where the interpolation gets done
	Point3 p(0, 0, 0);

	m_interp.Interp(fraction, &p);
	CallCallbacks(CALLBACKID(CAnmPositionInterpolator, value_changed), &p);

}

void CAnmPositionInterpolator::SetKey(FloatArray *pKey)
{
	m_interp.SetKey(pKey);
}

void CAnmPositionInterpolator::SetKeyValue(Point3Array *pKeyValue)
{
	m_interp.SetKeyValue(pKeyValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmPositionInterpolator, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmPositionInterpolator, keyValue, eValuePoint3Array, Point3Array *, GetKeyValue, SetKeyValue);

DEFINE_METHOD(CAnmPositionInterpolator, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmPositionInterpolator, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmPositionInterpolator, set_keyValue, eValuePoint3Array, Point3Array *, SetKeyValue);

DEFINE_CALLBACK(CAnmPositionInterpolator, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmPositionInterpolator, keyValue_changed, eValuePoint3Array);
DEFINE_CALLBACK(CAnmPositionInterpolator, value_changed, eValuePoint3);


BEGIN_DEFINE_NODE_MEMBERS(CAnmPositionInterpolator)
VALUEID(CAnmPositionInterpolator, key),
VALUEID(CAnmPositionInterpolator, keyValue),
METHODID(CAnmPositionInterpolator, set_fraction),
METHODID(CAnmPositionInterpolator, set_key),
METHODID(CAnmPositionInterpolator, set_keyValue),
CALLBACKID(CAnmPositionInterpolator, key_changed),
CALLBACKID(CAnmPositionInterpolator, keyValue_changed),
CALLBACKID(CAnmPositionInterpolator, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmPositionInterpolator, CAnmNode);


