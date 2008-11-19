/********************************************************************************
 * Flux
 *
 * File: AnmTooltip.cpp
 * Description: Rollover tooltip implementation file
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

// AnmTooltip.cpp : implementation file
//

#include "stdafx.h"
#include "fluxcore.h"
#include "AnmTooltip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAnmTooltip


static CString sClassName;


CAnmTooltip::CAnmTooltip()
{
	m_szMargins = CSize(2,2);
	// create font -- use system tooltip font
	CNonClientMetrics ncm;
	m_font.CreateFontIndirect(&ncm.lfStatusFont);
}

CAnmTooltip::~CAnmTooltip()
{
// N.B.: Had to do this otherwise we'd crap out on a subsequent instance
	sClassName = "";

}


IMPLEMENT_DYNAMIC(CAnmTooltip,CWnd)
BEGIN_MESSAGE_MAP(CAnmTooltip, CWnd)
	//{{AFX_MSG_MAP(CAnmTooltip)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_WM_TIMER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAnmTooltip message handlers

//////////////////
// Create window. pt is upper left or upper right corner depending on nStyle.
//
CAnmTooltip::Create(CPoint pt, CWnd* pParentWnd, UINT nStyle, UINT nID)
{
	m_nStyle = nStyle;
	return CreateEx(0,
		NULL,
		NULL,
		WS_POPUP|WS_VISIBLE,
		CRect(pt,CSize(0,0)),
		pParentWnd,
		nID);
}

//////////////////
// text changed: resize window to fit
//
LRESULT CAnmTooltip::OnSetText(WPARAM wp, LPARAM lp)
{
	CClientDC dc = this;
	CFont* pOldFont = dc.SelectObject(&m_font);
	CRect rc;
	GetWindowRect(&rc);
	int x = (m_nStyle & ANMTOOLTIP_JUSTIFYRIGHT) ? rc.right : rc.left;
	int y = rc.top;
	dc.DrawText(CString((LPCTSTR)lp), &rc, DT_CALCRECT);
	rc.InflateRect(m_szMargins);
	if (m_nStyle & ANMTOOLTIP_JUSTIFYRIGHT)
		x -= rc.Width();
	SetWindowPos(NULL,x,y,rc.Width(),rc.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
	return Default();
}

//////////////////
// Paint text using system colors
//
void CAnmTooltip::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	CString s;
	GetWindowText(s);
	CBrush b(GetSysColor(COLOR_INFOBK)); // use tooltip bg color
	dc.FillRect(&rc, &b);

	// draw text
	dc.SetBkMode(TRANSPARENT);
	CFont* pOldFont = dc.SelectObject(&m_font);
	dc.SetTextColor(GetSysColor(COLOR_INFOTEXT)); // tooltip text color
	dc.DrawText(s, &rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	dc.SelectObject(pOldFont);
}


//////////////////
// Register class if needed
//
BOOL CAnmTooltip::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (sClassName.IsEmpty())
		sClassName = AfxRegisterWndClass(0);
	cs.lpszClass = sClassName;
	cs.style = WS_POPUP|WS_BORDER;
	cs.dwExStyle |= WS_EX_TOOLWINDOW;
	return CWnd::PreCreateWindow(cs);
}

//////////////////
// CAnmTooltip is intended to be used on the stack,
// not heap, so don't auto-delete.
//
void CAnmTooltip::PostNcDestroy()
{
}

//////////////////
// Show window with delay. No delay means show now.
//
void CAnmTooltip::ShowDelayed(UINT msec)
{
	if (msec==0) {
		// no delay: show it now
		OnTimer(1);
	} else {
		// delay: set time
		SetTimer(1, msec, NULL);
	}
}

//////////////////
// Cancel text: kill timer and hide window
//
void CAnmTooltip::Cancel()
{
	KillTimer(1);
	ShowWindow(SW_HIDE);
}

//////////////////
// Timer popped: display myself and kill timer
//
void CAnmTooltip::OnTimer(UINT nIDEvent)
{
	ShowWindow(SW_SHOWNA);
	Invalidate();
	UpdateWindow();
	KillTimer(1);
}
