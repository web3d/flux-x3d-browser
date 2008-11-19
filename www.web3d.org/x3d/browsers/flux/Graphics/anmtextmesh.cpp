/********************************************************************************
 * Flux
 *
 * File: anmtextmesh.cpp
 * Description: Mesh class - support for 
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

#include "stdafx.h"
#include "anmtextmesh.h"
#include "anmfont.h"
#include "anmfontstyle.h"
#include "anmscreenfontstyle.h"
#include "anmextrudedfontstyle.h"
#include "anmpolygon.h"
#include "anmtext.h"
#include "anmtextglyph.h"
#include "anmworld.h"
#include "anmtexturedata.h"

#include "anmdevice.h"
#include "anmviewport.h"


#define  ANM_BITMAPED_TEXT_PERTIBATION    ( 0.1234f )

CAnmTextMesh::CAnmTextMesh(CAnmDevice *pDevice, CAnmText *pText, CAnmFont *pFont, CAnmNode *pFontStyle )
{

	m_textureSurface = NULL;
	m_pTextureCoords = NULL;
	m_back = true;
	m_side = true;
	m_front = true;


	m_trianglelist = NULL;


	Rebuild(pDevice, pText, pFont, pFontStyle );
}

CAnmTextMesh::~CAnmTextMesh()
{
	// do NOT delete m_textureSurface or m_pTextureCoords.
	// that is the Font's responsibility.

	SafeDelete(m_trianglelist);
	
	m_primitives.clear();
}


// Determine type of FontStyle and call appropriate Rebuild method
//
void CAnmTextMesh::Rebuild(CAnmDevice *pDevice, CAnmText *pText, CAnmFont *pFont, CAnmNode *pFontStyleNode )
{
	assert(pText);
	assert(pFont);

	// Clear out old primitive list; create new one
	m_primitives.clear();
	SafeDelete(m_trianglelist);

	m_trianglelist = new CAnmIndexedTriangleList(pDevice);
	AddPrimitive(m_trianglelist);

	// Clear out this data.
	// If this is a Bitmap font, we will get it again from the font.
	// The font stores it, ans is responsible for keeping and deleting it.
	// We only store pointers to it for usage.
	m_textureSurface = NULL;
	m_pTextureCoords = NULL;


	// get string count
	StringArray *pStrings = pText->GetStrings();

	if (pStrings == NULL && pStrings->size() <= 0)
		return;



	CAnmFontStyle* pFontStyle = NULL;
	CAnmScreenFontStyle* pFontStyleExt = NULL;
	CAnmExtrudedFontStyle* pExtrudedFontStyle = NULL;

	if( pFontStyleNode == NULL ||
		( pFontStyle = GETIMPLEMENTATION(CAnmFontStyle, pFontStyleNode) ) ) {
		RebuildStrokedText(pText, pFont, pFontStyleNode );
	}
	else if( 
		( pExtrudedFontStyle = GETIMPLEMENTATION(CAnmExtrudedFontStyle, pFontStyleNode) ) ) {
		RebuildStrokedText(pText, pFont, pFontStyleNode );
	}
	else if( pFontStyleExt = GETIMPLEMENTATION(CAnmScreenFontStyle, pFontStyleNode) ) {
		RebuildBitmapText( pText, pFont, pFontStyleExt );
	}

	// set up a vertex buffer if it's supported
	CAnmWorld *pWorld = pText->GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);
	m_trianglelist->BufferVertices(pApp->GetDevice());
	// sort the thing for picking
	m_trianglelist->Sort();
}


void CAnmTextMesh::RebuildBitmapText( CAnmText *pText, CAnmFont *pFont, CAnmScreenFontStyle *pFontStyleExt )
{
	// Get the Texture Data from the Font.
	//
	CAnmWorld *pWorld = pText->GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();



	CAnmViewport *pViewport = pDevice->GetViewport();
	assert(pViewport);

	int iMainPixelWidth = pViewport->Width();
	int iMainPixelHeight = pViewport->Height();



	m_textureSurface = pFont->GetBitmapData( pDevice, &m_pTextureCoords );

	if( !( m_textureSurface && m_pTextureCoords ) ) {
		return;
	}


	// get string count
	StringArray *pStrings = pText->GetStrings();


	// Now lay out all the text horizontally and vertically

	// get size and spacing from font
	float fontsz, fontsp;
	fontsz = ANMFONTSTYLE_DEFAULT_SIZE;
	fontsp = ANMFONTSTYLE_DEFAULT_SPACING;

	// krv
	// NOTE:  Justify[index]
	// Index is 0 for Major Axis, 1 for minor axis.
	// NOT text line number!
	// justifyMinor set here in comments because its NIY.

	eFontStyleJustify justify = eFontStyleJustifyLeft;
	eFontStyleJustify justifyMinor = eFontStyleJustifyLeft;
	if( pFontStyleExt ) {
		fontsz = pFontStyleExt->GetPointSize();
		fontsp = pFontStyleExt->GetSpacing();
		justify = pFontStyleExt->GetJustifyType(0);
		justifyMinor = pFontStyleExt->GetJustifyType(1);

		if (fontsz <= 0)
			fontsz = ANMFONTSTYLE_DEFAULT_SIZE;
		if (fontsp <= 0)
			fontsp  = ANMFONTSTYLE_DEFAULT_SPACING;
	}


	// measure the string lengths in world coordinates
	int nStrings = pStrings->size();

	FloatArray *pLengths = pText->GetLengths();
	int nLengths = 0;
	if (pLengths)
		nLengths = pLengths->size();

	float *wlengths = new float[nStrings];
	float maxwlength = 0.f;
	for (int i = 0; i < nStrings; i++)
	{
		String s = (*pStrings)[i];
		const char *cp = s->GetBuf();						// N.B.: not multibyte yet

		wlengths[i] = pFont->GetStringPixelWidth(cp);
		if (i < nLengths)
		{
			if ((*pLengths)[i] > maxwlength)
			{
				maxwlength = (*pLengths)[i];
			}
		}
		else if (wlengths[i] > maxwlength)
		{
			maxwlength = wlengths[i];
		}
	}	

	float maxfactor = 1.f;
	float maxextent = pText->GetMaxExtent();
	if (maxextent != 0.f && maxwlength > maxextent)
	{
		maxfactor = maxextent / maxwlength;
	}

	// N.B.: x,y offset and base index offset for vertices start at 0 but run up as glyph polys
	// are added to the mesh; in this way we can have all the polys for the string stored in
	// a single mesh. this is most likely more efficient to render

	// krv:  in the offsets with some fraction so that the pixels dont lie on the fence.
	// 
	float hoffset = ANM_BITMAPED_TEXT_PERTIBATION;
	float voffset = ANM_BITMAPED_TEXT_PERTIBATION;
	int indexoffset = 0;

	float charHeight = pFont->GetCharHeight( );


	float totalTextHeight = ( nStrings-1 ) * charHeight * fontsp + charHeight;

	if (justifyMinor == eFontStyleJustifyLeft)
		voffset += 0.f;
	else if (justifyMinor == eFontStyleJustifyMiddle)
		voffset += totalTextHeight / 2.0f;
	else if (justifyMinor == eFontStyleJustifyRight)
		voffset += totalTextHeight;
	else if (justifyMinor == eFontStyleJustifyFirst)
		voffset += fontsz;

	voffset -= charHeight;

	for (i = 0; i < nStrings; i++)
	{
		String s = (*pStrings)[i];

		const char *cp = s->GetBuf();						// N.B.: not multibyte yet
		int len = strlen(cp);

		float scalefactor, lenfactor;
		if (i < nLengths && (*pLengths)[i] != 0.f && wlengths[i] != 0.f)
			lenfactor = (*pLengths)[i] / wlengths[i] ;
		else
			lenfactor = 1.f;

		scalefactor = lenfactor * maxfactor;

		// if the string is too long, scale down the font (this will
		// also handle intercharacter spacing in conjunction with hspacefactor below)

		// if the string is shorter than length, stretch it by
		// using interchar spacing, *not* by scaling up the font


		float scaledsz = fontsz;
		float horScale = 1.0f;
		float hspacefactor = 1.f;

#ifdef ANM_STRETCH_TEXT_BY_SCALE
		horScale = scalefactor;
#else
		if (scalefactor < 1.f)
		{
			horScale = scalefactor;
		}
		else if (scalefactor > 1.f)
		{
			hspacefactor = scalefactor;
		}
#endif


		hoffset = ANM_BITMAPED_TEXT_PERTIBATION;
		if (justify == eFontStyleJustifyLeft || justify == eFontStyleJustifyFirst )
			hoffset += 0.f;
		else if (justify == eFontStyleJustifyMiddle)
			hoffset += ( - hspacefactor * horScale * wlengths[i] / 2 );
		else if (justify == eFontStyleJustifyRight)
			hoffset += ( - hspacefactor * horScale * wlengths[i] );

		// 
		//
		scaledsz = 1.0;

		float fTextureWidth = ( float ) m_textureSurface->GetWidth();
		float fTextureHeight = ( float ) m_textureSurface->GetHeight();

		float* puv;
		float dx, dy, charWidth;
		for (int j = 0; j < len; j++)
		{

			puv = m_pTextureCoords + ( ( cp[j] & 127 ) - 32 ) * ANM_N_TEXTURECOORDS_PER_CHAR;
			dx = puv[2] - puv[0];
			dy = puv[1] - puv[3];

			charWidth = dx * fTextureWidth;
			charHeight = dy * fTextureHeight;

			hoffset += puv[4]*fTextureWidth*hspacefactor * horScale;
			GenBitmapPolys( puv, charWidth*horScale, charHeight, hoffset, voffset, indexoffset);
			hoffset += ( puv[5]*fTextureWidth + charWidth ) * hspacefactor * horScale;
		}

		voffset -= ( charHeight * fontsp );
	}
	delete [] wlengths;


	// krv:
	// TODO:  Need to do backfaces inteligently.
	// generate the backfaces
	// well, if the text is in an overlay, they we really dont need to do this.
	// If you look at the text from behind, should it appear correct, or backward???
//	if (!pText->GetSolid())
//		m_trianglelist->GenerateBackfaces();

}


void CAnmTextMesh::GenBitmapPolys( float* puv, float charWidth, float charHeight,
				float hoffset, float voffset, int &indexoffset)
{


	m_trianglelist->AddVertex( sAnmVertex( Point3( hoffset,				charHeight + voffset, 0.0),
		Point3(0, 0, 1),
		Point2( puv[0], puv[1] ) ) );

	m_trianglelist->AddVertex( sAnmVertex( Point3( hoffset,				voffset,		0.0),
		Point3(0, 0, 1),
		Point2( puv[0], puv[3] ) ) );

	m_trianglelist->AddVertex( sAnmVertex( Point3( charWidth + hoffset,	charHeight + voffset, 0.0),
		Point3(0, 0, 1),
		Point2( puv[2], puv[1] ) ) );

	m_trianglelist->AddVertex( sAnmVertex( Point3( charWidth + hoffset,	voffset,		0.0),
		Point3(0, 0, 1),
		Point2( puv[2], puv[3]  ) ) );


	m_trianglelist->AddIndex(indexoffset + 0);
	m_trianglelist->AddIndex(indexoffset + 1);
	m_trianglelist->AddIndex(indexoffset + 2);

	m_trianglelist->AddIndex(indexoffset + 2);
	m_trianglelist->AddIndex(indexoffset + 1);
	m_trianglelist->AddIndex(indexoffset + 3);

	indexoffset += 4;
}



void CAnmTextMesh::RebuildStrokedText(CAnmText *pText, CAnmFont *pFont, CAnmNode *pFontStyleNode)
{
	CAnmWorld *pWorld = pText->GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();


	// get string count
	StringArray *pStrings = pText->GetStrings();

	// Fetch all the glyph outlines from the font
	CAnmTextGlyph *** pppGlyphs = CreateTextGlyphs(pText, pFont);
	assert(pppGlyphs != NULL);

	// Now lay out all the text horizontally and vertically

	// get size and spacing from font
	float fontsz, fontsp;
	fontsz = ANMFONTSTYLE_DEFAULT_SIZE;
	fontsp = ANMFONTSTYLE_DEFAULT_SPACING;

	// krv
	// NOTE:  Justify[index]
	// Index is 0 for Major Axis, 1 for minor axis.
	// NOT text line number!
	// justifyMinor set here in comments because its NIY.

	CAnmFontStyle* pFontStyle = NULL;
	CAnmExtrudedFontStyle* pExtrudedFontStyle = NULL;


	m_back = true;
	m_side = true;
	m_front = true;


	bool bExtruded = false;
	float depth = 0.0;

	eFontStyleJustify justify = eFontStyleJustifyLeft;
	eFontStyleJustify justifyMinor = eFontStyleJustifyLeft;
	if( pFontStyleNode ) {
		if( pFontStyle = GETIMPLEMENTATION(CAnmFontStyle, pFontStyleNode) ) {
			fontsz = pFontStyle->GetSize();
			fontsp = pFontStyle->GetSpacing();
			justify = pFontStyle->GetJustifyType(0);
			justifyMinor = pFontStyle->GetJustifyType(1);
		}
		else if( pExtrudedFontStyle = GETIMPLEMENTATION(CAnmExtrudedFontStyle, pFontStyleNode) ) {
			fontsz = pExtrudedFontStyle->GetSize();
			fontsp = pExtrudedFontStyle->GetSpacing();
			justify = pExtrudedFontStyle->GetJustifyType(0);
			justifyMinor = pExtrudedFontStyle->GetJustifyType(1);
			depth = pExtrudedFontStyle->GetDepth();
			m_back = pExtrudedFontStyle->GetBack();
			m_side = pExtrudedFontStyle->GetSide();
			m_front = pExtrudedFontStyle->GetFront();
		}
	}
	if (fontsz <= 0)
		fontsz = ANMFONTSTYLE_DEFAULT_SIZE;
	if (fontsp <= 0)
		fontsp  = ANMFONTSTYLE_DEFAULT_SPACING;

	if( depth > 0.000001 ) {
		bExtruded = true;
	}


	bool m_bMiddle = true;
	if( bExtruded && m_bMiddle ) {
	}


	// measure the string lengths in world coordinates
	int nStrings = pStrings->size();

	FloatArray *pLengths = pText->GetLengths();
	int nLengths = 0;
	if (pLengths)
		nLengths = pLengths->size();

	float *wlengths = new float[nStrings];
	float maxwlength = 0.f;
	for (int i = 0; i < nStrings; i++)
	{
		String s = (*pStrings)[i];
		const char *cp = s->GetBuf();						// N.B.: not multibyte yet

		wlengths[i] = pFont->GetStringWidth(cp);
		if (i < nLengths)
		{
			if ((*pLengths)[i] / fontsz > maxwlength)
			{
				maxwlength = (*pLengths)[i] / fontsz;
			}
		}
		else if (wlengths[i] > maxwlength)
		{
			maxwlength = wlengths[i];
		}
	}	

	float maxfactor = 1.f;
	float maxextent = pText->GetMaxExtent() / fontsz;
	if (maxextent != 0.f && maxwlength > maxextent)
	{
		maxfactor = maxextent / maxwlength;
	}

	// N.B.: x,y offset and base index offset for vertices start at 0 but run up as glyph polys
	// are added to the mesh; in this way we can have all the polys for the string stored in
	// a single mesh. this is most likely more efficient to render
	float hoffset = 0.0f;
	float voffset = 0.0f;
	int indexoffset = 0;

	float totalTextHeight = ( nStrings-1 ) * fontsp * fontsz + fontsz;

	if (justifyMinor == eFontStyleJustifyLeft)
		voffset = 0.f;
	else if (justifyMinor == eFontStyleJustifyMiddle)
		voffset = totalTextHeight / 2.0f;
	else if (justifyMinor == eFontStyleJustifyRight)
		voffset = totalTextHeight;
	else if (justifyMinor == eFontStyleJustifyFirst)
		voffset = fontsz;


	voffset -= fontsz;


	for (i = 0; i < nStrings; i++)
	{
		String s = (*pStrings)[i];

		const char *cp = s->GetBuf();						// N.B.: not multibyte yet
		int len = strlen(cp);

		float scalefactor, lenfactor;
		if (i < nLengths && (*pLengths)[i] != 0.f && wlengths[i] != 0.f)
			lenfactor = (*pLengths)[i]  / ( wlengths[i] * fontsz );
		else
			lenfactor = 1.f;

		scalefactor = lenfactor * maxfactor;

		// if the string is too long, scale down the font (this will
		// also handle intercharacter spacing in conjunction with hspacefactor below)

		// if the string is shorter than length, stretch it by
		// using interchar spacing, *not* by scaling up the font
		
		float scaledsz = fontsz;
		float horScale = 1.0f;
		float hspacefactor = 1.f;

#ifdef ANM_STRETCH_TEXT_BY_SCALE
		horScale = scalefactor;
#else

		if (scalefactor < 1.f)
		{
			horScale = scalefactor;
		}
		else if (scalefactor > 1.f)
		{
			hspacefactor = scalefactor;
		}

#endif
		hoffset = 0.0f;
		if (justify == eFontStyleJustifyLeft || justify == eFontStyleJustifyFirst )
			hoffset = 0.f;
		else if (justify == eFontStyleJustifyMiddle)
			hoffset = - hspacefactor*wlengths[i] / 2;
		else if (justify == eFontStyleJustifyRight)
			hoffset = - hspacefactor*wlengths[i];

		float xoffset;

		for (int j = 0; j < len; j++)
		{
			if (pppGlyphs[i][j])
				GenGlyphPolys(pppGlyphs[i][j], scaledsz, horScale,
					hoffset, voffset, indexoffset, bExtruded, depth);
			xoffset = pFont->GetCharWidth(cp[j]) * hspacefactor;
			hoffset += xoffset;
		}

		voffset -= ( fontsp * fontsz );
	}


	// generate the backfaces
	if (!pText->GetSolid()) {
		m_trianglelist->GenerateBackfaces(pDevice);
	}

	// now clean up
	DeleteTextGlyphs(pppGlyphs, pText);
	delete [] wlengths;
}






void CAnmTextMesh::GenGlyphPolys(CAnmTextGlyph *pGlyph, float fontsize, float fontWidthScale, 
						float hoffset, float voffset, int &indexoffset,
						bool bExtruded, float depth )
{
	assert(pGlyph);


	float halfDepth = depth/2.0f;

	int baseindex = indexoffset;

	std::vector<Point3> points = pGlyph->GetPoints();
	int nverts = points.size();
	std::vector<WORD> indices = pGlyph->GetIndices();
	int nindices = indices.size();

	if( m_front ) {
		// create a vertex for each point with default normal (N.B.: assumes RH rule)
		for (int i = 0; i < nverts; i++)
		{
			sAnmVertex vert;
			vert = sAnmVertex( 
				Point3( ( points[i].x + hoffset) * fontsize * fontWidthScale, ( points[i].y ) * fontsize + voffset, halfDepth ),
				Point3(0, 0, 1));
			m_trianglelist->AddVertex(vert);
			indexoffset++;
		}

		// add the indices

		for (i = 0; i < nindices; i++)
		{
			WORD index = indices[i] + baseindex;
			m_trianglelist->AddIndex(index);
		}
	}
	if( bExtruded ) {
		// Put the back faces in there.
		//

		// first do the back faces.
		//
		if( m_back ) {
			baseindex = indexoffset;;
			for (int i = 0; i < nverts; i++)
			{
				sAnmVertex vert;
				vert = sAnmVertex( 
					Point3( ( points[i].x + hoffset) * fontsize * fontWidthScale, ( points[i].y ) * fontsize + voffset, -halfDepth ),
					Point3(0, 0, -1));
				m_trianglelist->AddVertex(vert);
				indexoffset++;
			}

			// add the indices
			std::vector<WORD> indices = pGlyph->GetIndices();
			int nindices = indices.size();

			for (i = 0; i < nindices; i+=3)
			{
				// flip flop the first two to get back faces.
				//
				m_trianglelist->AddIndex(indices[i+1] + baseindex);
				m_trianglelist->AddIndex(indices[i+0] + baseindex);
				m_trianglelist->AddIndex(indices[i+2] + baseindex);
			}
		}
		// now do middle parts
		//

		if( m_side ) {

			baseindex = indexoffset;


			Point3 TheNorm( 0.0, 1.0, 0.0 );
			Point3 TheNormCrease;
			Point3 LastPoint( 0.0, 0.0, 0.0 );
			Point3 NextPoint( 0.0, 0.0, 0.0 );
			Point3 ThisPoint;
			bool bConnectToLast = false;
			float tmp;
			int iOffsetAtStartOfPoly = 0;

			std::vector<CAnmPolygon>& polys = pGlyph->GetPolygons();
			int npolys = polys.size();
			for( int ipoly=0; ipoly<npolys; ipoly++ ) {
				CAnmPolygon& Poly = polys[ipoly];

				// start of Ploygon Loop!
				//
				std::vector<CAnmOutline>& Outlines =  Poly.GetOutlines();
				int nOutlines = Outlines.size();
				bConnectToLast = false;

				// Need LastPoint, which is the very last point in the Poly.
				//

				if( nOutlines > 0 ) {
					std::vector<Point3>& PolyPoints = Outlines[nOutlines-1].GetPoints();
					int nPoints = PolyPoints.size();
					if( nPoints > 0 ) {

						LastPoint = Point3( ( PolyPoints[nPoints-1].x + hoffset) * fontsize * fontWidthScale,
										  ( PolyPoints[nPoints-1].y ) * fontsize + voffset, 0 );


					}
				}

				for( int iOutline=0; iOutline<nOutlines; iOutline++ ) {

					std::vector<Point3>& PolyPoints = Outlines[iOutline].GetPoints();
					int nPoints = PolyPoints.size();
					for( int iPoint=0; iPoint<nPoints; iPoint++ ) {

						sAnmVertex vert;
						
						TheNorm.x = 0.0;
						TheNorm.y = 1.0;
						TheNorm.z = 0.0;
						
						bool bCrease = false;

						ThisPoint = Point3( ( PolyPoints[iPoint].x + hoffset) * fontsize * fontWidthScale,
										  ( PolyPoints[iPoint].y ) * fontsize + voffset, 0 );

						if( iPoint == nPoints-1 ) {
							// Need NextPoint : first Point in next outline
							//
							int iNextOutline = 0;
							if( iOutline<nOutlines-1 ) {
								iNextOutline = iOutline+1;
							}


							std::vector<Point3>& PolyPoints = Outlines[iNextOutline].GetPoints();
							int nPoints = PolyPoints.size();
							if( nPoints > 0 ) {
								NextPoint = Point3( ( PolyPoints[0].x + hoffset) * fontsize * fontWidthScale,
												  ( PolyPoints[0].y ) * fontsize + voffset, 0 );
							}

						}
						else {
							NextPoint = Point3( ( PolyPoints[iPoint+1].x + hoffset) * fontsize * fontWidthScale, 
											  ( PolyPoints[iPoint+1].y ) * fontsize + voffset, 0 );

						}


						Point3 v1 = LastPoint - ThisPoint;
						Point3 v2 = NextPoint - ThisPoint;	

						if( v1.MagSquared() + v2.MagSquared() < 0.0000000001 ) {
							v1.x = v2.x = 1.0;
							v1.y = v2.y = 0.0;
						}

						v1.Normalize();
						v2.Normalize();

						// Rotate each by 90 degrees
						float tmp = v1.x;
						v1.x = -v1.y;
						v1.y = tmp;

						tmp = v2.x;
						v2.x = v2.y;
						v2.y = -tmp;

						// check crease angle
						//

						if( v1 * v2 < 0.5 ) {
							bCrease = true;
							TheNorm = v1;
							TheNormCrease = v2;
						}
						else {
							TheNorm.x = ( v1.x + v2.x );
							TheNorm.y = ( v1.y + v2.y );
							TheNorm.Normalize();
						}



						vert = sAnmVertex( 
								Point3( ThisPoint.x, ThisPoint.y, halfDepth ),
							TheNorm);
						m_trianglelist->AddVertex(vert);

						vert = sAnmVertex( 
								Point3( ThisPoint.x, ThisPoint.y, -halfDepth ),
							TheNorm);
						m_trianglelist->AddVertex(vert);



						if( bCrease ) {

							vert = sAnmVertex( 
								Point3( ThisPoint.x, ThisPoint.y, halfDepth ),
												TheNormCrease);
							m_trianglelist->AddVertex(vert);

							vert = sAnmVertex( 
								Point3( ThisPoint.x, ThisPoint.y, -halfDepth ),
												TheNormCrease);
							m_trianglelist->AddVertex(vert);
						}



						if( bConnectToLast ) {
							m_trianglelist->AddIndex(indexoffset-2);
							m_trianglelist->AddIndex(indexoffset-1);
							m_trianglelist->AddIndex(indexoffset);

							m_trianglelist->AddIndex(indexoffset);
							m_trianglelist->AddIndex(indexoffset-1);
							m_trianglelist->AddIndex(indexoffset+1);
						} 
						else {
							iOffsetAtStartOfPoly = indexoffset;
						}

						LastPoint = ThisPoint;

						indexoffset+=2;
						if( bCrease ) {
							indexoffset+=2;
						}

						bConnectToLast = true;


					}
				}

				// Close the Poly!
				//
				if( bConnectToLast && iOffsetAtStartOfPoly > 0 ) {
					int iEndIndex = indexoffset-2;
					

					m_trianglelist->AddIndex(iEndIndex);
					m_trianglelist->AddIndex(iEndIndex+1);
					m_trianglelist->AddIndex(iOffsetAtStartOfPoly+1);

					m_trianglelist->AddIndex(iEndIndex);
					m_trianglelist->AddIndex(iOffsetAtStartOfPoly+1);
					m_trianglelist->AddIndex(iOffsetAtStartOfPoly);


					
				}






			}
		}
	}
}



CAnmTextGlyph *** CAnmTextMesh::CreateTextGlyphs(CAnmText *pText, CAnmFont *pFont)
{
	StringArray *pStrings = pText->GetStrings();

	if (pStrings == NULL && pStrings->size() <= 0)
		return NULL;

	int i, j, nStrings;

	// allocate space for all the glyph structures
	nStrings = pStrings->size();

	CAnmTextGlyph *** pppGlyphs = new CAnmTextGlyph **[nStrings];
	for (i = 0; i < nStrings; i++)
	{
		String s = (*pStrings)[i];

		const char *cp = s->GetBuf();						// N.B.: not multibyte yet
		int len = strlen(cp);

		pppGlyphs[i] = new CAnmTextGlyph *[len];

		for (j = 0; j < len; j++)
		{
			pppGlyphs[i][j] = pFont->GetTextGlyph(cp[j]);
			if (pppGlyphs[i][j])
				pppGlyphs[i][j]->Ref();
		}
	}

	return pppGlyphs;
}

void CAnmTextMesh::DeleteTextGlyphs(CAnmTextGlyph ***pppGlyphs, CAnmText *pText)
{
	StringArray *pStrings = pText->GetStrings();

	if (pStrings == NULL && pStrings->size() <= 0)
		return;

	int i, j, nStrings;

	// delete space for all the allocated glyph structures
	nStrings = pStrings->size();

	for (i = 0; i < nStrings; i++)
	{
		String s = (*pStrings)[i];

		const char *cp = s->GetBuf();						// N.B.: not multibyte yet
		int len = strlen(cp);

		for (j = 0; j < len; j++)
		{
			SafeUnRef(pppGlyphs[i][j]);
		}

		delete pppGlyphs[i];
	}

	delete [] pppGlyphs;
}




// Get the bitmap Data ONLY IF it has been generated!
// If it has not been generated, then you dont need it, because its polygon font.
// It will return NULL if this is a polygon font.
// This must be called after Rebuild.
CAnmGraphics::Texture* CAnmTextMesh::GetTextureSurface()
{
	return m_textureSurface;
}

bool CAnmTextMesh::HasAlpha()
{
	return m_textureSurface != NULL;
}
