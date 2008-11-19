/********************************************************************************
 * Flux
 *
 * File: anmmatrixinvert.cpp
 * Description: Matrix Inversion Support
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
#include <math.h>

#include "anmMatrixInvert.h"


extern long	MMInvertMatrix( const float* m, float*mInv, long nCols );
extern long	MMDInvertMatrix( const double* m, double*mInv, long nCols );



// krv:
// THis was moved from the AnmTransform Node
// I also rewrote this so that it does not use the use the Matrix decomposition, unless
//  the matrix has a non-=trivial bottom row.
//

long MatrixInvert( matrix4& q, matrix4& a )
{
    if( ( fabs(a._44 - 1.0f) > .001f) ||
        (  fabs(a._14) > .001f || fabs(a._24) > .001f || fabs(a._34) > .001f ) ) {

		long retval = MMInvertMatrix(a.m[0], q.m[0], 4);
		return retval ? S_OK : S_FALSE;
	}
	else {
		
#define DOMATRIX_INVERSION_IN_DOUBLE
#ifdef DOMATRIX_INVERSION_IN_DOUBLE



		double a_11 = ( double ) a._11;
		double a_12 = ( double ) a._12;
		double a_13 = ( double ) a._13;

		double a_21 = ( double ) a._21;
		double a_22 = ( double ) a._22;
		double a_23 = ( double ) a._23;

		double a_31 = ( double ) a._31;
		double a_32 = ( double ) a._32;
		double a_33 = ( double ) a._33;

		double a_41 = ( double ) a._41;
		double a_42 = ( double ) a._42;
		double a_43 = ( double ) a._43;


		double q_11;
		double q_12;
		double q_13;

		double q_21;
		double q_22;
		double q_23;

		double q_31;
		double q_32;
		double q_33;

		double fDet =			( a_11 * ( a_22 * a_33 - a_23 * a_32 ) -
								  a_12 * ( a_21 * a_33 - a_23 * a_31 ) +
								  a_13 * ( a_21 * a_32 - a_22 * a_31 ) );

		if( fabs( fDet ) < 1.0e-50 ) {
			q = matrix4::Identity;
			return S_FALSE;
		}
		else {

			double fDetInv = 1.0 / fDet;

			q_11 =  fDetInv * ( a_22 * a_33 - a_23 * a_32 );
			q_12 = -fDetInv * ( a_12 * a_33 - a_13 * a_32 );
			q_13 =  fDetInv * ( a_12 * a_23 - a_13 * a_22 );
			q._14 = 0.0f;

			q_21 = -fDetInv * ( a_21 * a_33 - a_23 * a_31 );
			q_22 =  fDetInv * ( a_11 * a_33 - a_13 * a_31 );
			q_23 = -fDetInv * ( a_11 * a_23 - a_13 * a_21 );
			q._24 = 0.0f;

			q_31 =  fDetInv * ( a_21 * a_32 - a_22 * a_31 );
			q_32 = -fDetInv * ( a_11 * a_32 - a_12 * a_31 );
			q_33 =  fDetInv * ( a_11 * a_22 - a_12 * a_21 );
			q._34 = 0.0f;

			q._41 = ( float ) -( a_41 * q_11 + a_42 * q_21 + a_43 * q_31 );
			q._42 = ( float ) -( a_41 * q_12 + a_42 * q_22 + a_43 * q_32 );
			q._43 = ( float ) -( a_41 * q_13 + a_42 * q_23 + a_43 * q_33 );
			q._44 = 1.0f;


			q._11 = ( float )q_11;
			q._12 = ( float )q_12;
			q._13 = ( float )q_13;

			q._21 = ( float )q_21;
			q._22 = ( float )q_22;
			q._23 = ( float )q_23;

			q._31 = ( float )q_31;
			q._32 = ( float )q_32;
			q._33 = ( float )q_33;


			return S_OK;
		}		
	
#else
		


		FLOAT fDet =		   ( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
								 a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
								 a._13 * ( a._21 * a._32 - a._22 * a._31 ) );

		if( fabs( fDet ) < 1.0e-30 ) {
			q = matrix4::Identity;
			return S_FALSE;
		}
		else {

			FLOAT fDetInv = 1.0f / fDet;

			q._11 =  fDetInv * ( a._22 * a._33 - a._23 * a._32 );
			q._12 = -fDetInv * ( a._12 * a._33 - a._13 * a._32 );
			q._13 =  fDetInv * ( a._12 * a._23 - a._13 * a._22 );
			q._14 = 0.0f;

			q._21 = -fDetInv * ( a._21 * a._33 - a._23 * a._31 );
			q._22 =  fDetInv * ( a._11 * a._33 - a._13 * a._31 );
			q._23 = -fDetInv * ( a._11 * a._23 - a._13 * a._21 );
			q._24 = 0.0f;

			q._31 =  fDetInv * ( a._21 * a._32 - a._22 * a._31 );
			q._32 = -fDetInv * ( a._11 * a._32 - a._12 * a._31 );
			q._33 =  fDetInv * ( a._11 * a._22 - a._12 * a._21 );
			q._34 = 0.0f;

			q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
			q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
			q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
			q._44 = 1.0f;
		}
		return S_OK;
#endif
	
	}	
	
	
}



// krv:  Added Matrix Invert to the MMDMatrix file.
// Modified inversion method.

long MatrixInvert( matrix4D& q, matrix4D& a )
{
	

    if( ( fabs(a._44 - 1.0f) > .001f ) ||
        (  fabs(a._14) > .001f || fabs(a._24) > .001f || fabs(a._34) > .001f ) ) {
//		matrix4 a_f( a );
//		matrix4 q_f;
//		MatrixInvert( q_f, a_f );

		long retval = MMDInvertMatrix(a.m[0], q.m[0], 4);

		return retval ? S_OK : S_FALSE;
	}
	else {
		valType fDet =		   ( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
								 a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
								 a._13 * ( a._21 * a._32 - a._22 * a._31 ) );

		if( fabs( fDet ) < 1.0e-50 ) {
			q = matrix4D::Identity;
			return S_FALSE;
		}
		else {

			valType fDetInv = 1.0f / fDet;

			q._11 =  fDetInv * ( a._22 * a._33 - a._23 * a._32 );
			q._12 = -fDetInv * ( a._12 * a._33 - a._13 * a._32 );
			q._13 =  fDetInv * ( a._12 * a._23 - a._13 * a._22 );
			q._14 = 0.0f;

			q._21 = -fDetInv * ( a._21 * a._33 - a._23 * a._31 );
			q._22 =  fDetInv * ( a._11 * a._33 - a._13 * a._31 );
			q._23 = -fDetInv * ( a._11 * a._23 - a._13 * a._21 );
			q._24 = 0.0f;

			q._31 =  fDetInv * ( a._21 * a._32 - a._22 * a._31 );
			q._32 = -fDetInv * ( a._11 * a._32 - a._12 * a._31 );
			q._33 =  fDetInv * ( a._11 * a._22 - a._12 * a._21 );
			q._34 = 0.0f;

			q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
			q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
			q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
			q._44 = 1.0f;
			return S_OK;
		}		
	}
}


