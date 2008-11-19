/********************************************************************************
 * Flux
 *
 * File: anmfilledrectangle.cpp
 * Description: Filled Rectangle class - abstracts Windows
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
#include "anmfilledrectangle.h"
#include "anmdevice.h"

CAnmFilledRectangle::CAnmFilledRectangle(int x, int y, int width, int height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	
	// Create device context for drawing
    m_hDC = CreateCompatibleDC(NULL);

	// Get a drawing brush
	m_hBrush = (HBRUSH) GetStockObject(DKGRAY_BRUSH);
}

CAnmFilledRectangle::~CAnmFilledRectangle()
{
	if (m_hDC)
		DeleteDC(m_hDC);

	if (m_hBrush)
		DeleteObject(m_hBrush);

}

void CAnmFilledRectangle::MoveTo(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CAnmFilledRectangle::Resize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void CAnmFilledRectangle::Render(CAnmDevice *pDevice)
{
#if 0

	// N.B.: Commented out until we un-funk 2D drawing; also we did the rendering rework-- TP 8/2/04
	assert(m_hDC);

	HDC hDC;
	HRESULT hr = ((CAnmD3DDevice7 *)pDevice)->GetBackBuffer()->GetD3DSurface()->GetDC(&hDC);

	if (SUCCEEDED(hr))
	{
		RECT rect;
		rect.left = m_x;
		rect.right = m_x + m_width;
		rect.top = m_y;
		rect.bottom = m_y + m_height;

		HBRUSH hOldBrush = (HBRUSH) SelectObject(hDC, m_hBrush);

		FillRect(hDC, &rect, m_hBrush);

		SelectObject(hDC, hOldBrush);

		((CAnmD3DDevice7 *)pDevice)->GetBackBuffer()->GetD3DSurface()->ReleaseDC(hDC);
	}
#endif

}
