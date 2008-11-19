/********************************************************************************
 * Flux
 *
 * File: anmpointset.cpp
 * Description: PointSet node
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
#include "anmpointset.h"
#include "application.h"
#include "anmcolor.h"
#include "anmcoordinate.h"
#include "anmdevice.h"
#include "anmprimitive.h"
#include "anmmesh.h"
#include "anmworld.h"

CAnmPointSet::CAnmPointSet() : 
	
	CAnmGeometry()
{
	m_color = NULL;
	m_coord = NULL;
}

CAnmPointSet::~CAnmPointSet()
{
}

eAnmReturnStatus CAnmPointSet::AddChild(CAnmNode *pChild)
{
	CAnmGroup::AddChild(pChild);

	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmColor)))
		m_color = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_coord = pChild;

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmPointSet::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmColor)))
		m_color = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_coord = NULL;

	CAnmGroup::RemoveChild(pChild);

	return eAnmReturnAllGood;
}

void CAnmPointSet::ChildChanged(CAnmNode *pChild)
{
	if (pChild == m_color)
	{
		SetDirtyBits(eAnmPSColorDirty);
	}
	else if (pChild == m_coord)
	{
		SetDirtyBits(eAnmPSCoordDirty);
	}

	SetStateDirty();
}

void CAnmPointSet::Update()
{
	if (!TestDirtyBits(eAnmNodeStateDirty | eAnmNodeMatrixDirty))
		return;

	assert(m_mesh);

	tAnmPrimitiveArray *pPrimitives = m_mesh->GetPrimitives();
	CAnmPointList *pPointList =
		(CAnmPointList *) (*pPrimitives)[0];

	if (TestDirtyBits(eAnmPSColorDirty))
	{
	}
	if (TestDirtyBits(eAnmPSCoordDirty))
	{
		CAnmCoordinate *pCoord = GETIMPLEMENTATION(CAnmCoordinate, m_coord);

		pCoord->Update();
		pPointList->SetCoordinates(pCoord->GetPoint());
	}

	ClearStateDirty();
}

void CAnmPointSet::CreateMesh()
{
	Point3Array *pCoordinates = NULL;
	ColorArray *pColorValues = NULL;
	int nVerts = 0; 
	int nColors = 0;
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

	if (nVerts <= 0)
		return;

	if (pColorNode)
	{
		pColorValues = pColorNode->GetColor();
		nColors = pColorValues->size();
	}

	// Create a new empty mesh
	m_mesh = new CAnmMesh();
	
	assert(m_mesh);

	// Populate it with points
	int nVertsRemaining = nVerts;
	Point3 *pVertices = &(*pCoordinates)[0];
	Color *pColors = pColorValues ? &(*pColorValues)[0] : NULL;

	// N.B.: break a potentially large point set up into chunks; works best at 2^^15... bigger
	// and it seems to lose points
	int chunksize = ANMMAXVERTICES / 2;

	while (nVertsRemaining > 0)
	{
		int nVerts = min(nVertsRemaining, chunksize);

		CAnmPointList *pPointList = new CAnmPointList(pDevice);
		tAnmUnlitVertexArray *pPointListVertices = pPointList->GetUnlitVertices();
		pPointListVertices->reserve(nVerts);

		// copy the vertices and colors into D3D format
		for (int i = 0; i < nVerts; i++)
		{
			sAnmUnlitVertex vert(pVertices[i]);

			if (pColors && i < nColors)
				vert.SetDiffuseColor(pColors[i]);
			// else leave it black

			pPointListVertices->push_back(vert);

		}

		m_mesh->AddPrimitive(pPointList);

		nVertsRemaining -= chunksize;
		pVertices += chunksize;
	}
}

// CAnmGeometry overrides
void CAnmPointSet::Realize()
{
	CreateMesh();
	CAnmGeometry::Realize();		// chain down
}

eAnmRenderOptions CAnmPointSet::GetRenderOptions()
{
	if (m_color)
		return eAnmRenderOptionsUseVertexForEmissive;
	else
		return eAnmRenderOptionsNone;
}


// Accessors

void CAnmPointSet::SetColor(CAnmNode *pColor)
{
	if (m_color == pColor)
		return;

	if (m_color)
		RemoveChild(m_color);

	if (pColor == NULL)
		return;

	AddChild(pColor);
	m_color = pColor;

	SetDirtyBits(eAnmPSColorDirty);

	CallCallbacks(CALLBACKID(CAnmPointSet, color_changed), &m_color);
}


void CAnmPointSet::SetCoord(CAnmNode *pCoord)
{
	if (m_coord == pCoord)
		return;

	if (m_coord)
		RemoveChild(m_coord);

	if (pCoord == NULL)
		return;

	AddChild(pCoord);
	m_coord = pCoord;

	SetDirtyBits(eAnmPSCoordDirty);

	CallCallbacks(CALLBACKID(CAnmPointSet, coord_changed), &m_coord);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmPointSet, color, eValueNode, CAnmNode*, 
			 GetColor, SetColor);
DEFINE_VALUE(CAnmPointSet, coord, eValueNode, CAnmNode*, 
			 GetCoord, SetCoord);

DEFINE_METHOD(CAnmPointSet, set_color, eValueNode, CAnmNode*, SetColor);
DEFINE_METHOD(CAnmPointSet, set_coord, eValueNode, CAnmNode*, SetCoord);


DEFINE_CALLBACK(CAnmPointSet, color_changed, eValueNode);
DEFINE_CALLBACK(CAnmPointSet, coord_changed, eValueNode);



// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmPointSet)
VALUEID(CAnmPointSet, color), 
VALUEID(CAnmPointSet, coord), 

METHODID(CAnmPointSet, set_color), 
METHODID(CAnmPointSet, set_coord), 

CALLBACKID(CAnmPointSet, color_changed),
CALLBACKID(CAnmPointSet, coord_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmPointSet, CAnmGeometry);


