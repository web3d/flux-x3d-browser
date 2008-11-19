/********************************************************************************
 * Flux
 *
 * File: nurbssurfaceevaluator.cpp
 * Description: Nurbs Surface Evaluator
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
#include "anmnurbssurfaceevaluator.h"

CAnmNurbsSurfaceEvaluator::CAnmNurbsSurfaceEvaluator() :
	m_uDimension( 0 ),
	m_vDimension( 0 ),
	m_pControlPoints( NULL ),
	m_pControlTextureCoords( NULL ),
	m_pWeights( NULL ),
	m_bValidPoints( false ),
	m_bValidTextureCoords( false ),
	m_bValidWeights( false ),
	m_bValidDirty( true )
{
}

CAnmNurbsSurfaceEvaluator::~CAnmNurbsSurfaceEvaluator()
{
	SafeUnRef(m_pControlPoints);
	SafeUnRef(m_pControlTextureCoords);
	SafeUnRef(m_pWeights);
}



void CAnmNurbsSurfaceEvaluator::SetControlPoints( Point3Array* p )
{ 
	NURBDIRTY 
	if( p ) {
		p->Ref();
	}
	SafeUnRef(m_pControlPoints);
	m_pControlPoints=p; 
}

void CAnmNurbsSurfaceEvaluator::SetControlTextureCoords( Point2Array* p )
{ 
	NURBDIRTY 
	if( p ) {
		p->Ref();
	}
	SafeUnRef(m_pControlTextureCoords);
	m_pControlTextureCoords=p; 
}

void CAnmNurbsSurfaceEvaluator::SetWeights( DoubleArray* p )
{ 
	NURBDIRTY 
	if( p ) {
		p->Ref();
	}
	SafeUnRef(m_pWeights);
	m_pWeights=p; 
}


static void Normalize( float vec[] )
{
	float mag=0.0f;

	mag = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];

	if(mag < 1.0e-60) {
		vec[0] = 1.0;
		return;
	}

	mag = sqrt(mag);
	vec[0] /= mag;
	vec[1] /= mag;
	vec[2] /= mag;
}

void static CrossProduct( float vec1[], float vec2[], float vec[] )
{
	vec[0] = (vec1[1]*vec2[2] - vec1[2]*vec2[1]);
	vec[1] = (vec1[2]*vec2[0] - vec1[0]*vec2[2]);
	vec[2] = (vec1[0]*vec2[1] - vec1[1]*vec2[0]);
}

bool CAnmNurbsSurfaceEvaluator::Evaluate( 
					int iuspan,
					int ivspan,
					double duBasis[],
					double dvBasis[],
					float* pPointOut, float* TextureCoordOut )
{
	bool bStatus = false;

	bool bDoPoints = m_bValidPoints && pPointOut != NULL;
	bool bDoTextureCoords = m_bValidTextureCoords && TextureCoordOut != NULL;

	if( bDoPoints || bDoTextureCoords ) {

		DoEvaluate( 
					iuspan,
					ivspan,
					duBasis,
					dvBasis,
			( ( bDoPoints ) ? pPointOut : NULL ), 
			( ( bDoTextureCoords ) ? TextureCoordOut : NULL ) );

		bStatus = true;
	}
	return bStatus;
}







bool CAnmNurbsSurfaceEvaluator::Evaluate( 
					int iuspan, int iuspanEpsilon, 
					int ivspan, int ivspanEpsilon, 
					double duBasis[], double duBasisEpsilon[], 
					double dvBasis[], double dvBasisEpsilon[], 
					float* pPointOut, float* pNormalOut, float* TextureCoordOut )
{
	bool bStatus = false;
	if( m_bValidDirty ) {
		CheckValidity();
	}

	bool bDoPoints = m_bValidPoints && pPointOut != NULL;
	bool bDoNormals = bDoPoints && pNormalOut != NULL;
	bool bDoTextureCoords = m_bValidTextureCoords && TextureCoordOut != NULL;

	if( bDoPoints || bDoTextureCoords ) {

		DoEvaluate( iuspan, ivspan, 
					duBasis, dvBasis,
			( ( bDoPoints ) ? pPointOut : NULL ), 
			( ( bDoTextureCoords ) ? TextureCoordOut : NULL ) );

		bStatus = true;
		// If we need the normal, calculate it with finite difference.
		//
		if( bDoNormals ) {
			float Ru[3];
			float Rv[3];
			DoEvaluate( abs( iuspanEpsilon ), ivspan, 
						duBasisEpsilon, dvBasis,
						Ru );
			if( iuspanEpsilon >= 0 ) {
				Ru[0] -= pPointOut[0];
				Ru[1] -= pPointOut[1];
				Ru[2] -= pPointOut[2];
			}
			else {
				Ru[0] = pPointOut[0] - Ru[0];
				Ru[1] = pPointOut[1] - Ru[1];
				Ru[2] = pPointOut[2] - Ru[2];
			}

			DoEvaluate( iuspan, abs( ivspanEpsilon ), 
						duBasis, dvBasisEpsilon,
						Rv );
			if( ivspanEpsilon >= 0 ) {
				Rv[0] -= pPointOut[0];
				Rv[1] -= pPointOut[1];
				Rv[2] -= pPointOut[2];
			}
			else {
				Rv[0] = pPointOut[0] - Rv[0];
				Rv[1] = pPointOut[1] - Rv[1];
				Rv[2] = pPointOut[2] - Rv[2];
			}
			CrossProduct( Ru, Rv, pNormalOut );
			Normalize( pNormalOut );
		}
	}
	return bStatus;
}



bool CAnmNurbsSurfaceEvaluator::Evaluate( double u, double v, 
					float* pPointOut, float* pNormalOut, float* TextureCoordOut )
{
	bool bStatus = false;
	if( m_bValidDirty ) {
		CheckValidity();
	}

	bool bDoPoints = m_bValidPoints && pPointOut != NULL;
	bool bDoNormals = bDoPoints && pNormalOut != NULL;
	bool bDoTextureCoords = m_bValidTextureCoords && TextureCoordOut != NULL;

	if( bDoPoints || bDoTextureCoords ) {

		DoEvaluate( u, v, 
			( ( bDoPoints ) ? pPointOut : NULL ), 
			( ( bDoTextureCoords ) ? TextureCoordOut : NULL ) );

		bStatus = true;
		// If we need the normal, calculate it with finite difference.
		//
		if( bDoNormals ) {
			float Ru[3];
			float Rv[3];
			if( u < 0.99 ) {
				DoEvaluate( u + UVEPSILON, v, Ru );
				Ru[0] -= pPointOut[0];
				Ru[1] -= pPointOut[1];
				Ru[2] -= pPointOut[2];
			}
			else {
				DoEvaluate( u - UVEPSILON, v, Ru );
				Ru[0] = pPointOut[0] - Ru[0];
				Ru[1] = pPointOut[1] - Ru[1];
				Ru[2] = pPointOut[2] - Ru[2];
			}

			if( v < 0.99 ) {
				DoEvaluate( u, v + UVEPSILON, Rv );
				Rv[0] -= pPointOut[0];
				Rv[1] -= pPointOut[1];
				Rv[2] -= pPointOut[2];
			}
			else {
				DoEvaluate( u, v - UVEPSILON, Rv );
				Rv[0] = pPointOut[0] - Rv[0];
				Rv[1] = pPointOut[1] - Rv[1];
				Rv[2] = pPointOut[2] - Rv[2];
			}
			CrossProduct( Ru, Rv, pNormalOut );
			Normalize( pNormalOut );
		}
	}
	return bStatus;
}








bool CAnmNurbsSurfaceEvaluator::CalcNurbsBasisU( double u, 
												int* piuspan, double* duBasis,
												int* piuspanEpsilon, double* duBasisEpsilon )
{
	bool bStatus = false;
	if( m_bValidDirty ) {
		CheckValidity();
	}
	if( m_bValidPoints || m_bValidTextureCoords ) {

		bStatus = true;
		double uUse = u* ( m_uKnotVector.GetKnotTail() - m_uKnotVector.GetKnotHead() ) + m_uKnotVector.GetKnotHead();

		int iuspan = m_uKnotVector.FindNurbsSpan( uUse );

		m_uKnotVector.CalcNurbsBasis( iuspan, uUse, duBasis );

		*piuspan = iuspan;
		if( piuspanEpsilon && duBasisEpsilon ) {

			int iInvert = 1;
			if( u < 0.99 ) {
				u += UVEPSILON;
			}
			else {
				u -= UVEPSILON;
				iInvert = -1;
			}

			double uUse = u* ( m_uKnotVector.GetKnotTail() - m_uKnotVector.GetKnotHead() ) + m_uKnotVector.GetKnotHead();
			int iuspan = m_uKnotVector.FindNurbsSpan( uUse );
			m_uKnotVector.CalcNurbsBasis( iuspan, uUse, duBasisEpsilon );
			*piuspanEpsilon = iuspan*iInvert;
		}
	
	
	}
	return bStatus;
}


bool CAnmNurbsSurfaceEvaluator::CalcNurbsBasisV( double v, 
												int* pivspan, double* dvBasis,
												int* pivspanEpsilon, double* dvBasisEpsilon )
{
	bool bStatus = false;
	if( m_bValidDirty ) {
		CheckValidity();
	}
	if( m_bValidPoints || m_bValidTextureCoords ) {
		bStatus = true;
		double vUse = v* ( m_vKnotVector.GetKnotTail() - m_vKnotVector.GetKnotHead() ) + m_vKnotVector.GetKnotHead();
		int ivspan = m_vKnotVector.FindNurbsSpan( vUse );
		m_vKnotVector.CalcNurbsBasis( ivspan, vUse, dvBasis );
		*pivspan = ivspan;
		if( pivspanEpsilon && dvBasisEpsilon ) {

			int iInvert = 1;
			if( v < 0.99 ) {
				v += UVEPSILON;
			}
			else {
				v -= UVEPSILON;
				iInvert = -1;
			}

			double vUse = v* ( m_vKnotVector.GetKnotTail() - m_vKnotVector.GetKnotHead() ) + m_vKnotVector.GetKnotHead();
			int ivspan = m_vKnotVector.FindNurbsSpan( vUse );
			m_vKnotVector.CalcNurbsBasis( ivspan, vUse, dvBasisEpsilon );
			*pivspanEpsilon = ivspan*iInvert;
		}
	}
	return bStatus;
}




void CAnmNurbsSurfaceEvaluator::DoEvaluate( double u, double v, 
					float* pPointOut, float* TextureCoordOut )
{
	double duBasis[MAXBASIS];
	double dvBasis[MAXBASIS];


	double uUse = u* ( m_uKnotVector.GetKnotTail() - m_uKnotVector.GetKnotHead() ) + m_uKnotVector.GetKnotHead();
	double vUse = v* ( m_vKnotVector.GetKnotTail() - m_vKnotVector.GetKnotHead() ) + m_vKnotVector.GetKnotHead();

	int iuspan = m_uKnotVector.FindNurbsSpan( uUse );
	int ivspan = m_vKnotVector.FindNurbsSpan( vUse );

	m_uKnotVector.CalcNurbsBasis( iuspan, uUse, duBasis );
	m_vKnotVector.CalcNurbsBasis( ivspan, vUse, dvBasis );

	DoEvaluate( iuspan, ivspan,
				duBasis, dvBasis, 
				pPointOut, TextureCoordOut );

}



void CAnmNurbsSurfaceEvaluator::DoEvaluate( int iuspan, int ivspan,
								double duBasis[], double dvBasis[], 
								float* pPointOut, float* TextureCoordOut )
{


	int iudegree = m_uKnotVector.GetOrder() -1;
	int ivdegree = m_vKnotVector.GetOrder() -1;

	int iu0, iv0, iu, iv, ioff;
	double factor;


	if( pPointOut ) {
		pPointOut[0] =
		pPointOut[1] =
		pPointOut[2] = 0.0f;
	}
	if( TextureCoordOut ) {
		TextureCoordOut[0] =
		TextureCoordOut[1] = 0.0f;
	}

	Point3 TmpPoint3;
	Point2 TmpPoint2;

	if( m_bValidWeights ) {
		// For weighted ( Rational ) NURBS, we need to track the denominator
		//
		double denom = 0.0;
		for( iu=iuspan-iudegree, iu0=0; iu<=iuspan; iu++, iu0++ ) {
			for( iv=ivspan-ivdegree, iv0=0; iv<=ivspan; iv++, iv0++ ) {
				ioff = ( iu + iv*m_uDimension );
				factor = duBasis[iu0] * dvBasis[iv0] * (*m_pWeights)[ioff];
				if( pPointOut ) {
					TmpPoint3 = (*m_pControlPoints)[ ioff ] * ( float )factor;
					pPointOut[0] += TmpPoint3.x; 
					pPointOut[1] += TmpPoint3.y; 
					pPointOut[2] += TmpPoint3.z; 
				}
				if( TextureCoordOut ) {
					TmpPoint2 = (*m_pControlTextureCoords)[ ioff ];
					TextureCoordOut[0] += TmpPoint2.x * factor;
					TextureCoordOut[1] += TmpPoint2.y * factor;
				}
				denom += factor;
			}
		}
		if( denom > NURBZERO ) {
			if( pPointOut ) {
				pPointOut[0] /= denom;
				pPointOut[1] /= denom;
				pPointOut[2] /= denom;
			}
			if( TextureCoordOut ) {
				TextureCoordOut[0] /= denom;
				TextureCoordOut[1] /= denom;
			}
		}

	}
	else {
		// For un-weighted ( NON-Rational ) NURBS, we dont need to track the denominator
		//
		for( iu=iuspan-iudegree, iu0=0; iu<=iuspan; iu++, iu0++ ) {
			for( iv=ivspan-ivdegree, iv0=0; iv<=ivspan; iv++, iv0++ ) {
				ioff = ( iu + iv*m_uDimension );
				factor = duBasis[iu0] * dvBasis[iv0];
				if( pPointOut ) {
					TmpPoint3 = (*m_pControlPoints)[ ioff ] * ( float )factor;
					pPointOut[0] += TmpPoint3.x; 
					pPointOut[1] += TmpPoint3.y; 
					pPointOut[2] += TmpPoint3.z; 
				}
				if( TextureCoordOut ) {
					TmpPoint2 = (*m_pControlTextureCoords)[ ioff ];
					TextureCoordOut[0] += TmpPoint2.x * factor;
					TextureCoordOut[1] += TmpPoint2.y * factor;
				}
			}
		}
	}
}




void CAnmNurbsSurfaceEvaluator::CheckValidity()
{
	// Check the validity of the data.
	//
	if( m_bValidDirty ) {
	
		m_bValidPoints = false;
		m_bValidTextureCoords = false;
		m_bValidWeights = false;
		
		
		// First, ask the Knot vectors to validate themselves.
		// This will generate a valid KnotVector, if there is not one.
		//
		if( 
			m_uKnotVector.IsValid( m_uDimension ) &&
			m_vKnotVector.IsValid( m_vDimension ) ) {

			int nPnts = m_uDimension*m_vDimension;

			int nControlPoints = GetnControlPoints();
			m_bValidPoints = ( nControlPoints > 0 &&
								nControlPoints == nPnts );

			int nTexPoints = GetnControlTextureCoords();
			m_bValidTextureCoords = ( nTexPoints > 0 &&
								nTexPoints == nPnts );

			// See if we have weights, AND if they are all not 1.0.
			//
			int nWeights = GetnWeights();
			m_bValidWeights = ( nWeights > 0 &&
								nWeights == nPnts );

			if( m_bValidWeights ) {
				m_bValidWeights = false;
				int nWeights = GetnWeights();
				for( int iwght=0; iwght<nWeights; iwght++ ) {
					if( fabs( (*m_pWeights)[iwght] - 1.0 ) > NURBZERO ) {
						m_bValidWeights = true;
						break;
					}
				}
			}
		}
		m_bValidDirty = false;
	}
}


