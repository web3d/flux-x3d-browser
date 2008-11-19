/********************************************************************************
 * Flux
 *
 * File: anmquaternion.h
 * Description: Quaternion math support
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

#ifndef _anmquaternion_h
#define _anmquaternion_h

#include "anmnodedefs.h"

#define ANMQUATERNION_EPSILON 0.000000001

class CAnmQuaternion
{
public :

	double x;
	double y;
	double z;
	double w;


	// Constructor/destructor
	CAnmQuaternion();
	CAnmQuaternion(Rotation r);
	CAnmQuaternion(matrix4 mat);

	// Accessors
	void SetX(double nx) { x = nx; }
	void SetY(double ny) { y = ny; }
	void SetZ(double nz) { z = nz; }
	void SetW(double nw) { w = nw; }

	double X() { return x; }
	double Y() { return y; }
	double Z() { return z; }
	double W() { return w; }

	// Converters
	Rotation AsRotation();

	// Interpolation helpers
	static void LinearInterp(Float ratio, CAnmQuaternion *q1,
		CAnmQuaternion *q2, CAnmQuaternion *pResult);

	static void SphericalLinearInterp(Float ratio, CAnmQuaternion *q1,
		CAnmQuaternion *q2, CAnmQuaternion *pResult);

};

inline CAnmQuaternion operator* (CAnmQuaternion &q1, CAnmQuaternion &q2)
{
	if (fabs(q1.x) < ANMQUATERNION_EPSILON && fabs(q1.y) < ANMQUATERNION_EPSILON && fabs(q1.z) < ANMQUATERNION_EPSILON)
		return q2;
	if (fabs(q2.x) < ANMQUATERNION_EPSILON && fabs(q2.y) < ANMQUATERNION_EPSILON && fabs(q2.z) < ANMQUATERNION_EPSILON)
		return q1;

	CAnmQuaternion q3;

	// gnarly; however I don't think there's a better way
    q3.x = q1.w * q2.x +
	   q2.w * q1.x +
	   q1.y * q2.z - q1.z * q2.y;

    q3.y = q1.w * q2.y +
	   q2.w * q1.y +
	   q1.z * q2.x - q1.x * q2.z;

    q3.z = q1.w * q2.z +
	   q2.w * q1.z +
	   q1.x * q2.y - q1.y * q2.x;

    q3.w = q1.w * q2.w - (q1.x * q2.x +
		q1.y * q2.y + q1.z * q2.z);

	return q3;
}    

#endif // _anmquaternion_h
