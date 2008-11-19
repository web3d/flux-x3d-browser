/********************************************************************************
 * Flux
 *
 * File: anmnurbssurfaceevaluator.h
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

#ifndef _anmnurbssurfaceevaluator_h
#define _anmnurbssurfaceevaluator_h

#include "anmknotvector.h"
#include "anmnodedefs.h"

#define NURBDIRTY  { m_bValidDirty=true; }

class  CAnmNurbsSurfaceEvaluator 
{

protected:

 
public:

	// constructor/destructor
	CAnmNurbsSurfaceEvaluator();
	virtual ~CAnmNurbsSurfaceEvaluator();

	bool Evaluate( double u, double v, 
					float* pPointOut, 
					float* pNormalOut = NULL, 
					float* TextureCoordOut = NULL );

	bool Evaluate( 
					int iuspan,
					int ivspan,
					double duBasis[],
					double dvBasis[],
					float* pPointOut, float* TextureCoordOut = NULL );

	bool Evaluate(  int iuspan, int iuspanEpsilon, 
					int ivspan, int ivspanEpsilon, 
					double duBasis[], double duBasisEpsilon[], 
					double dvBasis[], double dvBasisEpsilon[], 
					float* pPointOut, float* pNormalOut, float* TextureCoordOut );


	bool CalcNurbsBasisV( double v, 
							int* pivspan, double* dvBasis,
							int* pivspanEpsilon = NULL, double* dvBasisEpsilon = NULL );
	bool CalcNurbsBasisU( double v, 
							int* pivspan, double* dvBasis,
							int* pivspanEpsilon = NULL, double* dvBasisEpsilon = NULL );


	void SetuOrder( int iorder ){ NURBDIRTY m_uKnotVector.SetOrder( iorder ); }
	void SetuKnotVector( DoubleArray* pKnot){ NURBDIRTY m_uKnotVector.SetKnotVector( pKnot ); }
	void SetvOrder( int iorder ){ NURBDIRTY m_vKnotVector.SetOrder( iorder ); }
	void SetvKnotVector( DoubleArray* pKnot){ NURBDIRTY m_vKnotVector.SetKnotVector( pKnot ); }

	void SetuDimension( int iDimension ){ NURBDIRTY m_uDimension = iDimension; }
	void SetvDimension( int iDimension ){ NURBDIRTY m_vDimension = iDimension; }

	void SetControlPoints( Point3Array* p );
	void SetControlTextureCoords( Point2Array* p );
	void SetWeights( DoubleArray* p );

	int GetnControlPoints(){ return ( m_pControlPoints ) ? m_pControlPoints->size() : 0 ; }
	int GetnControlTextureCoords(){ return ( m_pControlTextureCoords ) ? m_pControlTextureCoords->size() : 0 ; }
	int GetnWeights(){ return ( m_pWeights ) ? m_pWeights->size() : 0 ; }


	double GetuKnotStart() const { return m_uKnotVector.GetKnotStart(); }
	double GetuKnotEnd() const {   return m_uKnotVector.GetKnotEnd(); }
	double GetvKnotStart() const { return m_vKnotVector.GetKnotStart(); }
	double GetvKnotEnd() const {   return m_vKnotVector.GetKnotEnd(); }

	double GetunKnots( ) const { return m_uKnotVector.GetnKnots(); }
	double GetvnKnots( ) const { return m_vKnotVector.GetnKnots(); }
	double GetnKnots( int iUV ) const { return ( ( iUV==0 ) ? ( m_uKnotVector.GetnKnots() ) : ( m_vKnotVector.GetnKnots() ) ); }

	double GetuKnotValue( int i ) const { return m_uKnotVector.GetKnotValue(i); }
	double GetvKnotValue( int i ) const { return m_vKnotVector.GetKnotValue(i); }
	double GetKnotValue( int iUV, int i ) const { return ( ( iUV==0 ) ? ( m_uKnotVector.GetKnotValue(i) ) : ( m_vKnotVector.GetKnotValue(i) ) ); }


	bool IsValid( int nDimension );



	bool ArePointsValid(){ CheckValidity(); return m_bValidPoints; }
	bool AreTextureCoordsValid(){ CheckValidity(); return m_bValidTextureCoords; }

protected:

	void CheckValidity();
	void DoEvaluate( double u, double v, 
					float* pPointOut, float* TextureCoordOut = NULL );

	void DoEvaluate( int iuspan, int ivspan,
								double duBasis[], double dvBasis[], 
								float* pPointOut, float* TextureCoordOut = NULL );


	int m_uDimension;
	int m_vDimension;

	CAnmKnotVector m_uKnotVector;
	CAnmKnotVector m_vKnotVector;

	Point3Array* m_pControlPoints;
	Point2Array* m_pControlTextureCoords;
	DoubleArray* m_pWeights;

	bool	m_bValidDirty;
	bool	m_bValidPoints;
	bool	m_bValidTextureCoords;
	bool	m_bValidWeights;

};


#endif // _anmnurbssurfaceevaluator_h
