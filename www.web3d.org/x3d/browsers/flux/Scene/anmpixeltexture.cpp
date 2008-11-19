/********************************************************************************
 * Flux
 *
 * File: anmpixeltexture.cpp
 * Description: PixelTexture node
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

#include "anmpixeltexture.h"
#include "anmworld.h"

CAnmPixelTexture::CAnmPixelTexture()
		: CAnmTexture()						// Parent Appearance will add me
{
	m_image = NULL;
}


CAnmPixelTexture::~CAnmPixelTexture()
{
	// N.B.: any reason to delete the image here? it's allocated in the file reader right now
	// maybe we need to ref count these? 
}

void CAnmPixelTexture::Realize()
{
	CAnmTexture::Realize();

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();

	CAnmDevice *pDevice = pApp->GetDevice();

	CAnmGraphics *pGraphics = pApp->GetGraphics();
	assert(pGraphics);

	// don't create the surface if we're an empty image (less grief that way)
	if (m_image && m_image->width && m_image->height)
		m_textureSurface = pGraphics->CreateTexture(pDevice, m_image);
}

// Accessors
void CAnmPixelTexture::SetImage(Image *pImage)
{
	m_image = pImage;
}

// Tables for Callbacks, Methods and Set/GetValues


DEFINE_VALUE(CAnmPixelTexture, image, eValueImage, Image*, GetImage, SetImage);
DEFINE_INHERITED_VALUE(CAnmPixelTexture, repeatS, CAnmTexture);
DEFINE_INHERITED_VALUE(CAnmPixelTexture, repeatT, CAnmTexture);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmPixelTexture)
VALUEID(CAnmPixelTexture, image),
VALUEID(CAnmPixelTexture, repeatS),
VALUEID(CAnmPixelTexture, repeatT),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmPixelTexture, CAnmTexture);

