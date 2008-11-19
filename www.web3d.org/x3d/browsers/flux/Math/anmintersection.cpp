/********************************************************************************
 * Flux
 *
 * File: anmintersection.cpp
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

#include "stdafx.h"
#include "anmintersection.h"

#define ALLOW_MINOR_INITIAL_PENETRATION
#define INITIAL_PENETRATION_EPSILON			1.f // (1.0e-2f)


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////																		////
////			2 Dimensional Point and Vector classes						////
////																		////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#if 0
////////////////////////////////////////////////////////////////////////////////
// Point2
////////////////////////////////////////////////////////////////////////////////

struct Point2 {
	float x, y;

	Point2()													{											}
	Point2(float X, float Y)									{	x = X; y = Y;							}
	Point2(const Point3 &v)										{	return Point2(x, y); 					}

	inline void				Set(float X, float Y)				{	x = X; y = Y;							}
};
#endif


////////////////////////////////////////////////////////////////////////////////
// Vector3
////////////////////////////////////////////////////////////////////////////////

typedef Point3 Vector3;

inline const Vector3 operator-(const Vector3 &v)				{	return Vector3(-v.x, -v.y, -v.z);					}


////////////////////////////////////////////////////////////////////////////////
// Vector2
////////////////////////////////////////////////////////////////////////////////

struct Vector2 {
	float x, y;
	
	// Constructors
	Vector2()													{											}
	Vector2(float X, float Y)									{	x = X;		y = Y;						}
	Vector2(const Vector3 &v)									{	x = v.x;	y = v.y; 					}
	Vector2(const sAnmPoint2 &v)									{	x = v.x;	y = v.y; 					}
	
	// Accessors
	inline void				Set(float X, float Y)				{	x = X;		y = Y;						}
	inline void				Set(const Vector3 &v)				{	x = v.x;	y = v.y;					}
	
	// Operators
	inline const Vector2	operator-(void) const				{	return Vector2(-x, -y); 				}
	inline Vector2&			operator+=(Vector2 &v)				{	x += v.x;	y += v.y;	return(*this);	}
	inline Vector2&			operator-=(Vector2 &v)				{	x -= v.x;	y -= v.y;	return(*this);	}
	inline Vector2&			operator*=(float s)					{	x *= s;		y *= s;		return(*this);	}
	inline Vector2&			operator/=(float s)					{	return(*this *= 1.0f / s);				}

	// Functions
	inline float			MagnitudeSquared() const			{	return(x * x + y * y);					}
	inline float			Magnitude(void) const				{	return(sqrt(x * x + y * y));			}
	inline float			Norm(void) const					{	return(sqrt(x * x + y * y));			}
	inline void				Normalize(void)						{	*this /= Norm();						}
	inline const Vector2	Perp(void) const					{	return Vector2(-y, x);					}
	inline const Point2&	AsPoint(void) const					{	return(*((const Point2*)this));			}
	inline Point2&			AsPoint(void)						{	return(*((Point2*)this));				}
};

inline float CrossProduct(const Vector2 &a, const Vector2 &b)	{	return(a.x * b.y - a.y * b.x);			}
inline float DotProduct(  const Vector2 &a, const Vector2 &b)	{	return(a.x * b.x + a.y * b.y);			}
inline float   operator^(const Vector2 &a, const Vector2 &b)	{	return(CrossProduct(a, b));				}
inline float   operator*(const Vector2 &a, const Vector2 &b)	{	return(DotProduct(a, b));				}
inline Vector2 operator+(const Vector2 &a, const Vector2 &b)	{	return Vector2(a.x + b.x, a.y + b.y);	}
inline Vector2 operator-(const Vector2 &a, const Vector2 &b)	{	return Vector2(a.x - b.x, a.y - b.y);	}
// inline Vector2 operator-(const Point2  &a, const Point2  &b)	{	return Vector2(a.x - b.x, a.y - b.y);	}
inline Point2  operator+(const Point2  &a, const Vector2 &b)	{	return Point2 (a.x + b.x, a.y + b.y);	}
inline Point2  operator+(const Vector2 &a, const Point2  &b)	{	return Point2 (a.x + b.x, a.y + b.y);	}
inline Vector2 operator*(float s, const Vector2 &v)				{	return Vector2(s * v.x, s * v.y);		}
inline Vector2 operator*(const Vector2 &v, float s)				{	return Vector2(s * v.x, s * v.y);		}
inline Vector2 operator/(const Vector2 &v, float s)				{	return(v * (1.0f / s));					}
inline Vector2 Lerp(float t, const Vector2 &v0, const Vector2 &v1)	{	return Vector2(v0.x + t * (v1.x - v0.x), v0.y + t * (v1.y - v0.y));	}
inline float DistanceSquared(const Vector2 &a, const Vector2 &b){	return (a - b).MagnitudeSquared();		}
inline float Distance(const Vector2 &a, const Vector2 &b)		{	return (a - b).Magnitude();				}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////																		////
////	Reusable geometric support routines.								////
////																		////
////	These are generally useful, and should reside in a public library.	////
////																		////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// SquaredDistancePointSegment3D
////////////////////////////////////////////////////////////////////////////////

static float
SquaredDistancePointSegment3D(const Point3 &p, const Point3 &seg0, const Point3 &seg1)
{
	Vector3 segVec = seg1 - seg0;
	Vector3 ptVec  = p    - seg0;
	float t, segMag2;
	
	if ((t = segVec * ptVec) > 0) {
		if (t >= (segMag2 = segVec.MagSquared()))
			ptVec = p - seg1;
		else
			ptVec -= (t / segMag2) * segVec;
	}
	return(ptVec.MagSquared());
}


////////////////////////////////////////////////////////////////////////////////
// SquaredDistancePointSegment2D
////////////////////////////////////////////////////////////////////////////////

static float
SquaredDistancePointSegment2D(const Point2 &p, const Point2 &seg0, const Point2 &seg1)
{
	Vector2 segVec = seg1 - seg0;
	Vector2 ptVec  = p    - seg0;
	float t, segMag2;
	
	if ((t = segVec * ptVec) > 0) {
		if (t >= (segMag2 = segVec.MagnitudeSquared()))
			ptVec = p - seg1;
		else
			ptVec -= (t / segMag2) * segVec;
	}
	return(ptVec.MagnitudeSquared());
}


////////////////////////////////////////////////////////////////////////////////
// ClosestPointParametricallyToPointOnSegment2D
//		optionally returns the distance squared
////////////////////////////////////////////////////////////////////////////////

static float
ClosestPointParametricallyToPointOnSegment2D(const Point2 &p, const Point2 &seg0, const Point2 &seg1, float *r2)
{
	Vector2 segVec = seg1 - seg0;
	Vector2 ptVec  = p    - seg0;
	float t, segMag2;
	
	if ((t = segVec * ptVec) > 0) {
		if (t >= (segMag2 = segVec.MagnitudeSquared()))
			t = 1;
		else
			t /= segMag2;
	}
	else
		t = 0;
	if (r2) {
		ptVec -= t * segVec;
		*r2 = ptVec.MagnitudeSquared();
	}
	return(t);
}


////////////////////////////////////////////////////////////////////////////////
// PointInTriangle2D
////////////////////////////////////////////////////////////////////////////////

static BOOL
PointInTriangle2D(const Point2 &p, const Point2 triangle[3])
{
    float D;
    Vector2 tv[3], tp;
    
    tv[0] = p           - triangle[0];
    tv[1] = triangle[1] - triangle[0];
    tv[2] = triangle[2] - triangle[0];
    
    if ((D = tv[1].x * tv[2].y - tv[1].y * tv[2].x) == 0)
    	return (false);
    
    tp.x = (tv[0].x * tv[2].y - tv[0].y * tv[2].x) / D;
	if ((tp.x < 0.0f) || (tp.x > 1.0f))
		return(false);

    tp.y = (tv[0].y * tv[1].x - tv[0].x * tv[1].y) / D;
	if ((tp.y < 0.0f) || (tp.x + tp.y > 1.0f))
		return(false);
	
    return(true);
}


////////////////////////////////////////////////////////////////////////////////
// CircleIntersectsTriangle2D
//		This yields an exact result.
////////////////////////////////////////////////////////////////////////////////

static BOOL
CircleIntersectsTriangle2D(const Point2 &center, float radius, const Point2 triangle[3])
{
	const	Point2 *pt, *lastPt;
	Vector2	e[2], v, w;
	float	l, rn, r2, x;
	long	i;
	
	rn = -radius;
	r2 = radius * radius;
	
	for (i = 3, lastPt = &triangle[2], pt = &triangle[0]; i--; lastPt = pt++) {
		v    = center - *lastPt;
		e[0] = *pt    - *lastPt;
		if ((l = e[0].Magnitude()) != 0) {				/* Non-degenerate edge */
			e[0] /= l;									/* Unit vector alongthe edge */
			e[1] = e[0].Perp();							/* Unit vector perpendiclar to the edge */
			w.y = v * e[1];								/* Perpendicular distance to edge */
			if (w.y <= rn)								/* Too far away on the outside */
				return(false);							/* No way it can intersect */
			if (w.y < 0) {								/* On the outside, but close enought so thatthe circle might touch */
				w.x = v * e[0];							/* Parallel distance from lastPt */
				if (w.x < 0)							/* To the "left" of lastPt */
					return (w.MagnitudeSquared() < r2);	/* Will intersect only if it is close enough to lastPt */
				else if ((x = w.x - l) > 0)				/* To the "right" of pt */
					return ((x * x + w.y * w.y) < r2);	/* Will intersect only if it is close enough to pt */
			}
		}
		else {											/* Degenerate edge */
			if (v.MagnitudeSquared() < r2)				/* If close enough to the degenerate edge ... */
				return (true);							/* ... there is an intersection */
		}
	}
	return(true);
}



////////////////////////////////////////////////////////////////////////////////
// IntersectLinesParametrically2D
//	This is a bit sleazy - it should be computed in double precision.
////////////////////////////////////////////////////////////////////////////////

static BOOL
IntersectLinesParametrically2D(
	const Point2 &seg00, const Point2 &seg01,	/* First segment */
	const Point2 &seg10, const Point2 &seg11,	/* Second segment */
	float *t0,									/* Parametric value of intersection for segment 0 */
	float *t1									/* parametric value of intersection for segment 1 */
)
{
	float d;
	Vector2 v0, v1, v2;
	
	v0 = seg01 - seg00;
	v1 = seg11 - seg10;
	
	if ((d = v0.x * v1.y - v1.x * v0.y) == 0)
		return(false);
	
	v2 = seg10 - seg00;

	if (t0)	*t0 = (v1.y * v2.x - v1.x * v2.y) / d;
	if (t1)	*t1 = (v0.y * v2.x - v0.x * v2.y) / d;

	return(true);
}


////////////////////////////////////////////////////////////////////////////////
// IntersectSegmentsParametrically2D
////////////////////////////////////////////////////////////////////////////////

static BOOL
IntersectSegmentsParametrically2D(
	const Point2 &seg00, const Point2 &seg01,	/* First segment */
	const Point2 &seg10, const Point2 &seg11,	/* Second segment */
	float *t0,									/* Parametric value of intersection for segment 0 */
	float *t1									/* parametric value of intersection for segment 1 */
)
{
	float p0, p1;
	
	if (	!IntersectLinesParametrically2D(seg00, seg01, seg10, seg11, &p0, &p1)
		||	(p0 < 0.0f)
		||	(p0 > 1.0f)
		||	(p1 < 0.0f)
		||	(p1 > 1.0f)
	)
		return(false);

	if (t0)	*t0 = p0;
	if (t1)	*t1 = p1;
	return(true);
}




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////																		////
////	Geometric support routines.											////
////																		////
////	Those with the "My" prefix are probably not generally useful.		////
////																		////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// MyProject3DVectorsTo2D
//  v[0] is the plane normal
//	v[1] and v[2] make a [non-orthonormal] basis
//	v[3] = test vector
//
// The resultant projected triangle takes one of the following three forms:
//
//       2      |  2             |              2
//      /\      |   .            |            . 
//     /  \     |   \   .        |        .   /
//    /    \    |    \     .     |     .     /
//   ________   |     \_______   |   _______/
//  0        1        0       1      0      1
//
////////////////////////////////////////////////////////////////////////////////

static BOOL
MyProject3DVectorsTo2D(const Vector3 *v, Vector2 *u, int extraVectors)
{
	Vector3	basis[2];
		
	/* Compute the orthonormal basis */
	u[1].y = 0;															/* by construction */	;			
	if      ((u[1].x = v[1].Mag()) != 0)	basis[0] = v[1] / u[1].x;	/* Normalized basis in the direction v1 */
	else if ((u[2].x = v[2].Mag()) != 0)	basis[0] = v[2] / u[2].x;	/* Normalized basis in the direction v2, because v1 was null */
	else									basis[0] = v[1];			/* Degenerate triangle: produce zeros for all subsequent results */
	basis[1] = v[0] ^ basis[0];
	
	/* Convert coordinates from 3-space to 2-space on the plane, the 2D Y axis is the first one to be done */
	for (extraVectors++, v += 2, u += 2; extraVectors--; v++, u++) {
		u->x = *v * basis[0];
		u->y = *v * basis[1];
	}

	return (u[1].x != 0);
}




////////////////////////////////////////////////////////////////////////////////
// MyCircleIntersectsTriangle
//	(0,0) and (u[1].x,0) and u[2] make up the triangle
//	u[3] = circle center
//	u[0] is unused
////////////////////////////////////////////////////////////////////////////////

static BOOL
MyCircleIntersectsTriangle(const Point3& radius, const Vector2 *u)
{
	float radMag = radius.Mag();
	float	d;
	Vector2	bbox[2], u12, u32;
	int		x2;

	/* Determine bounding box of triangle's aura, and relative position of vertex 2 x */
	bbox[0].y = -radius.y;					/* min y */
	bbox[1].y =  radius.y + u[2].y;			/* max y */
	bbox[0].x = -radius.x;					/* min x, usual case */
	bbox[1].x =  radius.x + u[1].x;			/* max x, usual case */
	x2 = 0;									/* the usual case has x2 sittting in the middle */
	if (u[2].x > u[1].x) {		bbox[1].x = u[2].x + radius.x;	x2 =  1; }	/* overhangs on right */
	else if (u[2].x < 0) {		bbox[0].x = u[2].x - radius.x;	x2 = -1; }	/* overhangs on left */

	/* Cull against bounding box */
	if (	(u[3].x < bbox[0].x)			/* left  of triangle's bounding box */
		||	(u[3].x > bbox[1].x)			/* right of triangle's bounding box */
		||	(u[3].y < bbox[0].y)			/* below    triangle's bounding box */
		||	(u[3].y > bbox[1].y)			/* above    triangle's bounding box */
	)
		return(false);
	


	// krv:  Here we get the Mag of radius.  THis is a problem.
	// krvkrvkrv

	/* The bounding box does a tight trim on the bottom; now we trim the top */
	u12.x = u[1].x - u[2].x;				/* vector 12 */
	u12.y = u[1].y - u[2].y;
	u32.x = u[3].x - u[2].x;
	u32.y = u[3].y - u[2].y;
	if (x2 == 0) {	/* acute triangle: we can halve the test */
		if (u[3].x < u[2].x) {				/* left of vertex 2 */
			if (((d = u[2] ^ u[3]) > 0) && (d > (radMag * u[2].Norm())))
				return(false);				/* outside the triangle's aura */
		}
		else {								/* right of vertex 2 */
			if (((d = u12  ^ u32 ) > 0) && (d > (radMag * u12.Norm())))
				return(false);				/* outside the triangle's aura */
		}
	}
	else {			/* obtuse triangle: we have to do both parts of the test */
		if (	(((d = u[2] ^ u[3]) > 0) && (d > (radMag * u[2].Norm())))
			||	(((d = u12  ^ u32 ) > 0) && (d > (radMag *  u12.Norm())))
		)
			return(false);
	}
	
	return(true);
}


static BOOL
MyCircleIntersectsTriangle(float radius, const Vector2 *u)
{
	Point3 radius3( radius, radius, radius );
	return MyCircleIntersectsTriangle(radius3, u);

}


////////////////////////////////////////////////////////////////////////////////
// MySphereNormalShadowIntersectsTriangle
//  triangle[0] is the plane normal
//	(0,0,0) and triangle[1] and triangle[2] make up the triangle
//	triangle[3] = sphere center
////////////////////////////////////////////////////////////////////////////////

static BOOL
MySphereNormalShadowIntersectsTriangle(float radius, const Vector3 *triangle)
{
	Vector2	u[4];

	if (MyProject3DVectorsTo2D(triangle, u, 1))					/* Non-degenerate triangle */
		return(MyCircleIntersectsTriangle(radius, u));
	else														/* Degenerate triangle */
		return(triangle[3].MagSquared() < (radius * radius));
}


////////////////////////////////////////////////////////////////////////////////
// MyCylinderIntersectsTriangle
//  triangle[0] is the plane normal
//	(0,0,0) and triangle[1] and triangle[2] make up the triangle
//	triangle[3] = intersection point of cylinder axis with plane
//	triangle[4] = cylinder axis
////////////////////////////////////////////////////////////////////////////////

static BOOL
MyCylinderIntersectsTriangle( const Point3& radius, const Vector3 *triangle)
{
	float	s, d, M[2][2], x2, y2;
	Vector2	u[5], squ[4];
	int		i;
		
	if (MyProject3DVectorsTo2D(triangle, u, 2)) {	/* Non-degenerate triangle */
		/* Squash triangle and ellipse so that the ellipse becomes a circle */
		s = fabs(triangle[4] * triangle[0]);	/* Scale factor is the dot product between the cylinder axis and the plane normal */
		if ((s == 1.0f) || ((d = u[4].Magnitude()) == 0.0f)) {		/* Head on */
			return(MyCircleIntersectsTriangle(radius, u));
		}
		else {									/* Oblique */
			u[4] /= d;							/* Find axis of elongation */
			x2 = u[4].x * u[4].x;
			y2 = u[4].y * u[4].y;
			M[0][0] = (s * x2 + y2);	M[0][1] = ((1.0f - s) * u[4].x * u[4].y);
			M[1][0] = M[0][1];			M[1][1] = (x2 + s * y2);
			for (i = 1; i < 4; i++) {
				squ[i].x = u[i].x * M[0][0] + u[i].y * M[1][0];
				squ[i].y = u[i].x * M[0][1] + u[i].y * M[1][1];
			}
			return(MyCircleIntersectsTriangle(radius, squ));
		}
	}
	else {											/* Degenerate triangle */
		float magRadSquared = radius.MagSquared();
		return(((triangle[3] * triangle[4]) * triangle[4] - triangle[3]).MagSquared() < (magRadSquared));
	}
}


////////////////////////////////////////////////////////////////////////////////
// MyCapsuleGrazesTriangle
//		This is the case where the avatar's shoulders might be bumping against the walls.
//
//  triangle[0] is the plane normal
//	(0,0,0) and triangle[1] and triangle[2] make up the triangle
//  triangle[3] and triangle[4] are the capsule endpoints.
////////////////////////////////////////////////////////////////////////////////

static BOOL
MyCapsuleGrazesTriangle( float capsuleRadius, const Vector3 *triangle, float *t)
{
	Vector2	u[3], capMin, capMax, triMin, triMax;
	Vector3 tr[5], c[2], basis[2];
	float	rMax, t0, t1, r0, r1, r2, r, cr2, z, dtdx;
	int i;

	/* Compute coordinates relative to capsule endpoint #0. They were formerly relative to triangle vertex #0 */
	tr[0] =             - triangle[3];							/* Triangle vertex  #0 relative to capsule endpoint #0 */
	tr[1] = triangle[1] - triangle[3];							/* Triangle vertex  #1 relative to capsule endpoint #0 */
	tr[2] = triangle[2] - triangle[3];							/* Triangle vertex  #2 relative to capsule endpoint #0 */
	tr[4] = triangle[4] - triangle[3];							/* Capsule vector */


	/* Compute basis vectors and coordinates of the capsule endpoints with respect to them.
	 * We choose an orthonormal basis with the projected endpoint #0 of the capsule as the origin,
	 * the X-axis is the projected vector from capsule endpoint #0 to #1,
	 * and the triangle normal as the Z-axis.
	 * 2D points are used for everything except 3D points are used for the capsule endpoints,
	 * resulting in some ugly casting later when we coerce them to be 2D.
	 */
	c[0].z = triangle[0] * triangle[3];							/* Distance of capsule endpoint #0 from the plane */
	c[1].z = triangle[0] * triangle[4];							/* Distance of capsule endpoint #1 from the plane */
	basis[0] = tr[4] - (c[1].z - c[0].z) * triangle[0];			/* Component of capsule vector that lies in the plane */
	if ((c[1].x = basis[0].Mag()) != 0)							/* Nondegenerate capsule */
		basis[0] /= c[1].x;										/* Normalized projected capsule vector becomes basis vector 0 */
	else {														/* Degenerate capsule: we need to choose another basis */
		if ((r = triangle[1].Mag()) != 0)						/* The vector from vertex #0 to vertex #1 is nondegenerate */
			basis[0] = triangle[1];
		else if ((r = triangle[2].Mag()) != 0)					/* The vector from vertex #0 to vertex #2 is nondegenerate */
			basis[0] = triangle[2];
		else
			return(false);
		basis[0] /= r;											/* Normalize this basis vector */
	}
	c[1].y = c[0].x = c[0].y = 0;		
	basis[1] = triangle[0] ^ basis[0];							/* Cross normal with basis vector 0 to complete the frame */
	
	/* Compute triangle coordinates in the new coordinate system */
	for (i = 3; i--; ) {
		u[i].x = tr[i] * basis[0];
		u[i].y = tr[i] * basis[1];
	}
	
	/* Bounding box of capsule */
	cr2 = capsuleRadius * capsuleRadius;
	r0 = sqrt(cr2 - c[0].z * c[0].z);
	r1 = sqrt(cr2 - c[1].z * c[1].z);
	rMax = (r0 > r1) ? r0 : r1;
	capMin.x = -r0;
	capMax.x = c[1].x + r1;
	capMin.y = -rMax;
	capMax.y = rMax;
	
	/* Bounding box of triangle */
	if (u[0].x < u[1].x) {	triMin.x = u[0].x; triMax.x = u[1].x; } else { triMin.x = u[1].x; triMax.x = u[0].x; }
	if (u[2].x < triMin.x)	triMin.x = u[2].x;
	if (u[2].x > triMax.x)	triMax.x = u[2].x;
	if (u[0].y < u[1].y) {	triMin.y = u[0].y; triMax.y = u[1].y; } else { triMin.y = u[1].y; triMax.y = u[0].y; }
	if (u[2].y < triMin.y)	triMin.y = u[2].y;
	if (u[2].y > triMax.y)	triMax.y = u[2].y;
	
	/* Bounding box culling: this is pretty darn tight */
	if (	(triMin.x >= capMax.x)
		||	(triMax.x <= capMin.x)
		||	(triMin.y >= capMax.y)
		||	(triMax.y <= capMin.y)
	)
		return(false);
	
	if (t) {	/* Need to compute intersection */
		r2 = r0 * r0;
		dtdx = 1.0f / c[1].x;
		/* Check to see whether it intersects at t=0 */
		if (	PointInTriangle2D(*(Point2*)(&c[0]), (Point2*)u)
			||	(SquaredDistancePointSegment2D(*(Point2*)(&c[0]), u[0].AsPoint(), u[1].AsPoint()) < r2)
			||	(SquaredDistancePointSegment2D(*(Point2*)(&c[0]), u[1].AsPoint(), u[2].AsPoint()) < r2)
			||	(SquaredDistancePointSegment2D(*(Point2*)(&c[0]), u[2].AsPoint(), u[0].AsPoint()) < r2)
		) {
			*t = 0.0f;
			return(true);
		}
		
		/* Look for crossings */
		t0 = 10.0f;	/* across the big pond */
		if (IntersectSegmentsParametrically2D(*(Point2*)(&c[0]), *(Point2*)(&c[1]), u[0].AsPoint(), u[1].AsPoint(), &t1, NULL))
			t0 = t1;
		if (IntersectSegmentsParametrically2D(*(Point2*)(&c[0]), *(Point2*)(&c[1]), u[1].AsPoint(), u[2].AsPoint(), &t1, NULL) && (t1 < t0))
			t0 = t1;
		if (IntersectSegmentsParametrically2D(*(Point2*)(&c[0]), *(Point2*)(&c[1]), u[2].AsPoint(), u[0].AsPoint(), &t1, NULL) && (t1 < t0))
			t0 = t1;
		z = c[0].z + t0 * (c[1].z - c[0].z);	/* Height of capsule axis above/below the plane at the point of intersection */
		t0 -= sqrt(cr2 - z * z) * dtdx;			/* Backup by whatever sphere radius is left over */
		
		/* Look for close vertices */
		t1 = ClosestPointParametricallyToPointOnSegment2D(u[0].AsPoint(), *(Point2*)(&c[0]), *(Point2*)(&c[1]), &r2);
		if (((r2 = cr2 - r2) > 0) && ((t1 -= sqrt(r2) * dtdx) < t0))
			t0 = t1;
		t1 = ClosestPointParametricallyToPointOnSegment2D(u[1].AsPoint(), *(Point2*)(&c[0]), *(Point2*)(&c[1]), &r2);
		if (((r2 = cr2 - r2) > 0) && ((t1 -= sqrt(r2) * dtdx) < t0))
			t0 = t1;
		t1 = ClosestPointParametricallyToPointOnSegment2D(u[2].AsPoint(), *(Point2*)(&c[0]), *(Point2*)(&c[1]), &r2);
		if (((r2 = cr2 - r2) > 0) && ((t1 -= sqrt(r2) * dtdx) < t0))
			t0 = t1;

		/* Check for edges close to segment endpoint #1 */
		if (t0 > (1 - r1 / c[1].x)) {	/* Could be that there's a triangle edge close to segment endpoint #1 */ 
			r2 = r1 * r1;
			if ((t1 = SquaredDistancePointSegment2D(*(Point2*)(&c[1]), u[0].AsPoint(), u[1].AsPoint())) < r2) {
				if ((t1 = 1.0f - (r1 - sqrt(t1)) * dtdx) < t0)
					t0 = t1;
			}
			if ((t1 = SquaredDistancePointSegment2D(*(Point2*)(&c[1]), u[1].AsPoint(), u[2].AsPoint())) < r2) {
				if ((t1 = 1.0f - (r1 - sqrt(t1)) * dtdx) < t0)
					t0 = t1;
			}
			if ((t1 = SquaredDistancePointSegment2D(*(Point2*)(&c[1]), u[2].AsPoint(), u[0].AsPoint())) < r2) {
				if ((t1 = 1.0f - (r1 - sqrt(t1)) * dtdx) < t0)
					t0 = t1;
			}
		}

		if (t0 > 1)
			return(false);
		if (t0 < 0.0f)
			t0 = 0.0f;
		*t = t0;
	}
	return(true);
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////																		////
////								API										////
////																		////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// CAnmIntersection:: SphereIntersectTriangle
////////////////////////////////////////////////////////////////////////////////

BOOL
CAnmIntersection::SphereIntersectTriangle(const Point3 &sphereCenter, float sphereRadius, const tAnmTriangle &triangle)
{
	float	circleRadius, d;
	Vector3	v[4];

	/* Convert from an affine to a vector space, using pts[0] as the origin */
	v[1] = triangle.v[1] - triangle.v[0];	/* triangle vector 1 */
	v[2] = triangle.v[2] - triangle.v[0];	/* triangle vector 2 */
	v[3] = sphereCenter  - triangle.v[0];	/* sphere center vector */
	v[0] = v[1] ^ v[2];	v[0].Normalize();	/* triangle normal */
	
	/* Make sure the sphere intersects the plane of the triangle */
	if ((d = v[0] * v[3]) < 0) d = -d;
	if (d >= sphereRadius)
		return(false);
	
	circleRadius  = sqrt(sphereRadius * sphereRadius - d * d);
	
	return(MySphereNormalShadowIntersectsTriangle(circleRadius, v));
}


////////////////////////////////////////////////////////////////////////////////
// CAnmIntersection:: CapsuleIntersectTriangle
//
// This test is not exact. In particular, the endpoints are approximated
// with a circumscribing polygon in the plane of the triangle,
// and an exact sphere normal to the plane of the triangle.
//
// There are no false negatives.
// The maximum error for false positives is capsuleRadius * sqrt(2).
////////////////////////////////////////////////////////////////////////////////

BOOL
CAnmIntersection::CapsuleIntersectTriangle(
	const Point3		&capsulePt0,		//     one    endpoint of the capsule axis
	const Point3		&capsulePt1,		// the other  endpoint of the capsule axis
	const Point3		&capsuleRadius,		// the capsule or aura radius around the axial segment
	const tAnmTriangle	&triangle,
	float				*t
)
{
	Vector3	tv[5], cv[3];
	float	cd[3], cm[2], d, circleRadius, r;
	BOOL	intersects = true;
	float radiusNormal;

	/* Convert triangle from an affine to a vector space, using triangle.v[0] as the origin */
	tv[1] = triangle.v[1] - triangle.v[0];		/* triangle vector 1 */
	tv[2] = triangle.v[2] - triangle.v[0];		/* triangle vector 2 */
	cv[0] = capsulePt0    - triangle.v[0];		/* capsule  vector 0 */
	cv[1] = capsulePt1    - triangle.v[0];		/* capsule  vector 1 */
	cv[2] = capsulePt1    - capsulePt0;			/* capsule  vector 10 */
	tv[0] = tv[1] ^ tv[2];						/* triangle normal */
	if ((d = tv[0].Mag()) == 0) {				/* degenerate triangle */
		intersects = false;						/* @@@ We might want to do a capsule-line intersection @@@ */
		goto done;
	}
	tv[0] /= d;									/* Normalized triangle normal */
	
	/* Compute distances from capsule endpoints to the plane */
	cd[0] = tv[0] * cv[0];
	cd[1] = tv[0] * cv[1];
	cd[2] = tv[0] * cv[2];												/* Length of capsule in the normal direction */
	radiusNormal = fabs( tv[0] * capsuleRadius );

	if (cd[2] == 0) {	/* The capsule degenerates to a point, or runs parallel to the surface */
		/* Maybe we should do something special: we assume that we started outside of the intersection */
		intersects = false;
		goto done;
	}
	
	if ((cd[0] * cd[1]) > 0) {											/* The segment does not intersect the plane */
		if ((cm[0] = fabs(cd[0])) < (cm[1] = fabs(cd[1]))) {			/* Point 0 is closer (this rarely occurs) */
			if (cm[0] >= radiusNormal) {								/* but point 0 is too far away */
				intersects = false;										/* therefore: no intersection */
				goto done;
			}
			if (cm[1] >= radiusNormal) {								/* Sphere at point 1 doesn't touch plane, though sphere at point 0 does */
				tv[3] = cv[0];											/* Center of sphere at point 0 */
				r = sqrt(radiusNormal * radiusNormal - cm[0] * cm[0]);/* Radius of circle resulting from sphere-plane intersection */
				d = cm[0] - radiusNormal;								/* Backup to prevent intersection: this is necessarily negative */
				if (cd[2] != 0)											/* Capsule isn't degenerate or coplanar with the triangle */
					d /= fabs(cd[2]);									/* Enlarge distance according to 1/cos */
				else
					d = 0.0f;											/* ??? Don't move, because we can't get away from the plane */
				if (t != NULL) {
					if (d < 0.0f)
						d = 0.0f;
					*t = d;
				}
				#ifdef ALLOW_MINOR_INITIAL_PENETRATION
					if ((radiusNormal - cm[0]) <= (INITIAL_PENETRATION_EPSILON * radiusNormal)) {
						intersects = false;
						goto done;
					}
				#endif /* ALLOW_MINOR_INITIAL_PENETRATION */
				intersects = MySphereNormalShadowIntersectsTriangle(r, tv);
				goto done;
			}
			else {														/* Both points are close to the plane */
			twoClose:													/* Shoulders potentially bumping against walls */
				tv[3] = cv[0];											/* Capsule endpoint 0 */
				tv[4] = cv[1];											/* Capsule endpoint 1 */
				intersects = MyCapsuleGrazesTriangle(radiusNormal, tv, t);
				goto done;
			}
		}
		else {															/* Point 1 is closer */				
			if (cm[1] >= radiusNormal) {								/* but point 1 is too far away */
				intersects = false;
				goto done;
			}
			if (cm[0] < radiusNormal)									/* Arghh! both endpoints are close to the plane! */
				goto twoClose;
			tv[3] = cv[1];
			r = sqrt(radiusNormal * radiusNormal - cm[1] * cm[1]);
			if (t != NULL) {
				d = cm[0] - radiusNormal;								/* Backup to prevent intersection: this is necessarily negative */
				if (cd[2] != 0)											/* Capsule isn't degenerate or coplanar with the triangle */
					d /= fabs(cd[2]);									/* Enlarge distance according to 1/cos */
				else
					d = 0.0f;											/* Don't move, because we can't get away from the plane */
				if (d < 0.0f)											/* Assume that the capsule intersects the triangle solidly */
					d = 0.0f;
				*t = d;
			}
			intersects = MySphereNormalShadowIntersectsTriangle(r, tv);
			goto done;
		}
	}
	else {																/* The segment intersects the plane */
		if (cd[2] == 0)
			goto twoClose;												/* 2D test */
		d = -cd[0] / cd[2];												/* Parametric value of intersection of axis with plane */
		tv[3] = cv[0] + d * cv[2];										/* Intersection point: capsule intersects as an ellipse */
		tv[4] = cv[2];													/* Axis of capsule */
		tv[4].Normalize();												/* Normalized axis of capsule */
		if (t != NULL) {
			d -= radiusNormal / fabs(cd[2]);							/* Offset intersection by radius of sphere */
			if (d < 0.0f)
				d = 0.0f;
			*t = d;
		}
		intersects = MyCylinderIntersectsTriangle(capsuleRadius, tv);
		if (intersects == 0)
		{
			int debug = 1;
		}

		goto done;
	}
	intersects = true;
done:
//	#define TEST
	#ifdef TEST /* See if we got all of the Nan's and infinities */
		/* None of these tests seem to ever be true */
		if (intersects && (t != NULL) && (!(-HUGE_VAL < *t) || !(*t < HUGE_VAL)))
			*t = 0;
		if (intersects && (t != NULL) && ((*t < -0.1) || (*t > 1)))
			intersects = 0;
	#endif TEST
	return(intersects);
}


BOOL
CAnmIntersection::CapsuleIntersectBox(
	const Point3		&capsulePt0,		//     one    endpoint of the capsule axis
	const Point3		&capsulePt1,		// the other  endpoint of the capsule axis
	float				capsuleRadius,		// the capsule or aura radius around the axial segment
	const Point3		&minBox,			// the minimal {x,y,z} of the box
	const Point3		&maxBox				// the maximal {x,y,z} of the box
)
{
	Point3 capsuleRadius3( capsuleRadius, capsuleRadius, capsuleRadius );
	return CapsuleIntersectBox( capsulePt0, capsulePt1, capsuleRadius3, minBox, maxBox );
}


////////////////////////////////////////////////////////////////////////////////
// CAnmIntersection:: CapsuleIntersectBox
//
// This test is not exact. In particular, the endpoints are approximated with a circumscribing polyhedron.
// There are no false negatives.
// The maximum error for false positives is capsuleRadius * sqrt(3).
////////////////////////////////////////////////////////////////////////////////

BOOL
CAnmIntersection::CapsuleIntersectBox(
	const Point3		&capsulePt0,		//     one    endpoint of the capsule axis
	const Point3		&capsulePt1,		// the other  endpoint of the capsule axis
	const Point3		&capsuleRadius,		// the capsule or aura radius around the axial segment
	const Point3		&minBox,			// the minimal {x,y,z} of the box
	const Point3		&maxBox				// the maximal {x,y,z} of the box
)
{
	Point3	minCap, maxCap;
	
	/* Construct a bounding box for the capsule.
	 * It would be more efficient if this were done once beforehand.
	 */
	if (capsulePt0.x < capsulePt1.x) { minCap.x = capsulePt0.x; maxCap.x = capsulePt1.x; }	else { minCap.x = capsulePt1.x; maxCap.x = capsulePt0.x; }
	if (capsulePt0.y < capsulePt1.y) { minCap.y = capsulePt0.y; maxCap.y = capsulePt1.y; }	else { minCap.y = capsulePt1.y; maxCap.y = capsulePt0.y; }
	if (capsulePt0.z < capsulePt1.z) { minCap.z = capsulePt0.z; maxCap.z = capsulePt1.z; }	else { minCap.z = capsulePt1.z; maxCap.z = capsulePt0.z; }
	minCap.x -= capsuleRadius.x;	maxCap.x += capsuleRadius.x;
	minCap.y -= capsuleRadius.y;	maxCap.y += capsuleRadius.y;
	minCap.z -= capsuleRadius.z;	maxCap.z += capsuleRadius.z;
	
	/* Compare capsule and bounding box */
	if (	(minCap.x >= maxBox.x)	||	(maxCap.x <= minBox.x)
		||	(minCap.y >= maxBox.y)	||	(maxCap.y <= minBox.y)
		||	(minCap.z >= maxBox.z)	||	(maxCap.z <= minBox.z)
	)
		return(false);

	return true;
}

