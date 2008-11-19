/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or 
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */




//////////////////////////////////////////////////
//
// CPlugin class implementation
//

#include "stdafx.h"

#include "resource.h"

#include "plugin.h"
#include "scriptablepluginobject.h"
#include "npx3dexternalbrowser.h"
#include "npx3dfield.h"

#include "animaapp.h"
#include "anmdevice.h"
#include "anmmozillaplugin.h"
#include "anmmozplugin.h"
#include "anmcontextmenu.h"
#include "anmurlobject.h"
#include "anmworld.h"

#include "cwindow.h"

#include <mshtmhst.h>

#include "fluxcore\AnmConsole.h"
#include "fluxcore\AnmDashboard.h"
#include "fluxcore\AnmView.h"




#define ANM_TIMER_ID					( 666 )
// status window update interval, in milliseconds
#define ANM_STATUS_INTERVAL				( 250 )
#define ANM_CONSOLE_UPDATE_PER_STATUS	( 2 )


FluxPluginObject::~FluxPluginObject()
{
  if (m_browser)
	  NPN_ReleaseObject(m_browser);
}

NPIdentifier FluxPluginObject::m_methodgetBrowser = 0;
bool FluxPluginObject::m_classInitialized = false;

static NPObject *
AllocateFluxPluginObject(NPP npp, NPClass *aClass)
{
  return new FluxPluginObject(npp);
}

DEFINE_NPOBJECT_CLASS_WITH_BASE(FluxPluginObject,
                                 AllocateFluxPluginObject);

void FluxPluginObject::initClass()
{
	if (m_classInitialized)
		return;

	INIT_NPMETHODID(getBrowser);

	m_classInitialized = true;

}

bool
FluxPluginObject::HasMethod(NPIdentifier name)
{
  return TEST_NPMETHODID(name,getBrowser);
}

bool
FluxPluginObject::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
  if (TEST_NPMETHODID(name,getBrowser)) {

	CNPX3DExternalBrowser *pBrowser = getBrowser();

	OBJECT_TO_NPVARIANT(pBrowser, *result);

    return PR_TRUE;
  }

  return PR_FALSE;
}

bool
FluxPluginObject::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  STRINGZ_TO_NPVARIANT(NPstrdup("[Flux Player Object]"), *result);

  return PR_TRUE;
}

bool
FluxPluginObject::HasProperty(NPIdentifier name)
{
  return false;
}

bool
FluxPluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
/*
  if (name == sBar_id) {
    static int a = 17;

    INT32_TO_NPVARIANT(a, *result);

    a += 5;

    return true;
  }

  VOID_TO_NPVARIANT(*result);
*/

  return false;
}

bool
FluxPluginObject::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
/*
  if (name == sBar_id) {
    printf ("bar set\n");

    int a = 5;

    a = NPVToInteger(*value);

    return true;
  }

*/
  return false;
}

CNPX3DExternalBrowser *
FluxPluginObject::getBrowser()
{
	if (m_browser == NULL)
	{

#ifdef XP_WIN
		BOOL stillgoing = TRUE;

		do {
			MSG msg; 
			stillgoing = GetMessage(&msg, m_plugin->GetHWnd(), 0, 0); 
			DispatchMessage(&msg);
		} while (stillgoing && !m_plugin->IsInitialized() && !m_plugin->AppCreateFailed());

#endif

		if (m_plugin->AppCreateFailed())
			return NULL;

		m_browser = CNPX3DExternalBrowser::Create(mNpp);
		m_browser->SetPlugin(this);
		cAnimaApp *pApp = m_plugin->GetApp();
		m_browser->setClosure(pApp->GetBrowser());
	}

	if (m_browser)
	{
	    NPN_RetainObject(m_browser);

	}

	return m_browser;
 }

#define DASHBOARD_RATIO 8

static eRightMouseMenuType StringToRightMouseMenuType(char *value)
{
	if (!value || !strlen(value))
		return ANM_DEFAULT_RIGHTMOUSEMENUTYPE;

	if (!stricmp(value, "NONE"))
	{
		return eRightMouseMenuNone;
	}
	else if (!stricmp(value, "SHORT"))
	{
		return eRightMouseMenuShort;
	}
	else
	{
		return eRightMouseMenuFull;
	}

	return ANM_DEFAULT_RIGHTMOUSEMENUTYPE;
}

static eAnmFailureModeOptions StringToFailureMode(char *value)
{
	if (!value || !strlen(value))
		return ANM_DEFAULT_FAILUREMODE;

	if (!stricmp(value, "Quiet"))
	{
		return eAnmFailureModeQuiet;
	}
	else if (!stricmp(value, "Verbose"))
	{
		return eAnmFailureModeVerbose;
	}
	else if (!stricmp(value, "Halt"))
	{
		return eAnmFailureModeHalt;
	}

	return ANM_DEFAULT_FAILUREMODE;
}

CPlugin::CPlugin(NPP pNPInstance, uint16 mode, int16 argc, char* argn[], char* argv[]) :
  m_pNPInstance(pNPInstance),
  m_pNPStream(NULL),
  m_bInitialized(FALSE),
  m_pScriptableObject(NULL),
  m_Window( NULL )
{
#ifdef XP_WIN
  m_hWnd = NULL;
#endif

	m_hWnd = NULL;
	m_app = NULL;
	m_pWnd = NULL;
	m_plugin = NULL;
	m_src = NULL;
	m_bgcolor = 0L;
	m_navigation = true;
	m_antialias = true;
	m_primitivequality = eAnmPrimitiveGoodQuality;
	m_dashboardOn = true;
	m_loadscreen = true;
	m_rightmousemenu = ANM_DEFAULT_RIGHTMOUSEMENUTYPE;
	m_maxtexturesize = ANMDEFAULT_MAX_TEXTURESIZE;
	m_failuremode = ANM_DEFAULT_FAILUREMODE;
	m_appcreatefailed = false;
	m_console = NULL;
	m_dashboard = NULL;
	m_view = NULL;


	m_bStatusPending = false;
	m_StatusString[0] = '\0';
	m_bAppMsgCallFuncPending = false;



	for (int i = 0; i < argc; i++)
	{
	  char *name = argn[i];
	  char *value = argv[i];

	  if (!value || !strlen(value))
		  continue;

	  if (!stricmp(name, "src"))
	  {
		  m_src = new CAnmString(value);
	  }
	  else if (!stricmp(name, "bgcolor"))
	  {
		ulong bg = 0L;

		char buf[512];
		
		strcpy(buf, value);

		sscanf(buf, "%x", &bg);

		m_bgcolor = bg;

	  }
	  else if (!stricmp(name, "navigation"))
	  {
		if (!stricmp(value, "none"))
			m_navigation = false;
		else
			m_navigation = true;

	  }
	  else if (!stricmp(name, "antialias"))
	  {
		  if (!stricmp(value, "true") || !strcmp(value, "1"))
			  m_antialias = true;
		  else
			  m_antialias = false;
	  }
	  else if (!stricmp(name, "PrimitiveQuality"))
	  {
			if (!stricmp(value, "GOOD") || !stricmp(value, "LOW") )
				m_primitivequality = eAnmPrimitiveGoodQuality;
			else if (!stricmp(value, "BETTER") || !stricmp(value, "MEDIUM") )
				m_primitivequality = eAnmPrimitiveBetterQuality;
			else if (!stricmp(value, "BEST") || !stricmp(value, "HIGH") )
				m_primitivequality = eAnmPrimitiveBestQuality;
	  }
	  else if (!stricmp(name, "Dashboard"))
	  {
		  if (!stricmp(value, "true") || !strcmp(value, "1"))
			  m_dashboardOn = true;
		  else
			  m_dashboardOn = false;
	  }
	  else if (!stricmp(name, "LoadScreen"))
	  {
		  if (!stricmp(value, "true") || !strcmp(value, "1"))
			  m_loadscreen = true;
		  else
			  m_loadscreen = false;
	  }
	  else if (!stricmp(name, "RightMouseMenu"))
	  {
		  m_rightmousemenu = StringToRightMouseMenuType(value);
	  }
	  else if (!stricmp(name, "MaxTextureSize"))
	  {
		int maxtext = 0L;

		char buf[512];
		
		strcpy(buf, value);

		sscanf(buf, "%d", &maxtext);

		m_maxtexturesize = maxtext;

	  }
	  else if (!stricmp(name, "FailureMode"))
	  {
		  m_failuremode = StringToFailureMode(value);
	  }

	}

	strcpy(m_String,"Hi I'm Almost the Real Flux Dammit");

}

CPlugin::~CPlugin()
{
	// Free the buffer
	SafeDelete(m_src);

	// kill the window
	SafeDelete(m_pw);

	// kill the app
	SafeDelete(m_app);

	// delete the MOZ plugin helper object
	SafeDelete(m_plugin);

	// release the scriptable object
	if (m_pScriptableObject)
		NPN_ReleaseObject(m_pScriptableObject);


}

#ifdef XP_WIN
static LRESULT CALLBACK PluginWinProc(HWND, UINT, WPARAM, LPARAM);
static WNDPROC lpOldProc = NULL;
#endif

NPBool CPlugin::init(NPWindow* pNPWindow)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(pNPWindow == NULL)
	return FALSE;

	m_hWnd = (HWND)pNPWindow->window;
	if(m_hWnd == NULL)
	return FALSE;

	// subclass window so we can intercept window messages and
	// do our drawing to it
	lpOldProc = (WNDPROC) ::SetWindowLong(m_hWnd, GWL_WNDPROC, (long) PluginWinProc);

	// associate window with our nsPluginInstance object so we can access 
	// it in the window procedure
	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

	m_pWnd = new CAnmMozillaPlugin();
	m_pWnd->SubclassWindow(m_hWnd);

	try
	{
		m_plugin = new CAnmMOZPlugin(this);

		m_app = new cAnimaApp(AfxGetInstanceHandle(), m_plugin, m_pWnd);

		m_console = new CAnmConsole(m_pWnd);
		m_console->Create(IDD_CONSOLE, AfxGetMainWnd());


		RECT r;
		m_pWnd->GetClientRect(&r);

		int dbsize = ANMDASHBOARD_BIGHEIGHT;
		if (dbsize > (r.bottom - r.top) / DASHBOARD_RATIO)
			dbsize = ANMDASHBOARD_SMALLHEIGHT;

		m_view = new CAnmView();
		m_view->SetApplication(m_app);

		if (m_dashboardOn)
			r.bottom -= dbsize;
		m_view->Create(NULL, "View", WS_CHILD | WS_VISIBLE, r, m_pWnd, 2);

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

		m_dashboard->Create(NULL, "Dashboard", styleFlags, r, m_pWnd, 1);


		m_pw = new cWindow(m_app, m_view);

		m_app->SetBackgroundColor(m_bgcolor);

		if (!m_navigation)
		  m_app->SetNavigation(FALSE);

		if (m_antialias)
		  m_app->SetOptions(eAnmOptionsAntialiasing, TRUE);

				
		m_app->SetOptions(eAnmOptionsPrimitiveQuality, m_primitivequality);

		// ignore m_dashboardOn right now

		m_app->SetLoadScreen(m_loadscreen);

		m_app->SetRightMouseMenuType((eRightMouseMenuType) m_rightmousemenu);

		m_app->SetOptions(eAnmOptionsMaxTextureSize, m_maxtexturesize);

		m_app->SetOptions(eAnmOptionsFailureMode, m_failuremode);

		m_app->Init();
		if (m_src)
		  m_app->SetURL(m_src->GetBuf());
		m_app->SceneInit();
		m_app->Start();

		SetTimer( m_hWnd, ANM_TIMER_ID, ANM_STATUS_INTERVAL, NULL );

	}
	catch( CAnmError& err )
	{
		m_appcreatefailed = true;

		if (m_failuremode == eAnmFailureModeVerbose)
		{

			if( strstr( err.GetText(), "D3DERROR" ) ) {
				::MessageBox( NULL, "Error: Could not initialize MS Direct3D.\r\nPlease try to update your Graphics Card Drivers.\r\nIf that fails, please contact customer support at support@mediamachines.com", 
					"Flux - Fatal Error", MB_OK|MB_ICONEXCLAMATION );
			}
			else {
				::MessageBox( NULL, err.GetText(), "Flux - Fatal Error", MB_OK|MB_ICONEXCLAMATION );
			}
		}
		else if (m_failuremode == eAnmFailureModeHalt)
		{
			throw err;
		}

		// Clean everything up
		SafeDelete(m_app);

		return TRUE;
	}


	m_bInitialized = TRUE;
	return TRUE;
}

void CPlugin::shut()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// kill the console
	LockConsole();
	if( m_console ) {
		SafeDelete(m_console);
	}
	UnlockConsole();

	// kill the dashboard
	SafeDelete(m_dashboard);

	// kil the view window
	SafeDelete(m_view);

  // subclass it back
  m_pWnd->UnsubclassWindow();

  ::SetWindowLong(m_hWnd, GWL_WNDPROC, (long) lpOldProc);

  m_hWnd = NULL;

	// stop the app
	if (m_app)
		m_app->Finish();	

  m_bInitialized = FALSE;
}

NPBool CPlugin::isInitialized()
{
  return m_bInitialized;
}

int16 CPlugin::handleEvent(void* event)
{
#ifdef XP_MAC
  NPEvent* ev = (NPEvent*)event;
  if (m_Window) {
    Rect box = { m_Window->y, m_Window->x,
                 m_Window->y + m_Window->height, m_Window->x + m_Window->width };
    if (ev->what == updateEvt) {
      ::TETextBox(m_String, strlen(m_String), &box, teJustCenter);
    }
  }
#endif
  return 0;
}

// this will force to draw a version string in the plugin window
void CPlugin::showVersion()
{
  const char *ua = NPN_UserAgent(m_pNPInstance);
  strcpy(m_String, ua);

#ifdef XP_WIN
  InvalidateRect(m_hWnd, NULL, TRUE);
  UpdateWindow(m_hWnd);
#endif

  if (m_Window) {
    NPRect r =
      {
        (uint16)m_Window->y, (uint16)m_Window->x,
        (uint16)(m_Window->y + m_Window->height),
        (uint16)(m_Window->x + m_Window->width)
      };

    NPN_InvalidateRect(m_pNPInstance, &r);
  }
}

// this will clean the plugin window
void CPlugin::clear()
{
  strcpy(m_String, "");

#ifdef XP_WIN
  InvalidateRect(m_hWnd, NULL, TRUE);
  UpdateWindow(m_hWnd);
#endif
}

void CPlugin::getVersion(char* *aVersion)
{
  const char *ua = NPN_UserAgent(m_pNPInstance);
  char*& version = *aVersion;
  version = (char*)NPN_MemAlloc(1 + strlen(ua));
  if (version)
    strcpy(version, ua);
}

NPObject *
CPlugin::GetScriptableObject()
{
  if (!m_pScriptableObject) {
    m_pScriptableObject = (FluxPluginObject*)
      NPN_CreateObject(m_pNPInstance,
                       GET_NPOBJECT_CLASS(FluxPluginObject));
  }

  if (m_pScriptableObject) {
    NPN_RetainObject(m_pScriptableObject);

	m_pScriptableObject->SetPlugin(this);
  }

  return m_pScriptableObject;
}




void CPlugin::PostAppMsgCall( sAnmThreadedCallback *pTCB )
{
	LockAppMsgCallFuncList();
	m_AppMsgCallFuncList.push_back( pTCB );
	m_bAppMsgCallFuncPending = true;
	UnlockAppMsgCallFuncList();
}

void CPlugin::SetStatusText(const char *txt)
{
	LockStatus();
	int len = strlen( txt );
	strncpy( m_StatusString, txt, min( len+1, ANM_STATUSSTRINGLEN-2 ) );
	m_StatusString[ANM_STATUSSTRINGLEN-1] = '\0';
	UnlockStatus();
	m_bStatusPending = true;
}

void CPlugin::WriteToConsole(char *txt)
{
	LockConsole();
	if( m_console ) {
		m_console->Append( txt );
	}
	UnlockConsole();
}





void CPlugin::HandleTimer( int nIDEvent ) 
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

void CPlugin::UpdateConsole( ) 
{
	LockConsole();
	if( m_console ) {
		m_console->Update();
	}
	UnlockConsole();
}

void CPlugin::UpdateStatus( ) 
{
	if( GetNPInstance() && m_bStatusPending ) {
		LockStatus();
		NPN_Status( GetNPInstance(), m_StatusString);
		UnlockStatus();
		m_bStatusPending = false;
	}
}





void CPlugin::ComputeBaseUrl(char *pBaseUrl, int maxCharCount)
{
	assert(pBaseUrl);
	assert(maxCharCount >= 1);

	NPObject *pNPBrowser = NULL;

	// Get the NP Window object
	if (NPN_GetValue(m_pNPInstance, NPNVWindowNPObject, &pNPBrowser) 
		== NPERR_NO_ERROR)
	{
		NPVariant varLocation;

		// Get the Window's location object
		if (NPN_GetProperty(m_pNPInstance, pNPBrowser, NPN_GetStringIdentifier("location"), &varLocation))
		{
			NPObject *pNPLocation = NPVToObject(varLocation);

			// Get the location's href
			if (pNPLocation)
			{
				NPVariant varHref;

				if (NPN_GetProperty(m_pNPInstance, pNPLocation, NPN_GetStringIdentifier("href"), &varHref))
				{
					String href = NPVToString(varHref);

					if (href)
					{
						const char *cp = href->GetBuf();
						int hreflen = strlen(cp);

						if (hreflen > maxCharCount - 1)
							hreflen = maxCharCount - 1;

						strncpy(pBaseUrl, cp, hreflen);
						pBaseUrl[hreflen] = '\0';

						CAnmUrlObject::ComputeBaseUrl(pBaseUrl, pBaseUrl, maxCharCount);
					}
				}

				NPN_ReleaseObject(pNPLocation);
			}
		}

		NPN_ReleaseObject(pNPBrowser);
	}


}

void CPlugin::LinkToUrl(const char *url, const char *target)
{
	NPError stat = NPN_GetURL(m_pNPInstance, url, target);
}


#ifdef XP_WIN

void CPlugin::HandleSetFocus() 
{
	if (m_view)
		m_view->SetFocus();	
}


static LRESULT CALLBACK PluginWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CPlugin *plugin = (CPlugin *)GetWindowLong(hWnd, GWL_USERDATA);
	assert(plugin);

	if (plugin->AppCreateFailed())
		return DefWindowProc(hWnd, msg, wParam, lParam);

	cAnimaApp *pApp = plugin->GetApp();

	WORD xPos, yPos, width, height;

	bool doDefault = true;

  switch (msg) {

	case WM_SETCURSOR :
		
		::SetCursor(pApp->GetCursor().hCursor);

	  break;

    case WM_PAINT:
      {
		if (pApp && !pApp->Paused() && pApp->GetDevice())
		{
			CAnmWorld *pWorld = pApp->GetWorld();

			if (pWorld)
			{
				try
				{

					pWorld->Lock();
					if (pApp->PausedForResize())
						pApp->GetDevice()->Flip();
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
      break;

	case WM_SIZE :
		width = LOWORD(lParam); 
		height = HIWORD(lParam); 
		if( !pApp->Paused() && pApp->GetDevice() )
		{
			CAnmWorld *pWorld = pApp->GetWorld();

			assert(pWorld);

			if( pWorld ) {
				try
				{

					pWorld->Lock();
					pApp->GetDevice()->HandleSize(width, height);
					pWorld->Unlock();
				}
				catch (CAnmError &err)
				{
					pWorld->Unlock();

					::MessageBox( NULL, err.GetText(), "Flux - Fatal Error", MB_OK|MB_ICONEXCLAMATION );
				}
			}		

			plugin->HandleSize(width, height);
		}
		break;

	case WM_LBUTTONDOWN :
		SetFocus(hWnd);
		xPos = LOWORD(lParam); 
		yPos = HIWORD(lParam); 
		pApp->HandleMouseDown(ANM_LBUTTON, xPos, yPos);
		doDefault = false;
		break;

	case WM_LBUTTONUP:
		xPos = LOWORD(lParam); 
		yPos = HIWORD(lParam); 
		pApp->HandleMouseUp(ANM_LBUTTON, xPos, yPos);
		doDefault = false;
		break;

	case WM_MBUTTONDOWN :
		SetFocus(hWnd);
		xPos = LOWORD(lParam); 
		yPos = HIWORD(lParam); 
		pApp->HandleMouseDown(ANM_MBUTTON, xPos, yPos);
		doDefault = false;
		break;

	case WM_MBUTTONUP:
		xPos = LOWORD(lParam); 
		yPos = HIWORD(lParam); 
		pApp->HandleMouseUp(ANM_MBUTTON, xPos, yPos);
		doDefault = false;
		break;

	case WM_RBUTTONDOWN :
		SetFocus(hWnd);
		xPos = LOWORD(lParam); 
		yPos = HIWORD(lParam); 
		pApp->HandleMouseDown(ANM_RBUTTON, xPos, yPos);
		doDefault = false;
		break;

	case WM_RBUTTONUP:
		xPos = LOWORD(lParam); 
		yPos = HIWORD(lParam); 
		pApp->HandleMouseUp(ANM_RBUTTON, xPos, yPos);
		doDefault = false;
		break;

	case WM_MOUSEMOVE :
		xPos = LOWORD(lParam); 
		yPos = HIWORD(lParam); 
		pApp->HandleMouseMove(CPoint(xPos, yPos));
		break;

	case WM_MOUSEWHEEL :
		{
			WORD zDelta = HIWORD(wParam);
			WORD nFlags = LOWORD(wParam);

			float zdelt = (float) zDelta / WHEEL_DELTA;

			bool shiftKey = false;
			if (nFlags & MK_SHIFT)
				shiftKey = true;
			bool ctrlKey = false;
			if (nFlags & MK_CONTROL)
				ctrlKey = true;

			pApp->HandleWheel(zdelt, shiftKey, ctrlKey);
		}

		break;

	case WM_SETFOCUS :
		plugin->HandleSetFocus();
		break;


	case WM_KEYDOWN :
		pApp->HandleKey(wParam);
		break;

	case WM_CHAR :
		pApp->HandleChar(wParam);
		break;

	case WM_USERANMLINKTOURL :
		pApp->GetFileManager()->ProcessLinkRequest((sAnmLinkRequest *) lParam);
		break;

	case WM_USERANMCALLBROWSERFUNC :
		if (plugin->IsInitialized())
			CNPX3DBrowser::CallX3DCallback((sAnmThreadedBrowserCallback *) lParam);
		break;

	case WM_USERANMRESETDEVICE :
		pApp->ResetDevice();
		break;

	case WM_TIMER :
		plugin->HandleTimer( wParam );
		break;

    default:
	  return DefWindowProc(hWnd, msg, wParam, lParam);
      break;
  }

  if (doDefault)
	  return (*lpOldProc) /*DefWindowProc*/ (hWnd, msg, wParam, lParam);
  else
	  return 0L;
}



void CPlugin::PurgeAppMsgCallFuncList( ) 
{
	if( m_bAppMsgCallFuncPending ) {

		while( m_AppMsgCallFuncList.size() > 0 ) {
			LockAppMsgCallFuncList();
			sAnmThreadedCallback* pTCB = m_AppMsgCallFuncList.front();
			m_AppMsgCallFuncList.pop_front();
			UnlockAppMsgCallFuncList();

			if( pTCB ) {
				CNPX3DField::CallX3DCallback(pTCB);
			}
		}

		m_bAppMsgCallFuncPending = false;
	}
}


void CPlugin::ShowConsole()
{
	LockConsole();
	if (m_console) {
		m_console->ShowWindow(SW_SHOW);
	}
	UnlockConsole();
}

void CPlugin::AdviseViewpointBound(class CAnmViewpoint *pViewpoint)
{
	if( m_dashboard ) {
		m_dashboard->AdviseViewpointBound(pViewpoint);
	}
}

void CPlugin::AdviseNavigationModeChange(int navmode)
{
	if( m_dashboard ) {
		m_dashboard->AdviseNavigationModeChange(navmode);
	}
}

void CPlugin::AdviseCameraChange(struct sAnmCameraChange *pCameraChange)
{
	if( m_dashboard ) {
		m_dashboard->AdviseCameraChange(pCameraChange);
	}
}

void CPlugin::AdviseUndoMoveChange(bool canundo, bool canredo)
{
	if( m_dashboard ) {
		m_dashboard->AdviseUndoMoveChange(canundo, canredo);
	}
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

void CPlugin::ShowUserGuide()
{
	showUserGuide();
}

void CPlugin::HandleSize(int width, int height)
{
	int dbsize = ANMDASHBOARD_BIGHEIGHT;
	if (dbsize > (height / DASHBOARD_RATIO))
		dbsize = ANMDASHBOARD_SMALLHEIGHT;

	if (m_dashboardOn)
	{
		m_view->SetWindowPos(NULL, 0, 0, width, height - dbsize, 0);
		if( m_dashboard ) {
			m_dashboard->SetWindowPos(NULL, 0, height - dbsize, width, dbsize, 0);
		}
	}
	else
		m_view->SetWindowPos(NULL, 0, 0, width, height, 0);

}


#endif

