/********************************************************************************
 * Flux
 *
 * File: anmhsvcolor.cpp
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

#include "stdafx.h"
#include "anmhsvcolor.h"

CAnmHSVColor::CAnmHSVColor()
: h(0.f), s(0.f), v(0.f)
{

}

CAnmHSVColor::CAnmHSVColor(Color c)
{

	// RGB to HSV. Straight out of Foley and Van Dam
	// Code can be found at http://www.psc.edu/~burkardt/src/colors/colors.f90
	Float rc, gc, bc, rgbmax, rgbmin, rgbdif;

	rgbmax = max ( max ( c.r, c.g ), c.b );
	rgbmin = min ( min ( c.r, c.g ), c.b );
	rgbdif = rgbmax - rgbmin;

	v = rgbmax;

	// Compute the saturation

	if (rgbmax)
		s = rgbdif / rgbmax;
	else
		s = 0.0;

	// Compute the hue
	if (s == 0.0)
		h = 0.0;
	else
	{
		rc = ( rgbmax - c.r ) / rgbdif;
		gc = ( rgbmax - c.g ) / rgbdif;
		bc = ( rgbmax - c.b ) / rgbdif;
		if ( c.r == rgbmax )
		  h = bc - gc;
		else if ( c.g == rgbmax )
		  h = 2.0 + rc - bc;
		else
		  h = 4.0 + gc - rc;

		h *= 60.0;
		
		// make sure it's in between 0 and 360
//		h = fabs(fmod(h, 360.0 ));	// @@@ I don't think you want to reflect about the origin -- Turk @@@
		h = fmod(h, 360.0 );
		if (h < 0)
			h += 360.0;
	}
}

Color CAnmHSVColor::AsColor()
{
	Color c;

	Float f, hue, p, q, t;
	int i;

	if ( s == 0.0 )
	{
		c.r = c.g = c.b = v;
	}
	else
	{

		// Make sure HUE lies between 0 and 360.0

	    hue = h / 60.0;

		i = (int) hue;

		f = hue - (float) i;
		p = v * ( 1.0 - s );
		q = v * ( 1.0 - s * f );
		t = v * ( 1.0 - s + s * f );

		switch (i)
		{
			case 0 :
				{
				c.r = v;
				c.g = t;
				c.b = p;
				}
				break;

			case 1 :
				{
				  c.r = q;
				  c.g = v;
				  c.b = p;
				}
				break;

			case 2 :
				{
				  c.r = p;
				  c.g = v;
				  c.b = t;
				}
				break;

			case 3 :
				{
				  c.r = p;
				  c.g = q;
				  c.b = v;
				}
				break;

			case 4 :
				{
				  c.r = t;
				  c.g = p;
				  c.b = v;
				}
				break;

			case 5 :
				{
				  c.r = v;
				  c.g = p;
				  c.b = q;
				}
				break;

			default :
				assert(FALSE);
				break;
		}
	}


	return c;
}


