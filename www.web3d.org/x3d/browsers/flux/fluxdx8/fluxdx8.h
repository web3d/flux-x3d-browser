/********************************************************************************
 * Flux
 *
 * File: fluxdx8.h
 * Description: 
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

// fluxdx8.h : main header file for the FLUXDX8 DLL
//

#if !defined(AFX_FLUXDX8_H__4666832A_2730_44F3_8201_FB9ECCF667DD__INCLUDED_)
#define AFX_FLUXDX8_H__4666832A_2730_44F3_8201_FB9ECCF667DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFluxdx8App
// See fluxdx8.cpp for the implementation of this class
//

class CFluxdx8App : public CWinApp
{
public:
	CFluxdx8App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFluxdx8App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFluxdx8App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLUXDX8_H__4666832A_2730_44F3_8201_FB9ECCF667DD__INCLUDED_)
