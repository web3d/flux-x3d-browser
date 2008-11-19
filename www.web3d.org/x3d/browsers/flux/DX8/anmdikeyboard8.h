/********************************************************************************
 * Flux
 *
 * File: anmdikeyboard9.h
 * Description: Keyboard class for DirectX Version 9
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

#ifndef _anmdikeyboard8_h
#define _anmdikeyboard8_h

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

#include "keyboard.h"

class CAnmDIKeyboard8: public cKeyboard
{
	LPDIRECTINPUTDEVICE8	m_pDevice; // The Direct Input Device that represents the keyboard

	char					m_keyState[256];

	int AnmKey(int virtKey);

public:

	CAnmDIKeyboard8( class cApplication *pApp, HWND hWnd );
	~CAnmDIKeyboard8();

	// cKeyboard overrides
	virtual void ClearTable();
	virtual bool Poll( int key );
	virtual eResult Update();
	virtual char TranslateVirtualKey(int key);
};


#endif //_anmdikeyboard8_h
