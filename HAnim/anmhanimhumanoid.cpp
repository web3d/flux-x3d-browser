/********************************************************************************
 * Flux
 *
 * File: anmhanimhumanoid.cpp
 * Description: HAnim Humanoid node
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
#include "anmhanimhumanoid.h"
#include "anmcoordinate.h"
#include "anmhanimjoint.h"
#include "anmnormal.h"
	
CAnmHAnimHumanoid::CAnmHAnimHumanoid() : CAnmTransform()
{
	m_info = new StringArray;
	m_joints = new NodeArray;
	m_name = new CAnmString(ANMHANIMHUMANOID_DEFAULT_NAME);
	m_segments = new NodeArray;
	m_sites = new NodeArray;
	m_skeleton = new NodeArray;
	m_skin = new NodeArray;
	m_skinCoord = NULL;
	m_skinNormal = NULL;
	m_version = new CAnmString(ANMHANIMHUMANOID_DEFAULT_VERSION);
	m_viewpoints = new NodeArray;
}

CAnmHAnimHumanoid::~CAnmHAnimHumanoid()
{
	SafeUnRef(m_info);
	SafeUnRef(m_joints);
	SafeUnRef(m_name);
	SafeUnRef(m_segments);
	SafeUnRef(m_sites);
	SafeUnRef(m_skeleton);
	SafeUnRef(m_skin);
	SafeUnRef(m_skinCoord);
	SafeUnRef(m_skinNormal);
	SafeUnRef(m_version);
}

// Methods
void CAnmHAnimHumanoid::Realize()
{
	CAnmTransform::Realize();

	if (m_skinCoord)
	{
		// Initialize the skin if it's there
		InitializeSkin();
	}
}

void CAnmHAnimHumanoid::InitializeSkin()
{
	assert(m_skinCoord);

	CAnmNode *pImp = m_skinCoord->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
	{
		CAnmCoordinate *pCoord = (CAnmCoordinate *) pImp;

		CAnmNormal *pNormal = NULL;
		if( m_skinNormal ) 
		{
			pImp = m_skinNormal->GetImplementationNode();
			if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmNormal))) 
			{
				pNormal = (CAnmNormal *) pImp;
			}
		}

		// making sure the matrix is calculated before initializing the skin
		CAnmTransform::Update();

		InitializeSkin( pCoord, pNormal );
	}

}

void CAnmHAnimHumanoid::Update()
{
	CAnmTransform::Update();

	// Make sure the skin is initialize if it's changed
	if (m_skinCoord &&
		(TestDirtyBits(eAnmHumanoidSkinCoordDirty) ||
		TestDirtyBits(eAnmHumanoidSkinNormalDirty))
		)
	{
		InitializeSkin();
	}

	if (m_skinCoord &&
		(TestDirtyBits(eAnmHumanoidSkinCoordDirty) ||
		TestDirtyBits(eAnmHumanoidSkinNormalDirty) ||
		TestDirtyBits(eAnmNodeChildMatrixDirty))
		)
	{
		CAnmNode *pImp = m_skinCoord->GetImplementationNode();

		if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		{
			CAnmCoordinate *pCoord = (CAnmCoordinate *) pImp;

			CAnmNormal *pNormal = NULL;
			if( m_skinNormal ) 
			{
				pImp = m_skinNormal->GetImplementationNode();
				if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmNormal))) 
				{
					pNormal = (CAnmNormal *) pImp;
				}
			}
			UpdateSkin(pCoord, pNormal);
		}
	}

	ClearStateDirty();
}

void CAnmHAnimHumanoid::UpdateSkin(CAnmCoordinate *pCoord, CAnmNormal *pNormal)
{

 
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


				// krv: zero out the Coord Array.
				// Is there a faster way to do this?
				if( pCoord ) {
					Point3Array* pRawCoords = pCoord->GetPoint();
					int nCoords = pRawCoords->size();
					for( int iCoord=0; iCoord<nCoords; iCoord++ )
					{
						(*pRawCoords)[iCoord] = Point3::Zero;
					}
				}

				if( pNormal ) {
					Point3Array* pRawNormals = pNormal->GetVector();
					int nNorms = pRawNormals->size();
					for( int iNorm=0; iNorm<nNorms; iNorm++ )
					{
						(*pRawNormals)[iNorm] = Point3::Zero;
					}
				}

				
				// bootstrap the skin deformation using
				// identity matrix - Joints will compose their model-space
				// matrices into this
				pJoint->UpdateSkin(pCoord, pNormal, matrix4::Identity);


				// Post normalize the normal vectors
				//
				if( pNormal ) {
					Point3Array* pRawNormals = pNormal->GetVector();
					int nNorms = pRawNormals->size();
					for( int iNorm=0; iNorm<nNorms; iNorm++ )
					{
						(*pRawNormals)[iNorm].Normalize();
					}
				}


				// Tell the Coord that is is dirty, so the low level graphics data can get generated
				//
				pCoord->FlagUpdate();

				// This will break out after we find the first joint in the skeleton.
				// If we are allowed to ahve more than one joint in the skeleton ( Multiple RootJoints! )
				// then this will need to change.  Need to wait on a spec clearification!
				// krv: TODO Wait for spec cleaification.
				break;
			}
		}
	}
}


void CAnmHAnimHumanoid::InitializeSkin(CAnmCoordinate *pCoord, CAnmNormal *pNormal)
{


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

				// bootstrap the skin deformation using
				// identity matrix - Joints will compose their model-space
				// matrices into this

				pJoint->InitializeSkin(pCoord, pNormal, matrix4::Identity);
			}
		}
	}
}


void CAnmHAnimHumanoid::ChildChanged(CAnmNode *pChild)
{
	CAnmTransform::ChildChanged(pChild);
	
	// this is how we know when something changes in a single mesh
	SetStateDirty();
}


// Accessors
void CAnmHAnimHumanoid::SetInfo(StringArray *info)
{
	assert(info != NULL);

	SafeUnRef(m_info);
	m_info = info;
	m_info->Ref();

	// I don't think we need this
	// SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimHumanoid, info_changed), &info);
}

void CAnmHAnimHumanoid::SetJoints(NodeArray *pJoints)
{
	assert(pJoints != NULL);

	SafeUnRef(m_joints);
	m_joints = pJoints;
	m_joints->Ref();

	// I don't think we need this
	// SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimHumanoid, joints_changed), &pJoints);
}

void CAnmHAnimHumanoid::SetName(String name)
{
	SafeUnRef(m_name);
	m_name = name;
	m_name->Ref();

	// I don't think we need this
	// SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimHumanoid, name_changed), &name);
}


void CAnmHAnimHumanoid::SetSegments(NodeArray *pSegments)
{
	assert(pSegments != NULL);

	SafeUnRef(m_segments);
	m_segments = pSegments;
	m_segments->Ref();

	// I don't think we need this
	// SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimHumanoid, segments_changed), &pSegments);
}


void CAnmHAnimHumanoid::SetSites(NodeArray *pSites)
{
	assert(pSites != NULL);

	SafeUnRef(m_sites);
	m_sites = pSites;
	m_sites->Ref();

	// I don't think we need this
	// SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimHumanoid, sites_changed), &pSites);
}

// These two are special; they pass thru to Group::Add/RemoveChildren
void CAnmHAnimHumanoid::SetSkeleton(NodeArray *pSkeleton)
{
	assert(pSkeleton != NULL);

	// Take previous skeleton nodes out of child  list
	RemoveChildren(m_skeleton);
	SafeUnRef(m_skeleton);

	// Add new skeleton nodes to child list
	AddChildren(pSkeleton);
	m_skeleton = pSkeleton;
	m_skeleton->Ref();

	CallCallbacks(CALLBACKID(CAnmHAnimHumanoid, skeleton_changed), &pSkeleton);

}

void CAnmHAnimHumanoid::SetSkin(NodeArray *pSkin)
{
	assert(pSkin != NULL);

	// Take previous skin nodes out of child  list
	RemoveChildren(m_skin);
	SafeUnRef(m_skin);

	// Add new skin nodes to child list
	AddChildren(pSkin);
	m_skin = pSkin;
	m_skin->Ref();

	CallCallbacks(CALLBACKID(CAnmHAnimHumanoid, skin_changed), &pSkin);
}

void CAnmHAnimHumanoid::SetSkinCoord(CAnmNode *pSkinCoord)
{
	SafeUnRef(m_skinCoord);
	m_skinCoord = pSkinCoord;
	m_skinCoord->Ref();

	SetDirtyBits(eAnmHumanoidSkinCoordDirty);

	CallCallbacks(CALLBACKID(CAnmHAnimHumanoid, skinCoord_changed), &pSkinCoord);
}

void CAnmHAnimHumanoid::SetSkinNormal(CAnmNode *pSkinNormal)
{
	SafeUnRef(m_skinNormal);
	m_skinNormal = pSkinNormal;
	m_skinNormal->Ref();

	SetDirtyBits(eAnmHumanoidSkinNormalDirty);

	CallCallbacks(CALLBACKID(CAnmHAnimHumanoid, skinNormal_changed), &pSkinNormal);
}

void CAnmHAnimHumanoid::SetVersion(String version)
{
	SafeUnRef(m_version);
	m_version = version;
	m_version->Ref();

	// I don't think we need this
	// SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimHumanoid, version_changed), &version);
}

void CAnmHAnimHumanoid::SetViewpoints(NodeArray *pViewpoints)
{
	assert(pViewpoints != NULL);

	SafeUnRef(m_viewpoints);
	m_viewpoints = pViewpoints;
	m_viewpoints->Ref();

	// I don't think we need this
	// SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimHumanoid, viewpoints_changed), &pViewpoints);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmHAnimHumanoid, bboxCenter, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimHumanoid, bboxSize, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimHumanoid, center, CAnmTransform);
DEFINE_VALUE(CAnmHAnimHumanoid, info, eValueStringArray, StringArray *, GetInfo, SetInfo);
DEFINE_VALUE(CAnmHAnimHumanoid, joints, eValueNodeArray, NodeArray*, GetJoints, SetJoints);
DEFINE_VALUE(CAnmHAnimHumanoid, name, eValueString, String, GetName, SetName);
DEFINE_INHERITED_VALUE(CAnmHAnimHumanoid, rotation, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimHumanoid, scale, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimHumanoid, scaleOrientation, CAnmTransform);
DEFINE_VALUE(CAnmHAnimHumanoid, segments, eValueNodeArray, NodeArray*, GetSegments, SetSegments);
DEFINE_VALUE(CAnmHAnimHumanoid, sites, eValueNodeArray, NodeArray*, GetSites, SetSites);
DEFINE_VALUE(CAnmHAnimHumanoid, skeleton, eValueNodeArray, NodeArray*, GetSkeleton, SetSkeleton);
DEFINE_VALUE(CAnmHAnimHumanoid, skin, eValueNodeArray, NodeArray*, GetSkin, SetSkin);
DEFINE_VALUE(CAnmHAnimHumanoid, skinCoord, eValueNode, CAnmNode*, GetSkinCoord, SetSkinCoord);
DEFINE_VALUE(CAnmHAnimHumanoid, skinNormal, eValueNode, CAnmNode*, GetSkinNormal, SetSkinNormal);
DEFINE_INHERITED_VALUE(CAnmHAnimHumanoid, translation, CAnmTransform);
DEFINE_VALUE(CAnmHAnimHumanoid, version, eValueString, String, GetVersion, SetVersion);
DEFINE_VALUE(CAnmHAnimHumanoid, viewpoints, eValueNodeArray, NodeArray*, GetViewpoints, SetViewpoints);

DEFINE_INHERITED_METHOD(CAnmHAnimHumanoid, set_center, CAnmTransform);
DEFINE_METHOD(CAnmHAnimHumanoid, set_info, eValueStringArray, StringArray *, SetInfo);
DEFINE_METHOD(CAnmHAnimHumanoid, set_joints, eValueNodeArray, NodeArray*, SetJoints);
DEFINE_METHOD(CAnmHAnimHumanoid, set_name, eValueString, String, SetName);
DEFINE_INHERITED_METHOD(CAnmHAnimHumanoid, set_rotation, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimHumanoid, set_scale, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimHumanoid, set_scaleOrientation, CAnmTransform);
DEFINE_METHOD(CAnmHAnimHumanoid, set_segments, eValueNodeArray, NodeArray*, SetSegments);
DEFINE_METHOD(CAnmHAnimHumanoid, set_sites, eValueNodeArray, NodeArray*, SetSites);
DEFINE_METHOD(CAnmHAnimHumanoid, set_skeleton, eValueNodeArray, NodeArray*, SetSkeleton);
DEFINE_METHOD(CAnmHAnimHumanoid, set_skin, eValueNodeArray, NodeArray*, SetSkin);
DEFINE_METHOD(CAnmHAnimHumanoid, set_skinCoord, eValueNode, CAnmNode*, SetSkinCoord);
DEFINE_METHOD(CAnmHAnimHumanoid, set_skinNormal, eValueNode, CAnmNode *, SetSkinNormal);
DEFINE_INHERITED_METHOD(CAnmHAnimHumanoid, set_translation, CAnmTransform);
DEFINE_METHOD(CAnmHAnimHumanoid, set_version, eValueString, String, SetVersion);
DEFINE_METHOD(CAnmHAnimHumanoid, set_viewpoints, eValueNodeArray, NodeArray*, SetViewpoints);

DEFINE_INHERITED_CALLBACK(CAnmHAnimHumanoid, center_changed, CAnmTransform);
DEFINE_CALLBACK(CAnmHAnimHumanoid, info_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmHAnimHumanoid, joints_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmHAnimHumanoid, name_changed, eValueString);
DEFINE_INHERITED_CALLBACK(CAnmHAnimHumanoid, rotation_changed, CAnmTransform);
DEFINE_INHERITED_CALLBACK(CAnmHAnimHumanoid, scale_changed, CAnmTransform);
DEFINE_INHERITED_CALLBACK(CAnmHAnimHumanoid, scaleOrientation_changed, CAnmTransform);
DEFINE_CALLBACK(CAnmHAnimHumanoid, segments_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmHAnimHumanoid, sites_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmHAnimHumanoid, skeleton_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmHAnimHumanoid, skin_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmHAnimHumanoid, skinCoord_changed, eValueNode);
DEFINE_CALLBACK(CAnmHAnimHumanoid, skinNormal_changed, eValueNode);
DEFINE_INHERITED_CALLBACK(CAnmHAnimHumanoid, translation_changed, CAnmTransform);
DEFINE_CALLBACK(CAnmHAnimHumanoid, version_changed, eValueString);
DEFINE_CALLBACK(CAnmHAnimHumanoid, viewpoints_changed, eValueNodeArray);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmHAnimHumanoid)
VALUEID(CAnmHAnimHumanoid, bboxCenter),
VALUEID(CAnmHAnimHumanoid, bboxSize),
VALUEID(CAnmHAnimHumanoid, center),
VALUEID(CAnmHAnimHumanoid, info),
VALUEID(CAnmHAnimHumanoid, joints),
VALUEID(CAnmHAnimHumanoid, name),
VALUEID(CAnmHAnimHumanoid, rotation),
VALUEID(CAnmHAnimHumanoid, scale),
VALUEID(CAnmHAnimHumanoid, scaleOrientation),
VALUEID(CAnmHAnimHumanoid, segments),
VALUEID(CAnmHAnimHumanoid, sites),
VALUEID(CAnmHAnimHumanoid, skeleton),
VALUEID(CAnmHAnimHumanoid, skin),
VALUEID(CAnmHAnimHumanoid, skinCoord),
VALUEID(CAnmHAnimHumanoid, skinNormal),
VALUEID(CAnmHAnimHumanoid, translation),
VALUEID(CAnmHAnimHumanoid, version),
VALUEID(CAnmHAnimHumanoid, viewpoints),

METHODID(CAnmHAnimHumanoid, set_center),
METHODID(CAnmHAnimHumanoid, set_info),
METHODID(CAnmHAnimHumanoid, set_joints),
METHODID(CAnmHAnimHumanoid, set_name),
METHODID(CAnmHAnimHumanoid, set_rotation),
METHODID(CAnmHAnimHumanoid, set_scale),
METHODID(CAnmHAnimHumanoid, set_scaleOrientation),
METHODID(CAnmHAnimHumanoid, set_segments),
METHODID(CAnmHAnimHumanoid, set_sites),
METHODID(CAnmHAnimHumanoid, set_skeleton),
METHODID(CAnmHAnimHumanoid, set_skin),
METHODID(CAnmHAnimHumanoid, set_skinCoord),
METHODID(CAnmHAnimHumanoid, set_skinNormal),
METHODID(CAnmHAnimHumanoid, set_translation),
METHODID(CAnmHAnimHumanoid, set_version),
METHODID(CAnmHAnimHumanoid, set_viewpoints),

CALLBACKID(CAnmHAnimHumanoid, center_changed),
CALLBACKID(CAnmHAnimHumanoid, info_changed),
CALLBACKID(CAnmHAnimHumanoid, joints_changed),
CALLBACKID(CAnmHAnimHumanoid, name_changed),
CALLBACKID(CAnmHAnimHumanoid, rotation_changed),
CALLBACKID(CAnmHAnimHumanoid, scale_changed),
CALLBACKID(CAnmHAnimHumanoid, scaleOrientation_changed),
CALLBACKID(CAnmHAnimHumanoid, segments_changed),
CALLBACKID(CAnmHAnimHumanoid, sites_changed),
CALLBACKID(CAnmHAnimHumanoid, skeleton_changed),
CALLBACKID(CAnmHAnimHumanoid, skin_changed),
CALLBACKID(CAnmHAnimHumanoid, skinCoord_changed),
CALLBACKID(CAnmHAnimHumanoid, skinNormal_changed),
CALLBACKID(CAnmHAnimHumanoid, translation_changed),
CALLBACKID(CAnmHAnimHumanoid, version_changed),
CALLBACKID(CAnmHAnimHumanoid, viewpoints_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmHAnimHumanoid, CAnmTransform);

