/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: point3D.h
 *    Desc: declaration of a generic 3D vector class, with 
 *          operators aplenty
 * copyright (c) 1999 by Adrian Perez
 * See license.txt for modification and distribution information
 ******************************************************************/

#ifndef _POINT3D_H
#define _POINT3D_H

#include <math.h>
#include "mathGlobal.h"

#include "point3.h"


typedef double valType;

// struct point4;

struct EXPORT point3D 
{

	union 
	{
		// This structure here is nameless, which really isn't 
		// legal C++... most compilers will deal with it,
		// however.
		struct 
		{
            valType x,y,z; // 3 real components of the vector
        };
        valType	v[3]; // Array access useful in loops
    };

	// Default constructor, defined for template classes
	point3D()
	{
	}


	point3D( const point3& pntf )
	{
		x = pntf.x;
		y = pntf.y;
		z = pntf.z;
	}


	// Construct a point with 3 given inputs
	point3D( valType X, valType Y, valType Z ) : 
		x(X), y(Y), z(Z)
	{
	}

// 	point3D( const point4& in );

	// Construct a point using spherical coordinates
	static const point3D Spherical( valType theta, valType rho, valType phi )
	{
		return point3D(
			(valType)(phi*cos(rho)*sin(theta)), 
			(valType)(phi*sin(rho)), 
			(valType)(phi*cos(rho)*cos(theta))
		);
	}

	// Reassign a point without making a temporary structure
	void Assign( valType X, valType Y, valType Z );

	// Return the magnitude of a point
	valType Mag() const;

	// The length of the vector squared (to avoid the sqrt() )
	valType MagSquared() const;

	// Normalizes a vector (makes it's length one)
	void Normalize();

	// returns a Normalized version of this
	const point3D Normalized() const;

	// Computes the distance between two vectors
	static valType Dist( const point3D &a, const point3D &b )
	{
		// can't use the subtraction operator (it's not defined yet!)
		point3D distVec( b.x - a.x, b.y - a.y, b.z - a.z );
		return distVec.Mag();
	}

	point3D& operator += ( const point3D& in );
	point3D& operator -= ( const point3D& in );
	point3D& operator *= ( const valType& in );
	point3D& operator /= ( const valType& in );
	point3D  operator -  (); // this is plain old negation

	/**
	 * Constant vectors
	 */
	static const point3D Zero; // <0,0,0>
	static const point3D i; // <1,0,0>
	static const point3D j; // <0,1,0>
	static const point3D k; // <0,0,1>

}; 

//==========--------------------------  point3D Operators

/**
 * Accumulative addition of two vectors
 */
inline point3D& point3D::operator += ( const point3D& in )
{
	x += in.x;   
	y += in.y;   
	z += in.z;
	return *this; // This is so we can chain operations together
}


/**
 * Accumulative subtraction of two vectors
 */
inline point3D& point3D::operator -= ( const point3D& in )
{
	x -= in.x;   
	y -= in.y;   
	z -= in.z;
	return *this; // This is so we can chain operations together
}


/**
 * Accumulative multiplication of a vector by a scalar
 */
inline point3D& point3D::operator *= ( const valType& in )
{
	x *= in;   
	y *= in;   
	z *= in;
	return *this; // This is so we can chain operations together
}


/**
 * Accumulative division of a vector by a scalar
 */
inline point3D& point3D::operator /= ( const valType& in )
{
	valType inv = 1. / in; // do one (expensive) divide and 3 (cheap) mults
	x *= inv;   
	y *= inv;   
	z *= inv;
	return *this; // This is so we can chain operations together 
}

inline point3D point3D::operator - () 
{
	return point3D(
		-x,
		-y,
		-z
	);
}



/**
 * Adds two points together: ret = a + b
 */
inline const point3D operator+(point3D const &a, point3D const &b)
{
	return point3D
	(
		a.x+b.x,
		a.y+b.y,
		a.z+b.z
	);
}; 
 

/**
 * Subtracts to points : ret = a - b
 */
inline const point3D operator-(point3D const &a, point3D const &b)
{
	return point3D
	(
		a.x-b.x,
		a.y-b.y,
		a.z-b.z
	);
}; 


/**
 * Scales a vector by a valType : ret = a * b
 */
inline const point3D operator*(point3D const &a, valType const &b)
{
	return point3D
	(
		a.x*b,
		a.y*b,
		a.z*b
	);
}; 


/**
 * Scales a vector by a valType : ret = a * b
 */
inline const point3D operator*(valType  const &a, point3D const &b)
{
	return point3D
	(
		a*b.x,
		a*b.y,
		a*b.z
	);
}; 


/**
 * Divides a vector by a valType : ret = a / b
 */
inline const point3D operator/(point3D const &a, valType const &b)
{
	valType inv = 1. / b;
	return point3D
	(
		a.x*inv,
		a.y*inv,
		a.z*inv
	);
}; 


/**
 * Performs a vector cross product : ret = a cross b
 */
inline const point3D operator^(point3D const &a, point3D const &b)
{
	return point3D
	(
		(valType)((double)a.y * (double)b.z - (double)a.z * (double)b.y),
		(valType)((double)a.z * (double)b.x - (double)a.x * (double)b.z),
		(valType)((double)a.x * (double)b.y - (double)a.y * (double)b.x)
	);
} 


/**
 * Performs a vector dot product : ret = a dot b
 */
inline const valType operator*(point3D const &a, point3D const &b)
{
	return((valType)(
			(double)a.x * (double)b.x
		+	(double)a.y * (double)b.y
		+	(double)a.z * (double)b.z)
	);
} 


/**
 * Vector Equality, epsilon used due to numerical imprecision
 */
inline bool   operator==(point3D const &a, point3D const &b)
{
	if(fabs(a.x-b.x)<EPSILON)
	{
		if(fabs(a.y-b.y)<EPSILON)
		{
			if(fabs(a.z-b.z)<EPSILON)
			{
				return true;
			}
		}
	}
	return false;
};

//==========--------------------------  Inline functions
inline void point3D::Assign( valType X, valType Y, valType Z )
{
	x=X;	
	y=Y;	
	z=Z;
}

inline valType point3D::Mag() const
{
	return (valType)sqrt( (*this) * (*this) );
}

inline valType point3D::MagSquared() const
{ 
	return ( (*this) * (*this) );
}

inline void point3D::Normalize()
{
	valType invMag = 1./Mag();	

	(*this) *= invMag;
}

inline const point3D point3D::Normalized() const
{
	valType invMag = 1./Mag();	
	return (*this) * invMag;
}

#endif /*_POINT3D_H*/