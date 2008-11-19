/********************************************************************************
 * Flux
 *
 * File: anmcoordinate.h
 * Description: Coordinate node
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

#ifndef _anmcoordinate_h
#define _anmcoordinate_h

#include "anmnode.h"

class CAnmCoordinate : public CAnmNode
{
protected :

	Point3Array					*m_point;


public:

	// constructor/destructor
	CAnmCoordinate();
	virtual ~CAnmCoordinate();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual void FlagUpdate();

	// Accessors
	void SetPoint(Point3Array *pPoint);
	Point3Array	*GetPoint() { return m_point; }
	void GetPoint(Point3Array **pVal)
	{ 
		if (pVal)
			*pVal = m_point;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmCoordinate);

	DECLARE_CALLBACK(point_changed);

	DECLARE_VALUE(point);

	DECLARE_METHOD(set_point);

};

DECLARE_GETIMPLEMENTATION(CAnmCoordinate);

#endif _anmcoordinate_h
