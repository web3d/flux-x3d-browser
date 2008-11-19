/********************************************************************************
 * Flux
 *
 * File: anmoctree.h
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

#ifndef _anmoctree_h
#define _anmoctree_h

#include "anmvolumes.h"

#define ANMOCTREE_MAXTRIANGLES 512

class CAnmOctree {
protected :

	int				*m_triangleIndices;
	int				 m_triangleCount;
	CAnmOctree		*m_octants[8];
	CAnmBoundingBox	 m_bbox;

	void Build(class CAnmPrimitive *pPrim, int *triangleIndices, int nTriangles,
		CAnmBoundingBox bbox, int iDepth = 0 );

	static bool m_drawBoxes;

public :

	CAnmOctree(class CAnmPrimitive *pPrim, CAnmBoundingBox bbox);
	CAnmOctree(class CAnmPrimitive *pPrim, int *triangleIndices, int nTriangles,
		CAnmBoundingBox bbox, int iDepth = 0 );
	~CAnmOctree();

	bool TestPick(class CAnmPrimitive *pPrim, Point3 start, Point3 end, struct sAnmRenderGraphPickList *pPickList);
	bool SphereCollide(class CAnmPrimitive *pPrim, Point3 center, float radius, struct sAnmRenderGraphPickList *pPickList);
	bool CapsuleCollide(class CAnmPrimitive *pPrim, const Point3 &capsulePt0, const Point3 &capsulePt1, const Point3& capsuleRadius,
		struct sAnmRenderGraphPickList *pPickList);

	char OctIndex(Point3 v, Point3 c);

	void Render(class CAnmDevice *pDevice);

	static void SetDrawBoxes(bool drawBoxes);
	static bool GetDrawBoxes();

	// Accessors
	int GetTriangleIndexCount()
	{
		return m_triangleCount;
	}

	int *GetTriangleIndices()
	{
		return m_triangleIndices;
	}

	bool IsLeaf()
	{
		return m_triangleIndices != NULL;
	}

	void GetBoundingBox(CAnmBoundingBox *pBoundingBox)
	{
		assert(pBoundingBox);
		*pBoundingBox = m_bbox;
	}

	void GetOctants(CAnmOctree *octants[8])
	{
		for (int i = 0; i < 8; i++)
			octants[i] = m_octants[i];
	}

};

// generate one of 8 subtree indexes based on vertex's location relative to center
inline char CAnmOctree::OctIndex(Point3 v, Point3 c)
{
	char index = 0;

	if (v.z > c.z)
		index |= 1;
	if (v.y > c.y)
		index |= 2;
	if (v.x > c.x)
		index |= 4;

	return index;
}

#endif // _anmoctree_h
