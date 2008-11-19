/********************************************************************************
 * Flux
 *
 * File: anmnormalinterpolator.cpp
 * Description: Normal Interpolator class
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
#include "anmnormalinterpolator.h"
#include "anmworld.h"

CAnmNormalInterpolator::CAnmNormalInterpolator()
: CAnmNode()
{
}

CAnmNormalInterpolator::~CAnmNormalInterpolator()
{
}

// CAnmNode overrides
void CAnmNormalInterpolator::Realize()
{
	CAnmNode::Realize();
}


void CAnmNormalInterpolator::SetFraction(Float fraction)
{
	// this is where the interpolation gets done

	Point3 p(0, 1, 0);

	m_interp.NormalInterp(fraction, &p);
	CallCallbacks(CALLBACKID(CAnmNormalInterpolator, value_changed), &p);

}

void CAnmNormalInterpolator::SetKey(FloatArray *pKey)
{
	m_interp.SetKey(pKey);
}

void CAnmNormalInterpolator::SetKeyValue(Point3Array *pKeyValue)
{
	m_interp.SetKeyValue(pKeyValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmNormalInterpolator, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmNormalInterpolator, keyValue, eValuePoint3Array, Point3Array *, GetKeyValue, SetKeyValue);

DEFINE_METHOD(CAnmNormalInterpolator, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmNormalInterpolator, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmNormalInterpolator, set_keyValue, eValuePoint3Array, Point3Array *, SetKeyValue);

DEFINE_CALLBACK(CAnmNormalInterpolator, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmNormalInterpolator, keyValue_changed, eValuePoint3Array);
DEFINE_CALLBACK(CAnmNormalInterpolator, value_changed, eValuePoint3);


BEGIN_DEFINE_NODE_MEMBERS(CAnmNormalInterpolator)
VALUEID(CAnmNormalInterpolator, key),
VALUEID(CAnmNormalInterpolator, keyValue),
METHODID(CAnmNormalInterpolator, set_fraction),
METHODID(CAnmNormalInterpolator, set_key),
METHODID(CAnmNormalInterpolator, set_keyValue),
CALLBACKID(CAnmNormalInterpolator, key_changed),
CALLBACKID(CAnmNormalInterpolator, keyValue_changed),
CALLBACKID(CAnmNormalInterpolator, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmNormalInterpolator, CAnmNode);


