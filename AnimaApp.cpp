/********************************************************************************
 * Flux
 *
 * File: animaapp.cpp
 * Description: Flux main application class
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

#include "AnimaApp.h"
#include "anmcontextmenu.h"
#include "anmexternalbrowser.h"
#include "anmcamera.h"
#include "anmcontextmenu.h"
#include "cRenderThread.h"
#include "anmmessage.h"
#include "anmnetworkthread.h"
#include "anmfilemanager.h"
#include "anmfileloader.h"
#include "anmplugin.h"
#include "anmviewpoint.h"
#include "anmdevice.h"
#include "anmviewport.h"
#include "anmlayer.h"
#include "anmnavigationinfo.h"
#include "anmpickmanager.h"
#include "anmproto.h"
#include "anmroute.h"
#include "anmscene.h"
#include "anmupdatevisitor.h"
#include "anmurlobject.h"
#include "anmviewer.h"
#include "anmworld.h"
#include "inputlayer.h"
#include "resource.h"
#include "anmupdatecheck.h"

cAnimaApp::cAnimaApp(HINSTANCE hInstance, CAnmPlugin *pPlugin, CWnd *pWnd)
	: cApplication(hInstance, pWnd)
{
	m_title = string( "Anima Application in a Control" );
	m_prt = NULL;
	m_pnt = NULL;
	m_world = NULL;
	m_plugin = pPlugin;
	m_baseUrl = NULL;
	m_viewer = NULL;
	m_urlArray = NULL;
	m_displayon = false;
	m_rmousex = -1;
	m_rmousey = -1;
	m_hasfocus = false;
	m_hascapture = false;
	m_waitingForExternalEvents = false;

	ComputeBaseUrl();

	m_browser = new CAnmExternalBrowser(this);
	m_browser->SetBaseUrl(m_baseUrl);

	m_emptySceneSymbol = NULL;

	m_ctxMenu = NULL;
	m_viewMenu = NULL;
	m_speedMenu = NULL;
	m_helpMenu = NULL;

	m_hMainMenu = NULL;
	m_hRMPopup = NULL;

	m_hMainMenu = LoadMenu(m_hInstance, MAKEINTRESOURCE(IDR_CTXMENU));
	if (m_hMainMenu)
	{
		m_hRMPopup = GetSubMenu(m_hMainMenu, 0);

		if (m_hRMPopup)
		{
			m_ctxMenu = new CAnmContextMenu(this, m_hRMPopup);

			if (m_ctxMenu)
			{
				m_viewMenu = m_ctxMenu->GetSubMenu(eAnmCtxMenuViewMenu);
				m_speedMenu = m_ctxMenu->GetSubMenu(eAnmCtxMenuSpeedMenu);
				m_helpMenu = m_ctxMenu->GetSubMenu(eAnmCtxMenuHelpMenu);
			}
		}
	}

	m_undoindex = 0;
	m_LoadScreen = true;
	m_showNagIcon = true;
	m_rightMouseMenuType = ANM_DEFAULT_RIGHTMOUSEMENUTYPE;
}

cAnimaApp::~cAnimaApp()
{
	SafeUnRef(m_browser);

	if (m_input)
	{
		cKeyboard *pKeyboard = m_input->GetKeyboard();
		assert(pKeyboard);

		pKeyboard->RemoveReceiver(this);
	}

	// lock the rendering thread until we're completely deleted
	if (m_prt)
		m_prt->Lock();

	if (m_pnt)
		m_pnt->Lock();

	//krv:  Moved this to the CanmWorld destructor, so that the thread can be shut down
	// when we do a restart.
	// Free external resources (like MovieTexture threads, deadly otherwise)
//	FreeExternalResources();

	// Free up global allocations
	FreeGlobalObjects();

	// Clean up our members
	SafeDelete(m_baseUrl);

	SafeDelete(m_viewer);

	SafeDelete(m_world);

	SafeUnRef(m_urlArray);

	// Ok, let 'er rip now

	if (m_prt)
	{
		m_prt->Unlock();
	}

	if (m_pnt)
	{
		m_pnt->Unlock();
	}

	SafeDelete(m_emptySceneSymbol);

	if (m_hMainMenu)
		::DestroyMenu(m_hMainMenu);

}


#define WELCOMEMESSAGE_FORMAT "%s %s (%s)"

void cAnimaApp::Init()
{
	cApplication::Init();

	cKeyboard *pKeyboard = m_input->GetKeyboard();
	assert(pKeyboard);

	pKeyboard->AddReceiver(this);

	const char *browsername = m_browser->getName();
	const char *browserver = m_browser->getVersion();

	char *welcomemessage = (char *) malloc(strlen(WELCOMEMESSAGE_FORMAT) + 
		strlen(browsername) + 
		strlen(browserver) +
		strlen(m_platformversionname) +
		1);

	sprintf(welcomemessage, WELCOMEMESSAGE_FORMAT, browsername, browserver, m_platformversionname);

	WriteToConsole(welcomemessage);

	delete welcomemessage;
}


void cAnimaApp::Start()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_pnt = new CAnmNetworkThread(this);
	m_pnt->Begin();

	m_prt = new cRenderThread(this);
	m_prt->Begin();

	// start up the network thread if our scene posted any file load requests to the queue
	if (m_filemanager->LoadRequestsPending())
		m_pnt->Resume();
}

void cAnimaApp::Finish()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	Pause();
	m_pnt->End(true);		// wait for it
	m_prt->End(true);
	m_prt = NULL;
	m_pnt = NULL;
}

void cAnimaApp::SetURL(char *str)
{
	SafeUnRef(m_urlArray);
	m_urlArray = new StringArray;
	m_urlArray->push_back(new CAnmString(str));
}

void cAnimaApp::ComputeBaseUrl()
{
	m_baseUrl = new char[ANMMAXSTRBUF];
	m_baseUrl[0] = '\0';

	m_plugin->ComputeBaseUrl(m_baseUrl, ANMMAXSTRBUF);
}

CAnmScene *cAnimaApp::GetAppScene(const char *sceneFileName)
{
	char loadscreenbuf[ANMMAXSTRBUF];

	GetModuleFileName(AfxGetInstanceHandle(), loadscreenbuf, ANMMAXSTRBUF - strlen(sceneFileName) - 2);

	char *dir = strrchr(loadscreenbuf, '\\');

	bool gotloadscreendir = true;
	
	if (dir != NULL)
	{
		strcpy(dir + 1, sceneFileName);
	}
	else
		gotloadscreendir = false;

	// Couldn't find the module dir? We're done
	if (!gotloadscreendir)
		return NULL;

	// Couldn't find the file? We're done
	FILE *fp = fopen(loadscreenbuf, "rb");
	if(fp == NULL)
		return NULL;

	fclose(fp);

	// Set up a base URL for parsing
	char baseUrl[ANMMAXSTRBUF];

	strcpy(baseUrl, loadscreenbuf);
		
	CAnmUrlObject::ComputeBaseUrl(baseUrl, baseUrl, ANMMAXSTRBUF);
	String baseUrlString = new CAnmString(baseUrl);

	// Try to load the scene
	CAnmScene *pStartScene = CAnmFileLoader::CreateScene(loadscreenbuf, baseUrlString, 
		this);

	baseUrlString->UnRef();

	return pStartScene;
}

#define ANMLOADSCREENFILENAME "stf\\mmloadscreen.x3d"
#define ANMHUDFILENAME "stf\\mmhud.x3d"

CAnmScene *cAnimaApp::GetHUDScene()
{
	return GetAppScene(ANMHUDFILENAME);
}

CAnmScene *cAnimaApp::GetLoadScreenScene()
{
	return GetAppScene(ANMLOADSCREENFILENAME);
}

void cAnimaApp::SceneInit()
{
	Lock();

	// If we actually have an url, load it
	if (m_urlArray && m_urlArray ->size() > 0)
	{
		// try to load our load/splash screen as start scene - this may return NULL which is OK
		CAnmScene *pStartScene = NULL;

		if (m_LoadScreen)
		{
			pStartScene = GetLoadScreenScene();

		}
		// Load empty scene into the world
		ReplaceWorld(pStartScene);

		m_filemanager->RequestLoadUrl(m_urlArray, m_baseUrl, urlLoadedCallback, NULL,
			NULL, urlProgressCallback, this);
	}
	else {
		ReplaceWorld(NULL);
	}

	Unlock();
}

class CAnmAsset *cAnimaApp::urlLoadedCallback(String pRequestUrl, String pCacheFileName, void *userData)
{
	cAnimaApp *pApp = (cAnimaApp *) userData;

	assert(pApp != NULL);
	pApp->handleUrlLoaded(pRequestUrl, pCacheFileName);

	return NULL;

}

void cAnimaApp::handleUrlLoaded(String pRequestUrl, String pCacheFileName)
{
	// Compute a base url for subsequent loads
	const char *churl = pRequestUrl->GetBuf();

	if (CAnmUrlObject::IsRootedPath(churl) &&
		CAnmUrlObject::IsWebPath( m_baseUrl ) ) {
		char RootUrl[ANMMAXSTRBUF];
		CAnmUrlObject::ComputeRootedUrl( m_baseUrl, RootUrl, ANMMAXSTRBUF );
		strcpy(m_baseUrl, RootUrl);
		strcat(m_baseUrl, churl);
	}
	else if (CAnmUrlObject::IsRelativePath(churl)) {
		strcat(m_baseUrl, churl);
	}
	else {
		strcpy(m_baseUrl, churl);
	}

		
	CAnmUrlObject::ComputeBaseUrl(m_baseUrl, m_baseUrl, ANMMAXSTRBUF);
	String baseUrlString = new CAnmString(m_baseUrl);

	// Load the file
	eAnmReturnStatus stat = eAnmReturnAllBad;
	CAnmScene *pNewScene = NULL;

	BEGIN_PROFILE_BLOCK("LOAD FILE");

	pNewScene = CAnmFileLoader::CreateScene(pCacheFileName->GetBuf(), baseUrlString, 
		this);

	END_PROFILE_BLOCK("LOAD FILE");

	// Unref the newly allocated string; the file loader is holding onto it
	baseUrlString->UnRef();

	// If load successful, replace our world with the new scene
	if (pNewScene)
	{		
		ReplaceWorld(pNewScene);

		// Bind initial viewpoint if it was part of the world URL
		char *viewpointName;

		if ( m_world &&
			 ( viewpointName = CAnmUrlObject::GetViewpointAnchor(pRequestUrl) ) )
		{
			m_world->Lock();

			CAnmNode *pNode = pNewScene->GetNode(viewpointName);
			if (pNode)
			{
				CAnmViewpoint *pViewpoint = GETIMPLEMENTATION(CAnmViewpoint, pNode);
				if (pViewpoint)
					pViewpoint->SetBind(true);
			}

			m_world->Unlock();
		}

	}
}

void cAnimaApp::ReplaceWorld(CAnmScene *pScene)
{
	// Lock the app
	Lock();

	// CSIRO Bug 474:2.2.2 - Clear out any pending pick structures-- they're holding on to 
	// nodes refs
	m_pickmanager->ClearPickList();

	// Get rid of our old scene and set the new one
	SafeDelete(m_world);

	if (pScene == NULL)
	{
		if (m_emptySceneSymbol == NULL)
			m_emptySceneSymbol = CAnmFileLoader::CreateNewTopLevelScene(this);

		pScene = (CAnmScene *) m_emptySceneSymbol->GetClass()->Create();

		// Bind new scene to the app
		pScene->SetApp(this);

		// Make up some file header values
		pScene->SetEncoding(eAnmX3DNoSceneDefined);
		pScene->SetSpecificationVersion("3.0");
	}

	m_world = new CAnmWorld(this, pScene);

	// Clean out our sensor list
	m_sensors.clear();

	// Clean out our lights
	m_device->ClearLights();

	// Recreate the viewer object (for now)
	SafeDelete(m_viewer);
	m_viewer = new CAnmViewer(this);

	// set up default navigation; the scene will change this if necessary
	m_viewer->SetNavigationMode(eNavigationModeFly);

	// Realize the scene
	BEGIN_PROFILE_BLOCK("REALIZE");

	m_world->Realize();

	END_PROFILE_BLOCK("REALIZE");

	// Glue viewer and world together
	m_viewer->SetCamera(m_world->GetCamera());

	// Unlock the app
	Unlock();

	m_browser->HandleWorldLoaded();


	// krv
	// Do check for updates on startup.
	// Only do this once per instance
	//
	static bool s_bDidUpdateCheck = false;
	if( !s_bDidUpdateCheck ) {
		s_bDidUpdateCheck = true;
		new CAnmUpdateCheck( this );
	}
}

void cAnimaApp::urlProgressCallback(float progress, void *userData)
{
	cAnimaApp *pApp = (cAnimaApp *) userData;

	assert(pApp != NULL);
	pApp->handleUrlProgress(progress);
}

void cAnimaApp::handleUrlProgress(float progress)
{
}

void cAnimaApp::SceneEnd()
{
	// what the heck...
}

void cAnimaApp::PollViewer(bool mouseInput, bool kbInput)
{
	assert(m_viewer);
	if( m_viewer == NULL ) {
		return;
	}

	m_viewer->Poll(!mouseInput, !kbInput);
}

void cAnimaApp::HandleKey(UINT nChar)
{
	if (nChar == VK_DELETE || nChar == VK_BACK)
		return;

	if (m_world == NULL) {
		return;
	}

	switch(nChar)
	{
		case ' ' :
		case VK_NEXT :
			m_world->NextViewpoint();
			break;

		case VK_PRIOR :
			m_world->PreviousViewpoint();
			break;

		default :
			break;

	}
}

void cAnimaApp::HandleChar(UINT nChar)
{
	cApplication::HandleChar(nChar);
}

// N.B.: this is only to get around some goofiness with not receiving the backspace key
void cAnimaApp::KeyDown( int key )
{
	if (key == ANMKEY_DELETE || key == ANMKEY_BACK)
		HandleChar('\b');
}

void cAnimaApp::KeyUp( int key )
{
}

void cAnimaApp::HandleMouseMove(CPoint point)
{
	cApplication::HandleMouseMove(point.x, point.y);
}

void cAnimaApp::HandleWheel(float zdelta, bool shiftKey, bool ctrlKey)
{
	if (m_viewer) {
		m_viewer->HandleZScroll(zdelta, shiftKey, ctrlKey);
	}
}

void cAnimaApp::DoFrame( float timeDelta )
{
	// we're probably reloading the scene
	// I think this is best handled with a critical section-- later.

	if (Paused())
		return;

	assert(m_world);
	if (m_world == NULL) {
		return;
	}
	m_world->Render(m_device);

#if 1
	// maybe draw our status text
	ulong bgcolor = m_world->GetBackgroundColor();	// use world's, not ours
	ulong textcolor = ~bgcolor;
	textcolor = ((textcolor & 0xFF) << 16) | ((textcolor & 0xFF0000) >> 16) |
		(textcolor & 0xFF00) | 0xFF000000;
	if (m_displayon)
	{

		char statbuf[1024];
		char *antialias;
		if (GetOptions(eAnmOptionsAntialiasing))
			antialias = "\nAntialiasing ON";
		else
			antialias = "";

		int visiblecount = m_world->GetRenderGraph()->NumVisibleObjects();

		sprintf( statbuf, "Frame rate: %d FPS. Polys = %d. Visible objects = %d. %s", 
			GetFrameRate(),
				m_world->GetPolyCount(), 
				visiblecount,
				antialias);

			m_device->DrawTextString(
				0,
				0,
				textcolor,
				statbuf); 
	}
	else
		// don't ask
		; // m_device->DrawTextString(0, 0, textcolor, " ");
#endif


	// flip the device buffer
	m_device->Flip();
}

void cAnimaApp::UpdateScene()
{
	// Get the scene up to date
	CAnmUpdateVisitor updater;

	assert(m_world);
	if (m_world == NULL) {
		return;
	}

	m_world->Traverse(&updater);
}

void cAnimaApp::HandleMouseDown(int buttoncode, int x, int y)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetCapture(m_mainwindow->GetHWnd());
	m_hascapture = true;

	Lock();

	m_rmousex = x;
	m_rmousey = y;
	cApplication::HandleMouseDown(buttoncode, x, y);
	Unlock();
}

void cAnimaApp::HandleMouseMove(int x, int y)
{ 
	Lock();
	cApplication::HandleMouseMove(x, y); 
	Unlock();
}


void cAnimaApp::HandleMouseUp(int buttoncode, int x, int y)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());


	m_hascapture = false;
	ReleaseCapture();

	if (m_world == NULL) {
		return;
	}

	Lock();

	bool moved = (AnmMouseMoved(m_rmousex, x) || AnmMouseMoved(m_rmousey, y));

	cApplication::HandleMouseUp(buttoncode, x, y);

	Unlock();

	if (!moved && (buttoncode == ANM_MENUBUTTON))
	{
		if (m_rightMouseMenuType != eRightMouseMenuNone)
		{
			CAnmContextMenu *ctxMenu = NULL;

			if (m_rightMouseMenuType == eRightMouseMenuShort)
			{
				ctxMenu = m_helpMenu;
			}
			else if (m_ctxMenu)
			{
				ctxMenu = m_ctxMenu;

				m_world->Lock();

				m_ctxMenu->UpdateContextMenu();
				CAnmContextMenu::UpdateViewMenu(this, m_viewMenu);
				CAnmContextMenu::UpdateSpeedMenu(this, m_speedMenu);

				m_world->Unlock();
			}

			if (ctxMenu)
			{
				eAnmContextMenuCommand cmd = ctxMenu->TrackMenu(x, y);

				if (cmd != eAnmCtxMenuBadCommandID)
				{
					DoMenuCommand(cmd);
				}
			}
		}
	}
}

int cAnimaApp::GetMouseDown()
{ 
	Lock();
	int buttoncode = cApplication::GetMouseDown();
	Unlock();

	return buttoncode;
}

int cAnimaApp::MouseX()
{ 
	Lock();
	int x = cApplication::MouseX();
	Unlock();

	return x;
}

int cAnimaApp::MouseY()
{ 
	Lock();
	int y = cApplication::MouseY();
	Unlock();

	return y;
}

void cAnimaApp::GetMouseInfo(int *pMouseX, int *pMouseY, int *pMouseButtons)
{
	Lock();
	*pMouseX = cApplication::MouseX();
	*pMouseY = cApplication::MouseY();
	*pMouseButtons = cApplication::GetMouseDown();
	Unlock();
}

void cAnimaApp::SetFocus()
{
	Lock();
	m_hasfocus = true;

	if (m_prt)
	{
		m_prt->SetSimState(eAnmSimGotFocus);
	}

	Unlock();
}

void cAnimaApp::KillFocus()
{
	Lock();
	m_hasfocus = false;

	if (m_prt)
	{
		m_prt->SetSimState(eAnmSimLostFocus);
	}

	Unlock();
}

bool cAnimaApp::HasFocus()
{
	Lock();
	bool focus = m_hasfocus;
	Unlock();

	return focus;
}

void cAnimaApp::SetCursor(cCursor cursor)
{
	Lock();
	cApplication::SetCursor(cursor);
	Unlock();
}

cCursor cAnimaApp::GetCursor()
{
	Lock();
	cCursor curs = m_cursor;
	Unlock();

	return curs;
}

void cAnimaApp::SetDefaultCursor()
{
	Lock();
	cApplication::SetDefaultCursor();
	Unlock();
}

void cAnimaApp::PostAppMsgCall( sAnmThreadedCallback *pTCB )
{
	m_plugin->PostAppMsgCall(pTCB);
}

void cAnimaApp::SetStatusText(char *txt)
{
	m_plugin->SetStatusText(txt);
}

void cAnimaApp::WriteToConsole(char *txt)
{
	m_plugin->WriteToConsole(txt);
}

void cAnimaApp::Warn(char *txt)
{
	// ANMMSG_WARNING : "Warning: "
	char *warningmsg = (char *) malloc(strlen(ANMMSG_WARNING) + strlen(txt) + 1); 

	strcpy(warningmsg, ANMMSG_WARNING);
	strcat(warningmsg, txt);
	m_plugin->WriteToConsole(warningmsg);

	// ANMMSG_WARNINGSTATUSMESSAGE : "There were warnings; see FLUX console for details"
	m_plugin->SetStatusText(ANMMSG_WARNINGSTATUSMESSAGE);

	delete warningmsg;
}

void cAnimaApp::Simulate()
{
	Lock();

	if (m_world)
	{
		m_world->Lock();

		cApplication::Simulate();

		m_world->Unlock();
	}

	m_waitingForExternalEvents = false;

	Unlock();
	
	if (m_filemanager->LoadRequestsPending())
		m_pnt->Resume();
}

void cAnimaApp::WaitForExternalEvents()
{
	Lock();

	m_waitingForExternalEvents = true;

	Unlock();

	DWORD sleepTime = m_prt->GetSleepTime();

	while (true)
	{
		Lock();

		bool stillwaiting = m_waitingForExternalEvents;

		Unlock();

		if (!stillwaiting)
			break;

		Sleep(sleepTime);
	}
}

void cAnimaApp::ResetDevice()
{
	if (m_world == NULL || m_device == NULL ) {
		return;
	}
	m_world->Lock();
	m_device->Reset();
	m_world->Unlock();
}

void cAnimaApp::AdviseViewpointBound(CAnmViewpoint *pViewpoint)
{
	if( m_viewer == NULL ) {
		return;
	}

	m_plugin->AdviseViewpointBound(pViewpoint);
	m_viewer->AdviseViewpointBound(pViewpoint);
}

void cAnimaApp::AdviseNavigationModeChange(int navmode)
{
	m_plugin->AdviseNavigationModeChange(navmode);
}

void cAnimaApp::AdviseCameraChange(struct sAnmCameraChange *pCameraChange)
{
	AddUndo(pCameraChange);

	m_plugin->AdviseCameraChange(pCameraChange);
}

void cAnimaApp::ShowConsole()
{
	m_plugin->ShowConsole();
}

void cAnimaApp::ShowUserGuide()
{
	m_plugin->ShowUserGuide();
}

void cAnimaApp::LinkToUrl(const char *url, const char *target)
{
	m_plugin->LinkToUrl(url, target);
}

void cAnimaApp::AboutBox()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDialog about(IDD_ABOUTBOX_ANIMA);
	about.DoModal();
}

void cAnimaApp::DoMenuCommand(eAnmContextMenuCommand cmd)
{

	if( !m_world || !m_viewer ) {
		return;
	}

  AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (cmd)
	{
		case eAnmCtxMenuWalk :
			if (m_viewer)
				m_viewer->SetNavigationMode(eNavigationModeWalk);
			break;

		case eAnmCtxMenuFly :
			if (m_viewer)
				m_viewer->SetNavigationMode(eNavigationModeFly);
			break;

		case eAnmCtxMenuExamine :
			if (m_viewer)
				m_viewer->SetNavigationMode(eNavigationModeExamine);
			break;

		case eAnmCtxMenuSeek :
			if (m_viewer)
				m_viewer->SetSeekMode(true);
			break;

		case eAnmCtxMenuLevel :
			// NIY
			break;

		case eAnmCtxMenuBack :
			if (CanUndoMove())
				UndoMove();
			break;

		case eAnmCtxMenuForward :
			if (CanRedoMove())
				RedoMove();
			break;

		case eAnmCtxMenuViewFirst :
			m_world->FirstViewpoint();
			break;

		case eAnmCtxMenuViewLast :
			m_world->LastViewpoint();
			break;

		case eAnmCtxMenuViewNext :
			m_world->NextViewpoint();
			break;

		case eAnmCtxMenuViewPrevious :
			m_world->PreviousViewpoint();
			break;

		case eAnmCtxMenuViewRestore :
			{
			CAnmViewpoint *pViewpoint = m_world->GetViewpoint();
			if (pViewpoint)
				pViewpoint->Restore();
			}
			break;

		case eAnmCtxMenuSpeedSlower :
			if (m_viewer)
				m_viewer->SetUserSpeed(eUserSpeedSlowest);
			break;

		case eAnmCtxMenuSpeedSlow :
			if (m_viewer)
				m_viewer->SetUserSpeed(eUserSpeedSlow);
			break;

		case eAnmCtxMenuSpeedMedium :
			if (m_viewer)
				m_viewer->SetUserSpeed(eUserSpeedMedium);
			break;

		case eAnmCtxMenuSpeedFast :
			if (m_viewer)
				m_viewer->SetUserSpeed(eUserSpeedFast);
			break;

		case eAnmCtxMenuSpeedFaster :
			if (m_viewer)
				m_viewer->SetUserSpeed(eUserSpeedFastest);
			break;

		case eAnmCtxMenuInfoCenter :
			ShowConsole();
			break;

		case eAnmCtxMenuPreferences :
			// NIY
			break;

		case eAnmCtxMenuHelpUserGuide :
			ShowUserGuide();
			break;

		case eAnmCtxMenuHelpAbout :
			AboutBox();
			break;

		case eAnmCtxMenuBadCommandID :
			break;

		default :
			if ((int) cmd >= (int) eAnmCtxMenuViewpointsBase)
			{
				int viewindex = (int) cmd - (int) eAnmCtxMenuViewpointsBase;

				std::list<CAnmViewpoint *> *pViewpoints =
					m_world->GetViewpoints();
				std::list<class CAnmViewpoint *>::iterator iter;

				int i;
				for (i = 0, iter = pViewpoints->begin(); 
					i < viewindex; i++, iter++)
				{
					if (iter == pViewpoints->end())
						break;
				}

				if (iter != pViewpoints->end())
				{
					m_world->Lock();

					CAnmViewpoint *pViewpoint = *iter;
					m_world->BindToViewpoint(pViewpoint);
					pViewpoint->Restore();

					m_world->Unlock();
				}
			}
			break;
	}
}

void cAnimaApp::UndoMove()
{
	if (m_undoindex <= 0)
		return;

	sAnmCameraChange undo = m_undo[--m_undoindex];

	if (m_viewer) {
		m_viewer->ChangeCamera(undo.viewpoint, undo.frompos, undo.fromdir, undo.fromup);
	}

	m_plugin->AdviseUndoMoveChange(CanUndoMove(), CanRedoMove());
}

void cAnimaApp::RedoMove()
{
	int sz = m_undo.size();

	if (m_undoindex >= sz)
		return;

	sAnmCameraChange undo = m_undo[m_undoindex++];
	
	if (m_viewer) {
		m_viewer->ChangeCamera(undo.viewpoint, undo.topos, undo.todir, undo.toup);
	}

	m_plugin->AdviseUndoMoveChange(CanUndoMove(), CanRedoMove());
}

void cAnimaApp::AddUndo(sAnmCameraChange *pCameraChange)
{
	int sz = m_undo.size();
	if (m_undoindex >= sz)
	{
		m_undo.push_back(*pCameraChange);
		m_undoindex++;
	}
	else
	{
		m_undo[m_undoindex++] = *pCameraChange;
		m_undo.resize(m_undoindex);
	}

	m_plugin->AdviseUndoMoveChange(CanUndoMove(), CanRedoMove());
}

void cAnimaApp::ClearUndo()
{
	m_undo.clear();
	m_undoindex = 0;

	m_plugin->AdviseUndoMoveChange(CanUndoMove(), CanRedoMove());
}

bool cAnimaApp::CanUndoMove()
{
	return (m_undoindex > 0);
}

bool cAnimaApp::CanRedoMove()
{
	int sz = m_undo.size();

	return (m_undoindex < sz);
}

class CAnmLayer *cAnimaApp::CreateAppLayer()
{
	CAnmLayer *pLayer = NULL;

	// load the HUD
	CAnmScene *pHUDScene = GetHUDScene();

	if (pHUDScene)
	{
		NodeArray *pChildren = NULL;
		pHUDScene->GetChildren(&pChildren);

		CAnmNode *pLayerNode = (*pChildren)[0];
		assert(pLayerNode);

		if (!pLayerNode->GetClass()->IsA(GETCLASS(CAnmLayer)))
			assert(false);

		pLayer = (CAnmLayer *) pLayerNode;

		pLayer->Ref();

		pHUDScene->RemoveAllChildren();

		SafeUnRef(pHUDScene);
	}

	return pLayer;
}
