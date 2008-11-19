/********************************************************************************
 * Flux
 *
 * File: anmmousesensor.cpp
 * Description: MouseSensor class
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
#include "anmmousesensor.h"
#include "anmworld.h"


CAnmMouseSensor::CAnmMouseSensor()
: CAnmSensor(ANMMOUSESENSOR_DEFAULT_ENABLED)
{
	m_mousepoint = ANMMOUSESENSOR_BAD_POINTVAL;

	m_leftButtonDown = m_middleButtonDown = m_rightButtonDown = false;
}

CAnmMouseSensor::~CAnmMouseSensor()
{
}

void CAnmMouseSensor::Realize()
{
	// Chain to superclass for basic Realize stuff
	CAnmSensor::Realize();
}

void CAnmMouseSensor::Update()
{
	ClearStateDirty();
}

void CAnmMouseSensor::Poll()
{
	// shouldn't be here otherwise
	assert(m_enabled);

	// Make sure all the data is up to date
	if (StateDirty())
		Update();

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	int mousex, mousey, mousebuttons;

	pApp->GetMouseInfo(&mousex, &mousey, &mousebuttons);

	if (m_mousepoint.x != mousex || m_mousepoint.y != mousey)
	{
		m_mousepoint.x = mousex;
		m_mousepoint.y = mousey;

		CallCallbacks(CALLBACKID(CAnmMouseSensor, position_changed), &m_mousepoint);
	}

	bool leftButtonDown = (mousebuttons & ANM_LBUTTON);
	bool middleButtonDown = (mousebuttons & ANM_MBUTTON);
	bool rightButtonDown = (mousebuttons & ANM_RBUTTON);

	if (leftButtonDown != m_leftButtonDown)
	{
		m_leftButtonDown = leftButtonDown;
		CallCallbacks(CALLBACKID(CAnmMouseSensor, leftButtonDown), &leftButtonDown);
	}

	if (middleButtonDown != m_middleButtonDown)
	{
		m_middleButtonDown = middleButtonDown;
		CallCallbacks(CALLBACKID(CAnmMouseSensor, middleButtonDown), &middleButtonDown);
	}

	if (rightButtonDown != m_rightButtonDown)
	{
		m_rightButtonDown = rightButtonDown;
		CallCallbacks(CALLBACKID(CAnmMouseSensor, rightButtonDown), &rightButtonDown);
	}

}

void CAnmMouseSensor::SetEnabled(Boolean enabled)
{
	m_mousepoint = ANMMOUSESENSOR_BAD_POINTVAL;

	m_leftButtonDown = m_middleButtonDown = m_rightButtonDown = false;

	CAnmSensor::SetEnabled(enabled);
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_INHERITED_VALUE(CAnmMouseSensor, enabled, CAnmSensor);

DEFINE_INHERITED_METHOD(CAnmMouseSensor, set_enabled, CAnmSensor);

DEFINE_INHERITED_CALLBACK(CAnmMouseSensor, enabled_changed, CAnmSensor);

DEFINE_CALLBACK(CAnmMouseSensor, position_changed, eValuePoint2);
DEFINE_CALLBACK(CAnmMouseSensor, leftButtonDown, eValueBoolean);
DEFINE_CALLBACK(CAnmMouseSensor, middleButtonDown, eValueBoolean);
DEFINE_CALLBACK(CAnmMouseSensor, rightButtonDown, eValueBoolean);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmMouseSensor)
VALUEID(CAnmMouseSensor, enabled),

METHODID(CAnmMouseSensor, set_enabled),

CALLBACKID(CAnmMouseSensor, enabled_changed),
CALLBACKID(CAnmMouseSensor, position_changed),
CALLBACKID(CAnmMouseSensor, leftButtonDown),
CALLBACKID(CAnmMouseSensor, middleButtonDown),
CALLBACKID(CAnmMouseSensor, rightButtonDown),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmMouseSensor, CAnmSensor);
