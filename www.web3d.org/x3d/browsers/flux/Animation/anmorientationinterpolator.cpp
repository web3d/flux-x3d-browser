/********************************************************************************
 * Flux
 *
 * File: anmorientationinterpolator.cpp
 * Description: Orientation Interpolator class
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
#include "anmorientationinterpolator.h"
#include "anmworld.h"

CAnmOrientationInterpolator::CAnmOrientationInterpolator()
: CAnmNode()
{
}

CAnmOrientationInterpolator::~CAnmOrientationInterpolator()
{
}

// CAnmNode overrides
void CAnmOrientationInterpolator::Realize()
{
	CAnmNode::Realize();
}


void CAnmOrientationInterpolator::SetFraction(Float fraction)
{
	// this is where the interpolation gets done

	Rotation r(0, 1, 0, 0);

	m_interp.Interp(fraction, &r);
	CallCallbacks(CALLBACKID(CAnmOrientationInterpolator, value_changed), &r);

}

void CAnmOrientationInterpolator::SetKey(FloatArray *pKey)
{
	m_interp.SetKey(pKey);
}

void CAnmOrientationInterpolator::SetKeyValue(RotationArray *pKeyValue)
{
	m_interp.SetKeyValue(pKeyValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmOrientationInterpolator, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmOrientationInterpolator, keyValue, eValueRotationArray, RotationArray *, GetKeyValue, SetKeyValue);

DEFINE_METHOD(CAnmOrientationInterpolator, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmOrientationInterpolator, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmOrientationInterpolator, set_keyValue, eValueRotationArray, RotationArray *, SetKeyValue);

DEFINE_CALLBACK(CAnmOrientationInterpolator, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmOrientationInterpolator, keyValue_changed, eValueRotationArray);
DEFINE_CALLBACK(CAnmOrientationInterpolator, value_changed, eValueRotation);


BEGIN_DEFINE_NODE_MEMBERS(CAnmOrientationInterpolator)
VALUEID(CAnmOrientationInterpolator, key),
VALUEID(CAnmOrientationInterpolator, keyValue),
METHODID(CAnmOrientationInterpolator, set_fraction),
METHODID(CAnmOrientationInterpolator, set_key),
METHODID(CAnmOrientationInterpolator, set_keyValue),
CALLBACKID(CAnmOrientationInterpolator, key_changed),
CALLBACKID(CAnmOrientationInterpolator, keyValue_changed),
CALLBACKID(CAnmOrientationInterpolator, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmOrientationInterpolator, CAnmNode);


