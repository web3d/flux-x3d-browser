/********************************************************************************
 * Flux
 *
 * File: anmdimouse8.cpp
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

#include "stdafx.h"
#include "anmdimouse8.h"
#include "anmdinput8.h"
#include "anmdx8message.h"

CAnmDIMouse8::CAnmDIMouse8(  cApplication *pApp, HWND hWnd, bool bExclusive )
: cMouse(pApp, hWnd, bExclusive)
{
	HRESULT hr;

	/**
	 * Create the device
	 */
	CAnmDInput8 *pDInput8 = (CAnmDInput8 *) m_app->GetInput();
	LPDIRECTINPUT8 pDI = pDInput8->GetDInput();

	hr = pDI->CreateDevice(
		GUID_SysMouse, 		
        &m_pDevice, 
		NULL);
	if( FAILED( hr ))
	{
		// ANMMSG_COULDNTCREATEMOUSE : "[cMouse::Init]: Could not create the device"
		throw CAnmError(ANMMSG_COULDNTCREATEMOUSE);
	}

	/**
	 * Set the data format
	 */
	hr = m_pDevice->SetDataFormat(&c_dfDIMouse);
 	if( FAILED( hr ))
	{
		SafeRelease( m_pDevice );
		// ANMMSG_SETDATAFORMATMOUSEFAILED : "[cMouse::Init]: SetDataFormat failed"
		throw CAnmError(ANMMSG_SETDATAFORMATMOUSEFAILED);
	}

	/**
	 * Set the cooperative level
	 */
	if( bExclusive )
	{
		hr = m_pDevice->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND );
	}
	else
	{
		hr = m_pDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	}

	if( FAILED( hr ))
	{
		SafeRelease( m_pDevice );
		// ANMMSG_SETCOOPLEVELMOUSEFAILED : "[cMouse::Init]: SetCooperativeLevel failed"
		throw CAnmError(ANMMSG_SETCOOPLEVELMOUSEFAILED);
	}


	// CHANGE: we used to grab the mouse here, but this would cause
	// a crash if the app didn't have focus when it started up.  This is,
	// of course, an undesirable trait :)
	
	// Instead we just set the mouse initially to be stationary, with no
	// buttons pressed.  We can't do a GetDeviceState here since we don't
	// have the device acquired.

	m_lastState.lX = 0;
	m_lastState.lY = 0;
	m_lastState.lZ = 0;
	m_lastState.rgbButtons[0] = 0;
	m_lastState.rgbButtons[1] = 0;
	m_lastState.rgbButtons[2] = 0;
	m_lastState.rgbButtons[3] = 0;
}

CAnmDIMouse8::~CAnmDIMouse8()
{
	if( m_pDevice )
	{
		m_pDevice->Unacquire();
		SafeRelease( m_pDevice );
	}
}


eResult CAnmDIMouse8::Update()
{
	DIMOUSESTATE currState;
    HRESULT  hr; 
 
	// CHANGE: Polling can't hurt, but it can help.
    hr = m_pDevice->Poll(); 
	hr = m_pDevice->GetDeviceState( sizeof(DIMOUSESTATE), (void*)&currState );
	//	DP2("Mouse: %d %d\n", (int) currState.lX, (int) currState.lY);

    if( FAILED(hr) )
    { 
		hr = m_pDevice->Acquire();
		if( FAILED( hr ) )
		{
			return resFailed;
		}

	    hr = m_pDevice->Poll(); 
	    hr = m_pDevice->GetDeviceState( sizeof(DIMOUSESTATE),(void*)&currState );
		// DP2("Mouse: %d %d\n", (int) currState.lX, (int) currState.lY);
		if( FAILED( hr ) )
		{
			return resFailed;
		}
    } 
	if( m_pTarget )
	{
		int dx = currState.lX;
		int dy = currState.lY;
		if( dx || dy )
		{
			m_pTarget->MouseMoved( dx, dy );
		}
		if( currState.rgbButtons[0] & 0x80 )
		{
			// the button got pressed.
			m_pTarget->MouseButtonDown( 0 );
		}
		if( currState.rgbButtons[1] & 0x80 )
		{
			// the button got pressed.
			m_pTarget->MouseButtonDown( 1 );
		}
		if( currState.rgbButtons[2] & 0x80 )
		{
			// the button got pressed.
			m_pTarget->MouseButtonDown( 2 );
		}
		if( !(currState.rgbButtons[0] & 0x80) && (m_lastState.rgbButtons[0] & 0x80) )
		{
			// the button got released.
			m_pTarget->MouseButtonUp( 0 );
		}
		if( !(currState.rgbButtons[1] & 0x80) && (m_lastState.rgbButtons[1] & 0x80) )
		{
			// the button got released.
			m_pTarget->MouseButtonUp( 1 );
		}
		if( !(currState.rgbButtons[2] & 0x80) && (m_lastState.rgbButtons[2] & 0x80) )
		{
			// the button got released.
			m_pTarget->MouseButtonUp( 2 );
		}
	}
	m_lastState = currState;
	return resAllGood;
}


eResult CAnmDIMouse8::Acquire()
{
	HRESULT hr = m_pDevice->Acquire();
	if( FAILED(hr) )
	{
		return resFailed;
	}
	return resAllGood;
}


void CAnmDIMouse8::UnAcquire()
{
	m_pDevice->Unacquire();
}
