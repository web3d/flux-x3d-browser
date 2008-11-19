/********************************************************************************
 * Flux
 *
 * File: anmproximitysensor.cpp
 * Description: ProximitySensor class
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
#include "anmproximitysensor.h"
#include "anmcamera.h"
#include "anmlayer.h"
#include "anmtime.h"
#include "anmquaternion.h"
#include "anmupdatevisitor.h"
#include "anmvolumes.h"
#include "anmworld.h"
#include "anmMatrixInvert.h"

CAnmProximitySensor::CAnmProximitySensor() : 
CAnmSensor(ANMPROXSENSOR_DEFAULT_ENABLED),
m_center(ANMPROXSENSOR_DEFAULT_CENTER),
m_size(ANMPROXSENSOR_DEFAULT_SIZE),
m_wasActive(FALSE),
m_nowActive(FALSE),
m_previousPosition(ANMPROXSENSOR_BADPOINTVALUE),
m_currentPosition(ANMPROXSENSOR_BADPOINTVALUE),
m_previousOrientation(ANMPROXSENSOR_BADROTATIONVALUE),
m_currentOrientation(ANMPROXSENSOR_BADROTATIONVALUE)
{
	m_localMatrix = matrix4::Identity;
	m_camera = NULL;
	m_righthanded = false;
} 

CAnmProximitySensor::~CAnmProximitySensor()
{
}


void CAnmProximitySensor::Realize()
{
	CAnmSensor::Realize();		// just chain down for now

	CAnmLayer *pLayer = GetLayer();
	assert(pLayer);

	m_camera = pLayer->GetCamera();
	assert(m_camera);

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	if (pApp->GetOptions(eAnmOptionsCoordinateSystem) == eAnmCoordinatesRightHanded)
	{
		m_localMatrix._33 = -1;
		m_righthanded = true;
	}
}

void CAnmProximitySensor::Update()
{
	ClearStateDirty();
}


void CAnmProximitySensor::Poll()
{
	if (StateDirty())
		Update();

	// get center and size in world coords
	// local matrix was calculated during Render() last tick

	CAnmBoundingBox bbox(m_center, m_size);
	Point3 min, max;
	bbox.GetMinMax(&min, &max);

	// get current viewpoint position in world coordinates
	Point3 p, dir, up;
	m_camera->GetPosition(&p);
	m_camera->GetOrientation(&dir, &up);

	// translate viewpoint position to my local coordinates
	p = m_localMatrix * p;

	if (p.x >= min.x && p.x <= max.x && p.y >= min.y && p.y <= max.y
		&& p.z >= min.z && p.z <= max.z)
	{

		m_currentPosition = p;

		// get viewpoint orientation, convert to my local space,
		// then convert to a Rotation
		Point3 org = m_localMatrix * point3::Zero;
		// Pesky origin
		dir = m_localMatrix * dir - org;
		up = m_localMatrix * up - org;

		dir.Normalize();
		up.Normalize();

		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL

		cApplication *pApp = pWorld->GetApp();
		assert(pApp);

		matrix4 mat;
		if (!m_righthanded) // pApp->GetOptions(eAnmOptionsCoordinateSystem) == eAnmCoordinatesLeftHanded)
			mat.ToObjectLookAt(p, p + dir, up);
		else
			mat.ToObjectLookAt(p, p - dir, up);

		CAnmQuaternion q(mat);
		m_currentOrientation = q.AsRotation();

		// Did our quaternion flip us upside down? fun
		if (!(m_currentOrientation.vec == up))
		{
			m_currentOrientation.vec = - m_currentOrientation.vec;
			m_currentOrientation.theta = - m_currentOrientation.theta;
		}

		m_nowActive = TRUE;
	}
	else
		m_nowActive = FALSE;

	if (m_wasActive != m_nowActive)
	{
		HandleActive(m_nowActive);

	}

	if (m_nowActive)
	{
		if (!(m_currentPosition == m_previousPosition))
		{

			CallCallbacks(CALLBACKID(CAnmProximitySensor, position_changed), &m_currentPosition);
			m_previousPosition = m_currentPosition;
		}
		if (!(m_currentOrientation.vec == m_previousOrientation.vec)
			|| m_currentOrientation.theta != m_previousOrientation.theta)
		{

			CallCallbacks(CALLBACKID(CAnmProximitySensor, orientation_changed), &m_currentOrientation);
			m_previousOrientation = m_currentOrientation;
		}
	}
}

void CAnmProximitySensor::HandleActive(Boolean isActive)
{
	if (m_wasActive != isActive)
	{
		CallCallbacks(CALLBACKID(CAnmProximitySensor, isActive), &isActive);

		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL

		cApplication *pApp = pWorld->GetApp();
		assert(pApp);

		CAnmTime *pClock = pApp->GetClock();
		assert(pClock);

		Time t = pClock->GetCurrentTime();

		if (isActive)
			CallCallbacks(CALLBACKID(CAnmProximitySensor, enterTime), &t);
		else
			CallCallbacks(CALLBACKID(CAnmProximitySensor, exitTime), &t);

		m_wasActive = isActive;

		if (!isActive)
		{
			m_currentPosition = m_previousPosition = ANMPROXSENSOR_BADPOINTVALUE;
		}
	}
}


// N.B.: We use render traverse to get local matrix; only time to do it
void CAnmProximitySensor::Traverse(CAnmUpdateVisitor *pVisitor)
{
	assert(pVisitor);

	matrix4 worldmat;

	pVisitor->GetWorldMatrix(&worldmat);
	MatrixInvert(m_localMatrix, worldmat);

	CAnmSensor::Traverse(pVisitor);
}

void CAnmProximitySensor::SetCenter(Point3 center)
{
	m_center = center;

	SetMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmProximitySensor, center_changed), &center);
}

void CAnmProximitySensor::SetSize(Point3 size)
{
	m_size = size;

	SetMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmProximitySensor, size_changed), &size);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmProximitySensor, center, eValuePoint3, Point3, GetCenter, SetCenter);
DEFINE_VALUE(CAnmProximitySensor, enabled, eValueBoolean, Boolean, GetEnabled, SetEnabled);
DEFINE_VALUE(CAnmProximitySensor, size, eValuePoint3, Point3, GetSize, SetSize);


DEFINE_METHOD(CAnmProximitySensor, set_center, eValuePoint3, Point3, SetCenter);
DEFINE_METHOD(CAnmProximitySensor, set_enabled, eValueBoolean, Boolean, SetEnabled);
DEFINE_METHOD(CAnmProximitySensor, set_size, eValuePoint3, Point3, SetSize);

DEFINE_CALLBACK(CAnmProximitySensor, center_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmProximitySensor, enabled_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmProximitySensor, size_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmProximitySensor, isActive, eValueBoolean);
DEFINE_CALLBACK(CAnmProximitySensor, position_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmProximitySensor, orientation_changed, eValueRotation);
DEFINE_CALLBACK(CAnmProximitySensor, enterTime, eValueTime);
DEFINE_CALLBACK(CAnmProximitySensor, exitTime, eValueTime);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmProximitySensor)
VALUEID(CAnmProximitySensor, center),
VALUEID(CAnmProximitySensor, enabled),
VALUEID(CAnmProximitySensor, size),

METHODID(CAnmProximitySensor, set_center),
METHODID(CAnmProximitySensor, set_enabled),
METHODID(CAnmProximitySensor, set_size),

CALLBACKID(CAnmProximitySensor, center_changed),
CALLBACKID(CAnmProximitySensor, enabled_changed),
CALLBACKID(CAnmProximitySensor, size_changed),
CALLBACKID(CAnmProximitySensor, isActive),
CALLBACKID(CAnmProximitySensor, position_changed),
CALLBACKID(CAnmProximitySensor, orientation_changed),
CALLBACKID(CAnmProximitySensor, enterTime),
CALLBACKID(CAnmProximitySensor, exitTime),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmProximitySensor, CAnmSensor);
