/********************************************************************************
 * Flux
 *
 * File: anmvisibilitysensor.cpp
 * Description: VisibilitySensor class
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
#include "anmvisibilitysensor.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmfrustum.h"
#include "anmlayer.h"
#include "anmtime.h"
#include "anmupdatevisitor.h"
#include "anmviewport.h"
#include "anmvolumes.h"
#include "anmworld.h"

CAnmVisibilitySensor::CAnmVisibilitySensor() : 
CAnmSensor(ANMVISSENSOR_DEFAULT_ENABLED),
m_center(ANMVISSENSOR_DEFAULT_CENTER),
m_size(ANMVISSENSOR_DEFAULT_SIZE),
m_wasActive(false),
m_nowActive(false)
{
	m_localMatrix = matrix4::Identity;
} 

CAnmVisibilitySensor::~CAnmVisibilitySensor()
{
}


void CAnmVisibilitySensor::Realize()
{
	CAnmSensor::Realize();		// just chain down for now
}

void CAnmVisibilitySensor::Update()
{
	ClearStateDirty();
}

class CAnmFrustum *CAnmVisibilitySensor::GetFrustum()
{
	CAnmLayer *pLayer = GetLayer();
	assert(pLayer);

	CAnmCamera *pCamera = pLayer->GetCamera();
	assert(pCamera);

	return pCamera->GetFrustum();
}

void CAnmVisibilitySensor::Poll()
{
	if (StateDirty())
		Update();

	// Get current view frustum
	CAnmFrustum *pFrustum = GetFrustum();
	assert(pFrustum);

	// get our box in world coords
	// local matrix was calculated during Render() last tick

	CAnmBoundingBox bbox(m_center, m_size);
	CAnmBoundingBox tbbox = bbox.Transform(m_localMatrix);

	m_nowActive = ! pFrustum->Cull(tbbox);

	if (m_wasActive != m_nowActive)
	{
		HandleActive(m_nowActive);

	}
}

void CAnmVisibilitySensor::HandleActive(Boolean isActive)
{
	if (m_wasActive != isActive)
	{
		CallCallbacks(CALLBACKID(CAnmVisibilitySensor, isActive), &isActive);

		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL

		cApplication *pApp = pWorld->GetApp();
		assert(pApp);

		CAnmTime *pClock = pApp->GetClock();
		assert(pClock);

		Time t = pClock->GetCurrentTime();

		if (isActive)
			CallCallbacks(CALLBACKID(CAnmVisibilitySensor, enterTime), &t);
		else
			CallCallbacks(CALLBACKID(CAnmVisibilitySensor, exitTime), &t);

		m_wasActive = isActive;
	}
}


// N.B.: We use render traverse to get local matrix; only time to do it
void CAnmVisibilitySensor::Traverse(CAnmUpdateVisitor *pVisitor)
{
	assert(pVisitor);

	pVisitor->GetWorldMatrix(&m_localMatrix);

	CAnmSensor::Traverse(pVisitor);
}

void CAnmVisibilitySensor::SetCenter(Point3 center)
{
	m_center = center;

	SetMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmVisibilitySensor, center_changed), &center);
}

void CAnmVisibilitySensor::SetSize(Point3 size)
{
	m_size = size;

	SetMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmVisibilitySensor, size_changed), &size);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmVisibilitySensor, center, eValuePoint3, Point3, GetCenter, SetCenter);
DEFINE_VALUE(CAnmVisibilitySensor, enabled, eValueBoolean, Boolean, GetEnabled, SetEnabled);
DEFINE_VALUE(CAnmVisibilitySensor, size, eValuePoint3, Point3, GetSize, SetSize);


DEFINE_METHOD(CAnmVisibilitySensor, set_center, eValuePoint3, Point3, SetCenter);
DEFINE_METHOD(CAnmVisibilitySensor, set_enabled, eValueBoolean, Boolean, SetEnabled);
DEFINE_METHOD(CAnmVisibilitySensor, set_size, eValuePoint3, Point3, SetSize);

DEFINE_CALLBACK(CAnmVisibilitySensor, center_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmVisibilitySensor, enabled_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmVisibilitySensor, size_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmVisibilitySensor, isActive, eValueBoolean);
DEFINE_CALLBACK(CAnmVisibilitySensor, enterTime, eValueTime);
DEFINE_CALLBACK(CAnmVisibilitySensor, exitTime, eValueTime);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmVisibilitySensor)
VALUEID(CAnmVisibilitySensor, center),
VALUEID(CAnmVisibilitySensor, enabled),
VALUEID(CAnmVisibilitySensor, size),

METHODID(CAnmVisibilitySensor, set_center),
METHODID(CAnmVisibilitySensor, set_enabled),
METHODID(CAnmVisibilitySensor, set_size),

CALLBACKID(CAnmVisibilitySensor, center_changed),
CALLBACKID(CAnmVisibilitySensor, enabled_changed),
CALLBACKID(CAnmVisibilitySensor, size_changed),
CALLBACKID(CAnmVisibilitySensor, isActive),
CALLBACKID(CAnmVisibilitySensor, enterTime),
CALLBACKID(CAnmVisibilitySensor, exitTime),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmVisibilitySensor, CAnmSensor);
