/********************************************************************************
 * Flux
 *
 * File: anmscalarinterpolator.cpp
 * Description: Scalar Interpolator class
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
#include "anmsplinescalarinterpolator.h"


CAnmSplineScalarInterpolator::CAnmSplineScalarInterpolator() :
		m_interp( 0.0f ),
		CAnmNode( )
{
}


CAnmSplineScalarInterpolator::~CAnmSplineScalarInterpolator()
{
}

// CAnmNode overrides
void CAnmSplineScalarInterpolator::Realize()
{
	CAnmNode::Realize();
}



void CAnmSplineScalarInterpolator::SetFraction(Float fraction)
{
	// this is where the interpolation gets done
	Float p = 0.0;

	m_interp.SplineInterp(fraction, &p);
	CallCallbacks(CALLBACKID(CAnmSplineScalarInterpolator, value_changed), &p);
}

void CAnmSplineScalarInterpolator::SetKey(FloatArray *pKey)
{
	m_interp.SetKey(pKey);
}

void CAnmSplineScalarInterpolator::SetKeyValue(FloatArray *pKeyValue)
{
	m_interp.SetKeyValue(pKeyValue);
}
void CAnmSplineScalarInterpolator::SetKeyVelocity(FloatArray *pKeyVelocity)
{
	m_interp.SetKeyVelocity(pKeyVelocity);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmSplineScalarInterpolator, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmSplineScalarInterpolator, keyValue, eValueFloatArray, FloatArray *, GetKeyValue, SetKeyValue);
DEFINE_VALUE(CAnmSplineScalarInterpolator, closed, eValueBoolean, Boolean, GetClosed, SetClosed);
DEFINE_VALUE(CAnmSplineScalarInterpolator, normalizeVelocity, eValueBoolean, Boolean, GetNormalizeVelocity, SetNormalizeVelocity);
DEFINE_VALUE(CAnmSplineScalarInterpolator, keyVelocity, eValueFloatArray, FloatArray *, GetKeyVelocity, SetKeyVelocity);

DEFINE_METHOD(CAnmSplineScalarInterpolator, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmSplineScalarInterpolator, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmSplineScalarInterpolator, set_keyValue, eValueFloatArray, FloatArray *, SetKeyValue);
DEFINE_METHOD(CAnmSplineScalarInterpolator, set_closed, eValueBoolean, Boolean, SetClosed);
DEFINE_METHOD(CAnmSplineScalarInterpolator, set_normalizeVelocity, eValueBoolean, Boolean, SetNormalizeVelocity);
DEFINE_METHOD(CAnmSplineScalarInterpolator, set_keyVelocity, eValueFloatArray, FloatArray *, SetKeyVelocity);

DEFINE_CALLBACK(CAnmSplineScalarInterpolator, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmSplineScalarInterpolator, keyValue_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmSplineScalarInterpolator, closed_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmSplineScalarInterpolator, normalizeVelocity_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmSplineScalarInterpolator, keyVelocity_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmSplineScalarInterpolator, value_changed, eValueFloat);


BEGIN_DEFINE_NODE_MEMBERS(CAnmSplineScalarInterpolator)
VALUEID(CAnmSplineScalarInterpolator, key),
VALUEID(CAnmSplineScalarInterpolator, keyValue),
VALUEID(CAnmSplineScalarInterpolator, closed),
VALUEID(CAnmSplineScalarInterpolator, normalizeVelocity),
VALUEID(CAnmSplineScalarInterpolator, keyVelocity),
METHODID(CAnmSplineScalarInterpolator, set_fraction),
METHODID(CAnmSplineScalarInterpolator, set_key),
METHODID(CAnmSplineScalarInterpolator, set_keyValue),
METHODID(CAnmSplineScalarInterpolator, set_closed),
METHODID(CAnmSplineScalarInterpolator, set_normalizeVelocity),
METHODID(CAnmSplineScalarInterpolator, set_keyVelocity),
CALLBACKID(CAnmSplineScalarInterpolator, key_changed),
CALLBACKID(CAnmSplineScalarInterpolator, keyValue_changed),
CALLBACKID(CAnmSplineScalarInterpolator, closed_changed),
CALLBACKID(CAnmSplineScalarInterpolator, normalizeVelocity_changed),
CALLBACKID(CAnmSplineScalarInterpolator, keyVelocity_changed),
CALLBACKID(CAnmSplineScalarInterpolator, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmSplineScalarInterpolator, CAnmNode);


