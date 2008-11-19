/********************************************************************************
 * Flux
 *
 * File: anmmath.h
 * Description: Math definitions
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

#ifndef _anmmath_h
#define _anmmath_h

// math constants (actually from ECMAScript spec)
#define ANM_E		2.718281828459045
#define ANM_LN10	2.302585092994046
#define ANM_LN2		0.6931471805599453
#define ANM_PI		3.1415926535897932
#define ANM_SQRT1_2	0.7071067811865476
#define ANM_SQRT2	1.4142135623730951


// Matrix inversion methods
//
extern long MatrixInvert( matrix4& q, matrix4& a );
extern long MatrixInvert( matrix4D& q, matrix4D& a );


#endif // _anmmath_h
