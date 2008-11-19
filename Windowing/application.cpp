/********************************************************************************
 * Flux
 *
 * File: Application.cpp
 * Description: Base application class - sim loop etc.
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

#include <list>
using namespace std;

#include "..\resource.h"

#include "anmdevice.h"
#include "anmexternaleventsource.h"
#include "anmexternalresource.h"
#include "anmfileloader.h"
#include "anmfilemanager.h"
#include "anmfont.h"
#include "anmkeyboardsensor.h"
#include "anmmessage.h"
#include "anmpickmanager.h"
#include "anmpicksensor.h"
#include "anmplatform.h"
#include "anmproximitysensor.h"
#include "anmtexturedata.h"
#include "anmtime.h"
#include "anmviewport.h"
#include "anmvisibilitysensor.h"

#include "..\input\inputlayer.h"
#include "..\sound\soundlayer.h"
// #include "..\Anima.h"

// For global options; maybe a better way
#include "anmlight.h"
#include "anmoctree.h"
#include "anmtexture.h"
#include "anmtransform.h"


void* ::operator new (size_t size)
{
 void *p=malloc(size); 
 if (p==0) // did malloc succeed?
	// ANMMSG_OUTOFMEMORY : "Out of memory"
   throw CAnmError(ANMMSG_OUTOFMEMORY);
 return p;
}

void ::operator delete (void *p)
{
 free(p); 
}

cCursor cApplication::arrowCursor;
cCursor cApplication::helpCursor;
cCursor cApplication::planeSensorCursor;
cCursor cApplication::sphereSensorCursor;
cCursor cApplication::touchSensorCursor;
cCursor cApplication::cylinderSensorCursor;
cCursor cApplication::seekCursor;

cApplication::cApplication(HINSTANCE hInstance, CWnd *pWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_hInstance = AfxGetInstanceHandle(); // hInstance;
	m_controlwindow = pWnd;
	m_topLevelHWnd = NULL;
	m_device = NULL;
	m_graphics = NULL;
	m_input = NULL;
	m_sound = NULL;
	m_title = string( "Default window name" );
	m_width = 512;
	m_height = 384;
	m_bpp = 32;
	m_bExclusive = false;
	m_bDDrawDriverRequest = false;
	m_bActive = true;
	m_bPausedForResize = false;
	m_bFreeingResources = false;

	m_glFlags = 0;
	m_righthanded = TRUE;		// N.B.: where should we really put this? - TP
	m_primitiveQuality = ANMDEFAULT_PRIMITIVE_QUALITY;
	m_maxTextureSize = ANMDEFAULT_MAX_TEXTURESIZE;
	m_multiTexturing = TRUE;
	m_failuremode = ANM_DEFAULT_FAILUREMODE;

	CWinApp *pWinApp = AfxGetApp();
	// CAnimaApp *pApp = (CAnimaApp *) pWinApp;
	// pApp->SetApp(this);

	m_clock = NULL;

	m_mousedown = FALSE;
	m_mousex = m_mousey = -1;

	m_filemanager = new CAnmFileManager(this);

	m_pickmanager = new CAnmPickManager(this);

	helpCursor.hCursor = LoadCursor(NULL, IDC_HELP);
	arrowCursor.hCursor = LoadCursor(NULL, IDC_ARROW);

	planeSensorCursor.hCursor = LoadCursor(m_hInstance, (const char *) IDC_PLANESENSOR);
	sphereSensorCursor.hCursor = LoadCursor(m_hInstance, (const char *) IDC_SPHERESENSOR);
	touchSensorCursor.hCursor = LoadCursor(m_hInstance, (const char *) IDC_HANDCURSOR);
	cylinderSensorCursor.hCursor = LoadCursor(m_hInstance, (const char *) IDC_CYLINDERSENSOR);
	seekCursor.hCursor = LoadCursor(m_hInstance, (const char *) IDC_SEEKCURSOR);

	m_cursor = arrowCursor;
	m_exclusiveGrab = NULL;

	m_viewer = NULL;

	m_navflag = TRUE;

	m_backgroundColor = 0L;

	m_toplevelwndhook = NULL;

}


cApplication::~cApplication()
{

	UnhookAppWindow();

//	FreeExternalResources();
	FreeGlobalObjects();
	FreeTextureData();

	// krv:
	// Unload fonts here, before textures.
	//
	int sz = m_fontcache.size();
	for (int i = 0; i < sz; i++)
	{
		SafeDelete(m_fontcache[i]);
	}
	m_fontcache.clear();



	FreeTextures();

	SafeUnRef(m_device);
	SafeDelete(m_graphics);

	SafeDelete(m_input);
	SafeDelete(m_sound);

	delete m_filemanager;
	delete m_pickmanager;
}

void cApplication::FreeGlobalObjects()
{
	int sz = m_globalobjects.size();
	for (int i = 0; i < sz; i++)
	{
		CAnmObject *pObject = m_globalobjects[i];
		pObject->UnRef();
	}

	m_globalobjects.clear();
}

void cApplication::FreeExternalResources()
{
	m_bFreeingResources = true;
	std::list<CAnmExternalResource *>::iterator iter;
	for( 
		iter = m_externalresources.begin(); 
		iter != m_externalresources.end();
		iter++ )
	{
		CAnmExternalResource *pResource = *iter;
		pResource->FreeResource();
	}

	m_externalresources.clear();
	m_bFreeingResources = false;
}

void cApplication::UnloadSharedObjects()
{
	CAnmFileLoader::UnloadAllClassSymbols();
	// krv unload fonts in application destructor, where we unload textures.
//	CAnmFont::UnloadAllFonts();
}

 
void cApplication::FreeTextures()
{
	//	m_texturesurfacearray[i]->UnRef();
	// N.B.: this is because we seem to be bleeding texture memory
	// because of some other memory leak; until we find that leak
	// it's safer to explicitly delete to make damn sure we don't
	// hog all the system's texture memory
	int i;
	for ( i = 0; i < m_texturearray.size(); i++) {
		delete m_texturearray[i];
	}
	for ( i = 0; i < m_cubetexturearray.size(); i++) {
		delete m_cubetexturearray[i];
	}


	m_cubetexturearray.clear();
	m_texturearray.clear();
}

void cApplication::FreeTextureData()
{
	for (int i = 0; i < m_texturedataarray.size(); i++)
		m_texturedataarray[i]->UnRef();

	m_texturedataarray.clear();
}

extern HRESULT GetDXVersion(DWORD* pdwDirectXVersion, DWORD *dwDirectXVersionMajor, DWORD *dwDirectXVersionMinor, 
							TCHAR* strDirectXVersion, int cchDirectXVersion);

#define DXNAMEPREFIX "DirectX"
#define MAXDXNAMELEN (ANMPLATFORM_VERSION_MAXNAMELEN - 8)	// for "DirectX "

void cApplication::Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HWND hWnd = m_controlwindow->GetSafeHwnd();

	m_controlwindow->Detach();
	m_controlwindow->Attach(hWnd);

	m_topLevelHWnd = m_controlwindow->GetTopLevelParent()->GetSafeHwnd();

	HookAppWindow();

	char dxversionbuf[MAXDXNAMELEN];

	DWORD dxver, dxvermajor, dxverminor;

	// Get platform info and tuck it away
	m_platformtype = eAnmDirectX;						// only DirectX for now

	HRESULT hr = GetDXVersion(&dxver, &dxvermajor, &dxverminor, dxversionbuf, MAXDXNAMELEN);

	sprintf(m_platformversionname, "%s %s", DXNAMEPREFIX, dxversionbuf);
	
	m_platformmajorversion = dxvermajor;
	m_platformminorversion = dxverminor;

	m_platform = new CAnmPlatform(this, m_platformtype, m_platformmajorversion, m_platformminorversion);

	// Initialize subsystems
	InitTimer();
	InitGraphics();
	InitInput();
	InitSound();
	InitExtraSubsystems();

}

static ATOM fluxappatom = 0 ;
void cApplication::HookAppWindow()
{
	// Make an atom for my pointer and set it to the window; we'll be fetching that on every
	// windows message
	fluxappatom = GlobalAddAtom(ANMFLUXAPPPROP);
	SetProp(m_topLevelHWnd, MAKEINTATOM(fluxappatom), this);

	// hook the app window
	m_toplevelwndhook = SetWindowsHookEx(WH_CALLWNDPROC, WindowsHookProc, m_hInstance, 0);
}

void cApplication::UnhookAppWindow()
{
	// unhook the app window
	BOOL retstat = UnhookWindowsHookEx(m_toplevelwndhook);
	if (!retstat)
		return;

	// clean up the atom
	SetProp(m_topLevelHWnd, MAKEINTATOM(fluxappatom), NULL);
	GlobalDeleteAtom(fluxappatom);

}


LRESULT CALLBACK cApplication::WindowsHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(0, nCode, wParam, lParam);

	CWPSTRUCT *pHookData = (CWPSTRUCT *) lParam;

	if (!pHookData)
		return CallNextHookEx(0, nCode, wParam, lParam);

	HANDLE applong = GetProp(pHookData->hwnd, MAKEINTATOM(fluxappatom));

	if (applong)
	{
		cApplication *pApp = (cApplication *) applong;

		pApp->HandleWindowsHook(pHookData->message, pHookData->wParam, pHookData->lParam);
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

LRESULT CALLBACK cApplication::HandleWindowsHook(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int foo = 1;
	switch (uMsg)
	{
		case WM_ENTERSIZEMOVE :
			Pause();
			m_bPausedForResize = true;
			break;

		case WM_EXITSIZEMOVE :
			m_bPausedForResize = false;
			// N.B.: the following was: m_device->Reset(); ->
			// works better posting deferred reset, because it's thread-safe
			PostMessage(eAppMsgResetDevice, NULL);
			UnPause();
			break;
	}

	return 0L;
}


void cApplication::DoFrame( float timeDelta )
{
	assert (FALSE);
}

void cApplication::DoIdleFrame( float timeDelta )
{

	// oh, just do nothing...
}

void cApplication::UpdateScene()
{
	assert (FALSE);
}

static int nFrames = 0;
static int avgFrameRate = 0;
static double totalFrameTime = 0.0;

void cApplication::Simulate()
{
	ANMTIME newTime = m_clock->GetNewTime();
	ANMTIME delta = newTime - m_lastTime;
	assert(delta >= 0.f);
	if (delta < ANMAPP_MINFRAMETIME)
		return;

	m_lastTime = newTime;
	m_lastTimeDelta = delta;

	nFrames++;
	totalFrameTime += delta;
	avgFrameRate = (int) 1.f / (totalFrameTime / (double) nFrames);


	if( m_bActive )
	{
		// Update the Input devices
		if( GetInput() )
			GetInput()->UpdateDevices();

		// Commit any deferred sound changes
		if ( GetSound() )
			GetSound()->CommitChanges();

		// Poll all our sensors
		bool mouseInput, kbInput;

		PollSensors(mouseInput, kbInput);

		// Handle all events generated by sensors and external programs
		HandleEvents();

		// N.B.: update camera again based on any events this frame
		UpdateScene();

		// N.B.: now run through any new events generated
		// by camera change, and update the scene all over again
		// PollViewSensors();

		// N.B.: we actually have to poll *all* the sensors again because ->
		// maybe there are pickable things in a HUD which moved this frame. ->
		// Any way to optimize this if the camera didn't move? Probably-- TP 07/23/04
		PollSensors(mouseInput, kbInput);
	
		HandleEvents();

		UpdateScene();

		// Render the scene
		DoFrame( delta );

		// Get input from viewer for navigation
		if (m_navflag)
			PollViewer(mouseInput, kbInput);

	}
	else
	{
		DoIdleFrame( delta );
	}

}

void cApplication::AddSensor(CAnmSensor *pSensor)
{
	m_sensors.push_back(pSensor);
}

void cApplication::RemoveSensor(CAnmSensor *pSensor)
{
	m_sensors.remove(pSensor);
}

std::list<class CAnmSensor *> *cApplication::GetSensors()
{
	return &m_sensors;
}

void cApplication::PollSensors(bool &mouseInput, bool &kbInput)
{
	// clear the pick manager's object list; we do this every simulation tick
	// m_pickmanager->ClearPickList();

	mouseInput = false;
	kbInput = false;

	std::list< CAnmSensor * >::iterator iter;
	for( 
		iter = m_sensors.begin(); 
		iter != m_sensors.end();
		iter++ )
	{
		if ((*iter)->NumInstances() > 0)
		{
			(*iter)->Poll();

			// N.B.: this is hack city; maybe we can find a cleaner design soon...
			if ((*iter)->GetClass()->IsA(GETCLASS(CAnmPickSensor)))
			{
				CAnmPickSensor *pPickSensor = (CAnmPickSensor *) (*iter);
				if (pPickSensor->IsOver() || pPickSensor->IsActive())
					mouseInput = true;
			}
			else if ((*iter)->GetClass()->IsA(GETCLASS(CAnmKeyboardSensor)))
			{
				CAnmKeyboardSensor *pKBSensor = (CAnmKeyboardSensor *) (*iter);
				if (pKBSensor->IsActive())
					kbInput = true;
			}
		}
		else
		{
			CAnmSensor *pSensor = *iter;
			int debug = 1 ;
		}
	}
}

void cApplication::PollViewSensors()
{
	std::list< CAnmSensor * >::iterator iter;
	for( 
		iter = m_sensors.begin(); 
		iter != m_sensors.end();
		iter++ )
	{
		if ((*iter)->NumInstances() > 0)
		{
			// N.B.: this is hack city; maybe we can find a cleaner design soon...
			if ((*iter)->GetClass()->IsA(GETCLASS(CAnmProximitySensor)) ||
				(*iter)->GetClass()->IsA(GETCLASS(CAnmVisibilitySensor)))
				(*iter)->Poll();
		}
	}
}

void cApplication::PollViewer(bool mouseInput, bool kbInput)
{
}

void cApplication::AddEventReceiver(CAnmNode *pReceiver)
{
	// N.B.: this *really* needs to check for dup's!
	m_eventReceivers.push_back(pReceiver);
}

BOOL cApplication::AddGrabObject(class CAnmObject *pGrab)
{
	if (m_exclusiveGrab != NULL)
		return FALSE;

	assert(pGrab != NULL);

	m_grabObjects.push_back(pGrab);
	
	return TRUE;
}

BOOL cApplication::FindGrabObject(class CAnmObject *pGrab)
{
	if (m_exclusiveGrab != NULL)
		return FALSE;

	std::list< CAnmObject* >::iterator iter;
	for( 
		iter = m_grabObjects.begin(); 
		iter != m_grabObjects.end();
		iter++ )
	{
		if ((*iter) == pGrab)
			return TRUE;
	}

	return FALSE;
}

void cApplication::RemoveGrabObject(class CAnmObject *pGrab)
{
	if (m_exclusiveGrab != NULL)
		return;

	assert(pGrab != NULL);

	m_grabObjects.remove(pGrab);
}

BOOL cApplication::SetExclusiveGrab(class CAnmObject *pGrab)
{
	if (m_exclusiveGrab)
		return FALSE;
	else
	{
		m_exclusiveGrab = pGrab;
		return TRUE;
	}
}

class CAnmObject *cApplication::GetExclusiveGrab()
{
	return m_exclusiveGrab;
}

void cApplication::ReleaseExclusiveGrab()
{
	m_exclusiveGrab = NULL;
}

void cApplication::AddGlobalObject(class CAnmObject *pObject)
{
	m_globalobjects.push_back(pObject);
}

void cApplication::HandleEvents()
{
	// Handle external program event sources first
	std::list<CAnmExternalEventSource *>::iterator iter;
	for( 
		iter = m_externalsources.begin(); 
		iter != m_externalsources.end();
		iter++ )
	{
		CAnmExternalEventSource *pEES = *iter;

		pEES->ProcessEvents();
	}

	// Now handle internal scene graph event sources
	Boolean hadEvents;

	while (TRUE)
	{
		hadEvents = FALSE;
		std::list< CAnmNode * >::iterator iter;
		for( 
			iter = m_eventReceivers.begin(); 
			iter != m_eventReceivers.end();
			iter++ )
		{
			(*iter)->Lock();
			if ((*iter)->HandleEvents())
				hadEvents = TRUE;
			(*iter)->Unlock();
		}

		if (!hadEvents)
			break;
	}
}


void cApplication::SceneInit()
{
	// by default, we have no scene, so do nothing.
}


void cApplication::InitPrimaryWindow()
{
	new cWindow( this, m_width, m_height, m_title.c_str() );		
}




void cApplication::InitGraphics()
{
	m_device = CAnmDevice::CreateDevice(MainWindow()->GetHWnd(), this);
	m_graphics = CAnmGraphics::CreateGraphics(this);

}


void cApplication::InitInput()
{
	m_input = cInputLayer::Create(this, m_hInstance, m_topLevelHWnd, m_bExclusive, true, true );
}


void cApplication::InitSound()
{
	cSoundLayer::Create( this, MainWindow()->GetHWnd() );
}


void cApplication::InitExtraSubsystems()
{
}


void cApplication::SceneEnd()
{
}


void cApplication::InitTimer()
{
	m_clock = new CAnmTime();

	m_lastTime = ANMTIME_BADTIMEVALUE;
	m_lastTimeDelta = ANMAPP_MINFRAMETIME;

}

int cApplication::GetFrameRate()
{
	assert ( m_lastTimeDelta >= ANMAPP_MINFRAMETIME);
	return (int)(1.f/m_lastTimeDelta);
}

int cApplication::MouseX()
	{ 
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(m_mainwindow->GetHWnd(), &p);
		return p.x;
		
		//return m_mousex;
	}

int cApplication::MouseY() { 
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(m_mainwindow->GetHWnd(), &p);
		return p.y;
		//return m_mousey;
	}

int cApplication::GetMouseDown() {

		if (!m_hasfocus)
			return 0L;

		if (!m_hascapture)
			return 0L;

		int mousedown = 0L;

		int lbutton = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
		int mbutton = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
		int rbutton = GetAsyncKeyState(VK_RBUTTON) & 0x8000;

		// Deal with button-swapping since GetAsyncKeyState returns physical
		// mouse button state, not logical
		if (GetSystemMetrics(SM_SWAPBUTTON))
			swap(lbutton, rbutton);

		if (lbutton)
			mousedown |= ANM_LBUTTON;
		if (mbutton)
			mousedown |= ANM_MBUTTON;
		if (rbutton)
			mousedown |= ANM_RBUTTON;

		// fall back to foreground thread click state so mouse
		// clicks don't fall thru the cracks?
		if (mousedown)
			return mousedown;
		else
			return m_mousedown;
	}

void cApplication::GetMouseInfo(int *pMouseX, int *pMouseY, int *pMouseButtons)
{
	*pMouseX = MouseX();
	*pMouseY = MouseY();
	*pMouseButtons = GetMouseDown();
}


void cApplication::HandleChar(UINT nChar)
{
	std::list<iCharacterReceiver*>::iterator iter;
	for (iter = m_receivers.begin(); 
		iter != m_receivers.end(); iter ++)
	{
		(*iter)->NewCharacter(nChar);
	}
}

void cApplication::AddCharacterReceiver( iCharacterReceiver* pTarget )
{
	m_receivers.push_back(pTarget);
}

void cApplication::RemoveCharacterReceiver( iCharacterReceiver* pTarget )
{
	m_receivers.remove(pTarget);
}


void cApplication::SetCursor(cCursor cursor)
{
	m_cursor = cursor;

	// ::SetCursor(m_cursor.hCursor);
}

void cApplication::SetDefaultCursor()
{
	m_cursor = arrowCursor;

	// ::SetCursor(m_cursor.hCursor);
}

void cApplication::SetOptions(eAnmOptions optindex, int optvalue)
{
	switch (optindex)
	{
		case eAnmOptionsCoordinateSystem :
			if (optvalue == eAnmCoordinatesLeftHanded)
				m_righthanded = FALSE;
			else if (optvalue == eAnmCoordinatesRightHanded)
				m_righthanded = TRUE;
			break;

		case eAnmOptionsAntialiasing :
			if (m_device)
				m_device->SetAntialiasing(optvalue);
			break;

		case eAnmOptionsMultiTexturing :
			m_multiTexturing = optvalue;
			break;

		case eAnmOptionsWireframe :
			if (m_device)
				m_device->SetWireframe(optvalue);
			break;

		case eAnmOptionsPrimitiveQuality :
			switch (optvalue)
			{
				case eAnmPrimitiveGoodQuality :
				case eAnmPrimitiveBetterQuality :
				case eAnmPrimitiveBestQuality :

					m_primitiveQuality = (eAnmPrimitiveQualityOptions) optvalue;
					break;

				default :
					assert(FALSE);
					break;
			}
			break;

		case eAnmOptionsMaxTextureSize:
			m_maxTextureSize = optvalue;
			if (m_device)
				m_device->SetMaxTextureSize(optvalue);
			break;

		case eAnmOptionsDrawTextures :
			CAnmTexture::SetDrawTextures(optvalue);
			break;

		case eAnmOptionsDrawBoundingBoxes :
			CAnmTransform::SetDrawBoxes(optvalue);
			break;

		case eAnmOptionsDrawOctreeBoxes :
			CAnmOctree::SetDrawBoxes(optvalue);
			break;

		case eAnmOptionsShowLights :
			CAnmLight::SetShowLights(optvalue);
			break;
			
		case eAnmOptionsFailureMode:
			m_failuremode = (eAnmFailureModeOptions) optvalue;
			break;

		default :
			assert(FALSE);
			break;
	}
}

int cApplication::GetOptions(eAnmOptions optindex)
{
	int optvalue = 0;

	switch (optindex)
	{
		case eAnmOptionsCoordinateSystem :
			if (m_righthanded)
				optvalue = eAnmCoordinatesRightHanded;
			else
				optvalue = eAnmCoordinatesLeftHanded;
			break;

		case eAnmOptionsWireframe :
			if (m_device)
				optvalue = m_device->GetWireframe();
			break;
		
		case eAnmOptionsAntialiasing :
			if (m_device)
				optvalue = m_device->GetAntialiasing();
			break;

		case eAnmOptionsMultiTexturing :
				optvalue = m_multiTexturing;
			break;

		case eAnmOptionsPrimitiveQuality :
			switch (optvalue)
			{
				case eAnmPrimitiveGoodQuality :
				case eAnmPrimitiveBetterQuality :
				case eAnmPrimitiveBestQuality :

					optvalue = m_primitiveQuality;
					break;

				default :
					assert(FALSE);
					break;
			}
			break;

		case eAnmOptionsMaxTextureSize:
				optvalue = m_maxTextureSize;
			break;

		case eAnmOptionsDrawTextures :
			optvalue = CAnmTexture::GetDrawTextures();
			break;

		case eAnmOptionsDrawBoundingBoxes :
			optvalue = CAnmTransform::GetDrawBoxes();
			break;

		case eAnmOptionsDrawOctreeBoxes :
			optvalue = CAnmOctree::GetDrawBoxes();
			break;

		case eAnmOptionsShowLights :
			optvalue = CAnmLight::GetShowLights();
			break;

		case eAnmOptionsFailureMode:
			optvalue = m_failuremode;
			break;

		default :
			assert(FALSE);
			break;
	}

	return optvalue;
}

void cApplication::AdviseViewpointBound(class CAnmViewpoint *pViewpoint)
{
}

void cApplication::AdviseNavigationModeChange(int navmode)
{
}

void cApplication::AdviseCameraChange(struct sAnmCameraChange *pCameraChange)
{
}

void cApplication::AddExternalEventSource(CAnmExternalEventSource *pEES)
{
	m_externalsources.push_back(pEES);
}

void cApplication::RemoveExternalEventSource(CAnmExternalEventSource *pEES)
{
	m_externalsources.remove(pEES);
}

void cApplication::AddExternalResource(CAnmExternalResource *pResource)
{
	m_externalresources.push_back(pResource);
}

void cApplication::RemoveExternalResource(CAnmExternalResource *pResource)
{
	if( !m_bFreeingResources ) {
		m_externalresources.remove(pResource);
	}
}

void cApplication::ResetDevice()
{
	m_device->Reset();
}






void cApplication::PostMessage(eAppMessage msg, void *param)
{
	UINT wMsg = 0L;

	switch (msg)
	{
		case eAppMsgLinkToUrl :
			wMsg = WM_USERANMLINKTOURL;
			break;

		case eAppMsgCallFunction :
			PostAppMsgCall( ( sAnmThreadedCallback * ) param );
			return;

		case eAppMsgCallBrowserFunction :
			wMsg = WM_USERANMCALLBROWSERFUNC;
			break;

		case eAppMsgResetDevice :
			wMsg = WM_USERANMRESETDEVICE;
			break;

		default :
				return;
			break;
	}

	if (m_controlwindow && m_controlwindow->GetSafeHwnd())
		m_controlwindow->PostMessage(wMsg, 0, (LPARAM) param);
}

class CAnmLayer *cApplication::CreateAppLayer()
{
	return NULL;
}

