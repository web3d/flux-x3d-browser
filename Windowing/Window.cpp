/********************************************************************************
 * Flux
 *
 * File: window.cpp
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

#include "stdafx.h"
#include "Window.h"
#include "Application.h"

using namespace std;


/**
 * We can't unwind our stack across the dll boundary
 * into kernel32.dll or any other system dll's.  
 * because of this we must catch any errors that
 * occur in wndproc and then re-throw them inside
 * the message pump.
 */
CAnmError* g_pThrownError = NULL;



cWindow::cWindow( cApplication *pApp, int width, int height, const char* name ) :
	m_app( pApp ),
	m_width( width ),
	m_height( height ),
	m_hWnd( NULL )
{
	m_bActive = FALSE;
	m_app->SetMainWindow(this);

	// copy the name over
	m_name = string( name );
}

cWindow::cWindow(cApplication *pApp,  CWnd *pWnd)
{
	m_app = pApp;
	m_app->SetMainWindow(this);

	m_pWnd = pWnd;
	m_hWnd = *pWnd;
	m_Screenx = m_Screeny = -1;
	m_bActive = FALSE;
}

cWindow::~cWindow()
{
	// DestroyWindow( m_hWnd );
}


BOOL cWindow::RectMoved()
{
	BOOL rectmoved = FALSE;

	POINT p;
	p.x = p.y = 0;

	ClientToScreen(m_hWnd, &p);
	if (p.x != m_Screenx || p.y != m_Screeny)
	{
		m_Screenx = p.x;
		m_Screeny = p.y;

		rectmoved = TRUE;
	}

	return rectmoved;
}

int cWindow::GetWidth()
{
	RECT r;
	m_pWnd->GetClientRect(&r);
	return r.right - r.left;
}

int cWindow::GetHeight()
{
	RECT r;
	m_pWnd->GetClientRect(&r);
	return r.bottom - r.top;
}