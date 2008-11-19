/********************************************************************************
 * Flux
 *
 * File: anmtexture.cpp
 * Description: Base Texture node
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

#include "anmtexture.h"
#include "anmdevice.h"
#include "anmtexturedata.h"
#include "anmfilemanager.h"
#include "anmworld.h"

CAnmTexture::CAnmTexture()
		: CAnmGroup(),						// Parent Appearance will add me
	m_repeatS(ANMTEXTURE_DEFAULT_REPEATS),
	m_repeatT(ANMTEXTURE_DEFAULT_REPEATT)	
{
	m_textureSurface = NULL;
}


CAnmTexture::~CAnmTexture()
{
	SafeUnRef(m_textureSurface);
}

void CAnmTexture::Realize()
{
	CAnmGroup::Realize();
}

void CAnmTexture::Update()
{
	ClearStateDirty();
}

BOOL CAnmTexture::HasAlpha()
{
	if (m_textureSurface && m_textureSurface->HasAlpha())
		return TRUE;
	else
		return FALSE;
}

BOOL CAnmTexture::IsGrayscale()
{
	if (m_textureSurface && m_textureSurface->IsGrayscale())
		return TRUE;
	else
		return FALSE;
}

// set texture stage states based on:
//		- grayscale maps are intensity maps
//		- if base material has alpha, set alpha blend ops

void CAnmTexture::SetToDevice(CAnmDevice *pDevice, bool alphablending, bool lighting)
{
	if (m_textureSurface)
	{
		pDevice->SetTexture(0, m_textureSurface);
	
		// set some states
		eAnmTextureAddressMode addressU;
		eAnmTextureAddressMode addressV;
		
		// addressing modes (wrap or clamp)
		if (m_repeatS)
			addressU = eAnmTextureAddressWrap;
		else
			addressU = eAnmTextureAddressClamp;

		if (m_repeatT)
			addressV = eAnmTextureAddressWrap;
		else
			addressV = eAnmTextureAddressClamp;

		pDevice->SetTextureOption( 0, eAnmTextureAddressU, addressU);
		pDevice->SetTextureOption( 0, eAnmTextureAddressV, addressV);

		// color arguments (modulate or select for grayscale or RGB respectively)
		if (m_textureSurface->IsGrayscale())
		{
			// grayscale textures are intensity maps; use modulate mode
			pDevice->SetTextureOption(0, eAnmTextureColorArg1, eAnmTextureArgCurrent);
			pDevice->SetTextureOption(0, eAnmTextureColorArg2, eAnmTextureArgTexture);
			pDevice->SetTextureOption(0, eAnmTextureColorOp, eAnmTextureModeMODULATE);
		}
		else
		{
			if (lighting) //  && !alphablending)
			{
				// N.B.: major hack to deal with the fact that D3D textures won't
				// light otherwise. Basically the texture modulates with a lit white material
				pDevice->SetTextureOption(0, eAnmTextureColorArg1, eAnmTextureArgCurrent);
				pDevice->SetTextureOption(0, eAnmTextureColorArg2, eAnmTextureArgTexture);
				pDevice->SetTextureOption(0, eAnmTextureColorOp, eAnmTextureModeMODULATE);
			}
			else
			{
				pDevice->SetTextureOption(0, eAnmTextureColorArg1, eAnmTextureArgTexture );
				pDevice->SetTextureOption(0, eAnmTextureColorOp, eAnmTextureModeSELECTARG1 );
			}
		}

		// N.B.: do some texture filtering (should this be settable as an option?
		int width = m_textureSurface->GetWidth();
		int height = m_textureSurface->GetHeight();
		pDevice->SetTextureOption(0, eAnmTextureFilter, (width * height > ANMTEXTURE_MINSIZEFORFILTER));

		// 2D Texture coordinates for now, thank you
		// N.B.: this logic properly belongs with any geometry that uses ->
		// TextureCoordinate nodes because those are where the actual ->
		// coordinate space is defined...
		pDevice->SetTextureOption(0, eAnmTextureNumTextureCoordinates, 
			( GetTexCoordVectorSize() == 3 ) ? eAnmTextureCount3 : eAnmTextureCount2 );

		// set alpha ops if we're blending with previous stage
		if (alphablending)
		{
			if (lighting)
			{
				pDevice->SetTextureOption(0, eAnmTextureAlphaArg1, eAnmTextureArgCurrent);	
				pDevice->SetTextureOption(0, eAnmTextureAlphaArg2, eAnmTextureArgTexture);	
				pDevice->SetTextureOption(0, eAnmTextureAlphaOp, eAnmTextureModeMODULATE);
			}
			else
			{
				pDevice->SetTextureOption(0, eAnmTextureAlphaArg1, eAnmTextureArgTexture );
				pDevice->SetTextureOption(0, eAnmTextureAlphaOp, eAnmTextureModeSELECTARG1 );
			}
		}
	}
	else
	{
		pDevice->SetTexture(0, NULL);
	}
}



// static version  of this method for general setting of a Texture
//
void CAnmTexture::SetToDevice( CAnmDevice *pDevice, 
							  bool alphablending, bool lighting,
							  CAnmGraphics::Texture* pTextureSurface,
							  Boolean bRepeatS,
							  Boolean bRepeatT )
{
	if (pTextureSurface)
	{
		pDevice->SetTexture(0, pTextureSurface);
	
		// set some states
		eAnmTextureAddressMode addressU;
		eAnmTextureAddressMode addressV;
		
		// addressing modes (wrap or clamp)
		if (bRepeatS)
			addressU = eAnmTextureAddressWrap;
		else
			addressU = eAnmTextureAddressClamp;

		if (bRepeatT)
			addressV = eAnmTextureAddressWrap;
		else
			addressV = eAnmTextureAddressClamp;

		pDevice->SetTextureOption( 0, eAnmTextureAddressU, addressU);
		pDevice->SetTextureOption( 0, eAnmTextureAddressV, addressV);

		// color arguments (modulate or select for grayscale or RGB respectively)
		if (pTextureSurface->IsGrayscale())
		{
			// grayscale textures are intensity maps; use modulate mode
			pDevice->SetTextureOption(0, eAnmTextureColorArg1, eAnmTextureArgCurrent);
			pDevice->SetTextureOption(0, eAnmTextureColorArg2, eAnmTextureArgTexture);
			pDevice->SetTextureOption(0, eAnmTextureColorOp, eAnmTextureModeMODULATE);
		}
		else
		{
			if (lighting) //  && !alphablending)
			{
				// N.B.: major hack to deal with the fact that D3D textures won't
				// light otherwise. Basically the texture modulates with a lit white material
				pDevice->SetTextureOption(0, eAnmTextureColorArg1, eAnmTextureArgCurrent);
				pDevice->SetTextureOption(0, eAnmTextureColorArg2, eAnmTextureArgTexture);
				pDevice->SetTextureOption(0, eAnmTextureColorOp, eAnmTextureModeMODULATE);
			}
			else
			{
				pDevice->SetTextureOption(0, eAnmTextureColorArg1, eAnmTextureArgTexture );
				pDevice->SetTextureOption(0, eAnmTextureColorOp, eAnmTextureModeSELECTARG1 );
			}
		}

		// N.B.: do some texture filtering (should this be settable as an option?
		int width = pTextureSurface->GetWidth();
		int height = pTextureSurface->GetHeight();
		pDevice->SetTextureOption(0, eAnmTextureFilter, FALSE); // width * height > ANMTEXTURE_MINSIZEFORFILTER);

		// 2D Texture coordinates for now, thank you
		// N.B.: this logic properly belongs with any geometry that uses ->
		// TextureCoordinate nodes because those are where the actual ->
		// coordinate space is defined...
		pDevice->SetTextureOption(0, eAnmTextureNumTextureCoordinates, 
			eAnmTextureCount2);

		// set alpha ops if we're blending with previous stage
		if (alphablending)
		{
			pDevice->SetAlphaBlending(true);

			if (lighting)
			{
				pDevice->SetTextureOption(0, eAnmTextureAlphaArg1, eAnmTextureArgCurrent);	
				pDevice->SetTextureOption(0, eAnmTextureAlphaArg2, eAnmTextureArgTexture);	
				pDevice->SetTextureOption(0, eAnmTextureAlphaOp, eAnmTextureModeMODULATE);
			}
			else
			{
				pDevice->SetTextureOption(0, eAnmTextureAlphaArg1, eAnmTextureArgTexture );
				pDevice->SetTextureOption(0, eAnmTextureAlphaOp, eAnmTextureModeSELECTARG1 );
			}
		}
	}
	else
	{
		pDevice->SetTexture(0, NULL);
	}
}


bool CAnmTexture::m_drawTextures = true;

void CAnmTexture::SetDrawTextures(bool drawTextures)
{
	m_drawTextures = drawTextures;
}

bool CAnmTexture::GetDrawTextures()
{
	return m_drawTextures;
}



// Accessors
void CAnmTexture::SetRepeatS(Boolean repeatS)
{
	m_repeatS = repeatS;

	SetStateDirty();
}

void CAnmTexture::SetRepeatT(Boolean repeatT)
{
	m_repeatT = repeatT;

	SetStateDirty();
}


// Tables for Callbacks, Methods and Set/GetValues


DEFINE_VALUE(CAnmTexture, repeatS, eValueBoolean, Boolean, GetRepeatS, SetRepeatS);
DEFINE_VALUE(CAnmTexture, repeatT, eValueBoolean, Boolean, GetRepeatT, SetRepeatT);

// member table
BEGIN_DEFINE_MEMBERS(CAnmTexture)
VALUEID(CAnmTexture, repeatS),
VALUEID(CAnmTexture, repeatT),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmTexture, CAnmNode);


