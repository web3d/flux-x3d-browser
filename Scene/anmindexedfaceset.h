/********************************************************************************
 * Flux
 *
 * File: anmindexedfaceset.h
 * Description: IndexedFaceSet node
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

#ifndef _anmindexedfaceset_h
#define _anmindexedfaceset_h

#define ANMIFS_DEFAULT_CCW					TRUE
#define ANMIFS_DEFAULT_COLORPERVERTEX		TRUE
#define ANMIFS_DEFAULT_CONVEX				TRUE
#define ANMIFS_DEFAULT_CREASEANGLE			0.f
#define ANMIFS_DEFAULT_NORMALPERVERTEX		TRUE
#define ANMIFS_DEFAULT_SOLID				TRUE

#include "anmcomposedgeometry.h"
#include "anmprimitive.h"

class CAnmIndexedFaceSet  : public CAnmComposedGeometry
{
protected :

	IntegerArray				*m_colorIndex;			// array of vertex colors
	Boolean						 m_convex;				// convex shape hint
	IntegerArray				*m_coordIndex;			// list of triangles
	Float						 m_creaseAngle;			// crease angle for normals
	IntegerArray				*m_normalIndex;			// list of normals by vertex
	IntegerArray				*m_texCoordIndex;		// list of tex coords by vertex

	virtual void CreateMesh();

	virtual void CreateFaces();

	virtual void CreateFaces(int texturestage, Point3Array *pCoordinates, Point3Array *pNormalValues, 
								ColorArray *pColorValues, Point2Array *pTexCoordValues, float* pGeneratedNormalValues,
								int nVerts, int nNormals, int nColors, int nTexCoords, 
								int *pNormalIndices, int *pColorIndices, int *pTexCoordIndices, int* pGeneratedNormalIndex,
								int nIndices, int nNormalIndices, int nColorIndices, int nTexCoordIndices,
								bool generateUVs);
	virtual void CreateFacesNonIndexedTriList(int texturestage, Point3Array *pCoordinates, Point3Array *pNormalValues, 
								ColorArray *pColorValues, Point2Array *pTexCoordValues, float* pGeneratedNormalValues,
								int nVerts, int nNormals, int nColors, int nTexCoords, 
								int *pNormalIndices, int *pColorIndices, int *pTexCoordIndices, int* pGeneratedNormalIndex,
								int nIndices, int nNormalIndices, int nColorIndices, int nTexCoordIndices,
								bool generateUVs);
	virtual void CreateFacesIndexedTriList(int texturestage, Point3Array *pCoordinates, Point3Array *pNormalValues, 
								ColorArray *pColorValues, Point2Array *pTexCoordValues, float* pGeneratedNormalValues,
								int nVerts, int nNormals, int nColors, int nTexCoords, 
								int *pNormalIndices, int *pColorIndices, int *pTexCoordIndices, int* pGeneratedNormalIndex,
								int nIndices, int nNormalIndices, int nColorIndices, int nTexCoordIndices,
								bool generateUVs);

	virtual int *Tesselate(Point3Array *pCoords, int *indices, int &numIndices);

	bool PreGenerateNormals( float** ppNormalArray, int** ppNormIndexArray, int &nGeneratedNormals );
	static bool PreGenerateNormals( float** ppNormalArray, int** ppNormIndexArray, int &nGeneratedNormals,
		 Point3Array*  pCoordinates,
		 IntegerArray* coordIndex,
		 bool ccw,
		 float creaseAngle );

	void CAnmIndexedFaceSet::GenerateTextureCoordinates( 
				Point2Array* &pTexCoordValues, 
				int& nTexCoords, 
				Point3Array* pCoordinates, 
				int& nVerts );

public:

	CAnmIndexedFaceSet();
	~CAnmIndexedFaceSet();

	// CAnmComposedGeometry overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	// New methods

	// Accessors
	void SetColorIndices(IntegerArray *pColorIndices);
	IntegerArray *GetColorIndices() { return m_colorIndex; }
	void GetColorIndices(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_colorIndex;
	}

	void SetConvex(Boolean convex);
	Boolean GetConvex() { return m_convex; }
	void GetConvex(Boolean *pVal)
	{ 
		if (pVal)
			*pVal = m_convex;
	}
	
	void SetCoordIndices(IntegerArray *pCoordIndices);
	IntegerArray *GetCoordIndices() { return m_coordIndex; }
	void GetCoordIndices(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_coordIndex;
	}

	void SetCreaseAngle(Float creaseAngle);
	Float GetCreaseAngle() { return m_creaseAngle; }
	void GetCreaseAngle(Float *pVal)
	{ 
		if (pVal)
			*pVal = m_creaseAngle;
	}

	void SetNormalIndices(IntegerArray *pNormalIndices);
	IntegerArray *GetNormalIndices() { return m_normalIndex; }
	void GetNormalIndices(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_normalIndex;
	}

	void SetTexCoordIndices(IntegerArray *pTexCoordIndices);
	IntegerArray *GetTexCoordIndices() { return m_texCoordIndex; }
	void GetTexCoordIndices(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_texCoordIndex;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmIndexedFaceSet);

	DECLARE_VALUE(ccw);
	DECLARE_VALUE(color);
	DECLARE_VALUE(colorIndex);
	DECLARE_VALUE(colorPerVertex);
	DECLARE_VALUE(convex);
	DECLARE_VALUE(coord);
	DECLARE_VALUE(coordIndex);
	DECLARE_VALUE(creaseAngle);
	DECLARE_VALUE(normal);
	DECLARE_VALUE(normalIndex);
	DECLARE_VALUE(normalPerVertex);
	DECLARE_VALUE(solid);
	DECLARE_VALUE(texCoord);
	DECLARE_VALUE(texCoordIndex);

	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_colorIndex);
	DECLARE_METHOD(set_coord);
	DECLARE_METHOD(set_coordIndex);
	DECLARE_METHOD(set_normal);
	DECLARE_METHOD(set_normalIndex);
	DECLARE_METHOD(set_texCoord);
	DECLARE_METHOD(set_texCoordIndex);

	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(coord_changed);
	DECLARE_CALLBACK(normal_changed);
	DECLARE_CALLBACK(texCoord_changed);

};

#define ANMIFS_COLORINDEXDIRTY			ANMCG_NDIRTYBITS
#define ANMIFS_CONVEXDIRTY				(ANMIFS_COLORINDEXDIRTY + 1)
#define ANMIFS_COORDINDEXDIRTY			(ANMIFS_COLORINDEXDIRTY + 2)
#define ANMIFS_CREASEANGLEDIRTY			(ANMIFS_COLORINDEXDIRTY + 3)
#define ANMIFS_NORMALINDEXDIRTY			(ANMIFS_COLORINDEXDIRTY + 4)
#define ANMIFS_TEXCOORDINDEXDIRTY		(ANMIFS_COLORINDEXDIRTY + 4)
#define ANMIFS_NDIRTYBITS				(ANMIFS_TEXCOORDINDEXDIRTY + 1)

typedef enum eAnmIFSDirtyFlags {
eAnmIFSColorIndexDirty			= ANMDIRTYBIT(ANMIFS_COLORINDEXDIRTY),
eAnmIFSConvexDirty				= ANMDIRTYBIT(ANMIFS_CONVEXDIRTY),
eAnmIFSCoordIndexDirty			= ANMDIRTYBIT(ANMIFS_COORDINDEXDIRTY),
eAnmIFSCreaseAngleDirty			= ANMDIRTYBIT(ANMIFS_CREASEANGLEDIRTY),
eAnmIFSNormalIndexDirty			= ANMDIRTYBIT(ANMIFS_NORMALINDEXDIRTY),
eAnmIFSTexCoordIndexDirty		= ANMDIRTYBIT(ANMIFS_TEXCOORDINDEXDIRTY),

} eAnmIFSDirtyFlags;

#endif // _anmindexedfaceset_h
