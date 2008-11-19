/********************************************************************************
 * Flux
 *
 * File: anmtextmesh.h
 * Description: Text Mesh class - support for 
 *							polygonal text meshes
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

#ifndef _anmtextmesh_h
#define _anmtextmesh_h

#include "anmmesh.h"

class EXPORT CAnmTextMesh : public CAnmMesh
{
protected:
	
	class CAnmIndexedTriangleList	*m_trianglelist;

	void GenGlyphPolys(class CAnmTextGlyph *pGlyph, float fontsize, float fontWidthScale,
				float hoffset, float voffset, int &indexoffset, bool bExtruded, float depth);
	void GenBitmapPolys( float* puv, float charWidth, float charHeight,
				float hoffset, float voffset, int &indexoffset);

	class CAnmTextGlyph *** CreateTextGlyphs(class CAnmText *pText, class CAnmFont *pFont);
	void CAnmTextMesh::DeleteTextGlyphs(class CAnmTextGlyph ***pppGlyphs, class CAnmText *pText);

	int *Tesselate(Point3 *pVerts, int &numIndices, int *countours, int countourcount);


	void RebuildBitmapText(CAnmText *pText, CAnmFont *pFont, class CAnmScreenFontStyle *pFontStyleExt );
	void RebuildStrokedText(CAnmText *pText, CAnmFont *pFont, class CAnmNode *pFontStyleNode);


	// for Bitmap Text
	CAnmGraphics::Texture*  m_textureSurface;
	float*					m_pTextureCoords;

	Boolean				m_back;
	Boolean				m_side;
	Boolean				m_front;

public:

	// constructor/destructor
	CAnmTextMesh(class CAnmDevice *pDevice, class CAnmText *pText, class CAnmFont *pFont, class CAnmNode *pFontStyle );
	virtual ~CAnmTextMesh();

	// CAnmMesh overrides
	virtual bool HasAlpha();

	// Methods
	void Rebuild(class CAnmDevice *pDevice, class CAnmText *pText, class CAnmFont *pFont, class CAnmNode *pFontStyle );

	virtual CAnmGraphics::Texture* CAnmTextMesh::GetTextureSurface();

	
};


#endif // _anmtextmesh_h
