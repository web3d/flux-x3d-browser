/********************************************************************************
 * Flux
 *
 * File: anmindexedtriangleset.cpp
 * Description: IndexedTriangleSet node
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
#include "anmindexedtriangleset.h"

#include "anmcolor.h"
#include "anmcoordinate.h"
#include "anmnormal.h"
#include "anmtexturecoordinate.h"
#include "anmprimitive.h"
#include "anmmesh.h"
#include "anmdevice.h"
#include "anmworld.h"

CAnmIndexedTriangleSet::CAnmIndexedTriangleSet() : CAnmComposedGeometry()
{
	m_index = new IntegerArray;
}

CAnmIndexedTriangleSet::~CAnmIndexedTriangleSet()
{
	SafeUnRef(m_index);
}

void CAnmIndexedTriangleSet::Update()
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

void CAnmIndexedTriangleSet::CreateMesh()
{
	Point3Array *pCoordinates = NULL;
	Point3Array *pNormals = NULL;
	ColorArray *pColors = NULL;
	Point2Array *pTexCoords = NULL;
	int nVerts = 0; 
	int nNormals = 0;
	int nColors = 0;
	int nTexCoords = 0;
	int nIndices = m_index->size();
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

	if (pCoordNode == NULL || nIndices == 0)
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

	CAnmIndexedTriangleList *pTriangleList = new CAnmIndexedTriangleList(pDevice);
	tAnmVertexArray *pVerts = pTriangleList->GetVertices();

	for (int i = 0; i < nVerts; i++)
	{
		sAnmVertex v((*pCoordinates)[i]);
		
		if (pNormals && nNormals > i)
			v.SetNormal((*pNormals)[i]);

		if (pColors && nColors > i)
			v.SetDiffuseColor((*pColors)[i]);

		if (pTexCoords && nTexCoords > i)
			v.SetTexCoords((*pTexCoords)[i]);

		pVerts->push_back(v);
	}

	tAnmIndexArray *pIndices = pTriangleList->GetIndices();

	for (i = 0; i < nIndices / 3; i++)
	{
		pIndices->push_back((*m_index)[i * 3]);

		if (!m_ccw)
		{
			// re-order the second and third vertices
			pIndices->push_back((*m_index)[i * 3 + 2]);
			pIndices->push_back((*m_index)[i * 3 + 1]);
		}
		else
		{
			pIndices->push_back((*m_index)[i * 3 + 1]);
			pIndices->push_back((*m_index)[i * 3 + 2]);
		}
	}

	m_mesh = new CAnmMesh();
	assert(m_mesh);

	m_mesh->AddPrimitive(pTriangleList);

	// set a default color if we don't have one. do it before normal generation
	if (m_color == NULL)
		m_mesh->SetDiffuseColor(color4::White);

	eAnmReturnStatus retval;

	// Generate the normals if not provided
	if (pNormals == NULL)
	{
		retval = pTriangleList->GenerateNormals(m_normalPerVertex ? PI : 0.f);
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
void CAnmIndexedTriangleSet::Realize()
{
	CreateMesh();
	CAnmComposedGeometry::Realize();		// chain down

	ClearStateDirty();				// we're built, so we're clean
}

void CAnmIndexedTriangleSet::SetIndex(IntegerArray *pIndex)
{
	assert(pIndex != NULL);

	SafeUnRef(m_index);
	m_index = pIndex;
	m_index->Ref();

	SetDirtyBits(eAnmITSIndexDirty);
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmIndexedTriangleSet, ccw, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedTriangleSet, color, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedTriangleSet, coord, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedTriangleSet, colorPerVertex, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedTriangleSet, normal, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedTriangleSet, normalPerVertex, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedTriangleSet, solid, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedTriangleSet, texCoord, CAnmComposedGeometry);

DEFINE_VALUE(CAnmIndexedTriangleSet, index, eValueIntegerArray, IntegerArray*, 
			 GetIndex, SetIndex);

DEFINE_INHERITED_METHOD(CAnmIndexedTriangleSet, set_color, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmIndexedTriangleSet, set_coord, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmIndexedTriangleSet, set_normal, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmIndexedTriangleSet, set_texCoord, CAnmComposedGeometry);

DEFINE_METHOD(CAnmIndexedTriangleSet, set_index, eValueIntegerArray, IntegerArray*, SetIndex);

DEFINE_INHERITED_CALLBACK(CAnmIndexedTriangleSet, color_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmIndexedTriangleSet, coord_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmIndexedTriangleSet, normal_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmIndexedTriangleSet, texCoord_changed, CAnmComposedGeometry);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmIndexedTriangleSet)
VALUEID(CAnmIndexedTriangleSet, ccw), 
VALUEID(CAnmIndexedTriangleSet, color), 
VALUEID(CAnmIndexedTriangleSet, colorPerVertex), 
VALUEID(CAnmIndexedTriangleSet, coord), 
VALUEID(CAnmIndexedTriangleSet, index), 
VALUEID(CAnmIndexedTriangleSet, normal), 
VALUEID(CAnmIndexedTriangleSet, normalPerVertex),
VALUEID(CAnmIndexedTriangleSet, solid),
VALUEID(CAnmIndexedTriangleSet, texCoord),

METHODID(CAnmIndexedTriangleSet, set_color), 
METHODID(CAnmIndexedTriangleSet, set_coord), 
METHODID(CAnmIndexedTriangleSet, set_index), 
METHODID(CAnmIndexedTriangleSet, set_normal), 
METHODID(CAnmIndexedTriangleSet, set_texCoord),

CALLBACKID(CAnmIndexedTriangleSet, color_changed),
CALLBACKID(CAnmIndexedTriangleSet, coord_changed),
CALLBACKID(CAnmIndexedTriangleSet, normal_changed),
CALLBACKID(CAnmIndexedTriangleSet, texCoord_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmIndexedTriangleSet, CAnmComposedGeometry);


