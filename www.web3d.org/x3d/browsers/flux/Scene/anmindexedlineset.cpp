/********************************************************************************
 * Flux
 *
 * File: anmindexedlineset.cpp
 * Description: IndexedLineSet node
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
#include "anmindexedlineset.h"
#include "application.h"
#include "anmcolor.h"
#include "anmcoordinate.h"
#include "anmdevice.h"
#include "anmprimitive.h"
#include "anmmesh.h"
#include "anmworld.h"

CAnmIndexedLineSet::CAnmIndexedLineSet() : CAnmGeometry(),
	m_colorPerVertex(ANMILS_DEFAULT_COLORPERVERTEX)
{
	m_color = NULL;
	m_coord = NULL;

	m_colorIndex = new IntegerArray;
	m_coordIndex = new IntegerArray;
}


CAnmIndexedLineSet::~CAnmIndexedLineSet()
{
	SafeUnRef(m_colorIndex);
	SafeUnRef(m_coordIndex);
}

eAnmReturnStatus CAnmIndexedLineSet::AddChild(CAnmNode *pChild)
{
	CAnmGroup::AddChild(pChild);

	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmColor)))
		m_color = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_coord = pChild;

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmIndexedLineSet::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmColor)))
		m_color = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_coord = NULL;

	CAnmGroup::RemoveChild(pChild);

	return eAnmReturnAllGood;
}

void CAnmIndexedLineSet::ChildChanged(CAnmNode *pChild)
{
	if (pChild == m_color)
	{
		SetDirtyBits(eAnmILSColorDirty);
	}
	else if (pChild == m_coord)
	{
		SetDirtyBits(eAnmILSCoordDirty);
	}

	SetStateDirty();
}

void CAnmIndexedLineSet::Update()
{
	// N.B.: worth the extra check here?
	if (!TestDirtyBits(eAnmNodeStateDirty | eAnmNodeMatrixDirty))
		return;


	// N.B.: can't do this yet, because our mesh is a list of polylines
	// see CAnmMesh::AddLinesFromIndexedPolyLineList(); only way
	// would be to rebuild the mesh

	if (TestDirtyBits(eAnmILSColorDirty))
	{
	}
	if (TestDirtyBits(eAnmILSColorIndexDirty))
	{
	}
	if (TestDirtyBits(eAnmILSColorPerVertexDirty))
	{
	}
	if (TestDirtyBits(eAnmILSCoordDirty))
	{
	}
	if (TestDirtyBits(eAnmILSCoordIndexDirty))
	{
	}

	// chuck the old mesh
	SafeUnRef(m_mesh);

	// build a new one
	CreateMesh();

	// update the scene poly count
	if (m_mesh)
	{
		GenBoundingVolumes();
		m_polycount = m_mesh->GetPolyCount();

		CAnmWorld *pWorld = GetWorld();
		if (pWorld)
			// N.B.: this is just to update the polycount, duh
			pWorld->SetStateDirty();
	}

	// clear state flag
	ClearStateDirty();
}

void CAnmIndexedLineSet::CreateMesh()
{
	Point3Array *pCoordinates = NULL;
	ColorArray *pColorValues = NULL;
	int nVerts = 0; 
	int nColors = 0;
	int nIndices = m_coordIndex->size();
	int nColorIndices = m_colorIndex->size();
	int *pColorIndices = NULL;
	CAnmColor *pColorNode = GETIMPLEMENTATION(CAnmColor, m_color);
	CAnmCoordinate *pCoordNode = GETIMPLEMENTATION(CAnmCoordinate, m_coord);

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	// Do sanity checks
	if (pCoordNode == NULL || nIndices == 0)
	{
		return;								// what else to do?
	}

	// Do some setup
	pCoordinates = pCoordNode->GetPoint();
	nVerts = pCoordinates->size();

	if (pColorNode)
	{
		pColorValues = pColorNode->GetColor();
		nColors = pColorValues->size();
		if (nColorIndices > 0)
		{
			pColorIndices = &(*m_colorIndex)[0];
		}
		else if (m_colorPerVertex)
		{
			nColorIndices = nIndices;
			pColorIndices = &(*m_coordIndex)[0];
		}
	}

	// Create a new empty mesh
	m_mesh = new CAnmMesh();
	
	assert(m_mesh);

	// Populate it with polylines
	Point3 *pVertices = &(*pCoordinates)[0];
	Color *pColors = pColorValues ? &(*pColorValues)[0] : NULL;
	int *pindices = &(*m_coordIndex)[0];
	int *ppolyline = pindices;
	int polylineindex = 0;
	for (int i = 0; i < nIndices; )
	{
		int vertexcount = 0;		
		CAnmPolyLine *pPolyLine = new CAnmPolyLine(pDevice);
		tAnmUnlitVertexArray *pPolyLineVertices = pPolyLine->GetUnlitVertices();

		while(pindices[i] != -1 && i < nIndices)
		{
			vertexcount++;
			i++;

			//N.B.: should issue a warning if we never find a -1
		}

		pPolyLineVertices->reserve(vertexcount);

		// make the polyline
		for (int j = 0; j < vertexcount; j++)
		{
			int start = i - vertexcount;

			// get the coordinate info
			int vindex = ppolyline[j];
			Point3 p3 = pVertices[vindex];
			sAnmUnlitVertex vert(p3);

			// get colors
			if (pColors)
			{
				int cindex;

				if (pColorIndices != NULL)
				{
					if (m_colorPerVertex)
					{
						cindex = pColorIndices[start + j];
					}
					else
						cindex = pColorIndices[polylineindex];
				}
				else
					cindex = polylineindex;

				if (cindex >= 0 && cindex < nColors)
					vert.SetDiffuseColor(pColors[cindex]);
			}

			// add the vertex to the list
			pPolyLineVertices->push_back(vert);
		}

		m_mesh->AddPrimitive(pPolyLine);

		ppolyline = &pindices[++i];
		polylineindex++;
	}
}

// CAnmGeometry overrides
void CAnmIndexedLineSet::Realize()
{
	CreateMesh();
	CAnmGeometry::Realize();		// chain down
}


eAnmRenderOptions CAnmIndexedLineSet::GetRenderOptions()
{
	if (m_color)
		return eAnmRenderOptionsUseVertexForEmissive;
	else
		return eAnmRenderOptionsNone;
}


// Accessors

void CAnmIndexedLineSet::SetColor(CAnmNode *pColor)
{
	if (m_color == pColor)
		return;

	if (m_color)
		RemoveChild(m_color);

	if (pColor == NULL)
		return;

	AddChild(pColor);
	m_color = pColor;

	SetDirtyBits(eAnmILSColorDirty);

	CallCallbacks(CALLBACKID(CAnmIndexedLineSet, color_changed), &m_color);
}

void CAnmIndexedLineSet::SetColorIndices(IntegerArray *pColorIndices)
{
	assert(pColorIndices != NULL);

	SafeUnRef(m_colorIndex);
	m_colorIndex = pColorIndices;
	m_colorIndex->Ref();

	SetDirtyBits(eAnmILSColorIndexDirty);

	CallCallbacks(CALLBACKID(CAnmIndexedLineSet, colorIndex_changed), &pColorIndices);
}

void CAnmIndexedLineSet::SetColorPerVertex(Boolean colorPerVertex)
{
	m_colorPerVertex = colorPerVertex;

	SetDirtyBits(eAnmILSColorPerVertexDirty);

	CallCallbacks(CALLBACKID(CAnmIndexedLineSet, colorPerVertex_changed), &m_colorPerVertex);
}

void CAnmIndexedLineSet::SetCoord(CAnmNode *pCoord)
{
	if (m_coord == pCoord)
		return;

	if (m_coord)
		RemoveChild(m_coord);

	if (pCoord == NULL)
		return;

	AddChild(pCoord);
	m_coord = pCoord;

	SetDirtyBits(eAnmILSCoordDirty);

	CallCallbacks(CALLBACKID(CAnmIndexedLineSet, coord_changed), &m_coord);
}

void CAnmIndexedLineSet::SetCoordIndices(IntegerArray *pCoordIndices)
{
	assert(pCoordIndices != NULL);

	SafeUnRef(m_coordIndex);
	m_coordIndex = pCoordIndices;
	m_coordIndex->Ref();

	SetDirtyBits(eAnmILSCoordIndexDirty);

	CallCallbacks(CALLBACKID(CAnmIndexedLineSet, coordIndex_changed), &pCoordIndices);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmIndexedLineSet, color, eValueNode, CAnmNode*, 
			 GetColor, SetColor);
DEFINE_VALUE(CAnmIndexedLineSet, colorIndex, eValueIntegerArray, IntegerArray*, 
			 GetColorIndices, SetColorIndices);
DEFINE_VALUE(CAnmIndexedLineSet, colorPerVertex, eValueBoolean, Boolean, 
			 GetColorPerVertex, SetColorPerVertex);
DEFINE_VALUE(CAnmIndexedLineSet, coord, eValueNode, CAnmNode*, 
			 GetCoord, SetCoord);
DEFINE_VALUE(CAnmIndexedLineSet, coordIndex, eValueIntegerArray, IntegerArray*, 
			 GetCoordIndices, SetCoordIndices);

DEFINE_METHOD(CAnmIndexedLineSet, set_color, eValueNode, CAnmNode*, SetColor);
DEFINE_METHOD(CAnmIndexedLineSet, set_colorIndex, eValueIntegerArray, IntegerArray*, SetColorIndices);
DEFINE_METHOD(CAnmIndexedLineSet, set_colorPerVertex, eValueBoolean, Boolean, SetColorPerVertex);
DEFINE_METHOD(CAnmIndexedLineSet, set_coord, eValueNode, CAnmNode*, SetCoord);
DEFINE_METHOD(CAnmIndexedLineSet, set_coordIndex, eValueIntegerArray, IntegerArray*, SetCoordIndices);


DEFINE_CALLBACK(CAnmIndexedLineSet, color_changed, eValueNode);
DEFINE_CALLBACK(CAnmIndexedLineSet, colorIndex_changed, eValueIntegerArray);
DEFINE_CALLBACK(CAnmIndexedLineSet, colorPerVertex_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmIndexedLineSet, coord_changed, eValueNode);
DEFINE_CALLBACK(CAnmIndexedLineSet, coordIndex_changed, eValueIntegerArray);




// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmIndexedLineSet)
VALUEID(CAnmIndexedLineSet, color), 
VALUEID(CAnmIndexedLineSet, colorIndex), 
VALUEID(CAnmIndexedLineSet, colorPerVertex), 
VALUEID(CAnmIndexedLineSet, coord), 
VALUEID(CAnmIndexedLineSet, coordIndex), 

METHODID(CAnmIndexedLineSet, set_color), 
METHODID(CAnmIndexedLineSet, set_colorIndex), 
METHODID(CAnmIndexedLineSet, set_colorPerVertex), 
METHODID(CAnmIndexedLineSet, set_coord), 
METHODID(CAnmIndexedLineSet, set_coordIndex), 

CALLBACKID(CAnmIndexedLineSet, color_changed),
CALLBACKID(CAnmIndexedLineSet, colorIndex_changed),
CALLBACKID(CAnmIndexedLineSet, colorPerVertex_changed),
CALLBACKID(CAnmIndexedLineSet, coord_changed),
CALLBACKID(CAnmIndexedLineSet, coordIndex_changed),


END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmIndexedLineSet, CAnmGeometry);


