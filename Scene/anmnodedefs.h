/********************************************************************************
 * Flux
 *
 * File: anmnodedefs.h
 * Description: Basic data type and other definitions
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

#ifndef _anmnodedefs_h
#define _anmnodedefs_h

#include "..\animadefs.h"
#include "anmobject.h"

template <class _T_> class CAnmArray : public CAnmRefObject
{
typedef void (*MapFn) (_T_, void *); 

protected :

	std::vector<_T_>		 *m_items;

public :

	CAnmArray(int reserve = 0) {
		m_items = new std::vector<_T_>;
		if (reserve >= 0)
			m_items->reserve(reserve);
	};

	~CAnmArray()
	{
		delete m_items;
	}

	// utilities
	void Map(MapFn fn, void *fnData)
	{
		for (int i = 0; i < size(); i++)
		{
			(*fn)((*m_items)[i], fnData);
		}
	}

	CAnmArray<_T_> *Clone()
	{
		CAnmArray<_T_> *pNew = new CAnmArray<_T_>;
		for (int i = 0; i < size(); i++)
		{
			pNew->push_back((*m_items)[i]);
		}

		return pNew;
	}

	// pass-throughs for std::vector
	void clear() { m_items->clear(); }
	void reserve(size_t _N) { m_items->reserve(_N); }

	void begin() { (void) m_items->begin(); }
	void push_back(const _T_ &_X) { m_items->push_back(_X); }

	size_t size() { return m_items->size(); }
	size_t capacity() const { return m_items->capacity(); }
	void resize(size_t _N) { m_items->resize(_N); }

	_T_& operator[](unsigned int _N) const { return (*m_items)[_N]; }

};

typedef std::list<class CAnmNode *> tAnmNodeList;

// Basic field type definitions

typedef	bool							Boolean;
typedef CAnmArray<Boolean>				BooleanArray;

typedef color3							Color;
typedef CAnmArray<Color>				ColorArray;

typedef color4							Color4;
typedef CAnmArray<Color4>				Color4Array;

typedef double							Double;
typedef CAnmArray<Double>				DoubleArray;

typedef float							Float;
typedef CAnmArray<Float>				FloatArray;

typedef int								Integer;
typedef CAnmArray<Integer>				IntegerArray;

typedef class sAnmImage : public CAnmRefObject
{
public :

	int	width;
	int height;
	int numComponents;
	IntegerArray *pixels;

	sAnmImage()
	{
		width = 0;
		height = 0;
		numComponents = 0;
		pixels = NULL;
	}

	sAnmImage(int w, int h, int nC, IntegerArray *pPixels)
	{
		width = w;
		height = h;
		numComponents = nC;
		pixels = NULL;

		SetPixels(pPixels);
	}

	void SetPixels(IntegerArray *pPixels)
	{
		assert(pPixels);

		SafeUnRef(pixels);

		pixels = pPixels;
	}

	sAnmImage *Clone()
	{
		return new sAnmImage(width, height, numComponents, pixels->Clone());
	}
}										Image;
typedef CAnmArray<Image>				ImageArray;

typedef class CAnmNode *				Node;
typedef CAnmArray<Node>					NodeArray;

typedef struct sAnmRotation {

	union {
		struct {
		point3	vec;
		float	theta;
		};
		float	f[4];
	};

	sAnmRotation()
	{
		vec.x = 0.f;
		vec.y = 1.f;
		vec.z = 0.f;
		theta = 0.f;
	}
	sAnmRotation(float x, float y, float z, float t)
	{
		vec.x = x;
		vec.y = y;
		vec.z = z;
		theta = t;
	}
}										Rotation;
typedef CAnmArray<Rotation>				RotationArray;

typedef class CAnmString *				String;
typedef CAnmArray<String>				StringArray;

typedef double							Time;
typedef CAnmArray<Time>					TimeArray;

typedef struct sAnmPoint2 {
	float x; float y;

	sAnmPoint2()
	{
		x = 0.f;
		y = 0.f;
	}


	sAnmPoint2(float px, float py)
	{
		x = px;
		y = py;
	}

	float Mag() const 
	{
		return sqrt( x*x + y*y );
	}

	sAnmPoint2& operator += ( const sAnmPoint2& in );
	sAnmPoint2& operator -= ( const sAnmPoint2& in );
	sAnmPoint2& operator *= ( const float& in );
	sAnmPoint2& operator /= ( const float& in );
	sAnmPoint2  operator -  (); // this is plain old negation
	sAnmPoint2& operator = ( const float& in );




}										Point2;

//==========--------------------------  point2 Operators

/**
 * Accumulative addition of two vectors
 */
inline Point2& Point2::operator += ( const Point2& in )
{
	x += in.x;   
	y += in.y;   
	return *this; // This is so we can chain operations together
}


/**
 * Accumulative subtraction of two vectors
 */
inline Point2& Point2::operator -= ( const Point2& in )
{
	x -= in.x;   
	y -= in.y;   
	return *this; // This is so we can chain operations together
}


/**
 * Accumulative multiplication of a vector by a scalar
 */
inline Point2& Point2::operator *= ( const float& in )
{
	x *= in;   
	y *= in;   
	return *this; // This is so we can chain operations together
}


/**
 * Accumulative division of a vector by a scalar
 */
inline Point2& Point2::operator /= ( const float& in )
{
	float inv = 1.f / in; // do one (expensive) divide and 3 (cheap) mults
	x *= inv;   
	y *= inv;   
	return *this; // This is so we can chain operations together 
}

/**
 * set equal to a scalr value
 */
inline Point2& Point2::operator = ( const float& in )
{
	x = in;   
	y = in;   
	return *this; // This is so we can chain operations together 
}


inline Point2 Point2::operator - () 
{
	return Point2(
		-x,
		-y
	);
}






/**
 * Subtracts to points : ret = a - b
 */
inline const Point2 operator-(Point2 const &a, Point2 const &b)
{
	return Point2
	(
		a.x-b.x,
		a.y-b.y
	);
}; 







/**
 * Vector Equality, epsilon used due to numerical imprecision
 */
inline bool   operator==(Point2 const &a, Point2 const &b)
{
	if(fabs(a.x-b.x)<EPSILON)
	{
		if(fabs(a.y-b.y)<EPSILON)
		{
			return true;
		}
	}
	return false;
};



/**
 * Scales a vector by a float : ret = a * b
 */
inline const Point2 operator*(float  const &a, Point2 const &b)
{
	return Point2
	(
		a*b.x,
		a*b.y
	);
}; 



/**
 * Adds two points together: ret = a + b
 */
inline const Point2 operator+(Point2 const &a, Point2 const &b)
{
	return Point2
	(
		a.x+b.x,
		a.y+b.y
	);
}; 
 

/**
 * Scales a vector by a float : ret = a * b
 */
inline const Point2 operator*(Point2 const &a, float const &b)
{
	return Point2
	(
		a.x*b,
		a.y*b
	);
}; 




/**
 * Divides a vector by a float : ret = a / b
 */
inline const Point2 operator/(Point2 const &a, float const &b)
{
	float inv = 1.f / b;
	return Point2
	(
		a.x*inv,
		a.y*inv
	);
}; 


/**
 * Performs a vector cross product : ret = a cross b
 */
inline const float operator^(Point2 const &a, Point2 const &b)
{
	return (float)((double)a.x * (double)b.y - (double)a.y * (double)b.x);
} 


/**
 * Performs a vector dot product : ret = a dot b
 */
inline const float operator*(Point2 const &a, Point2 const &b)
{
	return((float)(
			(double)a.x * (double)b.x
		+	(double)a.y * (double)b.y)
	);
} 


































typedef CAnmArray<Point2>				Point2Array;

typedef point3							Point3;
typedef point3D							Point3D;
typedef CAnmArray<Point3>				Point3Array;
typedef CAnmArray<Point3D>				Point3DArray;

// Implementation node support
#define DECLARE_GETIMPLEMENTATION(_cls_) \
inline _cls_ *GETIMPLEMENTATION##_cls_(CAnmNode *pNode) \
{ \
	if (pNode == NULL) \
		return NULL; \
	CAnmNode *pImp = pNode->GetImplementationNode(); \
	if (pImp && pImp->GetClass()->IsA(GETCLASS(_cls_))) \
		return (_cls_ *) pImp; \
	else \
		return NULL; \
}

#define GETIMPLEMENTATION(_cls_, _nd_) (GETIMPLEMENTATION##_cls_(_nd_))


struct sAnmAlphaShape {
	matrix4 mat;
	class CAnmShape *pShape;

	sAnmAlphaShape()
	{
	}

	sAnmAlphaShape(matrix4 m, class CAnmShape *ps)
	{
		mat = m;
		pShape = ps;
	}
};

typedef std::vector<sAnmAlphaShape> tAnmAlphaShapeArray;

#endif // _anmnodedefs_h
