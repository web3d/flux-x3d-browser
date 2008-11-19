/********************************************************************************
 * Flux
 *
 * File: fluxcore.h
 * Description: main header file for the FLUXCORE DLL
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

// fluxcore.h : main header file for the FLUXCORE DLL
//

#if !defined(AFX_FLUXCORE_H__19B0465B_BD24_4839_835D_FB74A9147F9C__INCLUDED_)
#define AFX_FLUXCORE_H__19B0465B_BD24_4839_835D_FB74A9147F9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFluxcoreApp
// See fluxcore.cpp for the implementation of this class
//

class CFluxcoreApp : public CWinApp
{
public:
	CFluxcoreApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFluxcoreApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFluxcoreApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLUXCORE_H__19B0465B_BD24_4839_835D_FB74A9147F9C__INCLUDED_)
