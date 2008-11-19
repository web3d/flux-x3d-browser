/********************************************************************************
 * Flux
 *
 * File: anmdevice.cpp
 * Description: Base device class
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
#include "anmdevice.h"
#include "anmviewport.h"
#include "application.h"
#include "anmplatform.h"


CAnmDevice::CAnmDevice(HWND hWnd, cApplication *pApp)
{
	m_hWnd = hWnd;
	m_app = pApp;

	m_viewport		 = NULL;

	m_antialias = true;
	m_wireframe = false;
	m_maxTextureSize = ANMDEFAULT_MAX_TEXTURESIZE;

	m_zbufferbytes = 3;		// default to 24-bit

	m_maxActiveLights = ANMNUMDEFAULTACTIVELIGHTS;	// X3D default
	m_activeLights.reserve(ANMNUMDEFAULTACTIVELIGHTS);
	m_numActiveLights = 0;

	m_canMipMap = true;

	m_vertexShader = NULL;
	m_fragmentShader = NULL;
}

CAnmDevice::~CAnmDevice()
{
	SafeUnRef( m_viewport );
}

void CAnmDevice::MakeViewport()
{

	SafeUnRef(m_viewport);

	m_viewport = new CAnmViewport(this, m_rcViewportRect.right, m_rcViewportRect.bottom);
}

BOOL CAnmDevice::UpdateRectangles()
{
	RECT cr, sr;

	// viewport rect is equal to client window rect
	GetClientRect( m_hWnd, &cr );

	cr.bottom;

	// transform to screen coords to get screen rect
	sr = cr;
	ClientToScreen( m_hWnd, (POINT*)&sr.left );
	ClientToScreen( m_hWnd, (POINT*)&sr.right );

	int width = sr.right - sr.left;
	int height = sr.bottom - sr.top;

	if (width <= ANMDEVICE_MINDEVICEWIDTH || height <= ANMDEVICE_MINDEVICEHEIGHT)
		return FALSE;

	m_rcViewportRect = cr;

	if (m_rcScreenRect.top != sr.top ||
		m_rcScreenRect.left != sr.left ||
		m_rcScreenRect.bottom != sr.bottom ||
		m_rcScreenRect.right != sr.right)
	{
		m_rcScreenRect = sr;
		return TRUE;
	}

	return FALSE;
}

// Lighting help
void CAnmDevice::EnableLight(int index, bool on)
{
	if (on && (m_numActiveLights >= m_maxActiveLights))
	{
		// N.B.: should issue a warning
		return;
	}

	LightEnable(index, on);

	// save state into array for accurate count
	AllocActiveLight(index);
	if (on)
	{
		if (!m_activeLights[index])
		{
			m_numActiveLights++;
			m_activeLights[index] = true;
		}
	}
	else
	{
		if (m_activeLights[index])
		{
			m_numActiveLights--;
			m_activeLights[index] = false;
		}
	}
}

void CAnmDevice::ClearLights()
{
	int sz = m_activeLights.size();
	for (int i = 0; i < sz; i++)
	{
		if (m_activeLights[i])
		{
			LightEnable(i, false);
		}

		m_activeLights[i] = false;
	}

	m_numActiveLights = 0;
}

#if 1
CAnmDevice *CAnmDevice::CreateDevice(HWND hWnd, cApplication *pApp)
{
	assert(pApp);

	CAnmPlatform *pPlatform = pApp->GetPlatform();
	assert(pPlatform);

	return pPlatform->CreateDevice(hWnd, pApp);
}

#else
#ifdef DX9

#include "anmd3ddevice9.h"

// Factory
CAnmDevice *CAnmDevice::CreateDevice(HWND hWnd, cApplication *pApp)
{
	return new CAnmD3DDevice9(hWnd, pApp);
}


#else

#ifdef DX8

#include "anmd3ddevice8.h"

// Factory
CAnmDevice *CAnmDevice::CreateDevice(HWND hWnd, cApplication *pApp)
{
	return new CAnmD3DDevice8(hWnd, pApp);
}



#else

#include "anmd3ddevice7.h"

// Factory
CAnmDevice *CAnmDevice::CreateDevice(HWND hWnd, cApplication *pApp)
{
	return new CAnmD3DDevice7(hWnd, pApp);
}

#endif // DX8
#endif // DX9

#endif // 0