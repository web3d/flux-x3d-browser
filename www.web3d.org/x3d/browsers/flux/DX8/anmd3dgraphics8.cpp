/********************************************************************************
 * Flux
 *
 * File: anmd3dgraphics8.cpp
 * Description: Graphics layer definitions (DX8)
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
#include "anmd3dgraphics8.h"
#include "anmd3ddevice8.h"
#include "anmd3derrors.h"
#include "anmdx8message.h"
#include "anmprimitive.h"
#include "anmtexturedata.h"

#include "anmd3ddevice8.h"
#include "anmd3dgraphics8.h"

// Constructor/destructor
CAnmD3DGraphics8::CAnmD3DGraphics8(cApplication *pApp) : CAnmGraphics(pApp)
{
}

CAnmD3DGraphics8::~CAnmD3DGraphics8()
{
}

// Factories
CAnmGraphics::Light *CAnmD3DGraphics8::CreateLight(eAnmLightType lightType)
{
	CAnmD3DLight8 *pLight8 = new CAnmD3DLight8();
	pLight8->SetLightType(lightType);

	return pLight8;
}

CAnmGraphics::Material *CAnmD3DGraphics8::CreateMaterial()
{
	CAnmD3DMaterial8 *pMaterial8 = new CAnmD3DMaterial8();

	return pMaterial8;
}

CAnmGraphics::Fog *CAnmD3DGraphics8::CreateFog()
{
	CAnmD3DFog8 *pFog8 = new CAnmD3DFog8();

	return pFog8;
}

CAnmGraphics::Texture *CAnmD3DGraphics8::CreateTexture(CAnmDevice *pDevice, int stage)
{
	CAnmD3DTextureSurface8 *pTexture8 = new CAnmD3DTextureSurface8(pDevice, stage);

	return pTexture8;
}

CAnmGraphics::Texture *CAnmD3DGraphics8::CreateTexture(CAnmDevice *pDevice, CAnmTextureData *pTextureData,
		int stage, int width, int height)
{
	CAnmD3DTextureSurface8 *pTexture8 = new CAnmD3DTextureSurface8(pDevice, pTextureData, stage, width, height);

	return pTexture8;
}

CAnmGraphics::Texture *CAnmD3DGraphics8::CreateTexture(CAnmDevice *pDevice, Image *pImage,
		int stage)
{
	CAnmD3DTextureSurface8 *pTexture8 = new CAnmD3DTextureSurface8(pDevice, pImage, stage);

	return pTexture8;
}



CAnmGraphics::Texture *CAnmD3DGraphics8::CreateCubeTexture(CAnmDevice *pDevice, int stage)
{
	CAnmD3DCubeTextureSurface8 *pTexture8 = new CAnmD3DCubeTextureSurface8(pDevice, stage);

	return pTexture8;
}

CAnmGraphics::Texture *CAnmD3DGraphics8::CreateCubeTexture(CAnmDevice *pDevice, CAnmTextureData *pTextureData[6],
		int stage, int pixelDim )
{
	CAnmD3DCubeTextureSurface8 *pTexture8 = new CAnmD3DCubeTextureSurface8(pDevice, pTextureData, stage, pixelDim);

	return pTexture8;
}
/***
CAnmGraphics::Texture *CAnmD3DGraphics8::CreateCubeTexture(CAnmDevice *pDevice, Image *pImage[6],
		int stage)
{
	CAnmD3DCubeTextureSurface8 *pTexture8 = new CAnmD3DCubeTextureSurface8(pDevice, pImage, stage);

	return pTexture8;
}
***/


bool CAnmD3DGraphics8::AddTextureDataToCubeTexture( CAnmDevice *pDevice, Texture *pTextureSurface, int iFace, 
		class CAnmTextureData *pTextureData )
{
	bool bAdded = false;

	if( pDevice &&
		pTextureSurface &&
		pTextureData ) {

		CAnmD3DCubeTextureSurface8* pTexture8 = ( CAnmD3DCubeTextureSurface8* )pTextureSurface;


		bAdded = pTexture8->CopyTextureData( iFace, pTextureData );


	}
	return bAdded;
}


CAnmGraphics::RenderPrimitive *CAnmD3DGraphics8::CreatePrimitive(class CAnmPrimitive *pPrimitive)
{
	CAnmD3DRenderPrimitive8 *pPrimitive8 = new CAnmD3DRenderPrimitive8(pPrimitive);

	return pPrimitive8;
}

CAnmGraphics::Shader *CAnmD3DGraphics8::CreateShader(class CAnmShaderProgram *pShaderProgram)
{
	return NULL;
}


void CAnmD3DGraphics8::DrawBox(Point3 corners[8], Color c, CAnmDevice *pDevice)
{
	CAnmD3DDevice8 *pDevice8 = (CAnmD3DDevice8 *) pDevice;
	LPDIRECT3DDEVICE8 pD3DDevice = pDevice8->GetD3DDevice();

	sAnmUnlitVertex v[16];
	ULONG clr = c.MakeDWord();

	int indices[] =
	{ 0, 1, 2, 3, 0, 4, 5, 6, 7, 4, 5, 1, 2, 6, 7, 3};

	for (int i = 0; i < 16; i++)
	{
		v[i].vec = corners[indices[i]];
		v[i].diffuseColor = clr;
	}

	DWORD oldms;
	pD3DDevice->GetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, &oldms);
	pD3DDevice->SetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_COLOR1);

	DWORD currentShader;
	pD3DDevice->GetVertexShader(&currentShader);
	pD3DDevice->SetVertexShader(ANMUNLITVERTEXFLAGS);

	pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 15, (void *) &v[0], sizeof(sAnmUnlitVertex));

	if (currentShader != NULL)
		pD3DDevice->SetVertexShader(currentShader);

	pD3DDevice->SetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, oldms);
}

void CAnmD3DGraphics8::DrawAxes(Point3 origin, Point3 axes[3], Color c, CAnmDevice *pDevice)
{
	CAnmD3DDevice8 *pDevice8 = (CAnmD3DDevice8 *) pDevice;
	LPDIRECT3DDEVICE8 pD3DDevice = pDevice8->GetD3DDevice();

	sAnmUnlitVertex v[2];
	ULONG clr = c.MakeDWord();

	DWORD oldms;
	pD3DDevice->GetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, &oldms);
	pD3DDevice->SetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_COLOR1);

	v[0].vec = origin;
	v[0].diffuseColor = clr;
	v[1].diffuseColor = clr;

	DWORD currentShader;
	pD3DDevice->GetVertexShader(&currentShader);
	pD3DDevice->SetVertexShader(ANMUNLITVERTEXFLAGS);

	for (int i = 0; i < 3; i++)
	{
		v[1].vec = origin + axes[i];
		pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, (void *) &v[0], sizeof(sAnmUnlitVertex));
	}

	if (currentShader != NULL)
		pD3DDevice->SetVertexShader(currentShader);

	pD3DDevice->SetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, oldms);
}

// CAnmD3DTextureSurface8 - texture map class for DX7

// Initialize texture map from a bitmap file
CAnmD3DTextureSurface8::CAnmD3DTextureSurface8(CAnmDevice *pDevice, CAnmTextureData *pTextureData, int stage,
									   int width, int height)
	: CAnmGraphics::Texture(pDevice, stage)
{
	m_pTexture = NULL;
	m_texturedata = pTextureData;
	
	if (width == 0 || height == 0)
		InitSurface(pTextureData->GetWidth(), pTextureData->GetHeight(), pTextureData->IsMipmapped());
	else
		InitSurface(width, height, pTextureData->IsMipmapped());

	// get the bits into my surface
	if( pTextureData && m_pTexture ) {
		CopyTextureData(pTextureData);
	}
	else {
		// N.B.: length
		char errstring[ANMMAXSTRBUF];
		sprintf( errstring, "Warning: Cant create D3D Surface for image: %s",
			( m_texturedata && m_texturedata->GetFilename() ) ? m_texturedata->GetFilename()->GetBuf() : "" );
		pDevice->GetApp()->WriteToConsole(errstring);
	}
}



CAnmD3DTextureSurface8::CAnmD3DTextureSurface8(CAnmDevice *pDevice, Image *pImage, int stage)
	: CAnmGraphics::Texture(pDevice, stage)
{
	m_pTexture = NULL;
	m_texturedata = NULL;

	if (pImage->width > 0 && pImage->height > 0)
	{
		InitSurface(pImage->width, pImage->height);
		CopyPixels(pImage);
	}
}

CAnmD3DTextureSurface8::CAnmD3DTextureSurface8(CAnmDevice *pDevice, int stage)
	: CAnmGraphics::Texture(pDevice, stage)
{
	m_pTexture = NULL;
	m_texturedata = NULL;
}

CAnmD3DTextureSurface8::~CAnmD3DTextureSurface8()
{
	SafeRelease(m_pTexture);
}

// InitSurface - does all the grunt work

void CAnmD3DTextureSurface8::InitSurface(int width, int height, bool mipmap)
{
    // Get the device caps
	CAnmD3DDevice8 *pDevice8 = (CAnmD3DDevice8 *) m_device;
	D3DCAPS8 devdesc = pDevice8->GetD3DDesc();

	HRESULT hr;

	//Use the surface format of the current display mode as the format for the texture.
	//I don't know if this will work but lets give it a go. I think we should do
	//some extra checking/validating with CheckDeviceFormat() - TG
	D3DFORMAT adapterFormat = pDevice8->GetAdapterFormat();
	// Adjust width and height to be powers of 2, if the device requires it
	// Err on the side of downsampling-- TP 11/20/02
	int w = width;
	int h = height;
    if( devdesc.TextureCaps & D3DPTEXTURECAPS_POW2 )
    {
        for( w=1;  width>w;   w<<=1 );
        for( h=1; height>h; h<<=1 );
    }

	// mipmap = false;
	hr = pDevice8->GetD3DDevice()->CreateTexture(w, h, mipmap ? 0 : 1, 0, D3DFMT_A8R8G8B8 /*adapterFormat*/, 
		D3DPOOL_MANAGED, &m_pTexture);	

	m_width = w;
	m_height = h;
}

void CAnmD3DTextureSurface8::CopyPixels(Image *pImage)
{
	//Get the texture's surface description:
	LPDIRECT3DSURFACE8 pSurface = NULL;
	HRESULT hr = m_pTexture->GetSurfaceLevel(0, &pSurface);
	if(FAILED(hr)) {
		// ANMMSG_GETSURFACELEVELFAILED : "GetSurfaceLevel failed"
		throw CAnmError(ANMMSG_GETSURFACELEVELFAILED);
	}
	D3DSURFACE_DESC desc;
	hr = pSurface->GetDesc(&desc);
	if(FAILED(hr)) {
		// ANMMSG_GETDESCFAILED : "GetDesc failed"
		throw CAnmError(ANMMSG_GETDESCFAILED);
	}

	//create a temporary surface
	LPDIRECT3DSURFACE8 pTempSurface;
	CAnmD3DDevice8 *pDevice8 = (CAnmD3DDevice8 *) m_device;
	LPDIRECT3DDEVICE8 pDevice = pDevice8->GetD3DDevice();
	hr = pDevice->CreateImageSurface(desc.Width, desc.Height, desc.Format, &pTempSurface);
	if(FAILED(hr)) {
		// ANMMSG_CREATEIMAGESURFACEFAILED : "CreateImageSurface failed"
		throw CAnmError(ANMMSG_CREATEIMAGESURFACEFAILED);
	}

	//Lock the surface
	D3DLOCKED_RECT pRect;
	hr = pTempSurface->LockRect(&pRect, NULL, 0);
	if(FAILED(hr)) {
		// ANMMSG_LOCKRECTFAILED : "LockRect failed"
		throw CAnmError(ANMMSG_LOCKRECTFAILED);
	}

	//Copy the data in pImage to the surface
	int numBitsInFormat = GetNumBitsInFormat(desc.Format);
	if (numBitsInFormat != 16 && numBitsInFormat != 32)
	{
		pTempSurface->UnlockRect();
		// ANMMSG_UNKNOWNRGBBITCOUNTCD2S : "Unknown dwRGBBitCount [CAnmJPEGTextureData::CopyDataToSurface]"
		throw CAnmError(ANMMSG_UNKNOWNRGBBITCOUNTCD2S);
	}
	
	// Possibly scale the bitmap
	float XRatio, YRatio, X, Y;
	int Xc, Yc;
	int *XPoints = NULL;
	int *YPoints = NULL;

	BOOL scaling = FALSE;
	if (desc.Width != pImage->width || desc.Height != pImage->height)
	{
		scaling = TRUE;
		XRatio = (float) ( (float)pImage->width /
						  (float) desc.Width);

		YRatio = (float) ( (float)pImage->height /
						  (float)desc.Height );

		//alloc the look up table
		XPoints = new int[desc.Width];

		for( Xc=0, X = 0.f; Xc < desc.Width ; X+=XRatio, Xc++ )
		   XPoints[Xc] = X;

		//same for the y axis
		YPoints = new int[desc.Height];

		for( Yc=0, Y = 0.f ; Yc < desc.Height ; Y+=YRatio, Yc++ )
		   YPoints[Yc] = Y;

	}

    for( DWORD y=0; y<desc.Height; y++ )
    {
        WORD*  p16 =  (WORD*)((BYTE*)pRect.pBits + y*pRect.Pitch);
        DWORD* p32 = (DWORD*)((BYTE*)pRect.pBits + y*pRect.Pitch);
			
        for( DWORD x=0; x<desc.Width; x++ )
		{

			DWORD argb, rgba32;
			WORD rgba16;
			BYTE intensity, alpha;

			if (scaling)
				argb = (*pImage->pixels)[pImage->width * YPoints[desc.Height - y - 1] + XPoints[x]];
			else
				argb = (*pImage->pixels)[pImage->width  * (desc.Height - y - 1) + x];

			if (pImage->numComponents == 1)
			{
				intensity = argb & 0xFF;

				// intensity only; copy to all 3 color values, make alpha 0xFF
				argb = 0xFF | (intensity << 8) | (intensity << 16) | (intensity << 24);

			}
			else if (pImage->numComponents == 2)
			{
				// alpha + intensity
				alpha = argb & 0xFF;
				intensity = (argb & 0xFF00) >> 8;

				// intensity only; copy to all 3 color values, make alpha 0xFF
				argb = alpha | (intensity << 8) | (intensity << 16) | (intensity << 24);
			}
			else if (pImage->numComponents == 3)
			{
				argb <<= 8;
				argb |= 0xFF;
			}

            if (numBitsInFormat == 16 )
            {
				rgba16 = ((argb & 0x1F000000) >> 13) |		// red
						((argb & 0x1F0000) >> 13) |			// green
						((argb & 0x1F00) >> 8);				// blue

				// set top bit if there's high enough alpha value
				if ((argb & 0xFF) > 0xF)
					rgba16 |= (1 << 15);

                *p16++ = rgba16;
            }
            else if (numBitsInFormat == 32 )
            {
				rgba32 = ((argb >> 8) |				// move RGB down
						  (argb & 0xFF) << 24);		// move alpha up

				*p32++ = rgba32;
			}
		}
	}


	hr = pTempSurface->UnlockRect();
	if(FAILED(hr)) {
		// ANMMSG_UNLOCKRECTFAILED : "UnlockRect failed"
		throw CAnmError(ANMMSG_UNLOCKRECTFAILED);
	}

    // Copy the temp surface to the real texture surface
    //hr = m_pD3DSurface->Blt( NULL, pTempSurface, NULL, DDBLT_WAIT, NULL );
	pDevice->CopyRects(pTempSurface, NULL, 0, pSurface, NULL);

    // Done with the temp surface
    pTempSurface->Release();

	//done with the texture's surface
	pSurface->Release();

	// Done with scale data
	if (scaling)
	{
		delete XPoints;
		delete YPoints;
	}

	if ((pImage->numComponents <= 2))
		m_grayscale = true;

	if ((pImage->numComponents == 2) || (pImage->numComponents == 4))
		m_hasalpha = true;

//	if (m_hastransparentcolor)
//		pTextureSurface->SetHasTransparentColor(TRUE);
}

#define iR	2	
#define iG	1	
#define iB	0	

void CAnmD3DTextureSurface8::CopyTextureData(CAnmTextureData *pTextureData)
{
	// This is only a test...
	// throw CAnmError("forcing texture data exception");

	assert(pTextureData);
	assert(pTextureData->ImageValid());

	// Tell the texture surface there was some alpha
	if (pTextureData->HasAlpha())
		SetHasAlpha(true);

	if (pTextureData->IsGrayScale())
		SetIsGrayscale(true);

	//Get the texture's surface description:
	LPDIRECT3DSURFACE8 pSurface = NULL;
	HRESULT hr = m_pTexture->GetSurfaceLevel(0, &pSurface);
	if(FAILED(hr)) {
		// ANMMSG_GETSURFACELEVELFAILED : "GetSurfaceLevel failed"
		throw CAnmError(ANMMSG_GETSURFACELEVELFAILED);
	}

	CopyTextureDataToSurface( pSurface, pTextureData, 
		m_device, m_grayscale, m_hasalpha );

	//done with the texture's surface
	pSurface->Release();

	bool mipmap = m_pTexture->GetLevelCount() > 1;

	if (mipmap)
		GenMipMap();
}



void CAnmD3DTextureSurface8::CopyTextureDataToSurface( LPDIRECT3DSURFACE8 pSurface, CAnmTextureData *pTextureData,
							  class CAnmDevice *pAnmDevice,  bool bgrayscale, bool bhasalpha )
{

	if( !pSurface ) {
		throw CAnmError(ANMMSG_GETDESCFAILED);
	}

	D3DSURFACE_DESC desc;
	HRESULT hr = pSurface->GetDesc(&desc);
	if(FAILED(hr)) {
		// ANMMSG_GETDESCFAILED : "GetDesc failed"
		throw CAnmError(ANMMSG_GETDESCFAILED);
	}

	//create a temporary surface
	LPDIRECT3DSURFACE8 pTempSurface;
	CAnmD3DDevice8 *pDevice8 = (CAnmD3DDevice8 *) pAnmDevice;
	LPDIRECT3DDEVICE8 pDevice = pDevice8->GetD3DDevice();
	hr = pDevice->CreateImageSurface(desc.Width, desc.Height, desc.Format, &pTempSurface);
	if(FAILED(hr)) {
		// ANMMSG_CREATEIMAGESURFACEFAILED : "CreateImageSurface failed"
		throw CAnmError(ANMMSG_CREATEIMAGESURFACEFAILED);
	}

	//Lock the surface
	D3DLOCKED_RECT pRect;
	hr = pTempSurface->LockRect(&pRect, NULL, 0);
	if(FAILED(hr)) {
		// ANMMSG_LOCKRECTFAILED : "LockRect failed"
		throw CAnmError(ANMMSG_LOCKRECTFAILED);
	}

	//Copy the data in pImage to the surface
	int numBitsInFormat = GetNumBitsInFormat(desc.Format);
	if (numBitsInFormat != 16 && numBitsInFormat != 32)
	{
		pTempSurface->UnlockRect();
		// ANMMSG_UNKNOWNRGBBITCOUNTCD2S : "Unknown dwRGBBitCount [CAnmJPEGTextureData::CopyDataToSurface]"
		throw CAnmError(ANMMSG_UNKNOWNRGBBITCOUNTCD2S);
	}

	// Possibly scale the bitmap
	float XRatio, YRatio;

	int nBytesPerPixel = ( bgrayscale ) ? 1 : 3;
	if( bhasalpha ) {
		nBytesPerPixel++;
	}

	int width = pTextureData->GetWidth();
	int height = pTextureData->GetHeight();
	BOOL scalingX = (desc.Width != width );
	BOOL scalingY = (desc.Height != height );

	XRatio = (float) ( (float)width /
					  (float) desc.Width);

	YRatio = (float) ( (float)height /
					  (float)desc.Height );

	// krv; Get index of Alpha Byte, and set the alphaByte to full bits on.
	//
	int iA = ( bgrayscale ) ? 1 : 3;
	BYTE  alpahByte = 0xFF;
	BYTE *pPixelData = pTextureData->GetPixelData();
	BYTE* pPixSrc;
	int iYOffset;
	int iOffset;


    for( DWORD y=0; y<desc.Height; y++ )
    {
        WORD*  p16 =  (WORD*)((BYTE*)pRect.pBits + y*pRect.Pitch);
        BYTE*  p8 =   (BYTE*)(p16);

		// krv:  Calulate the YOffset now,
		//
		if( scalingY ) {
			iYOffset = ( nBytesPerPixel * width ) * ( ( ( int ) ( ( ( float ) (desc.Height - 1 - y ) ) * YRatio ) ) );
		}
		else {
			iYOffset = ( desc.Height - 1 - y ) *nBytesPerPixel*width;
		}
		iOffset = iYOffset;
        for( DWORD x=0; x<desc.Width; x++ )
        {
			if (scalingX) {
				iOffset = iYOffset + nBytesPerPixel * ( ( int ) ( ( ( float ) x )*XRatio ) );
			}

			pPixSrc = &pPixelData[iOffset];

            if (numBitsInFormat == 16 )
            {
				// N.B.: how do I do alpha on a 16-bit texture?
				if (bgrayscale)
				{
					*p16++ =  pPixSrc[0] |
							( pPixSrc[0] << 5) |
							( pPixSrc[0] << 10);
				}
				else
				{
					*p16++ =  pPixSrc[iR] |
							( pPixSrc[iG] << 5) |
							( pPixSrc[iB] << 10);
				}
            }
            else if (numBitsInFormat == 32 )
            {
				// krv: If we have alpha, set the alpha byte.
				if (bhasalpha)
				{
					alpahByte = pPixSrc[iA];
				}

				if (bgrayscale)
				{
					*p8++ = pPixSrc[0];
					*p8++ =	pPixSrc[0];
					*p8++ =	pPixSrc[0];
					*p8++ =	alpahByte;
				}
				else
				{
					*p8++ = pPixSrc[iR];
					*p8++ =	pPixSrc[iG];
					*p8++ =	pPixSrc[iB];
					*p8++ =	alpahByte;
				}
            }
			if (!scalingX) {
				iOffset+=nBytesPerPixel;
			}
        }
    }



	hr = pTempSurface->UnlockRect();
	if(FAILED(hr)) {
		// ANMMSG_UNLOCKRECTFAILED : "UnlockRect failed"
		throw CAnmError(ANMMSG_UNLOCKRECTFAILED);
	}

    // Copy the temp surface to the real texture surface
    //hr = m_pD3DSurface->Blt( NULL, pTempSurface, NULL, DDBLT_WAIT, NULL );
	pDevice->CopyRects(pTempSurface, NULL, 0, pSurface, NULL);

    // Done with the temp surface
    pTempSurface->Release();
}





void CAnmD3DTextureSurface8::GenMipMap()
{
	int nLevels = m_pTexture->GetLevelCount();
	assert (nLevels > 1);

	for (int i = 0; i < nLevels - 1 ; i++) 
	{
		LPDIRECT3DSURFACE8 pParentSurface = NULL;
		HRESULT hr = m_pTexture->GetSurfaceLevel(i, &pParentSurface);
		if(FAILED(hr)) {
			// ANMMSG_GETSURFACELEVELFAILEDMIP : "GetSurfaceLevel failed [CAnmD3DTextureSurface8::GenMipMap]"
			throw CAnmError(ANMMSG_GETSURFACELEVELFAILEDMIP);
		}

		LPDIRECT3DSURFACE8 pChildSurface = NULL;
		hr = m_pTexture->GetSurfaceLevel(i + 1, &pChildSurface);
		if(FAILED(hr)) {
			// ANMMSG_GETSURFACELEVELFAILEDMIP : "GetSurfaceLevel failed [CAnmD3DTextureSurface8::GenMipMap]"
			throw CAnmError(ANMMSG_GETSURFACELEVELFAILEDMIP);
		}

		GenMipMapLevel(pParentSurface, pChildSurface);

		SafeRelease(pParentSurface);
		SafeRelease(pChildSurface);
	}
}










/*
IDirect3DDevice8::CopyRects
Copies rectangular subsets of pixels from one surface to another. 

HRESULT CopyRects(
  IDirect3DSurface8* pSourceSurface,
  CONST RECT* pSourceRectsArray,
  UINT cRects,
  IDirect3DSurface8* pDestinationSurface,
  CONST POINT* pDestPointsArray
);

*/

void CAnmD3DTextureSurface8::GenMipMapLevel(LPDIRECT3DSURFACE8 pParentSurface, LPDIRECT3DSURFACE8 pChildSurface)
{
	HRESULT hr;

	// Get surface information
	D3DSURFACE_DESC psd;
	hr = pParentSurface->GetDesc(&psd);
	if(FAILED(hr)) {
		// ANMMSG_GETDESCFAILED : "GetDesc failed"
		throw CAnmError(ANMMSG_GETDESCFAILED);
	}

	D3DSURFACE_DESC csd;
	hr = pChildSurface->GetDesc(&csd);
	if(FAILED(hr)) {
		// ANMMSG_GETDESCFAILED : "GetDesc failed"
		throw CAnmError(ANMMSG_GETDESCFAILED);
	}

	int parentWidth = psd.Width;
	int parentHeight = psd.Height;

	int childWidth = csd.Width;
	int childHeight = csd.Height;


	// Check formats for sanity
	int numBitsInParentFormat = GetNumBitsInFormat(psd.Format);
	if (numBitsInParentFormat != 16 && numBitsInParentFormat != 32)
	{
		// ANMMSG_UNKNUMBITSPARENTMIP : "Unknown numBitsInParentFormat [CAnmD3DTextureSurface8::GenMipMapLevel]"
		throw CAnmError(ANMMSG_UNKNUMBITSPARENTMIP);
	}

	int numBitsInChildFormat = GetNumBitsInFormat(csd.Format);
	if (numBitsInChildFormat != 16 && numBitsInChildFormat != 32)
	{
		// ANMMSG_UNKNUMBITSCHILDMIP : "Unknown numBitsInChildFormat [CAnmD3DTextureSurface8::GenMipMapLevel]"
		throw CAnmError(ANMMSG_UNKNUMBITSCHILDMIP);
	}

	// Create a Temporary surface for the parent
	CAnmD3DDevice8 *pDevice8 = (CAnmD3DDevice8 *) m_device;
	LPDIRECT3DDEVICE8 pDevice = pDevice8->GetD3DDevice();

	LPDIRECT3DSURFACE8 pParentTempSurface;
	hr = pDevice->CreateImageSurface(psd.Width, psd.Height, psd.Format, &pParentTempSurface);
	if(FAILED(hr)) {
		// ANMMSG_CREATEIMAGESURFACEFAILED : "CreateImageSurface failed"
		throw CAnmError(ANMMSG_CREATEIMAGESURFACEFAILED);
	}

	// Copy the bits from the real parent surface into the temp surface
	hr = pDevice->CopyRects(pParentSurface, NULL, 0, pParentTempSurface, NULL);

	// Lock the parent surface
	D3DLOCKED_RECT pParentRect;
	hr = pParentTempSurface->LockRect(&pParentRect, NULL, 0);
	if(FAILED(hr)) {
		// ANMMSG_LOCKRECTFAILED : "LockRect failed"
		throw CAnmError(ANMMSG_LOCKRECTFAILED);
	}

	// Create a Temporary surface for the child
	LPDIRECT3DSURFACE8 pChildTempSurface;
	hr = pDevice->CreateImageSurface(csd.Width, csd.Height, csd.Format, &pChildTempSurface);
	if(FAILED(hr)) {
		// ANMMSG_CREATEIMAGESURFACEFAILED : "CreateImageSurface failed"
		throw CAnmError(ANMMSG_CREATEIMAGESURFACEFAILED);
	}

	// Lock the child surface
	D3DLOCKED_RECT pChildRect;
	hr = pChildTempSurface->LockRect(&pChildRect, NULL, 0);
	if(FAILED(hr)) {
		// ANMMSG_LOCKRECTFAILED : "LockRect failed"
		throw CAnmError(ANMMSG_LOCKRECTFAILED);
	}


	// Now copy the data, averaging each four pixels in the bigger map to a pixel in the smaller one
    for( DWORD y=0; y<csd.Height; y++ )
    {
		// pointers into smaller bitmap
        WORD*  pc16 =  (WORD*)((BYTE*)pChildRect.pBits + y*pChildRect.Pitch);
        DWORD* pc32 = (DWORD*)((BYTE*)pChildRect.pBits + y*pChildRect.Pitch);

		// pointers into bigger bitmap
        WORD*  pp16r0 =  (WORD*)((BYTE*)pParentRect.pBits + (y*2) * pParentRect.Pitch);
        WORD*  pp16r1 =  (WORD*)((BYTE*)pParentRect.pBits + (y*2+1) * pParentRect.Pitch);
        DWORD* pp32r0 = (DWORD*)((BYTE*)pParentRect.pBits + (y*2) * pParentRect.Pitch);
        DWORD* pp32r1 = (DWORD*)((BYTE*)pParentRect.pBits + (y*2+1) * pParentRect.Pitch);

		if (y * 2 + 1 >= psd.Height)
		{
			pp16r1 = NULL;
			pp32r1 = NULL;
		}

        for( DWORD x=0; x<csd.Width; x++ )
        {
			WORD c16[4];
			DWORD c32[4];

			WORD clr16;
			BYTE clr32[4];

            if( numBitsInChildFormat == 16 )
            {
				c16[0] = *pp16r0++;

				if (x * 2 + 1 < psd.Width)
					c16[1] = *pp16r0++;
				else
					c16[1] = c16[0];

				if (pp16r1)
				{
					c16[2] = *pp16r1++;
					
					if (x * 2 + 1 < psd.Width)
						c16[3] = *pp16r1++;
					else
						c16[3] = c16[2];
				}
				else
				{
					c16[2] = c16[0];
					c16[3] = c16[1];
				}

				clr16 = (c16[0] + c16[1] + c16[2] + c16[3]) / 4;

				*pc16++ = clr16;
            }
            else if( numBitsInChildFormat == 32 )
            {
				c32[0] = *pp32r0++;

				if (x * 2 + 1 < psd.Width)
					c32[1] = *pp32r0++;
				else
					c32[1] = c32[0];

				if (pp32r1)
				{
					c32[2] = *pp32r1++;

					if (x * 2 + 1 < psd.Width)
						c32[3] = *pp32r1++;
					else
						c32[3] = c32[2];
				}
				else
				{
					c32[2] = c32[0];
					c32[3] = c32[1];
				}


				clr32[0] = ((c32[0] & 0xFF) + (c32[1] & 0xFF) + (c32[2] & 0xFF) + (c32[3] & 0xFF)) / 4;
				clr32[1] = (((c32[0] & 0xFF00) >> 8) + ((c32[1] & 0xFF00) >> 8) + ((c32[2] & 0xFF00) >> 8) + ((c32[3] & 0xFF00) >> 8)) / 4;
				clr32[2] = (((c32[0] & 0xFF0000) >> 16) + ((c32[1] & 0xFF0000) >> 16) + ((c32[2] & 0xFF0000) >> 16) + ((c32[3] & 0xFF0000) >> 16)) / 4;
				clr32[3] = (((c32[0] & 0xFF000000) >> 24) + ((c32[1] & 0xFF000000) >> 24) + ((c32[2] & 0xFF000000) >> 24) + ((c32[3] & 0xFF000000) >> 24)) / 4;

				*pc32++ = clr32[0] | (clr32[1] << 8) | (clr32[2] << 16) | (clr32[3] << 24);
			}
        }
    }

	hr = pParentTempSurface->UnlockRect();
	if(FAILED(hr)) {
		// ANMMSG_UNLOCKRECTFAILED : "UnlockRect failed"
		throw CAnmError(ANMMSG_UNLOCKRECTFAILED);
	}

	hr = pChildTempSurface->UnlockRect();
	if(FAILED(hr)) {
		// ANMMSG_UNLOCKRECTFAILED : "UnlockRect failed"
		throw CAnmError(ANMMSG_UNLOCKRECTFAILED);
	}


	hr = pDevice->CopyRects(pChildTempSurface, NULL, 0, pChildSurface, NULL);

	pParentTempSurface->Release();
	pChildTempSurface->Release();
}


int CAnmD3DTextureSurface8::GetNumBitsInFormat(D3DFORMAT format)
{
	//Assume format is an unsigned format
	int numBits = 0;
	switch(format) {
	case D3DFMT_R8G8B8:
		numBits = 24;
		break;
	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
	case D3DFMT_A2B10G10R10:
	case D3DFMT_G16R16:
		numBits = 32;
		break;
	case D3DFMT_R5G6B5:
	case D3DFMT_X1R5G5B5:
	case D3DFMT_A1R5G5B5:
	case D3DFMT_A4R4G4B4:
	case D3DFMT_A8R3G3B2:
	case D3DFMT_X4R4G4B4:
	case D3DFMT_A8L8:
	case D3DFMT_R3G3B2:
		numBits = 16;
		break;
	case D3DFMT_A8:
	case D3DFMT_A8P8:
	case D3DFMT_P8:
	case D3DFMT_L8:
	case D3DFMT_A4L4:
		numBits = 8;
		break;
	default:
		numBits = 0;
		break;
	}
	return numBits;
}











// Top of CubeTexture

// CAnmD3DTextureSurface8 - texture map class for DX7

// Initialize texture map from a bitmap file
CAnmD3DCubeTextureSurface8::CAnmD3DCubeTextureSurface8(CAnmDevice *pDevice, CAnmTextureData *pTextureData[6], int stage,
									   int pixelDim )
	: CAnmGraphics::Texture(pDevice, stage)
{
	m_iPixelDim = pixelDim;
	int width = pixelDim;
	int height = pixelDim;
	m_pTexture = NULL;
	m_texturedata = NULL;
	
	bool bIsMipMapped = false;

	if (width == 0 || height == 0) {

		for( int i=0; i<6; i++ ) {
			if( pTextureData[i] ) {
				width = pTextureData[i]->GetWidth();
				height = pTextureData[i]->GetHeight();
			}
		}


	}
	InitSurface(width, bIsMipMapped);

	// get the bits into my surface
	if( m_pTexture ) {
		for( int i=0; i<6; i++ ) {
			if( pTextureData[i] &&
				pTextureData[i]->GetWidth() == width &&
				pTextureData[i]->GetHeight() == height ) {
				CopyTextureData( i, pTextureData[i] );
			}
		}
	}
	else {
		// N.B.: length
		char errstring[ANMMAXSTRBUF];
		sprintf( errstring, "Warning: Cant create D3D Surface for image: %s",
			( m_texturedata && m_texturedata->GetFilename() ) ? m_texturedata->GetFilename()->GetBuf() : "" );
		pDevice->GetApp()->WriteToConsole(errstring);
	}
}


/**
CAnmD3DCubeTextureSurface8::CAnmD3DCubeTextureSurface8(CAnmDevice *pDevice, Image *pImage, int stage)
	: CAnmGraphics::Texture(pDevice, stage)
{
	m_pTexture = NULL;
	m_texturedata = NULL;

	if (pImage->width > 0 && pImage->height > 0)
	{
		InitSurface(pImage->width, pImage->height);
		CopyPixels(pImage);
	}
}
**/


CAnmD3DCubeTextureSurface8::CAnmD3DCubeTextureSurface8(CAnmDevice *pDevice, int stage)
	: CAnmGraphics::Texture(pDevice, stage)
{
	m_iPixelDim = 0;
	m_pTexture = NULL;
	m_texturedata = NULL;
}

CAnmD3DCubeTextureSurface8::~CAnmD3DCubeTextureSurface8()
{
	SafeRelease(m_pTexture);
}

// InitSurface - does all the grunt work

void CAnmD3DCubeTextureSurface8::InitSurface( int pixelDim, bool mipmap )
{
	m_iPixelDim = pixelDim;
	int width = pixelDim;
	int height = pixelDim;
    // Get the device caps
	CAnmD3DDevice8 *pDevice8 = (CAnmD3DDevice8 *) m_device;
	D3DCAPS8 devdesc = pDevice8->GetD3DDesc();

	HRESULT hr;

	//Use the surface format of the current display mode as the format for the texture.
	//I don't know if this will work but lets give it a go. I think we should do
	//some extra checking/validating with CheckDeviceFormat() - TG
	D3DFORMAT adapterFormat = pDevice8->GetAdapterFormat();
	// Adjust width and height to be powers of 2, if the device requires it
	// Err on the side of downsampling-- TP 11/20/02
	int w = pixelDim;
	int h = pixelDim;
    if( devdesc.TextureCaps & D3DPTEXTURECAPS_POW2 )
    {
        for( w=1;  width>w;   w<<=1 );
        for( h=1; height>h; h<<=1 );
    }
 
	// mipmap = false;
	hr = pDevice8->GetD3DDevice()->CreateCubeTexture( w, mipmap ? 0 : 1, 0, D3DFMT_A8R8G8B8 /*adapterFormat*/, 
			D3DPOOL_MANAGED , &m_pTexture);	

	m_width = w;
	m_height = h;
}


bool CAnmD3DCubeTextureSurface8::CopyTextureData( int iFace, CAnmTextureData *pTextureData)
{
	bool bTextureAdded = false;
	if( pTextureData && 
		pTextureData->ImageValid() &&
		m_pTexture &&
		pTextureData->GetHeight() == m_iPixelDim &&
		pTextureData->GetWidth() == m_iPixelDim ) {

		m_cubetexturedata[iFace] = pTextureData;


		// This is only a test...
		// throw CAnmError("forcing texture data exception");

		assert(pTextureData);
		assert(pTextureData->ImageValid());

		// Tell the texture surface there was some alpha
		if (pTextureData->HasAlpha())
			SetHasAlpha(true);

		if (pTextureData->IsGrayScale())
			SetIsGrayscale(true);

		//Get the texture's surface description:
		LPDIRECT3DSURFACE8 pSurface = NULL;

		HRESULT hr = m_pTexture->GetCubeMapSurface( (D3DCUBEMAP_FACES)iFace, 0, &pSurface );
		if(FAILED(hr)) {
			// ANMMSG_GETSURFACELEVELFAILED : "GetSurfaceLevel failed"
			throw CAnmError(ANMMSG_GETSURFACELEVELFAILED);
		}





		CAnmD3DTextureSurface8::CopyTextureDataToSurface( pSurface, pTextureData, 
			m_device, m_grayscale, m_hasalpha );

		//done with the texture's surface
		pSurface->Release();
		bTextureAdded = true;
	//	bool mipmap = false; //m_pTexture->GetLevelCount() > 1;
	//	if (mipmap)
	//		GenMipMap();
	}
	return bTextureAdded;
}












// CAnmD3DRenderPrimitive7 - handles all Draw Primitive work for DX7
CAnmD3DRenderPrimitive8::CAnmD3DRenderPrimitive8(CAnmPrimitive *pPrimitive)
: CAnmGraphics::RenderPrimitive(pPrimitive)
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;

	switch(pPrimitive->GetPrimitiveType())
	{
 		case eAnmPrimitiveTriangleList :
			m_d3dprimtype = D3DPT_TRIANGLELIST;
			break;

		case eAnmPrimitiveTriangleStrip :
			m_d3dprimtype = D3DPT_TRIANGLESTRIP;
			break;

		case eAnmPrimitiveTriangleFan :
			m_d3dprimtype = D3DPT_TRIANGLEFAN;
			break;

		case eAnmPrimitivePolyLine :
			m_d3dprimtype = D3DPT_LINESTRIP;
			break;

		case eAnmPrimitivePointList :
			m_d3dprimtype = D3DPT_POINTLIST;
			break;

		case eAnmPrimitiveBadType:
		default :
			assert(FALSE);
			break;
	}
}

CAnmD3DRenderPrimitive8::~CAnmD3DRenderPrimitive8()
{
	SafeRelease(m_vertexBuffer);
	SafeRelease(m_indexBuffer);
}

void CAnmD3DRenderPrimitive8::Buffer(class CAnmDevice *pDevice)
{
	assert(m_d3dprimtype != D3DPT_LINESTRIP && m_d3dprimtype != D3DPT_POINTLIST);

	assert(pDevice);

	// only do this on hardware-accelerated devices
	if (!pDevice->IsHardwareAccelerated())
	{
		// N.B.: generate warning? who cares...
		// ANMMSG_VBCREATEFAILED : "Failed to create vertex buffer"
		// pDevice->GetApp()->Warn(ANMMSG_VBCREATEFAILED);
		return;
	}

	assert(m_primitive);

	tAnmVertexArray *pVerts = m_primitive->GetVertices();
	assert(pVerts);

	if (pVerts->size() <= 0)
		return;

	HRESULT hr;

	CAnmD3DDevice8 *pDevice8 = (CAnmD3DDevice8 *) pDevice;
	LPDIRECT3DDEVICE8 pD3DDevice = pDevice8->GetD3DDevice();
	assert(pD3DDevice);

	hr = pD3DDevice->CreateVertexBuffer(sizeof(sAnmVertex)*pVerts->size(), D3DUSAGE_WRITEONLY, 
									ANMVERTEXFLAGS, D3DPOOL_MANAGED, &m_vertexBuffer);
	if (FAILED(hr))
	{
		// N.B.: generate warning? who cares...
		// ANMMSG_VBCREATEFAILED : "Failed to create vertex buffer"

		char *cp = AnmD3DError(hr);
		pDevice->GetApp()->Warn(ANMMSG_VBCREATEFAILED);
		return;
	}

	CopyVerticesToBuffer(pVerts);

	tAnmIndexArray *pIndices = m_primitive->GetIndices();
	assert(pIndices);

	int nIndices = pIndices->size();

	if (nIndices > 0)
	{
		// Create and fill the index buffer:
		hr = pD3DDevice->CreateIndexBuffer(nIndices*sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_indexBuffer);
		
		if (FAILED(hr))
		{
			// N.B.: generate warning? who cares...
			// ANMMSG_IBCREATEFAILED : "Failed to create index buffer"

			pDevice->GetApp()->Warn(ANMMSG_IBCREATEFAILED);
			return;
		}

		WORD* pIndexData;
		m_indexBuffer->Lock(0, 0, (BYTE**)&pIndexData, 0);
		int i;
		for(i=0; i<nIndices; i++) {
			pIndexData[i] = (*pIndices)[i];
		}
		m_indexBuffer->Unlock();
	}
}

void CAnmD3DRenderPrimitive8::UpdateBuffer()
{
	assert(m_d3dprimtype != D3DPT_LINESTRIP && m_d3dprimtype != D3DPT_POINTLIST);

	if (m_vertexBuffer == NULL)
		return;

	assert(m_primitive);


	tAnmVertexArray *pVerts = m_primitive->GetVertices();
	assert(pVerts);

	CopyVerticesToBuffer(pVerts);
}

void CAnmD3DRenderPrimitive8::CopyVerticesToBuffer(tAnmVertexArray *pVerts)
{
	assert(m_vertexBuffer);

	HRESULT hr;

	// lock the buffer for write
	sAnmVertex *pData = NULL;

	hr = m_vertexBuffer->Lock(0, 0, (BYTE **)&pData, 0);

	if (FAILED(hr))
	{
		// N.B.: generate warning? who cares...
		// ANMMSG_VBLOCKFAILED : "Failed to lock vertex buffer"
		// pDevice->GetApp()->Warn(generate);

		return;
	}

	// write the data
	int nVerts = pVerts->size();
	for (int i = 0; i < nVerts; i++)
		*pData++ = (*pVerts)[i];

	// unlock the buffer
	m_vertexBuffer->Unlock();
}


void CAnmD3DRenderPrimitive8::Render(class CAnmDevice *pDevice)
{
	assert(pDevice);
	assert(m_primitive);

	// This is only a test
//	throw 0L;
//	throw CAnmError("Force throw to test error trap");

	LPDIRECT3DDEVICE8 pD3DDevice = ((CAnmD3DDevice8 *)pDevice)->GetD3DDevice();
	HRESULT hr;

	// See if there's a vertex shader, otherwise use fixed function pipeline
	CAnmGraphics::Shader *pVertexShader = pDevice->GetVertexShader();

	bool lit = m_primitive->IsLit();
	bool indexed = m_primitive->IsIndexed();

	// See if there's a pixel shader; if so, turn lighting off - let the pixel shader do it
	CAnmGraphics::Shader *pPixelShader = pDevice->GetFragmentShader();
	DWORD oldLightState = FALSE;
	if (pPixelShader != NULL) { // || !lit) {
		pD3DDevice->GetRenderState(D3DRS_LIGHTING, &oldLightState);
		pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	if (lit)
	{
		if (pVertexShader == NULL) {
			pD3DDevice->SetVertexShader(ANMVERTEXFLAGS);
		}

		tAnmVertexArray *pVerts = m_primitive->GetVertices();
		assert(pVerts);

		int nVerts = pVerts->size();


		if (indexed)
		{
			if ( nVerts && m_vertexBuffer && m_indexBuffer)
			{
				// Set up the stream source and indices
				pD3DDevice->SetStreamSource(0, m_vertexBuffer, sizeof(sAnmVertex));
				pD3DDevice->SetIndices(m_indexBuffer, 0);
				
				// Draw it
				pD3DDevice->DrawIndexedPrimitive(m_d3dprimtype, 0, nVerts, 0, m_primitive->NumTriangles());

				// Null out the stream source and indices to prevent leaks
				pD3DDevice->SetStreamSource(0, NULL, 0);
				pD3DDevice->SetIndices(NULL, 0);
			}
			// krv: check neverts
			else if( nVerts )
			{
				tAnmIndexArray *pIndices = m_primitive->GetIndices();
				assert(pIndices);

				hr = pD3DDevice->DrawIndexedPrimitiveUP(m_d3dprimtype,
							0, nVerts, m_primitive->NumTriangles(), &((*pIndices)[0]),
							D3DFMT_INDEX16, &((*pVerts)[0]), sizeof(sAnmVertex));

				if (FAILED(hr))
				{
					throw CAnmError(AnmD3DError(hr));
				}			
			}
		}
		else
		{
			if (m_vertexBuffer)
			{
				//not indexed with a vertex buffer:
				pD3DDevice->SetStreamSource(0, m_vertexBuffer, sizeof(sAnmVertex));
				pD3DDevice->DrawPrimitive(m_d3dprimtype, 0, m_primitive->NumTriangles());
				pD3DDevice->SetStreamSource(0, NULL, 0);
			}
			else
			{
				//not indexed, without a vertex buffer
				pD3DDevice->DrawPrimitiveUP(m_d3dprimtype, m_primitive->NumTriangles(), 
					&((*pVerts)[0]), sizeof(sAnmVertex));
			}
		}
	}
	else		// !lit
	{
		if (pVertexShader == NULL)
			pD3DDevice->SetVertexShader(ANMUNLITVERTEXFLAGS);
		
		CAnmUnlitPrimitive *pUnlitPrimitive = (CAnmUnlitPrimitive *) m_primitive;

		tAnmUnlitVertexArray *pVerts = pUnlitPrimitive->GetUnlitVertices();
		assert(pVerts);

		int nVerts = pVerts->size();

		D3DMATERIAL8 mat;
		DWORD oldedgeantialias;
		
		pD3DDevice->GetMaterial(&mat);
		D3DCOLORVALUE oldDiffuseColor = mat.Diffuse;
		// Just turn diffuse color off, we're supposed to ignore it -- TP 12-10-03
		mat.Diffuse = * (D3DCOLORVALUE *) &color4::GetBlack();

		hr = pD3DDevice->SetMaterial(&mat);

		// For line sets, set up edge antialiasing
		if (m_d3dprimtype == D3DPT_LINESTRIP)
		{
			hr = pD3DDevice->GetRenderState(D3DRENDERSTATE_EDGEANTIALIAS, &oldedgeantialias);
			hr = pD3DDevice->SetRenderState(D3DRENDERSTATE_EDGEANTIALIAS, TRUE);
		}


		if( nVerts ) {
			if (indexed)
			{
				tAnmIndexArray *pIndices = m_primitive->GetIndices();
				assert(pIndices);

				int nIndices = pIndices->size();

				if (m_d3dprimtype == D3DPT_LINESTRIP) 
				{
					pD3DDevice->DrawIndexedPrimitiveUP(m_d3dprimtype,
						0, nVerts, nVerts - 1, &((*pIndices)[0]),
						D3DFMT_INDEX16, (void *) &((*pVerts)[0]), sizeof(sAnmUnlitVertex));
				}
				else
				{
					pD3DDevice->DrawIndexedPrimitiveUP(m_d3dprimtype,
						0, nVerts, nVerts, &((*pIndices)[0]),
						D3DFMT_INDEX16, (void *) &((*pVerts)[0]), sizeof(sAnmUnlitVertex));
				}
			}
			else
			{
				if (m_d3dprimtype == D3DPT_LINESTRIP) 
				{
					pD3DDevice->DrawPrimitiveUP(m_d3dprimtype,
						nVerts - 1, (void *) &((*pVerts)[0]),  sizeof(sAnmUnlitVertex));
				}
				else
				{
					pD3DDevice->DrawPrimitiveUP(m_d3dprimtype,
						nVerts, (void *) &((*pVerts)[0]),  sizeof(sAnmUnlitVertex));
				}
			}

		}

		mat.Diffuse = oldDiffuseColor;
		hr = pD3DDevice->SetMaterial(&mat);

		// For line sets, restore edge antialiasing
		if (m_d3dprimtype == D3DPT_LINESTRIP)
		{
			hr = pD3DDevice->SetRenderState(D3DRENDERSTATE_EDGEANTIALIAS, oldedgeantialias);
		}
	}

	if (pPixelShader != NULL) // || !lit)
		pD3DDevice->SetRenderState(D3DRS_LIGHTING, oldLightState);

}
