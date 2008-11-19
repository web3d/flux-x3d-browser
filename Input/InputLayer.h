/********************************************************************************
 * Flux
 *
 * File: InputLayer.h
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

#ifndef _INPUTLAYER_H
#define _INPUTLAYER_H

#include "Keyboard.h"
#include "Mouse.h"

class EXPORT cInputLayer
{
protected :
	
	cKeyboard*		m_pKeyboard;
	cMouse*			m_pMouse;

	cInputLayer( class cApplication *pApp,
		HINSTANCE hInst, 
		HWND hWnd, 
		bool bExclusive, 
		bool bUseKeyboard = true, 
		bool bUseMouse = true );

public:

	virtual ~cInputLayer()
	{
	}

	cKeyboard* GetKeyboard()
	{
			return m_pKeyboard;
	}

	cMouse* GetMouse()
	{
		return m_pMouse;
	}

	void UpdateDevices();

	void SetFocus(); // called when the app gains focus
	void KillFocus(); // called when the app must release focus

	static cInputLayer *Create(class cApplication *pApp, 
		HINSTANCE hInst, 
		HWND hWnd, 
		bool bExclusive, 
		bool bUseKeyboard = true, 
		bool bUseMouse = true );
};


#endif //_INPUTLAYER_H
