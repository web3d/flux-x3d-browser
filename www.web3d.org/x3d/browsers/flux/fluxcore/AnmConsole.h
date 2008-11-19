/********************************************************************************
 * Flux
 *
 * File: AnmConsole.h
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

#if !defined(AFX_ANMCONSOLE_H__8D56E374_E73B_4CDE_814F_2DD3FBD3A55C__INCLUDED_)
#define AFX_ANMCONSOLE_H__8D56E374_E73B_4CDE_814F_2DD3FBD3A55C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnmConsole.h : header file
//
#include "anmcriticalsection.h"

/////////////////////////////////////////////////////////////////////////////
// CAnmConsole dialog

class EXPORT CAnmConsole : public CDialog
{
// Construction
public:
	CAnmConsole(CWnd* pParent = NULL);   // standard constructor

	void Append( char * buf );
	void Update( );

	// Thread safety
	void Lock()
	{
		m_criticalSection.Lock();
	}

	void Unlock()
	{
		m_criticalSection.Unlock();
	}

// Dialog Data
	//{{AFX_DATA(CAnmConsole)
	enum { IDD = IDD_CONSOLE };
	CEdit	m_TextWidget;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnmConsole)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	CString		m_consoleText;
	CStringList	m_AppendedText;
	CAnmCriticalSection					 m_criticalSection;
	bool		m_bDirty;
	bool		m_bShowing;
	// Generated message map functions
	//{{AFX_MSG(CAnmConsole)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANMCONSOLE_H__8D56E374_E73B_4CDE_814F_2DD3FBD3A55C__INCLUDED_)
