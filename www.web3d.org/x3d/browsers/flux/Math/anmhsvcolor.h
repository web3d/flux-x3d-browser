/********************************************************************************
 * Flux
 *
 * File: anmhsvcolor.h
 * Description: HSV Color class; for color interpolation
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

#ifndef _anmhsvcolor_h
#define _anmhsvcolor_h

#include "anmnodedefs.h"

class CAnmHSVColor
{
public :

	float h, s, v;

	// Constructor/destructor
	CAnmHSVColor();
	CAnmHSVColor(Color r);

	// Accessors
	void SetH(double nh) { h = nh; }
	void SetS(double ns) { s = ns; }
	void SetV(double nv) { v = nv; }

	double H() { return h; }
	double S() { return s; }
	double V() { return v; }

	// Converters
	Color AsColor();

};

#endif // _anmhsvcolor_h
