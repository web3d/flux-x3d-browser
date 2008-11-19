/********************************************************************************
 * Flux
 *
 * File: AnimaCtl.cpp 
 * Description: Implementation of the CAnimaCtrl ActiveX Control class
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

// AnimaCtl.cpp : Implementation of the CAnimaCtrl ActiveX Control class.


#include "stdafx.h"
#include "Anima.h"
#include "AnimaCtl.h"
#include "AnimaPpg.h"
#include "fluxcore\AnmConsole.h"
#include "fluxcore\AnmDashboard.h"
#include "fluxcore\AnmView.h"

#include "anmocxplugin.h"
#include "anmcontextmenu.h"

#include "windowing\window.h"
#include "windowing\application.h"
#include "AnimaApp.h"
#include "anmdevice.h"
#include "anmfilemanager.h"
#include "anmviewer.h"
#include "anmviewpoint.h"
#include "anmoctree.h"
#include "anmworld.h"

#include "anmbrowser.h"
#include "x3dexternalbrowser.h"
#include "fluxplayer.h"

#include "x3dfieldbase.h"

#include <mshtmhst.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ANM_TIMER_ID					( 666 )
// status window update interval, in milliseconds
#define ANM_STATUS_INTERVAL				( 50 )
#define ANM_CONSOLE_UPDATE_PER_STATUS	( 4 )

// Console update interval = ANM_STATUS_INTERVAL * ANM_CONSOLE_UPDATE_PER_STAT


IMPLEMENT_DYNCREATE(CAnimaCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CAnimaCtrl, COleControl)
	//{{AFX_MSG_MAP(CAnimaCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(IDR_NEXTVIEWPOINT, OnNextviewpoint)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CAnimaCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CAnimaCtrl)
	DISP_PROPERTY_NOTIFY(CAnimaCtrl, "src", m_src, OnSrcChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CAnimaCtrl, "bgcolor", m_bgcolor, OnBgcolorChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CAnimaCtrl, "navigation", m_navigation, OnNavigationChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CAnimaCtrl, "antialias", m_antialias, OnAntialiasChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CAnimaCtrl, "PrimitiveQuality", m_primitiveQuality, OnPrimitiveQualityChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CAnimaCtrl, "DASHBOARD", m_dashboardOn, OnDASHBOARDChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CAnimaCtrl, "NavigationMode", m_navigationMode, OnNavigationModeChanged, VT_BSTR)
	DISP_FUNCTION(CAnimaCtrl, "jumpTo", jumpTo, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CAnimaCtrl, "getBrowser", getBrowser, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION(CAnimaCtrl, "NextViewpoint", NextViewpoint, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CAnimaCtrl, "PreviousViewpoint", PreviousViewpoint, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CAnimaCtrl, "getFluxPlayer", getFluxPlayer, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_NOTIFY(CAnimaCtrl, "MaxTextureSize", m_maxTextureSize, OnMaxTextureSizeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CAnimaCtrl, "LoadScreen", m_LoadScreen, OnLoadScreenChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CAnimaCtrl, "RightMouseMenu", m_rightMouseMenu, OnRightMouseMenuChanged, VT_BSTR)
	DISP_FUNCTION(CAnimaCtrl, "getBrowserHandle", getBrowserHandle, VT_I4, VTS_NONE)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CAnimaCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CAnimaCtrl, COleControl)
	//{{AFX_EVENT_MAP(CAnimaCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CAnimaCtrl, 1)
	PROPPAGEID(CAnimaPropPage::guid)
END_PROPPAGEIDS(CAnimaCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CAnimaCtrl, "ANIMA.AnimaCtrl.1",
	0x918b202d, 0x8e8f, 0x4649, 0xa7, 0xb, 0xe9, 0xb1, 0x78, 0xfe, 0xdc, 0x58)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CAnimaCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DAnima =
		{ 0x44166b5d, 0xca84, 0x4dfe, { 0x9a, 0xbb, 0x5b, 0x3, 0xe7, 0x2e, 0x9c, 0x5 } };
const IID BASED_CODE IID_DAnimaEvents =
		{ 0xcfb6910f, 0x3849, 0x417e, { 0x92, 0x17, 0xa8, 0x53, 0x2e, 0x7a, 0x13, 0x80 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwAnimaOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CAnimaCtrl, IDS_ANIMA, _dwAnimaOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CAnimaCtrl::CAnimaCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CAnimaCtrl

BOOL CAnimaCtrl::CAnimaCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ANIMA,
			IDB_ANIMA,
			afxRegInsertable | afxRegApartmentThreading,
			_dwAnimaOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CAnimaCtrl::CAnimaCtrl - Constructor

CAnimaCtrl::CAnimaCtrl()
{
	InitializeIIDs(&IID_DAnima, &IID_DAnimaEvents);

	// TODO: Initialize your control's instance data here.

	m_app = NULL;
	m_plugin = NULL;
	m_pw = NULL;
	m_antialias = TRUE;
	m_LoadScreen = TRUE;
	m_browser = NULL;
	m_fluxplayer = NULL;
	m_console = NULL;
	m_dashboard = NULL;
	m_view = NULL;
	m_maxTextureSize = ANMDEFAULT_MAX_TEXTURESIZE;

	m_dashboardOn = TRUE;
	m_appcreatefailed = false;
	m_bStatusPending = false;
	m_bAppMsgCallFuncPending = false;
}


/////////////////////////////////////////////////////////////////////////////
// CAnimaCtrl::~CAnimaCtrl - Destructor

CAnimaCtrl::~CAnimaCtrl()
{
	// kill the app
	if (m_app)
		m_app->Finish();	
	SafeDelete(m_app);

	// kill the console
	LockConsole();
	SafeDelete(m_console);
	UnlockConsole();

	// kill the dashboard
	SafeDelete(m_dashboard);

	// kil the view window
	SafeDelete(m_view);

	// kill the window
	SafeDelete(m_pw);

	SafeDelete(m_plugin);

	SafeRelease(m_browser);
}


/////////////////////////////////////////////////////////////////////////////
// CAnimaCtrl::OnDraw - Drawing function

void CAnimaCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// return;

	// TODO: Replace the following code with your own drawing code.
	RECT r = rcBounds;
	r.top = r.bottom - 64;

	if (m_app && m_app->GetDevice())
	{
		CAnmWorld *pWorld = m_app->GetWorld();

		if (pWorld)
		{
			try
			{

				pWorld->Lock();
				if (m_app->PausedForResize())
					m_app->GetDevice()->Flip();
				pWorld->Unlock();
			}
			catch (CAnmError &err)
			{
				pWorld->Unlock();

				::MessageBox( NULL, err.GetText(), "Flux - Fatal Error", MB_OK|MB_ICONEXCLAMATION );
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CAnimaCtrl::DoPropExchange - Persistence support

void CAnimaCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

	BOOL val;
	val = PX_String(pPX, "src", m_src);
	val = PX_String(pPX, "bgcolor", m_bgcolor);
	val = PX_String(pPX, "navigation", m_navigation);
	val = PX_String(pPX, "PrimitiveQuality", m_primitiveQuality);
	val = PX_String(pPX, "NavigationMode", m_navigationMode);
	val = PX_Bool(pPX, "antialias", m_antialias);
	val = PX_Bool(pPX, "DASHBOARD", m_dashboardOn);
	val = PX_Long(pPX, "MaxTextureSize", m_maxTextureSize);
	val = PX_Bool(pPX, "LoadScreen", m_LoadScreen);
	val = PX_String(pPX, "RightMouseMenu", m_rightMouseMenu);

	// NEED TO ADD m_failuremode
}


/////////////////////////////////////////////////////////////////////////////
// CAnimaCtrl::OnResetState - Reset control to default state

void CAnimaCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CAnimaCtrl::AboutBox - Display an "About" box to the user

void CAnimaCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ANIMA);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CAnimaCtrl message handlers

#define DASHBOARD_RATIO 8

static eRightMouseMenuType StringToRightMouseMenuType(const CString &rightMouseMenu)
{
	if (rightMouseMenu.IsEmpty())
		return ANM_DEFAULT_RIGHTMOUSEMENUTYPE;

	if (!rightMouseMenu.CompareNoCase("NONE"))
	{
		return eRightMouseMenuNone;
	}
	else if (!rightMouseMenu.CompareNoCase("SHORT"))
	{
		return eRightMouseMenuShort;
	}
	else
	{
		return eRightMouseMenuFull;
	}

	return ANM_DEFAULT_RIGHTMOUSEMENUTYPE;
}


int CAnimaCtrl::CreateApp()
{
	IMoniker *pMk;
	IOleClientSite *pSite;
	pSite = GetClientSite();
	HRESULT hr = pSite->GetMoniker(OLEGETMONIKER_FORCEASSIGN, OLEWHICHMK_CONTAINER, &pMk);
	if (SUCCEEDED(hr))
	{
		hr = 1;

	}
	
	try
	{
		m_plugin = new CAnmOCXPlugin(this);

		m_app = new cAnimaApp(AfxGetInstanceHandle(), m_plugin, this);

		// m_pw = new cWindow(m_app, this) ;
	
		m_console = new CAnmConsole(this);
		m_console->Create(IDD_CONSOLE, AfxGetMainWnd());


		RECT r;
		GetClientRect(&r);

		int dbsize = ANMDASHBOARD_BIGHEIGHT;
		if (dbsize > (r.bottom - r.top) / DASHBOARD_RATIO)
			dbsize = ANMDASHBOARD_SMALLHEIGHT;

		m_view = new CAnmView();
		m_view->SetApplication(m_app);

		if (m_dashboardOn)
			r.bottom -= dbsize;
		m_view->Create(NULL, "View", WS_CHILD | WS_VISIBLE, r, this, 2);

		CAnmContextMenu *pContextMenu = m_app->GetContextMenu();

		HMENU hContextMenu = NULL;
		if (pContextMenu)
			hContextMenu = (HMENU) pContextMenu->GetMenuHandle();


		m_dashboard = new CDashboard1();
		m_dashboard->SetApplication(m_app);

		if (hContextMenu)
		{
			HMENU hViewMenu = GetSubMenu(hContextMenu, 9);
			m_dashboard->SetViewMenu(hViewMenu);
			HMENU hHelpMenu = GetSubMenu(hContextMenu, 14);
			m_dashboard->SetHelpMenu(hHelpMenu);
		}

		r.top = r.bottom;
		r.bottom = r.top + dbsize;
		DWORD styleFlags = WS_CHILD;
		if (m_dashboardOn)
			styleFlags |= WS_VISIBLE;

		m_dashboard->Create(NULL, "Dashboard", styleFlags, r, this, 1);

		m_pw = new cWindow(m_app, m_view);

		m_app->Init();

		// set control-related values before loading the file;
		// the file may override them
		if (!m_bgcolor.IsEmpty())
		{
			ulong bg = 0L;

			char buf[512];
			
			strcpy(buf, m_bgcolor);

			sscanf(buf, "%x", &bg);

			m_app->SetBackgroundColor(bg);
		}

		if (!m_navigation.IsEmpty())
		{
			if (!strcmp(m_navigation, "none"))
				m_app->SetNavigation(FALSE);
		}

		if (m_antialias)
		{
			m_app->SetOptions(eAnmOptionsAntialiasing, TRUE);
		}

		m_app->SetOptions(eAnmOptionsMaxTextureSize, m_maxTextureSize);

		if (!m_primitiveQuality.IsEmpty())
		{
			if (!stricmp(m_primitiveQuality, "GOOD") || !stricmp(m_primitiveQuality, "LOW") )
				m_app->SetOptions(eAnmOptionsPrimitiveQuality, eAnmPrimitiveGoodQuality);
			else if (!stricmp(m_primitiveQuality, "BETTER") || !stricmp(m_primitiveQuality, "MEDIUM") )
				m_app->SetOptions(eAnmOptionsPrimitiveQuality, eAnmPrimitiveBetterQuality);
			else if (!stricmp(m_primitiveQuality, "BEST") || !stricmp(m_primitiveQuality, "HIGH") )
				m_app->SetOptions(eAnmOptionsPrimitiveQuality, eAnmPrimitiveBestQuality);
		}

		if (!m_src.IsEmpty())
		{
		   m_app->SetURL( (char *) (const char*) m_src );
		}

		m_app->SetLoadScreen(m_LoadScreen);

		m_app->SetRightMouseMenuType(StringToRightMouseMenuType(m_rightMouseMenu));

		m_app->SceneInit();

		m_app->Start();

	}
	catch( CAnmError& err )
	{
		m_appcreatefailed = true;

		if( strstr( err.GetText(), "D3DERROR" ) ) {
			::MessageBox( NULL, "Error: Could not initialize MS Direct3D.\r\nPlease try to update your Graphics Card Drivers.\r\nIf that fails, please contact customer support at support@mediamachines.com", 
				"Flux - Fatal Error", MB_OK|MB_ICONEXCLAMATION );
		}
		else {
			::MessageBox( NULL, err.GetText(), "Flux - Fatal Error", MB_OK|MB_ICONEXCLAMATION );
		}

		// Clean everything up
		SafeDelete(m_app);

		// TODO: Cleanup your control's instance data here.
		// kill the console
		LockConsole();
		SafeDelete(m_console);
		UnlockConsole();

		// kill the dashboard
		SafeDelete(m_dashboard);

		// kil the view window
		SafeDelete(m_view);

		return 0L;

		return -1;
	}


	// the Browser object, our interface to the outside world
	Lock();

	hr = CoCreateInstance(CLSID_CoX3DExternalBrowser, NULL, 
		CLSCTX_INPROC_SERVER, IID_X3DExternalBrowser, (LPVOID*)&m_browser);

	if (m_browser)
		m_browser->setClosure(m_app->GetBrowser());

	hr = CoCreateInstance(CLSID_FluxPlayer, NULL, CLSCTX_INPROC_SERVER, IID_IFluxPlayer, (LPVOID*)&m_fluxplayer);
	if (m_fluxplayer)
		m_fluxplayer->setFluxControl(this);

	Unlock();


	SetTimer( ANM_TIMER_ID, ANM_STATUS_INTERVAL, NULL );

	return 0;
}

int CAnimaCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect r;
	GetClientRect(&r);
	if (r.Width() > 0 && r.Height() > 0)
		return CreateApp();

	return 0;
}


void CAnimaCtrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);

	if (cx == 0 && cy == 0)
		return;

	if (m_appcreatefailed)
		return;

	if (m_app == NULL)
	{
		CreateApp();
		if (!m_app)
			return;
	}

	int dbsize = ANMDASHBOARD_BIGHEIGHT;
	if (dbsize > (cy / DASHBOARD_RATIO))
		dbsize = ANMDASHBOARD_SMALLHEIGHT;

	if (m_dashboardOn)
	{
		m_view->SetWindowPos(NULL, 0, 0, cx, cy - dbsize, 0);
		m_dashboard->SetWindowPos(NULL, 0, cy - dbsize, cx, dbsize, 0);
	}
	else
		m_view->SetWindowPos(NULL, 0, 0, cx, cy, 0);
	
}


void CAnimaCtrl::OnSrcChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();

	if (!m_app)
		return;

	if (!m_src.IsEmpty())
	{
	   m_app->SetURL( (char *) (const char*) m_src );
	   m_app->SceneInit();
	}

}


void CAnimaCtrl::OnBgcolorChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CAnimaCtrl::OnNavigationChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}



void CAnimaCtrl::PostAppMsgCall( sAnmThreadedCallback *pTCB )
{
	LockAppMsgCallFuncList();
	m_AppMsgCallFuncList.push_back( pTCB );
	m_bAppMsgCallFuncPending = true;
	UnlockAppMsgCallFuncList();
}


void CAnimaCtrl::SetStatusText(char *txt)
{
	LockStatus();
	m_StatusString = txt;
	m_bStatusPending = true;
	UnlockStatus();
}

void CAnimaCtrl::WriteToConsole(char *txt)
{
	LockConsole();
	if( m_console ) {
		m_console->Append( txt );
	}
	UnlockConsole();
}


void CAnimaCtrl::jumpTo(LPCTSTR url, LPCTSTR target) 
{
	if (!m_app)
		return;

	// TODO: Add your dispatch handler code here
	CString curl(url);
	CString ctarget(target);

	m_app->GetFileManager()->LinkToUrl(curl.GetBuffer(256), "", ctarget.GetBuffer(256));
}

void CAnimaCtrl::OnAntialiasChanged() 
{
	if (!m_app)
		return;

	m_app->SetOptions(eAnmOptionsAntialiasing, m_antialias);

	SetModifiedFlag();
}



X3DBrowser *CAnimaCtrl::getBrowser() 
{
#if 0
	Lock();
	LPDISPATCH retval = m_browser;
	Unlock();
#endif

	BOOL stillgoing = TRUE;

	do {
		MSG msg; 
		stillgoing = GetMessage(&msg, m_hWnd, 0, 0); 
		DispatchMessage(&msg);
	} while (stillgoing && m_browser == NULL && !m_appcreatefailed);

	if (m_browser == NULL)
		return NULL;

	int refcount = m_browser->AddRef();
	return m_browser;

}

long CAnimaCtrl::getBrowserHandle() 
{
	return (long) getBrowser();
}

LRESULT CAnimaCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	// N.B.: we use the message queue to handle sequential hyperlinks and
	// url loads in the main (UI) thread
	if (message == WM_USERANMLINKTOURL)
	{
		m_app->GetFileManager()->ProcessLinkRequest((sAnmLinkRequest *) lParam);
	}
	else if (message == WM_USERANMCALLBROWSERFUNC)
	{
		X3DBrowserBase::CallX3DCallback((sAnmThreadedBrowserCallback *) lParam);
	}
	else if (message == WM_USERANMRESETDEVICE) {
		m_app->ResetDevice();
	}
	else if (message == WM_KEYDOWN)
	{
		int foo = 1;
	}

	return COleControl::WindowProc(message, wParam, lParam);
}


#define ARRAYSIZE(a)    (sizeof(a)/sizeof((a)[0]))

int LocalToBSTR(BSTR pWide, LPTSTR pLocal, DWORD dwChars)
{
*pWide = 0;

#ifdef UNICODE
lstrcpyn(pWide, pLocal, dwChars);
#else
MultiByteToWideChar( CP_ACP, 
                     0, 
                     pLocal, 
                     -1, 
                     pWide, 
                     dwChars); 
#endif

return lstrlenW(pWide);
}


#define ANM_USERGUIDE_URL	( "http://www.mediamachines.com/userguide.php" )
#define ANM_USERGUIDE_PATH	( "http://www.mediamachines.com" )

void showUserGuide()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ShellExecute( NULL, NULL, ANM_USERGUIDE_URL, NULL, ANM_USERGUIDE_PATH, SW_SHOWNORMAL );
}

void CAnimaCtrl::Lock()
{
	InitializeCriticalSection(&m_cs);
	EnterCriticalSection(&m_cs);
}

void CAnimaCtrl::Unlock()
{
	LeaveCriticalSection(&m_cs);

}

void CAnimaCtrl::OnPrimitiveQualityChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CAnimaCtrl::OnLoadScreenChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CAnimaCtrl::OnNextviewpoint() 
{
	int foo = 1;	
}


void CAnimaCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	COleControl::OnSetFocus(pOldWnd);
	
	if (m_view)
		m_view->SetFocus();
	
}

void CAnimaCtrl::AdviseViewpointBound(class CAnmViewpoint *pViewpoint)
{
	m_dashboard->AdviseViewpointBound(pViewpoint);
}

void CAnimaCtrl::AdviseNavigationModeChange(int navmode)
{
	m_dashboard->AdviseNavigationModeChange(navmode);
}

void CAnimaCtrl::AdviseCameraChange(struct sAnmCameraChange *pCameraChange)
{
	m_dashboard->AdviseCameraChange(pCameraChange);
}

void CAnimaCtrl::AdviseUndoMoveChange(bool canundo, bool canredo)
{
	m_dashboard->AdviseUndoMoveChange(canundo, canredo);
}

void CAnimaCtrl::ShowConsole()
{
	LockConsole();
	if( m_console ) {
		m_console->ShowWindow(SW_SHOW);
	}
	UnlockConsole();
}

void CAnimaCtrl::ShowUserGuide()
{
	showUserGuide();
}


void CAnimaCtrl::OnDASHBOARDChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();

	if (!m_app)
		return;

	if (!m_dashboardOn && m_dashboard->IsWindowVisible())
	{
		m_dashboard->SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW);
	}
	else if (m_dashboardOn && !m_dashboard->IsWindowVisible())
	{
		m_dashboard->SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW);
	}
}

void CAnimaCtrl::NextViewpoint() 
{
	if (!m_app)
		return;

	m_app->GetWorld()->NextViewpoint();
}

void CAnimaCtrl::PreviousViewpoint() 
{
	if (!m_app)
		return;

	m_app->GetWorld()->PreviousViewpoint();
}

void CAnimaCtrl::OnNavigationModeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();

	if (!m_app)
		return;

	if (m_navigationMode.IsEmpty())
		return;

	CAnmViewer *pViewer = m_app->GetViewer();
	assert(pViewer);

	if (!m_navigationMode.CompareNoCase("MOVE") ||
		!m_navigationMode.CompareNoCase("FLY") ||
		!m_navigationMode.CompareNoCase("WALK"))
	{
		pViewer->SetNavigationMode(eNavigationModeFly);
	}
	else if (!m_navigationMode.CompareNoCase("ROTATE") ||
		!m_navigationMode.CompareNoCase("EXAMINE"))
	{
		pViewer->SetNavigationMode(eNavigationModeExamine);
	}
}

void CAnimaCtrl::OnRightMouseMenuChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();

	if (!m_app)
		return;

	m_app->SetRightMouseMenuType(StringToRightMouseMenuType(m_rightMouseMenu));
}


LPDISPATCH CAnimaCtrl::getFluxPlayer() 
{
	LPDISPATCH retval = NULL;

	Lock();
	retval = m_fluxplayer;
	Unlock();

	return retval;
}

void CAnimaCtrl::OnMaxTextureSizeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}


void CAnimaCtrl::OnTimer( UINT nIDEvent ) 
{
	if( nIDEvent == ANM_TIMER_ID ) {
		static int iSkipper=0;

		if( ( iSkipper++ ) % ANM_CONSOLE_UPDATE_PER_STATUS == 0 ) {
			UpdateConsole();
			UpdateStatus();
		}
		PurgeAppMsgCallFuncList();
	}
}

void CAnimaCtrl::UpdateConsole( ) 
{
	LockConsole();
	if( m_console ) {
		m_console->Update();
	}
	UnlockConsole();
}

void CAnimaCtrl::UpdateStatus( ) 
{
	if( m_pInPlaceFrame && m_bStatusPending ) {

		static wchar_t buf[1000];
		LockStatus();
		mbstowcs( buf, m_StatusString.GetBuffer( 0 ), 999 );
		UnlockStatus();
		m_pInPlaceFrame->SetStatusText(buf);

		m_bStatusPending = false;
	}
}


void CAnimaCtrl::PurgeAppMsgCallFuncList( ) 
{
	if( m_bAppMsgCallFuncPending ) {


		while( m_AppMsgCallFuncList.size() > 0 ) {
			LockAppMsgCallFuncList();
			sAnmThreadedCallback* pTCB = m_AppMsgCallFuncList.front();
			m_AppMsgCallFuncList.pop_front();
			UnlockAppMsgCallFuncList();

			if( pTCB ) {
				X3DFieldBase::CallX3DCallback(pTCB);
			}
		}


		m_bAppMsgCallFuncPending = false;
	}
}


