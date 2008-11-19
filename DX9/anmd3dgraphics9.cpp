/********************************************************************************
 * Flux
 *
 * File: anmd3dgraphics9.cpp
 * Description: Graphics layer definitions (DX9)
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
#include "anmd3dgraphics9.h"
#include "anmd3ddevice9.h"
#include "anmd3derrors.h"
#include "anmdx9message.h"
#include "anmprimitive.h"
#include "anmshaderprogram.h"
#include "anmtexturedata.h"

// Constructor/destructor
CAnmD3DGraphics9::CAnmD3DGraphics9(cApplication *pApp) : CAnmGraphics(pApp)
{
}

CAnmD3DGraphics9::~CAnmD3DGraphics9()
{
}

// Factories
CAnmGraphics::Light *CAnmD3DGraphics9::CreateLight(eAnmLightType lightType)
{
	CAnmD3DLight9 *pLight9 = new CAnmD3DLight9();
	pLight9->SetLightType(lightType);

	return pLight9;
}

CAnmGraphics::Material *CAnmD3DGraphics9::CreateMaterial()
{
	CAnmD3DMaterial9 *pMaterial9 = new CAnmD3DMaterial9();

	return pMaterial9;
}

CAnmGraphics::Fog *CAnmD3DGraphics9::CreateFog()
{
	CAnmD3DFog9 *pFog9 = new CAnmD3DFog9();

	return pFog9;
}

CAnmGraphics::Texture *CAnmD3DGraphics9::CreateTexture(CAnmDevice *pDevice, int stage)
{
	CAnmD3DTextureSurface9 *pTexture9 = new CAnmD3DTextureSurface9(pDevice, stage);

	return pTexture9;
}

CAnmGraphics::Texture *CAnmD3DGraphics9::CreateTexture(CAnmDevice *pDevice, CAnmTextureData *pTextureData,
		int stage, int width, int height)
{
	CAnmD3DTextureSurface9 *pTexture9 = new CAnmD3DTextureSurface9(pDevice, pTextureData, stage, width, height);

	return pTexture9;
}

CAnmGraphics::Texture *CAnmD3DGraphics9::CreateTexture(CAnmDevice *pDevice, Image *pImage,
		int stage)
{
	CAnmD3DTextureSurface9 *pTexture9 = new CAnmD3DTextureSurface9(pDevice, pImage, stage);

	return pTexture9;
}



CAnmGraphics::Texture *CAnmD3DGraphics9::CreateCubeTexture(CAnmDevice *pDevice, int stage)
{
	CAnmD3DCubeTextureSurface9 *pTexture9 = new CAnmD3DCubeTextureSurface9(pDevice, stage);

	return pTexture9;
}

CAnmGraphics::Texture *CAnmD3DGraphics9::CreateCubeTexture(CAnmDevice *pDevice, CAnmTextureData *pTextureData[6],
		int stage, int pixelDim )
{
	CAnmD3DCubeTextureSurface9 *pTexture9 = new CAnmD3DCubeTextureSurface9(pDevice, pTextureData, stage, pixelDim);

	return pTexture9;
}
/***
CAnmGraphics::Texture *CAnmD3DGraphics9::CreateCubeTexture(CAnmDevice *pDevice, Image *pImage[6],
		int stage)
{
	CAnmD3DCubeTextureSurface9 *pTexture9 = new CAnmD3DCubeTextureSurface9(pDevice, pImage, stage);

	return pTexture9;
}
***/


bool CAnmD3DGraphics9::AddTextureDataToCubeTexture( CAnmDevice *pDevice, Texture *pTextureSurface, int iFace, 
		class CAnmTextureData *pTextureData )
{
	bool bAdded = false;

	if( pDevice &&
		pTextureSurface &&
		pTextureData ) {

		CAnmD3DCubeTextureSurface9* pTexture9 = ( CAnmD3DCubeTextureSurface9* )pTextureSurface;


		bAdded = pTexture9->CopyTextureData( iFace, pTextureData );


	}
	return bAdded;
}




CAnmGraphics::RenderPrimitive *CAnmD3DGraphics9::CreatePrimitive(class CAnmPrimitive *pPrimitive)
{
	CAnmD3DRenderPrimitive9 *pPrimitive9 = new CAnmD3DRenderPrimitive9(pPrimitive);

	return pPrimitive9;
}


CAnmGraphics::Shader *CAnmD3DGraphics9::CreateShader(CAnmShaderProgram *pShaderProgram)
{
	assert(pShaderProgram);

	eAnmShaderType shaderType = pShaderProgram->GetShaderType();

	if (shaderType == eAnmFragmentShader)
	{
		CAnmD3DFragmentShader9 *pFragmentShader9 = new CAnmD3DFragmentShader9(pShaderProgram);
		return pFragmentShader9;
	}
	else if (shaderType == eAnmVertexShader)
	{
		CAnmD3DVertexShader9 *pVertexShader9 = new CAnmD3DVertexShader9(pShaderProgram);
		return pVertexShader9;
	}

	return NULL;
}

void CAnmD3DGraphics9::DrawBox(Point3 corners[8], Color c, CAnmDevice *pDevice)
{
	CAnmD3DDevice9 *pDevice9 = (CAnmD3DDevice9 *) pDevice;
	LPDIRECT3DDEVICE9 pD3DDevice = pDevice9->GetD3DDevice();

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

	DWORD currentFVF;
	pD3DDevice->GetFVF(&currentFVF);
	pD3DDevice->SetFVF(ANMUNLITVERTEXFLAGS);

	pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 15, (void *) &v[0], sizeof(sAnmUnlitVertex));

	if (currentFVF != NULL)
		pD3DDevice->SetFVF(currentFVF);

	pD3DDevice->SetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, oldms);
}

void CAnmD3DGraphics9::DrawAxes(Point3 origin, Point3 axes[3], Color c, CAnmDevice *pDevice)
{
	CAnmD3DDevice9 *pDevice9 = (CAnmD3DDevice9 *) pDevice;
	LPDIRECT3DDEVICE9 pD3DDevice = pDevice9->GetD3DDevice();

	sAnmUnlitVertex v[2];
	ULONG clr = c.MakeDWord();

	DWORD oldms;
	pD3DDevice->GetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, &oldms);
	pD3DDevice->SetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_COLOR1);

	v[0].vec = origin;
	v[0].diffuseColor = clr;
	v[1].diffuseColor = clr;

	DWORD currentFVF;
	pD3DDevice->GetFVF(&currentFVF);
	pD3DDevice->SetFVF(ANMUNLITVERTEXFLAGS);

	for (int i = 0; i < 3; i++)
	{
		v[1].vec = origin + axes[i];
		pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, (void *) &v[0], sizeof(sAnmUnlitVertex));
	}

	if (currentFVF != NULL)
		pD3DDevice->SetFVF(currentFVF);

	pD3DDevice->SetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, oldms);
}


// CAnmD3DTextureSurface9 - texture map class for DX9

// Initialize texture map from a bitmap file
CAnmD3DTextureSurface9::CAnmD3DTextureSurface9(CAnmDevice *pDevice, CAnmTextureData *pTextureData, int stage,
									   int width, int height)
	: CAnmGraphics::Texture(pDevice, stage)
{
	m_pTexture = NULL;
	m_texturedata = pTextureData;
	
	if (width == 0 || height == 0)
		InitSurface(pTextureData->GetWidth(), pTextureData->GetHeight());
	else
		InitSurface(width, height);

	// get the bits into my surface
	CopyTextureData(pTextureData);

}



CAnmD3DTextureSurface9::CAnmD3DTextureSurface9(CAnmDevice *pDevice, Image *pImage, int stage)
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

CAnmD3DTextureSurface9::CAnmD3DTextureSurface9(CAnmDevice *pDevice, int stage)
	: CAnmGraphics::Texture(pDevice, stage)
{
	m_pTexture = NULL;
	m_texturedata = NULL;
}

CAnmD3DTextureSurface9::~CAnmD3DTextureSurface9()
{
	SafeRelease(m_pTexture);
}

// InitSurface - does all the grunt work

void CAnmD3DTextureSurface9::InitSurface(int width, int height, bool mipmap)
{
    // Get the device caps
	CAnmD3DDevice9 *pDevice9 = (CAnmD3DDevice9 *) m_device;
	D3DCAPS9 devdesc = pDevice9->GetD3DDesc();

	HRESULT hr;

	//Use the surface format of the current display mode as the format for the texture.
	//I don't know if this will work but lets give it a go. I think we should do
	//some extra checking/validating with CheckDeviceFormat() - TG
	D3DFORMAT adapterFormat = pDevice9->GetAdapterFormat();
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
	hr = pDevice9->GetD3DDevice()->CreateTexture(w, h, mipmap ? 0 : 1, 
		mipmap ? D3DUSAGE_AUTOGENMIPMAP : 0, D3DFMT_A8R8G8B8 /*adapterFormat*/, 
		D3DPOOL_MANAGED, &m_pTexture, NULL);

	m_width = w;
	m_height = h;
}

void CAnmD3DTextureSurface9::CopyPixels(Image *pImage)
{
	//Lock the texture
	D3DLOCKED_RECT pRect;
	HRESULT hr = m_pTexture->LockRect(0, &pRect, NULL, 0);
	if(FAILED(hr)) {
		// ANMMSG_LOCKRECTFAILED : "LockRect failed"
		throw CAnmError(ANMMSG_LOCKRECTFAILED);
	}

	//Get the texture's surface description:
	D3DSURFACE_DESC desc;
	hr = m_pTexture->GetLevelDesc(0, &desc);
	if(FAILED(hr)) {
		m_pTexture->UnlockRect(0);
		// ANMMSG_GETDESCFAILED : "GetDesc failed"
		throw CAnmError(ANMMSG_GETDESCFAILED);
	}

	//Copy the data in pImage to the surface
	int numBitsInFormat = GetNumBitsInFormat(desc.Format);
	if (numBitsInFormat != 16 && numBitsInFormat != 32)
	{
		m_pTexture->UnlockRect(0);
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


	hr = m_pTexture->UnlockRect(0);
	if(FAILED(hr)) {
		// ANMMSG_UNLOCKRECTFAILED : "UnlockRect failed"
		throw CAnmError(ANMMSG_UNLOCKRECTFAILED);
	}

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

}

#define iR	2	
#define iG	1	
#define iB	0	

void CAnmD3DTextureSurface9::CopyTextureData(CAnmTextureData *pTextureData)
{
	assert(pTextureData);
	assert(pTextureData->ImageValid());

	// Tell the texture surface there was some alpha
	if (pTextureData->HasAlpha())
		SetHasAlpha(true);

	if (pTextureData->IsGrayScale())
		SetIsGrayscale(true);

	//Lock the texture
	D3DLOCKED_RECT pRect;
	HRESULT hr = m_pTexture->LockRect(0, &pRect, NULL, 0);
	if(FAILED(hr)) {
		// ANMMSG_LOCKRECTFAILED : "LockRect failed"
		throw CAnmError(ANMMSG_LOCKRECTFAILED);
	}



	CopyTextureDataToSurface( m_pTexture, pRect, pTextureData,
						m_device, m_grayscale, m_hasalpha );


	hr = m_pTexture->UnlockRect(0);
	if(FAILED(hr)) {
		// ANMMSG_UNLOCKRECTFAILED : "UnlockRect failed"
		throw CAnmError(ANMMSG_UNLOCKRECTFAILED);
	}


	bool mipmap = m_pTexture->GetLevelCount() > 1;

	if (mipmap)
		GenMipMap();
}



void CAnmD3DTextureSurface9::CopyTextureDataToSurface( LPDIRECT3DTEXTURE9 pTexture, 
													  D3DLOCKED_RECT& pRect, CAnmTextureData *pTextureData,
							  class CAnmDevice *pAnmDevice,  bool bgrayscale, bool bhasalpha )
{



	//Get the texture's surface description:
	D3DSURFACE_DESC desc;
	HRESULT hr = pTexture->GetLevelDesc(0, &desc);
	if(FAILED(hr)) {
		pTexture->UnlockRect(0);
		// ANMMSG_GETDESCFAILED : "GetDesc failed"
		throw CAnmError(ANMMSG_GETDESCFAILED);
	}

	//Copy the data in pImage to the surface
	int numBitsInFormat = GetNumBitsInFormat(desc.Format);
	if (numBitsInFormat != 16 && numBitsInFormat != 32)
	{
		pTexture->UnlockRect(0);
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
}


void CAnmD3DCubeTextureSurface9::CopyTextureDataToSurface( LPDIRECT3DCUBETEXTURE9 pTexture, D3DCUBEMAP_FACES iface,
													  D3DLOCKED_RECT& pRect, CAnmTextureData *pTextureData,
							  class CAnmDevice *pAnmDevice,  bool bgrayscale, bool bhasalpha )
{



	//Get the texture's surface description:
	D3DSURFACE_DESC desc;
	HRESULT hr = pTexture->GetLevelDesc(0, &desc);
	if(FAILED(hr)) {
		pTexture->UnlockRect(iface, 0);
		// ANMMSG_GETDESCFAILED : "GetDesc failed"
		throw CAnmError(ANMMSG_GETDESCFAILED);
	}

	//Copy the data in pImage to the surface
	int numBitsInFormat = CAnmD3DTextureSurface9::GetNumBitsInFormat(desc.Format);
	if (numBitsInFormat != 16 && numBitsInFormat != 32)
	{
		pTexture->UnlockRect(iface, 0);
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
}


















void CAnmD3DTextureSurface9::GenMipMap()
{
	m_pTexture->GenerateMipSubLevels();
}

int CAnmD3DTextureSurface9::GetNumBitsInFormat(D3DFORMAT format)
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

// CAnmD3DTextureSurface9 - texture map class for DX7

// Initialize texture map from a bitmap file
CAnmD3DCubeTextureSurface9::CAnmD3DCubeTextureSurface9(CAnmDevice *pDevice, CAnmTextureData *pTextureData[6], int stage,
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
CAnmD3DCubeTextureSurface9::CAnmD3DCubeTextureSurface9(CAnmDevice *pDevice, Image *pImage, int stage)
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


CAnmD3DCubeTextureSurface9::CAnmD3DCubeTextureSurface9(CAnmDevice *pDevice, int stage)
	: CAnmGraphics::Texture(pDevice, stage)
{
	m_iPixelDim = 0;
	m_pTexture = NULL;
	m_texturedata = NULL;
}

CAnmD3DCubeTextureSurface9::~CAnmD3DCubeTextureSurface9()
{
	SafeRelease(m_pTexture);
}

// InitSurface - does all the grunt work

void CAnmD3DCubeTextureSurface9::InitSurface( int pixelDim, bool mipmap )
{
	m_iPixelDim = pixelDim;
	int width = pixelDim;
	int height = pixelDim;
    // Get the device caps
	CAnmD3DDevice9 *pDevice9 = (CAnmD3DDevice9 *) m_device;
	D3DCAPS9 devdesc = pDevice9->GetD3DDesc();

	HRESULT hr;

	//Use the surface format of the current display mode as the format for the texture.
	//I don't know if this will work but lets give it a go. I think we should do
	//some extra checking/validating with CheckDeviceFormat() - TG
	D3DFORMAT adapterFormat = pDevice9->GetAdapterFormat();
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
	hr = pDevice9->GetD3DDevice()->CreateCubeTexture( w, mipmap ? 0 : 1, 0, D3DFMT_A8R8G8B8 /*adapterFormat*/, 
		D3DPOOL_MANAGED , &m_pTexture, NULL );	

	m_width = w;
	m_height = h;
}


bool CAnmD3DCubeTextureSurface9::CopyTextureData( int iFace, CAnmTextureData *pTextureData)
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
		D3DLOCKED_RECT LockedRect;
		HRESULT hr = m_pTexture->LockRect( (D3DCUBEMAP_FACES)iFace, 0, &LockedRect, NULL, 0 );
		if(FAILED(hr)) {
			// ANMMSG_GETSURFACELEVELFAILED : "GetSurfaceLevel failed"
			throw CAnmError(ANMMSG_GETSURFACELEVELFAILED);
		}


		CopyTextureDataToSurface( m_pTexture, (D3DCUBEMAP_FACES)iFace, LockedRect, pTextureData, 
			m_device, m_grayscale, m_hasalpha );

		m_pTexture->UnlockRect((D3DCUBEMAP_FACES)iFace, 0);


		bTextureAdded = true;
	//	bool mipmap = false; //m_pTexture->GetLevelCount() > 1;
	//	if (mipmap)
	//		GenMipMap();
	}
	return bTextureAdded;
}




















// CAnmD3DRenderPrimitive9 - handles all Draw Primitive work for DX9
CAnmD3DRenderPrimitive9::CAnmD3DRenderPrimitive9(CAnmPrimitive *pPrimitive)
: CAnmGraphics::RenderPrimitive(pPrimitive)
{
	m_vertexBuffer = NULL;
	m_pMesh = NULL;

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

CAnmD3DRenderPrimitive9::~CAnmD3DRenderPrimitive9()
{
	SafeRelease(m_vertexBuffer);
	SafeRelease(m_pMesh);
}

void CAnmD3DRenderPrimitive9::DestroyMesh()
{
	if(m_pMesh != NULL) {
		HRESULT hr = m_pMesh->Release();
		if(FAILED(hr)) {
			// ANMMSG_COULDNTDESTROYMESH : "Could not destroy mesh"
			throw CAnmError(ANMMSG_COULDNTDESTROYMESH);
		}
		m_pMesh = NULL;
	}
}

HRESULT CAnmD3DRenderPrimitive9::BuildMesh(LPDIRECT3DDEVICE9 pD3DDevice)
{
		tAnmVertexArray *pVerts = m_primitive->GetVertices();
		assert(pVerts);

		int numVertices = pVerts->size();

		int numTriangles = m_primitive->NumTriangles();

		HRESULT hr;
	//if(m_pMesh == NULL) {
		//create the mesh:		
		hr = D3DXCreateMesh(numTriangles, numVertices, D3DXMESH_MANAGED,
			dwVertexDecl2, pD3DDevice, &m_pMesh);
		if(FAILED(hr))
			// ANMMSG_COULDNTCREATEMESH : "Mesh Creation Failed"
			throw CAnmError(ANMMSG_COULDNTCREATEMESH);

		//Fill the vertex buffer of the mesh:
		sAnmVertex2 *pVertices = new sAnmVertex2[numVertices];
		hr = m_pMesh->LockVertexBuffer(0, (VOID**)&pVertices);
		if(FAILED(hr))
			// ANMMSG_COULDNTLOCKMESHVB : "Locking mesh vertex buffer failed"
			throw CAnmError(ANMMSG_COULDNTLOCKMESHVB);
		
		for(int i=0; i<numVertices; i++) {
			pVertices[i].vec = (*pVerts)[i].vec;
			pVertices[i].normal = (*pVerts)[i].normal;
			pVertices[i].diffuseColor = (*pVerts)[i].diffuseColor;
			pVertices[i].texCoord = (*pVerts)[i].texCoord;
		}
		hr = m_pMesh->UnlockVertexBuffer();
		if(FAILED(hr))
			// ANMMSG_COULDNTUNLOCKMESHVB : "Could not unlock mesh vertex buffer"
			throw CAnmError(ANMMSG_COULDNTUNLOCKMESHVB);

		//Fill the index buffer of the mesh:
		if(m_primitive->IsIndexed()) {
			WORD* pIBIndices;
			hr = m_pMesh->LockIndexBuffer(0, (VOID**)&pIBIndices);
			if(FAILED(hr)) 
				// ANMMSG_COULDNTLOCKMESHIB : "Locking mesh index buffer failed"
				throw CAnmError("Locking mesh index buffer failed");

			tAnmIndexArray *pIndices = m_primitive->GetIndices();
			assert(pIndices);

			int numIndices = pIndices->size();

			for(i=0; i<numIndices; i++) {
				pIBIndices[i] = (*pIndices)[i];
			}

			hr = m_pMesh->UnlockIndexBuffer();

			if(FAILED(hr))
				// ANMMSG_COULDNTUNLOCKMESHIB : "Could not unlock mesh index buffer"
				throw CAnmError(ANMMSG_COULDNTUNLOCKMESHIB);
		}

		//compute the normals and binormals:
		//For some reason D3DXComputeTangent fails for some geometry, example: somewhere
		//in ACBM and somewhere inside the Lara Croft model.
		//For the time being we will only compute the tangents where we are using shaders
		//and keep our fingers crossed that D3DXComputeTangent works - eeekk. Note to self: Remember this
		//is only a prototype so don't get too upset if its not perfect TG.
		LPDIRECT3DVERTEXSHADER9 vertexShader;
		pD3DDevice->GetVertexShader(&vertexShader);
		if(vertexShader != NULL) {
			hr = D3DXComputeTangent(m_pMesh, 0, 1, 2, TRUE, NULL);
			
			if(hr == D3DERR_INVALIDCALL)
				// ANMMSG_COMPUTETANGENTINVCALL : "Compute tangent failed, invalid call"
				throw CAnmError(ANMMSG_COMPUTETANGENTINVCALL);

			if(hr == E_OUTOFMEMORY) 
				// ANMMSG_COMPUTETANGENTOUTOFMEM : "Compute tangent failed, out of memory"
				throw CAnmError(ANMMSG_COMPUTETANGENTOUTOFMEM);
		}
	//}

	return S_OK;
}


void CAnmD3DRenderPrimitive9::Buffer(class CAnmDevice *pDevice)
{
	assert(m_d3dprimtype != D3DPT_LINESTRIP && m_d3dprimtype != D3DPT_POINTLIST);

	assert(pDevice);

	// only do this on hardware-accelerated devices
	if (!pDevice->IsHardwareAccelerated())
	{
		// N.B.: generate warning? who cares...
		return;
	}

	assert(m_primitive);

	tAnmVertexArray *pVerts = m_primitive->GetVertices();
	assert(pVerts);

	HRESULT hr;

	CAnmD3DDevice9 *pDevice9 = (CAnmD3DDevice9 *) pDevice;
	LPDIRECT3DDEVICE9 pD3DDevice = pDevice9->GetD3DDevice();
	assert(pD3DDevice);

	hr = pD3DDevice->CreateVertexBuffer(sizeof(sAnmVertex)*pVerts->size(), D3DUSAGE_WRITEONLY, 
									ANMVERTEXFLAGS, D3DPOOL_MANAGED, &m_vertexBuffer, NULL);
	if (FAILED(hr))
	{
		// ANMMSG_VBCREATEFAILED : "Failed to create vertex buffer"
		// N.B.: generate warning? who cares...
		// pDevice->GetApp()->Warn(ANMMSG_VBCREATEFAILED);

		return;
	}

	CopyVerticesToBuffer(pVerts);
}

void CAnmD3DRenderPrimitive9::UpdateBuffer()
{
	assert(m_d3dprimtype != D3DPT_LINESTRIP && m_d3dprimtype != D3DPT_POINTLIST);

	if (m_vertexBuffer == NULL)
		return;

	assert(m_primitive);


	tAnmVertexArray *pVerts = m_primitive->GetVertices();
	assert(pVerts);

	CopyVerticesToBuffer(pVerts);
}

void CAnmD3DRenderPrimitive9::CopyVerticesToBuffer(tAnmVertexArray *pVerts)
{
	assert(m_vertexBuffer);

	HRESULT hr;

	// lock the buffer for write
	sAnmVertex *pData = NULL;

	hr = m_vertexBuffer->Lock(0, 0, (void **)&pData, 0);

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


void CAnmD3DRenderPrimitive9::Render(class CAnmDevice *pDevice)
{
	assert(pDevice);
	assert(m_primitive);

	LPDIRECT3DDEVICE9 pD3DDevice = ((CAnmD3DDevice9 *)pDevice)->GetD3DDevice();
	HRESULT hr;

	// See if there's a vertex shader, otherwise use fixed function pipeline
	LPDIRECT3DVERTEXSHADER9 vertexShader;
	pD3DDevice->GetVertexShader(&vertexShader);
	if (vertexShader != NULL)
	{
		//using a vertex shader
		LPDIRECT3DVERTEXDECLARATION9 pVertexDecl;
		hr = pD3DDevice->CreateVertexDeclaration(dwVertexDecl2, &pVertexDecl);
		if(FAILED(hr)) throw CAnmError("Could not create vertex declaration");
		hr = pD3DDevice->SetVertexDeclaration(pVertexDecl);
		if(FAILED(hr)) throw CAnmError("Could not set vertex declaration");
	}

	LPDIRECT3DPIXELSHADER9 pixelShader;
	pD3DDevice->GetPixelShader(&pixelShader);
	DWORD oldlightstate;
	if(pixelShader == NULL) {
		//turn on lighting -- no, this should be done by someone else
		// pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	else {
		//turn lighting off - let the pixel shader handle lighting the scene
		pD3DDevice->GetRenderState(D3DRS_LIGHTING, &oldlightstate);
		pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
	
	bool lit = m_primitive->IsLit();
	bool indexed = m_primitive->IsIndexed();

	if (lit)
	{
		if (vertexShader == NULL)
			pD3DDevice->SetFVF(ANMVERTEXFLAGS2);		

		if(m_pMesh == NULL) {
			BuildMesh(pD3DDevice);
		}

		//Get the device associated with the mesh:
		LPDIRECT3DDEVICE9 meshDevice;
		hr = m_pMesh->GetDevice(&meshDevice);
		if(FAILED(hr)) {
			throw CAnmError("GetDevice on mesh object failed");
		}
		if(meshDevice != pD3DDevice) {
			//the original device may have been lost somewhere. We need to
			//rebuild the mesh.
			DestroyMesh();
			BuildMesh(pD3DDevice);
		}
		meshDevice->Release();


		//Render the Mesh:
		LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
		hr = m_pMesh->GetVertexBuffer(&pVB);
		if(FAILED(hr)) throw CAnmError("Could not get mesh vertex buffer");

		DWORD numBytes = m_pMesh->GetNumBytesPerVertex();
		hr = pD3DDevice->SetStreamSource( 0, pVB, 0, numBytes);
		if(FAILED(hr)) {
			if(pVB != NULL)
				pVB->Release();
			throw CAnmError("Could not set stream source");
		}
		
		LPDIRECT3DINDEXBUFFER9 pIB=NULL;
		if(indexed) {		
			hr = m_pMesh->GetIndexBuffer(&pIB);
			if(FAILED(hr)) {
				if(pVB != NULL)
					pVB->Release();
				throw CAnmError("Could not get mesh index buffer");
			}		
			hr = pD3DDevice->SetIndices( pIB );
			if(FAILED(hr)) {
				if(pVB != NULL)
					pVB->Release();
				if(pIB != NULL)
					pIB->Release();
				throw CAnmError("Could not set indices");
			}
		}

		int numTriangles = m_pMesh->GetNumFaces();
		int numVertices = m_pMesh->GetNumVertices();
		if(indexed) {
			hr = pD3DDevice->DrawIndexedPrimitive(m_d3dprimtype, 0, 0, numVertices, 0, numTriangles);
		}
		else {
			hr = pD3DDevice->DrawPrimitive(m_d3dprimtype, 0, numTriangles);
		}
	}
	else		// !lit
	{
		if (vertexShader == NULL)
			pD3DDevice->SetFVF(ANMUNLITVERTEXFLAGS);		

		CAnmUnlitPrimitive *pUnlitPrimitive = (CAnmUnlitPrimitive *) m_primitive;

		tAnmUnlitVertexArray *pVerts = pUnlitPrimitive->GetUnlitVertices();
		assert(pVerts);

		int nVerts = pVerts->size();

		D3DMATERIAL9 mat;
		DWORD oldedgeantialias;
		HRESULT hr;
		
		pD3DDevice->GetMaterial(&mat);
		D3DCOLORVALUE oldDiffuseColor = mat.Diffuse;
		// Just turn diffuse color off, we're supposed to ignore it -- TP 12-10-03
		mat.Diffuse = * (D3DCOLORVALUE *) &color4::GetBlack();

		hr = pD3DDevice->SetMaterial(&mat);

		// For line sets, set up edge antialiasing
		if (m_d3dprimtype == D3DPT_LINESTRIP)
		{
			hr = pD3DDevice->GetRenderState(D3DRS_ANTIALIASEDLINEENABLE, &oldedgeantialias);
			hr = pD3DDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
		}

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

		mat.Diffuse = oldDiffuseColor;
		hr = pD3DDevice->SetMaterial(&mat);

		// For line sets, restore edge antialiasing
		if (m_d3dprimtype == D3DPT_LINESTRIP)
		{
			hr = pD3DDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, oldedgeantialias);
		}
	}


	if(pixelShader != NULL) {
		pD3DDevice->SetRenderState(D3DRS_LIGHTING, oldlightstate);
	}
}

// Shaders for DX9
// CAnmD3DFragmentShader9

CAnmD3DFragmentShader9::CAnmD3DFragmentShader9(CAnmShaderProgram *pShaderProgram) : CAnmGraphics::FragmentShader(pShaderProgram)
{
	m_pixelShader = NULL;
}

CAnmD3DFragmentShader9::~CAnmD3DFragmentShader9()
{
	SafeRelease(m_pixelShader);
}

// CAnmGraphics::FragmentShader overrides
void CAnmD3DFragmentShader9::CompileString(cApplication *pApp, const char *source)
{
	char *profile = NULL;
}

void CAnmD3DFragmentShader9::CompileFile(cApplication *pApp, const char *filename)
{
	String profile = NULL;
	char *profilestr = NULL;

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	CAnmD3DDevice9 *pDevice9 = (CAnmD3DDevice9 *) pDevice;

	profile = pDevice9->GetPixelShaderProfile();

	if (profile)
		profilestr = profile->GetBuf();


	HRESULT hr;
	LPD3DXBUFFER pCode = NULL; // points to the assembled shader code
	
	// N.B.: have to assume "main" for now as there is no way (in the spec) to specify an
	// arbitrarily named entry point

	const char *entryPoint = "main";

	hr = D3DXCompileShaderFromFile(filename, NULL, NULL, entryPoint, profilestr, NULL, &pCode, NULL, &m_constantTable.m_pConstants);

	if (FAILED(hr))
	{
		// do something
		char *cp = AnmD3DError(hr);
		return;
	}

	LPDIRECT3DDEVICE9 pD3DDevice = pDevice9->GetD3DDevice();
	
	hr = pD3DDevice->CreatePixelShader ((DWORD*)pCode->GetBufferPointer(), &m_pixelShader);

	if (FAILED(hr))
	{
		// do something
		char *cp = AnmD3DError(hr);
		return;
	}

}

eAnmReturnStatus CAnmD3DFragmentShader9::SetValue(char *valuename, eValueType valueType, void *pValue)
{
	return m_constantTable.SetValue(valuename, valueType, pValue);
}

void CAnmD3DFragmentShader9::SetupConstants(CAnmD3DDevice9 *pDevice9)
{
	assert(m_shaderProgram);

	m_constantTable.SetDevice(pDevice9);


	matrix4 viewMat;
	pDevice9->GetViewMatrix(&viewMat);

	Point3 camPosition = viewMat * point3(0, 0, 0);
	camPosition.z = - camPosition.z;

	m_constantTable.SetValue("cameraPosition", eValuePoint3, (void *) &camPosition);

	m_shaderProgram->CopyValuesToShader();
}

// CAnmD3DVertexShader9

CAnmD3DVertexShader9::CAnmD3DVertexShader9(CAnmShaderProgram *pShaderProgram) : CAnmGraphics::VertexShader(pShaderProgram)
{
	m_vertexShader = NULL;
}

CAnmD3DVertexShader9::~CAnmD3DVertexShader9()
{
	SafeRelease(m_vertexShader);
}

// CAnmGraphics::FragmentShader overrides
void CAnmD3DVertexShader9::CompileString(cApplication *pApp, const char *source)
{
	char *profile = NULL;
}

void CAnmD3DVertexShader9::CompileFile(cApplication *pApp, const char *filename)
{
	String profile = NULL;
	char *profilestr = NULL;

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	CAnmD3DDevice9 *pDevice9 = (CAnmD3DDevice9 *) pDevice;

	profile = pDevice9->GetVertexShaderProfile();

	if (profile)
		profilestr = profile->GetBuf();


	HRESULT hr;
	LPD3DXBUFFER pCode = NULL; // points to the assembled shader code
	
	// N.B.: have to assume "main" for now as there is no way (in the spec) to specify an
	// arbitrarily named entry point

	const char *entryPoint = "main";

	hr = D3DXCompileShaderFromFile(filename, NULL, NULL, entryPoint, profilestr, NULL, &pCode, NULL, &m_constantTable.m_pConstants);

	if (FAILED(hr))
	{
		// do something
		char *cp = AnmD3DError(hr);
		return;
	}

	LPDIRECT3DDEVICE9 pD3DDevice = pDevice9->GetD3DDevice();
	
	hr = pD3DDevice->CreateVertexShader ((DWORD*)pCode->GetBufferPointer(), &m_vertexShader);

	if (FAILED(hr))
	{
		// do something
		char *cp = AnmD3DError(hr);
		return;
	}
}

eAnmReturnStatus CAnmD3DVertexShader9::SetValue(char *valuename, eValueType valueType, void *pValue)
{
	return m_constantTable.SetValue(valuename, valueType, pValue);
}

void CAnmD3DVertexShader9::SetupConstants(CAnmD3DDevice9 *pDevice9)
{
	assert(m_shaderProgram);

	m_constantTable.SetDevice(pDevice9);

	// Do mandatory params

	//set the modelViewProj matrix for each frame - This must be done before binding the shaders
	//get the model/world, view and projection matrices:
	D3DXMATRIX matModel;
	pDevice9->GetWorldMatrix(&matModel);
	D3DXMATRIX matView;
	pDevice9->GetViewMatrix(&matView);
	D3DXMATRIX matProj;
	pDevice9->GetProjectionMatrix(&matProj);

	//Bind the model view projection matrix
	//Note that it is assumed that "modelViewProj" will be a parameter in main method of vertex shader
	D3DXMATRIX modelViewProj;
	D3DXMatrixMultiply(&modelViewProj, &matModel, &matView);
	D3DXMatrixMultiplyTranspose(&modelViewProj, &modelViewProj, &matProj);

	m_constantTable.SetMatrix("modelViewProj", &modelViewProj);

	//Bind the modelToWorld matrix
	//Note that it is assumed that "modelToWorld" will be a parameter in main method of vertex shader

	D3DXMATRIX matModelToWorldColumnMajor;
	D3DXMatrixTranspose(&matModelToWorldColumnMajor, &matModel);
	D3DXMATRIX matModelToWorldColumnMajor2(matModelToWorldColumnMajor);

	m_constantTable.SetMatrix("modelToWorld", &matModelToWorldColumnMajor2);

	//Bind the modelToWorldInvTrans matrix
	//Note that it is assumed that "modelToWorldInvTrans" will be a parameter in main method of vertex shader


	D3DXMATRIX matModelToWorldColumnMajorInverse;
	//I should check to make sure that the inverse can be calculated!! i.e. the determinant != 0
	D3DXMatrixInverse(&matModelToWorldColumnMajorInverse, NULL, &matModelToWorldColumnMajor);
	D3DXMATRIX matModelToWorldColumnMajorInverseTranspose;
	D3DXMatrixTranspose(&matModelToWorldColumnMajorInverseTranspose, &matModelToWorldColumnMajorInverse);
	D3DXMATRIX matModelToWorldColumnMajorInverseTranspose2(matModelToWorldColumnMajorInverseTranspose);

	m_constantTable.SetMatrix("modelToWorldInvTrans", &matModelToWorldColumnMajorInverseTranspose2);

	matrix4 viewMat;
	pDevice9->GetViewMatrix(&viewMat);

	Point3 camPosition = viewMat * point3(0, 0, 0);
	camPosition.z = - camPosition.z;

	m_constantTable.SetValue("cameraPosition", eValuePoint3, (void *) &camPosition);

/*
	m_constantTable.SetMatrix("model", &model);
	m_constantTable.SetMatrix("modelView", &modelView);
	m_constantTable.SetMatrix("modelViewProjection", &modelViewProjection);
*/

	// Now do optional ones
	m_shaderProgram->CopyValuesToShader();
}

// Constant table help
CAnmDX9ConstantTable::CAnmDX9ConstantTable()
{
	m_pConstants = NULL;
	m_pDevice9 = NULL;
}

CAnmDX9ConstantTable::~CAnmDX9ConstantTable()
{
	SafeRelease(m_pConstants);
}

void CAnmDX9ConstantTable::SetDevice(CAnmD3DDevice9 *pDevice9)
{
	m_pDevice9 = pDevice9;
}

/* 
typedef enum D3DXPARAMETER_TYPE
{
    D3DXPT_VOID,
    D3DXPT_BOOL,
    D3DXPT_INT,
    D3DXPT_FLOAT,
    D3DXPT_STRING,
    D3DXPT_TEXTURE,
    D3DXPT_TEXTURE1D,
    D3DXPT_TEXTURE2D,
    D3DXPT_TEXTURE3D,
    D3DXPT_TEXTURECUBE,
    D3DXPT_SAMPLER,
    D3DXPT_SAMPLER1D,
    D3DXPT_SAMPLER2D,
    D3DXPT_SAMPLER3D,
    D3DXPT_SAMPLERCUBE,
    D3DXPT_PIXELSHADER,
    D3DXPT_VERTEXSHADER,
    D3DXPT_PIXELFRAGMENT,
    D3DXPT_VERTEXFRAGMENT,
    D3DXPT_FORCE_DWORD = 0x7fffffff,
*/

eAnmReturnStatus CAnmDX9ConstantTable::SetValue(char *valuename, eValueType valueType, void *pValue)
{
	assert(m_pConstants != NULL);
	assert(m_pDevice9 != NULL);

	D3DXHANDLE handle;

	handle = m_pConstants->GetConstantByName(NULL, valuename);
	if (handle == NULL)
		return eAnmReturnAllBad;

	D3DXCONSTANT_DESC desc;
	unsigned int descCount = 1;

	m_pConstants->GetConstantDesc(handle, &desc, &descCount);
	D3DXPARAMETER_TYPE paramType = desc.Type;
	D3DXPARAMETER_CLASS paramClass = desc.Class;

	LPDIRECT3DDEVICE9 pD3DDevice9 = m_pDevice9->GetD3DDevice();
	assert(pD3DDevice9);


	switch (valueType)
	{
		case eValueColor :
			{
				Color *c;

				c = ((Color *) pValue);
				
				if (paramType == D3DXPT_FLOAT && paramClass == D3DXPC_VECTOR)
				{
					color4 c4 = *c;
					m_pConstants->SetVector(pD3DDevice9, handle, (D3DXVECTOR4 *) &c4);
				}				
			}
			break;

		case eValuePoint3 :
			{
				Point3 *p;
				p = ((Point3 *) pValue);

				if (paramType == D3DXPT_FLOAT && paramClass == D3DXPC_VECTOR)
				{
					float f4[4] = {p->x, p->y, p->y, 1};
					m_pConstants->SetVector(pD3DDevice9, handle, (D3DXVECTOR4 *) f4);
				}				
			}
			break;

		case eValueFloat :
			{
				float *pF;
				pF = ((float *) pValue);

				if (paramType == D3DXPT_FLOAT && paramClass == D3DXPC_SCALAR)
				{
					int debug =1;
				}				
			}
			break;

		case eValueString :
			{
			}
			break;

		default :
			break;

		}	

	return eAnmReturnAllGood;
}

HRESULT CAnmDX9ConstantTable::SetMatrix(char *matrixName, D3DXMATRIX *pMatrix)
{
	assert(m_pConstants != NULL);
	assert(m_pDevice9 != NULL);

	D3DXHANDLE handle;

	handle = m_pConstants->GetConstantByName(NULL, matrixName);
	if (handle == NULL)
		return E_FAIL;

	D3DXCONSTANT_DESC desc;
	unsigned int descCount = 1;

	HRESULT hr = m_pConstants->GetConstantDesc(handle, &desc, &descCount);

	if (!SUCCEEDED(hr))
		return E_FAIL;

	D3DXPARAMETER_TYPE paramType = desc.Type;
	D3DXPARAMETER_CLASS paramClass = desc.Class;

	LPDIRECT3DDEVICE9 pD3DDevice9 = m_pDevice9->GetD3DDevice();
	assert(pD3DDevice9);

	m_pConstants->SetMatrix(pD3DDevice9, handle, pMatrix);

	return S_OK;

}

