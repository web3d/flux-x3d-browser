/********************************************************************************
 * Flux
 *
 * File: anmcone.cpp
 * Description: Cone node
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
#include "anmcone.h"
#include "anmmesh.h"
#include "anmprimitive.h"
#include "anmworld.h"

CAnmCone::CAnmCone() : CAnmGeometry (),
m_bottomradius(ANMCONE_DEFAULT_BOTTOMRADIUS),
m_height(ANMCONE_DEFAULT_HEIGHT),
m_bottom(ANMCONE_DEFAULT_BOTTOM),
m_side(ANMCONE_DEFAULT_SIDE)
{
}

CAnmCone::~CAnmCone()
{
}

float CAnmCone::GenRadius()
{
	Point3 p(m_bottomradius, m_height / 2, 0);

	return p.Mag();
}

void CAnmCone::GenBoundingVolumes()
{
	m_bbox.SetCenterSize(Point3(0, 0, 0), Point3(m_bottomradius * 2, m_height, m_bottomradius * 2));
}

void CAnmCone::CreateConeMesh()
{
	int nConeSides;

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
		nConeSides = ANMCONE_NSIDES_GOOD;
	}
	else if (primQuality == eAnmPrimitiveBetterQuality)
	{
		nConeSides = ANMCONE_NSIDES_BETTER;
	}
	else
	{
		nConeSides = ANMCONE_NSIDES_BEST;
	}

	int i;
	sAnmVertex *sv = new sAnmVertex[nConeSides * 3];		// triangles for sides
	sAnmVertex *bv = new sAnmVertex[nConeSides + 2];		// triangle fan for bottom

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

	float dtheta = PI * 2.0f / nConeSides;

	float u = 0.0f;
	float du = 1.0f / nConeSides;
	float v = 0.0f;

	if (m_side)
	{
	
		sv[0] = sAnmVertex(0, 1, 0, 0, 1, 0, 0, 0);

		for (i = 0; i < nConeSides; i++)
		{
			float c = cos(theta);
			float s = sin(theta);

			sv[i * 3] = sAnmVertex(0, 1, 0, 0, 1, 0, u + du / 2.0f, 1);
			sv[i * 3 + 1] = sAnmVertex(cos(theta), -1, sin(theta), cos(theta), 0, sin(theta), u, 0.f);
			sv[i * 3 + 2] = sAnmVertex(cos(theta - dtheta), -1, sin(theta - dtheta),
				cos(theta - dtheta), 0, sin(theta - dtheta), u + du, 0.f);

			theta -= dtheta;
			u += du;
		}

		CAnmTriangleList *pList = new CAnmTriangleList(pDevice);
		pList->AddVertices(nConeSides * 3, &sv[0]);

		m_mesh->AddPrimitive(pList);
	}

	if (m_bottom)
	{
		if (righthanded)
			theta = PI * 1.5f;		// rotate the seam around to the back
		else
			theta = PI * 0.5f;

		bv[0] = sAnmVertex(0, -1, 0, 0, -1, 0, .5, .5);

		for (i = 1; i < nConeSides + 2; i++)
		{
			u = cos(theta) / 2.0f + .5f;
			v = sin(theta) / 2.0f + .5f;
			bv[i] = sAnmVertex(cos(theta), -1, sin(theta), 0, -1, 0, u, v);

			theta += dtheta;
		}

		CAnmTriangleFan *pFan = new CAnmTriangleFan(pDevice);
		pFan->AddVertices(nConeSides + 2, &bv[0]);

		m_mesh->AddPrimitive(pFan);
	
	}

	delete [] sv;
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

void CAnmCone::Realize()
{

	CreateConeMesh();


	CAnmGeometry::Realize();		// chain down
}

void CAnmCone::Update()
{
	if (StateDirty())
	{
		// set size values into the matrix
		SetModelScale(Point3(m_bottomradius, m_height / 2, m_bottomradius));

		ClearStateDirty();
	}
}

void CAnmCone::SetBottomRadius(Float bottomRadius)
{
	m_bottomradius = bottomRadius;
	SetStateAndMatrixDirty();
}

void CAnmCone::SetHeight(Float height)
{
	m_height = height;
	SetStateAndMatrixDirty();
}

void CAnmCone::SetBottom(Boolean bottom)
{
	m_bottom = bottom;
	SetStateDirty();
}


void CAnmCone::SetSide(Boolean side)
{
	m_side = side;
	SetStateDirty();
}




// Tables for Callbacks, Methods and Set/GetValues
DEFINE_INHERITED_VALUE(CAnmCone, solid, CAnmGeometry);

DEFINE_VALUE(CAnmCone, bottomRadius, eValueFloat, Float, GetBottomRadius, SetBottomRadius);
DEFINE_VALUE(CAnmCone, height, eValueFloat, Float, GetHeight, SetHeight);
DEFINE_VALUE(CAnmCone, bottom, eValueBoolean, Boolean, GetBottom, SetBottom);
DEFINE_VALUE(CAnmCone, side, eValueBoolean, Boolean, GetSide, SetSide);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmCone)
VALUEID(CAnmCone, bottomRadius),
VALUEID(CAnmCone, height),
VALUEID(CAnmCone, bottom),
VALUEID(CAnmCone, side),
VALUEID(CAnmCone, solid),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmCone, CAnmGeometry);
