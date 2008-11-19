/********************************************************************************
 * Flux
 *
 * File: anmtrianglestripset.cpp
 * Description: TriangleStripSet node
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
#include "anmtrianglestripset.h"

#include "anmcolor.h"
#include "anmcoordinate.h"
#include "anmnormal.h"
#include "anmtexturecoordinate.h"
#include "anmprimitive.h"
#include "anmmesh.h"
#include "anmdevice.h"
#include "anmworld.h"

CAnmTriangleStripSet::CAnmTriangleStripSet() : CAnmComposedGeometry()
{
	m_stripCount = new IntegerArray;
}

CAnmTriangleStripSet::~CAnmTriangleStripSet()
{
	SafeUnRef(m_stripCount);
}

void CAnmTriangleStripSet::Update()
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

void CAnmTriangleStripSet::CreateMesh()
{
	Point3Array *pCoordinates = NULL;
	Point3Array *pNormals = NULL;
	ColorArray *pColors = NULL;
	Point2Array *pTexCoords = NULL;
	int nVerts = 0; 
	int nNormals = 0;
	int nColors = 0;
	int nTexCoords = 0;
	int nStrips = 0;
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

	if (m_stripCount)
	{
		nStrips = m_stripCount->size();
	}

	m_mesh = new CAnmMesh();
	assert(m_mesh);

	int curvert = 0;
	for (int i = 0; i < nStrips; i++)
	{
		int vertexcount = (*m_stripCount)[i];
		
		CAnmTriangleStrip *pStrip = new CAnmTriangleStrip(pDevice);
		tAnmVertexArray *pStripVerts = pStrip->GetVertices();
		pStripVerts->reserve(vertexcount);

		for (int j = 0; j < vertexcount && curvert < nVerts; j++, curvert++)
		{
			Point3 p3 = (*pCoordinates)[curvert];
			sAnmVertex vert(p3);

			if (pColors && curvert < nColors)
				vert.SetDiffuseColor((*pColors)[curvert]);

			if (pNormals && curvert < nNormals)
				vert.SetNormal((*pNormals)[curvert]);

			if (pTexCoords && curvert < nTexCoords)
				vert.SetTexCoords((*pTexCoords)[curvert]);

			// add the vertex to the list
			pStripVerts->push_back(vert);
		}

		eAnmReturnStatus retval;

		// Generate the normals if not provided. N.B.: only works for perVertex
		if (pNormals == NULL)
		{
			retval = pStrip->GenerateNormals(0.f);
			if (ANMFAILED( retval ) )
			{		
				// N.B.: generate warning?
			}
		}

		// Generate the backfaces if the thing isn't solid
		if (!m_solid)
			pStrip->GenerateBackfaces(pDevice);

		// Add the strip
		m_mesh->AddPrimitive(pStrip);

		if (curvert >= nVerts)
			break;
	}
}

// CAnmComposedGeometry overrides
void CAnmTriangleStripSet::Realize()
{
	CreateMesh();
	CAnmComposedGeometry::Realize();		// chain down

	ClearStateDirty();				// we're built, so we're clean
}

// Accessors
void CAnmTriangleStripSet::SetStripCount(IntegerArray *pStripCount)
{
	assert(pStripCount != NULL);

	SafeUnRef(m_stripCount);
	m_stripCount = pStripCount;
	m_stripCount->Ref();

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmTriangleStripSet, stripCount_changed), &pStripCount);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmTriangleStripSet, ccw, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleStripSet, color, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleStripSet, coord, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleStripSet, colorPerVertex, CAnmComposedGeometry);
DEFINE_VALUE(CAnmTriangleStripSet, stripCount, eValueIntegerArray, IntegerArray*, 
			 GetStripCount, SetStripCount);
DEFINE_INHERITED_VALUE(CAnmTriangleStripSet, normal, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleStripSet, normalPerVertex, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleStripSet, solid, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleStripSet, texCoord, CAnmComposedGeometry);

DEFINE_INHERITED_METHOD(CAnmTriangleStripSet, set_color, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmTriangleStripSet, set_coord, CAnmComposedGeometry);
DEFINE_METHOD(CAnmTriangleStripSet, set_stripCount, eValueIntegerArray, IntegerArray*, SetStripCount);
DEFINE_INHERITED_METHOD(CAnmTriangleStripSet, set_normal, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmTriangleStripSet, set_texCoord, CAnmComposedGeometry);

DEFINE_INHERITED_CALLBACK(CAnmTriangleStripSet, color_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmTriangleStripSet, coord_changed, CAnmComposedGeometry);
DEFINE_CALLBACK(CAnmTriangleStripSet, stripCount_changed, eValueIntegerArray);
DEFINE_INHERITED_CALLBACK(CAnmTriangleStripSet, normal_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmTriangleStripSet, texCoord_changed, CAnmComposedGeometry);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmTriangleStripSet)
VALUEID(CAnmTriangleStripSet, ccw), 
VALUEID(CAnmTriangleStripSet, color), 
VALUEID(CAnmTriangleStripSet, colorPerVertex), 
VALUEID(CAnmTriangleStripSet, coord), 
VALUEID(CAnmTriangleStripSet, stripCount), 
VALUEID(CAnmTriangleStripSet, normal), 
VALUEID(CAnmTriangleStripSet, normalPerVertex),
VALUEID(CAnmTriangleStripSet, solid),
VALUEID(CAnmTriangleStripSet, texCoord),

METHODID(CAnmTriangleStripSet, set_color), 
METHODID(CAnmTriangleStripSet, set_coord), 
METHODID(CAnmTriangleStripSet, set_stripCount), 
METHODID(CAnmTriangleStripSet, set_normal), 
METHODID(CAnmTriangleStripSet, set_texCoord),

CALLBACKID(CAnmTriangleStripSet, color_changed),
CALLBACKID(CAnmTriangleStripSet, coord_changed),
CALLBACKID(CAnmTriangleStripSet, stripCount_changed),
CALLBACKID(CAnmTriangleStripSet, normal_changed),
CALLBACKID(CAnmTriangleStripSet, texCoord_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmTriangleStripSet, CAnmComposedGeometry);


