/********************************************************************************
 * Flux
 *
 * File: anmoctree.cpp
 * Description: Octree sorting
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
#include "anmoctree.h"
#include "anmprimitive.h"
#include "anmintersection.h"
#include "anmpickmanager.h"
#include "anmvolumes.h"
#include "anmmessage.h"

#include "anmdevice.h"



// krv: limit to the number of times we can recurse.
//
//    50 is too big. will cause crash.
// #define MAX_OCTREE_DEPTH		20

// Disable this feature due to fears of performance problems.
#define MAX_OCTREE_DEPTH		200000

CAnmOctree::CAnmOctree(class CAnmPrimitive *pPrim, CAnmBoundingBox bbox)
{
	assert(pPrim != NULL);

	int nTriangles = pPrim->NumTriangles();

	int *pTriangles = new int[nTriangles];

	for (int i = 0; i < nTriangles; i++)
		pTriangles[i] = i;

	Build(pPrim, pTriangles, nTriangles, bbox);
}

CAnmOctree::CAnmOctree(class CAnmPrimitive *pPrim, int *triangleIndices, int nTriangles,
		CAnmBoundingBox bbox, int iDepth )
{
	Build(pPrim, triangleIndices, nTriangles, bbox, iDepth );
}

CAnmOctree::~CAnmOctree()
{
	for (int oi = 0; oi < 8; oi++)
		SafeDelete(m_octants[oi]);

	SafeDelete(m_triangleIndices);

}


void CAnmOctree::Build(class CAnmPrimitive *pPrim, int *triangleIndices, int nTriangles,
		CAnmBoundingBox bbox, int iDepth )
{
	// initialize stuff
	m_triangleIndices = NULL;
	m_triangleCount = 0;
	m_bbox = bbox;
	Point3 bboxCenter, bboxSize;
	bbox.GetCenterSize(&bboxCenter, &bboxSize);
	for (char oi = 0; oi < 8; oi++)
		m_octants[oi] = NULL;


/************************
	// krv: Use iDepth count as limit instead of geometric boundingbox size
#define OCTREE_FRACTION_LIMIT	( 0.0001 )

	float sclSize =		fabs( bboxSize.x ) +	fabs( bboxSize.y ) +	fabs( bboxSize.z );
	float sclCenter =	fabs( bboxCenter.x ) +	fabs( bboxCenter.y ) +	fabs( bboxCenter.z );
	if( sclCenter > 0.00000001 &&
		sclSize / sclCenter < OCTREE_FRACTION_LIMIT ) {

		m_triangleIndices = triangleIndices;
		m_triangleCount = nTriangles;
		return;
	}
************************/

	// Make sure that we dont go too deep..
	// we're a leaf; just nab the index list and we're done
	if (nTriangles <= ANMOCTREE_MAXTRIANGLES || 
		iDepth > MAX_OCTREE_DEPTH )
	{
		m_triangleIndices = triangleIndices;
		m_triangleCount = nTriangles;
		return;
	}

	int i, tindex;
	char oi1, oi2, oi3;
	Point3 p;

	// scan entire list, getting counts first to save dynamic reallocs
	int counts[8];
	for (oi = 0; oi < 8; oi++)
		counts[oi] = 0;



	for (i = 0; i < nTriangles; i++)
	{
		tindex = triangleIndices[i];

		tAnmTriangle triangle;
		pPrim->GetTriangle(tindex, triangle);

		// note: some triangles may end up in more than one octant; ok because
		// we're not trying to avoid overdraw with this data structure
		oi1 = OctIndex(triangle.v[0], bboxCenter);
		counts[oi1]++;
		oi2 = OctIndex(triangle.v[1], bboxCenter);
		if (oi2 != oi1)
			counts[oi2]++;
		oi3 = OctIndex(triangle.v[2], bboxCenter);
		// $$$Bug xxxx sketchy octree logic; shoulda been AND, duh -- TP 1/30/03
		// if (oi3 != oi2 || oi3 != oi1)
		if (oi3 != oi2 && oi3 != oi1)
			counts[oi3]++;
	}

	int total = 0;
	for (oi = 0; oi < 8; oi++)
		total += counts[oi];

	// allocate and init new triangle arrays
	int *indices[8];
	int indexCounters[8];
	for (oi = 0; oi < 8; oi++)
	{
		if (counts[oi] > 0)
			indices[oi] = new int[counts[oi]];

		indexCounters[oi] = 0;
	}

	for (i = 0; i < nTriangles; i++)
	{
		tindex = triangleIndices[i];

		tAnmTriangle triangle;
		pPrim->GetTriangle(tindex, triangle);

		// note: some triangles may end up in more than one octant; ok because
		// we're not trying to avoid overdraw with this data structure
		oi1 = OctIndex(triangle.v[0], bboxCenter);
		indices [oi1] [ indexCounters[oi1]++ ] = tindex;

		oi2 = OctIndex(triangle.v[1], bboxCenter);
		if (oi2 != oi1)
			indices [oi2] [ indexCounters[oi2]++ ] = tindex;

		oi3 = OctIndex(triangle.v[2], bboxCenter);
		// $$$Bug xxxx sketchy octree logic; shoulda been AND, duh -- TP 1/30/03
		// if (oi3 != oi2 || oi3 != oi1)
		if (oi3 != oi2 && oi3 != oi1)
			indices [oi3] [ indexCounters[oi3]++ ] = tindex;
	}

	// N.B.: delete passed-in triangle array now that we've copied the data. is that weird?
	delete triangleIndices;

	Point3 newSize = bboxSize * .5f;
	Point3 newCenter;

/*
	Experimental:

	if (newSize.x < FLT_EPSILON &&
			newSize.y < FLT_EPSILON &&
			newSize.z < FLT_EPSILON)
			return;
*/
	// now create the children
	for (oi = 0; oi < 8; oi++)
	{
		if (oi & 1)		// +z
			newCenter.z = bboxCenter.z + newSize.z / 2;
		else
			newCenter.z = bboxCenter.z - newSize.z / 2;

		if (oi & 2)		// +y
			newCenter.y = bboxCenter.y + newSize.y / 2;
		else
			newCenter.y = bboxCenter.y - newSize.y / 2;

		if (oi & 4)		// +x
			newCenter.x = bboxCenter.x + newSize.x / 2;
		else
			newCenter.x = bboxCenter.x - newSize.x / 2;

		CAnmBoundingBox newBBox(newCenter, newSize);

		if (counts[oi])
		{
			m_octants[oi] = new CAnmOctree(pPrim, indices[oi], counts[oi], newBBox, iDepth+1 );

			if (m_octants[oi] == NULL)
				// ANMMSG_OCTREEALLOCFAILED : "Octree allocation failed."
				throw CAnmError(ANMMSG_OCTREEALLOCFAILED);
		}
	}
}


bool CAnmOctree::TestPick(CAnmPrimitive *pPrim, Point3 start, Point3 end, sAnmRenderGraphPickList *pPickList)
{
	// first, trivial reject on bounding box
	Point3 p, c, s;
	m_bbox.GetCenterSize(&c, &s);

	if (!CAnmIntersection::RayIntersectBox(start, end, c, s, &p))
		return false;

	bool picked = false;
	// now, see if we're a leaf
	if (m_triangleIndices)
	{
		// we're a leaf; just do linear search
		for(int i = 0; i < m_triangleCount; i++)
		{
			int tindex = m_triangleIndices[i];
			tAnmTriangle triangle;
			pPrim->GetTriangle(tindex, triangle);

			sAnmRenderGraphPickDesc pickDesc;

			if (CAnmIntersection::RayIntersectTriangle(start, end, triangle, &p))
			{
				memset(&pickDesc, 0, sizeof(pickDesc));
				pickDesc.hitPoint = p;

				plane3 plane(triangle.v[0], triangle.v[1], triangle.v[2]);
				pickDesc.hitNormal = plane.n;

				pPickList->Append(pickDesc);

				picked = true;
			}
		}

		return picked;
	}

	// otherwise search our children
	for (int oi = 0; oi < 8; oi++)
	{
		if (m_octants[oi] != NULL)
		{
			if (m_octants[oi]->TestPick(pPrim, start, end, pPickList))
				picked = true;
		}
	}

	return picked;
}

bool CAnmOctree::SphereCollide(CAnmPrimitive *pPrim, Point3 center, float radius, sAnmRenderGraphPickList *pPickList)
{
	// first, trivial reject on bounding box
	Point3 p, c, s;
	m_bbox.GetCenterSize(&c, &s);

	if (!CAnmIntersection::SphereIntersectBox(center, radius, c, s))
		return false;

	bool collide = false;
	// now, see if we're a leaf
	if (m_triangleIndices)
	{
		// we're a leaf; just do linear search
		for(int i = 0; i < m_triangleCount; i++)
		{
			int tindex = m_triangleIndices[i];
			tAnmTriangle triangle;
			pPrim->GetTriangle(tindex, triangle);

			sAnmRenderGraphPickDesc pickDesc;

			if (CAnmIntersection::SphereIntersectTriangle(center, radius, triangle))
			{
				memset(&pickDesc, 0, sizeof(pickDesc));

				// N.B.: this isn't really the intersection point; 
				// but we'll just take one of the corners for sorting
				pickDesc.hitPoint = triangle.v[0];

				plane3 plane(triangle.v[0], triangle.v[1], triangle.v[2]);
				pickDesc.hitNormal = plane.n;

				pPickList->Append(pickDesc);

				collide = true;
			}
		}

		return collide;
	}

	// otherwise search our children
	for (int oi = 0; oi < 8; oi++)
	{
		if (m_octants[oi] != NULL)
		{
			if (m_octants[oi]->SphereCollide(pPrim, center, radius, pPickList))
				collide = true;
		}
	}

	return collide;
}

bool CAnmOctree::CapsuleCollide(class CAnmPrimitive *pPrim, const Point3 &capsulePt0, const Point3 &capsulePt1, 
								const Point3& capsuleRadius,
		sAnmRenderGraphPickList *pPickList)
{
	// first, trivial reject on bounding box
	Point3 p, min, max;
	m_bbox.GetMinMax(&min, &max);

	if (!CAnmIntersection::CapsuleIntersectBox(capsulePt0, capsulePt1, capsuleRadius,
			min, max))
		return false;

	bool collide = false;
	// now, see if we're a leaf
	if (m_triangleIndices)
	{
		// we're a leaf; just do linear search
		for(int i = 0; i < m_triangleCount; i++)
		{
			int tindex = m_triangleIndices[i];
			tAnmTriangle triangle;
			pPrim->GetTriangle(tindex, triangle);

			float t;

			sAnmRenderGraphPickDesc pickDesc;

			if (CAnmIntersection::CapsuleIntersectTriangle(capsulePt0, capsulePt1,
					capsuleRadius, triangle, &t))
			{
				memset(&pickDesc, 0, sizeof(pickDesc));

				Point3 dir = capsulePt1 - capsulePt0;
				pickDesc.hitPoint = capsulePt0 + (dir * t);

				plane3 plane(triangle.v[0], triangle.v[1], triangle.v[2]);
				pickDesc.hitNormal = plane.n;

				pPickList->Append(pickDesc);

				collide = true;
			}
		}

		return collide;
	}

	// otherwise search our children
	for (int oi = 0; oi < 8; oi++)
	{
		if (m_octants[oi] != NULL)
		{
			if (m_octants[oi]->CapsuleCollide(pPrim, capsulePt0, capsulePt1, 
					capsuleRadius, pPickList))
				collide = true;
		}
	}

	return collide;
}

void CAnmOctree::Render(CAnmDevice *pDevice)
{
	if (!m_drawBoxes)
		return;

	Point3 corners[8];

	m_bbox.GetCorners(corners);

	cApplication *pApp = pDevice->GetApp();
	assert(pApp);

	CAnmGraphics *pGraphics = pApp->GetGraphics();
	assert(pGraphics);
	
	pGraphics->DrawBox(corners, color3::Green, pDevice);

	for (int oi = 0; oi < 8; oi++)
	{
		if (m_octants[oi])
			m_octants[oi]->Render(pDevice);
	}
}

bool CAnmOctree::m_drawBoxes = false;
void CAnmOctree::SetDrawBoxes(bool drawBoxes)
{
	m_drawBoxes = drawBoxes;
}

bool CAnmOctree::GetDrawBoxes()
{
	return m_drawBoxes;
}

