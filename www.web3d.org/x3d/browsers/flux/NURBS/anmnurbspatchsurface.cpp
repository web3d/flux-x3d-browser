/********************************************************************************
 * Flux
 *
 * File: anmnurbspatchsurface.cpp
 * Description: NurbsPatchSurface node
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
#include "anmnurbspatchsurface.h"
#include "anmcoordinate.h"
#include "anmmesh.h"
#include "anmprimitive.h"
#include "anmtexturecoordinate.h"
#include "anmnurbstexturecoordinate.h"
#include "anmworld.h"

CAnmNurbsPatchSurface::CAnmNurbsPatchSurface() : CAnmGeometry(),
m_uTessellation(ANMNURBSPATCHSURFACE_DEFAULT_UTESSELLATION),
m_vTessellation(ANMNURBSPATCHSURFACE_DEFAULT_VTESSELLATION),
m_uClosed(ANMNURBSPATCHSURFACE_DEFAULT_UCLOSED),
m_uDimension(ANMNURBSPATCHSURFACE_DEFAULT_UDIMENSION),
m_uOrder(ANMNURBSPATCHSURFACE_DEFAULT_UORDER),
m_vClosed(ANMNURBSPATCHSURFACE_DEFAULT_VCLOSED),
m_vDimension(ANMNURBSPATCHSURFACE_DEFAULT_VDIMENSION),
m_vOrder(ANMNURBSPATCHSURFACE_DEFAULT_VORDER),

m_ClosedControlPointArray( NULL ),
m_ClosedTexCoordArray( NULL ),
m_ClosedWeight( NULL ),
m_uUseClosed( FALSE ),
m_vUseClosed( FALSE ),
m_uClosedDimension( 0 ),
m_uClosedKnot( NULL ),
m_vClosedDimension( 0 ),
m_vClosedKnot( NULL ),
m_uKnotStart( 1.0 ),
m_uKnotEnd( 0.0 ),
m_vKnotStart( 1.0 ),
m_vKnotEnd( 0.0 )
{
	m_solid = ANMNURBSPATCHSURFACE_DEFAULT_SOLID;

	m_controlPoint = NULL;
	m_texCoord = NULL;

	m_weight = new DoubleArray;
	m_uKnot = new DoubleArray;
	m_vKnot = new DoubleArray;

}

CAnmNurbsPatchSurface::~CAnmNurbsPatchSurface()
{
	SafeUnRef(m_weight);
	SafeUnRef(m_uKnot);
	SafeUnRef(m_vKnot);
	SafeUnRef(m_ClosedControlPointArray);
	SafeUnRef(m_ClosedTexCoordArray);
	SafeUnRef(m_ClosedWeight);
	SafeUnRef(m_uClosedKnot);
	SafeUnRef(m_vClosedKnot);
}

void CAnmNurbsPatchSurface::Realize()
{
	CreateMesh();
	CAnmGeometry::Realize();		// chain down
	SetStateDirty();
}

void CAnmNurbsPatchSurface::Update()
{
	if (StateDirty())
	{
		CreateMesh();
		SetStateDirty();
	}
}


#define	TCM_USE_UV						0
#define TCM_USE_TEXCOORD				1
#define TCM_USE_NURBS_TEX_COORD_NODE	2

void CAnmNurbsPatchSurface::CreateMesh()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	// Here's where geometry gets created from Realize() or Update()

	// just use the CAnmTriangleStrip class in Graphics/anmprimitive.{cpp,h}. Note that

	SafeUnRef(m_mesh);

	bool bGotMesh = false;
	bool buClosed = false;
	bool bvClosed = false;

	

	if( IsMeshClosed( buClosed, bvClosed ) ) {
		bGotMesh = CreateClosedMesh( buClosed, bvClosed );
	}
	else {
		bGotMesh = CreateOpenMesh();
	}

	if( bGotMesh ) {

		// If we have a good mesh, create the back faces, if needed.
		//
		m_mesh->SetDiffuseColor(color4::White);

		if (!m_solid)
		{
			tAnmPrimitiveArray *pPrimitives = m_mesh->GetPrimitives();
			int nprims = pPrimitives->size();
			for (int i = 0; i < nprims ; i++)
			{
				CAnmPrimitive *pPrim = (*pPrimitives)[i];
				pPrim->GenerateBackfaces(pDevice);
			}
		}
	}
}


#define ZERO_DIFF 0.000001
// Test if either U or V control Points are closed, and the corresponding closed field is true,
//
bool CAnmNurbsPatchSurface::IsMeshClosed( bool& buClosed, bool& bvClosed )
{
	buClosed = false;
	bvClosed = false;


	if( m_controlPoint && ( m_uClosed || m_vClosed ) ) {
		CAnmNode *pImp = m_controlPoint->GetImplementationNode();
		if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate))) {
			CAnmCoordinate *pCoord = (CAnmCoordinate *) pImp;
			Point3Array* pPointArray = pCoord->GetPoint();


			if( m_uDimension*m_vDimension == pPointArray->size() ) {
				// Check uClosed.
				if( m_uClosed && m_uDimension > 2 && m_uOrder > 2 ) {
					int ivOff;
					int iuSeam0 = 0;
					int iuSeamf = ( m_uDimension-1 );
					buClosed = true;
					for( int iv=0; iv<m_vDimension && buClosed; iv++ ) {
						ivOff = iv*m_uDimension;
						Point3 Pnt = (*pPointArray)[iuSeam0 + ivOff] - (*pPointArray)[iuSeamf + ivOff];
						if( Pnt.MagSquared() > ZERO_DIFF ) {
							buClosed = false;
						}
					}
				}

				// Check vClosed.
				if( m_vClosed && m_vDimension > 2 && m_vOrder > 2 ) {
					int ivSeam0 = 0;
					int ivSeamf = ( m_vDimension-1 )*m_uDimension;
					bvClosed = true;
					for( int iu=0; iu<m_uDimension && bvClosed; iu++ ) {
						Point3 Pnt = (*pPointArray)[ivSeam0 + iu] - (*pPointArray)[ivSeamf + iu];
						if( Pnt.MagSquared() > ZERO_DIFF ) {
							bvClosed = false;
						}
					}
				}
			}
		}
	}

	return buClosed || bvClosed;
}




bool CAnmNurbsPatchSurface::CreateClosedMesh( bool buClosed, bool bvClosed )
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	
	bool bGotMesh = false;

	if( GenerateClosedArrays( buClosed, bvClosed ) ) {

/*****
	Point3Array					*m_ClosedControlPointArray;
	Point2Array					*m_ClosedTexCoordArray;
	DoubleArray					*m_ClosedWeight;
	Boolean						 m_uUseClosed;
	Boolean						 m_vUseClosed;
	Integer						 m_uClosedDimension;
	DoubleArray					*m_uClosedKnot;
	Integer						 m_vClosedDimension;
	DoubleArray					*m_vClosedKnot;
*********/




	

	



		// figure out the number of tessellation Points.
		//
		int iTexCoordMode = TCM_USE_UV;
		int nuPoints;
		int nvPoints;
		if( m_uTessellation > 0 ) {
			nuPoints = ( m_uTessellation ) + 1;
		}
		else if( m_uTessellation < 0 ){
			nuPoints = ( -m_uTessellation * m_uClosedDimension ) + 1;
		}
		else {
			nuPoints = ( 2 * m_uClosedDimension ) + 1;
		}

		if( m_vTessellation > 0 ) {
			nvPoints = ( m_vTessellation ) + 1;
		}
		else if( m_vTessellation < 0 ){
			nvPoints = ( -m_vTessellation * m_vClosedDimension ) + 1;
		}
		else {
			nvPoints = ( 2 * m_vClosedDimension ) + 1;
		}

		// Set the field data to the Nurbs Evaluator
		// Probably dont need to set these if nothing changed, but, this is insignificant
		//

		m_evaluator.SetuOrder( m_uOrder );
		if( m_uUseClosed ) { 
			m_evaluator.SetuKnotVector( m_uClosedKnot );
		}
		else {
			m_evaluator.SetuKnotVector( m_uKnot);
		}

		m_evaluator.SetvOrder( m_vOrder );
		if( m_vUseClosed ) { 
			m_evaluator.SetvKnotVector( m_vClosedKnot );
		}
		else {
			m_evaluator.SetvKnotVector( m_vKnot);
		}

		m_evaluator.SetuDimension( m_uClosedDimension );
		m_evaluator.SetvDimension( m_vClosedDimension );

		m_evaluator.SetWeights( m_ClosedWeight );

		m_evaluator.SetControlPoints( m_ClosedControlPointArray );
		m_evaluator.SetControlTextureCoords( NULL );


		CAnmNurbsTextureCoordinate* pNurbsTextureCoord = NULL;

		if( m_texCoord ) {
			CAnmNode *pImp = m_texCoord->GetImplementationNode();
			if( m_ClosedTexCoordArray ) {
				m_evaluator.SetControlTextureCoords( m_ClosedTexCoordArray );
				iTexCoordMode = TCM_USE_TEXCOORD;
			}
			else if( pImp && pImp->GetClass()->IsA(GETCLASS(CAnmNurbsTextureCoordinate))) {
				pNurbsTextureCoord = (CAnmNurbsTextureCoordinate *) pImp;
				iTexCoordMode = TCM_USE_NURBS_TEX_COORD_NODE;
			}
		}


		if( m_evaluator.ArePointsValid() && 
			nuPoints >= 2 &&
			nvPoints >= 2 ) {

			double du = 1.0 / double( nuPoints - 1 );
			double dv = 1.0 / double( nvPoints - 1 );
			double u;
			double v;
			double uEval;
			double vEval;

			float uTex;
			float vTex;
		
			int iV;
			int iU;
			int index = 0;
			int index0 = 0;

			float EvalPoint[3];
			float EvalNormal[3];
			float EvalTexCoord[2];

			float* pUseTexCoord = ( iTexCoordMode == TCM_USE_TEXCOORD ) ? EvalTexCoord : NULL;

			int i;
			sAnmVertex *sv = new sAnmVertex[nuPoints + 2*nvPoints + 2];		// triangle strip
			m_mesh = new CAnmMesh();
			assert(m_mesh);


			double uKnotStart;
			double uKnotEnd;
			double vKnotStart;
			double vKnotEnd;

			if( m_uKnotEnd > m_uKnotStart ) {
				uKnotStart = m_uKnotStart;
				uKnotEnd = m_uKnotEnd;
			}
			else {

				uKnotStart = m_evaluator.GetuKnotStart();
				uKnotStart *= ( (( double ) m_uOrder ) * 0.5 );
				uKnotEnd = 1.0 - uKnotStart;
			}
			
			if( m_vKnotEnd > m_vKnotStart ) {
				vKnotStart = m_vKnotStart;
				vKnotEnd = m_vKnotEnd;
			}
			else {
				vKnotStart = m_evaluator.GetvKnotStart();
				vKnotStart *= ( (( double ) m_vOrder ) * 0.5 );
				vKnotEnd = 1.0 - vKnotStart;
			}


			double uDeltaKnot = uKnotEnd - uKnotStart;
			double vDeltaKnot = vKnotEnd - vKnotStart;




		// Pre calculate the V Basis functions.
		//
		// Malloc Memory for the V Basis finction data.
		//
		double* pvBasis = new double[m_vOrder*nvPoints];
		double* pvBasisEpsilon = new double[m_vOrder*nvPoints];
		int*    pviSpan = new int[nvPoints];
		int*    pviSpanEpsilon = new int[nvPoints];

		// Fill the V Basis function Data.
		//
		double* pvBasisTmp = pvBasis;
		double* pvBasisEpsilonTmp = pvBasisEpsilon;
		int*    pviSpanTmp = pviSpan;
		int*    pviSpanEpsilonTmp = pviSpanEpsilon;
		for( iV=0, v=0.0; iV<nvPoints; iV++, v+=dv,
										pvBasisTmp+=m_vOrder,
										pvBasisEpsilonTmp+=m_vOrder,
										pviSpanTmp++,
										pviSpanEpsilonTmp++ ) {
			if( m_vUseClosed ) {
				vEval = vKnotStart + v*( vDeltaKnot );
			}
			else {
				vEval = v;
			}

			m_evaluator.CalcNurbsBasisV( vEval, 
							pviSpanTmp, pvBasisTmp,
							pviSpanEpsilonTmp, pvBasisEpsilonTmp );
		}



		double duBasis[MAXBASIS];
		double duBasisEpsilon[MAXBASIS];
		int iuSpan = 0;
		int iuSpanEpsilon = 0;



			// Loop through the u
			//
			for( u=0.0, iU=0; iU<nuPoints; iU++, u+=du ) {
				// Start the Data at iU, so we can use the data from the previous column
				//
				if( m_uUseClosed ) {
					uEval = uKnotStart + u*( uDeltaKnot );
				}
				else {
					uEval = u;
				}
				m_evaluator.CalcNurbsBasisU( uEval, 
								&iuSpan, duBasis,
								&iuSpanEpsilon, duBasisEpsilon );

				index0 = iU;
				// Loop through the v
				//
				pvBasisTmp = pvBasis;
				pvBasisEpsilonTmp = pvBasisEpsilon;
				pviSpanTmp = pviSpan;
				pviSpanEpsilonTmp = pviSpanEpsilon;
				for( index=index0, v=0.0, iV=0; iV<nvPoints; iV++, v+=dv, index+=2,
										pvBasisTmp+=m_vOrder,
										pvBasisEpsilonTmp+=m_vOrder,
										pviSpanTmp++,
										pviSpanEpsilonTmp++ ) {

					// Evaluate the Nurbs at this point.
					//
					m_evaluator.Evaluate( 
						iuSpan, iuSpanEpsilon, 
						*pviSpanTmp, *pviSpanEpsilonTmp, 
						duBasis, duBasisEpsilon, 
						pvBasisTmp, pvBasisEpsilonTmp, 
						EvalPoint, EvalNormal, pUseTexCoord );

					// Get our Texture Coords depending on the mode.
					//
					switch( iTexCoordMode ) {
						case TCM_USE_TEXCOORD:
							uTex = EvalTexCoord[0];
							vTex = EvalTexCoord[1];
							break;
						case TCM_USE_NURBS_TEX_COORD_NODE:
							// Use our NurbsTexCoord Node to evaluate the texture coords.
							//
							if( pNurbsTextureCoord->Evaluate( u, v, EvalTexCoord ) ) {
								uTex = EvalTexCoord[0];
								vTex = EvalTexCoord[1];
							}
							else {
								// if this fails, just throw the uv TexCoords in there.
								//
								uTex = u;
								vTex = v;
							}
							break;
						default:
							uTex = u;
							vTex = v;
							break;
					}
					sv[index] = sAnmVertex( EvalPoint[0], EvalPoint[1], EvalPoint[2], 
											EvalNormal[0], EvalNormal[1], EvalNormal[2], 
											uTex, vTex );

				}
				// Add the strip, if this is not hte first v column.
				//
				if( index0 > 0 ) {
					CAnmTriangleStrip *pStrip = new CAnmTriangleStrip(pDevice);
					pStrip->AddVertices(2 * nvPoints, &sv[index0-1]);
					m_mesh->AddPrimitive(pStrip);
				}
			}

			delete [] pvBasis;
			delete [] pvBasisEpsilon;
			delete [] pviSpan;
			delete [] pviSpanEpsilon;

			bGotMesh = true;

			delete [] sv;

		}
	}
	return bGotMesh;
}









bool CAnmNurbsPatchSurface::CreateOpenMesh()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	
	bool bGotMesh = false;

	// figure out the number of tessellation Points.
	//
	int iTexCoordMode = TCM_USE_UV;
	int nuPoints;
	int nvPoints;
	if( m_uTessellation > 0 ) {
		nuPoints = ( m_uTessellation ) + 1;
	}
	else if( m_uTessellation < 0 ){
		nuPoints = ( -m_uTessellation * m_uDimension ) + 1;
	}
	else {
		nuPoints = ( 2 * m_uDimension ) + 1;
	}

	if( m_vTessellation > 0 ) {
		nvPoints = ( m_vTessellation ) + 1;
	}
	else if( m_vTessellation < 0 ){
		nvPoints = ( -m_vTessellation * m_vDimension ) + 1;
	}
	else {
		nvPoints = ( 2 * m_vDimension ) + 1;
	}

	// Set the field data to the Nurbs Evaluator
	// Probably dont need to set these if nothing changed, but, this is insignificant
	//
	m_evaluator.SetuOrder( m_uOrder );
	m_evaluator.SetuKnotVector( m_uKnot);
	m_evaluator.SetvOrder( m_vOrder );
	m_evaluator.SetvKnotVector( m_vKnot);

	m_evaluator.SetuDimension( m_uDimension );
	m_evaluator.SetvDimension( m_vDimension );

	m_evaluator.SetWeights( m_weight );

	m_evaluator.SetControlPoints( NULL );
	m_evaluator.SetControlTextureCoords( NULL );


	CAnmNurbsTextureCoordinate* pNurbsTextureCoord = NULL;

	if( m_controlPoint ) {
		CAnmNode *pImp = m_controlPoint->GetImplementationNode();
		if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate))) {
			CAnmCoordinate *pCoord = (CAnmCoordinate *) pImp;
			m_evaluator.SetControlPoints( pCoord->GetPoint() );
		}
	}
	if( m_texCoord ) {
		CAnmNode *pImp = m_texCoord->GetImplementationNode();
		if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmTextureCoordinate))) {
			CAnmTextureCoordinate *pTexCoord = (CAnmTextureCoordinate *) pImp;
			m_evaluator.SetControlTextureCoords( pTexCoord->GetPoint() );
			iTexCoordMode = TCM_USE_TEXCOORD;
		}
		else if( pImp && pImp->GetClass()->IsA(GETCLASS(CAnmNurbsTextureCoordinate))) {
			pNurbsTextureCoord = (CAnmNurbsTextureCoordinate *) pImp;
			iTexCoordMode = TCM_USE_NURBS_TEX_COORD_NODE;
		}
	}


	if( m_evaluator.ArePointsValid() && 
		nuPoints >= 2 &&
		nvPoints >= 2 ) {

		double du = 1.0 / double( nuPoints - 1 );
		double dv = 1.0 / double( nvPoints - 1 );
		double u;
		double v;

		float uTex;
		float vTex;
	
		int iV;
		int iU;
		int index = 0;
		int index0 = 0;

		float EvalPoint[3];
		float EvalNormal[3];
		float EvalTexCoord[2];

		float* pUseTexCoord = ( iTexCoordMode == TCM_USE_TEXCOORD ) ? EvalTexCoord : NULL;

		int i;
		sAnmVertex *sv = new sAnmVertex[nuPoints + 2*nvPoints + 2];		// triangle strip
		m_mesh = new CAnmMesh();
		assert(m_mesh);


		// Pre calculate the V Basis functions.
		//
		// Malloc Memory for the V Basis finction data.
		//
		double* pvBasis = new double[m_vOrder*nvPoints];
		double* pvBasisEpsilon = new double[m_vOrder*nvPoints];
		int*    pviSpan = new int[nvPoints];
		int*    pviSpanEpsilon = new int[nvPoints];

		// Fill the V Basis function Data.
		//
		double* pvBasisTmp = pvBasis;
		double* pvBasisEpsilonTmp = pvBasisEpsilon;
		int*    pviSpanTmp = pviSpan;
		int*    pviSpanEpsilonTmp = pviSpanEpsilon;
		for( iV=0, v=0.0; iV<nvPoints; iV++, v+=dv,
										pvBasisTmp+=m_vOrder,
										pvBasisEpsilonTmp+=m_vOrder,
										pviSpanTmp++,
										pviSpanEpsilonTmp++ ) {

			m_evaluator.CalcNurbsBasisV( v, 
							pviSpanTmp, pvBasisTmp,
							pviSpanEpsilonTmp, pvBasisEpsilonTmp );
		}



		double duBasis[MAXBASIS];
		double duBasisEpsilon[MAXBASIS];
		int iuSpan = 0;
		int iuSpanEpsilon = 0;

		// Loop through the u
		//
		for( u=0.0, iU=0; iU<nuPoints; iU++, u+=du ) {
			// Start the Data at iU, so we can use the data from the previous column
			//

			m_evaluator.CalcNurbsBasisU( u, 
							&iuSpan, duBasis,
							&iuSpanEpsilon, duBasisEpsilon );


			index0 = iU;
			// Loop through the v
			//
			pvBasisTmp = pvBasis;
			pvBasisEpsilonTmp = pvBasisEpsilon;
			pviSpanTmp = pviSpan;
			pviSpanEpsilonTmp = pviSpanEpsilon;

			for( index=index0, v=0.0, iV=0; iV<nvPoints; iV++, v+=dv, index+=2,
										pvBasisTmp+=m_vOrder,
										pvBasisEpsilonTmp+=m_vOrder,
										pviSpanTmp++,
										pviSpanEpsilonTmp++ ) {

				// Evaluate the Nurbs at this point.
				//
				m_evaluator.Evaluate( 
					iuSpan, iuSpanEpsilon, 
					*pviSpanTmp, *pviSpanEpsilonTmp, 
					duBasis, duBasisEpsilon, 
					pvBasisTmp, pvBasisEpsilonTmp, 
					EvalPoint, EvalNormal, pUseTexCoord );

				// Get our Texture Coords depending on the mode.
				//
				switch( iTexCoordMode ) {
					case TCM_USE_TEXCOORD:
						uTex = EvalTexCoord[0];
						vTex = EvalTexCoord[1];
						break;
					case TCM_USE_NURBS_TEX_COORD_NODE:
						// Use our NurbsTexCoord Node to evaluate the texture coords.
						//
						if( pNurbsTextureCoord->Evaluate( u, v, EvalTexCoord ) ) {
							uTex = EvalTexCoord[0];
							vTex = EvalTexCoord[1];
						}
						else {
							// if this fails, just throw the uv TexCoords in there.
							//
							uTex = u;
							vTex = v;
						}
						break;
					default:
						uTex = u;
						vTex = v;
						break;
				}
				sv[index] = sAnmVertex( EvalPoint[0], EvalPoint[1], EvalPoint[2], 
										EvalNormal[0], EvalNormal[1], EvalNormal[2], 
										uTex, vTex );

			}
			// Add the strip, if this is not hte first v column.
			//
			if( index0 > 0 ) {
				CAnmTriangleStrip *pStrip = new CAnmTriangleStrip(pDevice);
				pStrip->AddVertices(2 * nvPoints, &sv[index0-1]);
				m_mesh->AddPrimitive(pStrip);
			}
		}

		// Free up the temp V Basis memory.
		//
		delete [] pvBasis;
		delete [] pvBasisEpsilon;
		delete [] pviSpan;
		delete [] pviSpanEpsilon;


		bGotMesh = true;

		delete [] sv;

	}

	return bGotMesh;
}






bool CAnmNurbsPatchSurface::GenerateClosedArrays( bool buClosed, bool bvClosed )
{
	bool bIsOK = true;

/*****
	Point3Array					*m_ClosedControlPointArray;
	Point2Array					*m_ClosedTexCoordArray;
	DoubleArray					*m_ClosedWeight;
	Boolean						 m_uUseClosed;
	Boolean						 m_vUseClosed;
	Integer						 m_uClosedDimension;
	DoubleArray					*m_uClosedKnot;
	Integer						 m_vClosedDimension;
	DoubleArray					*m_vClosedKnot;
*********/

	m_uUseClosed = buClosed;
	m_vUseClosed = bvClosed;


	m_uKnotStart = 1.0;
	m_uKnotEnd = 0.0;
	m_vKnotStart = 1.0;
	m_vKnotEnd = 0.0;


	Point3Array* pCoordArray = NULL;
	CAnmNode *pImp;
	int nPoints = m_uDimension * m_vDimension;

	if( m_controlPoint &&
		( pImp = m_controlPoint->GetImplementationNode() ) &&
		  pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate))) {
		CAnmCoordinate *pCoord = (CAnmCoordinate *) pImp;
		pCoordArray = pCoord->GetPoint();
	}

	if( pCoordArray == NULL ||
		pCoordArray->size() != nPoints ) {
		return false;
	}
	

	int iuAddedAtEnd = 0;
	int ivAddedAtEnd = 0;

	if( buClosed ) {
		iuAddedAtEnd = m_uOrder - 1;
		m_uClosedDimension = m_uDimension + 2 * iuAddedAtEnd;
	}
	else {
		m_uClosedDimension = m_uDimension;
	}

	if( bvClosed ) {
		ivAddedAtEnd = m_vOrder - 1;
		m_vClosedDimension = m_vDimension + 2 * ivAddedAtEnd;
	}
	else {
		m_vClosedDimension = m_vDimension;
	}
	int nClosedPoints = m_uClosedDimension * m_vClosedDimension;

	// krv  Is thes right to Ref the new array?
	//
	if( m_ClosedControlPointArray == NULL ) {
		m_ClosedControlPointArray = new Point3Array;
		m_ClosedControlPointArray->Ref();
	}
	if( m_ClosedControlPointArray == NULL ) {
		return false;
	}
	m_ClosedControlPointArray->resize( nClosedPoints );



	// Setup Closed Weight Array
	//
	if( m_weight &&
		m_weight->size() == nPoints ) {

		if( m_ClosedWeight == NULL ) {
			m_ClosedWeight = new DoubleArray;
			m_ClosedWeight->Ref();
		}
		m_ClosedWeight->resize( nClosedPoints );
	}
	else {
		SafeUnRef(m_ClosedWeight);
	}



	// Create our TextureCoord array.
	//
	Point2Array* pTexCoordArray = NULL;
	if( m_texCoord &&
		( pImp = m_texCoord->GetImplementationNode() ) &&
		pImp->GetClass()->IsA(GETCLASS(CAnmTextureCoordinate))) {
		CAnmTextureCoordinate *pTexCoord = (CAnmTextureCoordinate *) pImp;
		pTexCoordArray = pTexCoord->GetPoint();
		if( pTexCoordArray &&
			pTexCoordArray->size() == nPoints ) {
			if( m_ClosedTexCoordArray == NULL ) {
				m_ClosedTexCoordArray = new Point2Array;
				m_ClosedTexCoordArray->Ref();
			}
			m_ClosedTexCoordArray->resize( nClosedPoints );
		}
		else {
			pTexCoordArray = NULL;
		}
	}
	if( pTexCoordArray == NULL ) {
		SafeUnRef(m_ClosedTexCoordArray);
	}

	// First, just copy the middle part of the array that is the same in both arrys.
	//
	int iOffSrc;
	int iOffDst;
	int ivOffDst = ivAddedAtEnd*m_uClosedDimension;
	int ivOffSrc = 0;
	for( int iv=0; iv<m_vDimension; iv++, ivOffSrc+=m_uDimension, ivOffDst+=m_uClosedDimension ) {
		for( int iu=0; iu<m_uDimension; iu++ ) {
			iOffSrc = iu + ivOffSrc;
			iOffDst = iu + ivOffDst + iuAddedAtEnd;
			(*m_ClosedControlPointArray)[iOffDst] = (*pCoordArray)[iOffSrc];
			if( m_ClosedTexCoordArray ) {
				(*m_ClosedTexCoordArray)[iOffDst] = (*pTexCoordArray)[iOffSrc];
			}
			if( m_ClosedWeight ) {
				(*m_ClosedWeight)[iOffDst] = (*m_weight)[iOffSrc];
			}

		}
	}

	// Get the indecies of the Seams.
	//
	// THis is the index in U and V of the Control Point that is the Seam between the ends of the 
	// surfaces.
	int iuSeamA = iuAddedAtEnd;
	int iuSeamB = m_uDimension + iuAddedAtEnd - 1;
	int ivSeamA = ivAddedAtEnd;
	int ivSeamB = m_vDimension + ivAddedAtEnd - 1;
	int iOffSrc1;
	int iOffDst1;
	int iOffSrc2;
	int iOffDst2;

	if( buClosed ) {
		int iv;
		int ivOffSrc;
		int ivOffDst;

		int iuSrc;
		int iuDst;
		int iExtra1;

		for( iv=0; iv<m_vClosedDimension; iv++ ) {
			ivOffDst = iv*m_uClosedDimension;
			if( iv < ivSeamA ) {
				ivOffSrc = ( ivSeamA ) * m_uClosedDimension;
			}
			else if( iv > ivSeamB ) {
				ivOffSrc = ( ivSeamB ) * m_uClosedDimension;
			}
			else {
				ivOffSrc = iv*m_uClosedDimension;
			}
			for( int iExtra=0; iExtra<iuAddedAtEnd; iExtra++ ) {

				iExtra1 = iExtra+1;
				iuSrc = iuSeamB - iExtra1;
				iuDst = iuSeamA - iExtra1;
				iOffSrc1 = iuSrc + ivOffSrc;
				iOffDst1 = iuDst + ivOffDst;

				iuSrc = iuSeamA + iExtra1;
				iuDst = iuSeamB + iExtra1;
				iOffSrc2 = iuSrc + ivOffSrc;
				iOffDst2 = iuDst + ivOffDst;
				(*m_ClosedControlPointArray)[iOffDst1] = (*m_ClosedControlPointArray)[iOffSrc1];
				(*m_ClosedControlPointArray)[iOffDst2] = (*m_ClosedControlPointArray)[iOffSrc2];
				if( m_ClosedTexCoordArray ) {
					(*m_ClosedTexCoordArray)[iOffDst1] = (*m_ClosedTexCoordArray)[iOffSrc1];
					(*m_ClosedTexCoordArray)[iOffDst2] = (*m_ClosedTexCoordArray)[iOffSrc2];
				}
				if( m_ClosedWeight ) {
					(*m_ClosedWeight)[iOffDst1] = (*m_ClosedWeight)[iOffSrc1];
					(*m_ClosedWeight)[iOffDst2] = (*m_ClosedWeight)[iOffSrc2];
				}
			}
			if( m_ClosedWeight ) {
				(*m_ClosedWeight)[iuSeamA + ivOffDst] = 1.0;
				(*m_ClosedWeight)[iuSeamB + ivOffDst] = 1.0;
			}
		}
	}



	if( bvClosed ) {
		int iu;
		int iuOffSrc;
		int iuOffDst;

		int ivSrc;
		int ivDst;
		int iExtra1;

		for( iu=0; iu<m_uClosedDimension; iu++ ) {
			iuOffDst = iu;
			if( iu < iuSeamA ) {
				iuOffSrc = ( iuSeamA );
			}
			else if( iu > iuSeamB ) {
				iuOffSrc = ( iuSeamB );
			}
			else {
				iuOffSrc = iu;
			}

			for( int iExtra=0; iExtra<ivAddedAtEnd; iExtra++ ) {
				iExtra1 = iExtra+1;
				ivSrc = ( ivSeamB - iExtra1 )* m_uClosedDimension;
				ivDst = ( ivSeamA - iExtra1 )* m_uClosedDimension;
				iOffSrc1 = ivSrc + iuOffSrc;
				iOffDst1 = ivDst + iuOffDst;

				ivSrc = ( ivSeamA + iExtra1 )* m_uClosedDimension;
				ivDst = ( ivSeamB + iExtra1 )* m_uClosedDimension;
				iOffSrc2 = ivSrc + iuOffSrc;
				iOffDst2 = ivDst + iuOffDst;
				(*m_ClosedControlPointArray)[iOffDst1] = (*m_ClosedControlPointArray)[iOffSrc1];
				(*m_ClosedControlPointArray)[iOffDst2] = (*m_ClosedControlPointArray)[iOffSrc2];
				if( m_ClosedTexCoordArray ) {
					(*m_ClosedTexCoordArray)[iOffDst1] = (*m_ClosedTexCoordArray)[iOffSrc1];
					(*m_ClosedTexCoordArray)[iOffDst2] = (*m_ClosedTexCoordArray)[iOffSrc2];
				}
				if( m_ClosedWeight ) {
					(*m_ClosedWeight)[iOffDst1] = (*m_ClosedWeight)[iOffSrc1];
					(*m_ClosedWeight)[iOffDst2] = (*m_ClosedWeight)[iOffSrc2];
				}
			}
			if( m_ClosedWeight ) {
				(*m_ClosedWeight)[ivSeamA* m_uClosedDimension + iuOffDst] = 1.0;
				(*m_ClosedWeight)[ivSeamB* m_uClosedDimension + iuOffDst] = 1.0;
			}
		}
	}

	// now do the knot vectors!
	//
	// Loop through the UV for the two knot vectors.
	//
	for( int iUV=0; iUV<2; iUV++ ) {
		bool bClosed = false;
		int nDim = 0;
		int nKnots = 0;
		int nKnotsClosed = 0;
		int iBaseKnotA = -1;
		int iBaseKnotB = -1;
		int iBaseKnotAClosed = -1;
		int iBaseKnotBClosed = -1;
		int iSeamKnotA = 0;
		int iSeamKnotB = 0;

		int iAddedAtEnd = 0;

		int iOrder = 0;
		double dSeam;

		bool bGenerateKnot = false;

		DoubleArray* pClosedKnotVector = NULL;
		DoubleArray* pKnotVector = NULL;

		if( iUV == 0 ) {
			bClosed = buClosed;
			nDim = m_uDimension;
			iOrder = m_uOrder;
			iAddedAtEnd = iuAddedAtEnd;
			if( bClosed && m_uKnot ) {
				// Create a Knot Vector Class just so we can validate out Knot Vector.
				//
				CAnmKnotVector TmpKnotVector;
				TmpKnotVector.SetOrder( iOrder );
				TmpKnotVector.SetKnotVector( m_uKnot );
				nKnots = m_uKnot->size();

				if( TmpKnotVector.IsValid( nDim, true) ) {
					bGenerateKnot = true;
					nKnotsClosed = nKnots + 2*iuAddedAtEnd;
					if( m_uClosedKnot == NULL ) {
						m_uClosedKnot = new DoubleArray;
						m_uClosedKnot->Ref();
					}
					pClosedKnotVector = m_uClosedKnot;
					pKnotVector = m_uKnot;
				}
			}
			if( !bGenerateKnot ) {
				SafeUnRef(m_uClosedKnot);
			}
		}
		else {
			bClosed = bvClosed;
			nDim = m_vDimension;
			iOrder = m_vOrder;
			iAddedAtEnd = ivAddedAtEnd;

			if( bClosed && m_vKnot ) {
				// Create a Knot Vector Class just so we can validate out Knot Vector.
				//
				CAnmKnotVector TmpKnotVector;
				TmpKnotVector.SetOrder( iOrder );
				TmpKnotVector.SetKnotVector( m_vKnot );
				nKnots = m_vKnot->size();

				if( TmpKnotVector.IsValid( nDim, true) ) {
					bGenerateKnot = true;
					nKnotsClosed = nKnots + 2*ivAddedAtEnd;
					if( m_vClosedKnot == NULL ) {
						m_vClosedKnot = new DoubleArray;
						m_vClosedKnot->Ref();
					}
					pClosedKnotVector = m_vClosedKnot;
					pKnotVector = m_vKnot;
				}
			}
			if( !bGenerateKnot ) {
				SafeUnRef(m_vClosedKnot);
			}

		}
		// Now we can generate the new knot vector, if we need to.
		//
		if( bGenerateKnot ) {
			// first find the Base Knot of the original vector.
			// That is the first knot before the knots start increasing.
			pClosedKnotVector->resize( nKnotsClosed );

			for( int i=1; i<nKnots; i++ ) {
				if( (*pKnotVector)[i] - (*pKnotVector)[i-1] > ZERO_DIFF ) {
					iBaseKnotA = i-1;
					break;
				}
			}

			for( i=nKnots-2; i>=0; i-- ) {
				if( (*pKnotVector)[i+1] - (*pKnotVector)[i] > ZERO_DIFF ) {
					iBaseKnotB = i+1;
					break;
				}
			}
			if( iBaseKnotA >= 0 && iBaseKnotB > iBaseKnotA ) {
				

				iBaseKnotAClosed = iBaseKnotA;
				iBaseKnotBClosed = iBaseKnotB + iAddedAtEnd*2;

				iSeamKnotA = iBaseKnotAClosed+iAddedAtEnd;
				iSeamKnotB = iBaseKnotBClosed-iAddedAtEnd;

				// Copy the Knots inthe middle
				//
				for( int i=iSeamKnotA; i<=iSeamKnotB; i++ ) {
					(*pClosedKnotVector)[i] = (*pKnotVector)[i-iAddedAtEnd];
				}

				// Do the new Knot Insertion.
				//

				double Delta = ( (*pClosedKnotVector)[iSeamKnotB] - (*pClosedKnotVector)[iSeamKnotA] ) / ( ( double ) ( iSeamKnotB - iSeamKnotA ) );


				(*pClosedKnotVector)[iSeamKnotA] = (*pClosedKnotVector)[iSeamKnotA+iAddedAtEnd] - Delta*iAddedAtEnd;
				(*pClosedKnotVector)[iSeamKnotB] = (*pClosedKnotVector)[iSeamKnotB-iAddedAtEnd] + Delta*iAddedAtEnd;
				for( int iExtra=0; iExtra<iAddedAtEnd; iExtra++ ) {
					int iExtra1 = iExtra+1;
					(*pClosedKnotVector)[iSeamKnotA - iExtra1] = (*pClosedKnotVector)[iSeamKnotA] - Delta*iExtra1;
					(*pClosedKnotVector)[iSeamKnotA + iExtra1] = (*pClosedKnotVector)[iSeamKnotA] + Delta*iExtra1;

					(*pClosedKnotVector)[iSeamKnotB + iExtra1] = (*pClosedKnotVector)[iSeamKnotB] + Delta*iExtra1;
					(*pClosedKnotVector)[iSeamKnotB - iExtra1] = (*pClosedKnotVector)[iSeamKnotB] - Delta*iExtra1;

				}

				// Copy the Knots Before the BaseKnotA;
				//
				for( i=0; i<iBaseKnotAClosed; i++ ) {
					(*pClosedKnotVector)[i] = (*pClosedKnotVector)[iBaseKnotAClosed];
				}

				// Copy the Knots After the BaseKnotB;
				//
				for( i=iBaseKnotBClosed+1; i<nKnotsClosed; i++ ) {
					(*pClosedKnotVector)[i] = (*pClosedKnotVector)[iBaseKnotBClosed];
				}
				
				double uvStart = 0.0;
				double uvEnd = 0.0;

				if( iOrder % 2 == 0 ) {
					uvStart = (*pClosedKnotVector)[iBaseKnotAClosed + iOrder/2];
					uvEnd = (*pClosedKnotVector)[iBaseKnotBClosed - iOrder/2];
				}
				else {
					int i1 = iOrder/2;
					int i2 = i1+1;
					uvStart = ( (*pClosedKnotVector)[iBaseKnotAClosed + i1] + 
								(*pClosedKnotVector)[iBaseKnotAClosed + i2] ) * 0.5;
					uvEnd   = ( (*pClosedKnotVector)[iBaseKnotBClosed - i1] + 
								(*pClosedKnotVector)[iBaseKnotBClosed - i2] ) * 0.5;
				}


				double kn0 = (*pClosedKnotVector)[0];
				double knf = (*pClosedKnotVector)[nKnotsClosed-1];
				double delta = knf - kn0;


				if( delta > ZERO_DIFF ) {
					uvStart = ( uvStart-kn0 ) / delta;
					uvEnd = ( uvEnd-kn0 ) / delta;

					if( iUV == 0 ) {
						m_uKnotStart = uvStart;
						m_uKnotEnd = uvEnd;
					}
					else {
						m_vKnotStart = uvStart;
						m_vKnotEnd = uvEnd;
					}
				}

/*************************************************************

  // DEBUG CODE
  // Dumps knot vector to a file, and evaluates knots
  //

	FILE* ppp=fopen( "g:/keith/keith/db.txt", "w" );
	for( i=0; i<nKnotsClosed; i++ ) {
	fprintf( ppp, "%d:  %f\n", i, (*pClosedKnotVector)[i] );
	}



	CAnmKnotVector TmpKnotVector;
	TmpKnotVector.SetOrder( iOrder );
	TmpKnotVector.SetKnotVector( pClosedKnotVector );

	for( double uuu=0; uuu<1.00001; uuu+=0.0001 ) {

		double duBasis[MAXBASIS];
		double uUse = uuu* ( TmpKnotVector.GetKnotTail() - TmpKnotVector.GetKnotHead() ) + TmpKnotVector.GetKnotHead();
		int iuspan = TmpKnotVector.FindNurbsSpan( uUse );

		TmpKnotVector.CalcNurbsBasis( iuspan, uUse, duBasis );

		fprintf( ppp, "u=%5.3f:  ", uuu );
		int iu, iu0;
		for( iu=iuspan-iOrder-1, iu0=0; iu<=iuspan; iu++, iu0++ ) {
			fprintf( ppp, "(%2d)%5.3f, ",iu, duBasis[iu0] );
		}
		fprintf( ppp, "\n" );
	}

	fclose( ppp );

*************************************************************/

			}
			else {
				pClosedKnotVector->resize( 1 );
			}
		}
	}
	return bIsOK;
}







eAnmReturnStatus CAnmNurbsPatchSurface::AddChild(CAnmNode *pChild)
{
	CAnmGeometry::AddChild(pChild);

	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_controlPoint = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmTextureCoordinate)))
		m_texCoord = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmNurbsTextureCoordinate)))
		m_texCoord = pChild;

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmNurbsPatchSurface::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_controlPoint = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmTextureCoordinate)))
		m_texCoord = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmNurbsTextureCoordinate)))
		m_texCoord = NULL;

	CAnmGeometry::RemoveChild(pChild);

	return eAnmReturnAllGood;
}

void CAnmNurbsPatchSurface::ChildChanged(CAnmNode *pChild)
{
	if (pChild == m_controlPoint)
	{
		// $$$ do we need a special dirty bit just for the control point?
		SetMatrixDirty();
	}
	else if (pChild == m_texCoord)
	{
		// $$$ do we need a special dirty bit just for the texture coordinates?
	}

	SetStateDirty();
}

// Accessors

void CAnmNurbsPatchSurface::SetControlPoint(CAnmNode *pControlPoint)
{
	if (m_controlPoint == pControlPoint)
		return;

	if (m_controlPoint)
		RemoveChild(m_controlPoint);

	if (pControlPoint == NULL)
		return;

	AddChild(pControlPoint);
	m_controlPoint = pControlPoint;

	// $$$ do we need a special dirty bit just for the control point?
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmNurbsPatchSurface, controlPoint_changed), &m_controlPoint);
}

void CAnmNurbsPatchSurface::SetTexCoord(CAnmNode *pTexCoord)
{
	if (m_texCoord == pTexCoord)
		return;

	if (m_texCoord)
		RemoveChild(m_texCoord);

	if (pTexCoord == NULL)
		return;

	AddChild(pTexCoord);
	m_texCoord = pTexCoord;

	// $$$ do we need a special dirty bit just for the texture coordinates?
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmNurbsPatchSurface, texCoord_changed), &m_texCoord);
}

void CAnmNurbsPatchSurface::SetUTessellation(Integer uTessellation)
{
	m_uTessellation = uTessellation;

	// $$$ do we need a special dirty bit just for these?
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmNurbsPatchSurface, uTessellation_changed), &m_uTessellation);
}

void CAnmNurbsPatchSurface::SetVTessellation(Integer vTessellation)
{
	m_vTessellation = vTessellation;

	// $$$ do we need a special dirty bit just for these?
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmNurbsPatchSurface, vTessellation_changed), &m_vTessellation);
}

void CAnmNurbsPatchSurface::SetWeight(DoubleArray *pWeight)
{
	assert(pWeight != NULL);

	SafeUnRef(m_weight);
	m_weight = pWeight;
	m_weight->Ref();

	SetStateDirty();	

	CallCallbacks(CALLBACKID(CAnmNurbsPatchSurface, weight_changed), &pWeight);

}

void CAnmNurbsPatchSurface::SetUClosed(Boolean uClosed)
{
	m_uClosed = uClosed;
}

void CAnmNurbsPatchSurface::SetUDimension(Integer uDimension)
{
	m_uDimension = uDimension;
}

void CAnmNurbsPatchSurface::SetUKnot(DoubleArray *pUknot)
{
	assert(pUknot != NULL);

	SafeUnRef(m_uKnot);
	m_uKnot = pUknot;
	m_uKnot->Ref();

	SetStateDirty();
}

void CAnmNurbsPatchSurface::SetUOrder(Integer uOrder)
{
	m_uOrder = uOrder;
}

void CAnmNurbsPatchSurface::SetVClosed(Boolean vClosed)
{
	m_vClosed = vClosed;
}

void CAnmNurbsPatchSurface::SetVDimension(Integer vDimension)
{
	m_vDimension = vDimension;
}

void CAnmNurbsPatchSurface::SetVKnot(DoubleArray *pVknot)
{
	assert(pVknot != NULL);

	SafeUnRef(m_vKnot);
	m_vKnot = pVknot;
	m_vKnot->Ref();

	SetStateDirty();
}

void CAnmNurbsPatchSurface::SetVOrder(Integer vOrder)
{
	m_vOrder = vOrder;
}


// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmNurbsPatchSurface, controlPoint, eValueNode, CAnmNode*, GetControlPoint, SetControlPoint);
DEFINE_VALUE(CAnmNurbsPatchSurface, texCoord, eValueNode, CAnmNode *, GetTexCoord, SetTexCoord);
DEFINE_VALUE(CAnmNurbsPatchSurface, uTessellation, eValueInteger, Integer, GetUTessellation, SetUTessellation);
DEFINE_VALUE(CAnmNurbsPatchSurface, vTessellation, eValueInteger, Integer, GetVTessellation, SetVTessellation);
DEFINE_VALUE(CAnmNurbsPatchSurface, weight, eValueDoubleArray, DoubleArray*, GetWeight, SetWeight);
DEFINE_INHERITED_VALUE(CAnmNurbsPatchSurface, solid, CAnmGeometry);
DEFINE_VALUE(CAnmNurbsPatchSurface, uClosed, eValueBoolean, Boolean, GetUClosed, SetUClosed);
DEFINE_VALUE(CAnmNurbsPatchSurface, uDimension, eValueInteger, Integer, GetUDimension, SetUDimension);
DEFINE_VALUE(CAnmNurbsPatchSurface, uKnot, eValueDoubleArray, DoubleArray*, GetUKnot, SetUKnot);
DEFINE_VALUE(CAnmNurbsPatchSurface, uOrder, eValueInteger, Integer, GetUOrder, SetUOrder);
DEFINE_VALUE(CAnmNurbsPatchSurface, vClosed, eValueBoolean, Boolean, GetVClosed, SetVClosed);
DEFINE_VALUE(CAnmNurbsPatchSurface, vDimension, eValueInteger, Integer, GetVDimension, SetVDimension);
DEFINE_VALUE(CAnmNurbsPatchSurface, vKnot, eValueDoubleArray, DoubleArray*, GetVKnot, SetVKnot);
DEFINE_VALUE(CAnmNurbsPatchSurface, vOrder, eValueInteger, Integer, GetVOrder, SetVOrder);

DEFINE_METHOD(CAnmNurbsPatchSurface, set_controlPoint, eValueNode, CAnmNode*, SetControlPoint);
DEFINE_METHOD(CAnmNurbsPatchSurface, set_texCoord, eValueNode, CAnmNode*, SetTexCoord);
DEFINE_METHOD(CAnmNurbsPatchSurface, set_uTessellation, eValueInteger, Integer, SetUTessellation);
DEFINE_METHOD(CAnmNurbsPatchSurface, set_vTessellation, eValueInteger, Integer, SetVTessellation);
DEFINE_METHOD(CAnmNurbsPatchSurface, set_weight, eValueDoubleArray, DoubleArray*, SetWeight);

DEFINE_CALLBACK(CAnmNurbsPatchSurface, controlPoint_changed, eValueNode);
DEFINE_CALLBACK(CAnmNurbsPatchSurface, texCoord_changed, eValueNode);
DEFINE_CALLBACK(CAnmNurbsPatchSurface, uTessellation_changed, eValueInteger);
DEFINE_CALLBACK(CAnmNurbsPatchSurface, vTessellation_changed, eValueInteger);
DEFINE_CALLBACK(CAnmNurbsPatchSurface, weight_changed, eValueDoubleArray);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmNurbsPatchSurface)
VALUEID(CAnmNurbsPatchSurface, controlPoint),
VALUEID(CAnmNurbsPatchSurface, texCoord),
VALUEID(CAnmNurbsPatchSurface, uTessellation),
VALUEID(CAnmNurbsPatchSurface, vTessellation),
VALUEID(CAnmNurbsPatchSurface, weight),
VALUEID(CAnmNurbsPatchSurface, solid),
VALUEID(CAnmNurbsPatchSurface, uClosed),
VALUEID(CAnmNurbsPatchSurface, uDimension),
VALUEID(CAnmNurbsPatchSurface, uKnot),
VALUEID(CAnmNurbsPatchSurface, uOrder),
VALUEID(CAnmNurbsPatchSurface, vClosed),
VALUEID(CAnmNurbsPatchSurface, vDimension),
VALUEID(CAnmNurbsPatchSurface, vKnot),
VALUEID(CAnmNurbsPatchSurface, vOrder),

METHODID(CAnmNurbsPatchSurface, set_controlPoint), 
METHODID(CAnmNurbsPatchSurface, set_texCoord), 
METHODID(CAnmNurbsPatchSurface, set_uTessellation), 
METHODID(CAnmNurbsPatchSurface, set_vTessellation),
METHODID(CAnmNurbsPatchSurface, set_weight),

CALLBACKID(CAnmNurbsPatchSurface, controlPoint_changed),
CALLBACKID(CAnmNurbsPatchSurface, texCoord_changed),
CALLBACKID(CAnmNurbsPatchSurface, uTessellation_changed),
CALLBACKID(CAnmNurbsPatchSurface, vTessellation_changed),
CALLBACKID(CAnmNurbsPatchSurface, weight_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmNurbsPatchSurface, CAnmGeometry);

