/********************************************************************************
 * Flux
 *
 * File: anmgeometry.cpp
 * Description: Geometry node base class
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
#include "anmgeometry.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmintersection.h"
#include "anmmesh.h"
#include "anmpickmanager.h"
#include "anmshape.h"
#include "anmviewport.h"
#include "anmvisitor.h"
#include "anmworld.h"


CAnmGeometry::CAnmGeometry() : CAnmGroup(), // Parent Shape will add me
	m_solid(ANMGEOMETRY_DEFAULT_SOLID)
{
	m_mesh = NULL;
	m_modelscale = Point3(1, 1, 1);
	m_polycount = 0L;
}


CAnmGeometry::~CAnmGeometry()
{
	SafeUnRef(m_mesh);
}

// Realize method - N.B.: this might be overridden and chained down to
void CAnmGeometry::Realize()
{
	// make any data structures here

	// call Group's realize to get the children built

	CAnmGroup::Realize();

	// get the radius; we'll need that for a bunch of stuff
	GenBoundingVolumes();

	// get the poly count
	if (m_mesh)
		m_polycount = m_mesh->GetPolyCount();

}

// Update method - N.B.: this will be overridden by all subclasses...
void CAnmGeometry::Update()
{
	//... therefore we should never get here
	assert(FALSE);
}

// Traverse method
// N.B.: We really should never get here, but if a piece of geometry is "dangling" ->
// in the scene graph, e.g. as a PROTO field without a Shape wrapping it, we might; ->
// So let's just don't do anything
void CAnmGeometry::Traverse(CAnmUpdateVisitor *pVisitor)
{
}

void CAnmGeometry::Render(CAnmDevice *pDevice, int texturestage, class CAnmCamera *pCamera,
						  const matrix4 &worldmat)
{
	m_mesh->Render(pDevice, texturestage);
}

inline void ScalePoint(Point3 sc, Point3 &pt)
{
	pt.x *= sc.x;
	pt.y *= sc.y;
	pt.z *= sc.z;
}

inline void InvScalePoint(Point3 sc, Point3 &pt)
{
	pt.x /= sc.x;
	pt.y /= sc.y;
	pt.z /= sc.z;
}

void CAnmGeometry::SetModelScale(Point3 scale)
{
	m_modelscale = scale;
}

void CAnmGeometry::GenBoundingVolumes()
{
	if (m_mesh)
		m_mesh->GetBoundingBox(&m_bbox);
	else
		m_bbox.SetCenterSize(Point3(0, 0, 0), Point3(0, 0, 0));
}

bool CAnmGeometry::HasAlpha()
{
	if (m_mesh)
		return m_mesh->HasAlpha();
	else
		return false;
}

void CAnmGeometry::SetSolid(Boolean solid)
{
	m_solid = solid;
	SetDirtyBits(eAnmGeometrySolidDirty);
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmGeometry, solid, eValueBoolean, Boolean, 
			 GetSolid, SetSolid);

// member table
BEGIN_DEFINE_MEMBERS(CAnmGeometry)

VALUEID(CAnmGeometry, solid),

END_DEFINE_MEMBERS


DEFINE_ABSTRACT_CLASS(CAnmGeometry, CAnmGroup)
