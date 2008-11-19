/********************************************************************************
 * Flux
 *
 * File: anmtouchsensor.cpp
 * Description: TouchSensor class
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
#include "anmtouchsensor.h"
#include "anmworld.h"

CAnmTouchSensor::CAnmTouchSensor()
: CAnmPickSensor(TOUCHSENSOR_DEFAULT_ENABLED)
{
	m_hitpoint = ANMPICKSENSOR_BADPOINTVALUE;
}

CAnmTouchSensor::~CAnmTouchSensor()
{
}

void CAnmTouchSensor::Realize()
{
	CAnmPickSensor::Realize();
}

void CAnmTouchSensor::Update()
{
	ClearStateDirty();
}

void CAnmTouchSensor::HandleActive(Boolean active, Point3 from, Point3 to, Point3 hitpoint)
{
	if (!active)
	{
		// Deactivate (i.e. mouse up) triggers the touchTime callbacks
		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL

		cApplication *pApp = pWorld->GetApp();
		assert(pApp);

		CAnmTime *pClock = pApp->GetClock();
		assert(pClock);

		Time now = pClock->GetCurrentTime();
		CallCallbacks(CALLBACKID(CAnmTouchSensor, touchTime), &now);

		m_hitpoint = ANMPICKSENSOR_BADPOINTVALUE;
	}

	// Chain down to get basic activate/deactive behavior for all picking sensors
	CAnmPickSensor::HandleActive(active, from, to, hitpoint);
}

void CAnmTouchSensor::HandleMove(Point3 from, Point3 to, Point3 hitpoint)
{
	// Only report moves if the mouse is actually over our geometry
	if (m_over)
	{
		// Eventually deal with changes to hitNormal and hitTexCoord

		if (!(hitpoint == m_hitpoint))
		{
			CallCallbacks(CALLBACKID(CAnmTouchSensor, hitPoint_changed), &hitpoint);
			m_hitpoint = hitpoint;
		}
	}
}


cCursor CAnmTouchSensor::GetOverCursor()
{
	return cApplication::touchSensorCursor;
}

cCursor CAnmTouchSensor::GetActiveCursor()
{
	return cApplication::touchSensorCursor;
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_INHERITED_VALUE(CAnmTouchSensor, enabled, CAnmPickSensor);

DEFINE_INHERITED_METHOD(CAnmTouchSensor, set_enabled, CAnmPickSensor);

DEFINE_INHERITED_CALLBACK(CAnmTouchSensor, enabled_changed, CAnmPickSensor);
DEFINE_CALLBACK(CAnmTouchSensor, hitNormal_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmTouchSensor, hitPoint_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmTouchSensor, hitTexCoord_changed, eValuePoint2);
DEFINE_INHERITED_CALLBACK(CAnmTouchSensor, isActive, CAnmPickSensor);
DEFINE_INHERITED_CALLBACK(CAnmTouchSensor, isOver, CAnmPickSensor);
DEFINE_CALLBACK(CAnmTouchSensor, touchTime, eValueTime);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmTouchSensor)
VALUEID(CAnmTouchSensor, enabled),

METHODID(CAnmTouchSensor, set_enabled),

CALLBACKID(CAnmTouchSensor, enabled_changed),
CALLBACKID(CAnmTouchSensor, hitNormal_changed),
CALLBACKID(CAnmTouchSensor, hitPoint_changed),
CALLBACKID(CAnmTouchSensor, hitTexCoord_changed),
CALLBACKID(CAnmTouchSensor, isActive),
CALLBACKID(CAnmTouchSensor, isOver),
CALLBACKID(CAnmTouchSensor, touchTime),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmTouchSensor, CAnmPickSensor);
