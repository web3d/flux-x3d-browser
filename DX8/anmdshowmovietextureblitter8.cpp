/********************************************************************************
 * Flux
 *
 * File: anmdshowmovietextureblitter8.cpp
 * Description: Movie texture renderer for Direct Show/DX8
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
#include "anmdshowmovietextureblitter8.h"
#include "anmd3ddevice8.h"
#include "anmd3derrors.h"
#include "anmdx8message.h"

CAnmDShowMovieTextureBlitter8::CAnmDShowMovieTextureBlitter8(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface, LONG width, LONG height, LONG pitch, LONG byteDepth)
: CAnmMovieTextureBlitter(pDevice, pTextureSurface, width, height, pitch, byteDepth)
{
	assert(m_textureSurface);

	// Initialize the main texture surface
	CAnmD3DTextureSurface8 *pTexture8 = (CAnmD3DTextureSurface8 *) m_textureSurface;
	pTexture8->InitSurface(m_width, m_height, false); // don't mipmap

	LPDIRECT3DTEXTURE8 pD3DTexture8 = pTexture8->GetTexture();

	if (pD3DTexture8 == NULL)
	{
		int debug = 1;
	}

	LPDIRECT3DSURFACE8 pSurface = NULL;
	HRESULT hr = pD3DTexture8->GetSurfaceLevel(0, &pSurface);
	if(FAILED(hr)) {
		// ANMMSG_GETSURFACELEVELFAILED : "GetSurfaceLevel failed"
		throw CAnmError(ANMMSG_GETSURFACELEVELFAILED);
	}
	hr = pSurface->GetDesc(&m_desc);
	if(FAILED(hr)) {
		// ANMMSG_GETDESCFAILED : "GetDesc failed"
		throw CAnmError(ANMMSG_GETDESCFAILED);
	}
	
	pSurface->Release();

	CAnmD3DDevice8 *pDevice8 = (CAnmD3DDevice8 *) pDevice;
	LPDIRECT3DDEVICE8 pD3DDevice = pDevice8->GetD3DDevice();
	hr = pD3DDevice->CreateImageSurface(m_desc.Width, m_desc.Height, m_desc.Format, &m_tempD3DSurface);
	if(FAILED(hr)) {
		// ANMMSG_CREATEIMAGESURFACEFAILED : "CreateImageSurface failed"
		throw CAnmError(ANMMSG_CREATEIMAGESURFACEFAILED);
	}

}

CAnmDShowMovieTextureBlitter8::~CAnmDShowMovieTextureBlitter8()
{
	SafeRelease(m_tempD3DSurface);
}

HRESULT CAnmDShowMovieTextureBlitter8::CopyBits(BYTE  *pBmpBuffer)
{
	if (m_textureSurface == NULL)
		return E_FAIL;

	if (m_tempD3DSurface == NULL)
		return E_FAIL;

	HRESULT hr;

	CAnmD3DTextureSurface8 *pTextureSurface8 = (CAnmD3DTextureSurface8 *) m_textureSurface;
	LPDIRECT3DTEXTURE8 pTexture8 = pTextureSurface8->GetTexture();

	LPDIRECT3DSURFACE8 pSurface = NULL;
	hr = pTexture8->GetSurfaceLevel(0, &pSurface);
	if(FAILED(hr)) {
		// ANMMSG_GETSURFACELEVELFAILED : "GetSurfaceLevel failed"
		throw CAnmError(ANMMSG_GETSURFACELEVELFAILED);
	}
	
	//Lock the surface
	D3DLOCKED_RECT pRect;
	hr = m_tempD3DSurface->LockRect(&pRect, NULL, 0);
	if(FAILED(hr)) {
		// ANMMSG_LOCKRECTFAILED : "LockRect failed"
		throw CAnmError(ANMMSG_LOCKRECTFAILED);
	}

	//Copy the data in pImage to the surface
	int numBitsInFormat = pTextureSurface8->GetNumBitsInFormat(m_desc.Format);
	if (numBitsInFormat != 16 && numBitsInFormat != 32)
	{
		m_tempD3DSurface->UnlockRect();
		// ANMMSG_UNKNOWNRGBBITCOUNTCBMTB : "Unknown dwRGBBitCount [CAnmDShowMovieTextureBlitter8::CopyBits]"
		throw CAnmError(ANMMSG_UNKNOWNRGBBITCOUNTCBMTB);
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

	hr = m_tempD3DSurface->UnlockRect();
	if(FAILED(hr)) {
		// ANMMSG_UNLOCKRECTFAILED : "UnlockRect failed"
		throw CAnmError(ANMMSG_UNLOCKRECTFAILED);
	}

    // Copy the temp surface to the real texture surface
    //hr = m_pD3DSurface->Blt( NULL, pTempSurface, NULL, DDBLT_WAIT, NULL );
	LPDIRECT3DDEVICE8 pD3DDevice = ((CAnmD3DDevice8 *)m_device)->GetD3DDevice();
	hr = pD3DDevice->CopyRects(m_tempD3DSurface, NULL, 0, pSurface, NULL);

	//done with the texture's surface
	pSurface->Release();

	// Done with scale data
	if (scaling)
	{
		delete XPoints;
		delete YPoints;
	}

	int nLevels = pTexture8->GetLevelCount();
	if (nLevels > 1)
		pTextureSurface8->GenMipMap();
	
	return S_OK;
}

