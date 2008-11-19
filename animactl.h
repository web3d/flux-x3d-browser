/********************************************************************************
 * Flux
 *
 * File: AnimaCtl.h
 * Description: Declaration of the CAnimaCtrl ActiveX Control class
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

#if !defined(AFX_ANIMACTL_H__6A59887E_70FA_4168_B60D_32BC5CB3BC84__INCLUDED_)
#define AFX_ANIMACTL_H__6A59887E_70FA_4168_B60D_32BC5CB3BC84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// AnimaCtl.h : Declaration of the CAnimaCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CAnimaCtrl : See AnimaCtl.cpp for implementation.

class CAnimaCtrl : public COleControl
{
	DECLARE_DYNCREATE(CAnimaCtrl)

// Constructor
public:
	CAnimaCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimaCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL


// Implementation
protected:
	~CAnimaCtrl();

	class cAnimaApp						*m_app;
	class CAnmOCXPlugin					*m_plugin;
	class cWindow						*m_pw;
	interface X3DExternalBrowser		*m_browser;
	interface IFluxPlayer				*m_fluxplayer;
	class CAnmView						*m_view;
	class CAnmConsole					*m_console;
	class CDashboard1					*m_dashboard;
	bool								 m_appcreatefailed;

	CRITICAL_SECTION m_cs;			// to block external access
	CAnmCriticalSection					 m_criticalSectionStatus;  // just for status string
	CAnmCriticalSection					 m_criticalSectionAppMsgCallFunc;  // just for AppMsg Call Func
	CAnmCriticalSection					 m_criticalSectionConsole;


	int CreateApp();

	CString								m_StatusString;
	bool								m_bStatusPending;
	bool								m_bAppMsgCallFuncPending;

	std::list<struct sAnmThreadedCallback *>		 m_AppMsgCallFuncList;



	// Thread safety
	void LockStatus()
	{
		m_criticalSectionStatus.Lock();
	}

	void UnlockStatus()
	{
		m_criticalSectionStatus.Unlock();
	}

	void LockAppMsgCallFuncList()
	{
		m_criticalSectionAppMsgCallFunc.Lock();
	}

	void UnlockAppMsgCallFuncList()
	{
		m_criticalSectionAppMsgCallFunc.Unlock();
	}


	void LockConsole()
	{
		m_criticalSectionConsole.Lock();
	}

	void UnlockConsole()
	{
		m_criticalSectionConsole.Unlock();
	}



	DECLARE_OLECREATE_EX(CAnimaCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CAnimaCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CAnimaCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CAnimaCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CAnimaCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer( UINT nIDEvent );
	afx_msg void OnNextviewpoint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CAnimaCtrl)
	CString m_src;
	afx_msg void OnSrcChanged();
	CString m_bgcolor;
	afx_msg void OnBgcolorChanged();
	CString m_navigation;
	afx_msg void OnNavigationChanged();
	BOOL m_antialias;
	afx_msg void OnAntialiasChanged();
	CString m_primitiveQuality;
	afx_msg void OnPrimitiveQualityChanged();
	BOOL m_dashboardOn;
	afx_msg void OnDASHBOARDChanged();
	CString m_navigationMode;
	afx_msg void OnNavigationModeChanged();
	afx_msg void jumpTo(LPCTSTR url, LPCTSTR target);
	afx_msg interface X3DBrowser *getBrowser();
	afx_msg long getBrowserHandle();
	afx_msg void NextViewpoint();
	afx_msg void PreviousViewpoint();
	afx_msg LPDISPATCH getFluxPlayer();
	long m_maxTextureSize;
	afx_msg void OnMaxTextureSizeChanged();
	BOOL m_LoadScreen;
	afx_msg void OnLoadScreenChanged();
	CString m_rightMouseMenu;
	afx_msg void OnRightMouseMenuChanged();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CAnimaCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

	// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CAnimaCtrl)
	dispidSrc = 1L,
	dispidBgcolor = 2L,
	dispidNavigation = 3L,
	dispidAntialias = 4L,
	dispidPrimitiveQuality = 5L,
	dispidDASHBOARD = 6L,
	dispidNavigationMode = 7L,
	dispidJumpTo = 8L,
	dispidGetBrowser = 9L,
	dispidNextViewpoint = 10L,
	dispidPreviousViewpoint = 11L,
	dispidGetFluxPlayer = 12L,
	dispidMaxTextureSize = 13L,
	dispidLoadScreen = 14L,
	dispidRightMouseMenu = 15L,
	dispidGetBrowserHandle = 16L,
	//}}AFX_DISP_ID
	};

	void SetStatusText(char *txt);
	void WriteToConsole(char *txt);
	void PostAppMsgCall( sAnmThreadedCallback *pTCB );

	virtual void AdviseViewpointBound(class CAnmViewpoint *pViewpoint);
	virtual void AdviseNavigationModeChange(int navmode);
	virtual void AdviseCameraChange(struct sAnmCameraChange *pCameraChange);
	virtual void AdviseUndoMoveChange(bool canundo, bool canredo);

	virtual void ShowConsole();
	virtual void ShowUserGuide();


	virtual void UpdateConsole( );
	virtual void UpdateStatus( );
	virtual void PurgeAppMsgCallFuncList( );


	void Lock();
	void Unlock();

	class CDashboard1 *GetDashboard()
	{
		return m_dashboard;
	}

	class CAnmView *GetView()
	{
		return m_view;
	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMACTL_H__6A59887E_70FA_4168_B60D_32BC5CB3BC84__INCLUDED)
