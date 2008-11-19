/********************************************************************************
 * Flux
 *
 * File: anmstringsensor.cpp
 * Description: StringSensor class
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
#include "anmstringsensor.h"
#include "anmworld.h"

CAnmStringSensor::CAnmStringSensor()
: CAnmKeyboardSensor(ANMSTRINGSENSOR_DEFAULT_ENABLED),
	m_deletionAllowed(ANMSTRINGSENSOR_DEFAULT_DELETIONALLOWED)
{
	m_anmapp = NULL;
	m_stringBuffer = NULL;
	m_curstringbuffersize = 0;
	m_curcharindex = 0;
	m_newdata = false;
	m_terminatorReceived = false;
}

CAnmStringSensor::~CAnmStringSensor()
{
	if (m_anmapp)
	{
		m_anmapp->RemoveCharacterReceiver(this);
	}

	SafeDelete(m_stringBuffer);
}

void CAnmStringSensor::Realize()
{
	// Chain to superclass for basic Realize stuff
	CAnmKeyboardSensor::Realize();

	// Now add me to the keyboard's receiver list
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	m_anmapp = (cAnimaApp *) pApp;

	m_anmapp->AddCharacterReceiver(this);
}

void CAnmStringSensor::Update()
{
	ClearStateDirty();
}

void CAnmStringSensor::Poll()
{
	// shouldn't be here otherwise
	assert(m_enabled);

	Lock();
	if (m_newdata)
	{
		String s = new CAnmString(m_stringBuffer);
		CallCallbacks(CALLBACKID(CAnmStringSensor, enteredText), &s);
		m_newdata = false;

		if (m_terminatorReceived)
		{
			CallCallbacks(CALLBACKID(CAnmStringSensor, finalText), &s);
			m_terminatorReceived = false;

			ClearBuffer();
		}

		SafeUnRef(s);
	}

	Unlock();
}

void CAnmStringSensor::NewCharacter( int nchar )
{
	Lock();

	EnsureBufferSize();

	if (nchar == ANMSTRINGSENSOR_TERMINATORCHAR)
	{
		m_terminatorReceived = true;
	}
	else if (nchar == ANMSTRINGSENSOR_DELETECHAR)
	{
		m_stringBuffer[--m_curcharindex]='\0';
	}
	else
	{
		m_stringBuffer[m_curcharindex++] = nchar;
		m_stringBuffer[m_curcharindex] = '\0';
	}
	
	m_newdata = true;

	Unlock();
}

void CAnmStringSensor::ExpandBuffer()
{
	m_curstringbuffersize += ANMSTRINGSENSOR_ALLOCSIZE;
	if (m_stringBuffer)
		m_stringBuffer = (char *) realloc(m_stringBuffer, m_curstringbuffersize);
	else
		m_stringBuffer = new char[ANMSTRINGSENSOR_ALLOCSIZE];
}

void CAnmStringSensor::ClearBuffer()
{
	assert(m_stringBuffer);
	
	m_stringBuffer[0] = '\0';
	m_curcharindex = 0;
}

// Accessors
void CAnmStringSensor::SetDeletionAllowed(Boolean deletionAllowed)
{
	if (m_deletionAllowed == deletionAllowed)
		return;

	m_deletionAllowed = deletionAllowed;
	
	CallCallbacks(CALLBACKID(CAnmStringSensor, deletionAllowed_changed), &deletionAllowed);
}


// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmStringSensor, deletionAllowed, eValueBoolean, Boolean, GetDeletionAllowed, SetDeletionAllowed);
DEFINE_INHERITED_VALUE(CAnmStringSensor, enabled, CAnmKeyboardSensor);

DEFINE_METHOD(CAnmStringSensor, set_deletionAllowed, eValueBoolean, Boolean, SetDeletionAllowed);
DEFINE_INHERITED_METHOD(CAnmStringSensor, set_enabled, CAnmKeyboardSensor);

DEFINE_CALLBACK(CAnmStringSensor, deletionAllowed_changed, eValueBoolean);
DEFINE_INHERITED_CALLBACK(CAnmStringSensor, enabled_changed, CAnmKeyboardSensor);
DEFINE_CALLBACK(CAnmStringSensor, enteredText, eValueString);
DEFINE_CALLBACK(CAnmStringSensor, finalText, eValueString);
DEFINE_CALLBACK(CAnmStringSensor, isActive, eValueBoolean);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmStringSensor)
VALUEID(CAnmStringSensor, deletionAllowed),
VALUEID(CAnmStringSensor, enabled),

METHODID(CAnmStringSensor, set_deletionAllowed),
METHODID(CAnmStringSensor, set_enabled),

CALLBACKID(CAnmStringSensor, deletionAllowed_changed),
CALLBACKID(CAnmStringSensor, enabled_changed),
CALLBACKID(CAnmStringSensor, enteredText),
CALLBACKID(CAnmStringSensor, finalText),
CALLBACKID(CAnmStringSensor, isActive),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmStringSensor, CAnmKeyboardSensor);

