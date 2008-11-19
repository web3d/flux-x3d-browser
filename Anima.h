/********************************************************************************
 * Flux
 *
 * File: Anima.h
 * Description: main header file for ANIMA.DLL
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

#if !defined(AFX_ANIMA_H__ED492CDF_FEA9_46F1_8190_D2763606B0D3__INCLUDED_)
#define AFX_ANIMA_H__ED492CDF_FEA9_46F1_8190_D2763606B0D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Anima.h : main header file for ANIMA.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols
#include "Anima_i.h"

/////////////////////////////////////////////////////////////////////////////
// CAnimaApp : See Anima.cpp for implementation.

class CAnimaApp : public COleControlModule
{
	DECLARE_DYNAMIC(CAnimaApp)
protected :
	class cApplication *m_app;

public:
	BOOL InitInstance();
	int ExitInstance();

//	void SetApp(class cApplication *pApp) { m_app = pApp; }
//	class cApplication *GetApp() { return m_app; }
private:
	BOOL InitATL();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMA_H__ED492CDF_FEA9_46F1_8190_D2763606B0D3__INCLUDED)
