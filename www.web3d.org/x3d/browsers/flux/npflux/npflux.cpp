/********************************************************************************
 * Flux
 *
 * File: npflux.cpp
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

// npflux.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "npflux.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
   
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CNpfluxApp

BEGIN_MESSAGE_MAP(CNpfluxApp, CWinApp)
	//{{AFX_MSG_MAP(CNpfluxApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CNpfluxApp, COleControlModule)

/////////////////////////////////////////////////////////////////////////////
// CNpfluxApp construction

CNpfluxApp::CNpfluxApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNpfluxApp object

CNpfluxApp theApp;

 
BOOL CNpfluxApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();
	if (bInit)
	{
		// TODO: Add your own module initialization code here.

		// enables contained web browser control
		AfxEnableControlContainer();
	
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CAnimaApp::ExitInstance - DLL termination

int CNpfluxApp::ExitInstance()
{
	// TODO: Add your own module termination code here.


	return COleControlModule::ExitInstance();
}