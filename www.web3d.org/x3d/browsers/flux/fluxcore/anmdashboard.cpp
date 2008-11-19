/********************************************************************************
 * Flux
 *
 * File: AnmDashboard.cpp
 * Description: Navigation dashboard implementation file
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

// AnmDashboard.cpp : implementation file
//

#include "stdafx.h"
#include "fluxcore.h"
#include "AnmDashboard.h"
#include "anmdashboardcontrol.h"
#include "AnmTooltip.h"

#include "AnimaApp.h"
#include "anmcamera.h"
#include "anmcontextmenu.h"
#include "anmnavigationinfo.h"
#include "anmviewer.h"
#include "anmviewpoint.h"
#include "anmworld.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef enum eDashboardControlLayout {
	eDCLFixed,
	eDCLRelativeTo,
	eDCLRelativeToCenter,
	eDCLCenter,
	eDCLLeft,
	eDCLRight,
	eDCLTop,
	eDCLBottom,
} eDashboardControlLayout ;

typedef struct sAnmDashboardControl {
	UINT resID;
	int commandID;
	eDashboardControlLayout layoutX;
	eDashboardControlLayout layoutY;
	int relativeToX;
	int relativeToY;
	int xOffset;
	int yOffset;
	eAnmDashboardControlState initialState;
	char *tooltip;
} sAnmDashboardControl ;

#define RELATIVE_PREVIOUS -1

#define MOVEINDEX 0
#define ROTATEINDEX 1
#define SEEKINDEX_BIG 2
#define STRAIGHTENINDEX 3
#define UNDOINDEX 4
#define REDOINDEX 5
#define VIEWPREVIOUSINDEX 6
#define VIEWNAMESINDEX 7
#define VIEWNEXTINDEX 8
#define PREFERENCESINDEX 9
#define INFOINDEX 10
#define HELPINDEX 11

static sAnmDashboardControl controls[] = {
	
	// large buttons
	{ IDB_EXPLORE, IDC_MOVE, eDCLLeft, eDCLTop, 0, 0, 32, 2, eDCSNormal, "Explore" },
	{ IDB_EXAMINE, IDC_ROTATE, eDCLRelativeTo, eDCLRelativeTo, RELATIVE_PREVIOUS, RELATIVE_PREVIOUS, 2, 0, eDCSNormal, "Examine" },
	{ IDB_SEEK, IDC_SEEK, eDCLRelativeTo, eDCLRelativeTo, RELATIVE_PREVIOUS, RELATIVE_PREVIOUS, 36, 0, eDCSNormal, "Seek" },
	{ IDB_LEVEL, IDC_STRAIGHTEN, eDCLRelativeTo, eDCLRelativeTo, RELATIVE_PREVIOUS, RELATIVE_PREVIOUS, 8, 0, eDCSNormal, "Straighten up"},
	{ IDB_BACK, IDC_UNDOMOVE, eDCLRelativeTo, eDCLRelativeTo, RELATIVE_PREVIOUS, RELATIVE_PREVIOUS, 8, 0, eDCSDimmed, "Go to previous location"},
	{ IDB_FORWARD, IDC_REDOMOVE, eDCLRelativeTo, eDCLRelativeTo, RELATIVE_PREVIOUS, RELATIVE_PREVIOUS, 2, 0, eDCSDimmed, "Go to next location"},
	{ IDB_VIEWPREVIOUS, IDC_VIEWPREVIOUS, eDCLRelativeTo, eDCLRelativeTo, RELATIVE_PREVIOUS, RELATIVE_PREVIOUS, 12, 0, eDCSNormal, "Jump to previous viewpoint" },
	{ IDB_VIEWNAMES, IDC_VIEWNAMES, eDCLRelativeTo, eDCLRelativeTo, RELATIVE_PREVIOUS, RELATIVE_PREVIOUS, 2, 0, eDCSNormal, "Viewpoints"},
	{ IDB_VIEWNEXT, IDC_VIEWNEXT, eDCLRelativeTo, eDCLRelativeTo, RELATIVE_PREVIOUS, RELATIVE_PREVIOUS, 2, 0, eDCSNormal, "Jump to next viewpoint"},
	{ IDB_PREFERENCES, IDC_DASHBOARDPREFS, eDCLRelativeToCenter, eDCLRelativeTo, VIEWNAMESINDEX, VIEWNAMESINDEX, 07, 1, eDCSDimmed, "Preferences"},
	{ IDB_INFO, IDC_DASHBOARDINFO, eDCLRelativeTo, eDCLRelativeTo, RELATIVE_PREVIOUS, RELATIVE_PREVIOUS, -8, 0, eDCSNormal, "Open info center" },
	{ IDB_HELP, IDC_DASHBOARDHELP, eDCLRelativeTo, eDCLRelativeTo, PREFERENCESINDEX, RELATIVE_PREVIOUS, 8, 0, eDCSNormal, "Help topics" },
};


static const int nControls = sizeof(controls) / sizeof(sAnmDashboardControl);

#define CONTROLINDEX(_n_) (_n_)

/////////////////////////////////////////////////////////////////////////////
// CDashboard1

IMPLEMENT_DYNCREATE(CDashboard1, CWnd)

CDashboard1::CDashboard1()
{
	EnableAutomation();
	
	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
	
	AfxOleLockApp();

	m_overControl = NULL;
	m_pressedControl = NULL;
	m_inDrag = false;
	m_hViewMenu = NULL;
	m_hHelpMenu = NULL;
	m_tooltip = NULL;
	m_font = m_bigfont = m_smallfont = NULL;
	m_app = NULL;

	LoadBackground();
	CreateControls();
}


CDashboard1::~CDashboard1()
{
	DestroyControls();

	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	SafeDelete(m_tooltip);

	AfxOleUnlockApp();
}

void CDashboard1::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CDashboard1, CWnd)
	//{{AFX_MSG_MAP(CDashboard1)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CDashboard1, CWnd)
	//{{AFX_DISPATCH_MAP(CDashboard1)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IDashboard1 to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D181AEDA-E6CC-4556-8948-23D2C6222531}
static const IID IID_IDashboard1 =
{ 0xd181aeda, 0xe6cc, 0x4556, { 0x89, 0x48, 0x23, 0xd2, 0xc6, 0x22, 0x25, 0x31 } };

BEGIN_INTERFACE_MAP(CDashboard1, CWnd)
	INTERFACE_PART(CDashboard1, IID_IDashboard1, Dispatch)
END_INTERFACE_MAP()

// {25B54D15-EABB-4E27-B84C-2E2896368E89}
IMPLEMENT_OLECREATE(CDashboard1, "Anima.Dashboard", 0x25b54d15, 0xeabb, 0x4e27, 0xb8, 0x4c, 0x2e, 0x28, 0x96, 0x36, 0x8e, 0x89)


/////////////////////////////////////////////////////////////////////////////
// CDashboard1 message handlers

void CDashboard1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	HFONT hOldFont;

	if (m_font)
		hOldFont = (HFONT) dc.SelectObject(m_font);

	PaintBackground(&dc);
	PaintControls(&dc);

	if (m_font)
		dc.SelectObject(hOldFont);
}

int CDashboard1::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	RECT r;
	GetClientRect(&r);

	// create a DC for the control bitmaps
	CDC *pDC = GetDC();
	m_memDC.CreateCompatibleDC(pDC);
	ReleaseDC(pDC);

	MakeFonts(pDC->GetSafeHdc());

	// set up the timer
	SetTimer(1, 100, NULL);

	// create tooltip window
	m_tooltip = new CAnmTooltip;
	BOOL stat = m_tooltip->Create(CPoint(0, 0), NULL, CAnmTooltip::ANMTOOLTIP_JUSTIFYLEFT);


	if (!stat)
	{
		char * lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError(); 

		dw = 0x0000057F;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		lpDisplayBuf = LocalAlloc(LMEM_ZEROINIT, 
			(strlen(lpMsgBuf)+40)*sizeof(TCHAR)); 
		wsprintf((char *) lpDisplayBuf, 
			TEXT("m_tooltip->Create failed with error %d: %s"), dw, lpMsgBuf); 
		MessageBox((char *) lpDisplayBuf, TEXT("Error"), MB_OK); 

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
	}

	return 0;
}

void CDashboard1::LoadBackground()
{
	BOOL loaded = m_bgbitmap.LoadBitmap(IDB_DASHBOARD);
	if (loaded)
	{
		BITMAP bmp;
		m_bgbitmap.GetObject(sizeof(BITMAP), &bmp);

		m_bgbitwidth = bmp.bmWidth;
		m_bgbitheight = bmp.bmHeight;
	}
	else
		m_bgbitwidth = m_bgbitheight = 0;
}

void CDashboard1::PaintBackground(CDC *pDC)
{
  //CDC dcMem;                  // memory device context
  // dcMem.CreateCompatibleDC(pDC);

	if (m_bgbitmap.GetSafeHandle() == NULL)
		return;

	RECT r;
	GetClientRect(&r);

	CBitmap* pOldBmp = (CBitmap*) m_memDC.SelectObject(&m_bgbitmap);
	pDC->StretchBlt(0, 0, r.right, r.bottom, &m_memDC, 0, 0, m_bgbitwidth, m_bgbitheight, SRCCOPY);
	m_memDC.SelectObject(pOldBmp);
}

void CDashboard1::CreateControls()
{
	for (int i = 0; i < nControls; i++)
	{
		CAnmDashboardControl *pControl = new CAnmDashboardControl(this, 
			controls[i].resID, controls[i].commandID, 0, 0, false,
			controls[i].initialState,
			controls[i].tooltip);

		m_controls.push_back(pControl);
	}

	SetNavigationMode(eNavigationModeFly);

	m_controls[VIEWNAMESINDEX]->SetLabel("Views");
}

void CDashboard1::DestroyControls()
{
	int sz = m_controls.size();
	for (int i = 0; i < sz; i++)
	{
		CAnmDashboardControl *pControl = m_controls[i];
		if (pControl)
			delete pControl;
	}
}

void CDashboard1::PaintControls(CDC *pDC)
{
	int sz = m_controls.size();
	for (int i = 0; i < sz; i++)
	{
		CAnmDashboardControl *pControl = m_controls[i];
		if (pControl)
			pControl->Paint(pDC, &m_memDC);
	}
}

void CDashboard1::LayoutControls(int cx, int cy)
{
	int width = cx;
	int height = cy;
	float scaleX = (float) width / ANMDASHBOARD_IDEALWIDTH;
	float scaleY = (float) height / ANMDASHBOARD_BIGHEIGHT;
	scaleX = 1.0;
	scaleY = 1.0;
	bool overflow = false;

	RECT overflowRect;
	overflowRect.left = 0;
	overflowRect.right = cx;
	overflowRect.top = 0;
	overflowRect.bottom = cy;

	for (int i = 0; i < nControls; i++)
	{
		LPCRECT pControlBounds = m_controls[i]->GetBoundingRect();
		int cwidth;
		int cheight;
		m_controls[i]->GetBitmapDimensions(&cwidth, &cheight);
		LPCRECT pRelativeControlBoundsX;
		LPCRECT pRelativeControlBoundsY;

		int x, y;
		int relindexX, relindexY;
		int centerx, centery;

		// X layout
		switch (controls[i].layoutX)
		{
			case eDCLFixed :
				x = controls[i].xOffset * scaleX;
				break;

			case eDCLRelativeTo :
				relindexX = controls[i].relativeToX;

				if (relindexX == RELATIVE_PREVIOUS)
					relindexX = i - 1;

				assert(relindexX >= 0 && relindexX < nControls);

				pRelativeControlBoundsX = m_controls[relindexX]->GetBoundingRect();

				if (controls[i].xOffset > 0)
					x = pRelativeControlBoundsX->right + (float) controls[i].xOffset * scaleX;
				else if (controls[i].xOffset < 0)
					x = pRelativeControlBoundsX->left + (float) (controls[i].xOffset - cwidth) * scaleX;
				else
					x = pRelativeControlBoundsX->left;
				break;

			case eDCLRelativeToCenter :
				relindexX = controls[i].relativeToX;

				if (relindexX == RELATIVE_PREVIOUS)
					relindexX = i - 1;

				assert(relindexX >= 0 && relindexX < nControls);

				pRelativeControlBoundsX = m_controls[relindexX]->GetBoundingRect();

				centerx = pRelativeControlBoundsX->left +
					(pRelativeControlBoundsX->right - pRelativeControlBoundsX->left) / 2;
				x = centerx + (float) (controls[i].xOffset - cwidth / 2)  * scaleX;
				break;

			case eDCLCenter :
				x = (float) ((width - cwidth) / 2 + controls[i].xOffset) * scaleX;
				break;

			case eDCLLeft :
				x = controls[i].xOffset  * scaleX;
				break;

			case eDCLRight:
				x = (float) (width - cwidth - controls[i].xOffset) * scaleX;
				break;

			default :
				assert(FALSE);
				break;
		}

		// Y layout
		switch (controls[i].layoutY)
		{
			case eDCLFixed :
				y = controls[i].yOffset * scaleY;
				break;

			case eDCLRelativeTo :

				relindexY = controls[i].relativeToY;

				if (relindexY == RELATIVE_PREVIOUS)
					relindexY = i - 1;

				assert(relindexY >= 0 && relindexY < nControls);

				pRelativeControlBoundsY = m_controls[relindexY]->GetBoundingRect();

				if (controls[i].yOffset > 0)
					y = pRelativeControlBoundsY->bottom + (float) controls[i].yOffset * scaleY;
				else if (controls[i].yOffset < 0)
					y = pRelativeControlBoundsY->top + (float)(controls[i].yOffset - cheight) * scaleY;
				else
					y = pRelativeControlBoundsY->top;

				break;

			case eDCLRelativeToCenter :
				relindexY = controls[i].relativeToY;

				if (relindexY == RELATIVE_PREVIOUS)
					relindexY = i - 1;

				assert(relindexY >= 0 && relindexY < nControls);

				pRelativeControlBoundsY = m_controls[relindexY]->GetBoundingRect();

				centery = pRelativeControlBoundsY->top +
					(pRelativeControlBoundsY->bottom - pRelativeControlBoundsY->top) / 2;
				y = centery + controls[i].yOffset - cheight / 2;
				break;

			case eDCLCenter :
				y = (height - cheight) / 2 + controls[i].yOffset;
				break;

			case eDCLTop :
				y = controls[i].yOffset * scaleY;
				break;

			case eDCLBottom:
				y = (float) (height - cheight - controls[i].yOffset) * scaleY;
				break;

			default :
				assert(FALSE);
				break;
		}

		m_controls[i]->MoveTo(x, y);
		m_controls[i]->SetSize(cwidth * scaleX, cheight * scaleY);

		pControlBounds = m_controls[i]->GetBoundingRect();

		if (pControlBounds->left < 0)
		{
			overflowRect.left = pControlBounds->left;
			overflow = true;
		}
		if (pControlBounds->right > width)
		{
			overflowRect.right = pControlBounds->right;
			overflow = true;
		}
		if (pControlBounds->top < 0)
		{
			overflowRect.top = pControlBounds->top;
			overflow = true;
		}
		if (pControlBounds->bottom > height)
		{
			overflowRect.bottom = pControlBounds->bottom;
			overflow = true;
		}
	}

	if (overflow)
	{
		// not sure what to do yet
		int helpme = 1;
	}
}

void CDashboard1::PollControls(POINT pt)
{
	RECT r;
	GetClientRect(&r);
	if (!PtInRect(&r, pt))
	{
		SetOverControl(NULL);
		return;
	}

	bool found = false;
	int sz = m_controls.size();
	for (int i = 0; i < sz; i++)
	{
		CAnmDashboardControl *pControl = m_controls[i];
		if (pControl && pControl->Poll(pt))
		{
			SetOverControl(pControl);
			found = true;
			break;
		}
	}

	if (!found)
		SetOverControl(NULL);
}

void CDashboard1::OnTimer(UINT nIDEvent) 
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  
  if (m_inDrag)
		return;

	if (!IsWindowVisible())
		return;

	CWnd *pCapture = GetCapture();
	if (pCapture && pCapture != this)
		return;

	POINT point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	PollControls(point);

}

void CDashboard1::SetOverControl(CAnmDashboardControl *pControl)
{
	if (pControl == m_overControl)
		return;

	m_tooltip->Cancel(); // new item, or no item: cancel popup text

	if (m_overControl)
		m_overControl->SetHighlighted(false);
	
	m_overControl = pControl;

	if (m_overControl)
	{
		// new item, and not wholly visible: prepare popup tip
		LPCRECT pBounds = m_overControl->GetBoundingRect();
		CRect r = *pBounds;
		ClientToScreen(&r);	// text rect in screen coords
		RECT toolrect;
		m_tooltip->GetWindowRect(&toolrect);
		CRect tr = toolrect;
		m_tooltip->SetWindowText(m_overControl->GetTooltip());	// set tip text to that of item

		// move tip window over list text
		m_tooltip->SetWindowPos(NULL, r.left + 2, r.bottom + 2, 0,
			0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE);

		m_tooltip->ShowDelayed(0); // show popup text delayed

		if (m_overControl->GetState() != eDCSDimmed)
			m_overControl->SetHighlighted(true);
	}

}

void CDashboard1::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_inDrag = true;
//	SetFocus();
	SetCapture();

	if (m_overControl && (m_overControl->GetState() != eDCSDimmed))
	{
		m_overControl->SetHighlighted(false);
		m_overControl->SetPressed(true);
		m_pressedControl = m_overControl;

		// do "radio group" thing for move/rotate
		switch (m_pressedControl->GetCommandID())
		{
			case IDC_MOVE :

				m_controls[ROTATEINDEX]->SetPressed(false);

				break;
		
			case IDC_ROTATE :

				m_controls[MOVEINDEX]->SetPressed(false);

				break;
		}
	}
}


void CDashboard1::OnLButtonUp(UINT nFlags, CPoint point) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ReleaseCapture();

	PollControls(point);

	// unpress if a button was being pushed and now it's not over
	if (m_pressedControl && m_pressedControl != m_overControl)
	{
		m_pressedControl->SetPressed(false);

		switch (m_pressedControl->GetCommandID())
		{
			case IDC_MOVE :

				if (m_navmode == eNavigationModeExamine)
				{
					m_controls[ROTATEINDEX]->SetPressed(true);
				}
				else
				{
					m_controls[MOVEINDEX]->SetPressed(true);
				}
				break;
		
			case IDC_ROTATE :

				if (m_navmode != eNavigationModeExamine)
				{
					m_controls[MOVEINDEX]->SetPressed(true);
				}
				else
				{
					m_controls[ROTATEINDEX]->SetPressed(true);
				}
				break;
		}

		m_pressedControl = NULL;
	}

	if (m_pressedControl)
	{
		CAnmViewer *pViewer = m_app->GetViewer();

		int command = m_pressedControl->GetCommandID();
		switch (command)
		{
			// do "radio group" thing for move/rotate
			case IDC_MOVE :

				if (pViewer)
					pViewer->SetNavigationMode(eNavigationModeFly);

				SetNavigationMode(eNavigationModeFly);

				break;
		
			case IDC_ROTATE :

				if (pViewer)
					pViewer->SetNavigationMode(eNavigationModeExamine);

				SetNavigationMode(eNavigationModeExamine);

				break;

			case IDC_SEEK  :
				Seek();
				break;

			case IDC_STRAIGHTEN :
				Straighten();
				break;

			case IDC_UNDOMOVE  :
				Undo();
				break;

			case IDC_REDOMOVE :
				Redo();
				break;

			case IDC_VIEWNEXT :
				NextView();
				break;

			case IDC_VIEWNAMES :
				TrackViewMenu(point);
				break;

			case IDC_VIEWPREVIOUS :
				PreviousView();
				break;

			case IDC_DASHBOARDPREFS  :
				Preferences();
				break;

			case IDC_DASHBOARDINFO  :
				Info();
				break;

			case IDC_DASHBOARDHELP  :
				TrackHelpMenu(point);
				break;

			default :
				break;
		}

		if (command != IDC_MOVE && command != IDC_ROTATE)
			m_pressedControl->SetPressed(false);

	}


	m_pressedControl = NULL;

	m_inDrag = false;
}


void CDashboard1::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx != 0 && cy != 0)
		LayoutControls(cx, cy);	
}

void CDashboard1::SetViewMenu(HMENU hViewMenu)
{
	m_hViewMenu = hViewMenu;
}

void CDashboard1::SetHelpMenu(HMENU hHelpMenu)
{
	m_hHelpMenu = hHelpMenu;
}

void CDashboard1::Seek()
{
	CAnmViewer *pViewer = m_app->GetViewer();
	
	if (pViewer)
		pViewer->SetSeekMode(ANMVIEWER_SEEKMODESEEK);
}

void CDashboard1::ContinuousSeek()
{
	CAnmViewer *pViewer = m_app->GetViewer();
	
	if (pViewer)
		pViewer->SetSeekMode(ANMVIEWER_SEEKMODESEEK | ANMVIEWER_SEEKMODECONTINUOUS);
	
}

void CDashboard1::Straighten()
{
	CAnmViewer *pViewer = m_app->GetViewer();
	
	if (pViewer)
		pViewer->Straighten();
}

void CDashboard1::Undo()
{
	if (!m_app->CanUndoMove())
		return;

	m_app->UndoMove();

	if (!m_app->CanUndoMove())
	{
		m_controls[UNDOINDEX]->SetState(eDCSDimmed);
	}

	m_controls[REDOINDEX]->SetState(eDCSNormal);
}

void CDashboard1::Redo()
{
	if (!m_app->CanRedoMove())
		return;

	m_app->RedoMove();

	if (!m_app->CanRedoMove())
	{
		m_controls[REDOINDEX]->SetState(eDCSDimmed);
	}

	m_controls[UNDOINDEX]->SetState(eDCSNormal);

}

void CDashboard1::NextView()
{
	m_app->GetWorld()->NextViewpoint();
}

void CDashboard1::TrackViewMenu(POINT point)
{
	CAnmContextMenu::UpdateViewMenu(m_app, m_app->GetViewMenu());

	HWND hWnd = *AfxGetMainWnd();

	ClientToScreen(&point);

	int cmd = TrackPopupMenu(m_hViewMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
		point.x, point.y, 0,
		m_hWnd, NULL);

	if (cmd >= IDR_CTXVIEWPOINTS_BASE && cmd < IDR_CTXVIEWPOINTS_MAX)
	{
		CAnmWorld *pWorld = m_app->GetWorld();
		assert(pWorld);

		std::list<CAnmViewpoint *> *pViewpoints =
			pWorld->GetViewpoints();
		std::list<class CAnmViewpoint *>::iterator iter;

		int i;
		for (i = IDR_CTXVIEWPOINTS_BASE, iter = pViewpoints->begin();
			i < cmd; i++, iter++)
		{
			if (iter == pViewpoints->end())
				break;
		}

		if (iter != pViewpoints->end())
		{
			pWorld->Lock();

			CAnmViewpoint *pViewpoint = *iter;
			pWorld->BindToViewpoint(pViewpoint);
			pViewpoint->Restore();

			pWorld->Unlock();
		}
	}
		
}

void CDashboard1::TrackHelpMenu(POINT point)
{
	if (!m_hHelpMenu)
		return;

	HWND hWnd = *AfxGetMainWnd();

	ClientToScreen(&point);

	int cmd = TrackPopupMenu(m_hHelpMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
		point.x, point.y, 0,
		m_hWnd, NULL);

	switch(cmd)
	{
		case IDR_CTXHELPABOUT :
			m_app->AboutBox();
			break;

		case IDR_CTXHELPSHOWCONSOLE :
			m_app->ShowConsole();
			break;

		case IDR_CTXHELPUSERSGUIDE :
			m_app->ShowUserGuide();
			break;
		
		default :
			break;
	}
		
}

void CDashboard1::PreviousView()
{
	m_app->GetWorld()->PreviousViewpoint();
}

void CDashboard1::Preferences()
{
}

void CDashboard1::Info()
{
	m_app->ShowConsole();
}


void CDashboard1::SetApplication(cAnimaApp *pApp)
{
	m_app = pApp;

}

void CDashboard1::AdviseViewpointBound(CAnmViewpoint *pViewpoint)
{
	if (pViewpoint)
	{
		m_controls[VIEWNAMESINDEX]->SetLabel(pViewpoint->GetDescription()->GetBuf());
	}
}

void CDashboard1::AdviseUndoMoveChange(bool canundo, bool canredo)
{
	if (canundo)
		m_controls[UNDOINDEX]->SetState(eDCSNormal);
	else
		m_controls[UNDOINDEX]->SetState(eDCSDimmed);

	if (canredo)
		m_controls[REDOINDEX]->SetState(eDCSNormal);
	else
		m_controls[REDOINDEX]->SetState(eDCSDimmed);

}

void CDashboard1::SetNavigationMode(eNavigationMode navmode)
{
	m_navmode = navmode;

	switch (navmode)
	{
		case eNavigationModeWalk :
		case eNavigationModeFly :

			// select move mode
			m_controls[MOVEINDEX]->SetPressed(true);
			m_controls[ROTATEINDEX]->SetPressed(false);
			
			break;

		case eNavigationModeExamine :

			m_controls[ROTATEINDEX]->SetPressed(true);
			m_controls[MOVEINDEX]->SetPressed(false);
			break;
		
		default :
			break;
	}
}

void CDashboard1::AdviseNavigationModeChange(int navmode)
{
	eNavigationMode mode = (eNavigationMode) navmode;

	SetNavigationMode(mode);
}

void CDashboard1::AdviseCameraChange(sAnmCameraChange *pCameraChange)
{
	if (m_app->CanUndoMove())
		m_controls[UNDOINDEX]->SetState(eDCSNormal);
	else
		m_controls[UNDOINDEX]->SetState(eDCSDimmed);

	if (m_app->CanRedoMove())
		m_controls[REDOINDEX]->SetState(eDCSNormal);
	else
		m_controls[REDOINDEX]->SetState(eDCSDimmed);
}

void CDashboard1::OnDestroy() 
{
	KillTimer(1);

	if (m_font)
		::DeleteObject(m_font);

	if (m_bigfont)
		::DeleteObject(m_bigfont);

	if (m_smallfont)
		::DeleteObject(m_smallfont);

	m_font = m_bigfont = m_smallfont = NULL;

	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CDashboard1::MakeFonts(HDC hDC)
{
	// our font for big controls is the nonclient status text
	CNonClientMetrics ncm;
	m_bigfont = ::CreateFontIndirect(&ncm.lfStatusFont);

	// we go smaller for small controls, currently 8 pt
	LOGFONT logFont;
	
	memset(&logFont, 0, sizeof(LOGFONT));

	int logpixy = GetDeviceCaps(hDC, LOGPIXELSY);
	if (logpixy == 0)
		logpixy = 72;		// N.B.: why would this ever happen? It does sometimes...
	logFont.lfHeight = -8 * logpixy / 72;
	logFont.lfCharSet = ANSI_CHARSET; 
	logFont.lfWeight = FW_NORMAL;
	logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
	logFont.lfQuality = ANTIALIASED_QUALITY;
	strcpy(logFont.lfFaceName, ncm.lfStatusFont.lfFaceName);

	m_smallfont = ::CreateFontIndirect(&logFont);

	m_font = m_bigfont;
}

void CDashboard1::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	PollControls(point);

	if (m_overControl)
	{
		CAnmViewer *pViewer = m_app->GetViewer();

		int command = m_overControl->GetCommandID();

		if (command == IDC_SEEK)
			ContinuousSeek();
	}
}
