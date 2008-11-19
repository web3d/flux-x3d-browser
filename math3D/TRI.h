/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: tri.h
 *    Desc: template-based triangle container class
 *          
 * copyright (c) 1999 by Adrian Perez
 * See license.txt for modification and distribution information
 ******************************************************************/

#ifndef _TRI_H
#define _TRI_H

template <class type>
struct EXPORT tri 
{

    type v[3]; // Array access useful in for loops
	
	tri()
	{
		// nothing
	}

	tri( type v0, type v1, type v2 )
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}
};

#endif //_TRI_H