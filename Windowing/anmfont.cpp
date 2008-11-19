/********************************************************************************
 * Flux
 *
 * File: anmfont.cpp
 * Description: Font class - abstracts Windows fonts
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
#include "anmfont.h"
#include "anmtextglyph.h"
#include "anmworld.h"
#include "AnimaApp.h"
#include "anmtexturedata.h"




CAnmFont::CAnmFont(const char *facename, eFontFamily family, eFontStyle style, int iPointSize )
{

	if( iPointSize <= 0 ) {
		iPointSize = ANMFONT_POINTSIZE;
	}

	m_facename = new CAnmString(facename);
	m_family = family;
	m_style = style;
	m_iPointSize = iPointSize;

	m_pTextureData = NULL;
	m_pTextureCoords = NULL;
	m_textureSurface = NULL;


	m_hDC = NULL;
	m_hFont = MakeFont(facename, family, style, iPointSize );
	m_hOldFont = (HFONT) SelectObject(m_hDC, m_hFont);

	GetTextMetrics(m_hDC, &m_textMetrics);

	for (int i = 0; i < ANMGLYPHCACHESIZE; i++)
		m_glyphcache[i] = NULL;
}

CAnmFont::~CAnmFont()
{
	if (m_hOldFont)
		SelectObject(m_hDC, m_hOldFont);

	if (m_hDC)
		ReleaseDC(NULL, m_hDC);

	if (m_hFont)
		DeleteObject(m_hFont);

	if (m_pTextureCoords)
		delete [] m_pTextureCoords;

	if (m_pTextureData)
		delete m_pTextureData;

	SafeUnRef( m_textureSurface );


	for (int i = 0; i < ANMGLYPHCACHESIZE; i++)
		SafeUnRef(m_glyphcache[i]);

	SafeUnRef(m_facename);
}

CAnmFont *CAnmFont::FindFont(const char *facename, eFontFamily family,
							 eFontStyle style, int iPointSize, class cApplication * pApp )
{

	if( iPointSize <= 0 ) {
		iPointSize = ANMFONT_POINTSIZE;
	}

	CAnmFont *pFont = NULL;

	// Lookup font in our list; linear for now, should be OK

	tAnmFontArray* pFontcache = NULL;
	if( pApp ) {
		pFontcache = pApp->GetFontCache();
	}


	if( pFontcache ) {

		int sz = pFontcache->size();

		for (int i = 0; i < sz; i++)
		{
			pFont = (*pFontcache)[i];
			if (pFont->GetFamily() == family &&
				pFont->GetStyle() == style &&
				pFont->GetPointSize() == iPointSize )
			{
				const char *fname = pFont->GetFaceName()->GetBuf();
				if (!strcmp(fname, facename))
					return pFont;
			}
		}
	}

	// Didn't find it? Make a new one and add it to the list
	pFont = new CAnmFont(facename, family, style, iPointSize);

	if( pFontcache ) {
		pFontcache->push_back(pFont);
	}

	return pFont;
}

HFONT CAnmFont::MakeFont(const char *facename, eFontFamily family, eFontStyle style, int iPointSize )
{
	bool bNeedTTFont = false;
	if( iPointSize <= 0 ) {
		iPointSize = ANMFONT_POINTSIZE;
	}

	if( iPointSize == ANMFONT_POINTSIZE ) {
		bNeedTTFont = true;
	}


	m_hDC = CreateCompatibleDC(NULL);
	
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));

	int logpixy = GetDeviceCaps(m_hDC, LOGPIXELSY);
	logFont.lfHeight = -MulDiv(iPointSize, logpixy, 72);
	logFont.lfCharSet = ANSI_CHARSET;						// N.B. eventually deal with this
//	logFont.lfQuality = PROOF_QUALITY;						// best
	logFont.lfQuality = ANTIALIASED_QUALITY;				// antialias
//	logFont.lfQuality = NONANTIALIASED_QUALITY;				// non-antialias
	logFont.lfOutPrecision = ( bNeedTTFont ) ? OUT_TT_ONLY_PRECIS : OUT_TT_PRECIS;			// use TrueType

	switch (family)
	{
		case eFontFamilyUseName :
			strcpy(logFont.lfFaceName, facename);
			logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
			break;
		case eFontFamilySerif :
			logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
			break;
		case eFontFamilySansSerif :
			logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
			break;
		case eFontFamilyTypewriter :
			logFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
			break;
		case eFontFamilyBadFamily :
			break;

	}

	if (style & eFontStyleBold)
		logFont.lfWeight = FW_BOLD;
	else
		logFont.lfWeight = FW_NORMAL;

	if (style & eFontStyleItalic)
		logFont.lfItalic = TRUE;

	return CreateFontIndirect(&logFont);

}

CAnmFont *CAnmFont::DefaultFont()
{
	return new CAnmFont("Arial", eFontFamilyUseName, eFontStylePlain);
}


inline float FixedToFloat(FIXED fx)
{
	int i = * (int *) &fx;

	return ((float) i) / 65536.0;
}

CAnmTextGlyph *CAnmFont::GetTextGlyph(wchar_t c)
{
	// check our cache first; N.B.: 8-bit chars only for now
	if (c < 256 && m_glyphcache[c])
		return m_glyphcache[c];

	GLYPHMETRICS met;
	static MAT2 mat = { { 0, 1 } , {0, 0}, {0, 0 }, {0, 1} };

	char *gbuf = NULL;
	int bufsz = GetGlyphOutline(m_hDC, c, GGO_NATIVE, &met, 0, NULL, &mat);

	if (bufsz <= 0)
		return NULL;

	gbuf = new char[bufsz];
	GetGlyphOutline(m_hDC, c, GGO_NATIVE, &met, bufsz, gbuf, &mat);

	CAnmTextGlyph *pGlyph = new CAnmTextGlyph;
	char *savedbuf = gbuf;
	int sz = bufsz;

	while (sz > 0)
	{
		CAnmPolygon polygon;

		TTPOLYGONHEADER *phdr = (TTPOLYGONHEADER *) gbuf;
		Point3 firstPoint =
			Point3(FixedToFloat(phdr->pfxStart.x) / m_textMetrics.tmHeight,
				FixedToFloat(phdr->pfxStart.y)  / m_textMetrics.tmHeight,
				0.f);

		gbuf += sizeof(TTPOLYGONHEADER);
		int pdatasz = phdr->cb - sizeof(TTPOLYGONHEADER);
		
		while (pdatasz > 0)
		{
			CAnmOutline outline;

			//if (polygon.GetOutlines().size() == 0)
				outline.AddPoint(firstPoint);

			TTPOLYCURVE *pcrv = (TTPOLYCURVE *) gbuf;
			Point3 p;
			for (int i = 0; i < pcrv->cpfx; i++)
			{
				p = Point3(FixedToFloat(pcrv->apfx[i].x) / m_textMetrics.tmHeight,
						FixedToFloat(pcrv->apfx[i].y) / m_textMetrics.tmHeight,
						0.f);

				outline.AddPoint(p);

				firstPoint = p;
			}

			if (pcrv->wType == TT_PRIM_LINE)
				polygon.AddOutline(outline);
			else
				polygon.AddBezier(outline);

			int cdatasz = sizeof(TTPOLYCURVE) - sizeof(POINTFX) +
				pcrv->cpfx * sizeof(POINTFX);

			gbuf += cdatasz;
			pdatasz -= cdatasz;
		}

		pGlyph->AddPolygon(polygon);
		sz -= phdr->cb;

	}

	delete [] savedbuf;

	// save to cache; N.B.: 8-bit chars only for now
	if (c < 256)
		m_glyphcache[c] = pGlyph;

	return pGlyph;
}

float CAnmFont::GetCharWidth(wchar_t c)
{
	ABC abc;
	GetCharABCWidths(m_hDC, c, c, &abc);

	float totalwidth = abc.abcA + abc.abcB + abc.abcC;

	return totalwidth / (float) m_textMetrics.tmHeight;
}

float CAnmFont::GetCharHeight()
{
	return (float) m_textMetrics.tmHeight;
}

float CAnmFont::GetStringPixelWidth(const char *cp)
{
	SIZE size;
	GetTextExtentPoint32(m_hDC, cp, strlen(cp), &size);

	return (float) size.cx;
}

float CAnmFont::GetStringWidth(const char *cp)
{
	SIZE size;
	GetTextExtentPoint32(m_hDC, cp, strlen(cp), &size);

	return (float) size.cx / (float) m_textMetrics.tmHeight;
}
//  #define DUMP_TEXTURE_BMP

#ifdef DUMP_TEXTURE_BMP
void WriteBmp( char* filename,
					unsigned char* pdata0,
					int width, int height );

#endif


// GetBitmapData
// get the bitmap data.
// If it is there, then use it, otherwise, generate it.
// If the ppTextureCoordData is NOT null, a pointer to the tex coord data is also returned.
// Default is ppTextureCoordData=NULL.
//
CAnmGraphics::Texture* CAnmFont::GetBitmapData( CAnmDevice* pDevice, float **ppTextureCoordData )
{
	// If we dont have the data, generate it.
	//
	if( !( m_pTextureData && m_pTextureCoords && m_textureSurface ) ) {

		int iPointHeight = m_iPointSize;

		CAnmTextureData* pTextureData = NULL;
		DWORD dwTexWidth;
		DWORD dwTexHeight;
		FLOAT fScale = 1.0;


		// Large fonts need larger textures
		if( iPointHeight > 20 )
			dwTexWidth = 512;
		else
			dwTexWidth  = 256;


		HFONT hFont = m_hFont;
		if( NULL==hFont )
			return NULL;


		// First just create a tmp bitmap to figure out how big of a Bitmap we really need.
		// Prepare to create a bitmap
		DWORD*      pBitmapBits;
		BITMAPINFO bmi;
		ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
		bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth       =  (int)32;
		bmi.bmiHeader.biHeight      = -(int)32;
		bmi.bmiHeader.biPlanes      = 1;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biBitCount    = 32;

		// Create a DC and a bitmap for the font
		HDC     hDC       = CreateCompatibleDC( NULL );
		HBITMAP hbmBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS,
											  (VOID**)&pBitmapBits, NULL, 0 );
		SetMapMode( hDC, MM_TEXT );

		SelectObject( hDC, hbmBitmap );
		SelectObject( hDC, hFont );

		// Set text properties
		SetTextColor( hDC, RGB(255,255,255) );
		SetBkColor(   hDC, 0x00000000 );
		SetTextAlign( hDC, TA_TOP );


		int iXStart = iPointHeight;
		int iSizeY = m_textMetrics.tmHeight;
		ABC abc;




		// Loop through all printable character just check the size.
		// So we can get an optimal sized texture
		DWORD x = iXStart;
		DWORD y = 0;
		TCHAR str[2] = _T("x");
		for( unsigned char c=32; c<128; c++ )
		{

			str[0] = c;
			GetCharABCWidths(hDC, c, c, &abc);

			int iPreSpace = abc.abcA;
			int iPostSpace = abc.abcC;
			int icharWidth = abc.abcB;
			int iTotalWidth = iPreSpace + iPostSpace + icharWidth;

			if( iPreSpace < 0 ) {
				x -= iPreSpace;
			}

			if( (DWORD)(x+iTotalWidth+iXStart) > dwTexWidth )
			{
				x  = iXStart;
				y += iSizeY+1;
			}

			if( iPostSpace < 0 ) {
				x -= iPostSpace;
			}
			x += iTotalWidth+1;
		}
		int ybottom = y + iSizeY;

		//Now get the min required height of the texture.
		//
		for( int iShift=1; iShift<11 && ( ( 1<<iShift ) <= ybottom ); iShift++ ) {
		}
		dwTexHeight = ( 1<<iShift );


		HBITMAP hbmBitmapOld = hbmBitmap;

		// Now create the texture that we will use.
		// Prepare to create a bitmap
		ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
		bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth       =  (int)dwTexWidth;
		bmi.bmiHeader.biHeight      = -(int)dwTexHeight;
		bmi.bmiHeader.biPlanes      = 1;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biBitCount    = 32;

		// Create a DC and a bitmap for the font
		hbmBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS,
											  (VOID**)&pBitmapBits, NULL, 0 );

		SelectObject( hDC, hbmBitmap );
		SelectObject( hDC, hFont );
		SetMapMode( hDC, MM_TEXT );

		// Set text properties
		SetTextColor( hDC, RGB(255,255,255) );
		SetBkColor(   hDC, 0x00000000 );



		SetTextAlign( hDC, TA_TOP );

		DeleteObject( hbmBitmapOld );


		float* pTextureCoords = new float[ANM_N_TEXTURECOORDS_PER_CHAR*( 128-32 )];

		// Loop through all printable character and output them to the bitmap..
		// Meanwhile, keep track of the corresponding tex coords for each character.
		x = iXStart;
		y = 0;

		float fTexWidth = dwTexWidth;
		float fTexHeight = dwTexHeight;

// 	 FILE* fp = fopen( "g:/keith/fontrect.txt", "w" );

		for( c=32; c<128; c++ )
		{
			str[0] = c;

			GetCharABCWidths(hDC, c, c, &abc);


			int iPreSpace = abc.abcA;
			int iPostSpace = abc.abcC;
			int icharWidth = abc.abcB;

			int iTotalWidth = iPreSpace + iPostSpace + icharWidth;

// fprintf( fp, "%3d; {%c}  %3d %3d %3d \n", c, c, abc.abcA, abc.abcB, abc.abcC );

			if( iPreSpace < 0 ) {
				x -= iPreSpace;
			}

			if( (DWORD)(x+iTotalWidth+iXStart) > dwTexWidth )
			{
				x  = iXStart;
				y += iSizeY+1;
			}

			ExtTextOut( hDC, x+0, y+0, ETO_OPAQUE, NULL, str, 1, NULL );

			int ioff = ANM_N_TEXTURECOORDS_PER_CHAR*( c-32 );
			pTextureCoords[ioff+0] = (0.0+(FLOAT)(x+iPreSpace))/fTexWidth;
			pTextureCoords[ioff+1] = 1.0f - ((FLOAT)(y))/fTexHeight;
			pTextureCoords[ioff+2] = (0.0+(FLOAT)(x+iPreSpace+icharWidth))/fTexWidth;
			pTextureCoords[ioff+3] = 1.0f - ((FLOAT)(y+iSizeY))/fTexHeight;
			pTextureCoords[ioff+4] = ((FLOAT)(iPreSpace))/fTexWidth;
			pTextureCoords[ioff+5] = ((FLOAT)(iPostSpace))/fTexWidth;

			if( iPostSpace < 0 ) {
				x -= iPostSpace;
			}
			x += iTotalWidth+1;
		}
// 	 fclose( fp );

		bool bHasAlpha = true;
		bool bIsGreyscale = true;
		pTextureData = new CAnmTextureData( dwTexWidth, dwTexHeight, bHasAlpha, bIsGreyscale, false);
		unsigned char* pPixelDst = pTextureData->GetPixelData();


		BYTE bAlpha; // 4-bit measure of pixel intensity

		unsigned char c0, c1, c2, ca, cb;

		for( y=0; y < dwTexHeight; y++ )
		{
			for( x=0; x < dwTexWidth; x++ )
			{

				COLORREF color = GetPixel( hDC, x, dwTexHeight - y - 1 );
				ca = GetRValue( color );
				c0 = ( ( ca > 0 ) ? 255 : 0 );


#ifndef DUMP_TEXTURE_BMP
				*pPixelDst++ = c0;
				*pPixelDst++ = ca;
#else
				*pPixelDst++ = ca;
				*pPixelDst++ = ca;
				*pPixelDst++ = ca;
	#endif	// DUMP_TEXTURE_BMP

			}
		}

		// Done updating texture, so clean up used objects
		DeleteObject( hbmBitmap );
		DeleteDC( hDC );

	#ifdef DUMP_TEXTURE_BMP
		WriteBmp( "g:/keith/fontout.bmp",
			pTextureData->GetPixelData(), dwTexWidth, dwTexHeight );
	#endif


		m_pTextureCoords = pTextureCoords;
		m_pTextureData = pTextureData;

		m_textureSurface = CAnmGraphics::Texture::LoadTexture(pTextureData, pDevice );
		m_textureSurface->Ref();

	}

	if( ppTextureCoordData ) {
		*ppTextureCoordData = m_pTextureCoords;
	}
	return m_textureSurface;
}












#ifdef DUMP_TEXTURE_BMP



void WriteBmp( char* filename,
					unsigned char* pdata0,
					int width, int height )
{
	BITMAPFILEHEADER  bmheader;
	BITMAPINFOHEADER biNew;
	int datasize = 3*width*height;

	bmheader.bfType = 19778;
	bmheader.bfSize = datasize +
			sizeof( BITMAPFILEHEADER ) +
			sizeof(BITMAPINFOHEADER);
	bmheader.bfReserved1 = 0;
	bmheader.bfReserved2 = 0;
	bmheader.bfOffBits = sizeof( BITMAPFILEHEADER ) +
								sizeof(BITMAPINFOHEADER);

	biNew.biSize = sizeof( biNew );
	biNew.biWidth = width;
	biNew.biHeight = height;
	biNew.biPlanes = 1;
	biNew.biBitCount = 24;
	biNew.biCompression = 0;
	biNew.biSizeImage = datasize;
	biNew.biXPelsPerMeter = 3780;
	biNew.biYPelsPerMeter = 3780;
	biNew.biClrUsed = 0;
	biNew.biClrImportant = 0;

	unlink( filename );

	HANDLE hfbm = CreateFile(filename, GENERIC_WRITE,
		  0, (LPSECURITY_ATTRIBUTES) NULL,
		  CREATE_NEW, FILE_ATTRIBUTE_NORMAL,
		  (HANDLE) NULL);

	if( !hfbm ) {
		return;
	}


	DWORD dwRead;
	WriteFile(hfbm, &bmheader, sizeof(BITMAPFILEHEADER),
	  &dwRead, (LPOVERLAPPED)NULL);

	WriteFile(hfbm, &biNew, sizeof(BITMAPINFOHEADER),
	  &dwRead, (LPOVERLAPPED)NULL);



	WriteFile(hfbm, pdata0, datasize,
	  &dwRead, (LPOVERLAPPED)NULL);


	CloseHandle(hfbm);


}


#endif