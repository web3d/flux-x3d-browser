/********************************************************************************
 * Flux
 *
 * File: anmspheresensor.cpp
 * Description: SphereSensor class
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
#include "anmspheresensor.h"
#include "anmquaternion.h"
#include "anmlayer.h"


CAnmSphereSensor::CAnmSphereSensor()
: CAnmPickSensor(SPHERESENSOR_DEFAULT_ENABLED),
m_offset(SPHERESENSOR_DEFAULT_OFFSET),
m_autoOffset(SPHERESENSOR_DEFAULT_AUTOOFFSET),
m_hitpoint(ANMPICKSENSOR_BADPOINTVALUE),
m_trackPoint(ANMPICKSENSOR_BADPOINTVALUE),
m_lastRotation(SPHERESENSOR_DEFAULT_OFFSET),
m_InitMousex( 0 ),
m_InitMousey( 0 )
{
}

CAnmSphereSensor::~CAnmSphereSensor()
{
}

void CAnmSphereSensor::Realize()
{
	CAnmPickSensor::Realize();
}

void CAnmSphereSensor::Update()
{
	ClearStateDirty();
}

#define SPHERESENSOR_EPSILON 0.000001

//		Gain in Radians Per Pixel
//
#define SPHERESENSOR_GAIN    0.01f




void CAnmSphereSensor::HandleActive(Boolean active, Point3 from, Point3 to, Point3 hitpoint)
{
	if (active)
	{

		// Get Normailized Bearing Vector
		m_vBearing = ( to - from );
		m_vBearing.Normalize();


		// get initial track point, and compute radius (magnitude of vector)
		m_hitpoint = from;

		// Get mouse position from the pick manager
		CAnmLayer *pLayer = GetLayer();
		assert(pLayer);

		CAnmPickManager *pPickManager = pLayer->GetPickManager();
		assert(pPickManager);

		m_InitMousex = pPickManager->GetMouseX();
		m_InitMousey = pPickManager->GetMouseY();

		m_lastRotation = m_offset;
	}
	else
	{
		if (m_autoOffset)
			SetOffset(m_lastRotation);
	}
	// chain down to get basic activate/deactive behavior for all picking sensors
	CAnmPickSensor::HandleActive(active, from, to, hitpoint);
}

void CAnmSphereSensor::HandleMove(Point3 from, Point3 to, Point3 hitpoint)
{

	// Get mouse position from the pick manager
	CAnmLayer *pLayer = GetLayer();
	assert(pLayer);

	CAnmPickManager *pPickManager = pLayer->GetPickManager();
	assert(pPickManager);
	
	float fDxPixel = ( float ) ( pPickManager->GetMouseX() - m_InitMousex );
	float fDyPixel = ( float ) ( pPickManager->GetMouseY() - m_InitMousey );
	float dPixel = sqrt( fDxPixel*fDxPixel + fDyPixel*fDyPixel );

	Point3 Delta = from - m_hitpoint;

	// Get magnitude of angle
	//
	float mag = Delta.Mag();

	if( mag > SPHERESENSOR_EPSILON ) {

		// Get axis of rotation
		//
		Delta.Normalize();
		Point3 cross = Delta^m_vBearing;

		m_lastRotation.vec = cross;
		m_lastRotation.theta = SPHERESENSOR_GAIN * dPixel;

		m_trackPoint = hitpoint;

		// Add the offset.
		//
		CAnmQuaternion q1, q2, q3;
		q1 = m_lastRotation;
		q2 = m_offset;
		q3 = q1 * q2;
		m_lastRotation = q3.AsRotation();

		CallCallbacks(CALLBACKID(CAnmSphereSensor, trackPoint_changed), &m_trackPoint);
		CallCallbacks(CALLBACKID(CAnmSphereSensor, rotation_changed), &m_lastRotation);
	}
}


cCursor CAnmSphereSensor::GetOverCursor()
{
	return cApplication::sphereSensorCursor;
}

cCursor CAnmSphereSensor::GetActiveCursor()
{
	return cApplication::sphereSensorCursor;
}

// Accessors
void CAnmSphereSensor::SetAutoOffset(Boolean autoOffset)
{
	m_autoOffset = autoOffset;
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmSphereSensor, autoOffset_changed), &autoOffset);
}

void CAnmSphereSensor::SetOffset(Rotation offset)
{
	m_offset = offset;
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmSphereSensor, offset_changed), &offset);
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmSphereSensor, autoOffset, eValueBoolean, Boolean, GetAutoOffset, SetAutoOffset);
DEFINE_INHERITED_VALUE(CAnmSphereSensor, enabled, CAnmPickSensor);
DEFINE_VALUE(CAnmSphereSensor, offset, eValueRotation, Rotation, GetOffset, SetOffset);

DEFINE_METHOD(CAnmSphereSensor, set_autoOffset, eValueBoolean, Boolean, SetAutoOffset);
DEFINE_INHERITED_METHOD(CAnmSphereSensor, set_enabled, CAnmPickSensor);
DEFINE_METHOD(CAnmSphereSensor, set_offset, eValueRotation, Rotation, SetOffset);

DEFINE_CALLBACK(CAnmSphereSensor, autoOffset_changed, eValueBoolean);
DEFINE_INHERITED_CALLBACK(CAnmSphereSensor, enabled_changed, CAnmPickSensor);
DEFINE_CALLBACK(CAnmSphereSensor, offset_changed, eValueRotation);
DEFINE_CALLBACK(CAnmSphereSensor, rotation_changed, eValueRotation);
DEFINE_CALLBACK(CAnmSphereSensor, trackPoint_changed, eValuePoint3);
DEFINE_INHERITED_CALLBACK(CAnmSphereSensor, isActive, CAnmPickSensor);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmSphereSensor)
VALUEID(CAnmSphereSensor, autoOffset),
VALUEID(CAnmSphereSensor, enabled),
VALUEID(CAnmSphereSensor, offset),

METHODID(CAnmSphereSensor, set_autoOffset),
METHODID(CAnmSphereSensor, set_enabled),
METHODID(CAnmSphereSensor, set_offset),

CALLBACKID(CAnmSphereSensor, autoOffset_changed),
CALLBACKID(CAnmSphereSensor, enabled_changed),
CALLBACKID(CAnmSphereSensor, offset_changed),
CALLBACKID(CAnmSphereSensor, rotation_changed),
CALLBACKID(CAnmSphereSensor, trackPoint_changed),
CALLBACKID(CAnmSphereSensor, isActive),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmSphereSensor, CAnmPickSensor);
