/********************************************************************************
 * Flux
 *
 * File: anmcoordinateinterpolator2D.cpp
 * Description: 2D CoordinateInterpolator class
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
#include "anmcoordinateinterpolator2D.h"


CAnmCoordinateInterpolator2D::CAnmCoordinateInterpolator2D()
: CAnmNode()
{
	m_outpoints = new Point2Array;
}

CAnmCoordinateInterpolator2D::~CAnmCoordinateInterpolator2D()
{
	SafeUnRef(m_outpoints);
}

// CAnmNode overrides
void CAnmCoordinateInterpolator2D::Realize()
{
	CAnmNode::Realize();
}

void CAnmCoordinateInterpolator2D::SetFraction(Float fraction)
{
	// this is where the interpolation gets done

	m_interp.Interp(fraction, m_outpoints);
	CallCallbacks(CALLBACKID(CAnmCoordinateInterpolator2D, value_changed), &m_outpoints);

}

void CAnmCoordinateInterpolator2D::SetKey(FloatArray *pKey)
{
	m_interp.SetKey(pKey);
}

void CAnmCoordinateInterpolator2D::SetKeyValue(Point2Array *pKeyValue)
{
	m_interp.SetKeyValue(pKeyValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmCoordinateInterpolator2D, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmCoordinateInterpolator2D, keyValue, eValuePoint2Array, Point2Array *, GetKeyValue, SetKeyValue);

DEFINE_METHOD(CAnmCoordinateInterpolator2D, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmCoordinateInterpolator2D, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmCoordinateInterpolator2D, set_keyValue, eValuePoint2Array, Point2Array *, SetKeyValue);

DEFINE_CALLBACK(CAnmCoordinateInterpolator2D, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmCoordinateInterpolator2D, keyValue_changed, eValuePoint2Array);
DEFINE_CALLBACK(CAnmCoordinateInterpolator2D, value_changed, eValuePoint2Array);


BEGIN_DEFINE_NODE_MEMBERS(CAnmCoordinateInterpolator2D)
VALUEID(CAnmCoordinateInterpolator2D, key),
VALUEID(CAnmCoordinateInterpolator2D, keyValue),
METHODID(CAnmCoordinateInterpolator2D, set_fraction),
METHODID(CAnmCoordinateInterpolator2D, set_key),
METHODID(CAnmCoordinateInterpolator2D, set_keyValue),
CALLBACKID(CAnmCoordinateInterpolator2D, key_changed),
CALLBACKID(CAnmCoordinateInterpolator2D, keyValue_changed),
CALLBACKID(CAnmCoordinateInterpolator2D, value_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmCoordinateInterpolator2D, CAnmNode);


