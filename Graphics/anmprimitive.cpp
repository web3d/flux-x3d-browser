/********************************************************************************
 * Flux
 *
 * File: anmprimitive.cpp
 * Description: Primitive class - support for triangle
 *							primitives
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
#include "anmprimitive.h"
#include "anmdevice.h"
#include "anmoctree.h"
#include "anmintersection.h"
#include "anmpickmanager.h"
#include "anmvolumes.h"

#include <gl/glu.h>

CAnmPrimitive::CAnmPrimitive(CAnmDevice *pDevice, eAnmPrimitiveType ptype, bool indexed)
{
	m_primtype = ptype;
	m_indexed = indexed;
	m_diffuseColor = 0L;
	m_octree = NULL;
	m_backfaces = NULL;
	m_texturestage = 0;

	assert(pDevice);

	cApplication *pApp = pDevice->GetApp();
	assert(pApp);

	CAnmGraphics *pGraphics = pApp->GetGraphics();
	assert(pGraphics);

	m_renderprimitive = pGraphics->CreatePrimitive(this);

}

CAnmPrimitive::~CAnmPrimitive()
{
	// N.B.: somebody has to memory manage the two lists;
	// I think we need to make them Objects

	SafeDelete(m_octree);
	SafeDelete(m_renderprimitive);
	SafeDelete(m_backfaces);
}

void CAnmPrimitive::Render(CAnmDevice *pDevice)
{
	assert(pDevice);
	assert(m_renderprimitive);
	
	m_renderprimitive->Render(pDevice);

	// Render backfaces if present
	if (m_backfaces)
		m_backfaces->Render(pDevice);

#ifdef DEBUG
	// N.B.: this was only for debugging the octrees
	if (m_octree && CAnmOctree::GetDrawBoxes())
		m_octree->Render(pDevice);
#endif
}

void CAnmPrimitive::AddVertex(sAnmVertex vert)
{
	m_vertices.push_back(vert);
	InvalidateBoundingBox();
}

void CAnmPrimitive::AddIndex(WORD index)
{
	m_indices.push_back(index);
	InvalidateBoundingBox();
}

void CAnmPrimitive::AddVertices(int vertexcount, sAnmVertex *pVertices)
{
	int curcount = m_vertices.size();

	m_vertices.reserve(curcount + vertexcount);

	for (int i = 0; i < vertexcount; i++)
	{
		m_vertices.push_back(pVertices[i]);
	}

	InvalidateBoundingBox();
}

void CAnmPrimitive::AddIndices(int indexcount, WORD *pIndices)
{
	int curcount = m_indices.size();

	m_indices.reserve(curcount + indexcount);

	for (int i = 0; i < indexcount; i++)
	{
		m_indices.push_back(pIndices[i]);
	}

	InvalidateBoundingBox();
}

void CAnmPrimitive::SetVertex(int vindex, sAnmVertex v)
{
	assert(vindex >= 0 && vindex < m_vertices.size());

	m_vertices[vindex] = v;

	InvalidateBoundingBox();
}

sAnmVertex CAnmPrimitive::GetVertex(int vindex)
{
	assert(vindex >= 0 && vindex < m_vertices.size());

	return m_vertices[vindex];
}

void CAnmPrimitive::SetCoordinates(Point3Array *pCoords)
{
	assert(pCoords != NULL);

	int csize = pCoords->size();

	if (csize > m_vertices.size())
		csize = m_vertices.size();

	for (int i = 0; i < csize; i++)
	{
		m_vertices[i].vec = (*pCoords) [i];
	}

	// N.B.: forget about octree sorting and morphs on the same object
	// can't do it per-frame. so if somebody changes our coordinates,
	// just bail on the sorting
	if (m_octree)
		SafeDelete(m_octree);

	// If vertices are buffered, recopy data
	UpdatePrimitive();

	InvalidateBoundingBox();
}

void CAnmPrimitive::SetDiffuseColor(ULONG diffuseColor)
{
	if (diffuseColor != m_diffuseColor)
	{
		for (int i = 0; i < m_vertices.size(); i++)
		{
			m_vertices[i].SetDiffuseColor(diffuseColor);
		}

		m_diffuseColor = diffuseColor;
	}

	// If vertices are buffered, recopy data
	UpdatePrimitive();

	// Set diffuse color in backfaces if they're there
	if (m_backfaces)
		m_backfaces->SetDiffuseColor(diffuseColor);

}

void CAnmPrimitive::SetDiffuseColor(color4 diffuseColor)
{
	SetDiffuseColor(diffuseColor.MakeDWord());
}

void CAnmPrimitive::Scale(Float scalefactor)
{
	for (int i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].vec *= scalefactor;
	}

	// If vertices are buffered, recopy data
	UpdatePrimitive();

	InvalidateBoundingBox();
}

void CAnmPrimitive::MaybeMakeOctree()
{
	if (m_octree != NULL)
		return;

	// if nothing else, let's make sure our bbox is valid...
	if (!m_bbox.IsValid())
		GenBoundingBox();

	assert(m_bbox.IsValid());

	// no need for an octree; we don't have that many vertices
	if (NumTriangles() <= ANMOCTREE_MAXTRIANGLES)
		return;

	Point3 c, s;
	m_bbox.GetCenterSize(&c, &s);

	if (s.Mag() <= FLT_EPSILON)
		return;

	m_octree = new CAnmOctree(this, m_bbox);
}

void CAnmPrimitive::Sort()
{
	MaybeMakeOctree();

	if (m_backfaces)
		m_backfaces->Sort();
}

eAnmReturnStatus CAnmPrimitive::BufferVertices(CAnmDevice *pDevice)
{
	if (m_renderprimitive == NULL)
		return eAnmReturnAllBad;

	assert(pDevice);

	eAnmReturnStatus myretstat = eAnmReturnAllGood;
	m_renderprimitive->Buffer(pDevice);

	// Buffer backfaces too if they're there
	eAnmReturnStatus backfaceretstat = eAnmReturnAllGood;
	if (m_backfaces)
	{
		backfaceretstat = m_backfaces->BufferVertices(pDevice);
	}

	if (myretstat == eAnmReturnAllGood && backfaceretstat == eAnmReturnAllGood)
		return eAnmReturnAllGood;
	else
		return eAnmReturnAllBad;

}

eAnmReturnStatus CAnmPrimitive::UpdatePrimitive()
{
	if (m_renderprimitive == NULL)
		return eAnmReturnAllBad;

	m_renderprimitive->UpdateBuffer();

	return eAnmReturnAllGood;
}

void CAnmPrimitive::InvalidateBoundingBox()
{
	CAnmBoundingBox bbox;
	m_bbox = bbox;
}

void CAnmPrimitive::GenBoundingBox()
{
	Point3 minbox(0, 0, 0);
	Point3 maxbox(0, 0, 0);
	Point3 center(0, 0, 0);
	BOOL oncethru = FALSE;

	int i = 0;

	for (i = 0; i < m_vertices.size(); i++)
	{
		if (!oncethru)
		{
			maxbox = m_vertices[0].vec;
			minbox = maxbox;

			oncethru = TRUE;
		}

		Point3 p = m_vertices[i].vec;

		// Box: calc max and min
		if (p.x > maxbox.x)
			maxbox.x = p.x;

		if (p.y > maxbox.y)
			maxbox.y = p.y;

		if (p.z > maxbox.z)
			maxbox.z = p.z;

		if (p.x < minbox.x)
			minbox.x = p.x;

		if (p.y < minbox.y)
			minbox.y = p.y;

		if (p.z < minbox.z)
			minbox.z = p.z;
	}

	Point3 c = (maxbox + minbox) / 2;
	Point3 s = maxbox - minbox;

	m_bbox.SetCenterSize(c, s);
}

void CAnmPrimitive::GetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	if (!m_bbox.IsValid())
		GenBoundingBox();

	assert(m_bbox.IsValid());

	*pBoundingBox = m_bbox;
}

bool CAnmPrimitive::TestPick(Point3 start, Point3 end, sAnmRenderGraphPickList *pPickList)
{
	// use our octree if it's there
	bool picked = false;

	if (m_octree)
	{
		// Fixed CSIRO Bug 466:1 - Seek wasn't returning true if object had octree and backfaces
		if (m_octree->TestPick(this, start, end, pPickList))
			picked = true;

		if (m_backfaces && m_backfaces->TestPick(start, end, pPickList))
			picked = true;

		return picked;
	}

	// try trivial reject on bounding box
	Point3 p;
	// if (!CAnmIntersection::RayIntersectBox(start, end, m_bbox.GetCenter(), m_bbox.GetSize(), &p))
	//	return FALSE;

	int i;
	int nTriangles = NumTriangles();

	// Do linear search on our list of triangles
	for(i = 0; i < nTriangles; i++)
	{
		tAnmTriangle triangle;
		GetTriangle(i, triangle);

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
	
	if (m_backfaces && m_backfaces->TestPick(start, end, pPickList))
		picked = true;

	return picked;
}

bool CAnmPrimitive::SphereCollide(Point3 center, float radius, sAnmRenderGraphPickList *pPickList)
{
	bool collide = false;

	// use our octree if it's there
	if (m_octree)
	{
		if (m_octree->SphereCollide(this, center, radius, pPickList))
			collide = true;

		if (m_backfaces && m_backfaces->SphereCollide(center, radius, pPickList))
			collide = true;

		return collide;
	}

	// try trivial reject on bounding box
	Point3 p;
	// if (!CAnmIntersection::RayIntersectBox(start, end, m_bbox.GetCenter(), m_bbox.GetSize(), &p))
	//	return FALSE;

	int i;
	int nTriangles = NumTriangles();

	// Do linear search on our list of triangles
	for(i = 0; i < nTriangles; i++)
	{
		tAnmTriangle triangle;
		GetTriangle(i, triangle);

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
	
	if (m_backfaces && m_backfaces->SphereCollide(center, radius, pPickList))
		collide = true;

	return collide;
}

bool CAnmPrimitive::CapsuleCollide(const Point3 &capsulePt0, const Point3 &capsulePt1, const Point3 &capsuleRadius,
		sAnmRenderGraphPickList *pPickList)
{
	bool collide = false;

	// use our octree if it's there
	if (m_octree)
	{
		if (m_octree->CapsuleCollide(this, capsulePt0, capsulePt1, capsuleRadius, pPickList))
			collide = true;

		if (m_backfaces && m_backfaces->CapsuleCollide(capsulePt0, capsulePt1, capsuleRadius, pPickList))
			collide = true;

		return collide;
	}

	// try trivial reject on bounding box
	Point3 p;
	// if (!CAnmIntersection::RayIntersectBox(start, end, m_bbox.GetCenter(), m_bbox.GetSize(), &p))
	//	return FALSE;

	int i;
	int nTriangles = NumTriangles();

	// Do linear search on our list of triangles
	for(i = 0; i < nTriangles; i++)
	{
		tAnmTriangle triangle;
		GetTriangle(i, triangle);
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
	
	if (m_backfaces && m_backfaces->CapsuleCollide(capsulePt0, capsulePt1, capsuleRadius, pPickList))
		collide = true;

	return collide;
}

// Triangle lists
eAnmReturnStatus CAnmTriangleList::GenerateNormals(Float creaseAngle)
{
	// The only thing we can do for this node is generate face normals
	int sz = m_vertices.size();
	for (int i = 0; i < sz; i+=3)
	{
		plane3 plane(
			m_vertices[i].vec, 
			m_vertices[i + 1].vec, 
			m_vertices[i + 2].vec);

		m_vertices[i+0].SetNormal(plane.n);
		m_vertices[i+1].SetNormal(plane.n);
		m_vertices[i+2].SetNormal(plane.n);
	}

	return eAnmReturnAllGood;
}

int CAnmTriangleList::NumTriangles()
{
	return m_vertices.size() / 3;
}

void CAnmTriangleList::GetTriangle(int index, tAnmTriangle &triangle)
{
	assert (index >= 0 && index < m_vertices.size());

	triangle.v[0] = m_vertices[index * 3].vec;
	triangle.v[1] = m_vertices[index * 3 + 1].vec;
	triangle.v[2] = m_vertices[index * 3 + 2].vec;
}

void CAnmTriangleList::GenerateBackfaces(class CAnmDevice *pDevice)
{
	m_backfaces = new CAnmTriangleList(pDevice);

	tAnmVertexArray *pVertices = m_backfaces->GetVertices();

	int vsz = m_vertices.size();
	pVertices->reserve(vsz);

	// copy the faces, but flip the triangle order and normal direction
	for (int i = 0; i < vsz / 3; i++)
	{
		pVertices->push_back(m_vertices[i * 3]);
		(*pVertices)[i * 3].normal = - (m_vertices[i * 3].normal);

		pVertices->push_back(m_vertices[i * 3 + 2]);
		(*pVertices)[i * 3 + 1].normal = - (m_vertices[i * 3 + 2].normal);

		pVertices->push_back(m_vertices[i * 3 + 1]);
		(*pVertices)[i * 3 + 2].normal = - (m_vertices[i * 3 + 1].normal);
	}
}

// Indexed triangle lists
// returns TRUE if the given normals are at less than creaseAngle to each other
static const Point3 badnormal(0, 0, 0);
static BOOL NoCrease(Point3 &n1, Point3 &n2, float creaseAngle)
{
	// if the first one is sentinel value, or they're equal, there's no crease
	if (n1 == badnormal)
		return TRUE;
	else if (n1 == n2)
		return TRUE;

	float dot = n1 * n2;
	float ang = acos(dot);
	if (ang < creaseAngle)
		return TRUE;
	else
		return FALSE;
}

eAnmReturnStatus CAnmIndexedTriangleList::GenerateNormals(Float creaseAngle)
{
	// a lot to do here
	int i;
	int nVertices = m_vertices.size();
	int nIndices = m_indices.size();
	sAnmVertex vert;
	Point3 badnormal(0, 0, 0);

	// First set all the normals to zero.
	for(i = 0; i < nVertices; i++)
	{
		m_vertices[i].normal.Assign( 0,0,0 );
	}

	// Then go through and add each triangle's normal to each of its vertices.
	// If the angle between normals of a shared vertices is greater than the
	// supplied crease angle, we need to create a new vertex with the new
	// normal, i.e. no smooth shading across that edge.
	for(i = 0; i < nIndices / 3; i++)
	{
		plane3 plane(
			m_vertices[ m_indices[i * 3] ].vec,
			m_vertices[ m_indices[i * 3 + 1] ].vec,
			m_vertices[ m_indices[i * 3 + 2] ].vec );

		if (plane.n.Mag() < FLT_EPSILON)
			continue;

		// N.B.: there's a cheap hack in here. Splitting an index might
		// overflow the max # of vertices (currently 2^^16)... so we have to stop
		// splitting indices if we're already at the max. A better way to do this
		// is to split up the whole face set into digestible chunks... but it ain't
		// going to happen any time soon. -- TP 090302
		if (NoCrease(m_vertices[ m_indices[i * 3 ] ].normal, plane.n, creaseAngle) ||
			!ValidIndex(nVertices))
		{
			m_vertices[ m_indices[i * 3 ] ].normal += plane.n;
			m_vertices[ m_indices[i * 3 ] ].normal.Normalize();
		}
		else
		{
			vert = m_vertices[ m_indices[i * 3 ] ];
			vert.normal = plane.n;
			m_vertices.push_back(vert);
			m_indices[i * 3 ] = nVertices++;
		}

		if (NoCrease(m_vertices[ m_indices[i * 3 + 1] ].normal, plane.n, creaseAngle) ||
			!ValidIndex(nVertices))
		{
			m_vertices[ m_indices[i * 3 + 1] ].normal += plane.n;
			m_vertices[ m_indices[i * 3 + 1] ].normal.Normalize();
		}
		else
		{
			vert = m_vertices[ m_indices[i * 3 + 1] ];
			vert.normal = plane.n;
			m_vertices.push_back(vert);
			m_indices[i * 3 + 1] = nVertices++;
		}

		if (NoCrease(m_vertices[ m_indices[i * 3 + 2] ].normal, plane.n, creaseAngle) ||
			!ValidIndex(nVertices))
		{
			m_vertices[ m_indices[i * 3 + 2] ].normal += plane.n;
			m_vertices[ m_indices[i * 3 + 2] ].normal.Normalize();
		}
		else
		{
			vert = m_vertices[ m_indices[i * 3 + 2] ];
			vert.normal = plane.n;
			m_vertices.push_back(vert);
			m_indices[i * 3 + 2] = nVertices++;
		}
	}

	return eAnmReturnAllGood;
}

int CAnmIndexedTriangleList::NumTriangles()
{
	return m_indices.size() / 3;
}

void CAnmIndexedTriangleList::GetTriangle(int index, tAnmTriangle &triangle)
{
	assert (index >= 0 && index < m_indices.size() / 3);

	triangle.v[0] = m_vertices[ m_indices[index * 3] ].vec;
	triangle.v[1] = m_vertices[ m_indices[index * 3 + 1] ].vec;
	triangle.v[2] = m_vertices[ m_indices[index * 3 + 2] ].vec;
}

void CAnmIndexedTriangleList::GenerateBackfaces(class CAnmDevice *pDevice)
{
	m_backfaces = new CAnmIndexedTriangleList(pDevice);

	tAnmVertexArray *pVertices = m_backfaces->GetVertices();
	tAnmIndexArray *pIndices = ((CAnmIndexedTriangleList *)m_backfaces)->GetIndices();

	int vsz = m_vertices.size();
	int isz = m_indices.size();
	pVertices->reserve(vsz);
	pIndices->reserve(isz);

	// copy the faces, but flip the normal direction
	for (int i = 0; i < vsz; i++)
	{
		pVertices->push_back(m_vertices[i]);
		(*pVertices)[i].normal = - (m_vertices[i].normal);
	}

	// copy the indices but flip the triangle order
	for (i = 0; i < isz / 3; i++)
	{
		pIndices->push_back(m_indices[i * 3]);
		pIndices->push_back(m_indices[i * 3 + 2]);
		pIndices->push_back(m_indices[i * 3 + 1]);
	}
}

// Triangle strips
eAnmReturnStatus CAnmTriangleStrip::GenerateNormals(Float creaseAngle)
{
	int sz = m_vertices.size();

	Point3 norm(0, 0, 0);
	for (int i = 0; i < sz; i++)
	{
		m_vertices[i].SetNormal(norm);
	}

	tAnmTriangle t;
	for (i = 0; i < sz - 2; i++)
	{
		GetTriangle(i, t);
		plane3 plane(t.v[0], t.v[1], t.v[2]);
		Point3 n = plane.n + m_vertices[i].normal;
		n.Normalize();
		m_vertices[i].SetNormal(n);
		m_vertices[i+1].SetNormal(n);
		m_vertices[i+2].SetNormal(n);
	}

	return eAnmReturnAllGood;
}

void CAnmTriangleStrip::GenerateBackfaces(class CAnmDevice *pDevice)
{
	m_backfaces = new CAnmTriangleStrip(pDevice);

	tAnmVertexArray *pVertices = m_backfaces->GetVertices();

	int vsz = m_vertices.size();
	pVertices->reserve(vsz);
/***********

	krv:  This logic is wrong!  There are two problems here.
	1) WHen you flip flop the i, i+1., you need to set the normal to the flopped vertex like this.
		(*pVertices)[i].normal = -(m_vertices[i + 1].normal);
		(*pVertices)[i + 1].normal = -(m_vertices[i ].normal);
	2) The triangles coming out are just not right!  You get long, skinny triangles, 
	     that are inconsistant witht he source tris.

	The Solution is to do a Turn Edge on the triangle Pairs.
	   


	// copy the strip, but flip the triangle order and normal direction
	if (vsz > 0)
	{
		pVertices->push_back(m_vertices[0]);
		(*pVertices)[0].normal = - (m_vertices[0].normal);
	}

	for (int i = 1; i < vsz - 1; i+=2)
	{
		pVertices->push_back(m_vertices[i + 1]);
		pVertices->push_back(m_vertices[i]);

		(*pVertices)[i].normal = -(m_vertices[i].normal);
		(*pVertices)[i + 1].normal = -(m_vertices[i + 1].normal);
	}

	if (i == vsz - 1)
	{
		pVertices->push_back(m_vertices[i]);
		(*pVertices)[i].normal = -(m_vertices[i].normal);
	}
****************/

	// copy the strip, but flip the triangle order and normal direction

	// DONT do the first vertex separate.
	//
	for (int i = 0; i < vsz - 1; i+=2)
	{
		pVertices->push_back(m_vertices[i + 1]);
		pVertices->push_back(m_vertices[i]);

		// krv:  Make sure you flop flip the normals of the vertices that you.
		//       
		(*pVertices)[i].normal = -(m_vertices[i + 1].normal);
		(*pVertices)[i + 1].normal = -(m_vertices[i ].normal);
	}

	if (i == vsz - 1)
	{
		pVertices->push_back(m_vertices[i]);
		(*pVertices)[i].normal = -(m_vertices[i].normal);
	}
}

int CAnmTriangleStrip::NumTriangles()
{
	return m_vertices.size() - 2;
}

void CAnmTriangleStrip::GetTriangle(int index, tAnmTriangle &triangle)
{
	assert (index >= 0 && index < m_vertices.size() - 2);

	triangle.v[0] = m_vertices[index].vec;

	if (index % 2)
	{
		// reverse winding order for every other triangle
		triangle.v[1] = m_vertices[index + 2].vec;
		triangle.v[2] = m_vertices[index + 1].vec;
	}
	else
	{
		triangle.v[1] = m_vertices[index + 1].vec;
		triangle.v[2] = m_vertices[index + 2].vec;
	}
}

eAnmReturnStatus CAnmIndexedTriangleStrip::GenerateNormals(Float creaseAngle)
{
	// a lot to do here
	// uh, yeah!

	


	return eAnmReturnAllGood;
}

void CAnmIndexedTriangleStrip::GenerateBackfaces(class CAnmDevice *pDevice)
{
	// N.B.: need to implement
}

int CAnmIndexedTriangleStrip::NumTriangles()
{
	return m_indices.size() - 2;
}

void CAnmIndexedTriangleStrip::GetTriangle(int index, tAnmTriangle &triangle)
{
	assert (index >= 0 && index < m_indices.size() - 2);

	triangle.v[0] = m_vertices[ m_indices[index] ].vec;
	if (index % 2)
	{
		// reverse winding order for every other triangle
		triangle.v[1] = m_vertices[ m_indices[index + 2] ].vec;
		triangle.v[2] = m_vertices[ m_indices[index + 1] ].vec;
	}
	else
	{
		triangle.v[1] = m_vertices[ m_indices[index + 1] ].vec;
		triangle.v[2] = m_vertices[ m_indices[index + 2] ].vec;
	}
}

// Triangle fans
eAnmReturnStatus CAnmTriangleFan::GenerateNormals(Float creaseAngle)
{
	int sz = m_vertices.size();

	point3 norm(0, 0, 0);

	for (int i = 0; i < sz - 2; i++)
	{
		plane3 plane(m_vertices[i].vec, m_vertices[i+1].vec, m_vertices[i+2].vec);
		norm += plane.n;
		norm.Normalize();
	}

	for (i = 0; i < sz; i++)
		m_vertices[i].SetNormal(norm);

	return eAnmReturnAllGood;
}

void CAnmTriangleFan::GenerateBackfaces(class CAnmDevice *pDevice)
{
	m_backfaces = new CAnmTriangleFan(pDevice);

	tAnmVertexArray *pVertices = m_backfaces->GetVertices();

	int vsz = m_vertices.size();
	pVertices->reserve(vsz);

	// copy the fan, but flip the order and normal direction
	if (vsz > 0)
	{
		pVertices->push_back(m_vertices[0]);
		(*pVertices)[0].normal = - (m_vertices[0].normal);
	}

	for (int i = vsz - 1; i >= 1; i--)
	{
		pVertices->push_back(m_vertices[i]);
		(*pVertices)[vsz - i].normal = - (m_vertices[i].normal);
	}
}

int CAnmTriangleFan::NumTriangles()
{
	return m_vertices.size() - 2;
}

void CAnmTriangleFan::GetTriangle(int index, tAnmTriangle &triangle)
{
	assert (index >= 0 && index < (m_vertices.size() - 2));

	triangle.v[0] = m_vertices[0].vec;
	triangle.v[1] = m_vertices[index + 1].vec;
	triangle.v[2] = m_vertices[index + 2].vec;
}

eAnmReturnStatus CAnmIndexedTriangleFan::GenerateNormals(Float creaseAngle)
{
	// a lot to do here
	return eAnmReturnAllGood;
}

void CAnmIndexedTriangleFan::GenerateBackfaces(class CAnmDevice *pDevice)
{
	// N.B.: need to implement
}


int CAnmIndexedTriangleFan::NumTriangles()
{
	return m_vertices.size() - 2;
}

void CAnmIndexedTriangleFan::GetTriangle(int index, tAnmTriangle &triangle)
{
	assert (index >= 0 && index < (m_indices.size() - 2));

	triangle.v[0] = m_vertices[ m_indices[0] ].vec;
	triangle.v[1] = m_vertices[ m_indices[index + 1] ].vec;
	triangle.v[2] = m_vertices[ m_indices[index + 2] ].vec;
}

// Unlit primitives (PolyLine and PointList)
void CAnmUnlitPrimitive::Render(CAnmDevice *pDevice)
{
	assert(pDevice);
	assert(m_renderprimitive);

	m_renderprimitive->Render(pDevice);

#ifdef DEBUG
	// N.B.: this was only for debugging the octrees
	if (m_octree && CAnmOctree::GetDrawBoxes())
		m_octree->Render(pDevice);
#endif
}

void CAnmUnlitPrimitive::AddUnlitVertices(int vertexcount, sAnmUnlitVertex *pVertices)
{
	int curcount = m_unlitVertices.size();

	m_unlitVertices.reserve(curcount + vertexcount);

	for (int i = 0; i < vertexcount; i++)
	{
		m_unlitVertices.push_back(pVertices[i]);
	}

}

void CAnmUnlitPrimitive::SetUnlitVertex(int vindex, sAnmUnlitVertex v)
{
	assert(vindex >= 0 && vindex < m_unlitVertices.size());

	m_unlitVertices[vindex] = v;
}

sAnmUnlitVertex CAnmUnlitPrimitive::GetUnlitVertex(int vindex)
{
	assert(vindex >= 0 && vindex < m_vertices.size());

	return m_unlitVertices[vindex];
}

void CAnmUnlitPrimitive::SetCoordinates(Point3Array *pCoords)
{
	assert(pCoords != NULL);

	int csize = pCoords->size();

	if (csize > m_unlitVertices.size())
		csize = m_unlitVertices.size();

	for (int i = 0; i < csize; i++)
	{
		m_unlitVertices[i].vec = (*pCoords) [i];
	}

	// N.B.: forget about octree sorting and morphs on the same object
	// can't do it per-frame. so if somebody changes our coordinates,
	// just bail on the sorting
	if (m_octree)
		SafeDelete(m_octree);

	InvalidateBoundingBox();

}

void CAnmUnlitPrimitive::SetDiffuseColor(ULONG diffuseColor)
{
	if (diffuseColor != m_diffuseColor)
	{
		for (int i = 0; i < m_unlitVertices.size(); i++)
		{
			m_unlitVertices[i].SetDiffuseColor(diffuseColor);
		}

		m_diffuseColor = diffuseColor;
	}
}

void CAnmUnlitPrimitive::SetDiffuseColor(color4 diffuseColor)
{
	SetDiffuseColor(diffuseColor.MakeDWord());
}

void CAnmUnlitPrimitive::Scale(Float scalefactor)
{
	for (int i = 0; i < m_unlitVertices.size(); i++)
	{
		m_unlitVertices[i].vec *= scalefactor;
	}
}

eAnmReturnStatus CAnmUnlitPrimitive::BufferVertices(CAnmDevice *pDevice)
{
	// N.B.: forget vertex buffers for point and line sets...
	return eAnmReturnAllGood;

}

eAnmReturnStatus CAnmUnlitPrimitive::UpdatePrimitive()
{
	// N.B.: forget vertex buffers for point and line sets...
	return eAnmReturnAllGood;
}


void CAnmUnlitPrimitive::GenBoundingBox()
{
	Point3 minbox(0, 0, 0);
	Point3 maxbox(0, 0, 0);
	Point3 center(0, 0, 0);
	BOOL oncethru = FALSE;

	int i = 0;

	for (i = 0; i < m_unlitVertices.size(); i++)
	{
		if (!oncethru)
		{
			maxbox = m_unlitVertices[0].vec;
			minbox = maxbox;

			oncethru = TRUE;
		}

		Point3 p = m_unlitVertices[i].vec;

		// Box: calc max and min
		if (p.x > maxbox.x)
			maxbox.x = p.x;

		if (p.y > maxbox.y)
			maxbox.y = p.y;

		if (p.z > maxbox.z)
			maxbox.z = p.z;

		if (p.x < minbox.x)
			minbox.x = p.x;

		if (p.y < minbox.y)
			minbox.y = p.y;

		if (p.z < minbox.z)
			minbox.z = p.z;
	}

	Point3 c = (maxbox + minbox) / 2;
	Point3 s = maxbox - minbox;

	m_bbox.SetCenterSize(c, s);
}


