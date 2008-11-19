/********************************************************************************
 * Flux
 *
 * File: anmtextglyph.h
 * Description: Glyph class - single char in
 *							polygonal text meshes
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

#ifndef _anmtextglyph_h
#define _anmtextglyph_h

#include "anmpolygon.h"

class EXPORT CAnmTextGlyph : public CAnmRefObject
{
protected:
	
	std::vector<CAnmPolygon>		m_polygons;
	std::vector<Point3>				m_points;
	std::vector<WORD>				m_indices;

	void GeneratePolygons();
	int *Tesselate(Point3 *pVerts, int &numIndices, int *countours, int countourcount);

public:

	// constructor/destructor
	CAnmTextGlyph();

	// Methods
	void AddPolygon(CAnmPolygon poly);
	std::vector<CAnmPolygon> GetPolygons()
	{
		return m_polygons;
	}

	std::vector<Point3> GetPoints();
	std::vector<WORD> GetIndices();
};


#endif // _anmtextglyph_h
