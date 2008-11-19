/********************************************************************************
 * Flux
 *
 * File: anmcoordinate.cpp
 * Description: Coordinate node
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
#include "anmcoordinate.h"
#include "anmindexedfaceset.h"
#include "anmindexedlineset.h"
#include "anmpointset.h"

CAnmCoordinate::CAnmCoordinate() : CAnmNode () // IFS parent will add me
{
	m_point = new Point3Array;
}

CAnmCoordinate::~CAnmCoordinate()
{
	SafeUnRef(m_point);
}

void CAnmCoordinate::Realize()
{
	CAnmNode::Realize();
}

void CAnmCoordinate::Update()
{
	ClearStateDirty();
}

void CAnmCoordinate::FlagUpdate()
{
	SetStateDirty();
	NotifyParentsOfChange();
}

void CAnmCoordinate::SetPoint(Point3Array *pPoint)
{
	assert(pPoint != NULL);

	SafeUnRef(m_point);
	m_point = pPoint;
	m_point->Ref();

	SetStateDirty();
	NotifyParentsOfChange();


	CallCallbacks(CALLBACKID(CAnmCoordinate, point_changed), &pPoint);
}

DEFINE_VALUE(CAnmCoordinate, point, eValuePoint3Array, Point3Array*, 
			 GetPoint, SetPoint);
DEFINE_METHOD(CAnmCoordinate, set_point, eValuePoint3Array, Point3Array*, SetPoint);
DEFINE_CALLBACK(CAnmCoordinate, point_changed, eValuePoint3Array);

BEGIN_DEFINE_NODE_MEMBERS(CAnmCoordinate)
VALUEID(CAnmCoordinate, point), 
METHODID(CAnmCoordinate, set_point), 
CALLBACKID(CAnmCoordinate, point_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmCoordinate, CAnmNode);
