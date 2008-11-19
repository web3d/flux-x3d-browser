/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: matrix4D.h
 *    Desc: Declaration for a 4x4 homogenous matrix class
 *          
 * copyright (c) 1999 by Adrian Perez
 * See license.txt for modification and distribution information
 ******************************************************************/

#ifndef _MATRIX4D_H
#define _MATRIX4D_H




#include "point3D.h"
#include "matrix4.h"
// #include "point4.h"
// #include "lineSeg3.h"
// #include "plane3.h"

typedef double valType;


struct EXPORT matrix4D 
{

	/**
	 * we're using m[y][x] as our notation.
	 */
    union 
	{
        struct 
		{
            valType	_11, _12, _13, _14;
            valType	_21, _22, _23, _24;
            valType	_31, _32, _33, _34;
            valType	_41, _42, _43, _44;
        };
        valType	m[4][4];
    };

	/**
	 * justification for a function this ugly:
	 * provides an easy way to initialize static matrix variables 
	 * like base matrices for bezier curves and the identity
	 */
	matrix4D(
		valType in11, valType in12, valType in13, valType in14,
        valType in21, valType in22, valType in23, valType in24,
        valType in31, valType in32, valType in33, valType in34,
        valType in41, valType in42, valType in43, valType in44 )
	{
        _11 = in11; _12 = in12; _13 = in13; _14 = in14;
        _21 = in21; _22 = in22; _23 = in23; _24 = in24;
        _31 = in31; _32 = in32; _33 = in33; _34 = in34;
        _41 = in41; _42 = in42; _43 = in43; _44 = in44;
	}

	matrix4D()
	{
		// Do nothing.  
	}

	matrix4D( const matrix4& matf )
	{
		_11 = matf._11;
		_12 = matf._12;
		_13 = matf._13;
		_14 = matf._14;

		_21 = matf._21;
		_22 = matf._22;
		_23 = matf._23;
		_24 = matf._24;

		_31 = matf._31;
		_32 = matf._32;
		_33 = matf._33;
		_34 = matf._34;
		
		_41 = matf._41;
		_42 = matf._42;
		_43 = matf._43;
		_44 = matf._44;
	}



	void ToXRot( valType theta );
	static matrix4D XRot( valType theta );


	void ToYRot( valType theta );
	static matrix4D YRot( valType theta );


	void ToZRot( valType theta );
	static matrix4D ZRot( valType theta );


	void ToAxisAngle( const point3D& inAxis, valType angle );
	static matrix4D AxisAngle( const point3D& inAxis, valType angle );


	/**
	 * Do not confuse with Translate()
	 * Translation clears the matrix, making it a Translation matrix
	 */
	void ToTranslation( const point3D& p );
	static matrix4D Translation( const point3D& p );

	
	/**
	 * Note that scale is not a rigid-body transform
	 */
	void ToScale( const point3D& scale );
	static matrix4D Scale( const point3D& scale );

	
	void ToObjectLookAt( 
		const point3D& loc, 
		const point3D& lookAt, 
		const point3D& inUp = point3D(0,1,0) );
	static matrix4D ObjectLookAt( 
		const point3D& loc, 
		const point3D& lookAt, 
		const point3D& inUp = point3D(0,1,0) );

	
	void ToCameraLookAt( 
		const point3D& loc, 
		const point3D& lookAt, 
		const point3D& inUp = point3D(0,1,0) );
	static matrix4D CameraLookAt( 
		const point3D& loc, 
		const point3D& lookAt, 
		const point3D& inUp = point3D(0,1,0) );

	/**
	 * Computes the inverse of a rigid-body transform
	 */
	void ToInverse( const matrix4D& in );
	static matrix4D Inverse( const matrix4D& in );

	/**
	 * Make the matrix the identity matrix
	 */
	void MakeIdent();

	/**
	 * Rotate an existing matrix by a given yaw, pitch,
	 * and roll.
	 */
	void Rotate( valType yaw, valType pitch, valType roll );
	
	/**
	 * These functions are incremental; 
	 * they take an existing matrix and modify it.
	 * rotation information stays.
	 */
	void Translate( const point3D& amt );
	void Place( const point3D& loc );

	const point3D GetLoc() const;

	/**
	 * GetAxis only returns the correct axis if we're talking about a matrix that
	 * transforms 'up' towards worldspace.  For example, this will return the 
	 * correct axis if called on a object->world matrix, but incorrect one if
	 * called on the world->camera matrix.  (If one needs the axes for such a 
	 * matrix, invert the world->camera transform to get a camera->world 
	 * transform)
	 */
	point3D GetAxis( int axis ) const;

	static const matrix4D Identity;
};



//==========--------------------------  matrix4D operators


/**
 * returns matrix4D*matrix4D
 * not inlined due to the size.
 */
matrix4D operator*(matrix4D const &a, matrix4D const &b);
void MatMult( const matrix4D &a, const matrix4D& b, matrix4D* result );


/**
 * Specialization for multiplication by a point3D structure.
 * in a mathematically perfect world, we would construct
 * a 1x4 matrix for the point, and then multiply the 1x4
 * and the 4x4 matrices toegether.  However, this performs
 * much better.
 */
inline const point3D operator*( const matrix4D &a, const point3D &b)
{
	return point3D(
		b.x*a._11 + b.y*a._21 + b.z*a._31 + a._41,
		b.x*a._12 + b.y*a._22 + b.z*a._32 + a._42,
		b.x*a._13 + b.y*a._23 + b.z*a._33 + a._43
	);
};

inline const point3D operator*( const point3D &a, const matrix4D &b)
{
	return b*a;
};


/**
 * Specialization for multiplication by a point4 structure.
 */
/******
inline const point4 operator*( const matrix4D &a, const point4 &b)
{
	return point4(
		b.x*a._11 + b.y*a._21 + b.z*a._31 + b.w*a._41,
		b.x*a._12 + b.y*a._22 + b.z*a._32 + b.w*a._42,
		b.x*a._13 + b.y*a._23 + b.z*a._33 + b.w*a._43,
		b.x*a._14 + b.y*a._24 + b.z*a._34 + b.w*a._44
	);
};

inline const point4 operator*( const point4 &a, const matrix4D &b)
{
	return b*a;
};
*******/


//returns matrix4D*lineSeg3
/***
inline lineSeg3 operator*( lineSeg3 const &b, matrix4D const &a )
{
	return lineSeg3( b.v0 * a, b.v1 * a );
};
//returns matrix4D*lineSeg3
inline lineSeg3 operator*( matrix4D const &a, lineSeg3 const &b )
{
	return lineSeg3( b.v0 * a, b.v1 * a );
};
***/


#endif //_MATRIX4D_H