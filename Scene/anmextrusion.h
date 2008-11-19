/********************************************************************************
 * Flux
 *
 * File: anmextrusion.h
 * Description: Extrusion node
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

#ifndef _anmextrusion_h
#define _anmextrusion_h

#define ANMEXTRUSION_DEFAULT_BEGINCAP				TRUE
#define ANMEXTRUSION_DEFAULT_CCW					TRUE
#define ANMEXTRUSION_DEFAULT_CONVEX					TRUE
#define ANMEXTRUSION_DEFAULT_CREASEANGLE			0.f
#define ANMEXTRUSION_DEFAULT_ENDCAP					TRUE

#include "anmgeometry.h"

class CAnmExtrusion  : public CAnmGeometry
{
protected :
	
	Boolean						 m_beginCap;			// cap poly at beginning?
	Boolean						 m_ccw;					// counter-clockwise flag
	Boolean						 m_convex;				// are cross-sections convex?
	Float						 m_creaseAngle;			// crease angle for normal gen
	Point2Array					*m_crossSection;		// 2D cross-sections
	Boolean						 m_endCap;				// cap poly at end?
	RotationArray				*m_orientation;			// twists
	Point2Array					*m_scale;				// 2D scales for cross-sections
	Point3Array					*m_spine;				// the spine

	virtual void AddCap( bool bFlip, Point3* pVertCoord, Point2* pUVCoords, int ncapverts, struct sAnmVertex *capverts );
	virtual int *Tesselate(int vertexcount, struct sAnmVertex *pVerts, int &numIndices);

	virtual void CreateMesh();

	matrix4* GenerateSPCMatrixArray();
	Point3* CAnmExtrusion::GenerateVertexCoords( matrix4* pSPCMatrixArray );
	void AddTriListToMesh( class CAnmTriangleList *pTriangleList, bool bGenNormals, bool bBufferVerts );
	Point3* CAnmExtrusion::ComputeNormals( Point3* pVertCoords, int **ppIndexes );
	float* CAnmExtrusion::ComputeCsxTextureCoords();
	float* CAnmExtrusion::ComputeSpineTextureCoords();

public:

	CAnmExtrusion();
	~CAnmExtrusion();

	// CAnmGeometry overrides
	virtual void Realize();							// build lower-level rendering structures
	virtual void Update();							// re-render/reset rendering structures

	// New methods

	// Accessors
	void SetBeginCap(Boolean beginCap);
	Boolean	GetBeginCap() { return m_beginCap; }
	void GetBeginCap(Boolean *pVal)
	{ 
		if (pVal)
			*pVal = m_beginCap;
	}

	void SetCCW(Boolean ccw);
	Boolean	GetCCW() { return m_ccw; }
	void GetCCW(Boolean *pVal)
	{ 
		if (pVal)
			*pVal = m_ccw;
	}

	void SetConvex(Boolean convex);
	Boolean GetConvex() { return m_convex; }
	void GetConvex(Boolean *pVal)
	{ 
		if (pVal)
			*pVal = m_convex;
	}
	
	void SetCreaseAngle(Float creaseAngle);
	Float GetCreaseAngle() { return m_creaseAngle; }
	void GetCreaseAngle(Float *pVal)
	{ 
		if (pVal)
			*pVal = m_creaseAngle;
	}

	void SetCrossSection(Point2Array *pCrossSection);
	Point2Array	*GetCrossSection() { return m_crossSection; }
	void GetCrossSection(Point2Array **pVal)
	{ 
		if (pVal)
			*pVal = m_crossSection;
	}

	void SetEndCap(Boolean endCap);
	Boolean	GetEndCap() { return m_endCap; }
	void GetEndCap(Boolean *pVal)
	{ 
		if (pVal)
			*pVal = m_endCap;
	}

	void SetOrientation(RotationArray *pOrientation);
	RotationArray *GetOrientation() { return m_orientation; }
	void GetOrientation(RotationArray **pVal)
	{ 
		if (pVal)
			*pVal = m_orientation;
	}

	void SetScale(Point2Array *pScale);
	Point2Array	*GetScale() { return m_scale; }
	void GetScale(Point2Array **pVal)
	{ 
		if (pVal)
			*pVal = m_scale;
	}

	void SetSpine(Point3Array *pSpine);
	Point3Array	*GetSpine() { return m_spine; }
	void GetSpine(Point3Array **pVal)
	{ 
		if (pVal)
			*pVal = m_spine;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmExtrusion);

	DECLARE_VALUE(beginCap);
	DECLARE_VALUE(ccw);
	DECLARE_VALUE(convex);
	DECLARE_VALUE(creaseAngle);
	DECLARE_VALUE(crossSection);
	DECLARE_VALUE(endCap);
	DECLARE_VALUE(orientation);
	DECLARE_VALUE(scale);
	DECLARE_VALUE(solid);
	DECLARE_VALUE(spine);

	DECLARE_METHOD(set_crossSection);
	DECLARE_METHOD(set_orientation);
	DECLARE_METHOD(set_scale);
	DECLARE_METHOD(set_spine);

};

#endif // _anmextrusion_h
