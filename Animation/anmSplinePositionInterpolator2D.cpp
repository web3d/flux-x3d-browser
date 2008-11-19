/********************************************************************************
 * Flux
 *
 * File: anmpositioninterpolator2D.cpp
 * Description: Position Interpolator2D class
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
#include "anmsplinepositioninterpolator2D.h"


CAnmSplinePositionInterpolator2D::CAnmSplinePositionInterpolator2D() :
		m_interp( Point2(0, 0) ),
		CAnmNode( )
{
}


CAnmSplinePositionInterpolator2D::~CAnmSplinePositionInterpolator2D()
{
}

// CAnmNode overrides
void CAnmSplinePositionInterpolator2D::Realize()
{
	CAnmNode::Realize();
}



void CAnmSplinePositionInterpolator2D::SetFraction(Float fraction)
{
	// this is where the interpolation gets done
	Point2 p(0, 0);
	m_interp.SplineInterp(fraction, &p);
	CallCallbacks(CALLBACKID(CAnmSplinePositionInterpolator2D, value_changed), &p);

}

void CAnmSplinePositionInterpolator2D::SetKey(FloatArray *pKey)
{
	m_interp.SetKey(pKey);
}
void CAnmSplinePositionInterpolator2D::SetKeyValue(Point2Array *pKeyValue)
{
	m_interp.SetKeyValue(pKeyValue);
}
void CAnmSplinePositionInterpolator2D::SetKeyVelocity(Point2Array *pKeyVelocity)
{
	m_interp.SetKeyVelocity(pKeyVelocity);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmSplinePositionInterpolator2D, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmSplinePositionInterpolator2D, keyValue, eValuePoint2Array, Point2Array *, GetKeyValue, SetKeyValue);
DEFINE_VALUE(CAnmSplinePositionInterpolator2D, closed, eValueBoolean, Boolean, GetClosed, SetClosed);
DEFINE_VALUE(CAnmSplinePositionInterpolator2D, normalizeVelocity, eValueBoolean, Boolean, GetNormalizeVelocity, SetNormalizeVelocity);
DEFINE_VALUE(CAnmSplinePositionInterpolator2D, keyVelocity, eValuePoint2Array, Point2Array *, GetKeyVelocity, SetKeyVelocity);

DEFINE_METHOD(CAnmSplinePositionInterpolator2D, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmSplinePositionInterpolator2D, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmSplinePositionInterpolator2D, set_keyValue, eValuePoint2Array, Point2Array *, SetKeyValue);
DEFINE_METHOD(CAnmSplinePositionInterpolator2D, set_closed, eValueBoolean, Boolean, SetClosed);
DEFINE_METHOD(CAnmSplinePositionInterpolator2D, set_normalizeVelocity, eValueBoolean, Boolean, SetNormalizeVelocity);
DEFINE_METHOD(CAnmSplinePositionInterpolator2D, set_keyVelocity, eValuePoint2Array, Point2Array *, SetKeyVelocity);

DEFINE_CALLBACK(CAnmSplinePositionInterpolator2D, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmSplinePositionInterpolator2D, keyValue_changed, eValuePoint2Array);
DEFINE_CALLBACK(CAnmSplinePositionInterpolator2D, closed_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmSplinePositionInterpolator2D, normalizeVelocity_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmSplinePositionInterpolator2D, keyVelocity_changed, eValuePoint2Array);
DEFINE_CALLBACK(CAnmSplinePositionInterpolator2D, value_changed, eValuePoint2);


BEGIN_DEFINE_NODE_MEMBERS(CAnmSplinePositionInterpolator2D)
VALUEID(CAnmSplinePositionInterpolator2D, key),
VALUEID(CAnmSplinePositionInterpolator2D, keyValue),
VALUEID(CAnmSplinePositionInterpolator2D, closed),
VALUEID(CAnmSplinePositionInterpolator2D, normalizeVelocity),
VALUEID(CAnmSplinePositionInterpolator2D, keyVelocity),
METHODID(CAnmSplinePositionInterpolator2D, set_fraction),
METHODID(CAnmSplinePositionInterpolator2D, set_key),
METHODID(CAnmSplinePositionInterpolator2D, set_keyValue),
METHODID(CAnmSplinePositionInterpolator2D, set_closed),
METHODID(CAnmSplinePositionInterpolator2D, set_normalizeVelocity),
METHODID(CAnmSplinePositionInterpolator2D, set_keyVelocity),
CALLBACKID(CAnmSplinePositionInterpolator2D, key_changed),
CALLBACKID(CAnmSplinePositionInterpolator2D, keyValue_changed),
CALLBACKID(CAnmSplinePositionInterpolator2D, closed_changed),
CALLBACKID(CAnmSplinePositionInterpolator2D, normalizeVelocity_changed),
CALLBACKID(CAnmSplinePositionInterpolator2D, keyVelocity_changed),
CALLBACKID(CAnmSplinePositionInterpolator2D, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmSplinePositionInterpolator2D, CAnmNode);


