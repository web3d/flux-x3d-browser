/********************************************************************************
 * Flux
 *
 * File: anmkeysensor.h
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

#ifndef _anmkeysensor_h
#define _anmkeysensor_h

#include "anmkeyboardsensor.h"
#include "keyboard.h"

#define ANMKEYSENSOR_DEFAULT_ENABLED TRUE

#define ANMKEY_X3DF1		1
#define ANMKEY_X3DF2		2
#define ANMKEY_X3DF3		3
#define ANMKEY_X3DF4		4
#define ANMKEY_X3DF5		5
#define ANMKEY_X3DF6		6
#define ANMKEY_X3DF7		7
#define ANMKEY_X3DF8		8
#define ANMKEY_X3DF9		9
#define ANMKEY_X3DF10		10
#define ANMKEY_X3DF11		11
#define ANMKEY_X3DF12		12
#define ANMKEY_X3DHOME		13
#define ANMKEY_X3DEND		14
#define ANMKEY_X3DPGUP		15
#define ANMKEY_X3DPGDN		16
#define ANMKEY_X3DUP		17
#define ANMKEY_X3DDOWN		18
#define ANMKEY_X3DLEFT		19
#define ANMKEY_X3DRIGHT		20
#define ANMKEY_X3DF13		21	// N.B. need to spec this
#define ANMKEY_X3DF14		22	// N.B. need to spec this
#define ANMKEY_X3DF15		23	// N.B. need to spec this and all below this
#define ANMKEY_X3DESCAPE	24
#define ANMKEY_X3DDELETE	25
#define ANMKEY_X3DBACK		26
#define ANMKEY_X3DLSHIFT	27
#define ANMKEY_X3DRSHIFT	28
#define ANMKEY_X3DLCONTROL	29
#define ANMKEY_X3DRCONTROL	30
#define ANMKEY_X3DLALT		31
#define ANMKEY_X3DRALT		32

class CAnmKeySensor : public CAnmKeyboardSensor, public iKeyboardReceiver
{
protected:

	class cKeyboard		*m_keyboard;
	int					 m_newKeyPressed;
	int					 m_newKeyReleased;
	bool				 m_shiftKeyPressed;

	// iKeyboardReceiver overrides
	virtual void KeyDown( int key );
	virtual void KeyUp( int key );

	// Helpers
	virtual void HandleActionKeyPress(int key);
	virtual void HandleActionKeyRelease(int key);
	virtual void HandleShiftKey(bool pressed);
	virtual void HandleControlKey(bool pressed);
	virtual void HandleAltKey(bool pressed);
	virtual void HandleKeyPress(int key);
	virtual void HandleKeyRelease(int key);

	// N.B.: eventually need to deal with key repeat, I think

public:

	// constructor/destructor
	CAnmKeySensor();
	virtual ~CAnmKeySensor();

	// CAnmKeyboardSensor overrides
	virtual void Realize();
	virtual void Update();
	virtual void Poll();

	// New methods
	
	// Accessors
	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmKeySensor);

	DECLARE_VALUE(enabled);

	DECLARE_METHOD(set_enabled);

	DECLARE_CALLBACK(enabled_changed);

	DECLARE_CALLBACK(actionKeyPress);
	DECLARE_CALLBACK(actionKeyRelease);
	DECLARE_CALLBACK(altKey);
	DECLARE_CALLBACK(controlKey);
	DECLARE_CALLBACK(isActive);
	DECLARE_CALLBACK(keyPress);
	DECLARE_CALLBACK(keyRelease);
	DECLARE_CALLBACK(shiftKey);
};

#endif // _anmkeysensor_h

