/********************************************************************************
 * Flux
 *
 * File: anmbox.cpp
 * Description: Box node
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
#include "anmbox.h"
#include "anmmesh.h"
#include "anmworld.h"

CAnmMesh *CAnmBox::m_solidcubemesh = NULL;
CAnmMesh *CAnmBox::m_hollowcubemesh = NULL;

CAnmBox::CAnmBox() : CAnmGeometry(),
m_size(ANMBOX_DEFAULT_SIZE)
{
}

CAnmBox::~CAnmBox()
{
}

float CAnmBox::GenRadius()
{
	float result;

	Point3 px(m_size.x / 2, 0, 0), py(0, m_size.y / 2, 0), pz(0, 0, m_size.z / 2);

	result = px.Mag();
	if (py.Mag() > result)
		result = py.Mag();
	if (pz.Mag() > result)
		result = pz.Mag();

	return result;
}

void CAnmBox::GenBoundingVolumes()
{
	m_bbox.SetCenterSize(Point3(0, 0, 0), m_size);
}

void CAnmBox::SetSize(Point3 size)
{
	m_size = size;
	SetStateAndMatrixDirty();
}


// vertices and normals of a 2-unit cube
// duplicate the faces for texturing purposes; D3D needs separate faces otherwise
// it will wrap the textures around the entire cube
// all data in RH coordinates
static Point3 cube_vertices[] = {
	Point3(-1.f,-1.f, 1.f),
	Point3(-1.f,-1.f,-1.f),
	Point3(-1.f, 1.f, 1.f),
	Point3(-1.f, 1.f,-1.f),
	Point3(1.f,-1.f, 1.f),
	Point3(1.f,-1.f,-1.f),
	Point3(1.f, 1.f, 1.f),
	Point3(1.f, 1.f,-1.),
	Point3(-1.f,-1.f, 1.f),
	Point3(-1.f,-1.f,-1.f),
	Point3(-1.f, 1.f, 1.f),
	Point3(-1.f, 1.f,-1.f),
	Point3(1.f,-1.f, 1.f),
	Point3(1.f,-1.f,-1.f),
	Point3(1.f, 1.f, 1.f),
	Point3(1.f, 1.f,-1.f),
	Point3(-1.f,-1.f, 1.f),
	Point3(-1.f,-1.f,-1.f),
	Point3(-1.f, 1.f, 1.f),
	Point3(-1.f, 1.f,-1.f),
	Point3(1.f,-1.f, 1.f),
	Point3(1.f,-1.f,-1.f),
	Point3(1.f, 1.f, 1.f),
	Point3(1.f, 1.f,-1.f),
};

static Point3 cube_normals[] = {
	Point3(0.0f,  0.0f,  -1.0f),
	Point3(0.0f,  1.0f,  0.0f),
	Point3(1.0f,  0.0f,  0.0f),
	Point3(0.0f,  0.0f,  1.0f),
	Point3(0.0f, -1.0f,  0.0f),
	Point3(-1.0f,  0.0f,  0.0f),
};


static Point2 cube_uvs[] = {
	Point2(0.0f, 1.0f),
	Point2(1.0f, 1.0f),
	Point2(1.0f, 0.0f),
	Point2(0.0f, 0.0f),
};

static int cube_coordindices[] = {
	5, 1, 3, 7, -1,
	2, 6, 15, 11, -1,
	4, 13, 23, 14, -1,
	0, 12, 22, 10, -1,
	9, 21, 20, 8, -1,
	17, 16, 18, 19, -1,
};

static int cube_normalindices[] = {
	0, 0, 0, 0, -1,
	1, 1, 1, 1, -1,
	2, 2, 2, 2, -1,
	3, 3, 3, 3, -1,
	4, 4, 4, 4, -1,
	5, 5, 5, 5, -1,
};

static int cube_uvindices[] = {
	0, 1, 2, 3, -1,
	0, 1, 2, 3, -1,
	0, 1, 2, 3, -1,
	0, 1, 2, 3, -1,
	0, 1, 2, 3, -1,
	0, 1, 2, 3, -1,
};

static int cube_vertexcount = sizeof(cube_vertices) / sizeof(cube_vertices[0]);	
static int cube_normalcount = sizeof(cube_normals) / sizeof(cube_normals[0]);
static int cube_uvcount = sizeof(cube_uvs) / sizeof(cube_uvs[0]);
static int cube_coordindexcount = sizeof(cube_coordindices) / sizeof(cube_coordindices[0]);
static int cube_normalindexcount = sizeof(cube_normalindices) / sizeof(cube_normalindices[0]);
static int cube_uvindexcount = sizeof(cube_uvindices) / sizeof(cube_uvindices[0]);

static sAnmIndexedFaceList cube_facelist =
{
	cube_vertexcount,
	cube_vertices,
	cube_normalcount,
	cube_normals,
	0,
	NULL,
	cube_uvcount,
	cube_uvs,
	cube_coordindexcount,
	cube_coordindices,
	cube_normalindexcount,
	cube_normalindices,
	0,
	NULL,
	cube_uvindexcount,
	cube_uvindices,
	TRUE,
};

void CAnmBox::CreateCubeMesh(bool solid)
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	if (solid)
	{
		m_solidcubemesh = new CAnmMesh();
		m_solidcubemesh->AddFacesFromIndexedFaceList(pDevice, &cube_facelist, FALSE);
		m_solidcubemesh->SetDiffuseColor(color4::White);
	}
	else
	{
		m_hollowcubemesh = new CAnmMesh();
		m_hollowcubemesh->AddFacesFromIndexedFaceList(pDevice, &cube_facelist, FALSE);
		m_hollowcubemesh->SetDiffuseColor(color4::White);
		tAnmPrimitiveArray *pPrimitives = m_hollowcubemesh->GetPrimitives();
		int nprims = pPrimitives->size();
		for (int i = 0; i < nprims ; i++)
		{
			CAnmPrimitive *pPrim = (*pPrimitives)[i];
			pPrim->GenerateBackfaces(pDevice);
		}
	}
	// N.B.: need to add this is a global object; need to figure out mem management scheme;
	// is this a CAnmObject? who ref's it? who deletes the primitives?
}

void CAnmBox::Realize()
{
	if (m_solid)
	{
		if (m_solidcubemesh == NULL)
		{
			CreateCubeMesh(true);
		}

		assert(m_solidcubemesh);
		m_mesh = m_solidcubemesh;
	}
	else
	{
		if (m_hollowcubemesh == NULL)
		{
			CreateCubeMesh(false);
		}

		assert(m_hollowcubemesh);
		m_mesh = m_hollowcubemesh;
	}

	m_mesh->Ref();			// add a reference. N.B.: who gets rid of the original?

	CAnmGeometry::Realize();		// chain down
}

void CAnmBox::Update()
{
	if (StateDirty())
	{
		// set size values into the matrix
		SetModelScale(m_size / 2);

		m_bbox.SetCenterSize(Point3(0, 0, 0), m_size);

		ClearStateDirty();
	}
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_INHERITED_VALUE(CAnmBox, solid, CAnmGeometry);

DEFINE_VALUE(CAnmBox, size, eValuePoint3, Point3, GetSize, SetSize);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmBox)
VALUEID(CAnmBox, size),
VALUEID(CAnmBox, solid),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmBox, CAnmGeometry);

