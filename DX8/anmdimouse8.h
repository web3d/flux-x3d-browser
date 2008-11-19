/********************************************************************************
 * Flux
 *
 * File: anmdimouse8.h
 * Description: Mouse class for DirectX Version 8
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

#ifndef _anmdimouse8_h
#define _anmdimouse8_h

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

#include "mouse.h"

class CAnmDIMouse8 : public cMouse  
{
	LPDIRECTINPUTDEVICE8	m_pDevice; 

	DIMOUSESTATE			m_lastState;

public:

	CAnmDIMouse8( class cApplication *pApp, HWND hWnd, bool bExclusive );
	virtual ~CAnmDIMouse8();

	// cMouse overrides
	virtual eResult Update();
	virtual eResult Acquire();
	virtual void UnAcquire();
};

#endif // _anmdimouse8_h
