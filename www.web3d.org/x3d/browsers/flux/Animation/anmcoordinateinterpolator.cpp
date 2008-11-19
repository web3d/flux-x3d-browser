/********************************************************************************
 * Flux
 *
 * File: anmcoordinateinterpolator.cpp
 * Description: CoordinateInterpolator class
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
#include "anmcoordinateinterpolator.h"


CAnmCoordinateInterpolator::CAnmCoordinateInterpolator()
: CAnmNode()
{
	m_outpoints = new Point3Array;
}

CAnmCoordinateInterpolator::~CAnmCoordinateInterpolator()
{
	SafeUnRef(m_outpoints);
}

// CAnmNode overrides
void CAnmCoordinateInterpolator::Realize()
{
	CAnmNode::Realize();
}

void CAnmCoordinateInterpolator::SetFraction(Float fraction)
{
	// this is where the interpolation gets done

	m_interp.Interp(fraction, m_outpoints);
	CallCallbacks(CALLBACKID(CAnmCoordinateInterpolator, value_changed), &m_outpoints);

}

void CAnmCoordinateInterpolator::SetKey(FloatArray *pKey)
{
	m_interp.SetKey(pKey);
}

void CAnmCoordinateInterpolator::SetKeyValue(Point3Array *pKeyValue)
{
	m_interp.SetKeyValue(pKeyValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmCoordinateInterpolator, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmCoordinateInterpolator, keyValue, eValuePoint3Array, Point3Array *, GetKeyValue, SetKeyValue);

DEFINE_METHOD(CAnmCoordinateInterpolator, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmCoordinateInterpolator, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmCoordinateInterpolator, set_keyValue, eValuePoint3Array, Point3Array *, SetKeyValue);

DEFINE_CALLBACK(CAnmCoordinateInterpolator, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmCoordinateInterpolator, keyValue_changed, eValuePoint3Array);
DEFINE_CALLBACK(CAnmCoordinateInterpolator, value_changed, eValuePoint3Array);


BEGIN_DEFINE_NODE_MEMBERS(CAnmCoordinateInterpolator)
VALUEID(CAnmCoordinateInterpolator, key),
VALUEID(CAnmCoordinateInterpolator, keyValue),
METHODID(CAnmCoordinateInterpolator, set_fraction),
METHODID(CAnmCoordinateInterpolator, set_key),
METHODID(CAnmCoordinateInterpolator, set_keyValue),
CALLBACKID(CAnmCoordinateInterpolator, key_changed),
CALLBACKID(CAnmCoordinateInterpolator, keyValue_changed),
CALLBACKID(CAnmCoordinateInterpolator, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmCoordinateInterpolator, CAnmNode);


