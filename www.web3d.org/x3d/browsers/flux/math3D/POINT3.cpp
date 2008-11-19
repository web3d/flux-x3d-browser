/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: point3.cpp
 *    Desc: defenition of a generic 3D vector class, with 
 *          operators aplenty
 * copyright (c) 1999 by Adrian Perez
 * See license.txt for modification and distribution information
 ******************************************************************/

#include "stdafx.h"
#include "point3.h"
#include "point4.h"
#include "point3D.h"

const point3 point3::Zero( 0.f,0.f,0.f );

const point3 point3::i( 1.f, 0.f, 0.f );
const point3 point3::j( 0.f, 1.f, 0.f );
const point3 point3::k( 0.f, 0.f, 1.f );

point3::point3( const point4& in )
{
	/**
	 * If the homogenous coordinate isn't 1, normalize the point.
	 */
	if( in.w != 1.f )
	{
		float inv = 1/in.w;
		x = in.x * inv;
		y = in.y * inv;
		z = in.z * inv;
	}
	else
	{
		x = in.x;
		y = in.y;
		z = in.z;
	}
}

point3::point3( const point3D& pntf )
{
	x = ( float )pntf.x;
	y = ( float )pntf.y;
	z = ( float )pntf.z;
}
