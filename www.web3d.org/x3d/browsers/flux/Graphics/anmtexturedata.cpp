/********************************************************************************
 * Flux
 *
 * File: anmtexturedata.cpp
 * Description: TextureData class - loads image
 *							data for texture handling
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
#include "anmtexturedata.h"
#include "anmdevice.h"

// TP: don't include old headers - eventually remove
//#include "bitimage.h"
//#include "pngpvt.h"

// krv:  Include for new image IO 
#include "imageIO.h"


#include <fstream>
using namespace std ;

CAnmTextureData::CAnmTextureData(const char *filename, eAnmTextureDataFormat format)
{
	m_filename = new CAnmString(filename);
	m_format = format;
	m_width = 0;
	m_height = 0;
	m_hasalpha = FALSE;
	// m_hastransparentcolor = FALSE;
	m_grayscale = FALSE;
	m_imagevalid = FALSE;
	m_pPixelData = NULL;
	m_mipmap = true;
}


CAnmTextureData::CAnmTextureData( int width, int height, 
								 bool bHasAlpha, bool bGrey, bool mipmap ) :
	m_format( eAnmTextureGenerated ),
	m_width( width ),
	m_height( height ),
	m_hasalpha( bHasAlpha ),
	m_grayscale( bGrey ),
	m_imagevalid( TRUE ),
	m_mipmap(mipmap)
{
	m_filename = new CAnmString("");
	int nDepth = ( bGrey ) ? 1 : 3;
	if( bHasAlpha ) {
		nDepth++;
	}
	m_pPixelData = new unsigned char[m_width * m_height * nDepth];
}


CAnmTextureData::~CAnmTextureData()
{
	SafeDelete(m_filename);
	if( m_pPixelData ) {
		delete [] m_pPixelData;
	}
}

CAnmTextureData *CAnmTextureData::FindTextureData(cApplication *pApp, const char *filename)
{
	tAnmTextureDataArray *pDataArray = pApp->GetCachedTextureData();
	assert(pDataArray);

	for (int i = 0; i < pDataArray->size(); i++)
	{
		if (*((*pDataArray)[i]->GetFilename()) == filename)
			return (*pDataArray)[i];
	}

	return NULL;
}

CAnmTextureData *CAnmTextureData::LoadTextureData(cApplication *pApp, const char *filename, eAnmTextureDataFormat format)
{
	assert(pApp);

	CAnmTextureData *pTextureData = NULL;

	if ((pTextureData = FindTextureData(pApp, filename)) != NULL)
		return pTextureData;
	else
	{
		switch (format)
		{
			case eAnmTextureBMP :
				pTextureData = new CAnmBitmapTextureData(filename);
				break;
			case eAnmTextureJPG :
				pTextureData = new CAnmJPEGTextureData(filename);
				break;
			case eAnmTexturePNG :
				pTextureData = new CAnmPNGTextureData(filename);
				break;
			case eAnmTextureGIF :
				pTextureData = new CAnmGIFTextureData(filename);
				break;
			default :
				assert(FALSE);
				break;
		}

		if (!pTextureData->ImageValid())
			SafeDelete(pTextureData);

		if (pTextureData)
		{
			tAnmTextureDataArray *pDataArray = pApp->GetCachedTextureData();
			assert(pDataArray);

			pDataArray->push_back(pTextureData);
		}

		return pTextureData;
	}
}

// CAnmBitmapTextureData - handle Windows bitmap image type

CAnmBitmapTextureData::CAnmBitmapTextureData(const char *filename)
 : CAnmTextureData(filename, eAnmTextureBMP)
{
	// krv: read the bitmap file.
	//
	int width = 0;
	int height = 0;
	unsigned char* pPixelData = read_bmp( filename, &width, &height );

	if( pPixelData && width && height ) {
		// Good Texture Data, set our texture data
		m_width = width;
		m_height = height;
		m_pPixelData = pPixelData;
		m_hasalpha = FALSE;
		m_grayscale = FALSE;
		m_imagevalid = TRUE;
	}
}

// CAnmJPEGTextureData - handle JPEG image type

CAnmJPEGTextureData::CAnmJPEGTextureData(const char *filename)
 : CAnmTextureData(filename, eAnmTextureJPG)
{

	// krv: read the jpg file.
	//
	int width = 0;
	int height = 0;
	unsigned char* pPixelData = read_jpg_IJG( filename, &width, &height );

	if( pPixelData && width && height ) {
		// Good Texture Data, set our texture data
		m_width = width;
		m_height = height;
		m_pPixelData = pPixelData;
		m_hasalpha = FALSE;
		m_grayscale = FALSE;
		m_imagevalid = TRUE;
	}
}

// CAnmPNGTextureData - handle JPEG image type

CAnmPNGTextureData::CAnmPNGTextureData(const char *filename)
 : CAnmTextureData(filename, eAnmTexturePNG)
{

	// krv: read the png file.
	//
	int width = 0;
	int height = 0;
	int nBytesPerPixel = 0;
	unsigned char* pPixelData = read_png_png( filename, &width, &height, &nBytesPerPixel );

	if( pPixelData && width && height ) {
		// Good Texture Data, set our texture data
		m_width = width;
		m_height = height;
		m_pPixelData = pPixelData;
		m_hasalpha = ( nBytesPerPixel > 3 || nBytesPerPixel == 2 );
		m_grayscale = ( nBytesPerPixel < 3 );
		m_imagevalid = TRUE;
	}
}


CAnmGIFTextureData::CAnmGIFTextureData(const char *filename)
 : CAnmTextureData(filename, eAnmTextureGIF)
{

	// krv: read the GIF file.
	//
	int width = 0;
	int height = 0;
	int nBytesPerPixel = 0;
	unsigned char* pPixelData = read_gif( filename, &width, &height, &nBytesPerPixel );

	if( pPixelData && width && height ) {
		// Good Texture Data, set our texture data
		m_width = width;
		m_height = height;
		m_pPixelData = pPixelData;
		m_hasalpha = ( nBytesPerPixel > 3 || nBytesPerPixel == 2 );
		m_grayscale = ( nBytesPerPixel < 3 );
		m_imagevalid = TRUE;
	}
}

