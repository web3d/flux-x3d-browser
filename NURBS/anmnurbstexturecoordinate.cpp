/********************************************************************************
 * Flux
 *
 * File: anmnurbstexturecoordinate.cpp
 * Description: NurbsTextureCoordinate node
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
#include "anmnurbstexturecoordinate.h"

CAnmNurbsTextureCoordinate::CAnmNurbsTextureCoordinate() : CAnmNode(),
m_uDimension(ANMNURBSTEXTURECOORDINATE_DEFAULT_UDIMENSION),
m_uOrder(ANMNURBSTEXTURECOORDINATE_DEFAULT_UORDER),
m_vDimension(ANMNURBSTEXTURECOORDINATE_DEFAULT_VDIMENSION),
m_vOrder(ANMNURBSTEXTURECOORDINATE_DEFAULT_VORDER)
{
	m_controlPoint = new Point2Array;
	m_weight = new DoubleArray;
	m_uKnot = new DoubleArray;
	m_vKnot = new DoubleArray;

}

CAnmNurbsTextureCoordinate::~CAnmNurbsTextureCoordinate()
{
	SafeUnRef(m_controlPoint);
	SafeUnRef(m_weight);
	SafeUnRef(m_uKnot);
	SafeUnRef(m_vKnot);
}

void CAnmNurbsTextureCoordinate::Realize()
{
	CAnmNode::Realize();		// chain down
}

void CAnmNurbsTextureCoordinate::Update()
{

	if (StateDirty())
	{
		// Only reset the values if anything has changed!
		// 
		m_evaluator.SetuOrder( m_uOrder );
		m_evaluator.SetuKnotVector( m_uKnot);
		m_evaluator.SetvOrder( m_vOrder );
		m_evaluator.SetvKnotVector( m_vKnot);

		m_evaluator.SetuDimension( m_uDimension );
		m_evaluator.SetvDimension( m_vDimension );

		m_evaluator.SetWeights( m_weight );

		m_evaluator.SetControlTextureCoords( m_controlPoint );


		ClearStateDirty();

	}
}


bool CAnmNurbsTextureCoordinate::Evaluate( double u, double v, float* texCoord )
{
	// krv??? do we need to test dirty state first?????
	//
	if (StateDirty())
	{
		Update();
	}

	texCoord[0] = 
	texCoord[1] = 0.0f;
	return ( m_evaluator.AreTextureCoordsValid() &&
				m_evaluator.Evaluate( u, v, NULL, NULL, texCoord ) );
}


// Accessors

void CAnmNurbsTextureCoordinate::SetControlPoint(Point2Array *pControlPoint)
{
	assert(pControlPoint != NULL);

	SafeUnRef(m_controlPoint);
	m_controlPoint = pControlPoint;
	m_controlPoint->Ref();

	SetStateDirty();
	NotifyParentsOfChange();

	CallCallbacks(CALLBACKID(CAnmNurbsTextureCoordinate, controlPoint_changed), &pControlPoint);
}

void CAnmNurbsTextureCoordinate::SetWeight(DoubleArray *pWeight)
{
	assert(pWeight != NULL);

	SafeUnRef(m_weight);
	m_weight = pWeight;
	m_weight->Ref();

	SetStateDirty();	

	CallCallbacks(CALLBACKID(CAnmNurbsTextureCoordinate, weight_changed), &pWeight);

}

void CAnmNurbsTextureCoordinate::SetUDimension(Integer uDimension)
{
	m_uDimension = uDimension;
}

void CAnmNurbsTextureCoordinate::SetUKnot(DoubleArray *pUknot)
{
	assert(pUknot != NULL);

	SafeUnRef(m_uKnot);
	m_uKnot = pUknot;
	m_uKnot->Ref();

	SetStateDirty();
}

void CAnmNurbsTextureCoordinate::SetUOrder(Integer uOrder)
{
	m_uOrder = uOrder;
}

void CAnmNurbsTextureCoordinate::SetVDimension(Integer vDimension)
{
	m_vDimension = vDimension;
}

void CAnmNurbsTextureCoordinate::SetVKnot(DoubleArray *pVknot)
{
	assert(pVknot != NULL);

	SafeUnRef(m_vKnot);
	m_vKnot = pVknot;
	m_vKnot->Ref();

	SetStateDirty();
}

void CAnmNurbsTextureCoordinate::SetVOrder(Integer vOrder)
{
	m_vOrder = vOrder;
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmNurbsTextureCoordinate, controlPoint, eValuePoint2Array, Point2Array*, GetControlPoint, SetControlPoint);
DEFINE_VALUE(CAnmNurbsTextureCoordinate, weight, eValueDoubleArray, DoubleArray*, GetWeight, SetWeight);
DEFINE_VALUE(CAnmNurbsTextureCoordinate, uDimension, eValueInteger, Integer, GetUDimension, SetUDimension);
DEFINE_VALUE(CAnmNurbsTextureCoordinate, uKnot, eValueDoubleArray, DoubleArray*, GetUKnot, SetUKnot);
DEFINE_VALUE(CAnmNurbsTextureCoordinate, uOrder, eValueInteger, Integer, GetUOrder, SetUOrder);
DEFINE_VALUE(CAnmNurbsTextureCoordinate, vDimension, eValueInteger, Integer, GetVDimension, SetVDimension);
DEFINE_VALUE(CAnmNurbsTextureCoordinate, vKnot, eValueDoubleArray, DoubleArray*, GetVKnot, SetVKnot);
DEFINE_VALUE(CAnmNurbsTextureCoordinate, vOrder, eValueInteger, Integer, GetVOrder, SetVOrder);

DEFINE_METHOD(CAnmNurbsTextureCoordinate, set_controlPoint, eValuePoint2Array, Point2Array*, SetControlPoint);
DEFINE_METHOD(CAnmNurbsTextureCoordinate, set_weight, eValueDoubleArray, DoubleArray*, SetWeight);

DEFINE_CALLBACK(CAnmNurbsTextureCoordinate, controlPoint_changed, eValuePoint2Array);
DEFINE_CALLBACK(CAnmNurbsTextureCoordinate, weight_changed, eValueDoubleArray);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmNurbsTextureCoordinate)
VALUEID(CAnmNurbsTextureCoordinate, controlPoint),
VALUEID(CAnmNurbsTextureCoordinate, weight),
VALUEID(CAnmNurbsTextureCoordinate, uDimension),
VALUEID(CAnmNurbsTextureCoordinate, uKnot),
VALUEID(CAnmNurbsTextureCoordinate, uOrder),
VALUEID(CAnmNurbsTextureCoordinate, vDimension),
VALUEID(CAnmNurbsTextureCoordinate, vKnot),
VALUEID(CAnmNurbsTextureCoordinate, vOrder),

METHODID(CAnmNurbsTextureCoordinate, set_controlPoint), 
METHODID(CAnmNurbsTextureCoordinate, set_weight),

CALLBACKID(CAnmNurbsTextureCoordinate, controlPoint_changed),
CALLBACKID(CAnmNurbsTextureCoordinate, weight_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmNurbsTextureCoordinate, CAnmNode);

