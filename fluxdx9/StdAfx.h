// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C9FE5D14_B2CF_47F6_B4D6_F658A8FC3EF7__INCLUDED_)
#define AFX_STDAFX_H__C9FE5D14_B2CF_47F6_B4D6_F658A8FC3EF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <assert.h>

// XML stuff
#import <msxml3.dll> raw_interfaces_only 
using namespace MSXML2;

#include <mmsystem.h> // for timeGetTime

#include <float.h>

#include "..\animadefs.h"
#include "fluxdx9exports.h"

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

#endif // !defined(AFX_STDAFX_H__C9FE5D14_B2CF_47F6_B4D6_F658A8FC3EF7__INCLUDED_)
