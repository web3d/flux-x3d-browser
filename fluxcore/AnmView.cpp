/********************************************************************************
 * Flux
 *
 * File: AnmView.cpp
 * Description: implementation file
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

// AnmView.cpp : implementation file
//

#include "stdafx.h"
#include "fluxcore.h"
#include "AnmView.h"

#include "AnimaApp.h"
#include "anmdevice.h"
#include "anmlight.h"
#include "anmoctree.h"
#include "anmtexture.h"
#include "anmtransform.h"
#include "anmviewer.h"
#include "anmworld.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnmView

CAnmView::CAnmView()
{
	m_app = NULL;
}

CAnmView::~CAnmView()
{
}

void CAnmView::SetApplication(cAnimaApp *pApp)
{
	m_app = pApp;
}

BEGIN_MESSAGE_MAP(CAnmView, CWnd)
	//{{AFX_MSG_MAP(CAnmView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAnmView message handlers

void CAnmView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// CWnd::OnLButtonDown(nFlags, point);

	SetFocus();

	m_app->HandleMouseDown(ANM_LBUTTON, point.x, point.y);
}

void CAnmView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// CWnd::OnLButtonUp(nFlags, point);

	m_app->HandleMouseUp(ANM_LBUTTON, point.x, point.y);
}

void CAnmView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// CWnd::OnMButtonDown(nFlags, point);

	m_app->HandleMouseDown(ANM_MBUTTON, point.x, point.y);
}

void CAnmView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// CWnd::OnMButtonUp(nFlags, point);

	m_app->HandleMouseUp(ANM_MBUTTON, point.x, point.y);
}

extern void showUserGuide();

void CAnmView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// CWnd::OnRButtonDown(nFlags, point);

	m_app->HandleMouseDown(ANM_RBUTTON, point.x, point.y);
	return;

}

void CAnmView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// CWnd::OnRButtonUp(nFlags, point);

	m_app->HandleMouseUp(ANM_RBUTTON, point.x, point.y);
}

void CAnmView::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	m_app->KillFocus();
	
}

void CAnmView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnMouseMove(nFlags, point);

	m_app->HandleMouseMove(point);
}

BOOL CAnmView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	float zdelt = (float) zDelta / WHEEL_DELTA;

	bool shiftKey = false;
	if (nFlags & MK_SHIFT)
		shiftKey = true;
	bool ctrlKey = false;
	if (nFlags & MK_CONTROL)
		ctrlKey = true;

	m_app->HandleWheel(zdelt, shiftKey, ctrlKey);

	// make sure the framework doesn't pass these up to the parent window
	return 1L;
}

BOOL CAnmView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor(m_app->GetCursor().hCursor);

	return TRUE;
}

void CAnmView::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	m_app->SetFocus();
	
}

void CAnmView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);


#ifdef DEBUG
	if (nChar == 'h' || nChar == 'H')
	{
		m_app->SetDisplayOn(!m_app->GetDisplayOn());
	}
	else if (nChar == 'a' || nChar == 'A')
	{
		m_app->SetOptions(eAnmOptionsAntialiasing, 
			!m_app->GetOptions(eAnmOptionsAntialiasing));
	}
	else if (nChar == 'b' || nChar == 'B')
	{
		m_app->SetOptions(eAnmOptionsDrawBoundingBoxes, 
			!m_app->GetOptions(eAnmOptionsDrawBoundingBoxes));
	}
	else if (nChar == 'o' || nChar == 'O')
	{
		m_app->SetOptions(eAnmOptionsDrawOctreeBoxes, 
			!m_app->GetOptions(eAnmOptionsDrawOctreeBoxes));
	}
	else if (nChar == 'm' || nChar == 'M')
	{
		m_app->SetOptions(eAnmOptionsMultiTexturing, 
			!m_app->GetOptions(eAnmOptionsMultiTexturing));
	}
	else if (nChar == 'w' || nChar == 'W')
	{
		m_app->SetOptions(eAnmOptionsWireframe, 
			!m_app->GetOptions(eAnmOptionsWireframe));
	}
	else if (nChar == 'f' || nChar == 'F')
	{
		CAnmViewer *pViewer = m_app->GetViewer();
		eNavigationMode navmode = pViewer->GetNavigationMode();
		if (navmode == eNavigationModeFly || navmode == eNavigationModeWalk)
			pViewer->SetNavigationMode(eNavigationModeExamine);
		else if (navmode == eNavigationModeExamine)
			pViewer->SetNavigationMode(eNavigationModeFly);
	}
	else if (nChar == 't' || nChar == 'T')
	{
		m_app->SetOptions(eAnmOptionsDrawTextures, 
			!m_app->GetOptions(eAnmOptionsDrawTextures));
	}
	else if (nChar == 'l' || nChar == 'L')
	{
		m_app->SetOptions(eAnmOptionsShowLights, 
			!m_app->GetOptions(eAnmOptionsShowLights));
	}
	else
	{
		m_app->HandleKey(nChar);
	}
#else
		m_app->HandleKey(nChar);
#endif
}

void CAnmView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if( m_app->GetDevice() )
	{
		CAnmWorld *pWorld = m_app->GetWorld();

		assert(pWorld);

		try
		{

			pWorld->Lock();
			m_app->GetDevice()->HandleSize(cx, cy);
			pWorld->Unlock();
		}
		catch (CAnmError &err)
		{
			pWorld->Unlock();

			::MessageBox( NULL, err.GetText(), "Flux - Fatal Error", MB_OK|MB_ICONEXCLAMATION );
		}
	
	}
	
}

void CAnmView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_app->HandleChar(nChar);
}
