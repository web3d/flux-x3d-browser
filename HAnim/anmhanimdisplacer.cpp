/********************************************************************************
 * Flux
 *
 * File: anmhanimdisplacer.cpp
 * Description: HAnim Displacer node
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
#include "anmhanimdisplacer.h"
#include "anmcoordinate.h"
#include "anmnormal.h"

CAnmHAnimDisplacer::CAnmHAnimDisplacer() : CAnmNode ()
{
	m_coordIndex = new IntegerArray;
	m_displacements = new Point3Array;
	m_name = new CAnmString(ANMHANIMDISPLACER_DEFAULT_NAME);
	m_weight = ANMHANIMDISPLACER_DEFAULT_WEIGHT;
	m_coordinate = NULL;
}

CAnmHAnimDisplacer::~CAnmHAnimDisplacer()
{
	SafeUnRef(m_coordIndex);
	SafeUnRef(m_displacements);
	SafeUnRef(m_name);
}

void CAnmHAnimDisplacer::Realize()
{
	CAnmNode::Realize();
}


void CAnmHAnimDisplacer::Update()
{
	// krv:  do nothing????
	ClearStateDirty();
}

void CAnmHAnimDisplacer::DisplaceSegment( Point3Array* pRawCoords )
{
	// Loop through the coord Index array, and calculate the local vertex displacement
	// The Update method displaces the verticies on the Segment geometry.
	//
	int nVertEffected = m_coordIndex->size();
	if( nVertEffected > 0 ) {
		int nVerts = pRawCoords->size();
		for( int iVertEffected=0; iVertEffected<nVertEffected; iVertEffected++ ) {
			int iVertIndex =   (*m_coordIndex)[iVertEffected];
			if( iVertIndex >= 0 && iVertIndex < nVerts ) {
				// Get the gloabal vertex displacment times weight.
				// Add this component to the Vertex in the Coord Array
				(*pRawCoords)[iVertIndex] += (((*m_displacements)[iVertEffected] ) ) * m_weight;
			}
		}
	}
}

void CAnmHAnimDisplacer::DisplaceSingleMesh(Point3Array* pRawCoords, const matrix4 &mat)
{
	// Loop through the coord Index array, and calculate the local vertex displacement
	// Use the supplied matric to transform the displacment into the space of the Joint
	// This Displace method diplaces the verts on the single mesh

	int nVertEffected = m_coordIndex->size();
	if( nVertEffected > 0 ) {
		int nVerts = pRawCoords->size();
		for( int iVertEffected=0; iVertEffected<nVertEffected; iVertEffected++ ) {
			int iVertIndex =   (*m_coordIndex)[iVertEffected];
			if( iVertIndex >= 0 && iVertIndex < nVerts ) {
				// Get the gloabal vertex displacment times weight.
				// Add this component to the Vertex in the Coord Array
				(*pRawCoords)[iVertIndex] += ( mat * ((*m_displacements)[iVertEffected] ) ) * m_weight;
			}
		}
	}
	ClearStateDirty();
}



void CAnmHAnimDisplacer::SetCoordinate(CAnmCoordinate *pCoordinate)
{
	SafeUnRef(m_coordinate);
	m_coordinate = pCoordinate;
	if (m_coordinate) {
		m_coordinate->Ref();
	}
}

void CAnmHAnimDisplacer::SetCoordIndex(IntegerArray *pCoordIndex)
{
	assert(pCoordIndex != NULL);

	SafeUnRef(m_coordIndex);
	m_coordIndex = pCoordIndex;
	m_coordIndex->Ref();

//	NotifyParentsOfChange();
	SetStateDirty();
	SetDirtyBits(eAnmDisplacerCoordIndexDirty);

	CallCallbacks(CALLBACKID(CAnmHAnimDisplacer, coordIndex_changed), &pCoordIndex);
}

void CAnmHAnimDisplacer::SetDisplacements(Point3Array *pDisplacements)
{
	assert(pDisplacements != NULL);

	SafeUnRef(m_displacements);
	m_displacements = pDisplacements;
	m_displacements->Ref();

//	NotifyParentsOfChange();
	SetStateDirty();
	SetDirtyBits(eAnmDisplacerDisplacementsDirty);

	CallCallbacks(CALLBACKID(CAnmHAnimDisplacer, displacements_changed), &pDisplacements);
}


void CAnmHAnimDisplacer::SetName(String name)
{
	SafeUnRef(m_name);
	m_name = name;
	m_name->Ref();

	// N.B.: why bother? who cares?
	// SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimDisplacer, name_changed), &name);
}

void CAnmHAnimDisplacer::SetWeight(Float weight)
{
	m_weight = weight;

	SetStateDirty();

	SetDirtyBits(eAnmDisplacerWeightDirty);

//	NotifyParentsOfChange();

	CallCallbacks(CALLBACKID(CAnmHAnimDisplacer, weight_changed), &weight);
}

DEFINE_VALUE(CAnmHAnimDisplacer, coordIndex, eValueIntegerArray, IntegerArray*, 
			 GetCoordIndex, SetCoordIndex);
DEFINE_VALUE(CAnmHAnimDisplacer, displacements, eValuePoint3Array, Point3Array*, 
			 GetDisplacements, SetDisplacements);
DEFINE_VALUE(CAnmHAnimDisplacer, name, eValueString, String, 
			 GetName, SetName);
DEFINE_VALUE(CAnmHAnimDisplacer, weight, eValueFloat, Float, 
			 GetWeight, SetWeight);


DEFINE_METHOD(CAnmHAnimDisplacer, set_coordIndex, eValueIntegerArray, IntegerArray*, SetCoordIndex);
DEFINE_METHOD(CAnmHAnimDisplacer, set_displacements, eValuePoint3Array, Point3Array*, SetDisplacements);
DEFINE_METHOD(CAnmHAnimDisplacer, set_name, eValueString, String, SetName);
DEFINE_METHOD(CAnmHAnimDisplacer, set_weight, eValueFloat, Float, SetWeight);

DEFINE_CALLBACK(CAnmHAnimDisplacer, coordIndex_changed, eValueIntegerArray);
DEFINE_CALLBACK(CAnmHAnimDisplacer, displacements_changed, eValuePoint3Array);
DEFINE_CALLBACK(CAnmHAnimDisplacer, name_changed, eValueString);
DEFINE_CALLBACK(CAnmHAnimDisplacer, weight_changed, eValueFloat);

BEGIN_DEFINE_NODE_MEMBERS(CAnmHAnimDisplacer)

VALUEID(CAnmHAnimDisplacer, coordIndex), 
VALUEID(CAnmHAnimDisplacer, displacements), 
VALUEID(CAnmHAnimDisplacer, name), 
VALUEID(CAnmHAnimDisplacer, weight), 

METHODID(CAnmHAnimDisplacer, set_coordIndex), 
METHODID(CAnmHAnimDisplacer, set_displacements), 
METHODID(CAnmHAnimDisplacer, set_name), 
METHODID(CAnmHAnimDisplacer, set_weight), 

CALLBACKID(CAnmHAnimDisplacer, coordIndex_changed),
CALLBACKID(CAnmHAnimDisplacer, displacements_changed),
CALLBACKID(CAnmHAnimDisplacer, name_changed),
CALLBACKID(CAnmHAnimDisplacer, weight_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmHAnimDisplacer, CAnmNode);
