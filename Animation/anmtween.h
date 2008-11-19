/********************************************************************************
 * Flux
 *
 * File: anmtween.h
 * Description: Animation Tweening
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


#ifndef _anmtween_h
#define _anmtween_h

#include "anmnodedefs.h"
#include "anmhsvcolor.h"
#include "anmquaternion.h"

class CAnmTween {

public :
	static void Tween(Float ratio, Point3 *pp1, Point3 *pp2, Point3 *pResult);
	static void Tween(Float ratio, Color *pc1, Color *pc2, Color *pResult);
	static void RGBTween(Float ratio, Color *pc1, Color *pc2, Color *pResult);
	static void Tween(Float ratio, Rotation *pr1, Rotation *pr2, Rotation *pResult);
	static void Tween(Float ratio, Float *pf1, Float *pf2, Float *pResult);
	static void NormalTween(Float ratio, Point3 *pp1, Point3 *pp2, Point3 *pResult);
	static void Tween(Float ratio, Point2 *pp1, Point2 *pp2, Point2 *pResult);
};

inline void CAnmTween::Tween(Float ratio, Point3 *pp1, Point3 *pp2, Point3 *pResult)
{

	assert(pp1);
	assert(pp2);
	assert(pResult);

	pResult->x = pp1->x + (pp2->x - pp1->x) * ratio;
	pResult->y = pp1->y + (pp2->y - pp1->y) * ratio;
	pResult->z = pp1->z + (pp2->z - pp1->z) * ratio;
}

inline void CAnmTween::Tween(Float ratio, Rotation *pr1, Rotation *pr2, Rotation *pResult)
{

	assert(pr1);
	assert(pr2);
	assert(pResult);

	if (pr1->vec == pr2->vec && pr1->theta == pr2->theta)
	{
		*pResult = *pr1;
		return;
	}

	CAnmQuaternion q1(*pr1);
	CAnmQuaternion q2(*pr2);
	CAnmQuaternion qResult;

	CAnmQuaternion::SphericalLinearInterp(ratio, &q1, &q2, &qResult);

	*pResult = qResult.AsRotation();

}

inline void CAnmTween::Tween(Float ratio, Color *pc1, Color *pc2, Color *pResult)
{

	assert(pc1);
	assert(pc2);
	assert(pResult);


	CAnmHSVColor c1(*pc1);
	CAnmHSVColor c2(*pc2);
	CAnmHSVColor c3;
	float dh;

	/* Saturation and value are interpolated in R2 */
	c3.s = c1.s + (c2.s - c1.s) * ratio;
	c3.v = c1.v + (c2.v - c1.v) * ratio;

	/* Hue is interpolated in S1 */
	dh = c2.h - c1.h;						/* [0, +360) - [0, +360) = (-360, +360) */
	if ((dh <= -180.0f) || (dh > 180.0f)) {	/* (-360, -180]  ||  (180, 360) */
		if (dh < 0)	dh += 360.0f;			/* (-360, -180] ==> (   0, 180] */
		else		dh -= 360.0f;			/* (+180, +360) ==> (-180,   0) */
	}										/* dh now in (-180, +180] */
	c3.h = c1.h + dh * ratio;				/* h + dh = [0, +360) + (-180, +180] = (-180, +540) */
	
	/* Make sure the result is in [0, 360) */
	c3.h = fmod(c3.h, 360.0f);				/* (-360, +360) */
	if (c3.h < 0)
		c3.h += 360.0f;						/* [0, +360] */
	
	Color c = c3.AsColor();
	*pResult = c;
}

inline void CAnmTween::RGBTween(Float ratio, Color *pc1, Color *pc2, Color *pResult)
{
	assert(pc1);
	assert(pc2);
	assert(pResult);

	pResult->r = pc1->r + (pc2->r - pc1->r) * ratio;
	pResult->g = pc1->g + (pc2->g - pc1->g) * ratio;
	pResult->b = pc1->b + (pc2->b - pc1->b) * ratio;
}

inline void CAnmTween::Tween(Float ratio, Float *pf1, Float *pf2, Float *pResult)
{
	*pResult = *pf1 + (*pf2 - *pf1) * ratio;
}

inline void CAnmTween::NormalTween(Float ratio, Point3 *pp1, Point3 *pp2, Point3 *pResult)
{

	assert(pp1);
	assert(pp2);
	assert(pResult);


	Point3 p1 = *pp1;
	Point3 p2 = *pp2;
	p1.Normalize();
	p2.Normalize();

	// get the angle between the two vectors
	float dot = p1 * p2;
	dot = ClampValue(-1.f, dot, 1.f);
	float theta = acos(dot);

	// resulting vector
	float r1, r2;

	if (theta < FLT_EPSILON) {
		r1 = 1.0f;
		r2 = 0.0f;
	} else {
		r1 = (float) (sin((1.f - ratio) * theta) / sin(theta));
		r2 = (float) (sin(ratio * theta) / sin(theta));
	}

	Point3 p3(r1 * p1.x + r2 * p2.x, r1 * p1.y + r2 * p2.y, r1 * p1.z + r2 * p2.z);

	*pResult = p3;
}

inline void CAnmTween::Tween(Float ratio, Point2 *pp1, Point2 *pp2, Point2 *pResult)
{

	assert(pp1);
	assert(pp2);
	assert(pResult);

	pResult->x = pp1->x + (pp2->x - pp1->x) * ratio;
	pResult->y = pp1->y + (pp2->y - pp1->y) * ratio;
}









#endif // _anmtween_h
