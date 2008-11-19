/********************************************************************************
 * Flux
 *
 * File: anmpolygon.cpp
 * Description: Polygon object
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
#include "anmpolygon.h"

#define BEZSMOOTHFACTOR 7

void CAnmPolygon::AddBezier(CAnmOutline bez)
{
	std::vector<Point3> bezpoints = bez.GetPoints();
	int nPoints = bezpoints.size();

	Point3 p[3];
	
	assert(nPoints > 0);

	CAnmOutline out;

	p[0] = bezpoints[0];
	int i = 1;
	while (i < nPoints)
	{
		p[1] = bezpoints[i++];
		if (i == nPoints - 1)
		{
			p[2] = bezpoints[i++];
		}
		else
		{
			p[2] = Point3( (p[1] + bezpoints[i]) / 2);
		}

		for (int j = 0; j < BEZSMOOTHFACTOR; j++) {
			float t = (float) (j + 1) / BEZSMOOTHFACTOR;
			float x = (1 - t) * (1 - t) * p[0].x + 2 * t * (1 - t) * p[1].x + t * t * p[2].x;
			float y = (1 - t) * (1 - t) * p[0].y + 2 * t * (1 - t) * p[1].y + t * t * p[2].y;
			out.AddPoint(Point3(x, y, 0.0f));
		}

#if 0
		out.AddPoint(p[0]);
		out.AddPoint(p[1]);
		out.AddPoint(p[2]);
#endif

		p[0] = p[2];
	}

	AddOutline(out);
}
