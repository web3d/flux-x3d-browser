/********************************************************************************
 * Flux
 *
 * File: anmhanimsegment.cpp
 * Description: HAnim Segment node
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
#include "anmhanimsegment.h"
#include "anmcoordinate.h"
#include "anmhanimdisplacer.h"
#include "anmupdatevisitor.h"
	
CAnmHAnimSegment::CAnmHAnimSegment() : CAnmGroup()
{
	m_centerOfMass = ANMHANIMSEGMENT_DEFAULT_CENTEROFMASS;
	m_coord = NULL;
	m_displacers = new NodeArray;
	m_mass = ANMHANIMSEGMENT_DEFAULT_MASS;
	m_name = new CAnmString(ANMHANIMSEGMENT_DEFAULT_NAME);

	m_momentsOfInertia = new FloatArray;
//	m_momentsOfInertia->reserve(9);

	// $$$ Keith - Spec says momentsOfInertia is a 3x3 with default value of all 0's?
	// krv: you might want to put 1s on the diagonal, but this is NEVER used, so why bother 
//	for (int i = 0; i < 9; i++)
//	{
//		m_momentsOfInertia->push_back(0.f);
//	}

}
 
CAnmHAnimSegment::~CAnmHAnimSegment()
{
	SafeUnRef(m_coord);
	SafeUnRef(m_displacers);
	SafeUnRef(m_momentsOfInertia);
	SafeUnRef(m_name);
}

// Methods
void CAnmHAnimSegment::Update()
{

	if (TestDirtyBits(eAnmSegmentCoordDirty ) ) {
		InitializeDisplacedSegment();
	}


	if (TestDirtyBits(eAnmSegmentCoordDirty) ||
		TestDirtyBits(eAnmSegmentDisplacersDirty))
	{
		// first check if we have displacers and a coord field
		//
		int sz = m_displacers->size();
		if( m_coord && sz > 0 ) {

			// Get the coord array out of the coord field
			//
			CAnmNode * pImp = m_coord->GetImplementationNode();
			if ( pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
			{
				CAnmCoordinate *pCoord = (CAnmCoordinate *) pImp;
				Point3Array* pRawCoords = pCoord->GetPoint();
				int nCoords = pRawCoords->size();

				// Make sure the size of the base mesh matches the coord array.
				//
				if( nCoords == m_baseMesh.size() ) {
					// Copy the base mesh into the coord array.
					//
					for( int i = 0; i < nCoords; i++ ) {
						(*pRawCoords)[i] = m_baseMesh[i];
					}

					for ( i = 0; i < sz; i++)
					{
						CAnmNode *pNode = (*m_displacers)[i];
						CAnmNode *pImp = pNode->GetImplementationNode();

						if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmHAnimDisplacer)))
						{
							CAnmHAnimDisplacer *pDisplacer = (CAnmHAnimDisplacer *) pImp;
							pDisplacer->DisplaceSegment( pRawCoords );
						}
					}
					// Tell the Coord that is is dirty, so the low level graphics data can get generated
					//
					pCoord->FlagUpdate();
				}
			}
		}
	}

	ClearStateDirty();
}


void CAnmHAnimSegment::Realize()
{
	CAnmGroup::Realize();

	// Initialize the skin if it's there
	InitializeDisplacedSegment();
}

void CAnmHAnimSegment::InitializeDisplacedSegment()
{
	// Dont need to create a copy of the segment cood array if we have no displacers
	// krv , right, if we have displacers, we dont need to do this.
	m_baseMesh.clear();
	if (m_coord && m_displacers->size())
	{
		// Get the coord array, and copy the coords into the basemesh.
		//
		CAnmNode * pImp = m_coord->GetImplementationNode();
		if ( pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		{
			CAnmCoordinate *pCoord = (CAnmCoordinate *) pImp;
			Point3Array* pRawCoords = pCoord->GetPoint();
			int nCoords = pRawCoords->size();

			for( int i = 0; i < nCoords; i++ ) {
				m_baseMesh.push_back((*pRawCoords)[i]);
			}
		}
	}
}


// Accessors
void CAnmHAnimSegment::SetCenterOfMass(point3 p)
{
	m_centerOfMass = p;

	// $$$ Keith - Here's where we need to figure out traversal
	// do nothing.  never used.

	CallCallbacks(CALLBACKID(CAnmHAnimSegment, centerOfMass_changed), &p);
}


void CAnmHAnimSegment::SetCoord(CAnmNode *pCoord)
{
	SafeUnRef(m_coord);
	m_coord = pCoord;
	if (m_coord)
		m_coord->Ref();

	SetStateDirty();
	SetDirtyBits(eAnmSegmentCoordDirty);

	CallCallbacks(CALLBACKID(CAnmHAnimSegment, coord_changed), &pCoord);
}


void CAnmHAnimSegment::HandleDisplacersChanged()
{
	// here's how we signal that we need to recalc the skin based on
	// displacer changes

	SetStateDirty();
	SetDirtyBits(eAnmSegmentDisplacersDirty);
}


void CAnmHAnimSegment::DisplacersChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, 
											  void *callData, void *userData)
{
	// N.B.: all displacer callbacks come through here. at the moment
	// they only set a flag saying the displacer changed; they don't use
	// the call data. If you need the call data you will have to check
	// the reason param to figure out how to cast it

	CAnmHAnimDisplacer *pDisplacer = (CAnmHAnimDisplacer *) sourceObject;
	CAnmHAnimSegment *pSegment = (CAnmHAnimSegment *) userData;

	pSegment->HandleDisplacersChanged();
}


void CAnmHAnimSegment::AddDisplacerCallbacks()
{
	// Add callbacks so we know when anything changes in the displacer
	int sz = m_displacers->size();
	for (int i = 0; i < sz; i++)
	{
		CAnmNode *pNode = (*m_displacers)[i];
		CAnmNode *pImp = pNode->GetImplementationNode();

		if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmHAnimDisplacer)))
		{
			CAnmHAnimDisplacer *pDisplacer = (CAnmHAnimDisplacer *) pImp;

			pDisplacer->AddCallback(CALLBACKID(CAnmHAnimDisplacer, coordIndex_changed), 
				DisplacersChangedCallback, this);
			pDisplacer->AddCallback(CALLBACKID(CAnmHAnimDisplacer, displacements_changed), 
				DisplacersChangedCallback, this);
			pDisplacer->AddCallback(CALLBACKID(CAnmHAnimDisplacer, weight_changed), 
				DisplacersChangedCallback, this);
		}
	}
}




void CAnmHAnimSegment::SetDisplacers(NodeArray *pDisplacers)
{
	assert(pDisplacers != NULL);

	SafeUnRef(m_displacers);

	m_displacers = pDisplacers;
	m_displacers->Ref();

	SetStateDirty();
	SetDirtyBits(eAnmSegmentDisplacersDirty);

	// $$$ Keith - trying SetMatrixDirty() - let me know if it works
	// krv: dont know why we need to set matrix dirty?
	SetMatrixDirty();

	// Make sure we get notified when displacements change for morphing
	AddDisplacerCallbacks();


	CallCallbacks(CALLBACKID(CAnmHAnimSegment, displacers_changed), &pDisplacers);
}

void CAnmHAnimSegment::SetMass(Float mass)
{
	m_mass = mass;

	// $$$ Keith - Here's where we need to figure out traversal
	// Do nothing, noone cares.

	CallCallbacks(CALLBACKID(CAnmHAnimSegment, mass_changed), &mass);
}

void CAnmHAnimSegment::SetMomentsOfInertia(FloatArray *pMomentsOfInertia)
{
	assert(pMomentsOfInertia != NULL);

	SafeUnRef(m_momentsOfInertia);
	m_momentsOfInertia = pMomentsOfInertia;
	m_momentsOfInertia->Ref();

	// $$$ Keith - Here's where we need to figure out traversal
	// krv do nothing.  this is never used.


	CallCallbacks(CALLBACKID(CAnmHAnimSegment, momentsOfInertia_changed), &pMomentsOfInertia);
}


void CAnmHAnimSegment::SetName(String name)
{
	SafeUnRef(m_name);
	m_name = name;
	m_name->Ref();

	// I don't think we need this
	// SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimSegment, name_changed), &name);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmHAnimSegment, bboxCenter, CAnmGroup);
DEFINE_INHERITED_VALUE(CAnmHAnimSegment, bboxSize, CAnmGroup);
DEFINE_VALUE(CAnmHAnimSegment, centerOfMass, eValuePoint3, Point3, GetCenterOfMass, SetCenterOfMass);
DEFINE_INHERITED_VALUE(CAnmHAnimSegment, children, CAnmGroup);
DEFINE_VALUE(CAnmHAnimSegment, coord, eValueNode, CAnmNode*, GetCoord, SetCoord);
DEFINE_VALUE(CAnmHAnimSegment, displacers, eValueNodeArray, NodeArray*, GetDisplacers, SetDisplacers);
DEFINE_VALUE(CAnmHAnimSegment, mass, eValueFloat, Float, GetMass, SetMass);
DEFINE_VALUE(CAnmHAnimSegment, momentsOfInertia, eValueFloatArray, FloatArray*, GetMomentsOfInertia, SetMomentsOfInertia);
DEFINE_VALUE(CAnmHAnimSegment, name, eValueString, String, GetName, SetName);

DEFINE_INHERITED_METHOD(CAnmHAnimSegment, addChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmHAnimSegment, removeChildren, CAnmGroup);
DEFINE_METHOD(CAnmHAnimSegment, set_centerOfMass, eValuePoint3, Point3, SetCenterOfMass);
DEFINE_INHERITED_METHOD(CAnmHAnimSegment, set_children, CAnmGroup);
DEFINE_METHOD(CAnmHAnimSegment, set_coord, eValueNode, CAnmNode*, SetCoord);
DEFINE_METHOD(CAnmHAnimSegment, set_displacers, eValueNodeArray, NodeArray*, SetDisplacers);
DEFINE_METHOD(CAnmHAnimSegment, set_mass, eValueFloat, Float, SetMass);
DEFINE_METHOD(CAnmHAnimSegment, set_momentsOfInertia, eValueFloatArray, FloatArray*, SetMomentsOfInertia);
DEFINE_METHOD(CAnmHAnimSegment, set_name, eValueString, String, SetName);

DEFINE_CALLBACK(CAnmHAnimSegment, centerOfMass_changed, eValuePoint3);
DEFINE_INHERITED_CALLBACK(CAnmHAnimSegment, children_changed, CAnmGroup);
DEFINE_CALLBACK(CAnmHAnimSegment, coord_changed, eValueNode);
DEFINE_CALLBACK(CAnmHAnimSegment, displacers_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmHAnimSegment, mass_changed, eValueFloat);
DEFINE_CALLBACK(CAnmHAnimSegment, momentsOfInertia_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmHAnimSegment, name_changed, eValueString);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmHAnimSegment)
VALUEID(CAnmHAnimSegment, bboxCenter),
VALUEID(CAnmHAnimSegment, bboxSize),
VALUEID(CAnmHAnimSegment, centerOfMass),
VALUEID(CAnmHAnimSegment, children),
VALUEID(CAnmHAnimSegment, coord),
VALUEID(CAnmHAnimSegment, displacers),
VALUEID(CAnmHAnimSegment, mass),
VALUEID(CAnmHAnimSegment, momentsOfInertia),
VALUEID(CAnmHAnimSegment, name),

METHODID(CAnmHAnimSegment, addChildren),
METHODID(CAnmHAnimSegment, removeChildren),
METHODID(CAnmHAnimSegment, set_centerOfMass),
METHODID(CAnmHAnimSegment, set_children),
METHODID(CAnmHAnimSegment, set_coord),
METHODID(CAnmHAnimSegment, set_displacers),
METHODID(CAnmHAnimSegment, set_mass),
METHODID(CAnmHAnimSegment, set_momentsOfInertia),
METHODID(CAnmHAnimSegment, set_name),

CALLBACKID(CAnmHAnimSegment, centerOfMass_changed),
CALLBACKID(CAnmHAnimSegment, children_changed),
CALLBACKID(CAnmHAnimSegment, coord_changed),
CALLBACKID(CAnmHAnimSegment, displacers_changed),
CALLBACKID(CAnmHAnimSegment, mass_changed),
CALLBACKID(CAnmHAnimSegment, momentsOfInertia_changed),
CALLBACKID(CAnmHAnimSegment, name_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmHAnimSegment, CAnmGroup);

