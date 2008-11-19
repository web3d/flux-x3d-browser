/********************************************************************************
 * Flux
 *
 * File: anmtrianglefanset.cpp
 * Description: TriangleFanSet node
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
#include "anmtrianglefanset.h"

#include "anmcolor.h"
#include "anmcoordinate.h"
#include "anmnormal.h"
#include "anmtexturecoordinate.h"
#include "anmprimitive.h"
#include "anmmesh.h"
#include "anmdevice.h"
#include "anmworld.h"

CAnmTriangleFanSet::CAnmTriangleFanSet() : CAnmComposedGeometry()
{
	m_fanCount = new IntegerArray;
}

CAnmTriangleFanSet::~CAnmTriangleFanSet()
{
	SafeUnRef(m_fanCount);
}

void CAnmTriangleFanSet::Update()
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

void CAnmTriangleFanSet::CreateMesh()
{
	Point3Array *pCoordinates = NULL;
	Point3Array *pNormals = NULL;
	ColorArray *pColors = NULL;
	Point2Array *pTexCoords = NULL;
	int nVerts = 0; 
	int nNormals = 0;
	int nColors = 0;
	int nTexCoords = 0;
	int nFans = 0;
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

	if (m_fanCount)
	{
		nFans = m_fanCount->size();
	}

	m_mesh = new CAnmMesh();
	assert(m_mesh);

	int curvert = 0;
	for (int i = 0; i < nFans; i++)
	{
		int vertexcount = (*m_fanCount)[i];
		
		CAnmTriangleFan *pFan = new CAnmTriangleFan(pDevice);
		tAnmVertexArray *pFanVerts = pFan->GetVertices();
		pFanVerts->reserve(vertexcount);

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
			pFanVerts->push_back(vert);
		}

		eAnmReturnStatus retval;

		// Generate the normals if not provided. N.B.: only works for perVertex
		if (pNormals == NULL)
		{
			retval = pFan->GenerateNormals(0.f);
			if (ANMFAILED( retval ) )
			{		
				// N.B.: generate warning?
			}
		}

		// Generate the backfaces if the thing isn't solid
		if (!m_solid)
			pFan->GenerateBackfaces(pDevice);

		// Add the fan
		m_mesh->AddPrimitive(pFan);

		if (curvert >= nVerts)
			break;
	}
}

// CAnmComposedGeometry overrides
void CAnmTriangleFanSet::Realize()
{
	CreateMesh();
	CAnmComposedGeometry::Realize();		// chain down

	ClearStateDirty();				// we're built, so we're clean
}

// Accessors
void CAnmTriangleFanSet::SetFanCount(IntegerArray *pFanCount)
{
	assert(pFanCount != NULL);

	SafeUnRef(m_fanCount);
	m_fanCount = pFanCount;
	m_fanCount->Ref();

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmTriangleFanSet, fanCount_changed), &pFanCount);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmTriangleFanSet, ccw, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleFanSet, color, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleFanSet, coord, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleFanSet, colorPerVertex, CAnmComposedGeometry);
DEFINE_VALUE(CAnmTriangleFanSet, fanCount, eValueIntegerArray, IntegerArray*, 
			 GetFanCount, SetFanCount);
DEFINE_INHERITED_VALUE(CAnmTriangleFanSet, normal, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleFanSet, normalPerVertex, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleFanSet, solid, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmTriangleFanSet, texCoord, CAnmComposedGeometry);

DEFINE_INHERITED_METHOD(CAnmTriangleFanSet, set_color, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmTriangleFanSet, set_coord, CAnmComposedGeometry);
DEFINE_METHOD(CAnmTriangleFanSet, set_fanCount, eValueIntegerArray, IntegerArray*, SetFanCount);
DEFINE_INHERITED_METHOD(CAnmTriangleFanSet, set_normal, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmTriangleFanSet, set_texCoord, CAnmComposedGeometry);

DEFINE_INHERITED_CALLBACK(CAnmTriangleFanSet, color_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmTriangleFanSet, coord_changed, CAnmComposedGeometry);
DEFINE_CALLBACK(CAnmTriangleFanSet, fanCount_changed, eValueIntegerArray);
DEFINE_INHERITED_CALLBACK(CAnmTriangleFanSet, normal_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmTriangleFanSet, texCoord_changed, CAnmComposedGeometry);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmTriangleFanSet)
VALUEID(CAnmTriangleFanSet, ccw), 
VALUEID(CAnmTriangleFanSet, color), 
VALUEID(CAnmTriangleFanSet, colorPerVertex), 
VALUEID(CAnmTriangleFanSet, coord), 
VALUEID(CAnmTriangleFanSet, fanCount), 
VALUEID(CAnmTriangleFanSet, normal), 
VALUEID(CAnmTriangleFanSet, normalPerVertex),
VALUEID(CAnmTriangleFanSet, solid),
VALUEID(CAnmTriangleFanSet, texCoord),

METHODID(CAnmTriangleFanSet, set_color), 
METHODID(CAnmTriangleFanSet, set_coord), 
METHODID(CAnmTriangleFanSet, set_fanCount), 
METHODID(CAnmTriangleFanSet, set_normal), 
METHODID(CAnmTriangleFanSet, set_texCoord),

CALLBACKID(CAnmTriangleFanSet, color_changed),
CALLBACKID(CAnmTriangleFanSet, coord_changed),
CALLBACKID(CAnmTriangleFanSet, fanCount_changed),
CALLBACKID(CAnmTriangleFanSet, normal_changed),
CALLBACKID(CAnmTriangleFanSet, texCoord_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmTriangleFanSet, CAnmComposedGeometry);


