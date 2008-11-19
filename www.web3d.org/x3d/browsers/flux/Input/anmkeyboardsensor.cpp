/********************************************************************************
 * Flux
 *
 * File: anmkeyboardsensor.cpp
 * Description: KeyboardSensor class
 *							base class for keyboard input
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
#include "anmkeyboardsensor.h"

CAnmKeyboardSensor::CAnmKeyboardSensor(Boolean enabled)
: CAnmSensor(enabled)
{
	m_active = false;
}

CAnmKeyboardSensor::~CAnmKeyboardSensor()
{
}

void CAnmKeyboardSensor::Realize()
{
	// Just get the basic sensor Realize stuff
	CAnmSensor::Realize();
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_INHERITED_CALLBACK(CAnmKeyboardSensor, isActive, CAnmSensor);

// member table
BEGIN_DEFINE_MEMBERS(CAnmKeyboardSensor)
CALLBACKID(CAnmKeyboardSensor, isActive),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmKeyboardSensor, CAnmSensor);
