/********************************************************************************
 * Flux
 *
 * File: anmvolumes.h
 * Description: Bounding volume support
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

#ifndef _anmvolumes_h
#define _anmvolumes_h

#define ORIENTEDBOX 1

#if ORIENTEDBOX
#define ANMBBOX_INVALID_VALUE Point3(FLT_MAX, FLT_MAX, FLT_MAX)

class CAnmBoundingBox {

protected :


	Point3		m_origin;
	Point3		m_xMax;
	Point3		m_yMax;
	Point3		m_zMax;

public :

	// Constructors
	CAnmBoundingBox()
	{
		m_origin = m_xMax = m_yMax = m_zMax = 
			ANMBBOX_INVALID_VALUE;
	}

	CAnmBoundingBox(Point3 center, Point3 size)
	{
		SetCenterSize(center, size);
	}

	// Methods
	void SetMinMax(Point3 min, Point3 max)
	{
		SetCenterSize((max + min) / 2, max - min);
	}

	void SetCenterSize(Point3 center, Point3 size)
	{
		m_origin = center - (size / 2);
		m_xMax = m_origin + Point3(size.x, 0, 0);
		m_yMax = m_origin + Point3(0, size.y, 0);
		m_zMax = m_origin + Point3(0, 0, size.z);
	}

	void GetMinMax(Point3 *pMin, Point3 *pMax)
	{
		Point3 min = Point3(FLT_MAX, FLT_MAX, FLT_MAX);
		Point3 max = Point3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		Point3 corners[8];
		GetCorners(corners);

		for (int i = 0; i < 8; i++)
		{
			if (corners[i].x > max.x)
				max.x = corners[i].x;
			if (corners[i].x < min.x)
				min.x = corners[i].x;

			if (corners[i].y > max.y)
				max.y = corners[i].y;
			if (corners[i].y < min.y)
				min.y = corners[i].y;

			if (corners[i].z > max.z)
				max.z = corners[i].z;
			if (corners[i].z < min.z)
				min.z = corners[i].z;
		}

		*pMin = min;
		*pMax = max;
	}

	void GetCenterSize(Point3 *pCenter, Point3 *pSize)
	{
		Point3 min, max;

		GetMinMax(&min, &max);

		*pCenter = (min + max) / 2;
		*pSize = max - min;
	}

	float GetRadius()
	{
		Point3 center, size;

		GetCenterSize(&center, &size);

		return (size / 2).Mag();
	}

	void GetCorners(Point3 corners[8])
	{
		Point3 xAxis = m_xMax - m_origin;
		Point3 yAxis = m_yMax - m_origin;
		Point3 zAxis = m_zMax - m_origin;

		corners[0] = m_origin;

		corners[1] = m_origin + yAxis;

		corners[2] = m_origin + xAxis + yAxis;

		corners[3] = m_origin + xAxis;

		corners[4] = m_origin + zAxis;

		corners[5] = m_origin + yAxis + zAxis;

		corners[6] = m_origin + xAxis + yAxis + zAxis;

		corners[7] = m_origin + xAxis + zAxis;
	}

	void GetOriginAxes(Point3 *pOrigin, Point3 axes[3])
	{
		Point3 xAxis = m_xMax - m_origin;
		Point3 yAxis = m_yMax - m_origin;
		Point3 zAxis = m_zMax - m_origin;

		*pOrigin = m_origin;
		axes[0] = xAxis;
		axes[1] = yAxis;
		axes[2] = zAxis;
	}

	CAnmBoundingBox Transform(matrix4 &mat)
	{
		CAnmBoundingBox b;

		if (!IsValid())
			return b;

		b.m_origin = mat * m_origin;
		b.m_xMax = mat * m_xMax;
		b.m_yMax = mat * m_yMax;
		b.m_zMax = mat * m_zMax;

		return b;
	}

	void Merge(CAnmBoundingBox &b)
	{
		if (!b.IsValid())
			return;

		Point3 min, max, bMin, bMax;

		// N.B. just chuck orientation; simply axis-aligned
		// eventually maybe figure out minimum volume box and axes
		GetMinMax(&min, &max);
		b.GetMinMax(&bMin, &bMax);

		if (bMin.x < min.x)
			min.x = bMin.x;
		if (bMin.y < min.y)
			min.y = bMin.y;
		if (bMin.z < min.z)
			min.z = bMin.z;
		if (bMax.x > max.x)
			max.x = bMax.x;
		if (bMax.y > max.y)
			max.y = bMax.y;
		if (bMax.z > max.z)
			max.z = bMax.z;

		SetMinMax(min, max);
	}

	void Invalidate()
	{
		m_origin = ANMBBOX_INVALID_VALUE;
	}

	BOOL IsValid()
	{
		return !(m_origin == ANMBBOX_INVALID_VALUE);
	}

};


#else // ORIENTEDBOX

#define ANMBBOX_INVALID_MIN Point3(1, 1, 1)
#define ANMBBOX_INVALID_MAX Point3(-1, -1, -1)

class CAnmBoundingBox {

protected :


	Point3		m_min;
	Point3		m_max;

public :

	// Constructors
	CAnmBoundingBox()
	{
		m_min = ANMBBOX_INVALID_MIN;
		m_max = ANMBBOX_INVALID_MAX;
	}

	CAnmBoundingBox(Point3 center, Point3 size)
	{
		SetCenterSize(center, size);
	}

	// Methods
	void SetMinMax(Point3 min, Point3 max)
	{
		m_min = min;
		m_max = max;
	}

	Point3 GetMin()
	{
		return m_min;
	}

	Point3 GetMax()
	{
		return m_max;
	}

	void SetCenterSize(Point3 center, Point3 size)
	{
		m_min = center - (size / 2);
		m_max = center + (size / 2);
	}

	Point3 GetCenter()
	{
		return (m_max + m_min) / 2;
	}

	Point3 GetSize()
	{
		return m_max - m_min;
	}

	float GetRadius()
	{
		return m_max.Mag();
	}

	void GetCorners(Point3 corners[8])
	{
		corners[0] = m_min;

		corners[1] = m_min;
		corners[1].y = m_max.y;

		corners[2] = m_max;
		corners[2].z = m_min.z;

		corners[3] = m_min;
		corners[3].x = m_max.x;

		corners[4] = m_min;
		corners[4].z = m_max.z;

		corners[5] = m_max;
		corners[5].x = m_min.x;

		corners[6] = m_max;

		corners[7] = m_max;
		corners[7].y = m_min.y;
	}

	BOOL IsValid()
	{
		return (m_max.x >= m_min.x && m_max.y >= m_min.y && m_max.z >= m_min.z);
	}

};

#endif // ORIENTEDBOX


////////////////////////////////////////////////////////////////////////////////
// CAnmBoundingSphere
////////////////////////////////////////////////////////////////////////////////


class CAnmBoundingSphere {

protected:
	Point3		m_center;
	float		m_radius;

public:

	///// Constructors /////
	CAnmBoundingSphere()								{	m_center.x = FLT_MAX;
															m_center.y = FLT_MAX;
															m_center.z = FLT_MAX;
															m_radius   = FLT_MAX;
														}
	CAnmBoundingSphere(const Point3 &center, float radius)	{
															m_center = center;
															m_radius = radius;
														}
	
	///// Accessors ///
	void Set(const Point3 &center, float radius)		{	m_center = center; m_radius = radius; }
	void SetCenter(const Point3 &center)				{	m_center = center;		}
	void SetRadius(float radius)						{	m_radius = radius;		}
	void Get(Point3 *center, float *radius)				{	*center = m_center; *radius = m_radius;	}
	const Point3& GetCenter(void) const					{	return(m_center);	}
	float GetRadius(void) const							{	return(m_radius);	}

	///// Methods /////
	void Transform(const matrix4 &mat);					// Transformation by a matrix
	
	
	///// Operators /////
	void operator*=(const matrix4 &mat);				// Transformation by a matrix
	void operator+=(const CAnmBoundingSphere &sph);		// Union
	CAnmBoundingSphere operator+(const CAnmBoundingSphere &sph) const;
};

	
///// Functions /////
CAnmBoundingSphere operator*(const matrix4 &mat, CAnmBoundingSphere &sph);
CAnmBoundingSphere operator*(CAnmBoundingSphere &sph, const matrix4 &mat);



#endif // _anmvolumes_h
