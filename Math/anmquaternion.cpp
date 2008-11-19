/********************************************************************************
 * Flux
 *
 * File: anmquaternion.cpp
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

#include "stdafx.h"
#include "anmquaternion.h"

CAnmQuaternion::CAnmQuaternion()
: x(0.f), y(0.f), z(0.f), w(1.f)
{
}

CAnmQuaternion::CAnmQuaternion(Rotation r)
{
	if (r.vec.Mag() < ANMQUATERNION_EPSILON)
	{
		// check for boundary case
		x = y = z = 0.0;
		w = 1.0;
	}
	else
	{
		double sine;
		r.vec.Normalize();

		// quaternion: w = cos(t/2), x = s * sin(t/2) etc.
		sine = sin ( r.theta * 0.5 );
		x = r.vec.x * sine;
		y = r.vec.y * sine;
		z = r.vec.z * sine;
		w = cos ( r.theta * 0.5 );
	}
}

#if 0
CAnmQuaternion::CAnmQuaternion(matrix4 mat)
{
	// From Wertz pg. 415
	/*
	w = Math.sqrt(1.0 + m1.m00 + m1.m11 + m1.m22) / 2.0;
	double w4 = (4.0 * w);
	x = (m1.m12 - m1.m21) / w4 ;
	y = (m1.m20 - m1.m02) / w4 ;
	z = (m1.m01 - m1.m10) / w4 ;
	*/

	w = sqrt(1.0 + mat._11 + mat._22 + mat._33) / 2.0;
	double w4 = (4.0 * w);
	x = (mat._23 - mat._32) / w4 ;
	y = (mat._31 - mat._13) / w4 ;
	z = (mat._12 - mat._21) / w4 ;

}

#elif 0

CAnmQuaternion::CAnmQuaternion(matrix4 mat)
{
  float  tr, s, q[4];
  int    i, j, k;


  int nxt[3] = {1, 2, 0};


  tr = mat.m[0][0] + mat.m[1][1] + mat.m[2][2];


  // check the diagonal
  if (tr > 0.0) {
    s = sqrt (tr + 1.0);
    w = s / 2.0;
    s = 0.5 / s;
    x = (mat.m[1][2] - mat.m[2][1]) * s;
    y = (mat.m[2][0] - mat.m[0][2]) * s;
    z = (mat.m[0][1] - mat.m[1][0]) * s;
} else {		
	 // diagonal is negative
    	  i = 0;
          if (mat.m[1][1] > mat.m[0][0]) i = 1;
	     if (mat.m[2][2] > mat.m[i][i]) i = 2;
            j = nxt[i];
            k = nxt[j];


            s = sqrt ((mat.m[i][i] - (mat.m[j][j] + mat.m[k][k])) + 1.0);
      
	     q[i] = s * 0.5;
            
            if (s != 0.0) s = 0.5 / s;


	    q[3] = (mat.m[j][k] - mat.m[k][j]) * s;
            q[j] = (mat.m[i][j] + mat.m[j][i]) * s;
            q[k] = (mat.m[i][k] + mat.m[k][i]) * s;


	  x = q[0];
	  y = q[1];
	  z = q[2];
	  w = q[3];
  }
}

#else

////////////////////////////////////////////////////////////////////////////////
// CAnmQuaternion::CAnmQuaternion
//
// This code was adapted from code from the robotics lab at Carnegie-Mellon
////////////////////////////////////////////////////////////////////////////////

CAnmQuaternion::CAnmQuaternion(matrix4 mat)
{
	double s, a, b, c, max, q[4];
	int i, index;
	
	// Compute squared quaternion values from the matrix
	a = (1.0 - mat.m[0][0]) * 0.25;
	b = (1.0 - mat.m[1][1]) * 0.25;
	c = (1.0 - mat.m[2][2]) * 0.25;
	q[0] = b + c - a;
	q[1] = c + a - b;
	q[2] = a + b - c;
	q[3] = (1.0 + mat.m[0][0] + mat.m[1][1] + mat.m[2][2]) * 0.25;

	// Find overall scale
	for (i = 0, s = 0.0; i < 4; i++) {
		if (q[i] < 0.0)	// Oops: squares should all b non-negative
			q[i] = 0.0;
		s += q[i];
	}
	
	// Take square roots, and find maximum component
	s = 1.0 / s;
	for (i = 0, max = 0.0, index = 0; i < 4; i++) {
		q[i] = sqrt(s * q[i]);
		if (q[i] > max) {
			max = q[i];
			index = i;
		}
	}
	
	// Pivot around the largent component, and determine the signs
	switch (index) {
		case 0:		// X is largest
			if ((mat.m[1][0] + mat.m[0][1]) < 0.0) q[1] = -q[1];
			if ((mat.m[2][0] + mat.m[0][2]) < 0.0) q[2] = -q[2];
			if ((mat.m[1][2] - mat.m[2][1]) < 0.0) q[3] = -q[3];
			break;
		case 1:		// Y is largest
			if ((mat.m[1][0] + mat.m[0][1]) < 0.0) q[0] = -q[0];
			if ((mat.m[1][2] + mat.m[2][1]) < 0.0) q[2] = -q[2];
			if ((mat.m[2][0] - mat.m[0][2]) < 0.0) q[3] = -q[3];
			break;
		case 2:		// Z is largest
			if ((mat.m[2][0] + mat.m[0][2]) < 0.0) q[0] = -q[0];
			if ((mat.m[1][2] + mat.m[2][1]) < 0.0) q[1] = -q[1];
			if ((mat.m[0][1] - mat.m[1][0]) < 0.0) q[3] = -q[3];
			break;
		case 3:		// W is largest
			if ((mat.m[1][2] - mat.m[2][1]) < 0.0) q[0] = -q[0];
			if ((mat.m[2][0] - mat.m[0][2]) < 0.0) q[1] = -q[1];
			if ((mat.m[0][1] - mat.m[1][0]) < 0.0) q[2] = -q[2];
			break;
	}

	  x = q[0];
	  y = q[1];
	  z = q[2];
	  w = q[3];
}

#endif

Rotation CAnmQuaternion::AsRotation()
{
	Rotation r;

	r.vec.Assign(x, y, z);
	if (r.vec == point3::Zero)
	{
		r.vec.Assign(0, 1, 0);
		r.theta = 0.f;
	}
	else
	{
		r.vec.Normalize();
		r.theta = acos(w) * 2.f;
	}

	return r;
}

void CAnmQuaternion::LinearInterp(Float ratio, CAnmQuaternion *q1,
								  CAnmQuaternion *q2,
								  CAnmQuaternion *pResult)
{

	assert(q1);
	assert(q2);
	assert(pResult);

	pResult->x = q1->x + (q2->x - q1->x) * ratio;
	pResult->y = q1->y + (q2->y - q1->y) * ratio;
	pResult->z = q1->z + (q2->z - q1->z) * ratio;
	pResult->w = q1->w + (q2->w - q1->w) * ratio;
}


void CAnmQuaternion::SphericalLinearInterp(Float ratio, CAnmQuaternion *q1,
								CAnmQuaternion *q2,
								  CAnmQuaternion *pResult)
{
	double  omega, cosomega, sinomega;
	double scaleQ1, scaleQ2, scale2Q2;
	double newX, newY, newZ, newW;

	cosomega = q1->X() * q2->X() + 
	    q1->Y() * q2->Y() + 
	    q1->Z() * q2->Z() +
	    q1->W() * q2->W();

	if(cosomega < 0)
	{
		cosomega = -cosomega;
		scale2Q2 = -1.f;
	}
	else
		scale2Q2 = 1.f;

	if ((1.0 - cosomega) > ANMQUATERNION_EPSILON)
    {
	   omega = acos(cosomega);
	   sinomega = sin (omega);
	   scaleQ1 = sin((1.0 - ratio) * omega) / sinomega;
	   scaleQ2  = sin(ratio * omega) / sinomega;
	}
    else
    {
        scaleQ1 = 1.0 - ratio;
        scaleQ2 = ratio;
    }

	scaleQ2 *= scale2Q2;

	newX = scaleQ1 * q1->X() + scaleQ2 * q2->X();
	newY = scaleQ1 * q1->Y() + scaleQ2 * q2->Y();
	newZ = scaleQ1 * q1->Z() + scaleQ2 * q2->Z();
	newW = scaleQ1 * q1->W() + scaleQ2 * q2->W();

	pResult->SetX(newX);
	pResult->SetY(newY);
	pResult->SetZ(newZ);
	pResult->SetW(newW);

}

