/********************************************************************************
 * Flux
 *
 * File: anmplanesensor.cpp
 * Description: PlaneSensor class
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
#include "anmplanesensor.h"

CAnmPlaneSensor::CAnmPlaneSensor()
: CAnmPickSensor(PLANESENSOR_DEFAULT_ENABLED),
m_minPosition(PLANESENSOR_DEFAULT_MINPOSITION),
m_maxPosition(PLANESENSOR_DEFAULT_MAXPOSITION),
m_offset(PLANESENSOR_DEFAULT_OFFSET),
m_autoOffset(PLANESENSOR_DEFAULT_AUTOOFFSET),
m_hitpoint(ANMPICKSENSOR_BADPOINTVALUE),
m_lastDragPoint(ANMPICKSENSOR_BADPOINTVALUE),
m_lastTranslation(PLANESENSOR_DEFAULT_OFFSET)
{
}

CAnmPlaneSensor::~CAnmPlaneSensor()
{
}

void CAnmPlaneSensor::Realize()
{
	CAnmPickSensor::Realize();
}

void CAnmPlaneSensor::Update()
{
	ClearStateDirty();
}

void CAnmPlaneSensor::HandleActive(Boolean active, Point3 from, Point3 to, Point3 hitpoint)
{
	if (active)
	{
		// Save initial drag point
		m_hitpoint = hitpoint;

		CalcPlaneDrag(&m_lastDragPoint, from, to);
	}
	else
	{
		if (m_autoOffset)
			SetOffset(m_lastTranslation);
	}

	// chain down to get basic activate/deactive behavior for all picking sensors
	CAnmPickSensor::HandleActive(active, from, to, hitpoint);
}

void CAnmPlaneSensor::HandleMove(Point3 from, Point3 to, Point3 hitpoint)
{
	Point3 dragpoint, trackPoint;

	CalcPlaneDrag(&dragpoint, from, to);

	// Calculate translation (clamped)
	m_lastTranslation = dragpoint - m_lastDragPoint + m_offset;

	// try clamping the x,y values
	bool validmaxx = m_maxPosition.x >= m_minPosition.x;
	bool validmaxy = m_maxPosition.y >= m_minPosition.y;

	if (validmaxx)
	{
		if (m_lastTranslation.x > m_maxPosition.x)
			m_lastTranslation.x = m_maxPosition.x;
		if (m_lastTranslation.x < m_minPosition.x)
			m_lastTranslation.x = m_minPosition.x;
	}

	if (validmaxy)
	{
		if (m_lastTranslation.y > m_maxPosition.y)
			m_lastTranslation.y = m_maxPosition.y;
		if (m_lastTranslation.y < m_minPosition.y)
			m_lastTranslation.y = m_minPosition.y;
	}

	// Calculate track point (unclamped)
	trackPoint = m_hitpoint + dragpoint - m_lastDragPoint;

	CallCallbacks(CALLBACKID(CAnmPlaneSensor, trackPoint_changed), &trackPoint);

	// N.B.: need to clamp the X,Y values eventually
	CallCallbacks(CALLBACKID(CAnmPlaneSensor, translation_changed), &m_lastTranslation);

}

eAnmReturnStatus CAnmPlaneSensor::CalcPlaneDrag(Point3 *pDrag, Point3 from, Point3 to)
{	
	// Get the line of sight
	Point3 dir = to - from;

	dir.Normalize();

	// Constrain motion along the sensor's XY plane
	if (dir.z == 0.f)
	{
		// should this ever happen? not sure what we can do.
		return eAnmReturnAllBad;
	}

	Float ratio = from.z / dir.z;

	// move X and Y coordinates by ratio, but constrain Z
	pDrag->x = from.x - dir.x * ratio;
	pDrag->y = from.y - dir.y * ratio;
	pDrag->z = 0;

	return eAnmReturnAllGood;
}

cCursor CAnmPlaneSensor::GetOverCursor()
{
	return cApplication::planeSensorCursor;
}

cCursor CAnmPlaneSensor::GetActiveCursor()
{
	return cApplication::planeSensorCursor;
}

// Accessors
void CAnmPlaneSensor::SetAutoOffset(Boolean autoOffset)
{
	m_autoOffset = autoOffset;
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmPlaneSensor, autoOffset_changed), &autoOffset);
}

void CAnmPlaneSensor::SetMaxPosition(Point2 maxPosition)
{
	m_maxPosition = maxPosition;
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmPlaneSensor, maxPosition_changed), &maxPosition);
}

void CAnmPlaneSensor::SetMinPosition(Point2 minPosition)
{
	m_minPosition = minPosition;
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmPlaneSensor, minPosition_changed), &minPosition);
}

void CAnmPlaneSensor::SetOffset(Point3 offset)
{
	m_offset = offset;
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmPlaneSensor, offset_changed), &offset);
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmPlaneSensor, autoOffset, eValueBoolean, Boolean, GetAutoOffset, SetAutoOffset);
DEFINE_INHERITED_VALUE(CAnmPlaneSensor, enabled, CAnmPickSensor);
DEFINE_VALUE(CAnmPlaneSensor, maxPosition, eValuePoint2, Point2, GetMaxPosition, SetMaxPosition);
DEFINE_VALUE(CAnmPlaneSensor, minPosition, eValuePoint2, Point2, GetMinPosition, SetMinPosition);
DEFINE_VALUE(CAnmPlaneSensor, offset, eValuePoint3, Point3, GetOffset, SetOffset);

DEFINE_METHOD(CAnmPlaneSensor, set_autoOffset, eValueBoolean, Boolean, SetAutoOffset);
DEFINE_INHERITED_METHOD(CAnmPlaneSensor, set_enabled, CAnmPickSensor);
DEFINE_METHOD(CAnmPlaneSensor, set_maxPosition, eValuePoint2, Point2, SetMaxPosition);
DEFINE_METHOD(CAnmPlaneSensor, set_minPosition, eValuePoint2, Point2, SetMinPosition);
DEFINE_METHOD(CAnmPlaneSensor, set_offset, eValuePoint3, Point3, SetOffset);

DEFINE_CALLBACK(CAnmPlaneSensor, autoOffset_changed, eValueBoolean);
DEFINE_INHERITED_CALLBACK(CAnmPlaneSensor, enabled_changed, CAnmPickSensor);
DEFINE_CALLBACK(CAnmPlaneSensor, maxPosition_changed, eValuePoint2);
DEFINE_CALLBACK(CAnmPlaneSensor, minPosition_changed, eValuePoint2);
DEFINE_CALLBACK(CAnmPlaneSensor, offset_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmPlaneSensor, trackPoint_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmPlaneSensor, translation_changed, eValuePoint3);
DEFINE_INHERITED_CALLBACK(CAnmPlaneSensor, isActive, CAnmPickSensor);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmPlaneSensor)
VALUEID(CAnmPlaneSensor, autoOffset),
VALUEID(CAnmPlaneSensor, enabled),
VALUEID(CAnmPlaneSensor, maxPosition),
VALUEID(CAnmPlaneSensor, minPosition),
VALUEID(CAnmPlaneSensor, offset),

METHODID(CAnmPlaneSensor, set_autoOffset),
METHODID(CAnmPlaneSensor, set_enabled),
METHODID(CAnmPlaneSensor, set_maxPosition),
METHODID(CAnmPlaneSensor, set_minPosition),
METHODID(CAnmPlaneSensor, set_offset),

CALLBACKID(CAnmPlaneSensor, autoOffset_changed),
CALLBACKID(CAnmPlaneSensor, enabled_changed),
CALLBACKID(CAnmPlaneSensor, maxPosition_changed),
CALLBACKID(CAnmPlaneSensor, minPosition_changed),
CALLBACKID(CAnmPlaneSensor, offset_changed),
CALLBACKID(CAnmPlaneSensor, trackPoint_changed),
CALLBACKID(CAnmPlaneSensor, translation_changed),
CALLBACKID(CAnmPlaneSensor, isActive),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmPlaneSensor, CAnmPickSensor);
