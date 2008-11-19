/********************************************************************************
 * Flux
 *
 * File: anmelevationgrid.h
 * Description: ElevationGrid node
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

#ifndef _anmelevationgrid_h
#define _anmelevationgrid_h

#define ANMELV_DEFAULT_CCW					TRUE
#define ANMELV_DEFAULT_COLORPERVERTEX		TRUE
#define ANMELV_DEFAULT_CREASEANGLE			0.f
#define ANMELV_DEFAULT_NORMALPERVERTEX		TRUE
#define ANMELV_DEFAULT_SOLID				TRUE
#define ANMELV_DEFAULT_XDIMENSION			0.f
#define ANMELV_DEFAULT_XSPACING				1.0f
#define ANMELV_DEFAULT_ZDIMENSION			0.f
#define ANMELV_DEFAULT_ZSPACING				1.0f

#include "anmcomposedgeometry.h"


class CAnmElevationGrid  : public CAnmComposedGeometry
{
protected :
	
	Float						 m_creaseAngle;			// crease angle for normals
	FloatArray					*m_height;
	Integer						 m_xDimension;
	Float						 m_xSpacing;
	Integer						 m_zDimension;
	Float						 m_zSpacing;

	virtual void CreateMesh();
	bool BuildGrid();
	bool BuildGridIndexedTriSet();
	bool BuildGridNonIndexedTriSet();
	void AddTriListToMesh( class CAnmTriangleList *pTriangleList, bool bGenNormals, bool bBufferVerts );
	Point3Array* CAnmElevationGrid::CalculateNormals();

public:

	CAnmElevationGrid();
	~CAnmElevationGrid();

	// CAnmComposedGeometry overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	// New methods

	// Accessors
	void SetCreaseAngle(Float creaseAngle);
	Float GetCreaseAngle() { return m_creaseAngle; }
	void GetCreaseAngle(Float *pVal)
	{ 
		if (pVal)
			*pVal = m_creaseAngle;
	}

	void SetHeight(FloatArray *pHeight);
	FloatArray *GetHeight() { return m_height; }
	void GetHeight(FloatArray **pVal)
	{ 
		if (pVal)
			*pVal = m_height;
	}

	void SetXDimension(Integer xDimension);
	Integer	GetXDimension() { return m_xDimension; }
	void GetXDimension(Integer *pVal)
	{ 
		if (pVal)
			*pVal = m_xDimension;
	}

	void SetXSpacing(Float xSpacing);
	Float	GetXSpacing() { return m_xSpacing; }
	void GetXSpacing(Float *pVal)
	{ 
		if (pVal)
			*pVal = m_xSpacing;
	}

	void SetZDimension(Integer zDimension);
	Integer	GetZDimension() { return m_zDimension; }
	void GetZDimension(Integer *pVal)
	{ 
		if (pVal)
			*pVal = m_zDimension;
	}

	void SetZSpacing(Float zSpacing);
	Float	GetZSpacing() { return m_zSpacing; }
	void GetZSpacing(Float *pVal)
	{ 
		if (pVal)
			*pVal = m_zSpacing;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmElevationGrid);

	DECLARE_VALUE(ccw);
	DECLARE_VALUE(color);
	DECLARE_VALUE(colorPerVertex);
	DECLARE_VALUE(creaseAngle);
	DECLARE_VALUE(height);
	DECLARE_VALUE(normal);
	DECLARE_VALUE(normalPerVertex);
	DECLARE_VALUE(solid);
	DECLARE_VALUE(texCoord);
	DECLARE_VALUE(xDimension);
	DECLARE_VALUE(xSpacing);
	DECLARE_VALUE(zDimension);
	DECLARE_VALUE(zSpacing);

	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_height);
	DECLARE_METHOD(set_normal);
	DECLARE_METHOD(set_texCoord);

	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(normal_changed);
	DECLARE_CALLBACK(texCoord_changed);

};

#define ANMELGRID_HEIGHTDIRTY				ANMCG_NDIRTYBITS
#define ANMELGRID_NDIRTYBITS				(ANMELGRID_HEIGHTDIRTY + 1)

typedef enum eAnmELVDirtyFlags {
eAnmELVHeightDirty				= ANMDIRTYBIT(ANMELGRID_HEIGHTDIRTY),
} eAnmELVDirtyFlags;

#endif // _anmelevationgrid_h
