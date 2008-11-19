/********************************************************************************
 * Flux
 *
 * File: anmcomposedgeometry.h
 * Description: ComposedGeometry node; base for
 *							geometry types composed from separate
 *							Color, Normal, TextureCoordinate nodes
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

#ifndef _anmcomposedgeometry_h
#define _anmcomposedgeometry_h

#define ANMCG_DEFAULT_CCW					TRUE
#define ANMCG_DEFAULT_COLORPERVERTEX		TRUE
#define ANMCG_DEFAULT_NORMALPERVERTEX		TRUE

#include "anmgeometry.h"

class CAnmComposedGeometry  : public CAnmGeometry
{
protected :
	
	Boolean						 m_ccw;					// counter-clockwise flag
	Boolean						 m_colorPerVertex;		// vertex color flag
	Boolean						 m_normalPerVertex;		// vertex normal flag

	CAnmNode					*m_color;
	CAnmNode					*m_coord;
	CAnmNode					*m_normal;
	CAnmNode					*m_texCoord;

public:

	CAnmComposedGeometry();
	~CAnmComposedGeometry();

	// CAnmGeometry overrides
	virtual eAnmReturnStatus AddChild(CAnmNode *pChild);
	virtual eAnmReturnStatus RemoveChild(CAnmNode *pChild);
	virtual void ChildChanged(CAnmNode *pChild);

	virtual eAnmRenderOptions GetRenderOptions();
	virtual class CAnmNode *GetTextureCoordinate()
	{
		return GetTexCoord();
	}

	// New methods

	// Accessors
	void SetCCW(Boolean ccw);
	Boolean	GetCCW() { return m_ccw; }
	void GetCCW(Boolean *pVal)
	{ 
		if (pVal)
			*pVal = m_ccw;
	}

	void SetColor(CAnmNode *pColor);
	class CAnmNode *GetColor() { return m_color; }
	void GetColor(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_color;
	}

	void SetColorPerVertex(Boolean colorPerVertex);
	Boolean	GetColorPerVertex() { return m_colorPerVertex; }
	void GetColorPerVertex(Boolean *pVal)
	{ 
		if (pVal)
			*pVal = m_colorPerVertex;
	}

	void SetCoord(CAnmNode *pCoordinate);
	class CAnmNode *GetCoord() { return m_coord; }
	void GetCoord(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_coord;
	}
	
	void SetNormal(CAnmNode *pNormal);
	class CAnmNode *GetNormal() { return m_normal; }
	void GetNormal(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_normal;
	}

	void SetNormalPerVertex(Boolean normalPerVertex);
	Boolean	GetNormalPerVertex() { return m_normalPerVertex; }
	void GetNormalPerVertex(Boolean *pVal)
	{ 
		if (pVal)
			*pVal = m_normalPerVertex;
	}

	void SetTexCoord(CAnmNode *pTexCoord);
	class CAnmNode *GetTexCoord() { return m_texCoord; }
	void GetTexCoord(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_texCoord;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmComposedGeometry);

	DECLARE_VALUE(ccw);
	DECLARE_VALUE(color);
	DECLARE_VALUE(colorPerVertex);
	DECLARE_VALUE(coord);
	DECLARE_VALUE(normal);
	DECLARE_VALUE(normalPerVertex);
	DECLARE_VALUE(solid);
	DECLARE_VALUE(texCoord);

	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_coord);
	DECLARE_METHOD(set_normal);
	DECLARE_METHOD(set_texCoord);

	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(coord_changed);
	DECLARE_CALLBACK(normal_changed);
	DECLARE_CALLBACK(texCoord_changed);

};

#define ANMCG_CCWDIRTY					(ANMGEOMETRY_NDIRTYBITS)
#define ANMCG_COLORDIRTY				(ANMCG_CCWDIRTY + 1)
#define ANMCG_COLORPVDIRTY				(ANMCG_CCWDIRTY + 2)
#define ANMCG_COORDDIRTY				(ANMCG_CCWDIRTY + 3)
#define ANMCG_NORMALDIRTY				(ANMCG_CCWDIRTY + 4)
#define ANMCG_NORMALPVDIRTY				(ANMCG_CCWDIRTY + 5)
#define ANMCG_TEXCOORDDIRTY				(ANMCG_CCWDIRTY + 6)
#define ANMCG_NDIRTYBITS				(ANMCG_TEXCOORDDIRTY + 1)

typedef enum eAnmCGDirtyFlags {
eAnmCGCCWDirty					= ANMDIRTYBIT(ANMCG_CCWDIRTY),
eAnmCGColorDirty				= ANMDIRTYBIT(ANMCG_COLORDIRTY),
eAnmCGColorPerVertexDirty		= ANMDIRTYBIT(ANMCG_COLORPVDIRTY),
eAnmCGCoordDirty				= ANMDIRTYBIT(ANMCG_COORDDIRTY),
eAnmCGNormalDirty				= ANMDIRTYBIT(ANMCG_NORMALDIRTY),
eAnmCGNormalPerVertexDirty		= ANMDIRTYBIT(ANMCG_NORMALPVDIRTY),
eAnmCGTexCoordDirty				= ANMDIRTYBIT(ANMCG_TEXCOORDDIRTY),
} eAnmCGDirtyFlags;

#endif // _anmcomposedgeometry_h
