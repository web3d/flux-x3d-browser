/********************************************************************************
 * Flux
 *
 * File: anmdinput8.h
 * Description: Input class for DirectX Version 8
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

#ifndef _anmdinput8_h
#define _anmdinput8_h

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

#include "InputLayer.h"

class CAnmDInput8 : public cInputLayer
{
	// The DI7 object
	LPDIRECTINPUT8	m_pDI;

public:

	CAnmDInput8( class cApplication *pApp,
		HINSTANCE hInst, 
		HWND hWnd, 
		bool bExclusive, 
		bool bUseKeyboard = true, 
		bool bUseMouse = true );

	virtual ~CAnmDInput8();

	LPDIRECTINPUT8 GetDInput()
	{
		return m_pDI; 
	}
};


#endif //_anmdinput8_h
