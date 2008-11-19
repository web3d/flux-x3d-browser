/********************************************************************************
 * Flux
 *
 * File: anmplatform.cpp
 * Description: Platform class - abstracts graphics services
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

#include "stdafx.h"
#include "anmplatform.h"

CAnmPlatform::CAnmPlatform(cApplication *pApp, eAnmPlatformType platformtype, int majorversion, int minorversion)
{
	m_app = pApp;
	m_hLibraryModule = NULL;
	m_createDeviceProc = NULL;
	m_createMovieTextureBlitterProc = NULL;

	const char *libraryname = NULL;

	majorversion = 8; // just for right now

	switch (platformtype)
	{
		case eAnmDirectX :
			if (majorversion == 9)
			{
				libraryname = ANMDX9LIBRARYNAME;
			}
			else if (majorversion == 8)
			{
				libraryname = ANMDX8LIBRARYNAME;
			}
			else if (majorversion == 7)
			{
				libraryname = ANMDX7LIBRARYNAME;
			}
			break;

		case eAnmOpenGL :
		default :
			
			assert(FALSE);

			break;
	}

	assert(libraryname);

	char modpath[ANMMAXSTRBUF];

	GetModuleFileName(AfxGetInstanceHandle(), modpath, ANMMAXSTRBUF - strlen(libraryname) - 2);

	char *dir = strrchr(modpath, '\\');

	if (dir == NULL)
		return;

	strcpy(dir + 1, libraryname);

	m_hLibraryModule = LoadLibrary(modpath);
	if (m_hLibraryModule != NULL)
	{
		LoadEntryPoints();
	}
}

CAnmPlatform::~CAnmPlatform()
{
}

void CAnmPlatform::LoadEntryPoints()
{
	m_createDeviceProc = (ANMCREATEDEVICEPROC) GetProcAddress(m_hLibraryModule, ANMPLATFORM_CREATEDEVICEPROCNAME);

	m_createGraphicsProc = (ANMCREATEGRAPHICSPROC) GetProcAddress(m_hLibraryModule, ANMPLATFORM_CREATEGRAPHICSPROCNAME);

	m_createInputProc = (ANMCREATEINPUTPROC) GetProcAddress(m_hLibraryModule, ANMPLATFORM_CREATEINPUTPROCNAME);

	m_createMovieTextureBlitterProc = (ANMCREATEMOVIETEXTUREBLITTERPROC) 
		GetProcAddress(m_hLibraryModule, ANMPLATFORM_CREATEDMOVIETEXTUREBLITTERPROCNAME);

}

class CAnmDevice *CAnmPlatform::CreateDevice(HWND hWnd, class cApplication *pApp)
{
	assert(m_createDeviceProc);

	return (*m_createDeviceProc)(hWnd, pApp);
}

class CAnmGraphics *CAnmPlatform::CreateGraphics(class cApplication *pApp)
{
	assert(m_createGraphicsProc);

	return (*m_createGraphicsProc)(pApp);
}

class cInputLayer *CAnmPlatform::CreateInput(class cApplication *pApp, HINSTANCE hInst, HWND hWnd, bool bExclusive, 
		bool bUseKeyboard, bool bUseMouse)
{
	assert(m_createInputProc);

	return (*m_createInputProc)(pApp, hInst, hWnd, bExclusive, bUseKeyboard, bUseMouse);
}

class CAnmMovieTextureBlitter *CAnmPlatform::CreateMovieTextureBlitter(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface,
														 LONG width, LONG height, LONG pitch, LONG byteDepth)
{
	assert(m_createMovieTextureBlitterProc);

	return (*m_createMovieTextureBlitterProc)(pDevice, pTextureSurface, width, height, pitch, byteDepth);
}

