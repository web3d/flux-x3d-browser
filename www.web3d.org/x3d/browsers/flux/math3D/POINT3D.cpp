/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: point3D.cpp
 *    Desc: defenition of a generic 3D vector class, with 
 *          operators aplenty
 * copyright (c) 1999 by Adrian Perez
 * See license.txt for modification and distribution information
 ******************************************************************/

#include "stdafx.h"
#include "point3D.h"
#include "point4.h"

const point3D point3D::Zero( 0.,0.,0. );

const point3D point3D::i( 1., 0., 0. );
const point3D point3D::j( 0., 1., 0. );
const point3D point3D::k( 0., 0., 1. );

/**

point3D::point3D( const point4& in )
{
	if( in.w != 1. )
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

**/