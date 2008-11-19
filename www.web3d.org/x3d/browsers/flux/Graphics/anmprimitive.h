/********************************************************************************
 * Flux
 *
 * File: anmprimitive.h
 * Description: Primitive class - support for triangle
 *							primitives
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

#ifndef _anmprimitive_h
#define _anmprimitive_h

#include "anmvolumes.h"

#define ANMVERTEX_DEFAULT_ULONG_COLOR 0xffffffff

struct EXPORT sAnmVertex {
	Point3 vec;
	Point3 normal;
	ULONG diffuseColor;
	Point2 texCoord;

	sAnmVertex()
	{
	}

	sAnmVertex(Point3 p)
	{
		vec = p;
		diffuseColor = ANMVERTEX_DEFAULT_ULONG_COLOR;
		texCoord = Point2(0, 0);
	}

	sAnmVertex(Point3 p, Point3 n)
	{
		vec = p;
		normal = n;
		diffuseColor = ANMVERTEX_DEFAULT_ULONG_COLOR;
		texCoord = Point2(0, 0);
	}

	sAnmVertex(Point3 p, Point3 n, Point2 tc)
	{
		vec = p;
		normal = n;
		texCoord = tc;
		diffuseColor = ANMVERTEX_DEFAULT_ULONG_COLOR;
	}

	sAnmVertex(Point3 p, Point3 n, Point2 tc, ULONG dc)
	{
		vec = p;
		normal = n;
		texCoord = tc;
		diffuseColor = dc;
	}

	sAnmVertex(Point3 p, Point3 n, Point2 tc, color4 dc)
	{
		vec = p;
		normal = n;
		texCoord = tc;
		diffuseColor = dc.MakeDWord();
	}

	sAnmVertex(Float x, Float y, Float z)
	{
		vec.x = x;
		vec.y = y;
		vec.z = z;

		diffuseColor = ANMVERTEX_DEFAULT_ULONG_COLOR;
		texCoord = Point2(0, 0);
	}

	sAnmVertex(Float x, Float y, Float z, Float nx, Float ny, Float nz)
	{
		vec.x = x;
		vec.y = y;
		vec.z = z;

		normal.x = nx;
		normal.y = ny;
		normal.z = nz;

		diffuseColor = ANMVERTEX_DEFAULT_ULONG_COLOR;
		texCoord = Point2(0, 0);
	}

	sAnmVertex(Float x, Float y, Float z, Float nx, Float ny, Float nz, Float u, Float v)
	{
		vec.x = x;
		vec.y = y;
		vec.z = z;

		normal.x = nx;
		normal.y = ny;
		normal.z = nz;

		texCoord.x = u;
		texCoord.y = v;

		diffuseColor = ANMVERTEX_DEFAULT_ULONG_COLOR;
	}

	void SetVec(Point3 p)
	{
		vec = p;
	}

	void SetNormal(Point3 p)
	{
		normal = p;
	}

	void SetDiffuseColor(ULONG c)
	{
		diffuseColor = c;
	}

	void SetDiffuseColor(color4 c)
	{
		diffuseColor = c.MakeDWord();
	}

	void SetTexCoords(Point2 p)
	{
		texCoord = p;
	}

	void Assign(float x, float y, float z)
	{
		vec.x = x;
		vec.y = y;
		vec.z = z;
	}

};


#define ANMVERTEXFLAGS (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) 

#define ANMUNLITVERTEX_DEFAULT_ULONG_COLOR 0xffffffff

struct EXPORT sAnmUnlitVertex {
	Point3 vec;
	ULONG diffuseColor;
	Point2 texCoord;

	sAnmUnlitVertex()
	{
	}

	sAnmUnlitVertex(Point3 p)
	{
		vec = p;
		diffuseColor = ANMUNLITVERTEX_DEFAULT_ULONG_COLOR;
		texCoord = Point2(0, 0);
	}

	sAnmUnlitVertex(Point3 p, Point2 tc)
	{
		vec = p;
		texCoord = tc;
		diffuseColor = ANMUNLITVERTEX_DEFAULT_ULONG_COLOR;
	}

	sAnmUnlitVertex(Point3 p, Point2 tc, ULONG dc)
	{
		vec = p;
		texCoord = tc;
		diffuseColor = dc;
	}

	sAnmUnlitVertex(Point3 p, Point2 tc, color4 dc)
	{
		vec = p;
		texCoord = tc;
		diffuseColor = dc.MakeDWord();
	}

	sAnmUnlitVertex(Float x, Float y, Float z)
	{
		vec.x = x;
		vec.y = y;
		vec.z = z;

		diffuseColor = ANMUNLITVERTEX_DEFAULT_ULONG_COLOR;
		texCoord = Point2(0, 0);
	}

	sAnmUnlitVertex(Float x, Float y, Float z, Float u, Float v)
	{
		vec.x = x;
		vec.y = y;
		vec.z = z;

		texCoord.x = u;
		texCoord.y = v;

		diffuseColor = ANMUNLITVERTEX_DEFAULT_ULONG_COLOR;
	}

	void SetVec(Point3 p)
	{
		vec = p;
	}

	void SetDiffuseColor(ULONG c)
	{
		diffuseColor = c;
	}

	void SetDiffuseColor(color4 c)
	{
		diffuseColor = c.MakeDWord();
	}

	void SetTexCoords(Point2 p)
	{
		texCoord = p;
	}
};

#define ANMUNLITVERTEXFLAGS (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) 


typedef std::vector<sAnmVertex> tAnmVertexArray;
typedef std::vector<sAnmUnlitVertex> tAnmUnlitVertexArray;
typedef std::vector<WORD>	tAnmIndexArray;
#define ANMMAXVERTICES		65536
#define ANMMAXINDICES		65536

struct EXPORT sAnmIndexedFaceList {
	int vertexcount;
	Point3 *vertices;
	int normalcount;
	Point3 *normals;
	int colorcount;
	Color *colors;
	int uvcount;
	Point2 *uvs;
	int vindexcount;
	int *vindices;
	int nindexcount;
	int *nindices;
	int cindexcount;
	int *cindices;
	int tindexcount;
	int *tindices;
	BOOL ccwflag;
};

class EXPORT CAnmPrimitive
{
protected:

	class CAnmGraphics::RenderPrimitive *m_renderprimitive;

	eAnmPrimitiveType				 m_primtype;
	tAnmVertexArray					 m_vertices;
	tAnmIndexArray					 m_indices;
	bool							 m_indexed;
	ULONG							 m_diffuseColor;
	CAnmBoundingBox					 m_bbox;
	class CAnmOctree				*m_octree;
	class CAnmPrimitive				*m_backfaces;
	int								 m_texturestage;

	virtual void InvalidateBoundingBox();
	virtual void GenBoundingBox();
	void MaybeMakeOctree();
	virtual eAnmReturnStatus UpdatePrimitive();

public:

	// constructor/destructor
	CAnmPrimitive(class CAnmDevice *pDevice, eAnmPrimitiveType ptype = eAnmPrimitiveBadType, bool indexed = FALSE);
	virtual ~CAnmPrimitive();
	
	// Methods
	virtual void Render(class CAnmDevice *pDevice);

	virtual void AddVertex(sAnmVertex vert);
	virtual void AddIndex(WORD index);
	virtual void AddVertices(int vertexcount, sAnmVertex *pVertices);
	virtual void AddIndices(int indexcount, WORD *pVertices);
	virtual void SetVertex(int vindex, sAnmVertex v);
	virtual void SetCoordinates(Point3Array *pCoords);
	sAnmVertex GetVertex(int vindex);

	virtual eAnmReturnStatus GenerateNormals(Float creaseAngle) PURE;
	virtual void GenerateBackfaces(class CAnmDevice *pDevice) PURE;
	virtual eAnmReturnStatus BufferVertices(CAnmDevice *pDevice);
	virtual void Sort();

	virtual void GetBoundingBox(class CAnmBoundingBox *pBoundingBox);
	virtual bool TestPick(Point3 start, Point3 end, struct sAnmRenderGraphPickList *pPickList);
	virtual bool SphereCollide(Point3 center, float radius, struct sAnmRenderGraphPickList *pPickList);
	virtual bool CapsuleCollide(const Point3 &capsulePt0, const Point3 &capsulePt1, const Point3& capsuleRadius,
		struct sAnmRenderGraphPickList *pPickList);

	virtual int NumTriangles() PURE;
	virtual void GetTriangle(int index, tAnmTriangle &triangle) PURE;

	// For convenience; sets properties on all vertices
	virtual void SetDiffuseColor(ULONG diffuseColor);
	virtual void SetDiffuseColor(color4 diffuseColor);
	virtual void Scale(Float scalefactor);

	// Accessors
	virtual eAnmPrimitiveType GetPrimitiveType()
	{
		return m_primtype;
	}

	virtual tAnmVertexArray *GetVertices()
	{
		return &m_vertices;
	}

	virtual tAnmIndexArray *GetIndices()
	{
		return &m_indices;
	}

	virtual bool IsIndexed()
	{
		return m_indexed;
	}

	virtual bool ValidIndex(int index)
	{
		return index >= 0 && index < ANMMAXVERTICES;
	}

	void SetTextureStage(int stage)
	{
		m_texturestage = stage;
	}

	int GetTextureStage()
	{
		return m_texturestage;
	}

	class CAnmOctree *GetOctree()
	{
		return m_octree;
	}

	virtual bool IsLit()
	{
		return true;
	}

};

class EXPORT CAnmTriangleList : public CAnmPrimitive
{
protected :
	

public :
	CAnmTriangleList(CAnmDevice *pDevice, bool indexed = false) : 
		CAnmPrimitive(pDevice, eAnmPrimitiveTriangleList, indexed)
	{
	}

		
	// CAnmPrimitive overrides
	virtual eAnmReturnStatus GenerateNormals(Float creaseAngle);
	virtual void GenerateBackfaces(class CAnmDevice *pDevice);
	virtual int NumTriangles();
	virtual void GetTriangle(int index, tAnmTriangle &triangle);
};

class EXPORT CAnmIndexedTriangleList : public CAnmTriangleList
{

public :
	CAnmIndexedTriangleList(CAnmDevice *pDevice) : CAnmTriangleList(pDevice, true)
	{
		m_backfaces = NULL;
	}

	~CAnmIndexedTriangleList()
	{
		SafeDelete(m_backfaces);
	}

	// CAnmPrimitive/CAnmTriangleList overrides
	virtual eAnmReturnStatus GenerateNormals(Float creaseAngle);
	virtual void GenerateBackfaces(class CAnmDevice *pDevice);
	virtual int NumTriangles();
	virtual void GetTriangle(int index, tAnmTriangle &triangle);
};

class EXPORT CAnmTriangleStrip : public CAnmPrimitive
{
public :
	CAnmTriangleStrip(CAnmDevice *pDevice, bool indexed = FALSE) : CAnmPrimitive(pDevice, eAnmPrimitiveTriangleStrip, indexed)
	{
	}
	virtual eAnmReturnStatus GenerateNormals(Float creaseAngle);
	virtual void GenerateBackfaces(class CAnmDevice *pDevice);
	virtual int NumTriangles();
	virtual void GetTriangle(int index, tAnmTriangle &triangle);
};

class EXPORT CAnmIndexedTriangleStrip : public CAnmTriangleStrip
{
public :
	CAnmIndexedTriangleStrip(CAnmDevice *pDevice) : CAnmTriangleStrip(pDevice, true)
	{
	}
	virtual eAnmReturnStatus GenerateNormals(Float creaseAngle);
	virtual void GenerateBackfaces(class CAnmDevice *pDevice);
	virtual int NumTriangles();
	virtual void GetTriangle(int index, tAnmTriangle &triangle);
};

class EXPORT CAnmTriangleFan : public CAnmPrimitive
{
public :
	CAnmTriangleFan(CAnmDevice *pDevice, bool indexed = false) : CAnmPrimitive(pDevice, eAnmPrimitiveTriangleFan, indexed)
	{
	}
	virtual eAnmReturnStatus GenerateNormals(Float creaseAngle);
	virtual void GenerateBackfaces(class CAnmDevice *pDevice);
	virtual int NumTriangles();
	virtual void GetTriangle(int index, tAnmTriangle &triangle);
};

class EXPORT CAnmIndexedTriangleFan : public CAnmTriangleFan
{
public :
	CAnmIndexedTriangleFan(CAnmDevice *pDevice) : CAnmTriangleFan(pDevice, true)
	{
	}
	virtual eAnmReturnStatus GenerateNormals(Float creaseAngle);
	virtual void GenerateBackfaces(class CAnmDevice *pDevice);
	virtual int NumTriangles();
	virtual void GetTriangle(int index, tAnmTriangle &triangle);
};

class EXPORT CAnmUnlitPrimitive : public CAnmPrimitive
{
protected:

	tAnmUnlitVertexArray			 m_unlitVertices;

	virtual void GenBoundingBox();
	virtual eAnmReturnStatus UpdatePrimitive();

public:

	// constructor/destructor
	CAnmUnlitPrimitive(CAnmDevice *pDevice, eAnmPrimitiveType ptype = eAnmPrimitiveBadType, bool indexed = FALSE)
		: CAnmPrimitive(pDevice, ptype, indexed)
	{
	}

	virtual ~CAnmUnlitPrimitive()
	{
	}
	
	// CAnmPrimitive overrides
	virtual void Render(class CAnmDevice *pDevice);
	virtual void SetCoordinates(Point3Array *pCoords);

	// New methods
	virtual void AddUnlitVertices(int vertexcount, sAnmUnlitVertex *pUnlitVertices);
	virtual void SetUnlitVertex(int vindex, sAnmUnlitVertex v);
	sAnmUnlitVertex GetUnlitVertex(int vindex);

	virtual eAnmReturnStatus BufferVertices(CAnmDevice *pDevice);

	// For convenience; sets properties on all vertices
	virtual void SetDiffuseColor(ULONG diffuseColor);
	virtual void SetDiffuseColor(color4 diffuseColor);
	virtual void Scale(Float scalefactor);

	// Accessors
	virtual tAnmUnlitVertexArray *GetUnlitVertices()
	{
		return &m_unlitVertices;
	}

	virtual bool IsLit()
	{
		return false;
	}
};

class EXPORT CAnmPolyLine : public CAnmUnlitPrimitive
{
public :
	CAnmPolyLine(CAnmDevice *pDevice, bool indexed = false) : CAnmUnlitPrimitive(pDevice, eAnmPrimitivePolyLine, indexed)
	{
	}

	// triangle ops are no-ops for a polyline
	virtual eAnmReturnStatus GenerateNormals(Float creaseAngle)
	{
		return eAnmReturnAllGood;
	}

	virtual void GenerateBackfaces(class CAnmDevice *pDevice)
	{
	}

	virtual int NumTriangles()
	{
		return 0;
	}

	virtual void GetTriangle(int index, tAnmTriangle &triangle)
	{
	}

};

class EXPORT CAnmPointList : public CAnmUnlitPrimitive
{
public :
	CAnmPointList(CAnmDevice *pDevice, bool indexed = false) : CAnmUnlitPrimitive(pDevice, eAnmPrimitivePointList, indexed)
	{
	}

	// triangle ops are no-ops for a point list
	virtual eAnmReturnStatus GenerateNormals(Float creaseAngle)
	{
		return eAnmReturnAllGood;
	}

	virtual void GenerateBackfaces(class CAnmDevice *pDevice)
	{
	}

	virtual int NumTriangles()
	{
		return 0;
	}

	virtual void GetTriangle(int index, tAnmTriangle &triangle)
	{
	}

};

typedef std::vector<CAnmPrimitive *> tAnmPrimitiveArray;

#endif // _anmprimitive_h
