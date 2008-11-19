/********************************************************************************
 * Flux
 *
 * File: AnimaPpg.h
 * Description: Declaration of the CAnimaPropPage property page class
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

#if !defined(AFX_ANIMAPPG_H__492C6F01_681F_44F6_A517_5B2A4109109F__INCLUDED_)
#define AFX_ANIMAPPG_H__492C6F01_681F_44F6_A517_5B2A4109109F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// AnimaPpg.h : Declaration of the CAnimaPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CAnimaPropPage : See AnimaPpg.cpp.cpp for implementation.

class CAnimaPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAnimaPropPage)
	DECLARE_OLECREATE_EX(CAnimaPropPage)

// Constructor
public:
	CAnimaPropPage();

// Dialog Data
	//{{AFX_DATA(CAnimaPropPage)
	enum { IDD = IDD_PROPPAGE_ANIMA };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CAnimaPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMAPPG_H__492C6F01_681F_44F6_A517_5B2A4109109F__INCLUDED)
