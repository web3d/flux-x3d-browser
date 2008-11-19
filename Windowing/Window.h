/********************************************************************************
 * Flux
 *
 * File: window.h
 * Description: Window abstraction
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

#ifndef _WINDOW_H
#define _WINDOW_H

#include <windows.h>

#include <list>
#include <string>


/**
 * cWindow 
 * Window encapsulator
 */
class EXPORT cWindow  
{
protected:

	// Width and height of the client area
	int m_Screenx, m_Screeny, m_width, m_height;

	// Handle to the window
	HWND m_hWnd;
	CWnd *m_pWnd;

	// Instance handle;

	class cApplication *m_app;

	// Name of the window 
	std::string m_name;

	// True if the window is active	
	BOOL m_bActive;

public:
	cWindow( class cApplication *pApp,
		int width, 
		int height, 
		const char* name = "Default window name" );

	cWindow(class cApplication *pApp, CWnd *pWnd);

	~cWindow();

	HWND GetHWnd()
	{
		return m_hWnd;
	}

	CWnd *GetWnd()
	{
		return m_pWnd;
	}

	class cApplication *GetApplication() { return m_app; }

	BOOL IsActive()
	{ 
		return m_bActive; 
	}

	BOOL RectMoved();

	int GetWidth();
	int GetHeight();
};


#endif //_WINDOW_H
