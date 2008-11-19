/********************************************************************************
 * Flux
 *
 * File: anmdshowmovietextureblitter8.h
 * Description: Movie texture renderer for Direct Show/DX7
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

#ifndef _anmdshowmovietextureblitter8_h
#define _anmdshowmovietextureblitter8_h

#include "anmd3dgraphics8.h"
#include "anmmovietexturerenderer.h"

class CAnmDShowMovieTextureBlitter8 : public CAnmMovieTextureBlitter
{
protected:

	LPDIRECT3DSURFACE8				 m_tempD3DSurface;
	D3DSURFACE_DESC					 m_desc;

public:

	// Constructor/destructor
    CAnmDShowMovieTextureBlitter8(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface,
		LONG width, LONG height, LONG pitch, LONG byteDepth);
    virtual ~CAnmDShowMovieTextureBlitter8();

	// CAnmDShowMovieTextureRenderer overrides
	virtual HRESULT CopyBits(BYTE  *pBmpBuffer);
};

#endif // _anmdshowmovietextureblitter8_h
