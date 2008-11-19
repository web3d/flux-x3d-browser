/********************************************************************************
 * Flux
 *
 * File: anmplatform.h
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

#ifndef _anmplatform_h
#define _anmplatform_h

#define ANMDX7LIBRARYNAME "fluxdx7.dll"
#define ANMDX8LIBRARYNAME "fluxdx8.dll"
#define ANMDX9LIBRARYNAME "fluxdx9.dll"

typedef class CAnmDevice * (*ANMCREATEDEVICEPROC) (HWND hWnd, class cApplication *pApp);

typedef class CAnmGraphics * (*ANMCREATEGRAPHICSPROC) (class cApplication *pApp);

typedef class cInputLayer * (*ANMCREATEINPUTPROC) (class cApplication *pApp, HINSTANCE hInst, HWND hWnd, bool bExclusive, 
		bool bUseKeyboard, bool bUseMouse);

typedef class CAnmMovieTextureBlitter * (*ANMCREATEMOVIETEXTUREBLITTERPROC) (class CAnmDevice *pDevice, 
																	   CAnmGraphics::Texture *pTextureSurface, LONG width, LONG height, LONG pitch, LONG byteDepth);


#define ANMPLATFORM_CREATEDEVICEPROCNAME				"CreateDevice"
#define ANMPLATFORM_CREATEINPUTPROCNAME					"CreateInput"
#define ANMPLATFORM_CREATEGRAPHICSPROCNAME				"CreateGraphics"
#define ANMPLATFORM_CREATEDMOVIETEXTUREBLITTERPROCNAME	"CreateMovieTextureBlitter"

class CAnmPlatform
{
protected:

	cApplication					*m_app;
	HMODULE							 m_hLibraryModule;
	ANMCREATEDEVICEPROC				 m_createDeviceProc;
	ANMCREATEGRAPHICSPROC			 m_createGraphicsProc;
	ANMCREATEINPUTPROC				 m_createInputProc;
	ANMCREATEMOVIETEXTUREBLITTERPROC m_createMovieTextureBlitterProc;

	void LoadEntryPoints();

public:

	CAnmPlatform(cApplication *pApp, eAnmPlatformType platformtype, int majorversion, int minorversion);
	virtual ~CAnmPlatform();

	virtual class CAnmDevice *CreateDevice(HWND hWnd, class cApplication *pApp);
	virtual class CAnmGraphics *CreateGraphics(class cApplication *pApp);
	virtual class cInputLayer *CreateInput(class cApplication *pApp, HINSTANCE hInst, HWND hWnd, bool bExclusive, 
		bool bUseKeyboard, bool bUseMouse);
	virtual class CAnmMovieTextureBlitter *CreateMovieTextureBlitter(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface,
														 LONG width, LONG height, LONG pitch, LONG byteDepth);

};

#endif // _anmplatform_h
