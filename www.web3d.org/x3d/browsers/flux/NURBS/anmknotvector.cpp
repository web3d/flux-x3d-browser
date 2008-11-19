/********************************************************************************
 * Flux
 *
 * File: anmknotvector.cpp
 * Description: Nurbs support class
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
#include "anmknotvector.h"

CAnmKnotVector::CAnmKnotVector() :
	m_iOrder( 0 ),
	m_pKnot( NULL ),
	m_pGeneratedKnotVector( NULL )
{
}




CAnmKnotVector::~CAnmKnotVector()
{ 
	SafeUnRef( m_pGeneratedKnotVector ); 
	SafeUnRef( m_pKnot ); 
}


void CAnmKnotVector::SetKnotVector( DoubleArray* pKnot)
{ 
	if( pKnot ) {
		pKnot->Ref();
	}
	SafeUnRef(m_pKnot);
	m_pKnot = pKnot; 
}



static void DoNurbsBasis( int i, double u, int p, DoubleArray* vKnot, double* vBasis )
{
	double saved = 0.0;
	double temp = 0.0;

	double right[MAXBASIS];
	double left[MAXBASIS];


	vBasis[0] = 1.0;
	for( int j=1; j<=p; j++ ) {
		left[j] = u - (*vKnot)[i+1-j];
		right[j] = (*vKnot)[i+j] - u;
		saved = 0.0;
		for( int r=0; r<j; r++ ) {
			temp = vBasis[r] / ( right[r+1] + left[j-r] );
			vBasis[r] = saved+right[r+1]*temp;
			saved = left[j-r]*temp;
		}
		vBasis[j] = saved;
	}
}


int CAnmKnotVector::FindNurbsSpan( double u)
{
	int n = GetnKnots() - m_iOrder;
	int p = m_iOrder - 1;

	if( u >= (*m_pKnot)[n+1] ) {
		return n;
	}
	if( u < (*m_pKnot)[p] ) {
		return p-1;
	}

	int low = p;
	int high = n+1;
	int mid = ( low + high ) / 2;
	while( u < (*m_pKnot)[mid] || u >= (*m_pKnot)[mid+1] ) {
		if( u < (*m_pKnot)[mid] ) {
			high = mid;
		}
		else {
			low = mid;
		}
		mid = ( low + high ) / 2;
	}
	return mid;


}

void CAnmKnotVector::CalcNurbsBasis( int& i, double u, Double* dBasis )
{
	if( i < m_iOrder - 1 ) {
		for( int k=1; k<m_iOrder; k++ ) {
			dBasis[k] = 0.0;
		}
		dBasis[0] = 1.0;
		i = m_iOrder - 1;
	}
	else if( i >= GetnKnots()-m_iOrder ) {
		for( int k=0; k<m_iOrder-1; k++ ) {
			dBasis[k] = 0.0;
		}
		dBasis[m_iOrder-1] = 1.0;
		i = GetnKnots()-m_iOrder - 1;
	}
	else {
		DoNurbsBasis( i, u, m_iOrder - 1, m_pKnot, dBasis );
	}
}



bool CAnmKnotVector::IsValid( int nDimension, bool bDontGenerate )
{
	bool bIsValid = false;
	if( m_iOrder >= 2 && 
		m_iOrder < MAXBASIS &&
		nDimension >= m_iOrder ) {


		// krv??? Spec says we need this many knots.  I think not!
//		int nKnots = nDimension + m_iOrder - 1;
		int nKnots = nDimension + m_iOrder;
		int nKnotsGot = GetnKnots();
		if( nKnots == nKnotsGot ) {
			// Make sure the vector is not decreasing.
			//
			bIsValid = true;
			for( int i=1; i<nKnots && bIsValid; i++ ) {
				if( (*m_pKnot)[i] - (*m_pKnot)[i-1] < -NURBZERO ) {
					bIsValid = false;
				}
			}
		}

		if( !bIsValid && !bDontGenerate ) {
			if( GetnKnots() > 0 ) {
				// krv???
				// TODO issue warning!
			}
			MakeUniformKnots( nKnots );
			// Its valid now!
			//
			bIsValid = true;
		}
		
	}
	if( m_iOrder >= MAXBASIS ) {
		// krv???
		// TODO issue warning!
	}
	return bIsValid;
}


void CAnmKnotVector::MakeUniformKnots( int nuKnots )
{
	SafeUnRef( m_pKnot ); 

	if( !m_pGeneratedKnotVector ) {
		m_pGeneratedKnotVector = new DoubleArray;
		m_pGeneratedKnotVector->Ref();
	}
	m_pGeneratedKnotVector->resize( nuKnots );
	m_pKnot = m_pGeneratedKnotVector;
	m_pKnot->Ref();

	int i;
	for( int iknot=0; iknot<m_iOrder; iknot++ ) {
		(*m_pKnot)[iknot] = 0.0f;
		(*m_pKnot)[ nuKnots - iknot - 1] = 1.0f;
	}
	int nleft = nuKnots - 2*m_iOrder;
	double fknot, delta = ( 1.0 / ( double)( nleft+1 ));
	for( fknot = delta, i=0, iknot = m_iOrder; i<nleft; iknot++, i++, fknot+=delta ) {
		(*m_pKnot)[iknot] = fknot;
	}
}

