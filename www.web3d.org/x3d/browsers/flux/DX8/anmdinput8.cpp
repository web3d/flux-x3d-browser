/********************************************************************************
 * Flux
 *
 * File: anmdinput8.cpp
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

#include "stdafx.h"
#include "anmdinput8.h"
#include "anmdikeyboard8.h"
#include "anmdimouse8.h"
#include "anmdx8message.h"

CAnmDInput8::CAnmDInput8(cApplication *pApp, 
	HINSTANCE hInst, 
	HWND hWnd, 
	bool bExclusive, 
	bool bUseKeyboard, 
	bool bUseMouse )
	: cInputLayer(pApp, hInst, hWnd, bExclusive, bUseKeyboard, bUseMouse)
{
	HRESULT hr;

	/**
	 * Create the DI7 object
	 */
	hr = DirectInput8Create(
		hInst, 
		DIRECTINPUT_VERSION, 
		IID_IDirectInput8,
		(void**)&m_pDI,
		NULL); 

    if( FAILED(hr) )
	{
		// ANMMSG_COULDNTCREATEDI8 : "DirectInput8 object could not be created"
		throw CAnmError(ANMMSG_COULDNTCREATEDI8); 
	}

	try 
	{
		if( bUseKeyboard )
		{
			m_pKeyboard = new CAnmDIKeyboard8(pApp, hWnd );
		}
		if( bUseMouse )
		{
			m_pMouse = new CAnmDIMouse8(pApp, hWnd, bExclusive );
		}
	}
	catch( ... )
	{
		SafeRelease( m_pDI );
		throw;
	}

}


CAnmDInput8::~CAnmDInput8()
{
	if( m_pDI )
	{
		if( m_pKeyboard )
		{
			delete m_pKeyboard; // this does all the de-init.
		}

		if( m_pMouse )
		{
			delete m_pMouse; // this does all the de-init.
		}
		ExtraSafeRelease( m_pDI );
	}

}
