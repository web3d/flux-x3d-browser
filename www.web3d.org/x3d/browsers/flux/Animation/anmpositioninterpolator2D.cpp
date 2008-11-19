/********************************************************************************
 * Flux
 *
 * File: anmpositioninterpolator2D.cpp
 * Description: 2D Position Interpolator class
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
#include "anmpositioninterpolator2D.h"


CAnmPositionInterpolator2D::CAnmPositionInterpolator2D()
: CAnmNode()
{
}


CAnmPositionInterpolator2D::~CAnmPositionInterpolator2D()
{
}

// CAnmNode overrides
void CAnmPositionInterpolator2D::Realize()
{
	CAnmNode::Realize();
}



void CAnmPositionInterpolator2D::SetFraction(Float fraction)
{
	// this is where the interpolation gets done
	Point2 p(0, 0);

	m_interp.Interp(fraction, &p);
	CallCallbacks(CALLBACKID(CAnmPositionInterpolator2D, value_changed), &p);

}

void CAnmPositionInterpolator2D::SetKey(FloatArray *pKey)
{
	m_interp.SetKey(pKey);
}

void CAnmPositionInterpolator2D::SetKeyValue(Point2Array *pKeyValue)
{
	m_interp.SetKeyValue(pKeyValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmPositionInterpolator2D, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmPositionInterpolator2D, keyValue, eValuePoint2Array, Point2Array *, GetKeyValue, SetKeyValue);

DEFINE_METHOD(CAnmPositionInterpolator2D, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmPositionInterpolator2D, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmPositionInterpolator2D, set_keyValue, eValuePoint2Array, Point2Array *, SetKeyValue);

DEFINE_CALLBACK(CAnmPositionInterpolator2D, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmPositionInterpolator2D, keyValue_changed, eValuePoint2Array);
DEFINE_CALLBACK(CAnmPositionInterpolator2D, value_changed, eValuePoint2);


BEGIN_DEFINE_NODE_MEMBERS(CAnmPositionInterpolator2D)
VALUEID(CAnmPositionInterpolator2D, key),
VALUEID(CAnmPositionInterpolator2D, keyValue),
METHODID(CAnmPositionInterpolator2D, set_fraction),
METHODID(CAnmPositionInterpolator2D, set_key),
METHODID(CAnmPositionInterpolator2D, set_keyValue),
CALLBACKID(CAnmPositionInterpolator2D, key_changed),
CALLBACKID(CAnmPositionInterpolator2D, keyValue_changed),
CALLBACKID(CAnmPositionInterpolator2D, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmPositionInterpolator2D, CAnmNode);


