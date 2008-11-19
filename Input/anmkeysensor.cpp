/********************************************************************************
 * Flux
 *
 * File: anmkeysensor.cpp
 * Description: KeySensor class
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
#include "anmkeysensor.h"
#include "anmworld.h"
#include "inputlayer.h"

CAnmKeySensor::CAnmKeySensor()
: CAnmKeyboardSensor(ANMKEYSENSOR_DEFAULT_ENABLED)
{
	m_keyboard = NULL;
	m_newKeyPressed = m_newKeyReleased = ANMKEY_NOKEY;
	m_shiftKeyPressed = false;
}

CAnmKeySensor::~CAnmKeySensor()
{
	if (m_keyboard)
		m_keyboard->RemoveReceiver(this);
}

void CAnmKeySensor::Realize()
{
	// Chain to superclass for basic Realize stuff
	CAnmKeyboardSensor::Realize();

	// Now add me to the keyboard's receiver list
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	m_keyboard = pApp->GetInput()->GetKeyboard();
	m_keyboard->AddReceiver(this);
}

void CAnmKeySensor::Update()
{
	ClearStateDirty();
}

void CAnmKeySensor::Poll()
{
	// shouldn't be here otherwise
	assert(m_enabled);

	// Make sure all the data is up to date
	if (StateDirty())
		Update();

	if (m_newKeyPressed != ANMKEY_NOKEY)
	{
		HandleKeyPress(m_newKeyPressed);
		m_newKeyPressed = ANMKEY_NOKEY;
	}

	if (m_newKeyReleased != ANMKEY_NOKEY)
	{
		HandleKeyRelease(m_newKeyReleased);
		m_newKeyReleased = ANMKEY_NOKEY;
	}
}

void CAnmKeySensor::KeyDown( int key )
{
	m_newKeyPressed = key;
}

void CAnmKeySensor::KeyUp( int key )
{
	m_newKeyReleased = key;
}

void CAnmKeySensor::HandleKeyPress(int key)
{
	int actionkey = ANMKEY_NOKEY;

	char buf[2];
	String s;

	switch (key)
	{
		case (ANMKEY_HOME) :
			actionkey = ANMKEY_X3DHOME;
			break;

		case (ANMKEY_END) :
			actionkey = ANMKEY_X3DEND;
			break;

		case (ANMKEY_PGUP) :
			actionkey = ANMKEY_X3DPGUP;
			break;

		case (ANMKEY_PGDN) :
			actionkey = ANMKEY_X3DPGDN;
			break;

		case (ANMKEY_UP) :
			actionkey = ANMKEY_X3DUP;
			break;

		case (ANMKEY_DOWN) :
			actionkey = ANMKEY_X3DDOWN;
			break;

		case (ANMKEY_LEFT) :
			actionkey = ANMKEY_X3DLEFT;
			break;

		case (ANMKEY_RIGHT) :
			actionkey = ANMKEY_X3DRIGHT;
			break;

		case (ANMKEY_F1) :
			actionkey = ANMKEY_X3DF1;
			break;

		case (ANMKEY_F2) :
			actionkey = ANMKEY_X3DF2;
			break;

		case (ANMKEY_F3) :
			actionkey = ANMKEY_X3DF3;
			break;

		case (ANMKEY_F4) :
			actionkey = ANMKEY_X3DF4;
			break;

		case (ANMKEY_F5) :
			actionkey = ANMKEY_X3DF5;
			break;

		case (ANMKEY_F6) :
			actionkey = ANMKEY_X3DF6;
			break;

		case (ANMKEY_F7) :
			actionkey = ANMKEY_X3DF7;
			break;

		case (ANMKEY_F8) :
			actionkey = ANMKEY_X3DF8;
			break;

		case (ANMKEY_F9) :
			actionkey = ANMKEY_X3DF9;
			break;

		case (ANMKEY_F10) :
			actionkey = ANMKEY_X3DF10;
			break;

		case (ANMKEY_F11) :
			actionkey = ANMKEY_X3DF11;
			break;

		case (ANMKEY_F12) :
			actionkey = ANMKEY_X3DF12;
			break;

		case ANMKEY_LSHIFT :
		case ANMKEY_RSHIFT :
			HandleShiftKey(true);
			break;

		case ANMKEY_LCONTROL :
		case ANMKEY_RCONTROL :
			HandleControlKey(true);
			break;

		case ANMKEY_LALT :
		case ANMKEY_RALT :
			HandleAltKey(true);
			break;

		case ANMKEY_DELETE :
			actionkey = ANMKEY_X3DDELETE;
			break;

		case ANMKEY_BACK :
			actionkey = ANMKEY_X3DBACK;
			break;

		case ANMKEY_ESCAPE :
			actionkey = ANMKEY_X3DESCAPE;
			break;

		default :
			{
				// normal keys (and hey, 'action too', are now in xlation table,
				// and already xlated.
				buf[0] = (char)key;
/* Original code
				CAnmWorld *pWorld = GetWorld();
				assert(pWorld);

				cApplication *pApp = pWorld->GetApp();
				assert(pApp);

				cInputLayer *pInput = pApp->GetInput();
				assert(pInput);

				cKeyboard *pKeyboard = pInput->GetKeyboard();
				assert(pKeyboard);

				buf[0] = pKeyboard->TranslateVirtualKey(key);
original code */
				if (buf[0])
				{
					buf[1] = '\0';
					s = new CAnmString(buf);
					CallCallbacks(CALLBACKID(CAnmKeySensor, keyPress), &s);
					SafeUnRef(s);
				}
			}
			break;
	}

	if (actionkey != ANMKEY_NOKEY)
		HandleActionKeyPress(actionkey);

	m_active = true;
	CallCallbacks(CALLBACKID(CAnmKeySensor, isActive), &m_active);

}

void CAnmKeySensor::HandleKeyRelease(int key)
{
	int actionkey = ANMKEY_NOKEY;

	char buf[2];
	String s;

	switch (key)
	{
		case (ANMKEY_HOME) :
			actionkey = ANMKEY_X3DHOME;
			break;

		case (ANMKEY_END) :
			actionkey = ANMKEY_X3DEND;
			break;

		case (ANMKEY_PGUP) :
			actionkey = ANMKEY_X3DPGUP;
			break;

		case (ANMKEY_PGDN) :
			actionkey = ANMKEY_X3DPGDN;
			break;

		case (ANMKEY_UP) :
			actionkey = ANMKEY_X3DUP;
			break;

		case (ANMKEY_DOWN) :
			actionkey = ANMKEY_X3DDOWN;
			break;

		case (ANMKEY_LEFT) :
			actionkey = ANMKEY_X3DLEFT;
			break;

		case (ANMKEY_RIGHT) :
			actionkey = ANMKEY_X3DRIGHT;
			break;

		case (ANMKEY_F1) :
			actionkey = ANMKEY_X3DF1;
			break;

		case (ANMKEY_F2) :
			actionkey = ANMKEY_X3DF2;
			break;

		case (ANMKEY_F3) :
			actionkey = ANMKEY_X3DF3;
			break;

		case (ANMKEY_F4) :
			actionkey = ANMKEY_X3DF4;
			break;

		case (ANMKEY_F5) :
			actionkey = ANMKEY_X3DF5;
			break;

		case (ANMKEY_F6) :
			actionkey = ANMKEY_X3DF6;
			break;

		case (ANMKEY_F7) :
			actionkey = ANMKEY_X3DF7;
			break;

		case (ANMKEY_F8) :
			actionkey = ANMKEY_X3DF8;
			break;

		case (ANMKEY_F9) :
			actionkey = ANMKEY_X3DF9;
			break;

		case (ANMKEY_F10) :
			actionkey = ANMKEY_X3DF10;
			break;

		case (ANMKEY_F11) :
			actionkey = ANMKEY_X3DF11;
			break;

		case (ANMKEY_F12) :
			actionkey = ANMKEY_X3DF12;
			break;

		case ANMKEY_LSHIFT :
		case ANMKEY_RSHIFT :
			HandleShiftKey(false);
			break;

		case ANMKEY_LCONTROL :
		case ANMKEY_RCONTROL :
			HandleControlKey(false);
			break;

		case ANMKEY_LALT :
		case ANMKEY_RALT :
			HandleAltKey(false);
			break;

		default :
			{
				// normal keys (and hey, 'action too', are now in xlation table,
				// and already xlated.
				buf[0] = (char)key;
/* Original code
				CAnmWorld *pWorld = GetWorld();
				assert(pWorld);

				cApplication *pApp = pWorld->GetApp();
				assert(pApp);

				cInputLayer *pInput = pApp->GetInput();
				assert(pInput);

				cKeyboard *pKeyboard = pInput->GetKeyboard();
				assert(pKeyboard);

				buf[0] = pKeyboard->TranslateVirtualKey(key);
original code */
				if (buf[0])
				{
					buf[1] = '\0';
					s = new CAnmString(buf);
					CallCallbacks(CALLBACKID(CAnmKeySensor, keyRelease), &s);
					SafeUnRef(s);
				}
			}
			break;
	}

	if (actionkey != ANMKEY_NOKEY)
		HandleActionKeyRelease(actionkey);

	m_active = false;
	CallCallbacks(CALLBACKID(CAnmKeySensor, isActive), &m_active);

}

void CAnmKeySensor::HandleActionKeyPress(int key)
{
	CallCallbacks(CALLBACKID(CAnmKeySensor, actionKeyPress), &key);
}

void CAnmKeySensor::HandleActionKeyRelease(int key)
{
	CallCallbacks(CALLBACKID(CAnmKeySensor, actionKeyRelease), &key);
}

void CAnmKeySensor::HandleShiftKey(bool pressed)
{
	m_shiftKeyPressed = pressed;

	CallCallbacks(CALLBACKID(CAnmKeySensor, shiftKey), &pressed);
}

void CAnmKeySensor::HandleControlKey(bool pressed)
{
	CallCallbacks(CALLBACKID(CAnmKeySensor, controlKey), &pressed);
}

void CAnmKeySensor::HandleAltKey(bool pressed)
{
	CallCallbacks(CALLBACKID(CAnmKeySensor, altKey), &pressed);
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_INHERITED_VALUE(CAnmKeySensor, enabled, CAnmKeyboardSensor);

DEFINE_INHERITED_METHOD(CAnmKeySensor, set_enabled, CAnmKeyboardSensor);

DEFINE_INHERITED_CALLBACK(CAnmKeySensor, enabled_changed, CAnmKeyboardSensor);
DEFINE_CALLBACK(CAnmKeySensor, actionKeyPress, eValueInteger);
DEFINE_CALLBACK(CAnmKeySensor, actionKeyRelease, eValueInteger);
DEFINE_CALLBACK(CAnmKeySensor, altKey, eValueBoolean);
DEFINE_CALLBACK(CAnmKeySensor, controlKey, eValueBoolean);
DEFINE_INHERITED_CALLBACK(CAnmKeySensor, isActive, CAnmKeyboardSensor);
DEFINE_CALLBACK(CAnmKeySensor, keyPress, eValueString);
DEFINE_CALLBACK(CAnmKeySensor, keyRelease, eValueString);
DEFINE_CALLBACK(CAnmKeySensor, shiftKey, eValueBoolean);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmKeySensor)
VALUEID(CAnmKeySensor, enabled),

METHODID(CAnmKeySensor, set_enabled),

CALLBACKID(CAnmKeySensor, enabled_changed),
CALLBACKID(CAnmKeySensor, actionKeyPress),
CALLBACKID(CAnmKeySensor, actionKeyRelease),
CALLBACKID(CAnmKeySensor, altKey),
CALLBACKID(CAnmKeySensor, controlKey),
CALLBACKID(CAnmKeySensor, isActive),
CALLBACKID(CAnmKeySensor, keyPress),
CALLBACKID(CAnmKeySensor, keyRelease),
CALLBACKID(CAnmKeySensor, shiftKey),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmKeySensor, CAnmKeyboardSensor);
