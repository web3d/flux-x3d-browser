/********************************************************************************
 * Flux
 *
 * File: anmlod.cpp
 * Description: LOD node class
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

#include "anmlod.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmproximitysensor.h"
#include "anmviewport.h"
#include "anmvisitor.h"
	
CAnmLOD::CAnmLOD() : CAnmGroup (),
	m_center(ANMLOD_DEFAULT_CENTER)
{
	m_range = new FloatArray;
	m_proxSensor = new CAnmProximitySensor;
	m_proxSensor->SetOwner(this);
	m_proxSensor->AddCallback(CALLBACKID(CAnmProximitySensor, position_changed), CameraChangedCallback, this);
	m_proxSensor->SetSize(Point3(FLT_MAX, FLT_MAX, FLT_MAX));
	m_levelToDisplay = NULL;
	m_isSwitch = true;
	m_whichChoice = ANMGROUP_DEFAULT_WHICHCHOICE;
}

CAnmLOD::~CAnmLOD()
{
	SafeUnRef(m_range);
	SafeUnRef(m_proxSensor);
}

void CAnmLOD::Realize()
{
	// N.B.: anything? just chain to group? maybe we don't want to realize children
	// if they're never switched in
	CAnmGroup::Realize();

	m_proxSensor->Realize();
}

float CAnmLOD::GetRadius()
{
	// N.B.: I think this is correct: LOD bounding volume is volume of largest child
	return CAnmGroup::GetRadius();
}

void CAnmLOD::GetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	// N.B.: I think this is correct: LOD bounding volume is volume of largest child
	CAnmGroup::GetBoundingBox(pBoundingBox);
}

void CAnmLOD::GetObjectCenter(Point3 *pCenter)
{
	// N.B.: I think this is correct: LOD bounding volume is volume of largest child
	CAnmGroup::GetObjectCenter(pCenter);
}

// helper
CAnmNode *CAnmLOD::getLevelToDisplay(Point3 camerapos)
{
	Point3 lod2view = m_center - camerapos;

	float dist = lod2view.Mag();

	int nRanges = m_range->size();

	for (int range = 0; range < nRanges; range++)
	{
		if (dist < (*m_range)[range])
			break;
	}

	int nChildren = m_children.size();
	CAnmNode *pChild = NULL;

	if (range >= nChildren)
		range = nChildren - 1;

	int n = 0;
	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		if (n == range)
		{
			pChild = (*iter);
			break;
		}

		n++;
	}

	return pChild;
}

// Accessors
void CAnmLOD::SetCenter(Point3 p)
{
	m_center = p;

	// N.B.: any dirty bits needed?
	SetMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmLOD, center_changed), &p);
}

void CAnmLOD::SetLevel(NodeArray *pLevel)
{
	SetChildren(pLevel);

	// N.B.: Group class's dirty bit will be set

	CallCallbacks(CALLBACKID(CAnmLOD, level_changed), &pLevel);
}

void CAnmLOD::SetRange(FloatArray *pRange)
{
	assert(pRange != NULL);

	SafeUnRef(m_range);
	m_range = pRange;
	m_range->Ref();

	// N.B.: any dirty bits needed?
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmLOD, range_changed), &pRange);
}

void CAnmLOD::CameraChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData)
{
	CAnmProximitySensor *pPS = (CAnmProximitySensor *) sourceObject;
	CAnmLOD *pLOD = (CAnmLOD *) userData;
	Point3 *pp =  (Point3 *) callData;

	pLOD->HandleCameraChanged(*pp);
}

void CAnmLOD::HandleCameraChanged(Point3 camerapos)
{
	CAnmNode *pLevelToDisplay = getLevelToDisplay(camerapos);
	if (pLevelToDisplay != m_levelToDisplay)
	{
		m_levelToDisplay = pLevelToDisplay;

		SetChildrenDirty();
		SetStateDirty();
	}
}

void CAnmLOD::Traverse(CAnmUpdateVisitor *pVisitor)
{
	// Make sure to update the proximity sensor if we're re-traversing
	if (TestDirtyBits(ANMGROUPDIRTY_CHECKTRAVERSECHILDREN))
		m_proxSensor->Traverse(pVisitor);

	CAnmGroup::Traverse(pVisitor);
}

void CAnmLOD::PreTraverse()
{
	CAnmGroup::PreTraverse();

	m_proxSensor->PreTraverse();
}

void CAnmLOD::PostTraverse()
{
	CAnmGroup::PostTraverse();

	m_proxSensor->PostTraverse();
}

void CAnmLOD::Update()
{
	m_whichChoice = ANMGROUP_DEFAULT_WHICHCHOICE;

	int n = 0;

	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		if ((*iter) == m_levelToDisplay)
		{
			m_whichChoice = n;
			break;
		}

		n++;
	}

	ClearStateDirty();
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmLOD, center, eValuePoint3, Point3, GetCenter, SetCenter);
DEFINE_INHERITED_VALUE(CAnmLOD, children, CAnmGroup);
DEFINE_VALUE(CAnmLOD, level, eValueNodeArray, NodeArray *, GetLevel, SetLevel);
DEFINE_VALUE(CAnmLOD, range, eValueFloatArray, FloatArray *, GetRange, SetRange);

DEFINE_METHOD(CAnmLOD, set_center, eValuePoint3, Point3, SetCenter);
DEFINE_INHERITED_METHOD(CAnmLOD, set_children, CAnmGroup);
DEFINE_METHOD(CAnmLOD, set_level, eValueNodeArray, NodeArray *, SetLevel);
DEFINE_METHOD(CAnmLOD, set_range, eValueFloatArray, FloatArray *, SetRange);

DEFINE_CALLBACK(CAnmLOD, center_changed, eValuePoint3);
DEFINE_INHERITED_CALLBACK(CAnmLOD, children_changed, CAnmGroup);
DEFINE_CALLBACK(CAnmLOD, level_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmLOD, range_changed, eValueFloatArray);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmLOD)
VALUEID(CAnmLOD, center),
VALUEID(CAnmLOD, children),
VALUEID(CAnmLOD, level),
VALUEID(CAnmLOD, range),
METHODID(CAnmLOD, set_center),
METHODID(CAnmLOD, set_children),
METHODID(CAnmLOD, set_level),
METHODID(CAnmLOD, set_range),
CALLBACKID(CAnmLOD, center_changed),
CALLBACKID(CAnmLOD, children_changed),
CALLBACKID(CAnmLOD, level_changed),
CALLBACKID(CAnmLOD, range_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmLOD, CAnmGroup);

