/********************************************************************************
 * Flux
 *
 * File: anmindexedlineset.h
 * Description: IndexedLineSet node
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

#ifndef _anmindexedlineset_h
#define _anmindexedlineset_h

#define ANMILS_DEFAULT_COLORPERVERTEX		TRUE

#include "anmgeometry.h"

class CAnmIndexedLineSet  : public CAnmGeometry
{

	IntegerArray			*m_colorIndex;			// array of vertex colors
	Boolean					 m_colorPerVertex;		// vertex color flag
	IntegerArray			*m_coordIndex;			// list of triangles

	CAnmNode				*m_color;
	CAnmNode				*m_coord;

	virtual void CreateMesh();

public:

	CAnmIndexedLineSet();
	~CAnmIndexedLineSet();

	// CAnmGeometry overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual eAnmReturnStatus AddChild(CAnmNode *pChild);
	virtual eAnmReturnStatus RemoveChild(CAnmNode *pChild);
	virtual void ChildChanged(CAnmNode *pChild);

	virtual eAnmRenderOptions GetRenderOptions();

	// New methods


	// Accessors
	void SetColor(CAnmNode *pColor);
	class CAnmNode *GetColor() { return m_color; }
	void GetColor(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_color;
	}

	void SetColorIndices(IntegerArray *pColorIndices);
	IntegerArray *GetColorIndices() { return m_colorIndex; }
	void GetColorIndices(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_colorIndex;
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
	
	void SetCoordIndices(IntegerArray *pCoordIndices);
	IntegerArray *GetCoordIndices() { return m_coordIndex; }
	void GetCoordIndices(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_coordIndex;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmIndexedLineSet);

	DECLARE_VALUE(color);
	DECLARE_VALUE(colorIndex);
	DECLARE_VALUE(colorPerVertex);
	DECLARE_VALUE(coord);
	DECLARE_VALUE(coordIndex);

	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_colorIndex);
	DECLARE_METHOD(set_colorPerVertex);
	DECLARE_METHOD(set_coord);
	DECLARE_METHOD(set_coordIndex);

	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(colorIndex_changed);
	DECLARE_CALLBACK(colorPerVertex_changed);
	DECLARE_CALLBACK(coord_changed);
	DECLARE_CALLBACK(coordIndex_changed);
};

#define ANMILS_COLORDIRTY				ANMGEOMETRY_NDIRTYBITS
#define ANMILS_COLORINDEXDIRTY			(ANMILS_COLORDIRTY + 1)
#define ANMILS_COLORPVDIRTY				(ANMILS_COLORDIRTY + 2)
#define ANMILS_COORDDIRTY				(ANMILS_COLORDIRTY + 3)
#define ANMILS_COORDINDEXDIRTY			(ANMILS_COLORDIRTY + 4)

typedef enum eAnmILSDirtyFlags {
eAnmILSColorDirty				= ANMDIRTYBIT(ANMILS_COLORDIRTY),
eAnmILSColorIndexDirty			= ANMDIRTYBIT(ANMILS_COLORINDEXDIRTY),
eAnmILSColorPerVertexDirty		= ANMDIRTYBIT(ANMILS_COLORPVDIRTY),
eAnmILSCoordDirty				= ANMDIRTYBIT(ANMILS_COORDDIRTY),
eAnmILSCoordIndexDirty			= ANMDIRTYBIT(ANMILS_COORDINDEXDIRTY),
} eAnmILSDirtyFlags;

#endif // _anmindexedlineset_h
