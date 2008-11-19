/********************************************************************************
 * Flux
 *
 * File: anmnurbssurfaceinterpolator.cpp
 * Description: NurbsSurfaceInterpolator node
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
#include "anmnurbssurfaceinterpolator.h"
#include "anmcoordinate.h"

CAnmNurbsSurfaceInterpolator::CAnmNurbsSurfaceInterpolator() : CAnmGroup(),
m_uDimension(ANMNURBSSURFACEINTERPOLATOR_DEFAULT_UDIMENSION),
m_uOrder(ANMNURBSSURFACEINTERPOLATOR_DEFAULT_UORDER),
m_vDimension(ANMNURBSSURFACEINTERPOLATOR_DEFAULT_VDIMENSION),
m_vOrder(ANMNURBSSURFACEINTERPOLATOR_DEFAULT_VORDER)
{
	m_controlPoint = NULL;

	m_weight = new DoubleArray;
	m_uKnot = new DoubleArray;
	m_vKnot = new DoubleArray;

}

CAnmNurbsSurfaceInterpolator::~CAnmNurbsSurfaceInterpolator()
{
	SafeUnRef(m_weight);
	SafeUnRef(m_uKnot);
	SafeUnRef(m_vKnot);
}

void CAnmNurbsSurfaceInterpolator::Realize()
{
	CAnmGroup::Realize();		// chain down
}

void CAnmNurbsSurfaceInterpolator::Update()
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

		m_evaluator.SetControlPoints( NULL );
		m_evaluator.SetControlTextureCoords( NULL );

		if( m_controlPoint ) {
			CAnmNode *pImp = m_controlPoint->GetImplementationNode();
			if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate))) {
				CAnmCoordinate *pCoord = (CAnmCoordinate *) pImp;
				m_evaluator.SetControlPoints( pCoord->GetPoint() );
			}
		}

		ClearStateDirty();

	}
}

eAnmReturnStatus CAnmNurbsSurfaceInterpolator::AddChild(CAnmNode *pChild)
{
	CAnmGroup::AddChild(pChild);

	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_controlPoint = pChild;

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmNurbsSurfaceInterpolator::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_controlPoint = NULL;

	CAnmGroup::RemoveChild(pChild);

	return eAnmReturnAllGood;
}

void CAnmNurbsSurfaceInterpolator::ChildChanged(CAnmNode *pChild)
{
	if (pChild == m_controlPoint)
	{
		// $$$ do we need a special dirty bit just for the control point?
		SetStateDirty();
	}

	SetStateDirty();
}

// Accessors

void CAnmNurbsSurfaceInterpolator::SetControlPoint(CAnmNode *pControlPoint)
{
	if (m_controlPoint == pControlPoint)
		return;

	if (m_controlPoint)
		RemoveChild(m_controlPoint);

	if (pControlPoint == NULL)
		return;

	AddChild(pControlPoint);
	m_controlPoint = pControlPoint;

	// $$$ do we need a special dirty bit just for the control point?
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmNurbsSurfaceInterpolator, controlPoint_changed), &m_controlPoint);
}

void CAnmNurbsSurfaceInterpolator::SetFraction(Point2 fraction)
{
	if( m_evaluator.ArePointsValid() ) {
		float EvalPoint[3];
		float EvalNormal[3];
		double u = fraction.x;
		double v = fraction.y;
		if( m_evaluator.Evaluate( u, v, EvalPoint, EvalNormal ) ) {

			Point3 position( EvalPoint[0], EvalPoint[1], EvalPoint[2] );
			Point3 normal( EvalNormal[0], EvalNormal[1], EvalNormal[2] );

			// Then generate position_changed and normal_changed events like so:

			CallCallbacks(CALLBACKID(CAnmNurbsSurfaceInterpolator, position_changed), &position);
			CallCallbacks(CALLBACKID(CAnmNurbsSurfaceInterpolator, normal_changed), &normal);
		}

	}

}

void CAnmNurbsSurfaceInterpolator::SetWeight(DoubleArray *pWeight)
{
	assert(pWeight != NULL);

	SafeUnRef(m_weight);
	m_weight = pWeight;
	m_weight->Ref();

	SetStateDirty();	

	CallCallbacks(CALLBACKID(CAnmNurbsSurfaceInterpolator, weight_changed), &pWeight);

}

void CAnmNurbsSurfaceInterpolator::SetUDimension(Integer uDimension)
{
	m_uDimension = uDimension;
}

void CAnmNurbsSurfaceInterpolator::SetUKnot(DoubleArray *pUknot)
{
	assert(pUknot != NULL);

	SafeUnRef(m_uKnot);
	m_uKnot = pUknot;
	m_uKnot->Ref();

	SetStateDirty();
}

void CAnmNurbsSurfaceInterpolator::SetUOrder(Integer uOrder)
{
	m_uOrder = uOrder;
}

void CAnmNurbsSurfaceInterpolator::SetVDimension(Integer vDimension)
{
	m_vDimension = vDimension;
}

void CAnmNurbsSurfaceInterpolator::SetVKnot(DoubleArray *pVknot)
{
	assert(pVknot != NULL);

	SafeUnRef(m_vKnot);
	m_vKnot = pVknot;
	m_vKnot->Ref();

	SetStateDirty();
}

void CAnmNurbsSurfaceInterpolator::SetVOrder(Integer vOrder)
{
	m_vOrder = vOrder;
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmNurbsSurfaceInterpolator, controlPoint, eValueNode, CAnmNode*, GetControlPoint, SetControlPoint);
DEFINE_VALUE(CAnmNurbsSurfaceInterpolator, weight, eValueDoubleArray, DoubleArray*, GetWeight, SetWeight);
DEFINE_VALUE(CAnmNurbsSurfaceInterpolator, uDimension, eValueInteger, Integer, GetUDimension, SetUDimension);
DEFINE_VALUE(CAnmNurbsSurfaceInterpolator, uKnot, eValueDoubleArray, DoubleArray*, GetUKnot, SetUKnot);
DEFINE_VALUE(CAnmNurbsSurfaceInterpolator, uOrder, eValueInteger, Integer, GetUOrder, SetUOrder);
DEFINE_VALUE(CAnmNurbsSurfaceInterpolator, vDimension, eValueInteger, Integer, GetVDimension, SetVDimension);
DEFINE_VALUE(CAnmNurbsSurfaceInterpolator, vKnot, eValueDoubleArray, DoubleArray*, GetVKnot, SetVKnot);
DEFINE_VALUE(CAnmNurbsSurfaceInterpolator, vOrder, eValueInteger, Integer, GetVOrder, SetVOrder);

DEFINE_METHOD(CAnmNurbsSurfaceInterpolator, set_controlPoint, eValueNode, CAnmNode*, SetControlPoint);
DEFINE_METHOD(CAnmNurbsSurfaceInterpolator, set_fraction, eValuePoint2, Point2, SetFraction);
DEFINE_METHOD(CAnmNurbsSurfaceInterpolator, set_weight, eValueDoubleArray, DoubleArray*, SetWeight);

DEFINE_CALLBACK(CAnmNurbsSurfaceInterpolator, controlPoint_changed, eValueNode);
DEFINE_CALLBACK(CAnmNurbsSurfaceInterpolator, position_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmNurbsSurfaceInterpolator, normal_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmNurbsSurfaceInterpolator, weight_changed, eValueDoubleArray);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmNurbsSurfaceInterpolator)
VALUEID(CAnmNurbsSurfaceInterpolator, controlPoint),
VALUEID(CAnmNurbsSurfaceInterpolator, weight),
VALUEID(CAnmNurbsSurfaceInterpolator, uDimension),
VALUEID(CAnmNurbsSurfaceInterpolator, uKnot),
VALUEID(CAnmNurbsSurfaceInterpolator, uOrder),
VALUEID(CAnmNurbsSurfaceInterpolator, vDimension),
VALUEID(CAnmNurbsSurfaceInterpolator, vKnot),
VALUEID(CAnmNurbsSurfaceInterpolator, vOrder),

METHODID(CAnmNurbsSurfaceInterpolator, set_controlPoint), 
METHODID(CAnmNurbsSurfaceInterpolator, set_fraction), 
METHODID(CAnmNurbsSurfaceInterpolator, set_weight),

CALLBACKID(CAnmNurbsSurfaceInterpolator, controlPoint_changed),
CALLBACKID(CAnmNurbsSurfaceInterpolator, position_changed),
CALLBACKID(CAnmNurbsSurfaceInterpolator, normal_changed),
CALLBACKID(CAnmNurbsSurfaceInterpolator, weight_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmNurbsSurfaceInterpolator, CAnmGroup);

