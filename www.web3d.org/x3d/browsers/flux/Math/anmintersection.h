/********************************************************************************
 * Flux
 *
 * File: anmintersection.h
 * Description: Intersection support
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

#ifndef _anmintersection_h
#define _anmintersection_h

#include "anmprimitive.h"		// N.B.: for tAnmTriangle; perhaps a reorg is in order

#define INTRSCT_RIGHT	0
#define INTRSCT_LEFT	1
#define INTRSCT_MIDDLE	2

class CAnmIntersection {

public :

	static BOOL RayIntersectBox(Point3 from, Point3 to, Point3 bboxCenter, Point3 bboxSize,
		Point3 *hitPoint);
	static BOOL RayIntersectTriangle(Point3 from, Point3 to, tAnmTriangle triangle, Point3 *hitPoint);
	static BOOL PointInTriangle(Point3 p, tAnmTriangle triangle);
	static BOOL SphereIntersectSphere(const Point3 &center0, float radius0, const Point3 &center1, float radius1);
	static BOOL SphereIntersectBox(const Point3 &sphCenter, float sphRadius, const Point3 &bboxCenter, const Point3 &bboxSize);
	static BOOL SphereIntersectTriangle(const Point3 &sphCenter, float sphRadius, const tAnmTriangle &triangle);
	static BOOL CapsuleIntersectTriangle(
		const Point3 &capsulePt0, const Point3 &capsulePt1, const Point3& capsuleRadius,
		const tAnmTriangle &triangle, float *t
	);
	static BOOL CAnmIntersection::CapsuleIntersectBox(
		const Point3 &capsulePt0, const Point3 &capsulePt1, const Point3 &capsuleRadius,
		const Point3 &minBox, const Point3 &maxBox
	);
	static BOOL CAnmIntersection::CapsuleIntersectBox(
		const Point3 &capsulePt0, const Point3 &capsulePt1, float capsuleRadius,
		const Point3 &minBox, const Point3 &maxBox
	);
};

#define ASPOINT(_f_) (*(Point3 *) _f_)


////////////////////////////////////////////////////////////////////////////////
// CAnmIntersection::RayIntersectBox
////////////////////////////////////////////////////////////////////////////////

inline BOOL CAnmIntersection::RayIntersectBox(Point3 from, Point3 to, Point3 bboxCenter, Point3 bboxSize,
		Point3 *hitPoint)
{
	float minB[3], maxB[3];
	float  origin[3], dir[3];
	float  coord[3];

	ASPOINT(minB) = bboxCenter - (bboxSize * .5f);
	ASPOINT(maxB) = bboxCenter + (bboxSize * .5f);
	ASPOINT(origin) = from;
	ASPOINT(dir) = to - from;

	char inside = TRUE;
	char quadrant[3];
	register int i;
	int whichPlane;
	double maxT[3];
	double candidatePlane[3];

	/* Find candidate planes; this loop can be avoided if
   	rays cast all from the eye(assume perpsective view) */
	for (i=0; i<3; i++)
		if(origin[i] < minB[i]) {
			quadrant[i] = INTRSCT_LEFT;
			candidatePlane[i] = minB[i];
			inside = FALSE;
		}else if (origin[i] > maxB[i]) {
			quadrant[i] = INTRSCT_RIGHT;
			candidatePlane[i] = maxB[i];
			inside = FALSE;
		}else	{
			quadrant[i] = INTRSCT_MIDDLE;
		}

	/* Ray origin inside bounding box */
	if(inside)	{
		*hitPoint = from;
		return (TRUE);
	}


	/* Calculate T distances to candidate planes */
	for (i = 0; i < 3; i++)
		if (quadrant[i] != INTRSCT_MIDDLE && dir[i] !=0.)
			maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
		else
			maxT[i] = -1.;

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i < 3; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.) return (FALSE);
	for (i = 0; i < 3; i++)
		if (whichPlane != i) {
			coord[i] = origin[i] + maxT[whichPlane] *dir[i];
			if (coord[i] < minB[i] || coord[i] > maxB[i])
				return (FALSE);
		} else {
			coord[i] = candidatePlane[i];
		}

	*hitPoint = ASPOINT(coord);

	return (TRUE);				/* ray hits box */
}	


////////////////////////////////////////////////////////////////////////////////
// CAnmIntersection::PointInTriangle
//
// Solve for [ S T ] in
//
// [ S T ] | Ux Uy Uz | = [ Wx Wy Wz ]
//         | Vx Vy Vz |
//
// using the pseudo-inverse thusly:
//
// S    U    = W
//  1x2  2x3    1x3
//
//            t           t
// S    U    U    = W    U
//  1x2  2x3  3x2    1x3  3x2
//
//              [  t           t    -1  ]         +
// S     = W    [ U    ( U    U    )    ] = W    U
//  1x2     1x3 [  3x2    2x3  3x2  2x2 ]    1x3  3x2
//
// The bracketed term (U+) above is the pseudo-inverse.
////////////////////////////////////////////////////////////////////////////////

inline BOOL CAnmIntersection::PointInTriangle(Point3 p, tAnmTriangle triangle)
{
    float   uu, uv, vv, wu, wv, D;
    Point3 u = triangle.v[1] - triangle.v[0];	// #1 new basis vector
    Point3 v = triangle.v[2] - triangle.v[0];	// #2 new basis vector
	Point3 w = p             - triangle.v[0];	// vector to point in question

	// Compute the 2x2 matrix A = U Ut = | u*u u*v |
	//                                   | u*v v*v |
    uu = u * u;				// Squared magnitude of u = A00
    uv = u * v;				// Dot product of u and v = A01 = A10
    vv = v * v;				// Squared magnitude of v = A11
    D = uu * vv - uv * uv;	// Positive determinant of A, needed for inverse

	// Compute the 1x3 matrix B = W Ut = [ w*u w*v ]
	wu = w * u;				// Dot product of w and u
    wv = w * v;				// Dot product of w and v
 
    // Convert to coordinates in the new basis    -1
	//                                     S = B A
    float s, t;
    s = (wu * vv - wv * uv) / D;		// s coordinate w.r.t. the u basis vector
    if ((s < 0.0f) || (s > 1.0f))		// p is outside triangle
        return FALSE;
    t = (wv * uu - wu * uv) / D;		// t coordinate w.r.t. the v basis vector
    if ((t < 0.0f) || ((s + t) > 1.0f))	// p is outside triangle
        return FALSE;

    return TRUE;						// p is inside triangle: s in [0,1], t in [0,1], s+t < 1
}


////////////////////////////////////////////////////////////////////////////////
// CAnmIntersection::RayIntersectTriangle
////////////////////////////////////////////////////////////////////////////////

inline BOOL CAnmIntersection::RayIntersectTriangle(Point3 from, Point3 to,
												   tAnmTriangle triangle, Point3 *hitPoint)
{
	plane3 plane(triangle.v[0], triangle.v[1], triangle.v[2]);

	ePointLoc startRes = plane.TestPoint(from);
	ePointLoc endRes = plane.TestPoint(to);
	
	bool frontback = false;
	if (startRes == ptFront && (endRes == ptBack || endRes == ptCoplanar))
		frontback = true;
	else if (endRes == ptBack && (startRes == ptFront || startRes == ptCoplanar))
		frontback = true;

	if (frontback)
	{
		Point3 splitPoint = plane.Split(from, to);
		if (PointInTriangle(splitPoint, triangle))
		{
			*hitPoint = splitPoint;
			return TRUE;
		}
	}
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////
// CAnmIntersection::SphereIntersectSphere
////////////////////////////////////////////////////////////////////////////////

inline BOOL
CAnmIntersection::SphereIntersectSphere(
	const Point3 &center0, float radius0,	// first sphere
	const Point3 &center1, float radius1	// second sphere
)
{
	return(point3::Dist(center0, center1) <= (radius0 + radius1));
}


////////////////////////////////////////////////////////////////////////////////
// CAnmIntersection::SphereIntersectBox
////////////////////////////////////////////////////////////////////////////////

inline BOOL
CAnmIntersection::SphereIntersectBox(
	const Point3 &sphCenter,  float sphRadius,			// sphere
	const Point3 &bboxCenter, const Point3 &bboxSize	// bounding box
)
{
	return(	(fabs(sphCenter.x - bboxCenter.x) <= (bboxSize.x * 0.5f))
		&&	(fabs(sphCenter.y - bboxCenter.y) <= (bboxSize.y * 0.5f))
		&&	(fabs(sphCenter.z - bboxCenter.z) <= (bboxSize.z * 0.5f))
	);
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//		Utility Functions -- these might be better placed elsewhere.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// CircumscribingRadiusOfBoundingBoxSize
//	Handy utility to convert from axis-aligned bounding boxes to bounding spheres.
//	Since the centers are the same, we just need to convert the size to a radius.
////////////////////////////////////////////////////////////////////////

inline float
CircumscribingRadiusOfBoundingBoxSize(const Point3 &boxSize)
{
	return(boxSize.Mag() * 0.5f);
}


////////////////////////////////////////////////////////////////////////////////
// CircumscribingBoxSizeOfBoundingSphereRadius
//	Handy utility to convert from bounding spheres to axis-aligned bounding boxes.
//	Since the centers are the same, we just need to convert the radius to a size.
////////////////////////////////////////////////////////////////////////

inline const Point3
CircumscribingBoxSizeOfBoundingSphereRadius(float r)
{
	r *= 2.0f;
	return(Point3(r, r, r));
}


#endif // _anmintersection_h
