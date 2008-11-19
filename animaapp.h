/********************************************************************************
 * Flux
 *
 * File: animaapp.h
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



#include "application.h"
#include "anmmenucommands.h"
#include "anmviewer.h"

typedef enum eRightMouseMenuType {
	eRightMouseMenuNone,
	eRightMouseMenuShort,
	eRightMouseMenuFull,
} eRightMouseMenuType ;

#define ANM_DEFAULT_RIGHTMOUSEMENUTYPE eRightMouseMenuFull

class EXPORT cAnimaApp : public cApplication, public CAnmCriticalSection
{
protected :

	void ComputeBaseUrl();
	static class CAnmAsset *urlLoadedCallback(String pRequestUrl, String pCacheFileName, void *userData);
	void handleUrlLoaded(String pRequestUrl, String pCacheFileName);
	static void urlProgressCallback(float progress, void *userData);
	void handleUrlProgress(float progress);

	string					 m_cachedfilename;
	class CAnmWorld			*m_world;
	class CAnmBrowser		*m_browser;
	class cRenderThread		*m_prt;
	class CAnmNetworkThread *m_pnt;
	class CAnmPlugin		*m_plugin;
	class CAnmSceneSymbol	*m_emptySceneSymbol;
	char					*m_baseUrl;
	StringArray				*m_urlArray;
	bool					 m_displayon;
	int						 m_rmousex;
	int						 m_rmousey;
	bool					 m_LoadScreen;
	bool					 m_showNagIcon;
	eRightMouseMenuType		 m_rightMouseMenuType;
	bool					 m_waitingForExternalEvents;

	class CAnmContextMenu	*m_ctxMenu;
	class CAnmContextMenu	*m_viewMenu;
	class CAnmContextMenu	*m_speedMenu;
	class CAnmContextMenu	*m_helpMenu;
	std::vector<sAnmCameraChange>	 m_undo;
	int								 m_undoindex;

	HMENU					 m_hMainMenu;
	HMENU					 m_hRMPopup;

	// iKeyboardReceiver overrides
	virtual void KeyDown( int key );
	virtual void KeyUp( int key );

	// Command handling
	void DoMenuCommand(eAnmContextMenuCommand cmd);
	void ClearUndo();
	void AddUndo(sAnmCameraChange *pCameraChange);

	// Load/startup screen stuff
	class CAnmScene *GetAppScene(const char *sceneFileName);
	class CAnmScene *GetHUDScene();
	class CAnmScene *GetLoadScreenScene();

public:


	cAnimaApp(HINSTANCE hInstance, class CAnmPlugin *pPlugin, CWnd *pWnd);

	virtual ~cAnimaApp();


	//==========--------------------------  cApplication

	virtual void Init(); // TP
	virtual void Start();
	virtual void Finish();
	virtual void SetURL(char *str);

	virtual void DoFrame( float timeDelta );
	virtual void SceneInit();

	virtual class CAnmWorld *GetWorld()
	{
		class CAnmWorld *pWorld = NULL;
		
		Lock();
		pWorld = m_world;
		Unlock();

		return pWorld;
	}

	virtual void SceneEnd();

	// cApplication overrides
	virtual void SetStatusText(char *txt);
	virtual void WriteToConsole(char *txt);
	virtual void Warn(char *txt);
	virtual void cAnimaApp::PostAppMsgCall( sAnmThreadedCallback *pTCB );

	virtual void HandleMouseDown(int buttoncode, int x, int y);
	virtual void HandleMouseMove(int x, int y);
	virtual void HandleMouseUp(int buttoncode, int x, int y);
	virtual void HandleWheel(float zdelta, bool shiftKey, bool ctrlKey);

	virtual void PollViewer(bool mouseInput, bool kbInput);

	int GetMouseDown();
	int MouseX();
	int MouseY();
	void SetFocus();
	void KillFocus();
	bool HasFocus();
	void GetMouseInfo(int *pMouseX, int *pMouseY, int *pMouseButtons);

	virtual void SetCursor(cCursor cursor);
	virtual cCursor GetCursor();
	virtual void SetDefaultCursor();

	virtual void HandleKey(UINT nChar);
	virtual void HandleChar(UINT nChar);
	virtual void HandleMouseMove(CPoint point);

	virtual void Simulate();
	virtual void UpdateScene();
	virtual void WaitForExternalEvents();

	// Menu/command methods
	void UndoMove();
	void RedoMove();
	bool CanUndoMove();
	bool CanRedoMove();

	// Accessors
	virtual class CAnmNetworkThread *GetNetworkThread()
	{
		return m_pnt;
	}

	virtual class CAnmBrowser *GetBrowser()
	{
		return m_browser;
	}

	virtual bool GetDisplayOn()
	{
		return m_displayon;
	}

	virtual void SetDisplayOn(bool on)
	{
		m_displayon = on;
	}

	virtual class CAnmContextMenu *GetContextMenu()
	{
		return m_ctxMenu;
	}

	virtual class CAnmContextMenu *GetViewMenu()
	{
		return m_viewMenu;
	}

	virtual class CAnmContextMenu *GetSpeedMenu()
	{
		return m_speedMenu;
	}

	virtual class CAnmContextMenu *GetHelpMenu()
	{
		return m_helpMenu;
	}

	void SetLoadScreen(bool LoadScreen)
	{
		m_LoadScreen = LoadScreen;
	}

	bool GetLoadScreen()
	{
		return m_LoadScreen;
	}

	void SetRightMouseMenuType(eRightMouseMenuType rmtype)
	{
		m_rightMouseMenuType = rmtype;
	}

	eRightMouseMenuType SetRightMouseMenuType()
	{
		return m_rightMouseMenuType;
	}

	virtual class CAnmLayer *CreateAppLayer();

	virtual void AdviseViewpointBound(class CAnmViewpoint *pViewpoint);
	virtual void AdviseNavigationModeChange(int navmode);
	virtual void AdviseCameraChange(struct sAnmCameraChange *pCameraChange);
	virtual void LinkToUrl(const char *url, const char *target);

	virtual void ShowConsole();
	virtual void ShowUserGuide();
	virtual void AboutBox();

	virtual void ResetDevice();

	virtual void ReplaceWorld(class CAnmScene *pScene);
};

