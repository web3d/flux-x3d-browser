// npflux.h : main header file for the NPFLUX DLL
//

#if !defined(AFX_NPFLUX_H__62052316_F767_4371_9FB1_F5D3B3D15884__INCLUDED_)
#define AFX_NPFLUX_H__62052316_F767_4371_9FB1_F5D3B3D15884__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNpfluxApp
// See npflux.cpp for the implementation of this class
//

class CNpfluxApp : public COleControlModule
{
	DECLARE_DYNAMIC(CNpfluxApp)

public:
	CNpfluxApp();
	BOOL InitInstance();
	int ExitInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNpfluxApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNpfluxApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NPFLUX_H__62052316_F767_4371_9FB1_F5D3B3D15884__INCLUDED_)
