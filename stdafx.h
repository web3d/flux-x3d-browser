/********************************************************************************
 * Flux
 *
 * File: stdafx.h
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

#if !defined(AFX_STDAFX_H__7F80D174_4EDA_4E41_A8B0_5C5690BCE341__INCLUDED_)
#define AFX_STDAFX_H__7F80D174_4EDA_4E41_A8B0_5C5690BCE341__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Comon Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Delete the two includes below if you do not wish to use the MFC
//  database classes
#include <afxdb.h>			// MFC database classes
#include <afxdao.h>			// MFC DAO database classes

#include <assert.h>

// XML stuff
#import <msxml3.dll> raw_interfaces_only 
using namespace MSXML2;

#include <mmsystem.h> // for timeGetTime

#include <float.h>

#include "..\animadefs.h"

#include "..\animaerrors.h"

#include "..\math3d\point3.h"
#include "..\math3d\point3D.h"
#include "..\math3d\matrix4.h"
#include "..\math3d\matrix4D.h"
#include "..\math3d\mathD3D.h"
#include "..\math3d\color3.h"
#include "..\math3d\color4.h"
#include "..\math3D\tri.h"

#include "..\core\anmobject.h"
#include "..\core\anmstring.h"
#include "..\scene\anmnodedefs.h"

#include "..\time\anmtime.h"
#include "..\graphics\anmgraphics.h"

#include "..\windowing\Application.h"
#include "..\windowing\Window.h"




	#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7F80D174_4EDA_4E41_A8B0_5C5690BCE341__INCLUDED_)
