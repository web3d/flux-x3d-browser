/********************************************************************************
 * Flux
 *
 * File: anmcolorinterpolator.cpp
 * Description: ColorInterpolator class
 *						Interpolates RGB colors through HLS conversion
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
#include "anmcolorinterpolator.h"

CAnmColorInterpolator::CAnmColorInterpolator()
: CAnmNode()
{
}


CAnmColorInterpolator::~CAnmColorInterpolator()
{
}

// CAnmNode overrides
void CAnmColorInterpolator::Realize()
{
	CAnmNode::Realize();
}

void CAnmColorInterpolator::SetFraction(Float fraction)
{
	// this is where the interpolation gets done

	Color c(1, 1, 1);

	m_interp.Interp(fraction, &c);
	CallCallbacks(CALLBACKID(CAnmColorInterpolator, value_changed), &c);

}

void CAnmColorInterpolator::SetKey(FloatArray *pKey)
{
	m_interp.SetKey(pKey);
}

void CAnmColorInterpolator::SetKeyValue(ColorArray *pKeyValue)
{
	m_interp.SetKeyValue(pKeyValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmColorInterpolator, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmColorInterpolator, keyValue, eValueColorArray, ColorArray *, GetKeyValue, SetKeyValue);

DEFINE_METHOD(CAnmColorInterpolator, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmColorInterpolator, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmColorInterpolator, set_keyValue, eValueColorArray, ColorArray *, SetKeyValue);

DEFINE_CALLBACK(CAnmColorInterpolator, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmColorInterpolator, keyValue_changed, eValueColorArray);
DEFINE_CALLBACK(CAnmColorInterpolator, value_changed, eValueColor);


BEGIN_DEFINE_NODE_MEMBERS(CAnmColorInterpolator)
VALUEID(CAnmColorInterpolator, key),
VALUEID(CAnmColorInterpolator, keyValue),
METHODID(CAnmColorInterpolator, set_fraction),
METHODID(CAnmColorInterpolator, set_key),
METHODID(CAnmColorInterpolator, set_keyValue),
CALLBACKID(CAnmColorInterpolator, key_changed),
CALLBACKID(CAnmColorInterpolator, keyValue_changed),
CALLBACKID(CAnmColorInterpolator, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmColorInterpolator, CAnmNode);


