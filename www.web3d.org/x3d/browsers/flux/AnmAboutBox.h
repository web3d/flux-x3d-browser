/********************************************************************************
 * Flux
 *
 * File: AnmAboutBox.h
 * Description: About box dialog
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

#if !defined(AFX_ANMABOUTBOX_H__12247906_2D9C_4113_AE0D_09BB9701EB87__INCLUDED_)
#define AFX_ANMABOUTBOX_H__12247906_2D9C_4113_AE0D_09BB9701EB87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnmAboutBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnmAboutBox dialog

class CAnmAboutBox : public CDialog
{
// Construction
public:
	CAnmAboutBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnmAboutBox)
	enum { IDD = IDD_ABOUTBOX_ANIMA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnmAboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnmAboutBox)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANMABOUTBOX_H__12247906_2D9C_4113_AE0D_09BB9701EB87__INCLUDED_)
