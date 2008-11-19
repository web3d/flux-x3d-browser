/********************************************************************************
 * Flux
 *
 * File: AnmConsole.cpp
 * Description: Info console implementation file
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

// AnmConsole.cpp : implementation file
//

#include "stdafx.h"
#include "fluxcore.h"
#include "AnmConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnmConsole dialog


CAnmConsole::CAnmConsole(CWnd* pParent /*=NULL*/)
	: CDialog(CAnmConsole::IDD, pParent)
{
	m_bDirty = false;
	m_bShowing = false;
	//{{AFX_DATA_INIT(CAnmConsole)
	//}}AFX_DATA_INIT
}


void CAnmConsole::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnmConsole)
	DDX_Control(pDX, IDC_CONSOLETEXT, m_TextWidget);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnmConsole, CDialog)
	//{{AFX_MSG_MAP(CAnmConsole)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnmConsole message handlers


void CAnmConsole::Append( char * buf )
{

	// Just add the string to a List for speed.
	//
	Lock();
	m_AppendedText.AddTail( CString( L"\r\n" ) + CString( buf ) );
	m_bDirty = true;
	Unlock();
}

void CAnmConsole::Update( )
{
	if( m_bShowing &&
		m_bDirty &&
		::IsWindow(m_TextWidget.m_hWnd) ) {

		// First append to a tmp string, so that we only append the big string once.
		// Use raw char data for speed.  CStrings are slooooow.

		// We must set the lock while reading m_AppendedText.
		//
		Lock();

		// First time, we just get a char count.
		//
		int nChars = 3;
		for( POSITION pos=m_AppendedText.GetHeadPosition(); pos; ) {
			nChars += m_AppendedText.GetNext( pos ).GetLength();
		}

		// Create a buffer for our chars.
		//
		char* pTmpBuffer = new char[nChars];
		char* pTmp = pTmpBuffer;
		for( pos=m_AppendedText.GetHeadPosition(); pos; ) {
			CString& TmpStr = m_AppendedText.GetNext( pos );
			int len = TmpStr.GetLength();
			strncpy( pTmp, TmpStr.GetBuffer( 0 ), len );
			pTmp += len;
		}
		*pTmp = '\0';
		m_AppendedText.RemoveAll();

		Unlock();

		m_consoleText += pTmpBuffer;
		delete [] pTmpBuffer;

		m_TextWidget.SetWindowText( m_consoleText );
		int nline = m_TextWidget.GetLineCount();
		if( nline > 3 ) {
			m_TextWidget.LineScroll( nline - 1 );
		}

		m_bDirty = false;
	}
}

void CAnmConsole::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	if( bShow && !m_bShowing ) {
		m_bShowing = bShow;
		Update( );
	}
	m_bShowing = bShow;
}

void CAnmConsole::OnClose() 
{
	m_bShowing = false;
	CDialog::OnClose();
}
