/********************************************************************************
 * Flux
 *
 * File: anmtexturecoordinate.h
 * Description: TextureCoordinate node
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

#ifndef _anmtexturecoordinate_h
#define _anmtexturecoordinate_h

#include "anmnode.h"

class CAnmTextureCoordinate : public CAnmNode
{
protected :

	Point2Array					*m_point;

public:

	// constructor/destructor
	CAnmTextureCoordinate();
	virtual ~CAnmTextureCoordinate();

	// CAnmNode overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();								// re-render/reset rendering structures

	void SetPoint(Point2Array *pPoint);
	Point2Array	*GetPoint() { return m_point; }
	void GetPoint(Point2Array **pVal)
	{ 
		if (pVal)
			*pVal = m_point;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmTextureCoordinate);

	DECLARE_CALLBACK(point_changed);

	DECLARE_VALUE(point);

	DECLARE_METHOD(set_point);

};

DECLARE_GETIMPLEMENTATION(CAnmTextureCoordinate);

#endif _anmtexturecoordinate_h
