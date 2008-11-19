/********************************************************************************
 * Flux
 *
 * File: anmfrustum.cpp
 * Description: Frustum support
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
#include "anmfrustum.h"
#include "anmvolumes.h"

#define TOP_PLANE		0
#define BOTTOM_PLANE	1
#define LEFT_PLANE		2
#define RIGHT_PLANE		3
#define HITHER_PLANE	4
#define YON_PLANE		5

//const float	rootHalf = 0.7071067812f;


////////////////////////////////////////////////////////////////////////////////
// Dot(const point3 &v, const point3 &p)
// Only for internal use -- bad form -- do not export
////////////////////////////////////////////////////////////////////////////////

static inline float Dot(const point3 &v, const point3 &p)
{
	return((float)(	(double)p.x * (double)v.x
				+	(double)p.y * (double)v.y
				+	(double)p.z * (double)v.z
			)
	);
}


////////////////////////////////////////////////////////////////////////////////
// CAnmFrustum::CAnmFrustum(void)
////////////////////////////////////////////////////////////////////////////////

CAnmFrustum::CAnmFrustum(void)
{
	Set(
		point3( 0, 0,  1),	// Look toward the origin from +Z
		point3(0, 0, -1),	// Looking down the -Z axis
		point3(0, 1,  0),	// +Y is up
		1, 1,				// 90 degree FOV in X and Y
		1, 1000				// 1000:1 Z range
	);
}


////////////////////////////////////////////////////////////////////////////////
// CAnmFrustum::Set(p0, p1, p2, p3, p4, p5)
////////////////////////////////////////////////////////////////////////////////

void CAnmFrustum::Set(const plane3 &p0, const plane3 &p1, const plane3 &p2, const plane3 &p3, const plane3 &p4, const plane3 &p5)
{
	m_halfSpace[0] = p0;
	m_halfSpace[1] = p1;
	m_halfSpace[2] = p2;
	m_halfSpace[3] = p3;
	m_halfSpace[4] = p4;
	m_halfSpace[5] = p5;
};


////////////////////////////////////////////////////////////////////////////////
// CAnmFrustum::Get(p0, p1, p2, p3, p4, p5)
////////////////////////////////////////////////////////////////////////////////

void CAnmFrustum::Get(plane3 &p0, plane3 &p1, plane3 &p2, plane3 &p3, plane3 &p4, plane3 &p5) const
{
	p0 = m_halfSpace[0];
	p1 = m_halfSpace[1];
	p2 = m_halfSpace[2];
	p3 = m_halfSpace[3];
	p4 = m_halfSpace[4];
	p5 = m_halfSpace[5];
};


////////////////////////////////////////////////////////////////////////////////
// CAnmFrustum::Set(location, lookat, up, tanX, tanY, hither, yon)
////////////////////////////////////////////////////////////////////////////////

void CAnmFrustum::Set(
	const point3	&location,	// location of the camera
	const point3	&aLookat,	// viewing direction
	const point3	&aUp,		// up vector
	float			tanX,		// tangent of half the horizontal view angle
	float			tanY,		// tangent of half the vertical   view angle
	float			hither,		// near clipping plane
	float			yon			// far clipping plane
)
{
	point3	p;
	float	c, s, d;
	point3	par, perp;
	point3	lookat = aLookat, up = aUp;
	
	// This makes lookat and up orthonormal; uncomment below if this is not true
	// lookat.Normalize();
	// up -= Dot(up, lookat) * lookat;	// Gram-Schmidt orthogonalization
	// up.Normalize();

	// Here we assume that lookat & up are orthonormal.
	
	// hither
	m_halfSpace[HITHER_PLANE] = plane3(lookat, location + lookat * hither);
	
	// yon
	m_halfSpace[YON_PLANE] = plane3(-lookat, location + lookat * yon);

	// top & bottom
	c = 1.0 / sqrt(tanY * tanY + 1.0);
	s = tanY * c;
	par  = s * lookat;
	perp = c * up;
	m_halfSpace[   TOP_PLANE] = plane3(par - perp, location);
	m_halfSpace[BOTTOM_PLANE] = plane3(par + perp, location);
	
	// left & right
	c = 1.0 / sqrt(tanX * tanX + 1.0);
	s = tanX * c;
	par  = s * lookat;
	perp = c * (lookat ^ up);	// = c * right
	m_halfSpace[RIGHT_PLANE] = plane3(par - perp, location);
	m_halfSpace[ LEFT_PLANE] = plane3(par + perp, location);
}


////////////////////////////////////////////////////////////////////////////////
// CAnmFrustum::SetOrtho( x0, x1, y0, y1, z0, z1 )
////////////////////////////////////////////////////////////////////////////////

void CAnmFrustum::SetOrtho(
	float			x0,			// Left
	float			x1, 		// right
	float			y0,			// top
	float			y1,			// bottom
	float			hither,		// Near Clip Plane
	float			yon			// Far Clipping Plane
)
{
	m_halfSpace[HITHER_PLANE]	= plane3( Point3( 0.0,	0.0, 1.0 ), Point3( 0.0, 0.0, hither ) );
	m_halfSpace[YON_PLANE]		= plane3( Point3( 0.0,	0.0, -1.0 ), Point3( 0.0, 0.0,  yon) );
	m_halfSpace[TOP_PLANE]		= plane3( Point3( 0.0,	1.0, 0.0 ), Point3( 0.0, y0, 0.0 ) );
	m_halfSpace[BOTTOM_PLANE]	= plane3( Point3( 0.0, -1.0, 0.0 ), Point3( 0.0, y1, 0.0 ) );
	m_halfSpace[RIGHT_PLANE]	= plane3( Point3( 1.0,	0.0, 0.0 ), Point3( x0, 0.0, 0.0 ) );
	m_halfSpace[LEFT_PLANE]		= plane3( Point3( -1.0, 0.0, 0.0 ), Point3( x1, 0.0, 0.0 ) );
}


////////////////////////////////////////////////////////////////////////////////
// CAnmFrustum::CAnmFrustum(const CAnmBoundingSphere &bSphere)
////////////////////////////////////////////////////////////////////////////////

bool CAnmFrustum::Cull(const CAnmBoundingSphere &bs) const
{
	const plane3	*pp;
	int				i;
	bSphere3		bsph(bs.GetRadius(), bs.GetCenter());
	
	for (i = 6, pp = m_halfSpace; i--; pp++)
		if (pp->TestBSphere(bsph) == sphereBack)	// behind any plane
			return true;	// guaranteed invisible

	return false;			// might be visible
}


////////////////////////////////////////////////////////////////////////////////
// CAnmFrustum::CAnmFrustum(const CAnmBoundingSphere &bSphere)
////////////////////////////////////////////////////////////////////////////////

bool CAnmFrustum::Cull(CAnmBoundingBox &bBox) const
{
	const plane3	*pp;
	int				i;
	Point3 			corners[8];
	
	bBox.GetCorners(corners);
	for (i = 6, pp = m_halfSpace; i--; pp++)
		if (pp->TestPList(corners, 8) == plistBack)	// behind any plane
			return true;	// guaranteed invisible

	return false;
}

eFrustumLoc CAnmFrustum::TestBox(CAnmBoundingBox &bBox) const
{
	bool allinside = true, alloutside = true;

	const plane3	*pp;
	int				i;
	Point3 			corners[8];
	
	bBox.GetCorners(corners);
	for (i = 6, pp = m_halfSpace; i--; pp++)
	{
		ePListLoc plistloc = pp->TestPList(corners, 8);
		if (plistloc != plistFront)	// behind any plane
		{
			allinside = false;
		}
		else if (plistloc != plistBack)
		{
			alloutside = false;
		}
	}

	if( allinside && !alloutside)
	{
		// Everything's inside
		return eInsideFrustum;
	}
	else if( !allinside && alloutside)
	{
		// Everything's outside
		return eOutsideFrustum;
	}
	else // N.B.: probably don't need this if( !allinside && !alloutside )
	{
		// Some were in front, some were in back
		return eIntersectFrustum;
	}

}

eFrustumLoc CAnmFrustum::TestPoint(Point3 &p3) const
{
	bool allinside = true, alloutside = true;

	const plane3	*pp;
	int				i;
	
	for (i = 6, pp = m_halfSpace; i--; pp++)
	{
		ePListLoc plistloc = pp->TestPList(&p3, 1);
		if (plistloc != plistFront)	// behind any plane
		{
			allinside = false;
		}
		else if (plistloc != plistBack)
		{
			alloutside = false;
		}
	}

	if( allinside && !alloutside)
	{
		// Everything's inside
		return eInsideFrustum;
	}
	else if( !allinside && alloutside)
	{
		// Everything's outside
		return eOutsideFrustum;
	}
	else // N.B.: probably don't need this if( !allinside && !alloutside )
	{
		// Some were in front, some were in back
		return eIntersectFrustum;
	}

}


//N.B.: a work in progress...
void CAnmFrustum::GetCenter(point3 &center)
{
	point3 p[3];
	plane3 p1, p2;

	for (int i = 0; i < 2; i++)
	{
		p1 = m_halfSpace[i * 2];
		p2 = m_halfSpace[i * 2 + 1];
		p[i] = ((p1.d * p1.n) + (p2.d * p2.n)) / 2;
	}

	center.x = p[0].x;
	center.y = p[1].y;
	center.z = p[2].z;
}

