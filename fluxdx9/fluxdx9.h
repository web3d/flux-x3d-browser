// fluxdx9.h : main header file for the FLUXDX9 DLL
//

#if !defined(AFX_FLUXDX9_H__6D9A7D31_5170_42DE_A771_1DECDC1461A1__INCLUDED_)
#define AFX_FLUXDX9_H__6D9A7D31_5170_42DE_A771_1DECDC1461A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFluxdx9App
// See fluxdx9.cpp for the implementation of this class
//

class CFluxdx9App : public CWinApp
{
public:
	CFluxdx9App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFluxdx9App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFluxdx9App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLUXDX9_H__6D9A7D31_5170_42DE_A771_1DECDC1461A1__INCLUDED_)
