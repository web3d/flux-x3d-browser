/********************************************************************************
 * Flux
 *
 * File: anmpolygon.h
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

#ifndef _anmpolygon_h
#define _anmpolygon_h

class CAnmOutline {

protected :
	
	std::vector<Point3>			m_points;

public :

	void AddPoint(Point3 p)
	{
		m_points.push_back(p);
	}

	std::vector<Point3> GetPoints()
	{
		return m_points;
	}
};

class CAnmPolygon {

protected :


	std::vector<CAnmOutline>	m_outlines;

public :

	// Constructors
	CAnmPolygon()
	{
	}

	// Methods
	void AddOutline(CAnmOutline outline)
	{
		m_outlines.push_back(outline);
	}

	void AddBezier(CAnmOutline outline);

	std::vector<CAnmOutline> GetOutlines()
	{
		return m_outlines;
	}

};


#endif // _anmpolygon_h
