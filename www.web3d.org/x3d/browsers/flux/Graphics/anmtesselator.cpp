/********************************************************************************
 * Flux
 *
 * File: anmtesselator.cpp
 * Description: Tesselator class - tesselates non-
 *							convex polygons using GLU tesselator
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
#include "anmtesselator.h"

#include <gl/glu.h>

CAnmTesselator::CAnmTesselator()
{
	m_GLUtesselator = gluNewTess();
	assert(m_GLUtesselator != NULL);

	m_coords = NULL;
	m_primtype = -1;
}

CAnmTesselator::~CAnmTesselator()
{
	if (m_GLUtesselator)
		gluDeleteTess(m_GLUtesselator);
}


inline Point3 VecFromCoords(double *coords, int index)
{
	return Point3((float) coords[index * 3],
				(float) coords[index * 3 + 1],
				(float) coords[index * 3 + 2]);
}

inline Point3 Normal(double *coords, int startIndex)
{
	Point3 vec[3];
	vec[0] = VecFromCoords(coords, startIndex);
	vec[1] = VecFromCoords(coords, startIndex + 1);
	vec[2] = VecFromCoords(coords, startIndex + 2);

	plane3 plane(vec[0], vec[1], vec[2]);

	return plane.n;
}

int  *CAnmTesselator::Tesselate(double *coords, int &numIndices)
{
	// Can't handle non-planar polys yet
#if 0
	if (!PlanarTest(coords, numIndices))
	{
		numIndices = 0;
		return NULL;
	}
#endif

	m_indices.clear();


	// do stuff
	gluTessCallback(m_GLUtesselator, GLU_TESS_BEGIN_DATA, (void (CALLBACK*)()) TessBeginCallback);
	gluTessCallback(m_GLUtesselator, GLU_TESS_VERTEX_DATA, (void (CALLBACK*)()) TessVertexCallback);
	gluTessCallback(m_GLUtesselator, GLU_TESS_COMBINE_DATA, (void (CALLBACK*)()) TessCombineCallback);
	gluTessCallback(m_GLUtesselator, GLU_TESS_ERROR_DATA, (void (CALLBACK*)()) TessErrorCallback);

	m_coords = coords;
	int i = 0;
	double *coord = NULL;

	assert(numIndices >= 3);
	Point3 normal = Normal(m_coords, 0);

	gluTessBeginPolygon(m_GLUtesselator, this);
	gluTessBeginContour(m_GLUtesselator);

	for (i = 0; i < numIndices; i++)
	{
		coord = &coords[i * 3];

		gluTessVertex(m_GLUtesselator, coord, coord);
	}
	gluTessEndContour(m_GLUtesselator);

	m_combineFlag = FALSE;
	m_errorFlag = FALSE;
	m_errnum = 0;

	// N.B.: wrap in a try/catch pair because GLU tesselator is crashing on some data... 
	// whew -- TP 09/02/02
	try {
		gluTessEndPolygon(m_GLUtesselator);
	}
	catch (...)
	{
		if (m_combineFlag)
		{
			m_indices.clear();
		}
		else
		{
			int foo = 1;
		}
	}

	// set the output for numIndices and copy the data... if there is any
	numIndices = m_indices.size();
	if (numIndices <= 0)
		return NULL;

	int *newIndices = new int[numIndices];
	for (i = 0; i < numIndices; i++)
		newIndices[i] = m_indices[i];

	return newIndices;
}

int  *CAnmTesselator::Tesselate(double *coords, int &numIndices, int *countours, int countourcount)
{
	m_indices.clear();

	// do stuff
	gluTessCallback(m_GLUtesselator, GLU_TESS_BEGIN_DATA, (void (CALLBACK*)()) TessBeginCallback);
	gluTessCallback(m_GLUtesselator, GLU_TESS_VERTEX_DATA, (void (CALLBACK*)()) TessVertexCallback);
	gluTessCallback(m_GLUtesselator, GLU_TESS_COMBINE_DATA, (void (CALLBACK*)()) TessCombineCallback);
	gluTessCallback(m_GLUtesselator, GLU_TESS_ERROR_DATA, (void (CALLBACK*)()) TessErrorCallback);

	m_coords = coords;
	int i = 0, j = 0, k = 0;
	double *coord = NULL;

	assert(numIndices >= 3);
	Point3 normal = Normal(m_coords, 0);

	gluTessBeginPolygon(m_GLUtesselator, this);
	for (i = 0, k = 0; i < countourcount; i++)
	{
		gluTessBeginContour(m_GLUtesselator);
		for (j = 0; j < countours[i]; j++, k++)
		{
			coord = &coords[k * 3];

			gluTessVertex(m_GLUtesselator, coord, coord);
		}
		gluTessEndContour(m_GLUtesselator);
	}

	m_combineFlag = FALSE;
	m_errorFlag = FALSE;
	m_errnum = 0;

	// N.B.: wrap in a try/catch pair because GLU tesselator is crashing on some data... 
	// whew -- TP 09/02/02
	try {
		gluTessEndPolygon(m_GLUtesselator);
	}
	catch (...)
	{
		if (m_combineFlag)
		{
			m_indices.clear();
		}
		else
		{
			int foo = 1;
		}
	}

	// set the output for numIndices and copy the data... if there is any
	numIndices = m_indices.size();
	if (numIndices <= 0)
		return NULL;

	int *newIndices = new int[numIndices];
	for (i = 0; i < numIndices; i++)
		newIndices[i] = m_indices[i];

	return newIndices;
}

BOOL CAnmTesselator::PlanarTest(double *coords, int &numIndices)
{
	assert(numIndices >= 3);

	Point3 normal;
	BOOL retval = TRUE;
	for (int i = 0; i < numIndices - 2; i++)
	{
		Point3 vec[3];
		vec[0] = VecFromCoords(coords, i);
		vec[1] = VecFromCoords(coords, i + 1);
		vec[2] = VecFromCoords(coords, i + 2);

		plane3 plane(vec[0], vec[1], vec[2]);
		if (i == 0)
			normal = plane.n;
		else if (!(plane.n == normal))
		{
			retval = FALSE;
			break;
		}
	}

	return retval;
}

void CAnmTesselator::TessBeginCallback(int type, void *polygon_data)
{
	CAnmTesselator *pTesselator = (CAnmTesselator *) polygon_data;
	pTesselator->BeginPrimitive(type);
}

void CAnmTesselator::TessVertexCallback(void *vertex_data, void *polygon_data)
{
	CAnmTesselator *pTesselator = (CAnmTesselator *) polygon_data;
	pTesselator->AddIndex((double *) vertex_data);
}

void CAnmTesselator::TessCombineCallback(double coords[3], void *vertex_data[4],
	float weight[4], void **outData, void *polygon_data)
{
	CAnmTesselator *pTesselator = (CAnmTesselator *) polygon_data;
	pTesselator->Combine(coords, (double **) vertex_data, weight, (double **) outData);
}

void CAnmTesselator::TessErrorCallback(int errnum, void *polygon_data)
{
	CAnmTesselator *pTesselator = (CAnmTesselator *) polygon_data;
	pTesselator->Error(errnum);
}

void CAnmTesselator::BeginPrimitive(int type)
{
	m_primtype = type;
	m_firstprimindex = m_indices.size();
	m_previous[0] = m_previous[1] = -1;
}

void CAnmTesselator::AddIndex(double *coord)
{
	GLenum primtype = (GLenum) m_primtype;

	int index = (coord - m_coords) / 3;
	int i0, i1, i2;
	// total for all primitives
	int totalindices = m_indices.size();
	// total for this primitive
	int numindices = totalindices - m_firstprimindex;

	switch (primtype)
	{
		case GL_TRIANGLES :
			m_indices.push_back(index);
			break;

		case GL_TRIANGLE_FAN :
			// first 3 vertices of fan
			if (numindices < 3)
				m_indices.push_back(index);
			else
			{
				// subsequent vertices; each one makes a new triangle
				// with original vertex and previously added vertex
				//		i0 = prim's first vertex, 
				//		i1 = previously added vertex 
				//		i2 = this vertex
				i0 = m_indices[m_firstprimindex];
				i1 = m_indices[totalindices - 1];
				i2 = index;
				m_indices.push_back(i0);
				m_indices.push_back(i1);
				m_indices.push_back(i2);
			}

			break;

		case GL_TRIANGLE_STRIP :
			// first 3 vertices of strip
			if (numindices < 3)
				m_indices.push_back(index);
			else
			{
				// for sanity
				assert(m_previous[0] != -1);
				assert(m_previous[1] != -1);

				// subsequent vertices; each one makes a new triangle
				// from 2 previously added vertices
				if (numindices % 2)
				{
					//		i0 = 2nd most previously added vertex
					//		i1 = this vertex
					//		i2 = most previously added vertex
					i0 = m_previous[0];
					i1 = index;
					i2 = m_previous[1];
				}
				else
				{
					//		i0 = 2nd most previously added vertex
					//		i1 = most previously added vertex
					//		i2 = this vertex
					i0 = m_previous[0];
					i1 = m_previous[1];
					i2 = index;
				}

				// toss in the triangle
				m_indices.push_back(i0);
				m_indices.push_back(i1);
				m_indices.push_back(i2);
			}

			// save away the two most recent indices
			m_previous[0] = m_previous[1]; // this'll be bogus 1st time around, but it's ok
			m_previous[1] = index;

			break;
		
		default :
			assert(FALSE);
			break;
	}
}

void CAnmTesselator::Combine(double coords[3], double *vertex_data[4],
	float weight[4], double **outData)
{
	// don't interpolate; we're not set up to add vertices. just
	// return the first vertex.
	// this shouldn't happen too often anyway, he said, fingers crossed...

	*outData = vertex_data[0];

	// flag in case all hell breaks loose
	m_combineFlag = TRUE;
}

void CAnmTesselator::Error(int errnum)
{
	// N.B.: should issue some kind of warning here
	int foo = 1;

	char *cp = (char *) gluErrorString(errnum);
}
