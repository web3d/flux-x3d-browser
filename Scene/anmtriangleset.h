/********************************************************************************
 * Flux
 *
 * File: anmtriangleset.h
 * Description: TriangleSet node
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

#ifndef _anmtriangleset_h
#define _anmtriangleset_h

#include "anmcomposedgeometry.h"

class CAnmTriangleSet  : public CAnmComposedGeometry
{
protected :

	virtual void CreateMesh();

public:

	CAnmTriangleSet();
	~CAnmTriangleSet();

	// CAnmComposedGeometry overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	// New methods

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmTriangleSet);

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

#endif // _anmtriangleset_h
