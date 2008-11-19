/********************************************************************************
 * Flux
 *
 * File: anmtexturecoordinate.cpp
 * Description: TextureCoordinate node
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
#include "anmtexturecoordinate.h"
#include "anmindexedfaceset.h"

CAnmTextureCoordinate::CAnmTextureCoordinate() : CAnmNode () // Parent IFS will add me
{
	m_point = new Point2Array;
}

CAnmTextureCoordinate::~CAnmTextureCoordinate()
{
	SafeUnRef(m_point);
}

void CAnmTextureCoordinate::Realize()
{
	CAnmNode::Realize();
}

void CAnmTextureCoordinate::Update()
{
	ClearStateDirty();
}


void CAnmTextureCoordinate::SetPoint(Point2Array *pPoint)
{
	assert(pPoint != NULL);

	SafeUnRef(m_point);
	m_point = pPoint;
	m_point->Ref();

	SetStateDirty();
	NotifyParentsOfChange();

	CallCallbacks(CALLBACKID(CAnmTextureCoordinate, point_changed), &pPoint);
}

DEFINE_VALUE(CAnmTextureCoordinate, point, eValuePoint2Array, Point2Array*, 
			 GetPoint, SetPoint);
DEFINE_METHOD(CAnmTextureCoordinate, set_point, eValuePoint2Array, Point2Array*, SetPoint);
DEFINE_CALLBACK(CAnmTextureCoordinate, point_changed, eValuePoint2Array);

BEGIN_DEFINE_NODE_MEMBERS(CAnmTextureCoordinate)
VALUEID(CAnmTextureCoordinate, point), 
METHODID(CAnmTextureCoordinate, set_point), 
CALLBACKID(CAnmTextureCoordinate, point_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmTextureCoordinate, CAnmNode);
