/********************************************************************************
 * Flux
 *
 * File: anmtrianglestripset.h
 * Description: TriangleStripSet node
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

#ifndef _anmtrianglestripset_h
#define _anmtrianglestripset_h

#include "anmcomposedgeometry.h"

class CAnmTriangleStripSet  : public CAnmComposedGeometry
{
protected :

	IntegerArray			*m_stripCount;			// # of coords per strip

	virtual void CreateMesh();

public:

	CAnmTriangleStripSet();
	~CAnmTriangleStripSet();

	// CAnmComposedGeometry overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	// New methods

	// Accessors
	void SetStripCount(IntegerArray *pStripCount);
	IntegerArray *GetStripCount() { return m_stripCount; }
	void GetStripCount(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_stripCount;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmTriangleStripSet);

	DECLARE_VALUE(ccw);
	DECLARE_VALUE(color);
	DECLARE_VALUE(colorPerVertex);
	DECLARE_VALUE(coord);
	DECLARE_VALUE(stripCount);
	DECLARE_VALUE(normal);
	DECLARE_VALUE(normalPerVertex);
	DECLARE_VALUE(solid);
	DECLARE_VALUE(texCoord);

	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_coord);
	DECLARE_METHOD(set_stripCount);
	DECLARE_METHOD(set_normal);
	DECLARE_METHOD(set_texCoord);

	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(coord_changed);
	DECLARE_CALLBACK(stripCount_changed);
	DECLARE_CALLBACK(normal_changed);
	DECLARE_CALLBACK(texCoord_changed);

};

#endif // _anmtrianglestripset_h
