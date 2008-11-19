/********************************************************************************
 * Flux
 *
 * File: anmsphere.cpp
 * Description: Sphere node
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
#include "anmsphere.h"
#include "anmmesh.h"
#include "anmprimitive.h"
#include "anmworld.h"

CAnmMesh *CAnmSphere::m_solidspheremesh = NULL;
CAnmMesh *CAnmSphere::m_hollowspheremesh = NULL;

CAnmSphere::CAnmSphere() : CAnmGeometry(), 
m_radius(ANMSPHERE_DEFAULT_RADIUS)
{
}

CAnmSphere::~CAnmSphere()
{
}

void CAnmSphere::SetRadius(Float radius)
{
	m_radius = radius;
	SetStateAndMatrixDirty();
}


void CAnmSphere::Realize()
{
	if (m_solid)
	{
		if (m_solidspheremesh == NULL)
		{
			CAnmWorld *pWorld = GetWorld();
//			assert(pWorld);
			ABORT_IF_WORLD_IS_NULL

			cApplication *pApp = pWorld->GetApp();
			assert(pApp);

			CreateSphereMesh(pApp, true);

		}
		assert(m_solidspheremesh);
		m_mesh = m_solidspheremesh;
	}
	else
	{
		if (m_hollowspheremesh == NULL)
		{
			CAnmWorld *pWorld = GetWorld();
//			assert(pWorld);
			ABORT_IF_WORLD_IS_NULL

			cApplication *pApp = pWorld->GetApp();
			assert(pApp);

			CreateSphereMesh(pApp, false);

		}
		assert(m_hollowspheremesh);
		m_mesh = m_hollowspheremesh;
	}

	m_mesh->Ref();			// add a reference. N.B.: who gets rid of the original?

	CAnmGeometry::Realize();		// chain down
}

void CAnmSphere::Update()
{
	if (StateDirty())
	{
		// set radius value into the matrix
		SetModelScale(Point3(m_radius, m_radius, m_radius));

		ClearStateDirty();
	}
}


void CAnmSphere::GenBoundingVolumes()
{
	m_bbox.SetCenterSize(Point3(0, 0, 0), Point3(m_radius * 2, m_radius * 2, m_radius * 2));
}

// GenerateSphere - from a paper written by Paul Bourke, May 1992
// http://astronomy.swin.edu.au/pbourke/modelling/sphere/
// I'm using the first (polar coordinate) method because the other more sophisticated
// methods generate vertices in an order that I can't handle


void CAnmSphere::CreateSphereMesh(cApplication *pApp, bool solid)
{
	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	int nSphereCrossSections;
	int nSphereSections;
	int nSphereVertices;

	eAnmPrimitiveQualityOptions primQuality = (eAnmPrimitiveQualityOptions)
		pApp->GetOptions(eAnmOptionsPrimitiveQuality);

	if (primQuality == eAnmPrimitiveGoodQuality)
	{
		nSphereCrossSections = ANMSPHERE_NCROSSSECTIONS_GOOD;
		nSphereSections	= ANMSPHERE_NSECTIONS_GOOD;
		nSphereVertices = ((nSphereSections+1)*nSphereCrossSections);
	}
	else if (primQuality == eAnmPrimitiveBetterQuality)
	{
		nSphereCrossSections = ANMSPHERE_NCROSSSECTIONS_BETTER;
		nSphereSections	= ANMSPHERE_NSECTIONS_BETTER;
		nSphereVertices = ((nSphereSections+1)*nSphereCrossSections);
	}
	else
	{
		nSphereCrossSections = ANMSPHERE_NCROSSSECTIONS_BEST;
		nSphereSections	= ANMSPHERE_NSECTIONS_BEST;
		nSphereVertices = ((nSphereSections+1)*nSphereCrossSections);
	}

	int i, j;

	float *r = new float[nSphereCrossSections];
	float *h = new float[nSphereCrossSections];

	float phi = 0;
	float dphi = PI / (float) (nSphereCrossSections-1);
	
	for (i = 0; i < nSphereCrossSections; i++)
	{
		if (i == 0 || i == nSphereCrossSections - 1)
		{
			r[i] = 0;
		}
		else
		{
			r[i] = sin(phi);
		}
		h[i] = cos(phi);
		phi += dphi;
	}

	float *x = new float[nSphereSections + 1];
	float *z = new float[nSphereSections + 1];

	// create angle multipliers for each section
	float theta = 1.5f * PI;
	float dtheta = 2.f * PI / (float) nSphereSections;

	for (i=0; i<nSphereSections+1; i++) {
		x[i] = cos(theta);
		z[i] = sin(theta);
		theta += dtheta;
	}

	// Create the mesh
	CAnmIndexedTriangleList *pTriangleList = new CAnmIndexedTriangleList(pDevice);
	tAnmVertexArray *pVertices = pTriangleList->GetVertices();
	tAnmIndexArray *pIndices = pTriangleList->GetIndices();

	pVertices->reserve(nSphereVertices);
	pIndices->reserve(nSphereSections * nSphereCrossSections * 6);

	float v = 1.0f;
	float dv = 1.0f / (nSphereCrossSections-1);
	float du = 1.0f / nSphereSections;
	for (i = 0; i < nSphereCrossSections; i++)
	{
		float u = 1.0f;

		for (j = 0; j < nSphereSections + 1; j++)
		{
			Point3 vec;

			vec.x = x[j] * r[i];
			vec.y = h[i];
			vec.z = z[j] * r[i];

			sAnmVertex vert(vec, vec, Point2(u, v));	// cool, the normal is just the vector location
			pVertices->push_back(vert);

			u -= du;
		}

		v -= dv;
	}

	for (i = 1; i < nSphereCrossSections; i++)
	{
		for (j = 1; j < nSphereSections + 1; j++)
		{
			int offset1 = i * (nSphereSections + 1);
			int offset2 = (i - 1) * (nSphereSections + 1);

			// at the poles, we have 3-vertex facets. everywhere
			// else it's 4
			if (i == 1 || i == nSphereCrossSections - 2)
			{
				pIndices->push_back(offset1 + j - 1);
				pIndices->push_back(offset2 + j);
				pIndices->push_back(offset1 + j);

				pIndices->push_back(offset1 + j - 1);
				pIndices->push_back(offset2 + j - 1);
				pIndices->push_back(offset2 + j);
			} 
			else
			{
				pIndices->push_back(offset1 + j);
				pIndices->push_back(offset2 + j - 1);
				pIndices->push_back(offset2 + j);

				pIndices->push_back(offset1 + j - 1);
				pIndices->push_back(offset2 + j - 1);
				pIndices->push_back(offset1 + j);
			}
		}
	}


	CAnmMesh *pMesh = new CAnmMesh();

	pMesh->AddPrimitive(pTriangleList);
	pMesh->SetDiffuseColor(color4::White);

	if (solid)
	{
		m_solidspheremesh = pMesh;
	}
	else
	{
		m_hollowspheremesh = pMesh;
		tAnmPrimitiveArray *pPrimitives = m_hollowspheremesh->GetPrimitives();
		int nprims = pPrimitives->size();
		for (int i = 0; i < nprims ; i++)
		{
			CAnmPrimitive *pPrim = (*pPrimitives)[i];
			pPrim->GenerateBackfaces(pDevice);
		}
	}

	delete r;
	delete h;
	delete x;
	delete z;
}


// Tables for Callbacks, Methods and Set/GetValues
DEFINE_INHERITED_VALUE(CAnmSphere, solid, CAnmGeometry);
DEFINE_VALUE(CAnmSphere, radius, eValueFloat, Float, GetRadius, SetRadius);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmSphere)
VALUEID(CAnmSphere, radius),
VALUEID(CAnmSphere, solid),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmSphere, CAnmGeometry);

