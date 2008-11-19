/********************************************************************************
 * Flux
 *
 * File: Application.h
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

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <windows.h>
#include <list>
#include "keyboard.h"

class cWindow;

struct cCursor
{
public :

	HCURSOR			hCursor;			// N.B. Windows-specific

};

struct iCharacterReceiver
{
	virtual void NewCharacter(int nchar) {};
};


#define ANMDEFAULT_PRIMITIVE_QUALITY eAnmPrimitiveBetterQuality
#define ANMDEFAULT_MAX_TEXTURESIZE 512

// Platform support
#define ANMPLATFORM_VERSION_MAXNAMELEN 128

typedef enum eAnmPlatformType {
	eAnmDirectX,
	eAnmOpenGL,
};

// mouse button codes

#define ANM_NOBUTTON		0
#define ANM_LBUTTON			1
#define ANM_MBUTTON			2
#define ANM_RBUTTON			4
#define ANM_PRIMARYBUTTON	ANM_LBUTTON
#define ANM_MENUBUTTON		ANM_RBUTTON
#define ANM_SCROLLBUTTON	ANM_MBUTTON

#define ANM_MOUSEDEADZONE				2

inline AnmMouseMoved(int origval, int newval)
{
	return abs(newval - origval) >= ANM_MOUSEDEADZONE;
}

typedef enum eAppMessage {
	eAppMsgLinkToUrl,
	eAppMsgCallFunction,
	eAppMsgCallBrowserFunction,
	eAppMsgResetDevice,
} eAppMessage ;

// These types help with cacheing texture assets
typedef std::vector<class CAnmGraphics::Texture *> tAnmTextureArray;
typedef std::vector<class CAnmTextureData *> tAnmTextureDataArray;
typedef std::vector<class CAnmFont *> tAnmFontArray;

class EXPORT cApplication : public iKeyboardReceiver
{
protected:

	//==========--------------------------  Member variables
	
	string	m_title; // Name the window should have
	int		m_width; // Width of our application
	int		m_height; // Height of our application
	int		m_bpp; // Desired depth (may not be possible)

	bool	m_bExclusive; // Is our application exclusive?

	bool	m_bDDrawDriverRequest; // Iterate DDraw drivers?
	string	m_dDrawDriverName; // Named DDraw driver

	bool	m_bActive; // Is the application active?
	bool	m_bPausedForResize; // Is the application being resized?

	uint	m_glFlags; // Flags for the graphics layer

	BOOL	m_righthanded;	// right-handed coords TP
	BOOL	m_multiTexturing;
	eAnmPrimitiveQualityOptions		m_primitiveQuality;
	int		m_maxTextureSize;
	eAnmFailureModeOptions			m_failuremode;

	HINSTANCE m_hInstance; // TP
	HWND	 m_topLevelHWnd;	// TP
	class CAnmDevice *m_device; // TP
	class cInputLayer *m_input; // TP
	class cSoundLayer *m_sound;
	class cWindow *m_mainwindow; // TP
	CWnd			*m_controlwindow;
	class CAnmGraphics					*m_graphics;


	class CAnmTime *m_clock;		// TP
	ANMTIME	m_lastTime;
	double m_lastTimeDelta;

	virtual void InitPrimaryWindow();
	virtual void InitGraphics();
	virtual void InitInput();
	virtual void InitSound();
	virtual void InitExtraSubsystems();

	class CAnmFileManager				*m_filemanager;
	class CAnmPickManager				*m_pickmanager;
	std::list<class CAnmSensor *>		 m_sensors;
	class CAnmViewer					*m_viewer;
	std::list<class CAnmNode *>			 m_eventReceivers;
	std::list<class CAnmObject *>		 m_grabObjects;
	std::vector<class CAnmObject *>		 m_globalobjects;

	tAnmFontArray						 m_fontcache;


	int									 m_mousedown;
	int									 m_mousex;
	int									 m_mousey;
	bool								 m_hasfocus;
	bool								 m_hascapture;

	std::list<iCharacterReceiver*>		
										 m_receivers;

	cCursor		m_cursor;
	class CAnmObject					*m_exclusiveGrab;

	BOOL								 m_navflag;
	ulong								 m_backgroundColor;

	std::list<class CAnmExternalEventSource *>
	
										 m_externalsources;

	std::list<class CAnmExternalResource *>
										 m_externalresources;


	tAnmTextureDataArray				 m_texturedataarray;
	tAnmTextureArray					 m_texturearray;
	tAnmTextureArray					 m_cubetexturearray;

	class CAnmPlatform					*m_platform;
	eAnmPlatformType					 m_platformtype;
	int									 m_platformmajorversion;
	int									 m_platformminorversion;
	char							 m_platformversionname[ANMPLATFORM_VERSION_MAXNAMELEN];

	bool								 m_bFreeingResources;


	void FreeGlobalObjects();
	void FreeTextureData();
	void FreeTextures();

#ifdef WIN32

	HHOOK  								 m_toplevelwndhook;
	void HookAppWindow();
	void UnhookAppWindow();

	static LRESULT CALLBACK WindowsHookProc(int nCode, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK HandleWindowsHook(UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif

public:

	cApplication(HINSTANCE hInstance, CWnd *pWnd = NULL);
	virtual ~cApplication();


	/**
	 * Init the application.  All user-level init should go here
	 */
	virtual void Init();

	// Cleanup
	static void UnloadSharedObjects();

	/**
	 * Handles all code that should be run each frame
	 * timeDelta == amount of time (in seconds) since last call
	 */

	virtual void Simulate();
	virtual void UpdateScene();
	virtual void DoFrame( float timeDelta );
	virtual void DoIdleFrame( float timeDelta );

	/**
	 * Scene Init is called by Init.  This way user-level class 
	 * doesn't need to worry about setting up D3D or handling any 
	 * errors during setup.
	 */
	virtual void SceneInit();

	virtual void SceneEnd();

	void Pause() { m_bActive = false; }
	void UnPause() { m_bActive = true; }
	bool Paused() { return !m_bActive; }
	bool PausedForResize() { return m_bPausedForResize; }

	HINSTANCE GetInstance() { return m_hInstance; }

	class CAnmDevice *GetDevice() { return m_device; }

	class cInputLayer *GetInput() { return m_input; }

	void SetInput(class cInputLayer *pInp) { m_input = pInp; }

	class cSoundLayer *GetSound() { return m_sound; }

	void SetSound(class cSoundLayer *pSnd) { m_sound = pSnd; }

	class CAnmGraphics *GetGraphics() { return m_graphics; }

	class cWindow *MainWindow() { return m_mainwindow; }

	void SetMainWindow (class cWindow *pW) { m_mainwindow = pW; }

	CWnd *GetControlWindow() { return m_controlwindow; }

	HWND GetTopLevelHWnd() { return m_topLevelHWnd; }

	virtual void SetStatusText(char *txt) {}
	virtual void WriteToConsole(char *txt) {}
	virtual void PostAppMsgCall( struct sAnmThreadedCallback *pTCB ) {}

	virtual void Warn(char *txt) {}
	void ShowConsole() {};
	void ShowUserGuide() {};
	void FreeExternalResources();

	virtual void ResetDevice();

	// Timer control
	virtual void InitTimer();
	virtual int GetFrameRate();

	virtual CAnmTime *GetClock() { return m_clock; }

	// Sensor management
	virtual void AddSensor(class CAnmSensor *pSensor);
	virtual void RemoveSensor(class CAnmSensor *pSensor);
	virtual void PollSensors(bool &mouseInput, bool &kbInput);
	virtual void PollViewSensors();
	virtual std::list<class CAnmSensor *> *GetSensors();

	// Viewer interaction
	virtual void PollViewer(bool mouseInput, bool kbInput);

	// Event queue stuff
	virtual void AddEventReceiver(class CAnmNode *pReceiver);
	virtual void HandleEvents();

	// Memory management
	virtual void AddGlobalObject(class CAnmObject *pObject);

	// Help with input
	virtual void HandleMouseDown(int buttoncode, int x, int y) { m_mousedown |= buttoncode; m_mousex = x, m_mousey = y;}
	virtual void HandleMouseMove(int x, int y) { m_mousex = x, m_mousey = y; }
	virtual void HandleMouseUp(int buttoncode, int x, int y) { m_mousedown &= ~buttoncode; }

	virtual void HandleWheel(float zdelta, bool shiftKey, bool ctrlKey) {}

	virtual class CAnmFileManager *GetFileManager()
	{
		return m_filemanager;
	}

	virtual class CAnmPickManager *GetPickManager()
	{
		return m_pickmanager;
	}

	virtual int GetMouseDown();
	virtual int MouseX();
	virtual int MouseY();
	virtual bool HasFocus() { return m_hasfocus; }
	virtual void GetMouseInfo(int *pMouseX, int *pMouseY, int *pMouseButtons);

	// Use this to establish a CharacterReceiver as the current input focus
	virtual void HandleChar(UINT nChar);
	void AddCharacterReceiver( iCharacterReceiver* pTarget );
	void RemoveCharacterReceiver( iCharacterReceiver* pTarget );
	
	BOOL AddGrabObject(class CAnmObject *pGrab);
	BOOL FindGrabObject(class CAnmObject *pGrab);
	void RemoveGrabObject(class CAnmObject *pGrab);

	BOOL SetExclusiveGrab(class CAnmObject *pGrab);
	class CAnmObject *GetExclusiveGrab();
	void ReleaseExclusiveGrab();

	// cursor stuff
	virtual void SetCursor(cCursor cursor);
	virtual cCursor GetCursor() { return m_cursor; }
	virtual void SetDefaultCursor();

	static cCursor helpCursor;
	static cCursor arrowCursor;

	// N.B.: these probaby belong somewhere else
	static cCursor planeSensorCursor;
	static cCursor sphereSensorCursor;
	static cCursor touchSensorCursor;
	static cCursor cylinderSensorCursor;

	static cCursor seekCursor;

	// global options
	void SetOptions(eAnmOptions optindex, int optvalue);
	int GetOptions(eAnmOptions optindex);

	//
	void SetNavigation(BOOL navflag)
	{
		m_navflag = navflag;
	}

	class CAnmViewer *GetViewer() { return m_viewer; }

	void SetBackgroundColor(ulong bgcolor)
	{
		m_backgroundColor = bgcolor;
	}

	ulong GetBackgroundColor()
	{
		return m_backgroundColor;
	}

	virtual class CAnmLayer *CreateAppLayer();

	// Interface to the scene
	virtual class CAnmWorld *GetWorld() { return NULL ; }
	virtual void ReplaceWorld(class CAnmScene *pScene) {}

	// Viewpoint/camera controls
	virtual void AdviseViewpointBound(class CAnmViewpoint *pViewpoint);
	virtual void AdviseNavigationModeChange(int navmode);
	virtual void AdviseCameraChange(struct sAnmCameraChange *pCameraChange);

	virtual void AddExternalEventSource(class CAnmExternalEventSource *pEES);
	virtual void RemoveExternalEventSource(class CAnmExternalEventSource *pEES);

	virtual void AddExternalResource(class CAnmExternalResource *pResource);
	virtual void RemoveExternalResource(class CAnmExternalResource *pResource);

	virtual tAnmFontArray* GetFontCache(){ return &m_fontcache; }


	// Texture cacheing stuff
	tAnmTextureDataArray *GetCachedTextureData()
	{
		return &m_texturedataarray;
	}

	tAnmTextureArray *GetCachedTextures()
	{
		return &m_texturearray;
	}

	tAnmTextureArray *GetCachedCubeTextures()
	{
		return &m_cubetexturearray;
	}

	// User messaging
	virtual void PostMessage(eAppMessage msg, void *param);

#ifdef WIN32
	static LRESULT CALLBACK TopLevelWindowOverride(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

	// Interface to platform drivers
	class CAnmPlatform *GetPlatform()
	{
		return m_platform;
	}
};

// Windows stuff

#ifdef WIN32
#define WM_USERANMLINKTOURL			(WM_USER+1)
#define WM_USERANMCALLBROWSERFUNC	(WM_USER+5)
#define WM_USERANMRESETDEVICE		(WM_USER+6)


#define ANMFLUXAPPPROP "FLUX-APPLICATION"

#endif

#define ANMAPP_MINFRAMETIME 0.01		//N.B.: max 100 FPS

#endif // _APPLICATION_H
