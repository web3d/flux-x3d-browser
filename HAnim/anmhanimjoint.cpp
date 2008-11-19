/********************************************************************************
 * Flux
 *
 * File: anmhanimjoint.cpp
 * Description: HAnim Joint node
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
#include "anmhanimjoint.h"
#include "anmcoordinate.h"
#include "anmhanimdisplacer.h"
#include "anmhanimsegment.h"
#include "anmnormal.h"
#include "anmupdatevisitor.h"
#include "anmMatrixInvert.h"


// krv??? If we dont deal with Groups in the Skeletons, then we will crash
// Putting Groups in the skel is not to spec.

#define ALLOW_GROUPS_IN_SKEL_UPDATE   1
#define ALLOW_GROUPS_IN_SKEL_INIT     1


// crash
//#define ALLOW_GROUPS_IN_SKEL_UPDATE   0
//#define ALLOW_GROUPS_IN_SKEL_INIT     1


// N.B. move to our own math lib soon
// krv Is this in a header somewhere?  Other files also have a local definition.


CAnmHAnimJoint::CAnmHAnimJoint() : CAnmTransform()
{
	m_displacers = new NodeArray;
	m_limitOrientation = ANMHANIMJOINT_DEFAULT_LIMITORIENTATION;
	m_llimit = new FloatArray;
	m_name = new CAnmString(ANMHANIMJOINT_DEFAULT_NAME);
	m_skinCoordIndex = new IntegerArray;
	m_skinCoordWeight = new FloatArray;
	m_skinNormalIndex = new IntegerArray;
	m_skinNormalWeight = new FloatArray;
	m_stiffness = new FloatArray;
	m_ulimit = new FloatArray;

	m_singleMesh = false;
}

CAnmHAnimJoint::~CAnmHAnimJoint()
{
	SafeUnRef(m_displacers);
	SafeUnRef(m_llimit);
	SafeUnRef(m_name);
	SafeUnRef(m_skinCoordIndex);
	SafeUnRef(m_skinCoordWeight);
	SafeUnRef(m_skinNormalIndex);
	SafeUnRef(m_skinNormalWeight);
	SafeUnRef(m_stiffness);
	SafeUnRef(m_ulimit);
}

// Methods


void CAnmHAnimJoint::Realize()
{
	CAnmTransform::Realize();

	// making sure the matrix is calculated before initializing the skin
	CAnmTransform::Update();
}

void CAnmHAnimJoint::ChildChanged(CAnmNode *pChild)
{
	CAnmTransform::ChildChanged(pChild);
	
	NotifyParentsOfChange();
}

void CAnmHAnimJoint::SetCenter(Point3 p)
{
	CAnmTransform::SetCenter(p);

	// this tells the humanoid to do the UpdateSkin() thing for single mesh
	if (m_singleMesh)
		NotifyParentsOfChange();
}

void CAnmHAnimJoint::SetRotation(Rotation r)
{
	CAnmTransform::SetRotation(r);

	// this tells the humanoid to do the UpdateSkin() thing for single mesh
	if (m_singleMesh)
		NotifyParentsOfChange();
}

void CAnmHAnimJoint::SetScale(Point3 p)
{
	CAnmTransform::SetScale(p);

	// this tells the humanoid to do the UpdateSkin() thing for single mesh
	if (m_singleMesh)
		NotifyParentsOfChange();
}

void CAnmHAnimJoint::SetScaleOrientation(Rotation r)
{
	CAnmTransform::SetScaleOrientation(r);

	// this tells the humanoid to do the UpdateSkin() thing for single mesh
	if (m_singleMesh)
		NotifyParentsOfChange();
}
 
void CAnmHAnimJoint::SetTranslation(Point3 p)
{
	CAnmTransform::SetTranslation(p);

	// this tells the humanoid to do the UpdateSkin() thing for single mesh
	if (m_singleMesh)
		NotifyParentsOfChange();
}


// krv:  static member to recurse through group Nodes in skeleton.
//
void CAnmHAnimJoint::UpdateSkin(CAnmGroup *pGroup, CAnmCoordinate *pCoord, CAnmNormal *pNormal, const matrix4 &mat)
{

	// Here's where we recurse and hit the children
	tAnmNodeList* pKids = pGroup->Children();
	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = pKids->begin(); 
		iter != pKids->end();
		iter++ )
	{
		CAnmNode *pNode = *iter;
	
		if (pNode)
		{
			CAnmNode *pImp = pNode->GetImplementationNode();

			if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmHAnimJoint)))
			{
				CAnmHAnimJoint *pJoint = (CAnmHAnimJoint *) pImp;

				pJoint->UpdateSkin(pCoord, pNormal, mat);
			}
			// krv:  Allows for Group Nodes inside the skeleton, NOT to Spec.
			//
			else if ( ALLOW_GROUPS_IN_SKEL_UPDATE && pImp && pImp->GetClass()->IsA(GETCLASS(CAnmGroup)))
			{
				CAnmGroup *pGroup = (CAnmGroup *) pImp;

				UpdateSkin( pGroup, pCoord, pNormal, mat);
			}
		}
	}
}





void CAnmHAnimJoint::UpdateSkin(CAnmCoordinate *pCoord, CAnmNormal *pNormal, const matrix4 &mat)
{
	Update();
	
	// 1. This composes my matrix with the passed-in one to get into my local space
	matrix4 localmat = m_matrix * mat;

	// 2. Here's where we change the coordinate and normal values based
	// on the saved matrix and/or displacers

	// First Add the Verticies that are attached to this Joint
	// Loop through the skin coord Index array, and calculate the local vertex

	FloatArray*  pWeights   = GetSkinCoordWeight();
	int nVertEffected = min( m_skinCoordIndex->size(), pWeights->size() );

	Point3Array* pRawCoords = ( ( pCoord ) ? ( pCoord->GetPoint() ) : NULL );

	// add the components to the verts
	//
	if( pCoord && nVertEffected > 0 ) {
		int nVerts = pRawCoords->size();
		for( int iVertEffected=0; iVertEffected<nVertEffected; iVertEffected++ ) {
			int iVertIndex = (*m_skinCoordIndex)[iVertEffected];
			if( iVertIndex >= 0 && iVertIndex < nVerts ) {
				// Get the gloabal vertex location times weight.
				// Add this component to the Vertex in the Coord Array
				(*pRawCoords)[iVertIndex] += 
					( localmat* (m_localCoords[iVertEffected]) ) * ((*pWeights)[iVertEffected]);
			}
		}
	}


	// Loop through the skin Normal Index array, and update the normals
	// Get the Normal count up here, so we know if we need to inver the matrix
	FloatArray* pNormalWeights   = GetSkinNormalWeight();
	int nNormalsEffected = min( m_skinNormalIndex->size(), pNormalWeights->size() );
	if( pNormal && nNormalsEffected > 0 ) {

		Point3Array* pRawNormals = pNormal->GetVector();
		int nVerts = pRawNormals->size();

		// loop through normals to get local normal releative to Joint
		//
		for( int iVertEffected=0; iVertEffected<nNormalsEffected; iVertEffected++ ) {
			int iVertIndex = (*m_skinNormalIndex)[iVertEffected];
			if( iVertIndex >= 0 && iVertIndex < nVerts ) {
				point3 globalNormal = ( ( localmat* ( m_localNormals[iVertEffected] ) ) - localmat.GetLoc() ) * 
					((*pNormalWeights)[iVertEffected]);
						
				(*pRawNormals)[iVertIndex] += globalNormal;
			}
		}
	}



	// Second, loop through displacers and add the displacments.
	//
	if( pRawCoords ) {
		int nDisplacers = m_displacers->size();
		for (int iDisplacer = 0; iDisplacer < nDisplacers; iDisplacer++)
		{
			CAnmNode *pNode = (*m_displacers)[iDisplacer];
			CAnmNode *pImp = pNode->GetImplementationNode();

			if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmHAnimDisplacer)))
			{
				CAnmHAnimDisplacer *pDisplacer = (CAnmHAnimDisplacer *) pImp;
				pDisplacer->DisplaceSingleMesh( pRawCoords, localmat );
			}
		}
	}


	// 3. Here's where we recurse and hit the children

	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		CAnmNode *pNode = *iter;
	
		if (pNode)
		{
			CAnmNode *pImp = pNode->GetImplementationNode();

			if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmHAnimJoint)))
			{
				CAnmHAnimJoint *pJoint = (CAnmHAnimJoint *) pImp;

				pJoint->UpdateSkin(pCoord, pNormal, localmat);
			}
			// krv:  Allows for Group Nodes inside the skeleton, NOT to Spec.
			//
			else if ( ALLOW_GROUPS_IN_SKEL_UPDATE && pImp && pImp->GetClass()->IsA(GETCLASS(CAnmGroup)))
			{
				CAnmGroup *pGroup = (CAnmGroup *) pImp;

				UpdateSkin( pGroup, pCoord, pNormal, localmat);
			}
		}
	}

}


void CAnmHAnimJoint::InitializeSkin(CAnmCoordinate *pCoord, CAnmNormal *pNormal, const matrix4 &mat)
{
	// Flag single mesh for later optimization
	m_singleMesh = true;

	// 1. This composes my matrix with the passed-in one to get into my local space
	matrix4 localmat = m_matrix * mat;

	// Invert it to get the local vector to each effected vertex.
	//


	m_localCoords.clear();
	m_localNormals.clear();
	// 2. Here's where we get the local vectro fromt the local Joint coord to the effected vertex.
	//

	// Loop through the skin coord Index array, and calculate the local vertex
	FloatArray*  pWeights   = GetSkinCoordWeight();
	int nVertEffected = min( m_skinCoordIndex->size(), pWeights->size() );


	// Get the Normal count up here, so we know if we need to inver the matrix
	FloatArray* pNormalWeights   = GetSkinNormalWeight();
	int nNormalsEffected = min( m_skinNormalIndex->size(), pNormalWeights->size() );

	if( nVertEffected > 0 || nNormalsEffected > 0 ) {

		matrix4 localMatrixInverted;
		MatrixInvert(localMatrixInverted, localmat);

		if( pCoord && nVertEffected > 0 ) {

			Point3Array* pRawCoords = pCoord->GetPoint();
			int nVerts = pRawCoords->size();

			// loop through verts. to get local vertex releative to Joint
			//
			for( int iVertEffected=0; iVertEffected<nVertEffected; iVertEffected++ ) {
				int iVertIndex = (*m_skinCoordIndex)[iVertEffected];
				if( iVertIndex >= 0 && iVertIndex < nVerts ) {
					m_localCoords.push_back(localMatrixInverted * ((*pRawCoords)[iVertIndex] ));
				}
			}
		}

		// Loop through the skin Normal Index array, and calculate the local normals
		if( pNormal && nNormalsEffected > 0 ) {

			Point3Array* pRawNormals = pNormal->GetVector();
			int nVerts = pRawNormals->size();

			// loop through normals to get local normal releative to Joint
			//
			for( int iVertEffected=0; iVertEffected<nNormalsEffected; iVertEffected++ ) {
				int iVertIndex = (*m_skinNormalIndex)[iVertEffected];
				if( iVertIndex >= 0 && iVertIndex < nVerts ) {
					point3 localNormal = localMatrixInverted * ((*pRawNormals)[iVertIndex] ) - 
						localMatrixInverted.GetLoc();
					m_localNormals.push_back( localNormal );
				}
			}
		}
	}



	// 3. Here's where we recurse and hit the children

	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		CAnmNode *pNode = *iter;
	
		if (pNode)
		{
			CAnmNode *pImp = pNode->GetImplementationNode();

			if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmHAnimJoint)))
			{
				CAnmHAnimJoint *pJoint = (CAnmHAnimJoint *) pImp;

				pJoint->InitializeSkin(pCoord, pNormal, localmat);
			}
			// krv:  Allows for Group Nodes inside the skeleton, NOT to Spec.
			//
			else if ( ALLOW_GROUPS_IN_SKEL_INIT && pImp && pImp->GetClass()->IsA(GETCLASS(CAnmGroup)))
			{
				CAnmGroup *pGroup = (CAnmGroup *) pImp;

				InitializeSkin( pGroup, pCoord, pNormal, localmat);
			}
		}
	}
}




// krv:  static member to recurse through group Nodes in skeleton.
//
void CAnmHAnimJoint::InitializeSkin(CAnmGroup *pGroup, CAnmCoordinate *pCoord, CAnmNormal *pNormal, const matrix4 &mat)
{
	// Here's where we recurse and hit the children
	tAnmNodeList* pKids = pGroup->Children();
	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = pKids->begin(); 
		iter != pKids->end();
		iter++ )
	{
		CAnmNode *pNode = *iter;
	
		if (pNode)
		{
			CAnmNode *pImp = pNode->GetImplementationNode();

			if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmHAnimJoint)))
			{
				CAnmHAnimJoint *pJoint = (CAnmHAnimJoint *) pImp;

				pJoint->InitializeSkin(pCoord, pNormal, mat);
			}
			// krv:  Allows for Group Nodes inside the skeleton, NOT to Spec.
			//
			else if ( ALLOW_GROUPS_IN_SKEL_INIT && pImp && pImp->GetClass()->IsA(GETCLASS(CAnmGroup)))
			{
				CAnmGroup *pGroup = (CAnmGroup *) pImp;

				InitializeSkin(pGroup, pCoord, pNormal, mat);
			}
		}
	}
}



void CAnmHAnimJoint::HandleDisplacersChanged()
{
	// here's how we signal that we need to recalc the skin based on
	// displacer changes
	// this tells the humanoid to do the UpdateSkin() thing for single mesh

	SetStateAndMatrixDirty();
	NotifyParentsOfChange();
}

void CAnmHAnimJoint::DisplacersChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, 
											  void *callData, void *userData)
{
	// N.B.: all displacer callbacks come through here. at the moment
	// they only set a flag saying the displacer changed; they don't use
	// the call data. If you need the call data you will have to check
	// the reason param to figure out how to cast it

	CAnmHAnimDisplacer *pDisplacer = (CAnmHAnimDisplacer *) sourceObject;
	CAnmHAnimJoint *pJoint = (CAnmHAnimJoint *) userData;

	pJoint->HandleDisplacersChanged();
}

void CAnmHAnimJoint::AddDisplacerCallbacks()
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

// Accessors
void CAnmHAnimJoint::SetDisplacers(NodeArray *pDisplacers)
{
	assert(pDisplacers != NULL);

	SafeUnRef(m_displacers);
	m_displacers = pDisplacers;
	m_displacers->Ref();


	// Make sure we get notified when displacements change for morphing
	AddDisplacerCallbacks();

	// $$$ Keith - trying SetStateAndMatrixDirty() - let me know if it works
	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimJoint, displacers_changed), &pDisplacers);
}


void CAnmHAnimJoint::SetLimitOrientation(Rotation r)
{
	m_limitOrientation = r;
	CallCallbacks(CALLBACKID(CAnmHAnimJoint, limitOrientation_changed), &r);
}

void CAnmHAnimJoint::SetLLimit(FloatArray *pLLimit)
{
	assert(pLLimit != NULL);

	SafeUnRef(m_llimit);
	m_llimit = pLLimit;
	m_llimit->Ref();
	CallCallbacks(CALLBACKID(CAnmHAnimJoint, llimit_changed), &pLLimit);
}

void CAnmHAnimJoint::SetName(String name)
{
	SafeUnRef(m_name);
	m_name = name;
	m_name->Ref();

	// I don't think we need this
	// SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimJoint, name_changed), &name);
}

void CAnmHAnimJoint::SetSkinCoordIndex(IntegerArray *pSkinCoordIndex)
{
	assert(pSkinCoordIndex != NULL);

	SafeUnRef(m_skinCoordIndex);
	m_skinCoordIndex = pSkinCoordIndex;
	m_skinCoordIndex->Ref();

	// $$$ Keith - trying SetStateAndMatrixDirty() - let me know if it works
	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimJoint, skinCoordIndex_changed), &pSkinCoordIndex);
}

void CAnmHAnimJoint::SetSkinCoordWeight(FloatArray *pSkinCoordWeight)
{
	assert(pSkinCoordWeight != NULL);

	SafeUnRef(m_skinCoordWeight);
	m_skinCoordWeight = pSkinCoordWeight;
	m_skinCoordWeight->Ref();

	// $$$ Keith - trying SetStateAndMatrixDirty() - let me know if it works
	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimJoint, skinCoordWeight_changed), &pSkinCoordWeight);
}


void CAnmHAnimJoint::SetSkinNormalIndex(IntegerArray *pSkinNormalIndex)
{
	assert(pSkinNormalIndex != NULL);

	SafeUnRef(m_skinNormalIndex);
	m_skinNormalIndex = pSkinNormalIndex;
	m_skinNormalIndex->Ref();

	// $$$ Keith - trying SetStateAndMatrixDirty() - let me know if it works
	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimJoint, skinNormalIndex_changed), &pSkinNormalIndex);
}

void CAnmHAnimJoint::SetSkinNormalWeight(FloatArray *pSkinNormalWeight)
{
	assert(pSkinNormalWeight != NULL);

	SafeUnRef(m_skinNormalWeight);
	m_skinNormalWeight = pSkinNormalWeight;
	m_skinNormalWeight->Ref();

	// $$$ Keith - trying SetStateAndMatrixDirty() - let me know if it works
	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimJoint, skinNormalWeight_changed), &pSkinNormalWeight);
}



void CAnmHAnimJoint::SetStiffness(FloatArray *pStiffness)
{
	assert(pStiffness != NULL);

	SafeUnRef(m_stiffness);
	m_stiffness = pStiffness;
	m_stiffness->Ref();

	CallCallbacks(CALLBACKID(CAnmHAnimJoint, stiffness_changed), &pStiffness);
}

void CAnmHAnimJoint::SetULimit(FloatArray *pULimit)
{
	assert(pULimit != NULL);

	SafeUnRef(m_ulimit);
	m_ulimit = pULimit;
	m_ulimit->Ref();

	CallCallbacks(CALLBACKID(CAnmHAnimJoint, ulimit_changed), &pULimit);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmHAnimJoint, bboxCenter, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimJoint, bboxSize, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimJoint, center, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimJoint, children, CAnmTransform);
DEFINE_VALUE(CAnmHAnimJoint, displacers, eValueNodeArray, NodeArray*, GetDisplacers, SetDisplacers);
DEFINE_VALUE(CAnmHAnimJoint, limitOrientation, eValueRotation, Rotation, GetLimitOrientation, SetLimitOrientation);
DEFINE_VALUE(CAnmHAnimJoint, llimit, eValueFloatArray, FloatArray*, GetLLimit, SetLLimit);
DEFINE_VALUE(CAnmHAnimJoint, name, eValueString, String, GetName, SetName);
DEFINE_INHERITED_VALUE(CAnmHAnimJoint, rotation, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimJoint, scale, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimJoint, scaleOrientation, CAnmTransform);
DEFINE_VALUE(CAnmHAnimJoint, skinCoordIndex, eValueIntegerArray, IntegerArray*, GetSkinCoordIndex, SetSkinCoordIndex);
DEFINE_VALUE(CAnmHAnimJoint, skinCoordWeight, eValueFloatArray, FloatArray*, GetSkinCoordWeight, SetSkinCoordWeight);
DEFINE_VALUE(CAnmHAnimJoint, skinNormalIndex, eValueIntegerArray, IntegerArray*, GetSkinNormalIndex, SetSkinNormalIndex);
DEFINE_VALUE(CAnmHAnimJoint, skinNormalWeight, eValueFloatArray, FloatArray*, GetSkinNormalWeight, SetSkinNormalWeight);
DEFINE_VALUE(CAnmHAnimJoint, stiffness, eValueFloatArray, FloatArray*, GetStiffness, SetStiffness);
DEFINE_INHERITED_VALUE(CAnmHAnimJoint, translation, CAnmTransform);
DEFINE_VALUE(CAnmHAnimJoint, ulimit, eValueFloatArray, FloatArray*, GetULimit, SetULimit);

DEFINE_INHERITED_METHOD(CAnmHAnimJoint, addChildren, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimJoint, removeChildren, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimJoint, set_center, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimJoint, set_children, CAnmTransform);
DEFINE_METHOD(CAnmHAnimJoint, set_displacers, eValueNodeArray, NodeArray*, SetDisplacers);
DEFINE_METHOD(CAnmHAnimJoint, set_limitOrientation, eValueRotation, Rotation, SetLimitOrientation);
DEFINE_METHOD(CAnmHAnimJoint, set_llimit, eValueFloatArray, FloatArray*, SetLLimit);
DEFINE_METHOD(CAnmHAnimJoint, set_name, eValueString, String, SetName);
DEFINE_INHERITED_METHOD(CAnmHAnimJoint, set_rotation, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimJoint, set_scale, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimJoint, set_scaleOrientation, CAnmTransform);
DEFINE_METHOD(CAnmHAnimJoint, set_skinCoordIndex, eValueIntegerArray, IntegerArray*, SetSkinCoordIndex);
DEFINE_METHOD(CAnmHAnimJoint, set_skinCoordWeight, eValueFloatArray, FloatArray*, SetSkinCoordWeight);
DEFINE_METHOD(CAnmHAnimJoint, set_skinNormalIndex, eValueIntegerArray, IntegerArray*, SetSkinNormalIndex);
DEFINE_METHOD(CAnmHAnimJoint, set_skinNormalWeight, eValueFloatArray, FloatArray*, SetSkinNormalWeight);
DEFINE_METHOD(CAnmHAnimJoint, set_stiffness, eValueFloatArray, FloatArray*, SetStiffness);
DEFINE_INHERITED_METHOD(CAnmHAnimJoint, set_translation, CAnmTransform);
DEFINE_METHOD(CAnmHAnimJoint, set_ulimit, eValueFloatArray, FloatArray*, SetULimit);

DEFINE_INHERITED_CALLBACK(CAnmHAnimJoint, center_changed, CAnmTransform);
DEFINE_INHERITED_CALLBACK(CAnmHAnimJoint, children_changed, CAnmTransform);
DEFINE_CALLBACK(CAnmHAnimJoint, displacers_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmHAnimJoint, limitOrientation_changed, eValueRotation);
DEFINE_CALLBACK(CAnmHAnimJoint, llimit_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmHAnimJoint, name_changed, eValueString);
DEFINE_INHERITED_CALLBACK(CAnmHAnimJoint, rotation_changed, CAnmTransform);
DEFINE_INHERITED_CALLBACK(CAnmHAnimJoint, scale_changed, CAnmTransform);
DEFINE_INHERITED_CALLBACK(CAnmHAnimJoint, scaleOrientation_changed, CAnmTransform);
DEFINE_CALLBACK(CAnmHAnimJoint, skinCoordIndex_changed, eValueIntegerArray);
DEFINE_CALLBACK(CAnmHAnimJoint, skinCoordWeight_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmHAnimJoint, skinNormalIndex_changed, eValueIntegerArray);
DEFINE_CALLBACK(CAnmHAnimJoint, skinNormalWeight_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmHAnimJoint, stiffness_changed, eValueFloatArray);
DEFINE_INHERITED_CALLBACK(CAnmHAnimJoint, translation_changed, CAnmTransform);
DEFINE_CALLBACK(CAnmHAnimJoint, ulimit_changed, eValueFloatArray);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmHAnimJoint)
VALUEID(CAnmHAnimJoint, bboxCenter),
VALUEID(CAnmHAnimJoint, bboxSize),
VALUEID(CAnmHAnimJoint, center),
VALUEID(CAnmHAnimJoint, children),
VALUEID(CAnmHAnimJoint, displacers),
VALUEID(CAnmHAnimJoint, limitOrientation),
VALUEID(CAnmHAnimJoint, llimit),
VALUEID(CAnmHAnimJoint, name),
VALUEID(CAnmHAnimJoint, rotation),
VALUEID(CAnmHAnimJoint, scale),
VALUEID(CAnmHAnimJoint, scaleOrientation),
VALUEID(CAnmHAnimJoint, skinCoordIndex),
VALUEID(CAnmHAnimJoint, skinCoordWeight),
VALUEID(CAnmHAnimJoint, skinNormalIndex),
VALUEID(CAnmHAnimJoint, skinNormalWeight),
VALUEID(CAnmHAnimJoint, stiffness),
VALUEID(CAnmHAnimJoint, translation),
VALUEID(CAnmHAnimJoint, ulimit),

METHODID(CAnmHAnimJoint, addChildren),
METHODID(CAnmHAnimJoint, removeChildren),
METHODID(CAnmHAnimJoint, set_center),
METHODID(CAnmHAnimJoint, set_children),
METHODID(CAnmHAnimJoint, set_displacers),
METHODID(CAnmHAnimJoint, set_limitOrientation),
METHODID(CAnmHAnimJoint, set_llimit),
METHODID(CAnmHAnimJoint, set_name),
METHODID(CAnmHAnimJoint, set_rotation),
METHODID(CAnmHAnimJoint, set_scale),
METHODID(CAnmHAnimJoint, set_scaleOrientation),
METHODID(CAnmHAnimJoint, set_skinCoordIndex),
METHODID(CAnmHAnimJoint, set_skinCoordWeight),
METHODID(CAnmHAnimJoint, set_skinNormalIndex),
METHODID(CAnmHAnimJoint, set_skinNormalWeight),
METHODID(CAnmHAnimJoint, set_stiffness),
METHODID(CAnmHAnimJoint, set_translation),
METHODID(CAnmHAnimJoint, set_ulimit),

CALLBACKID(CAnmHAnimJoint, center_changed),
CALLBACKID(CAnmHAnimJoint, children_changed),
CALLBACKID(CAnmHAnimJoint, displacers_changed),
CALLBACKID(CAnmHAnimJoint, limitOrientation_changed),
CALLBACKID(CAnmHAnimJoint, llimit_changed),
CALLBACKID(CAnmHAnimJoint, name_changed),
CALLBACKID(CAnmHAnimJoint, rotation_changed),
CALLBACKID(CAnmHAnimJoint, scale_changed),
CALLBACKID(CAnmHAnimJoint, scaleOrientation_changed),
CALLBACKID(CAnmHAnimJoint, skinCoordIndex_changed),
CALLBACKID(CAnmHAnimJoint, skinCoordWeight_changed),
CALLBACKID(CAnmHAnimJoint, skinNormalIndex_changed),
CALLBACKID(CAnmHAnimJoint, skinNormalWeight_changed),
CALLBACKID(CAnmHAnimJoint, stiffness_changed),
CALLBACKID(CAnmHAnimJoint, translation_changed),
CALLBACKID(CAnmHAnimJoint, ulimit_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmHAnimJoint, CAnmTransform);

