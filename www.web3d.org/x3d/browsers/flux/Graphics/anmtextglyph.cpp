/********************************************************************************
 * Flux
 *
 * File: anmtextglyph.cpp
 * Description: Text Glyph class - single char in
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

#include "stdafx.h"
#include "anmtextglyph.h"
#include "anmtesselator.h"

CAnmTextGlyph::CAnmTextGlyph()
{
}

void CAnmTextGlyph::AddPolygon(CAnmPolygon poly)
{
	m_polygons.push_back(poly);
}

std::vector<Point3> CAnmTextGlyph::GetPoints()
{
	if (m_points.size() == 0)
		GeneratePolygons();

	return m_points;
}

std::vector<WORD> CAnmTextGlyph::GetIndices()
{
	if (m_indices.size() == 0)
		GeneratePolygons();

	return m_indices;
}

void CAnmTextGlyph::GeneratePolygons()
{
	// for each poly, go through list of curves and add the points, then tesselate
	int i, j, k;
	std::vector<int> countours;
	int npolys = m_polygons.size();
	for (i = 0; i < npolys; i++)
	{
		CAnmPolygon poly = m_polygons[i];

		// add all the points from each curve
		std::vector<CAnmOutline> outlines = poly.GetOutlines();

		int countoursize = 0;
		int noutlines = outlines.size();
		for (j = 0; j < noutlines; j++)
		{
			CAnmOutline outline = outlines[j];

			std::vector<Point3> outlinepoints = outline.GetPoints();

			int npoints = outlinepoints.size();
			for (k = 0; k < npoints; k++)
			{
				m_points.push_back(outlinepoints[k]);
				countoursize++;
			}
		}

		countours.push_back(countoursize);			
	}

	int nverts = m_points.size();

	// tesselate the poly
	int nindices = nverts;
	if (nindices >= 3)
	{
		int *indices = Tesselate(&m_points[0], nindices, &countours[0], countours.size());
		if (indices)
		{
			// try CCW winding instead TP 5/05/03 -- for (k = 0; k < nindices; k++)
			for (k = nindices - 1; k >= 0; --k)
			{
				WORD index = indices[k];
				m_indices.push_back(index);
			}
			delete [] indices;
		}
	}

}


int *CAnmTextGlyph::Tesselate(Point3 *verts, int &numIndices, int *countours, int countourcount)
{
	// copy vertices 1 for 1 into new array
	std::vector<double> coords;
	for (int i = 0; i < numIndices; i++)
	{
		coords.push_back((double) verts[i].x);
		coords.push_back((double) verts[i].y);
		coords.push_back((double) verts[i].z);
	}

	int nindices = numIndices;

	// tesselate; this will create a new index array and update the count
	CAnmTesselator t;
	int *newindices = t.Tesselate(&coords[0], nindices, countours, countourcount);

	numIndices = nindices;
	return nindices ? newindices : NULL;
}
