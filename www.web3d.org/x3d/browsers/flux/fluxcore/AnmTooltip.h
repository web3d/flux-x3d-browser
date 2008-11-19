/********************************************************************************
 * Flux
 *
 * File: AnmTooltip.h
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

#if !defined(AFX_ANMTOOLTIP_H__1034C20A_C95F_4E9E_9B46_071E02FB60D9__INCLUDED_)
#define AFX_ANMTOOLTIP_H__1034C20A_C95F_4E9E_9B46_071E02FB60D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnmTooltip.h : header file
//


//////////////////
// Get NONCLIENTMETRICS info: ctor calls SystemParametersInfo.
//
class CNonClientMetrics : public NONCLIENTMETRICS {
public:
	CNonClientMetrics() {
		cbSize = sizeof(NONCLIENTMETRICS);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS,0,this,0);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CAnmTooltip window

class EXPORT CAnmTooltip : public CWnd
{
// Construction
public:
	CSize m_szMargins;		// extra space around text: change if you like
	enum {ANMTOOLTIP_JUSTIFYLEFT=0, ANMTOOLTIP_JUSTIFYRIGHT};
	CAnmTooltip();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnmTooltip)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAnmTooltip();
	BOOL Create(CPoint pt, CWnd* pParentWnd, UINT nStyle=0, UINT nID=0);
	void ShowDelayed(UINT msec);
	void Cancel();


	// Generated message map functions
protected:

	CFont	m_font;				// font to use (same as tooltips)
	UINT	m_nStyle;			// style (see enum below)

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnSetText(WPARAM wp, LPARAM lp);

	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


	//{{AFX_MSG(CAnmTooltip)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DECLARE_DYNAMIC(CAnmTooltip);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANMTOOLTIP_H__1034C20A_C95F_4E9E_9B46_071E02FB60D9__INCLUDED_)
