/********************************************************************************
 * Flux
 *
 *
 * File:			Anima.cpp
 * Description:	Flux OCX DLL file
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
	
// Anima.cpp : Implementation of CAnimaApp and DLL registration.

#include "stdafx.h"
#include "Anima.h"
#include "comcat.h"
#include "objsafe.h"
#include "AnimaCtl.h"
#include <initguid.h>
#include "Anima_i.c"
#include "X3DNode.h"
#include "X3DField.h"
#include "X3DBrowser.h"
#include "X3DSFBool.h"
#include "X3DMField.h"
#include "X3DMFColor.h"
#include "X3DMFNode.h"
#include "X3DSFTime.h"
#include "X3DEventOutObserver.h"
#include "X3DSFColor.h"
#include "X3DSFFloat.h"
#include "X3DSFInt32.h"
#include "X3DSFNode.h"
#include "X3DSFRotation.h"
#include "X3DSFString.h"
#include "X3DSFVec2f.h"
#include "X3DSFVec3f.h"
#include "X3DMFFloat.h"
#include "X3DMFInt32.h"
#include "X3DMFRotation.h"
#include "X3DMFString.h"
#include "X3DMFVec2f.h"
#include "X3DMFVec3f.h"
#include "X3DSFDouble.h"
#include "FluxPlayer.h"
#include "X3DExecutionContext.h"
#include "X3DScene.h"
#include "X3DBrowserListener.h"
#include "X3DExternalBrowser.h"
#include "X3DFieldEventListener.h"
#include "X3DRoute.h"
#include "X3DNodeArray.h"
#include "X3DFieldDefinition.h"
#include "X3DFieldDefinitionArray.h"
#include "X3DProtoDeclaration.h"
#include "X3DProtoDeclarationArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAnimaApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xcb2426cf, 0x7360, 0x42ca, { 0xae, 0xf2, 0x6f, 0xbd, 0x9f, 0x95, 0x9e, 0xd4 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

IMPLEMENT_DYNAMIC(CAnimaApp, COleControlModule)

////////////////////////////////////////////////////////////////////////////
// CAnimaApp::InitInstance - DLL initialization

BOOL CAnimaApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();
	if (!InitATL())
		return FALSE;

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

int CAnimaApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	cApplication::UnloadSharedObjects();

	_Module.Term();


	return COleControlModule::ExitInstance();
}

HRESULT CreateComponentCategory(CATID catid, WCHAR *catDescription)
{

    ICatRegister *pcr = NULL ;
    HRESULT hr = S_OK ;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
            NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (FAILED(hr))
        return hr;

    // Make sure the HKCR\Component Categories\{..catid...}
    // key is registered.
    CATEGORYINFO catinfo;
    catinfo.catid = catid;
    catinfo.lcid = 0x0409 ; // english

    // Make sure the provided description is not too long.
    // Only copy the first 127 characters if it is.
    int len = wcslen(catDescription);
    if (len>127)
        len = 127;
    wcsncpy(catinfo.szDescription, catDescription, len);
        // Make sure the description is null terminated.
        catinfo.szDescription[len] = '\0';

    hr = pcr->RegisterCategories(1, &catinfo);
        pcr->Release();

    return hr;
}

HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
// Register your component categories information.
    ICatRegister *pcr = NULL ;
    HRESULT hr = S_OK ;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
                NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (SUCCEEDED(hr))
    {
       // Register this category as being "implemented" by the class.
       CATID rgcatid[1] ;
       rgcatid[0] = catid;
       hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
    }

    if (pcr != NULL)
        pcr->Release();
		
    return hr;
}

HRESULT RegisterCategories(void)
{
    // Mark the control as safe for initializing.

    HRESULT hr = CreateComponentCategory(CATID_SafeForInitializing, L"Controls \
        safely initializable from persistent data!");
    if (FAILED(hr))
	{
	::MessageBox(NULL, "Failed in CreateComponentCategory - Safe Initializing", "Flux Player", 0);

        return hr;
	}

    hr = RegisterCLSIDInCategory(CAnimaCtrl::guid, CATID_SafeForInitializing);
    if (FAILED(hr))
	{
	::MessageBox(NULL, "Failed in RegisterCLSIDInCategory - Safe Initializing", "Flux Player", 0);

        return hr;
	}

    // Mark the control as safe for scripting.

    hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls \
        safely scriptable!");
    if (FAILED(hr))
	{
	::MessageBox(NULL, "Failed in CreateComponentCategory - Safe Scripting", "Flux Player", 0);

        return hr;
	}

    hr = RegisterCLSIDInCategory(CAnimaCtrl::guid, CATID_SafeForScripting);
    if (FAILED(hr))
	{
	::MessageBox(NULL, "Failed in RegisterCLSIDInCategory - Safe Scripting", "Flux Player", 0);

        return hr;
	}
    

	return S_OK;
}

HRESULT RegisterMimeTypesAndExtensions(void)
{
    HKEY        hkey = NULL;
    HKEY        hkey1 = NULL;
    BOOL        fErr = FALSE;
    char        szSubKey[513];
	LPOLESTR	pProgID = NULL;

    // file extension for new mime type
#ifndef REGISTER_WRL
    const char* pszMTExtensions[] = { ".x3d", ".x3d.gz", ".x3dz",
									  ".x3dv", ".x3dv.gz", ".x3dvz", ".fx3d", ".fx3"};
    const char* pszMTContentTypesForExtensions[] = { "model/x3d", "model/x3d", "model/x3d",
									  "model/x3d-vrml", "model/x3d-vrml", "model/x3d-vrml", "model/fx3d", "model/fx3d"};

	const int nFileExtensions = sizeof(pszMTExtensions) / sizeof(const char *);

    // text for new mime content types
    const char* pszMTContentTypes[] = { "model/x3d", "model/x3d+xml", "model/x3d-vrml", 
		"x-world/x-x3d", "x-world/x-x3d+xml", "x-world/x-x3d-vrml", "model/fx3d", "x-world/x-fx3d"};
	const int nContentTypes = sizeof(pszMTContentTypes) / sizeof(const char *);

	const char *pszMTCombinedContentTypes =
		"model/x3d,model/x3d+xml,model/x3d-vrml,x-world/x-x3d,x-world/x-x3d+xml,x-world/x-x3d-vrml,model/fx3d,x-world/x-fx3d";

	const char *pszMTPrimaryExtensions[] = 
	{".x3d", ".x3d", ".x3dv", ".x3d", ".x3d", ".x3dv", ".fx3d", ".fx3" };

    const char* pszMTSubKeys[] = {
		"MIME\\DataBase\\Content Type\\model/x3d",
		"MIME\\DataBase\\Content Type\\model/x3d+xml",
		"MIME\\DataBase\\Content Type\\model/x3d-vrml",
		"MIME\\DataBase\\Content Type\\x-world/x-x3d", 
		"MIME\\DataBase\\Content Type\\x-world/x-x3d+xml", 
		"MIME\\DataBase\\Content Type\\x-world/x-x3d-vrml", 
		"MIME\\DataBase\\Content Type\\model/fx3d",
		"MIME\\DataBase\\Content Type\\x-world/x-fx3d", 
	};
#else // 0
    const char* pszMTExtensions[] = { ".x3d", ".x3d.gz", ".x3dz",
									  ".x3dv", ".x3dv.gz", ".x3dvz",
									  ".wrl", ".wrl.gz", ".fx3d", ".fx3"};

    const char* pszMTContentTypesForExtensions[] = { "model/x3d", "model/x3d", "model/x3d",
									  "model/x3d-vrml", "model/x3d-vrml", "model/x3d-vrml",
									  "model/vrml", "model/vrml", "model/fx3d", "model/fx3d"};

	const int nFileExtensions = sizeof(pszMTExtensions) / sizeof(const char *);

    // text for new mime content types
    const char* pszMTContentTypes[] = { "model/x3d", "model/x3d+xml", "model/x3d-vrml", 
		"x-world/x-x3d", "x-world/x-x3d+xml", "x-world/x-x3d-vrml", 
		"model/vrml", "x-world/x-vrml", "model/fx3d", "x-world/x-fx3d"};

	const int nContentTypes = sizeof(pszMTContentTypes) / sizeof(const char *);

	const char *pszMTCombinedContentTypes =
		"model/x3d,model/x3d+xml,model/x3d-vrml,x-world/x-x3d,x-world/x-x3d+xml,x-world/x-x3d-vrml,model/vrml,x-world/x-vrml,model/fx3d,x-world/x-fx3d";

	const char *pszMTPrimaryExtensions[] = 
	{".x3d", ".x3d", ".x3dv", ".x3d", ".x3d", ".x3dv", ".wrl", ".wrl", ".fx3d", ".fx3d"};

    const char* pszMTSubKeys[] = {
		"MIME\\DataBase\\Content Type\\model/x3d",
		"MIME\\DataBase\\Content Type\\model/x3d+xml",
		"MIME\\DataBase\\Content Type\\model/x3d-vrml",
		"MIME\\DataBase\\Content Type\\x-world/x-x3d", 
		"MIME\\DataBase\\Content Type\\x-world/x-x3d+xml", 
		"MIME\\DataBase\\Content Type\\x-world/x-x3d-vrml", 
		"MIME\\DataBase\\Content Type\\model/vrml",
		"MIME\\DataBase\\Content Type\\x-world/x-vrml",
		"MIME\\DataBase\\Content Type\\model/fx3d",
		"MIME\\DataBase\\Content Type\\x-world/x-fx3d", 
	};
#endif // 0


    // extension named value
    const char* pszMTExtVal = "Extension";
    // clsid
    const char* pszMTCLSID = "{918B202D-8E8F-4649-A70B-E9B178FEDC58}";
    // clsid named value name
    const char* pszMTCLSIDVal = "CLSID";
    // progid; N.B.: s/b calculated
    const char* pszMTProgID= "Anima.AnimaCtrl.1";
    // progid named value name
    const char* pszMTProgIDVal = "";
    // content type named value name
    const char* pszMTContentVal = "Content Type";
    // EnableFullPage key
    const char* pszMTFullPage = "EnableFullPage";

	for (int i = 0; i < nContentTypes; i++) 
    {
        // create new mime type key for our new mimetype.  Only necessary for new mime types
        if ( ERROR_SUCCESS != RegCreateKey(HKEY_CLASSES_ROOT, pszMTSubKeys[i], &hkey) )
		{
			fErr = TRUE;
            break;
		}

        // add extension value to that mime type key to associate .wrl files with the 
        // model/vrml mime type
        if ( ERROR_SUCCESS != RegSetValueEx(hkey, pszMTExtVal, 0, REG_SZ, 
            (const BYTE *)pszMTPrimaryExtensions[i], strlen(pszMTPrimaryExtensions[i])) )
		{
			fErr = TRUE;
            break;
		}

        // Add class id to associate this object with the mime type
        if ( ERROR_SUCCESS != RegSetValueEx(hkey, pszMTCLSIDVal, 0, REG_SZ,
            (const BYTE *)pszMTCLSID, strlen(pszMTCLSID)) )
		{
			fErr = TRUE;
            break;
		}

        RegCloseKey(hkey);
		hkey = NULL;
	}

	if (!fErr)
	{
#define ANMFLUXPLAYERNAME "fluxplayer.exe"
		
		char playerexebuf[ANMMAXSTRBUF];

		playerexebuf[0]='\"';
		GetModuleFileName(AfxGetInstanceHandle(), playerexebuf+1, ANMMAXSTRBUF - strlen(ANMFLUXPLAYERNAME) - 10);

		char *dir = strrchr(playerexebuf, '\\');

		bool gotplayerdir = true;
		
		if (dir != NULL)
		{
			strcpy(dir + 1, ANMFLUXPLAYERNAME);
			strcat(playerexebuf,"\" \"%1\"");
		}
		else
			gotplayerdir = false;


		for (i = 0; i < nFileExtensions; i++)
		{
			// Register .mtp as a file extension this is only necessary for new file extensions, addimg
			// a new player for .avi files for instance would not require this
			if ( ERROR_SUCCESS != RegCreateKey(HKEY_CLASSES_ROOT, pszMTExtensions[i], &hkey) )
			{
				fErr = TRUE;
				break;
			}

			// Add content type to associate this extension with the content type.  This is required
			// and is used when the mime type is unknown and IE looks up associations by extension
			if ( ERROR_SUCCESS != RegSetValueEx(hkey, pszMTContentVal, 0, REG_SZ,
				(const BYTE *)pszMTContentTypesForExtensions[i], strlen(pszMTContentTypesForExtensions[i])) )
			{
				fErr = TRUE;
				break;
			}

			// Add prog ID to associate this extension with the prog ID.  This is required
			// and is used when the mime type is unknown and IE looks up associations by extension
			if ( ERROR_SUCCESS != RegSetValueEx(hkey, pszMTProgIDVal, 0, REG_SZ,
				(const BYTE *)pszMTProgID, strlen(pszMTProgID)) )
			{
				fErr = TRUE;
				break;
			}

			RegCloseKey(hkey);
			hkey = NULL;

			// Open the key under the control's clsid HKEY_CLASSES_ROOT\CLSID\<CLSID>
			wsprintf(szSubKey, "%s\\%s", pszMTCLSIDVal, pszMTCLSID);
			if ( ERROR_SUCCESS != RegOpenKey(HKEY_CLASSES_ROOT, szSubKey, &hkey) )
			{
				fErr = TRUE;
				break;
			}

			// Create the EnableFullPage and extension key under this so that we can display files
			// with the extension full frame in the browser
			wsprintf(szSubKey, "%s\\%s", pszMTFullPage, pszMTExtensions[i]);
			if ( ERROR_SUCCESS != RegCreateKey(hkey, szSubKey, &hkey1) )
			{
				fErr = TRUE;
				break;
			}

			RegCloseKey(hkey);
			RegCloseKey(hkey1);

#ifdef REGISTER_SHELL_COMMAND
			
			const char* shellCmd = 
				"c:\\program files\\internet explorer\\iexplore.exe %1";

			const char *IEPathKey =
				
			"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE";
			if ( ERROR_SUCCESS == 
				RegOpenKeyEx(HKEY_LOCAL_MACHINE, IEPathKey, 0, KEY_QUERY_VALUE, &hkey) )
			{
				long cnt=512;
				char IECommand[516];
				if ( ERROR_SUCCESS == RegQueryValue(hkey, "", IECommand,
					&cnt) )
				{
					RegCloseKey(hkey);

					// Append the %1 arg
					char *cp = IECommand + strlen(IECommand);
					strcpy(cp, " %1");

					if ( ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT, pszMTProgID, &hkey) )
					{
						if ( ERROR_SUCCESS == RegCreateKey(hkey, "Shell\\Open\\Command", &hkey1) )
						{
							if ( ERROR_SUCCESS == RegSetValueEx(hkey1, "", 0, REG_SZ,
//								(const BYTE *)IECommand, strlen(IECommand)) )
								(const BYTE *)playerexebuf, strlen(playerexebuf)) )
								
							{
							}

							RegCloseKey(hkey1);
						}

						RegCloseKey(hkey);
					}
				}
			}

#endif // defined(REGISTER_SHELL_COMMAND)

			hkey = NULL;
			hkey1 = NULL;
		}
	}

    if ( hkey )
        RegCloseKey(hkey);

    if ( hkey1 )
        RegCloseKey(hkey1);

    if ( fErr )
	{
        MessageBox(0, "Cannot register player for mime type", "Flux Player", MB_OK);
		return S_FALSE;
	}
	else
		return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);
	
	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);


	HRESULT hr = RegisterCategories();
	if (FAILED(hr))
		return hr;

	hr = RegisterMimeTypesAndExtensions();
	if (FAILED(hr))
		return hr;

	return _Module.RegisterServer(TRUE);
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	_Module.UnregisterServer(TRUE); //TRUE indicates that typelib is unreg'd

	return NOERROR;
}

	
CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_CoX3DNode, CX3DNode)
OBJECT_ENTRY(CLSID_CoX3DField, CX3DField)
OBJECT_ENTRY(CLSID_CoX3DBrowser, CX3DBrowser)
OBJECT_ENTRY(CLSID_CoX3DSFBool, CX3DSFBool)
OBJECT_ENTRY(CLSID_CoX3DMField, CX3DMField)
OBJECT_ENTRY(CLSID_CoX3DMFColor, CX3DMFColor)
OBJECT_ENTRY(CLSID_CoX3DMFNode, CX3DMFNode)
OBJECT_ENTRY(CLSID_CoX3DSFTime, CX3DSFTime)
OBJECT_ENTRY(CLSID_CoX3DEventOutObserver, CX3DEventOutObserver)
OBJECT_ENTRY(CLSID_CoX3DSFColor, CX3DSFColor)
OBJECT_ENTRY(CLSID_CoX3DSFFloat, CX3DSFFloat)
OBJECT_ENTRY(CLSID_CoX3DSFInt32, CX3DSFInt32)
OBJECT_ENTRY(CLSID_CoX3DSFNode, CX3DSFNode)
OBJECT_ENTRY(CLSID_CoX3DSFRotation, CX3DSFRotation)
OBJECT_ENTRY(CLSID_CoX3DSFString, CX3DSFString)
OBJECT_ENTRY(CLSID_CoX3DSFVec2f, CX3DSFVec2f)
OBJECT_ENTRY(CLSID_CoX3DSFVec3f, CX3DSFVec3f)
OBJECT_ENTRY(CLSID_CoX3DMFFloat, CX3DMFFloat)
OBJECT_ENTRY(CLSID_CoX3DMFInt32, CX3DMFInt32)
OBJECT_ENTRY(CLSID_CoX3DMFRotation, CX3DMFRotation)
OBJECT_ENTRY(CLSID_CoX3DMFString, CX3DMFString)
OBJECT_ENTRY(CLSID_CoX3DMFVec2f, CX3DMFVec2f)
OBJECT_ENTRY(CLSID_CoX3DMFVec3f, CX3DMFVec3f)
OBJECT_ENTRY(CLSID_CoX3DSFDouble, CX3DSFDouble)
OBJECT_ENTRY(CLSID_FluxPlayer, CFluxPlayer)
OBJECT_ENTRY(CLSID_CoX3DExecutionContext, CX3DExecutionContext)
OBJECT_ENTRY(CLSID_CoX3DScene, CX3DScene)
OBJECT_ENTRY(CLSID_CoX3DBrowserListener, CX3DBrowserListener)
OBJECT_ENTRY(CLSID_CoX3DExternalBrowser, CX3DExternalBrowser)
OBJECT_ENTRY(CLSID_CoX3DFieldEventListener, CX3DFieldEventListener)
OBJECT_ENTRY(CLSID_CoX3DRoute, CX3DRoute)
OBJECT_ENTRY(CLSID_CoX3DNodeArray, CX3DNodeArray)
OBJECT_ENTRY(CLSID_CoX3DFieldDefinition, CX3DFieldDefinition)
OBJECT_ENTRY(CLSID_CoX3DFieldDefinitionArray, CX3DFieldDefinitionArray)
OBJECT_ENTRY(CLSID_CoX3DProtoDeclaration, CX3DProtoDeclaration)
OBJECT_ENTRY(CLSID_CoX3DProtoDeclarationArray, CX3DProtoDeclarationArray)
END_OBJECT_MAP()

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(AfxDllGetClassObject(rclsid, riid, ppv) == S_OK)
		return S_OK;
	return _Module.GetClassObject(rclsid, riid, ppv);
}

BOOL CAnimaApp::InitATL()
{
	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	return TRUE;

}
