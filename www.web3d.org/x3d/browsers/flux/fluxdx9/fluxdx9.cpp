/********************************************************************************
 * Flux
 *
 * File: fluxdx9.cpp
 * Description: Defines the initialization routines for the DLL
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

// fluxdx9.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "fluxdx9.h"
#include "..\windowing\anmplatform.h"
#include "anmd3ddevice9.h"
#include "anmd3dgraphics9.h"
#include "anmdinput8.h"
#include "anmdshowmovietextureblitter9.h"

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
// CFluxdx9App

BEGIN_MESSAGE_MAP(CFluxdx9App, CWinApp)
	//{{AFX_MSG_MAP(CFluxdx9App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFluxdx9App construction

CFluxdx9App::CFluxdx9App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFluxdx9App object

CFluxdx9App theApp;

extern "C" EXPORT CAnmDevice *CreateDevice(HWND hWnd, cApplication *pApp)
{
	return new CAnmD3DDevice9(hWnd, pApp);
}

extern "C" EXPORT CAnmGraphics *CreateGraphics(class cApplication *pApp)
{
	return new CAnmD3DGraphics9(pApp);
}

extern "C" EXPORT cInputLayer *CreateInput(class cApplication *pApp, HINSTANCE hInst, HWND hWnd, bool bExclusive, 
		bool bUseKeyboard, bool bUseMouse)
{
	return new CAnmDInput8(
		pApp,
		hInst, 
		hWnd, 
		bExclusive, 
		bUseKeyboard, 
		bUseMouse );
}

extern "C" EXPORT CAnmMovieTextureBlitter *CreateMovieTextureBlitter(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface,
														 LONG width, LONG height, LONG pitch, LONG byteDepth)
{
	return new CAnmDShowMovieTextureBlitter9(pDevice, pTextureSurface, width, height, pitch, byteDepth);
}
