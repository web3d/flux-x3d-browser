/********************************************************************************
 * Flux
 *
 * File: anmfrustum.h
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

#ifndef _anmfrustum_h
#define _anmfrustum_h

// Opaque class declarations
class CAnmBoundingSphere;
class CAnmBoundingBox;

typedef enum eFrustumLoc {
	eInsideFrustum,
	eOutsideFrustum,
	eIntersectFrustum,
} eBoxFrustumLoc ;

class CAnmFrustum {
protected:
	plane3 m_halfSpace[6];
	
public:
	////////// Constructors //////////
	
	CAnmFrustum(void);				// canonical frustum: don't rely on the values


	////////// Accessors //////////
	
	void Set(const plane3 &p0, const plane3 &p1, const plane3 &p2, const plane3 &p3, const plane3 &p4, const plane3 &p5);
	void Get(plane3 &p0, plane3 &p1, plane3 &p2, plane3 &p3, plane3 &p4, plane3 &p5) const;
	
	void SetHalfSpace(int i, const plane3 &plane)	{ m_halfSpace[i] = plane; }
	const plane3& GetHalfSpace(int i)			{ return(m_halfSpace[i]); }
	
	void Set(
		const  point3	&location,	// location of the camera
		const point3	&lookat,	// viewing direction
		const point3	&up,		// up vector
		float			tanX,		// tangent of half the horizontal view angle
		float			tanY,		// tangent of half the vertical   view angle
		float			hither,		// near clipping plane
		float			yon			// far clipping plane
	);

	void SetOrtho(
		float			x0,			// Left
		float			x1, 		// right
		float			y0,			// top
		float			y1,			// bottom
		float			hither,		// Near Clip Plane
		float			yon			// Far Clipping Plane
	);

	////////// Member Functions //////////
	
	bool Cull(const CAnmBoundingSphere &bSphere) const;	// returns true if not visible
	bool Cull(CAnmBoundingBox &bBox) const;		// returns true if not visible
	//bool Cull(const CAnmPolygon &poly) const;			// returns true if not visible

	eFrustumLoc TestBox(CAnmBoundingBox &bBox) const; // categorize box (in, out, partial)
	eFrustumLoc TestPoint(Point3 &p3) const; // categorize point (in, out, partial)

	void GetCenter(point3 &center);
};


#endif // _anmfrustum_h
