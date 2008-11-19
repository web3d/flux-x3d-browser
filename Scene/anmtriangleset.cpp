/********************************************************************************
 * Flux
 *
 * File: anmtriangleset.cpp
 * Description: TriangleSet node
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
#include "anmtriangleset.h"

#include "anmcolor.h"
#include "anmcoordinate.h"
#include "anmnormal.h"
#include "anmtexturecoordinate.h"
#include "anmprimitive.h"
#include "anmmesh.h"
#include "anmdevice.h"
#include "anmworld.h"

CAnmTriangleSet::CAnmTriangleSet() : CAnmComposedGeometry()
{
}

CAnmTriangleSet::~CAnmTriangleSet()
{
}

void CAnmTriangleSet::Update()
{
	// N.B.: worth the extra check here?
	if (!TestDirtyBits(eAnmNodeStateDirty | eAnmNodeMatrixDirty))
		return;

	if (m_mesh == NULL)
	{
		// N.B.: I think we should issue a warning here, no?
		CreateMesh();

		// get the poly count
		if (m_mesh)
		{
			GenBoundingVolumes();
			m_polycount = m_mesh->GetPolyCount();

			CAnmWorld *pWorld = GetWorld();
			if (pWorld)
				// N.B.: this is just to update the polycount, duh
				pWorld->SetStateDirty();
		}

		ClearStateDirty();
		return;
	}

	tAnmPrimitiveArray *pPrimitives = m_mesh->GetPrimitives();
	if (pPrimitives->size() <= 0)
	{
		// N.B.: I think we should issue a warning here, no?
		return;
	}

	CAnmIndexedTriangleList *pTriangleList =
		(CAnmIndexedTriangleList *) (*pPrimitives)[0];

	if (TestDirtyBits(eAnmCGColorDirty))
	{
	}
	if (TestDirtyBits(eAnmCGCoordDirty))
	{
	}
	if (TestDirtyBits(eAnmCGNormalDirty))
	{
	}
	if (TestDirtyBits(eAnmCGTexCoordDirty))
	{
	}

	pPrimitives->clear();
	SafeDelete(pTriangleList);

	CreateMesh();
	GenBoundingVolumes();

	ClearStateDirty();
}

void CAnmTriangleSet::CreateMesh()
{
	Point3Array *pCoordinates = NULL;
	Point3Array *pNormals = NULL;
	ColorArray *pColors = NULL;
	Point2Array *pTexCoords = NULL;
	int nVerts = 0; 
	int nNormals = 0;
	int nColors = 0;
	int nTexCoords = 0;
	CAnmColor *pColorNode = GETIMPLEMENTATION(CAnmColor, m_color);
	CAnmCoordinate *pCoordNode = GETIMPLEMENTATION(CAnmCoordinate, m_coord);
	CAnmNormal *pNormalNode = GETIMPLEMENTATION(CAnmNormal, m_normal);
	CAnmTextureCoordinate *pTexCoordNode = GETIMPLEMENTATION(CAnmTextureCoordinate, m_texCoord);

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	if (pCoordNode == NULL)
	{
		// N.B.: I think we should issue a warning here, no?
		return;								// what else to do?
	}

	pCoordinates = pCoordNode->GetPoint();
	assert(pCoordinates);

	nVerts = pCoordinates->size();

	if (pNormalNode)
	{
		pNormals = pNormalNode->GetVector();
		nNormals = pNormals->size();
	}

	if (pColorNode)
	{
		pColors = pColorNode->GetColor();
		nColors = pColors->size();
	}

	if (pTexCoordNode)
	{
		pTexCoords = pTexCoordNode->GetPoint();
		nTexCoords = pTexCoords->size();
	}

	CAnmTriangleList *pTriangleList = new CAnmTriangleList(pDevice);
	tAnmVertexArray *pVerts = pTriangleList->GetVertices();

	for (int i = 0; i < nVerts / 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int index = i * 3 + j;
			// re-order clockwise face data
			if (!m_ccw && j > 0)
			{
				if (j == 1)
					index = i * 3 + 2;
				else if (j == 2)
					index = i * 3 + 1;
			}

			sAnmVertex v((*pCoordinates)[index]);
			
			if (pNormals && nNormals > index)
				v.SetNormal((*pNormals)[index]);

			if (pColors && nColors > index)
				v.SetDiffuseColor((*pColors)[index]);

			if (pTexCoords && nTexCoords > index)
				v.SetTexCoords((*pTexCoords)[index]);

			pVerts->push_back(v);
		}
	}

	m_mesh = new CAnmMesh();
	assert(m_mesh);

	m_mesh->AddPrimitive(pTriangleList);

	// set a default color if we don't have one. do it before normal generation
	if (m_color == NULL)
		m_mesh->SetDiffuseColor(color4::White);

	eAnmReturnStatus retval;

	// Generate the normals if not provided; just face normals for this node
	if (pNormals == NULL)
	{
		retval = pTriangleList->GenerateNormals(0.f);
		if (ANMFAILED( retval ) )
		{		
			// N.B.: generate warning?
		}
	}

	// Generate the backfaces if the thing isn't solid
	if (!m_solid)
		pTriangleList->GenerateBackfaces(pDevice);

	// Sort it if it's big
	pTriangleList->Sort();

	// try to buffer the vertex list in hardware; may or may not succeed but ok either way
	retval = pTriangleList->BufferVertices(pDevice);

}

// CAnmComposedGeometry overrides
void CAnmTriangleSet::Realize()
{
	CreateMesh();
	CAnmComposedGeometry::Realize();		// chain down

	ClearStateDirty();				// we're built, so we're clean
}



// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmTriangleSet, ccw, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleSet, color, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleSet, coord, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleSet, colorPerVertex, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleSet, normal, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleSet, normalPerVertex, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleSet, solid, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleSet, texCoord, CAnmComposedGeometry);

DEFINE_INHERITED_METHOD(CAnmTriangleSet, set_color, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmTriangleSet, set_coord, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmTriangleSet, set_normal, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmTriangleSet, set_texCoord, CAnmComposedGeometry);

DEFINE_INHERITED_CALLBACK(CAnmTriangleSet, color_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmTriangleSet, coord_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmTriangleSet, normal_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmTriangleSet, texCoord_changed, CAnmComposedGeometry);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmTriangleSet)
VALUEID(CAnmTriangleSet, ccw), 
VALUEID(CAnmTriangleSet, color), 
VALUEID(CAnmTriangleSet, colorPerVertex), 
VALUEID(CAnmTriangleSet, coord), 
VALUEID(CAnmTriangleSet, normal), 
VALUEID(CAnmTriangleSet, normalPerVertex),
VALUEID(CAnmTriangleSet, solid),
VALUEID(CAnmTriangleSet, texCoord),

METHODID(CAnmTriangleSet, set_color), 
METHODID(CAnmTriangleSet, set_coord), 
METHODID(CAnmTriangleSet, set_normal), 
METHODID(CAnmTriangleSet, set_texCoord),

CALLBACKID(CAnmTriangleSet, color_changed),
CALLBACKID(CAnmTriangleSet, coord_changed),
CALLBACKID(CAnmTriangleSet, normal_changed),
CALLBACKID(CAnmTriangleSet, texCoord_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmTriangleSet, CAnmComposedGeometry);


