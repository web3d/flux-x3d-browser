/********************************************************************************
 * Flux
 *
 * File: anmindexedtriangleset.h
 * Description: IndexedTriangleSet node
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

#ifndef _anmindexedtriangleset_h
#define _anmindexedtriangleset_h

#include "anmcomposedgeometry.h"

class CAnmIndexedTriangleSet  : public CAnmComposedGeometry
{
protected :

	IntegerArray				*m_index;				// array of indices; 3 per triangle

	virtual void CreateMesh();

public:

	CAnmIndexedTriangleSet();
	~CAnmIndexedTriangleSet();

	// CAnmComposedGeometry overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	// New methods

	// Accessors
	void SetIndex(IntegerArray *pIndex);
	IntegerArray *GetIndex() { return m_index; }
	void GetIndex(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_index;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmIndexedTriangleSet);

	DECLARE_VALUE(ccw);
	DECLARE_VALUE(color);
	DECLARE_VALUE(colorPerVertex);
	DECLARE_VALUE(coord);
	DECLARE_VALUE(index);
	DECLARE_VALUE(normal);
	DECLARE_VALUE(normalPerVertex);
	DECLARE_VALUE(solid);
	DECLARE_VALUE(texCoord);

	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_coord);
	DECLARE_METHOD(set_index);
	DECLARE_METHOD(set_normal);
	DECLARE_METHOD(set_texCoord);

	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(coord_changed);
	DECLARE_CALLBACK(normal_changed);
	DECLARE_CALLBACK(texCoord_changed);

};

#define ANMITS_INDEXDIRTY				ANMCG_NDIRTYBITS
#define ANMITS_NDIRTYBITS				(ANMITS_INDEXDIRTY + 1)

typedef enum eAnmITSDirtyFlags {
eAnmITSIndexDirty			= ANMDIRTYBIT(ANMITS_INDEXDIRTY),
} eAnmITSDirtyFlags;

#endif // _anmindexedtriangleset_h
