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
#include "anmscalarinterpolator.h"

CAnmScalarInterpolator::CAnmScalarInterpolator()
: CAnmNode()
{
}

CAnmScalarInterpolator::~CAnmScalarInterpolator()
{
}

// CAnmNode overrides
void CAnmScalarInterpolator::Realize()
{
	CAnmNode::Realize();
}


void CAnmScalarInterpolator::SetFraction(Float fraction)
{
	// this is where the interpolation gets done

	Float f = 0.f;

	m_interp.Interp(fraction, &f);
	CallCallbacks(CALLBACKID(CAnmScalarInterpolator, value_changed), &f);

}

void CAnmScalarInterpolator::SetKey(FloatArray *pKey)
{
	m_interp.SetKey(pKey);
}

void CAnmScalarInterpolator::SetKeyValue(FloatArray *pKeyValue)
{
	m_interp.SetKeyValue(pKeyValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmScalarInterpolator, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmScalarInterpolator, keyValue, eValueFloatArray, FloatArray *, GetKeyValue, SetKeyValue);

DEFINE_METHOD(CAnmScalarInterpolator, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmScalarInterpolator, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmScalarInterpolator, set_keyValue, eValueFloatArray, FloatArray *, SetKeyValue);

DEFINE_CALLBACK(CAnmScalarInterpolator, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmScalarInterpolator, keyValue_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmScalarInterpolator, value_changed, eValueFloat);


BEGIN_DEFINE_NODE_MEMBERS(CAnmScalarInterpolator)
VALUEID(CAnmScalarInterpolator, key),
VALUEID(CAnmScalarInterpolator, keyValue),
METHODID(CAnmScalarInterpolator, set_fraction),
METHODID(CAnmScalarInterpolator, set_key),
METHODID(CAnmScalarInterpolator, set_keyValue),
CALLBACKID(CAnmScalarInterpolator, key_changed),
CALLBACKID(CAnmScalarInterpolator, keyValue_changed),
CALLBACKID(CAnmScalarInterpolator, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmScalarInterpolator, CAnmNode);


