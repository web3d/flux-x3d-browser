/********************************************************************************
 * Flux
 *
 * File: anmdshowmovietextureblitter9.cpp
 * Description: Movie texture renderer for Direct Show/DX9
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
#include "anmdshowmovietextureblitter9.h"
#include "anmd3ddevice9.h"
#include "anmd3derrors.h"

CAnmDShowMovieTextureBlitter9::CAnmDShowMovieTextureBlitter9(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface, LONG width, LONG height, LONG pitch, LONG byteDepth )
: CAnmMovieTextureBlitter(pDevice, pTextureSurface, width, height, pitch, byteDepth)
{
	assert(m_textureSurface);

	// Initialize the main texture surface
	CAnmD3DTextureSurface9 *pTexture9 = (CAnmD3DTextureSurface9 *) m_textureSurface;
	pTexture9->InitSurface(m_width, m_height, false); // don't mipmap

	LPDIRECT3DTEXTURE9 pD3DTexture9 = pTexture9->GetTexture();

	if (pD3DTexture9 == NULL)
	{
		int debug = 1;
	}

	LPDIRECT3DSURFACE9 pSurface = NULL;
	HRESULT hr = pD3DTexture9->GetSurfaceLevel(0, &pSurface);
	if(FAILED(hr)) {
		throw CAnmError("GetSurfaceLevel failed");
	}
	hr = pSurface->GetDesc(&m_desc);
	if(FAILED(hr)) {
		throw CAnmError("GetDesc failed");
	}
	
	pSurface->Release();
}

CAnmDShowMovieTextureBlitter9::~CAnmDShowMovieTextureBlitter9()
{
}

HRESULT CAnmDShowMovieTextureBlitter9::CopyBits(BYTE  *pBmpBuffer)
{
	if (m_textureSurface == NULL)
		return E_FAIL;

	CAnmD3DTextureSurface9 *pTextureSurface9 = (CAnmD3DTextureSurface9 *) m_textureSurface;
	LPDIRECT3DTEXTURE9 pTexture9 = pTextureSurface9->GetTexture();

	D3DLOCKED_RECT pRect;
	HRESULT hr = pTexture9->LockRect(0, &pRect, NULL, 0);

	//Copy the data in pImage to the surface
	int numBitsInFormat = pTextureSurface9->GetNumBitsInFormat(m_desc.Format);
	if (numBitsInFormat != 16 && numBitsInFormat != 32)
	{
		pTexture9->UnlockRect(0);
		throw CAnmError("Unknown dwRGBBitCount [CAnmDShowMovieTextureBlitter8::CopyBits]");
	}

	// Possibly scale the bitmap
	// Copy the bitmap image to the surface
	BOOL scaling = FALSE;
	if (m_desc.Width != m_width || m_desc.Height != m_height)
	{
		scaling = TRUE;
	}

	float XRatio, YRatio, X, Y;
	int Xc, Yc;
	int *XPoints = NULL;
	int *YPoints = NULL;

	if (scaling)
	{
		XRatio = (float) ( (float)m_width /
						  (float) m_desc.Width);

		YRatio = (float) ( (float)m_height /
						  (float)m_desc.Height );

		//alloc the look up table
		XPoints = new int[m_desc.Width];

		for( Xc=0, X = 0.f; Xc < m_desc.Width ; X+=XRatio, Xc++ )
		   XPoints[Xc] = X;

		//same for the y axis
		YPoints = new int[m_desc.Height];

		for( Yc=0, Y = 0.f ; Yc < m_desc.Height ; Y+=YRatio, Yc++ )
		   YPoints[Yc] = Y;

	}


	BYTE* pSrc;
	BYTE* pSrc0;
    for( DWORD y=0; y<m_desc.Height; y++ )
    {
        WORD*  p16 =  (WORD*)((BYTE*)pRect.pBits + (m_desc.Height - y - 1) * pRect.Pitch);
        DWORD* p32 = (DWORD*)((BYTE*)pRect.pBits + (m_desc.Height - y - 1) * pRect.Pitch);

		pSrc = pBmpBuffer + y * m_pitch;
		if (scaling) {
			pSrc0 = pBmpBuffer + YPoints[y] * m_pitch;
		}

        for( DWORD x=0; x<m_desc.Width; x++ )
        {
            if (numBitsInFormat == 16 )
            {
				// N.B.: how do I do alpha on a 16-bit texture?
				if (scaling)
	                *p16 = pBmpBuffer[YPoints[y] * m_pitch + XPoints[x]*3] |
					(pBmpBuffer[YPoints[y] * m_pitch + XPoints[x]*3+1] << 5) |
						(pBmpBuffer[YPoints[y] * m_pitch + XPoints[x]*3+2] << 10);
				else
	                *p16 = pBmpBuffer[y *m_pitch + x*3] | 
					(pBmpBuffer[y * m_pitch + x*3+1] << 5) |
						(pBmpBuffer[y * m_pitch + x*3+2] << 10);
                p16++;
            }
            else if (numBitsInFormat == 32 )
            {
				if (scaling) {
					pSrc = pSrc0 + XPoints[x]*m_byteDepth;
				}
				if( m_byteDepth == 4 ) {
					*p32 =	pSrc[0] | 
							pSrc[1] << 8 |
							pSrc[2] << 16 |
							pSrc[3] << 24;
				}
				else {
					*p32 =	pSrc[0] | 
							pSrc[1] << 8 |
							pSrc[2] << 16 |
							0xFF000000;
				}
				pSrc+=m_byteDepth;

				p32++;
            }
        }
    }

	hr = pTexture9->UnlockRect(0);
	if(FAILED(hr)) {
		throw CAnmError("UnlockRect failed");
	}
	// Done with scale data
	if (scaling)
	{
		delete XPoints;
		delete YPoints;
	}
	
	return S_OK;
}

