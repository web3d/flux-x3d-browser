/********************************************************************************
 * Flux
 *
 * File: InputLayer.cpp
 * Description: manages keyboard/mouse input
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
#include "InputLayer.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "..\windowing\Application.h"
#include "..\windowing\Window.h"
#include "anmplatform.h"

#if 1
cInputLayer *cInputLayer::Create(class cApplication *pApp, 
	HINSTANCE hInst, 
	HWND hWnd, 
	bool bExclusive, 
	bool bUseKeyboard, 
	bool bUseMouse)
{
	assert(pApp);

	CAnmPlatform *pPlatform = pApp->GetPlatform();
	assert(pPlatform);

	return pPlatform->CreateInput(pApp,
		hInst, 
		hWnd, 
		bExclusive, 
		bUseKeyboard, 
		bUseMouse );
}

#else
#if defined(DX8) || defined(DX9)

#include "anmdinput8.h"

cInputLayer *cInputLayer::Create(class cApplication *pApp, 
	HINSTANCE hInst, 
	HWND hWnd, 
	bool bExclusive, 
	bool bUseKeyboard, 
	bool bUseMouse)
{
	// everything is taken care of in the constructor
	return new CAnmDInput8(
		pApp,
		hInst, 
		hWnd, 
		bExclusive, 
		bUseKeyboard, 
		bUseMouse );
}

#else

#include "anmdinput7.h"

cInputLayer *cInputLayer::Create(class cApplication *pApp, 
	HINSTANCE hInst, 
	HWND hWnd, 
	bool bExclusive, 
	bool bUseKeyboard, 
	bool bUseMouse)
{
	// everything is taken care of in the constructor
	return new CAnmDInput7(
		pApp,
		hInst, 
		hWnd, 
		bExclusive, 
		bUseKeyboard, 
		bUseMouse );
}

#endif // DX8 || DX9
#endif // 0

cInputLayer::cInputLayer(cApplication *pApp, 
	HINSTANCE hInst, 
	HWND hWnd, 
	bool bExclusive, 
	bool bUseKeyboard, 
	bool bUseMouse )
{
	m_pKeyboard = NULL;
	m_pMouse = NULL;

	// set this up early because other people will access it
	pApp->SetInput(this);
}

void cInputLayer::UpdateDevices()
{
	if( m_pKeyboard )
	{
		m_pKeyboard->Update();
	}
	if( m_pMouse )
	{
		m_pMouse->Update();
	}
}


void cInputLayer::SetFocus()
{
	if( m_pKeyboard )
	{
		m_pKeyboard->ClearTable();
	}
	if( m_pMouse )
	{
		m_pMouse->Acquire();
	}
}


void cInputLayer::KillFocus()
{
	if( m_pKeyboard )
	{
		m_pKeyboard->ClearTable();
	}
	if( m_pMouse )
	{
		m_pMouse->UnAcquire();
	}
}
