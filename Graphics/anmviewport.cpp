/********************************************************************************
 * Flux
 *
 * File: anmviewport.h
 * Description: Viewport class
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
#include "anmviewport.h"
#include "anmdevice.h"
#include "anmviewpoint.h"
#include "anmcamera.h"
#include "anmtransform.h"
#include "anmworld.h"

#include "anmprimitive.h"
#include "anmMatrixInvert.h"

CAnmViewport::CAnmViewport(CAnmDevice *pDevice, int width, int height) :
	m_device (pDevice),
	m_width(width),
	m_height(height)
{
	MakeD3DViewport();
}

CAnmViewport::~CAnmViewport()
{
}

void CAnmViewport::MakeD3DViewport()
{
	m_device->SetViewportParams(0, 0, m_width, m_height, 0.0f, 1.0f);
}

void CAnmViewport::HandleSize(int newwidth, int newheight)
{
	m_width = newwidth;
	m_height = newheight;

	// MakeD3DViewport();
} 

