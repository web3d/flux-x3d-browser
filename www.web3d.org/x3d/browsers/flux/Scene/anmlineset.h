/********************************************************************************
 * Flux
 *
 * File: anmlineset.h
 * Description: LineSet node
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

#ifndef _anmlineset_h
#define _anmlineset_h

#include "anmgeometry.h"

class CAnmLineSet  : public CAnmGeometry
{

	IntegerArray			*m_lineCount;			// # of coords per line

	CAnmNode				*m_color;
	CAnmNode				*m_coord;

	virtual void CreateMesh();

public:

	CAnmLineSet();
	~CAnmLineSet();

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

	void SetCoord(CAnmNode *pCoordinate);
	class CAnmNode *GetCoord() { return m_coord; }
	void GetCoord(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_coord;
	}
	
	void SetLineCount(IntegerArray *pLineCount);
	IntegerArray *GetLineCount() { return m_lineCount; }
	void GetLineCount(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_lineCount;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmLineSet);

	DECLARE_VALUE(color);
	DECLARE_VALUE(coord);
	DECLARE_VALUE(lineCount);

	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_coord);
	DECLARE_METHOD(set_lineCount);

	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(coord_changed);
	DECLARE_CALLBACK(lineCount_changed);
};

#define ANMLS_COLORDIRTY				ANMGEOMETRY_NDIRTYBITS
#define ANMLS_COORDDIRTY				(ANMLS_COLORDIRTY + 1)
#define ANMLS_LINECOUNTDIRTY			(ANMLS_COLORDIRTY + 2)

typedef enum eAnmLSDirtyFlags {
eAnmLSColorDirty				= ANMDIRTYBIT(ANMLS_COLORDIRTY),
eAnmLSCoordDirty				= ANMDIRTYBIT(ANMLS_COORDDIRTY),
eAnmLSLineCountDirty			= ANMDIRTYBIT(ANMLS_LINECOUNTDIRTY),
} eAnmLSDirtyFlags;

#endif // _anmlineset_h
