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

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "scriptablepluginobject.h"

#define	ANM_STATUSSTRINGLEN    (1000)

class CPlugin
{
private:
	NPP m_pNPInstance;
	class cAnimaApp			*m_app;
	class CWnd				*m_pWnd;
	class CAnmMOZPlugin		*m_plugin;
	class cWindow			*m_pw;
	String					 m_src;
	ulong					 m_bgcolor;
	bool					 m_navigation;
	bool					 m_antialias;
	int						 m_primitivequality;
	bool					 m_dashboardOn;
	bool					 m_loadscreen;
	int						 m_rightmousemenu;
	int						 m_maxtexturesize;
	int						 m_failuremode;
	bool					 m_appcreatefailed;
	class CAnmView						*m_view;
	class CAnmConsole					*m_console;
	class CDashboard1					*m_dashboard;
	

#ifdef XP_WIN
  HWND m_hWnd; 
#endif

  NPWindow * m_Window;
  
  NPStream * m_pNPStream;
  bool m_bInitialized;
  bool m_bAppMsgCallFuncPending;

  CAnmCriticalSection					 m_criticalSectionStatus;  // just for status string
  CAnmCriticalSection					 m_criticalSectionAppMsgCallFunc;  // just for AppMsg Call Func
  CAnmCriticalSection					 m_criticalSectionConsole;

  bool									 m_bStatusPending;
  char									 m_StatusString[ANM_STATUSSTRINGLEN];
  std::list<struct sAnmThreadedCallback *>		 m_AppMsgCallFuncList;

  class FluxPluginObject *m_pScriptableObject;


	void UpdateConsole( );
	void UpdateStatus( );


public:
  char m_String[128];

public:
  CPlugin(NPP pNPInstance, uint16 mode, int16 argc, char* argn[], char* argv[]);
  virtual ~CPlugin();

  NPBool init(NPWindow* pNPWindow);
  void shut();
  NPBool isInitialized();
  
  int16 handleEvent(void* event);

  void showVersion();
  void clear();
  void getVersion(char* *aVersion);

  NPObject *GetScriptableObject();

	void HandleSetFocus();
	void HandleKillFocus();
	void HandleSize(int width, int height);
	void HandleTimer( int nIDEvent );

	virtual void SetStatusText(const char *txt);
	virtual void WriteToConsole(char *txt);
	virtual void PostAppMsgCall( sAnmThreadedCallback *pTCB );


	virtual void ComputeBaseUrl(char *pBaseUrl, int maxCharCount);
	virtual void LinkToUrl(const char *url, const char *target);
	virtual void AdviseViewpointBound(class CAnmViewpoint *pViewpoint);
	virtual void AdviseNavigationModeChange(int navmode);
	virtual void AdviseCameraChange(struct sAnmCameraChange *pCameraChange);
	virtual void AdviseUndoMoveChange(bool canundo, bool canredo);

	void PurgeAppMsgCallFuncList( );


	void Lock();
	void Unlock();
	// Thread safety
	void LockStatus()
	{
		m_criticalSectionStatus.Lock();
	}

	void UnlockStatus()
	{
		m_criticalSectionStatus.Unlock();
	}

	void LockConsole()
	{
		m_criticalSectionConsole.Lock();
	}

	void UnlockConsole()
	{
		m_criticalSectionConsole.Unlock();
	}

	void LockAppMsgCallFuncList()
	{
		m_criticalSectionAppMsgCallFunc.Lock();
	}

	void UnlockAppMsgCallFuncList()
	{
		m_criticalSectionAppMsgCallFunc.Unlock();
	}

	// Accessors
  class cAnimaApp *GetApp()
  {
	  return m_app;
  }

  NPP GetNPInstance()
  {
	  return m_pNPInstance;
  }

  bool IsInitialized()
  {
	  return m_bInitialized;
  }

  bool AppCreateFailed()
  {
	  return m_appcreatefailed;
  }

  	virtual void ShowConsole();
	virtual void ShowUserGuide();

	class CDashboard1 *GetDashboard()
	{
		return m_dashboard;
	}

	class CAnmView *GetView()
	{
		return m_view;
	}

	class CAnmConsole *GetConsole()
	{
		return m_console;
	}

#ifdef XP_WIN
  HWND GetHWnd()
  {
	  return m_hWnd;
  } 

#endif
};

class FluxPluginObject : public ScriptablePluginObject
{
protected :

	static bool m_classInitialized;
	static NPIdentifier m_methodgetBrowser;

	class CPlugin *m_plugin;
	class CNPX3DExternalBrowser *m_browser;

	void initClass();

	CNPX3DExternalBrowser *getBrowser();

public:
  FluxPluginObject(NPP npp)
    : ScriptablePluginObject(npp)
  {
	  m_browser = NULL;
	  initClass();
  }

  virtual ~FluxPluginObject();

  void SetPlugin(class CPlugin *pPlugin)
  {
	  m_plugin = pPlugin;
  }

	void NotifyBrowserDestroyed()
	{
		// Don't try to release the thing twice
		m_browser = NULL;
	}

  virtual bool HasMethod(NPIdentifier name);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name, NPVariant *result);
  virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
};


#endif // __PLUGIN_H__
