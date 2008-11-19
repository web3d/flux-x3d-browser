/********************************************************************************
 * Flux
 *
 * File: anmcylinder.cpp
 * Description: Cylinder node
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
#include "anmcylinder.h"
#include "anmmesh.h"
#include "anmprimitive.h"
#include "anmworld.h"

CAnmCylinder::CAnmCylinder() :  CAnmGeometry (),
m_radius(ANMCYLINDER_DEFAULT_RADIUS),
m_height(ANMCYLINDER_DEFAULT_HEIGHT),
m_bottom(ANMCYLINDER_DEFAULT_BOTTOM),
m_side(ANMCYLINDER_DEFAULT_SIDE),
m_top(ANMCYLINDER_DEFAULT_TOP)
{
}

CAnmCylinder::~CAnmCylinder()
{
}

float CAnmCylinder::GenRadius()
{
	Point3 p(m_radius, m_height / 2, 0);

	return p.Mag();
}


void CAnmCylinder::GenBoundingVolumes()
{
	m_bbox.SetCenterSize(Point3(0, 0, 0), Point3(m_radius * 2, m_height, m_radius * 2));
}

void CAnmCylinder::CreateCylinderMesh()
{
	int nCylinderSides;

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	eAnmPrimitiveQualityOptions primQuality =
		(eAnmPrimitiveQualityOptions) pApp->GetOptions(eAnmOptionsPrimitiveQuality);

	if (primQuality == eAnmPrimitiveGoodQuality)
	{
		nCylinderSides = ANMCYLINDER_NUMSIDES_GOOD;
	}
	else if (primQuality == eAnmPrimitiveBetterQuality)
	{
		nCylinderSides = ANMCYLINDER_NUMSIDES_BETTER;
	}
	else
	{
		nCylinderSides = ANMCYLINDER_NUMSIDES_BEST;
	}

	int i;
	sAnmVertex *sv = new sAnmVertex[2 * nCylinderSides + 2];		// triangle strip for sides
	sAnmVertex *tv = new sAnmVertex[nCylinderSides + 2];			// triangle fan for top
	sAnmVertex *bv = new sAnmVertex[nCylinderSides + 2];			// triangle fan for bottom

	m_mesh = new CAnmMesh();

	int righthanded =
		(pApp->GetOptions(eAnmOptionsCoordinateSystem) == 
			eAnmCoordinatesRightHanded);

	// rotate around a disk by degree increments to generate x-z coords for vertices
	float theta;
	
	if (righthanded)
		theta = PI * 1.5f;		// rotate the seam around to the back
	else
		theta = PI * 0.5f;

	float dtheta = PI * 2.0f / nCylinderSides;

	float u = 0.0f;
	float du = 1.0f / (nCylinderSides + 1);
	float v = 0.0f;

	if (m_side)
	{
	
		if (righthanded)
			theta = PI * 1.5f;		// rotate the seam around to the back
		else
			theta = PI * 0.5f;

		for (i = 0; i <  nCylinderSides + 1; i++)
		{
			sv[i * 2] = sAnmVertex(cos(theta), 1, sin(theta), cos(theta), 0, sin(theta), u, 1.f);
			sv[i * 2 + 1] = sAnmVertex(cos(theta), -1, sin(theta), cos(theta), 0, sin(theta), u, 0.f);
			theta -= dtheta;
			u += du;
		}

		CAnmTriangleStrip *pStrip = new CAnmTriangleStrip(pDevice);
		pStrip->AddVertices(2 * nCylinderSides + 2, &sv[0]);

		m_mesh->AddPrimitive(pStrip);
	}


	if (m_top)
	{
	
		if (righthanded)
			theta = PI * 1.5f;		// rotate the seam around to the back
		else
			theta = PI * 0.5f;


		tv[0] = sAnmVertex(0, 1, 0, 0, 1, 0, .5, .5);

		for (i = 1; i < nCylinderSides + 2; i++)
		{
			u = cos(theta) / 2.0f + .5f;
			v = -sin(theta) / 2.0f + .5f;
			tv[i] = sAnmVertex(cos(theta), 1, sin(theta), 0, 1, 0, u, v);
			theta -= dtheta;
		}


		CAnmTriangleFan *pFan = new CAnmTriangleFan(pDevice);
		pFan->AddVertices(nCylinderSides + 2, &tv[0]);

		m_mesh->AddPrimitive(pFan);
	}

	if (m_bottom)
	{
		if (righthanded)
			theta = PI * 1.5f;		// rotate the seam around to the back
		else
			theta = PI * 0.5f;

		bv[0] = sAnmVertex(0, -1, 0, 0, -1, 0, .5, .5);

		for (i = 1; i < nCylinderSides + 2; i++)
		{
			u = cos(theta) / 2.0f + .5f;
			v = sin(theta) / 2.0f + .5f;
			bv[i] = sAnmVertex(cos(theta), -1, sin(theta), 0, -1, 0, u, v);
			theta += dtheta;
		}

		CAnmTriangleFan *pFan = new CAnmTriangleFan(pDevice);
		pFan->AddVertices(nCylinderSides + 2, &bv[0]);

		m_mesh->AddPrimitive(pFan);	
	}

	delete [] sv;
	delete [] tv;
	delete [] bv;

	m_mesh->SetDiffuseColor(color4::White);

	if (!m_solid)
	{
		tAnmPrimitiveArray *pPrimitives = m_mesh->GetPrimitives();
		int nprims = pPrimitives->size();
		for (int i = 0; i < nprims ; i++)
		{
			CAnmPrimitive *pPrim = (*pPrimitives)[i];
			pPrim->GenerateBackfaces(pDevice);
		}
	}
}

void CAnmCylinder::Realize()
{
	CreateCylinderMesh();

	CAnmGeometry::Realize();		// chain down
}

void CAnmCylinder::Update()
{
	if (StateDirty())
	{
		// set size values into the matrix
		SetModelScale(Point3(m_radius, m_height / 2, m_radius));

		ClearStateDirty();
	}
}


void CAnmCylinder::SetBottom(Boolean bottom)
{
	m_bottom = bottom;
	SetStateDirty();
}

void CAnmCylinder::SetHeight(Float height)
{
	m_height = height;
	SetStateAndMatrixDirty();
}

void CAnmCylinder::SetRadius(Float radius)
{
	m_radius = radius;
	SetStateAndMatrixDirty();
}

void CAnmCylinder::SetSide(Boolean side)
{
	m_side = side;
	SetStateDirty();
}

void CAnmCylinder::SetTop(Boolean top)
{
	m_top = top;
	SetStateDirty();
}



// Tables for Callbacks, Methods and Set/GetValues
DEFINE_INHERITED_VALUE(CAnmCylinder, solid, CAnmGeometry);

DEFINE_VALUE(CAnmCylinder, bottom, eValueBoolean, Boolean, GetBottom, SetBottom);
DEFINE_VALUE(CAnmCylinder, height, eValueFloat, Float, GetHeight, SetHeight);
DEFINE_VALUE(CAnmCylinder, radius, eValueFloat, Float, GetRadius, SetRadius);
DEFINE_VALUE(CAnmCylinder, side, eValueBoolean, Boolean, GetSide, SetSide);
DEFINE_VALUE(CAnmCylinder, top, eValueBoolean, Boolean, GetTop, SetTop);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmCylinder)
VALUEID(CAnmCylinder, bottom),
VALUEID(CAnmCylinder, height),
VALUEID(CAnmCylinder, radius),
VALUEID(CAnmCylinder, side),
VALUEID(CAnmCylinder, solid),
VALUEID(CAnmCylinder, top),


END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmCylinder, CAnmGeometry);
