/********************************************************************************
 * Flux
 *
 * File: AnmDashboard.h
 * Description: 
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

#if !defined(AFX_ANMDASHBOARD_H__4C498A3F_ACE2_4FF4_B3F0_A1941455E920__INCLUDED_)
#define AFX_ANMDASHBOARD_H__4C498A3F_ACE2_4FF4_B3F0_A1941455E920__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnmDashboard.h : header file
//

#include "anmnavigationinfo.h"
#include "anmviewer.h"

/////////////////////////////////////////////////////////////////////////////
// CDashboard1 window

class EXPORT CDashboard1 : public CWnd
{
	DECLARE_DYNCREATE(CDashboard1)

// Construction
public:
	CDashboard1();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDashboard1)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDashboard1();

	void SetApplication(class cAnimaApp *pApp);
	void SetViewMenu(HMENU hViewMenu);
	void SetHelpMenu(HMENU hHelpMenu);
	void AdviseViewpointBound(class CAnmViewpoint *pViewpoint);
	virtual void AdviseNavigationModeChange(int navmode);
	virtual void AdviseCameraChange(struct sAnmCameraChange *pCameraChange);
	virtual void AdviseUndoMoveChange(bool canundo, bool canredo);


	// Generated message map functions
protected:
	//{{AFX_MSG(CDashboard1)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CDashboard1)
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CDashboard1)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	CBitmap							 m_bgbitmap;
	HFONT							 m_font;
	HFONT							 m_bigfont;
	HFONT							 m_smallfont;
	HMENU							 m_hViewMenu;
	HMENU							 m_hHelpMenu;
	int								 m_bgbitwidth;
	int								 m_bgbitheight;
	CDC								 m_memDC;
	class CAnmDashboardControl		*m_overControl;
	class CAnmDashboardControl		*m_pressedControl;
	bool							 m_inDrag;
	class cAnimaApp					*m_app;
	class CAnmTooltip				*m_tooltip;
	eNavigationMode					 m_navmode;

	std::vector<class CAnmDashboardControl *> m_controls;

	void LoadBackground();
	void PaintBackground(CDC *pDC);
	void CreateControls();
	void DestroyControls();
	void AddControl(class CAnmDashboardControl *pControl);
	void PaintControls(CDC *pDC);
	void PollControls(POINT pt);
	void LayoutControls(int cx, int cy);
	void SetOverControl(class CAnmDashboardControl *pControl);
	void PaintTooltip(CDC *pDC);

	void Seek();
	void ContinuousSeek();
	void Straighten();
	void Undo();
	void Redo();
	void NextView();
	void PreviousView();
	void Preferences();
	void Info();

	void SetNavigationMode(eNavigationMode navmode);
	void MakeFonts(HDC hDC);

	void TrackViewMenu(POINT point);
	void TrackHelpMenu(POINT point);

};

#define TOOLTIP_POINTSIZE 10
#define TOOLTIP_LEFT 10
#define TOOLTIP_BOTTOM 4
#define TOOLTIP_HEIGHT 12
#define TOOLTIP_MARGIN 2

#define ANMDASHBOARD_BIGHEIGHT 56
#define ANMDASHBOARD_SMALLHEIGHT 38
#define ANMDASHBOARD_IDEALWIDTH 512

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANMDASHBOARD_H__4C498A3F_ACE2_4FF4_B3F0_A1941455E920__INCLUDED_)
