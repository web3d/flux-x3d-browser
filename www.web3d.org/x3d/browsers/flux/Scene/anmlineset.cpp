/********************************************************************************
 * Flux
 *
 * File: anmlineset.cpp
 * Description: LineSet node
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
#include "anmlineset.h"
#include "application.h"
#include "anmcolor.h"
#include "anmcoordinate.h"
#include "anmdevice.h"
#include "anmprimitive.h"
#include "anmmesh.h"
#include "anmworld.h"

CAnmLineSet::CAnmLineSet() : CAnmGeometry()
{
	m_color = NULL;
	m_coord = NULL;

	m_lineCount = new IntegerArray;
}


CAnmLineSet::~CAnmLineSet()
{
	SafeUnRef(m_lineCount);
}

eAnmReturnStatus CAnmLineSet::AddChild(CAnmNode *pChild)
{
	CAnmGroup::AddChild(pChild);

	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmColor)))
		m_color = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_coord = pChild;

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmLineSet::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmColor)))
		m_color = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_coord = NULL;

	CAnmGroup::RemoveChild(pChild);

	return eAnmReturnAllGood;
}

void CAnmLineSet::ChildChanged(CAnmNode *pChild)
{
	if (pChild == m_color)
	{
		SetDirtyBits(eAnmLSColorDirty);
	}
	else if (pChild == m_coord)
	{
		SetDirtyBits(eAnmLSCoordDirty);
	}

	SetStateDirty();
}

void CAnmLineSet::Update()
{
	// N.B.: worth the extra check here?
	if (!TestDirtyBits(eAnmNodeStateDirty | eAnmNodeMatrixDirty))
		return;

	if (m_mesh == NULL)
	{
		CreateMesh();

		ClearStateDirty();
		return;
	}

	// N.B.: can't do this yet, because our mesh is a list of polylines
	// see CAnmMesh::AddLinesFromIndexedPolyLineList(); only way
	// would be to rebuild the mesh

	if (TestDirtyBits(eAnmLSColorDirty))
	{
	}
	if (TestDirtyBits(eAnmLSCoordDirty))
	{
	}
	if (TestDirtyBits(eAnmLSLineCountDirty))
	{
	}

	ClearStateDirty();
}

void CAnmLineSet::CreateMesh()
{
	Point3Array *pCoordinates = NULL;
	ColorArray *pColorValues = NULL;
	int nVerts = 0; 
	int nColors = 0;
	int nLines  = 0;
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

	// Sanity check
	if (pCoordNode == NULL)
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
	}

	if (m_lineCount)
		nLines = m_lineCount->size();

	// Create an empty mesh
	m_mesh = new CAnmMesh();
	
	assert(m_mesh);

	// Populate it with polylines
	Point3 *pVertices = &(*pCoordinates)[0];
	int *pLines = nLines ? &(*m_lineCount)[0] : NULL;
	Color *pColors  = pColorValues ? &(*pColorValues)[0] : NULL;
	int curvert = 0;
	for (int i = 0; i < nLines; )
	{
		int vertexcount = pLines[i];		
		CAnmPolyLine *pPolyLine = new CAnmPolyLine(pDevice);
		tAnmUnlitVertexArray *pPolyLineVertices = pPolyLine->GetUnlitVertices();
		pPolyLineVertices->reserve(vertexcount);

		for (int j = 0; j < vertexcount && curvert < nVerts; j++, curvert++)
		{
			Point3 p3 = pVertices[curvert];
			sAnmUnlitVertex vert(p3);

			if (pColors && curvert < nColors)
				vert.SetDiffuseColor(pColors[curvert]);

			// add the vertex to the list
			pPolyLineVertices->push_back(vert);
		}

		m_mesh->AddPrimitive(pPolyLine);

		if (curvert >= nVerts)
			break;
	}
}

// CAnmGeometry overrides
void CAnmLineSet::Realize()
{
	CreateMesh();
	CAnmGeometry::Realize();		// chain down
}


eAnmRenderOptions CAnmLineSet::GetRenderOptions()
{
	if (m_color)
		return eAnmRenderOptionsUseVertexForEmissive;
	else
		return eAnmRenderOptionsNone;
}


// Accessors

void CAnmLineSet::SetColor(CAnmNode *pColor)
{
	if (m_color == pColor)
		return;

	if (m_color)
		RemoveChild(m_color);

	if (pColor == NULL)
		return;

	AddChild(pColor);
	m_color = pColor;

	SetDirtyBits(eAnmLSColorDirty);

	CallCallbacks(CALLBACKID(CAnmLineSet, color_changed), &m_color);
}

void CAnmLineSet::SetCoord(CAnmNode *pCoord)
{
	if (m_coord == pCoord)
		return;

	if (m_coord)
		RemoveChild(m_coord);

	if (pCoord == NULL)
		return;

	AddChild(pCoord);
	m_coord = pCoord;

	SetDirtyBits(eAnmLSCoordDirty);

	CallCallbacks(CALLBACKID(CAnmLineSet, coord_changed), &m_coord);
}

void CAnmLineSet::SetLineCount(IntegerArray *pLineCount)
{
	assert(pLineCount != NULL);

	SafeUnRef(m_lineCount);
	m_lineCount = pLineCount;
	m_lineCount->Ref();

	SetDirtyBits(eAnmLSLineCountDirty);

	CallCallbacks(CALLBACKID(CAnmLineSet, lineCount_changed), &pLineCount);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmLineSet, color, eValueNode, CAnmNode*, 
			 GetColor, SetColor);
DEFINE_VALUE(CAnmLineSet, coord, eValueNode, CAnmNode*, 
			 GetCoord, SetCoord);
DEFINE_VALUE(CAnmLineSet, lineCount, eValueIntegerArray, IntegerArray*, 
			 GetLineCount, SetLineCount);

DEFINE_METHOD(CAnmLineSet, set_color, eValueNode, CAnmNode*, SetColor);
DEFINE_METHOD(CAnmLineSet, set_coord, eValueNode, CAnmNode*, SetCoord);
DEFINE_METHOD(CAnmLineSet, set_lineCount, eValueIntegerArray, IntegerArray*, SetLineCount);


DEFINE_CALLBACK(CAnmLineSet, color_changed, eValueNode);
DEFINE_CALLBACK(CAnmLineSet, coord_changed, eValueNode);
DEFINE_CALLBACK(CAnmLineSet, lineCount_changed, eValueIntegerArray);




// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmLineSet)
VALUEID(CAnmLineSet, color), 
VALUEID(CAnmLineSet, coord), 
VALUEID(CAnmLineSet, lineCount), 

METHODID(CAnmLineSet, set_color), 
METHODID(CAnmLineSet, set_coord), 
METHODID(CAnmLineSet, set_lineCount), 

CALLBACKID(CAnmLineSet, color_changed),
CALLBACKID(CAnmLineSet, coord_changed),
CALLBACKID(CAnmLineSet, lineCount_changed),


END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmLineSet, CAnmGeometry);


