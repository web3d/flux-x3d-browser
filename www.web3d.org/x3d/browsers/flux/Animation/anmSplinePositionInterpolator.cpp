/********************************************************************************
 * Flux
 *
 * File: anmsplinepositioninterpolator.cpp
 * Description: 
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
#include "anmsplinepositioninterpolator.h"


CAnmSplinePositionInterpolator::CAnmSplinePositionInterpolator() :
		m_interp( Point3(0, 0, 0) ),
		CAnmNode( )
{
}


CAnmSplinePositionInterpolator::~CAnmSplinePositionInterpolator()
{
}

// CAnmNode overrides
void CAnmSplinePositionInterpolator::Realize()
{
	CAnmNode::Realize();
}



void CAnmSplinePositionInterpolator::SetFraction(Float fraction)
{
	// this is where the interpolation gets done
	Point3 p(0, 0, 0);

	m_interp.SplineInterp(fraction, &p);
	CallCallbacks(CALLBACKID(CAnmSplinePositionInterpolator, value_changed), &p);

}

void CAnmSplinePositionInterpolator::SetKey(FloatArray *pKey)
{
	m_interp.SetKey(pKey);
}

void CAnmSplinePositionInterpolator::SetKeyValue(Point3Array *pKeyValue)
{
	m_interp.SetKeyValue(pKeyValue);
}
void CAnmSplinePositionInterpolator::SetKeyVelocity(Point3Array *pKeyVelocity)
{
	m_interp.SetKeyVelocity(pKeyVelocity);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmSplinePositionInterpolator, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmSplinePositionInterpolator, keyValue, eValuePoint3Array, Point3Array *, GetKeyValue, SetKeyValue);
DEFINE_VALUE(CAnmSplinePositionInterpolator, closed, eValueBoolean, Boolean, GetClosed, SetClosed);
DEFINE_VALUE(CAnmSplinePositionInterpolator, normalizeVelocity, eValueBoolean, Boolean, GetNormalizeVelocity, SetNormalizeVelocity);
DEFINE_VALUE(CAnmSplinePositionInterpolator, keyVelocity, eValuePoint3Array, Point3Array *, GetKeyVelocity, SetKeyVelocity);

DEFINE_METHOD(CAnmSplinePositionInterpolator, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmSplinePositionInterpolator, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmSplinePositionInterpolator, set_keyValue, eValuePoint3Array, Point3Array *, SetKeyValue);
DEFINE_METHOD(CAnmSplinePositionInterpolator, set_closed, eValueBoolean, Boolean, SetClosed);
DEFINE_METHOD(CAnmSplinePositionInterpolator, set_normalizeVelocity, eValueBoolean, Boolean, SetNormalizeVelocity);
DEFINE_METHOD(CAnmSplinePositionInterpolator, set_keyVelocity, eValuePoint3Array, Point3Array *, SetKeyVelocity);

DEFINE_CALLBACK(CAnmSplinePositionInterpolator, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmSplinePositionInterpolator, keyValue_changed, eValuePoint3Array);
DEFINE_CALLBACK(CAnmSplinePositionInterpolator, closed_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmSplinePositionInterpolator, normalizeVelocity_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmSplinePositionInterpolator, keyVelocity_changed, eValuePoint3Array);
DEFINE_CALLBACK(CAnmSplinePositionInterpolator, value_changed, eValuePoint3);


BEGIN_DEFINE_NODE_MEMBERS(CAnmSplinePositionInterpolator)
VALUEID(CAnmSplinePositionInterpolator, key),
VALUEID(CAnmSplinePositionInterpolator, keyValue),
VALUEID(CAnmSplinePositionInterpolator, closed),
VALUEID(CAnmSplinePositionInterpolator, normalizeVelocity),
VALUEID(CAnmSplinePositionInterpolator, keyVelocity),
METHODID(CAnmSplinePositionInterpolator, set_fraction),
METHODID(CAnmSplinePositionInterpolator, set_key),
METHODID(CAnmSplinePositionInterpolator, set_keyValue),
METHODID(CAnmSplinePositionInterpolator, set_closed),
METHODID(CAnmSplinePositionInterpolator, set_normalizeVelocity),
METHODID(CAnmSplinePositionInterpolator, set_keyVelocity),
CALLBACKID(CAnmSplinePositionInterpolator, key_changed),
CALLBACKID(CAnmSplinePositionInterpolator, keyValue_changed),
CALLBACKID(CAnmSplinePositionInterpolator, closed_changed),
CALLBACKID(CAnmSplinePositionInterpolator, normalizeVelocity_changed),
CALLBACKID(CAnmSplinePositionInterpolator, keyVelocity_changed),
CALLBACKID(CAnmSplinePositionInterpolator, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmSplinePositionInterpolator, CAnmNode);


