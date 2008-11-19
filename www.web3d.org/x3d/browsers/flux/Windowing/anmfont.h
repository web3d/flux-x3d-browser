/********************************************************************************
 * Flux
 *
 * File: anmfont.h
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

#ifndef _anmfont_h
#define _anmfont_h

#include "anmrefobject.h"

typedef enum eFontFamily {
	eFontFamilySerif,
	eFontFamilySansSerif,
	eFontFamilyTypewriter,
	eFontFamilyUseName,
	eFontFamilyBadFamily	= -1,
} eFontFamily;

typedef enum eFontStyle {
	eFontStylePlain			= 0x0,
	eFontStyleBold			= 0x1,
	eFontStyleItalic		= 0x2,
	eFontStyleBoldItalic	= 0x3,
	eFontStyleBadStyle		= -1,
} eFontStyle;

#define ANMFONT_POINTSIZE 72		// we'll use 72 dpi font for now...

#define ANM_N_TEXTURECOORDS_PER_CHAR	6

// CAnmFont - hides details of Windows font management
#define ANMGLYPHCACHESIZE 256				// N.B.: caches 8-bit glyphs only right now
class CAnmFont : public CAnmRefObject
{
protected:

	HDC							 m_hDC;				// N.B.: Windows-only
	HFONT						 m_hFont;
	HFONT						 m_hOldFont;
	TEXTMETRIC					 m_textMetrics;
	class CAnmTextGlyph			*m_glyphcache[ANMGLYPHCACHESIZE];
	String						 m_facename;
	eFontFamily					 m_family;
	eFontStyle					 m_style;
	int							 m_iPointSize;


	class CAnmTextureData*		 m_pTextureData;
	float*						 m_pTextureCoords;
	class CAnmGraphics::Texture* m_textureSurface;

	HFONT MakeFont(const char *facename, eFontFamily family, eFontStyle style, int iPointSize = ANMFONT_POINTSIZE );

public:

	CAnmFont(const char *facename, eFontFamily family, eFontStyle style, int iPointSize = ANMFONT_POINTSIZE );

	virtual ~CAnmFont();

	static CAnmFont *FindFont(const char *facename, eFontFamily family, eFontStyle style, 
		int iPointSize, class cApplication * pApp );
	static CAnmFont *DefaultFont();

	class CAnmTextGlyph *GetTextGlyph(wchar_t c);

	float GetCharWidth(wchar_t c);
	float GetStringWidth(const char *cp);		// N.B.: not multibyte yet
	float GetStringPixelWidth(const char *cp);
	float GetCharHeight();


	class CAnmGraphics::Texture* CAnmFont::GetBitmapData( CAnmDevice* pDevice, float **ppTextureCoordData = NULL );


	// Accessors
	int GetPointSize(){ return m_iPointSize; }

	String GetFaceName()
	{
		return m_facename;
	}

	eFontFamily GetFamily()
	{
		return m_family;
	}

	eFontStyle GetStyle()
	{
		return m_style;
	}
};

#endif // _anmfont_h
