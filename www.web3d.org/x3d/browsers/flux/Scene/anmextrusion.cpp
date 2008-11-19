/********************************************************************************
 * Flux
 *
 * File: anmextrusion.cpp
 * Description: Extrusion node
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
#include "anmextrusion.h"
#include "anmdevice.h"
#include "anmmesh.h"
#include "anmprimitive.h"
#include "anmtesselator.h"
#include "anmworld.h"


#define MAX_VERTS_IN_INDEXED_TRI_LIST		( 64*1024 )

#define ZeroExt		( 0.0001 )


CAnmExtrusion::CAnmExtrusion() : CAnmGeometry(),
	m_beginCap(ANMEXTRUSION_DEFAULT_BEGINCAP),
	m_ccw(ANMEXTRUSION_DEFAULT_CCW),
	m_convex(ANMEXTRUSION_DEFAULT_CONVEX),
	m_creaseAngle(ANMEXTRUSION_DEFAULT_CREASEANGLE),
	m_endCap(ANMEXTRUSION_DEFAULT_ENDCAP)
{
	// Create defaults for all fields
	m_crossSection = new Point2Array;
	m_crossSection->push_back(Point2(1, 1));
	m_crossSection->push_back(Point2(1, -1));
	m_crossSection->push_back(Point2(-1, -1));
	m_crossSection->push_back(Point2(-1, 1));
	m_crossSection->push_back(Point2(1, 1));

	m_orientation = new RotationArray;
	m_orientation->push_back(Rotation(0, 0, 1, 0));

	m_scale = new Point2Array;
	m_scale->push_back(Point2(1, 1));

	m_spine = new Point3Array;
	m_spine->push_back(Point3(0, 0, 0));
	m_spine->push_back(Point3(0, 1, 0));
}

CAnmExtrusion::~CAnmExtrusion()
{
	SafeUnRef(m_crossSection);
	SafeUnRef(m_orientation);
	SafeUnRef(m_scale);
	SafeUnRef(m_spine);
}

void CAnmExtrusion::Update()
{
	// N.B.: worth the extra check here?
	if (!TestDirtyBits(eAnmNodeStateDirty | eAnmNodeMatrixDirty))
		return;

	SafeUnRef(m_mesh);

	CreateMesh();
	GenBoundingVolumes();

	ClearStateDirty();
}






#define SETINDEX( iLocInd, idx, idy ) { \
	iFace = iLocToFace[idx][idy+1]; \
	ivert0 = iBase + idx + idy*crosssectionsize; \
	iSmoothOffset = pNormalIndexes[ivert0*5 + 1 + iFace]; \
	assert( iSmoothOffset>= 0 ); \
	if( idy == 0 ) { \
		indices[iLocInd] = piFirstVertexIndexOfVertThis[iCsx+idx] + iSmoothOffset; \
	} \
	else { \
		indices[iLocInd] = piFirstVertexIndexOfVertLast[iCsx+idx] + iSmoothOffset; \
	} \
}





void CAnmExtrusion::CreateMesh()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);


	int i,j, iBase;
	// Some constants
	const Point3 defYAxis(0, 1, 0);
	const Point3 defZAxis(0, 0, 1);
	const Point3 origin(0, 0, 0);

	// Sanity checks
	assert(m_crossSection);
	assert(m_orientation);
	assert(m_scale);
	assert(m_spine);

	// Test for valid field values
	int spinesize = m_spine->size();
	if(spinesize < 2)
		return;					// what can we do?

	int scalesize = m_scale->size();
	if(scalesize < 1)
		return;					// what can we do?

	int orientationsize = m_orientation->size();
	if(orientationsize < 1)
		return;					// what can we do?

	int crosssectionsize = m_crossSection->size();
	if(crosssectionsize < 2)
		return;					// what can we do?

	// Compute Spine-aligned cross-section plane (SCP) values
	// krv:  get the SCP Matricies;
	//
	matrix4* pSCPMatrixes = GenerateSPCMatrixArray();
	if( !pSCPMatrixes ) {
		return;					// what can we do?
	}

	// Compute Verts coords, so we dont have to redo the Mastrix Multilications
	//
	Point3* pVertexCoords = GenerateVertexCoords( pSCPMatrixes );
	if( !pVertexCoords ) {
		return;					// what can we do?
	}

	// Compute Normals
	//
	int* pNormalIndexes = NULL;
	Point3* pNormals = ComputeNormals( pVertexCoords, &pNormalIndexes );
	if( !( pNormals && pNormalIndexes ) ) {
		return;					// what can we do?
	}


	// Compute Body Texture Coords
	//
	float* pUCsxTexCoords = ComputeCsxTextureCoords();
	float* pVSpineTexCoords = ComputeSpineTextureCoords();


	int nQuads = (  spinesize-1 ) * (  crosssectionsize-1 );

	// there might be 4 Verts in the VertexArray per actual Vert.
	//

	// Create mesh
	m_mesh = new CAnmMesh();

	float uTex, vTex;
	Point2 pnt2;
	Point3 pnt3;

	sAnmVertex* pVert;

	sAnmVertex* pXSectVerts = new sAnmVertex[crosssectionsize*4];
	int* piFirstVertexIndexOfVert0 = new int[crosssectionsize*2];


	int* piFirstVertexIndexOfVertThis = piFirstVertexIndexOfVert0;
	int* piFirstVertexIndexOfVertLast = piFirstVertexIndexOfVert0 + crosssectionsize;


	CAnmIndexedTriangleList* pTriangleList = new CAnmIndexedTriangleList(pDevice);
	tAnmVertexArray *pVerts = pTriangleList->GetVertices();
	tAnmIndexArray *pIndices = pTriangleList->GetIndices();

	WORD indices[6];

	Point2* pUVCoords = NULL;
	// First, we generate the U and V Texture Coords for the Caps.
	//
	if( m_beginCap || m_endCap ) {
		// we need the min and max vals of the xscetion to generate our tex coords.
		//
		float xMin, xMax, yMin, yMax, xDelta, yDelta;
		pnt2 = (*m_crossSection)[0];
		xMin = xMax = pnt2.x;
		yMin = yMax = pnt2.y;
		for( int iCsx=1; iCsx<crosssectionsize; iCsx++ ) {
			pnt2 = (*m_crossSection)[iCsx];
			xMin = min( xMin, pnt2.x );
			xMax = max( xMax, pnt2.x );
			yMin = min( yMin, pnt2.y );
			yMax = max( yMax, pnt2.y );
		}
		xDelta = xMax - xMin;
		yDelta = yMax - yMin;

		if( xDelta < 0.000000001 ) {
			xDelta = 1.0;
		}
		if( yDelta < 0.000000001 ) {
			yDelta = 1.0;
		}


		pUVCoords = new Point2[crosssectionsize];
		for( iCsx=0; iCsx<crosssectionsize; iCsx++ ) {
			pnt2 = (*m_crossSection)[iCsx];
			pUVCoords[iCsx].x = ( pnt2.x - xMin ) / xDelta;
			pUVCoords[iCsx].y = ( pnt2.y - yMin ) / yDelta;
		}
	}


	int iFace, ivert0, iSmoothOffset;

	int iLocToFace[2][2];
	iLocToFace[0][0] = 0;
	iLocToFace[1][0] = 3;
	iLocToFace[0][1] = 1;
	iLocToFace[1][1] = 2;



	int index, index4, index5, nSmooth, iSmooth;

	Point3* pTmpCoord = pVertexCoords;
	int iVertsAddedInThisList = 0;
	int iSpinesInThisList = 0;

	for( int iSpn=0; iSpn<spinesize; iSpn++ ) {
		vTex = pVSpineTexCoords[iSpn];
		// Calc the ring of Verts for the xsection
		//
		int iVertsAddedInThisXSect = 0;
		for( int iCsx=0; iCsx<crosssectionsize; iCsx++, pTmpCoord++ ) {
			uTex = pUCsxTexCoords[iCsx];
			piFirstVertexIndexOfVertThis[iCsx] = iVertsAddedInThisList + iVertsAddedInThisXSect;
			Point2 UVTexCoord( uTex, vTex );
			index = iSpn*crosssectionsize + iCsx;
			index4 = index*4;
			index5 = index*5;
			nSmooth = pNormalIndexes[index5];
			for( iSmooth=0; iSmooth<nSmooth; iSmooth++ ) {
				pVert = &pXSectVerts[iVertsAddedInThisXSect];
				pVert->vec = *pTmpCoord;
				pVert->texCoord = UVTexCoord;
				pVert->normal = pNormals[index4+iSmooth];
				iVertsAddedInThisXSect++;
			}
		}

		pTriangleList->AddVertices(iVertsAddedInThisXSect, pXSectVerts );
		// Lay ing the Indecies
		//
		if( iSpinesInThisList > 0 ) {
			for( int iCsx=0; iCsx<crosssectionsize-1; iCsx++ ) {
				iBase = iCsx + iSpn*crosssectionsize;

				if( m_ccw ) {
					SETINDEX( 0, 0, -1 );
					SETINDEX( 1, 1, -1 );
					SETINDEX( 2, 0, 0 );

					SETINDEX( 3, 0, 0 );
					SETINDEX( 4, 1, -1 );
					SETINDEX( 5, 1, 0 );
				}
				else {
					SETINDEX( 1, 0, -1 );
					SETINDEX( 0, 1, -1 );
					SETINDEX( 2, 0, 0 );

					SETINDEX( 4, 0, 0 );
					SETINDEX( 3, 1, -1 );
					SETINDEX( 5, 1, 0 );
				}

				pTriangleList->AddIndices(6, indices);
			}
		}
		iVertsAddedInThisList += iVertsAddedInThisXSect;
		iSpinesInThisList++;


		// Flip flop first and last
		int* piTmp = piFirstVertexIndexOfVertThis;
		piFirstVertexIndexOfVertThis = piFirstVertexIndexOfVertLast;
		piFirstVertexIndexOfVertLast = piTmp;

		// check if we have enough room in our current TriangleList for another
		// cross section of verts.  Worst case is 4 verts per coord.
		//

		if( iVertsAddedInThisList + crosssectionsize*4 > MAX_VERTS_IN_INDEXED_TRI_LIST ) {
			AddTriListToMesh( pTriangleList, false, true );



			bool bThisIsTheLastSpine = ( iSpn == spinesize-1 );


			// add the last ring of verts to our new if we are not done.
			// 
			if( !bThisIsTheLastSpine ) {
				// generate a new tri list.
				//
				pTriangleList = new CAnmIndexedTriangleList(pDevice);
				pVerts = pTriangleList->GetVertices();
				pIndices = pTriangleList->GetIndices();
				pTriangleList->AddVertices(iVertsAddedInThisXSect, pXSectVerts );
				iVertsAddedInThisList = iVertsAddedInThisXSect;

				// Adjust our IndexOfFirstVert array so the first value is zero.
				//
				int iOff = piFirstVertexIndexOfVertLast[0];
				for( int i=0; i<crosssectionsize; i++ ) {
					piFirstVertexIndexOfVertLast[i] -= iOff;
				}
			}
			else {
				pTriangleList = NULL;
			}
		}
	}

	if( pTriangleList && pIndices->size() > 0 ) {
		AddTriListToMesh( pTriangleList, false, true );
	}


	// now we can add the caps
	//
	//	Just passing in pXSectVerts as a work space.  Not using any values.
	//
	if( m_beginCap ) {
		AddCap( false, &pVertexCoords[0],
			pUVCoords, crosssectionsize, pXSectVerts );
	}
	if( m_endCap ) {
		AddCap( true,  &pVertexCoords[(spinesize-1)*crosssectionsize], 
			pUVCoords, crosssectionsize, pXSectVerts );
	}


	delete [] pUCsxTexCoords;
	delete [] pVSpineTexCoords;

	delete [] piFirstVertexIndexOfVert0;
	delete [] pVertexCoords;
	delete [] pSCPMatrixes;
	delete [] pNormals;
	delete [] pXSectVerts;
	delete [] pUVCoords;
}



void CAnmExtrusion::AddTriListToMesh( CAnmTriangleList *pTriangleList, bool bGenNormals, bool bBufferVerts )
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);


	// Add the grid to our mesh
	m_mesh->AddPrimitive(pTriangleList);

	// Generate normals if they weren't supplied
	eAnmReturnStatus retval;
	if( bGenNormals )
	{
		pTriangleList->GenerateNormals(m_creaseAngle);
	}

	// Generate backfaces if the thing isn't solid
	if (!m_solid) {
		pTriangleList->GenerateBackfaces(pDevice);
	}

	if( bBufferVerts ) {
	// Try to buffer the vertex list in hardware; may or may not succeed but ok either way
		pTriangleList->BufferVertices(pDevice);
	}

	// Sort the grid for picking
	// N.B.: don't do this for now, we're blowing up something with a stack overflow
	// pTriangleList->Sort();
}




int *CAnmExtrusion::Tesselate(int vertexcount, sAnmVertex *pVerts, int &numIndices)
{
	// copy vertices 1 for 1 into new array
	std::vector<double> coords;
	for (int i = 0; i < vertexcount; i++)
	{
		coords.push_back((double) pVerts[i].vec.x);
		coords.push_back((double) pVerts[i].vec.y);
		coords.push_back((double) pVerts[i].vec.z);
	}

	int nindices = numIndices;

	// tesselate; this will create a new index array and update the count
	CAnmTesselator t;
	int *newindices = t.Tesselate(&coords[0], nindices);

	numIndices = nindices;
	return nindices ? newindices : NULL;
}


void CAnmExtrusion::AddCap( bool bFlip, Point3* pVertCoord, Point2* pUVCoords, int ncapverts, sAnmVertex *capverts )
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);


	if( m_ccw ) {
		bFlip = !bFlip;
	}

	// add begin or end cap. vertices are passed in; this just deals
	// with creating either a simple fan or fancy tesselated thingy
	// if the face isn't convex
	if( bFlip ) {
		for( int i=0; i<ncapverts; i++ ) {
			capverts[i].vec      = pVertCoord[ncapverts-i-1];
			capverts[i].texCoord = pUVCoords[ncapverts-i-1];
		}
	}
	else {
		for( int i=0; i<ncapverts; i++ ) {
			capverts[i].vec      = pVertCoord[i];
			capverts[i].texCoord = pUVCoords[i];
		}
	}

	CAnmPrimitive *pPrim = NULL;
	if (m_convex)
	{
		CAnmTriangleFan *pFan = new CAnmTriangleFan(pDevice);
		pFan->AddVertices(ncapverts, capverts );
		pPrim = pFan;
	}
	else
	{
		CAnmIndexedTriangleList *pCapList = new CAnmIndexedTriangleList(pDevice);
		pCapList->AddVertices(ncapverts, &capverts[0]);

		// Tesselate the thing
		int nindices = ncapverts;
		int *indices = Tesselate(ncapverts, &capverts[0], nindices);

		if (indices)
		{
			for (int k = 0; k < nindices; k++)
			{
				WORD index = indices[k];
				pCapList->AddIndices(1, &index);
			}
			delete [] indices;
		}

		pPrim = pCapList;
	}



	pPrim->GenerateNormals(0.f);

	if (!m_solid) {
		pPrim->GenerateBackfaces(pDevice);
	}

	m_mesh->AddPrimitive(pPrim);
}

static int Decriment4( int iThis )
{
	int iOut = iThis - 1;
	if( iOut < 0 ) {
		iOut=3;
	}
	return iOut;
}




static int WrapIndex( int iii, bool bClosed, int nCount )
{
	int iRtn = iii;
	if( bClosed ) {
		if( iii<0 ) {
			iRtn=nCount-2;
		}
		else if( iii>=nCount ) {
			iRtn=1;
		}
	}
	return iRtn;
}



Point3* CAnmExtrusion::ComputeNormals( Point3* pVertCoords, int **ppIndexes )
{
	Point3* pNewNormals = NULL;


	int nSpines = m_spine->size();
	int nCsx = m_crossSection->size();
	int nVerts = nSpines * nCsx;

	bool bSpineIsClosed = ( nSpines > 2 && ((*m_spine)[nSpines - 1] == (*m_spine)[0]) );

	Point2 p21 = (*m_crossSection)[nCsx - 1];
	Point2 p22 = (*m_crossSection)[0];
	bool bSectionIsClosed = (p21.x == p22.x && p21.y == p22.y);


	// krv
	// THis is a multipass approach as calculating the normslas
	// We go through each vertex, and calc four normals ( or less for edge cases )
	// We get one normal for each face that touches the vertex
	/************

	The Face and Edge emueration looks like this:

	Face Enumeration:

        i, z
        ^
        |
      3 | 0 
    ----o-----> j, x
      2 | 1
        |
        
Edge / Adjecent Vertex Enumeration

        0
        |
        |
  3 ----o----- 1
        |
        |
        2

  **************/



	Point3 *pGeneratedNormalValues = new Point3[4 * nVerts];
	int* pNormalIndex = new int[5 * nVerts];

	// ofsets for i and j to get us to the adjacent vertex,
	
	int jxOff[4] = { 0, 1, 0, -1 };
	int izOff[4] = { 1, 0, -1, 0 };
	int jxKittyCornerOff[4] = { 1,  1, -1, -1 };
	int izKittyCornerOff[4] = { 1, -1, -1,  1 };
	int iKitty, jKitty;


	Point3 FaceNormals[4];
	Point3 TmpVerts[4];
	int iFace, iNext, iii, jjj, index, iVertIndex, iEdge, iNextVert, iThisVert;
	int iPrevFace, iNextFace, iSmooth;
	float x, y, z, theDot;
	float crease_dot = cos( m_creaseAngle ) - 0.01;

	Point3 SmoothingNormals;
	Point3 DefNorm( 0, 1, 0 );
	float d0, d1, d2, dmin, dmax;
	// Loop thought each Vertex, I and J.
	//
	for (int i = 0; i < nSpines; i++)
	{
		for (int j = 0; j < nCsx; j++)
		{

			// start the passes for each vertex.
			//

			bool bGotFace[4] = { false, false, false, false };
			bool bGotVert[4] = { false, false, false, false };
			bool bGotEdge[4] = { false, false, false, false };
			bool bSmoothEdge[4] = { false, false, false, false };
			int iSmoothingGroup[4] = { -1, -1, -1, -1 };

			index = i * nCsx + j;

			Point3 PntCenter( pVertCoords[index] );
			// First calc all four adjacent verts coords
			//
			for( iEdge=0; iEdge<4; iEdge++ ) {
				// Get index of Adjecent Vertex
				// if we are closed then wrap
				//
				iii = WrapIndex( i + izOff[iEdge], bSpineIsClosed, nSpines );
				jjj = WrapIndex( j + jxOff[iEdge], bSectionIsClosed, nCsx );

				if( iii >= 0 && iii < nSpines &&
					jjj >= 0 && jjj < nCsx ) {

					bGotVert[iEdge] = true;
					iVertIndex = iii * nCsx + jjj;
					TmpVerts[iEdge] = pVertCoords[iVertIndex];
				}
			}

			// Now calc the four face normals
			//
			for( iFace=0; iFace<4; iFace++ ) {
				iThisVert = iFace;
				iNextVert = ( iFace+1 ) %4;
				if( bGotVert[iThisVert] && bGotVert[iNextVert] ) {
					bGotFace[iFace] = true;
					// Check for degenerate triangle, which will yield a bad normal.
					//
					d0 = ( TmpVerts[iThisVert] - PntCenter ).Mag();
					d1 = ( TmpVerts[iNextVert] - PntCenter ).Mag();
					d2 = ( TmpVerts[iThisVert] - TmpVerts[iNextVert] ).Mag();
					dmin = min( d0, min( d1, d2 ) );
					dmax = max( d0, max( d1, d2 ) );
					if( dmax > 1.0e-10 && ( dmin / dmax < .01 ) ) {
						// The aspect ratio of the triangle is bad.
						// Get the Kitty Corner Vertex, and use that as the middle point
						// for our cross product.
						//  If this is also degnerate, then the entire quad is degenerate, so it does not matter.
						//
						iKitty = WrapIndex( i + izKittyCornerOff[iFace], bSpineIsClosed, nSpines );
						jKitty = WrapIndex( j + jxKittyCornerOff[iFace], bSectionIsClosed, nCsx );
						plane3 plane(
							TmpVerts[iNextVert], 
							pVertCoords[ iKitty * nCsx + jKitty ],
							TmpVerts[iThisVert] );
						FaceNormals[iFace] = plane.n;
					}
					else {
						plane3 plane(
							TmpVerts[iThisVert], 
							PntCenter,
							TmpVerts[iNextVert] );
						FaceNormals[iFace] = plane.n;
					}
				}
			}


			// Now get the dot product of adjacent face normals.
			// Determine if the Edge should be smoothed out.
			//
			for( iEdge=0; iEdge<4; iEdge++ ) {
				iPrevFace = Decriment4( iEdge );
				iNextFace = iEdge;
				if( bGotFace[iPrevFace] && bGotFace[iNextFace] ) {
					bGotEdge[iEdge] = true;

					theDot = FaceNormals[iPrevFace] * FaceNormals[iNextFace];
					bSmoothEdge[iEdge] = ( theDot > crease_dot );
				}
			}

			// Now group the Faces into Smoothing Groups
			//
			int iNextGroup = 0;

			// Now calc the Smoothing Groups
			//
			for( iFace=0; iFace<4; iFace++ ) {
				if( bGotFace[iFace] ) {
					if( iNextGroup == 0 ) {
						// first one, just set it.
						//
						iSmoothingGroup[iFace] = iNextGroup++;
					}
					else {
						// Determine if we are smooth with Previous Face;
						//
						if( bGotEdge[iFace] && bSmoothEdge[iFace] ) {
							// Merge with Previous Group
							//
							iSmoothingGroup[iFace] = iNextGroup-1;
						}
						else {
							// Create new group
							//
							iSmoothingGroup[iFace] = iNextGroup++;
						}
					}
				}
			}

			// now test for smoothness between first and last.
			//
			if( bGotEdge[0] && bSmoothEdge[0] && iNextGroup > 1 ) {
				// merge the first and last, and any face with in that group.
				//
				iNextGroup--;
				for( iFace=0; iFace<4; iFace++ ) {
					if( iSmoothingGroup[iFace] == iNextGroup ) {
						iSmoothingGroup[iFace] = 0;
					}
				}
			}

			//now, get the normal for each Smoothing Group.
			//
			int index4 = index*4;
			int index5 = index*5;
			for( iSmooth=0; iSmooth<iNextGroup; iSmooth++ ) {
				SmoothingNormals = Point3::Zero;
				for( iFace=0; iFace<4; iFace++ ) {
					if( iSmoothingGroup[iFace] == iSmooth ) {
						SmoothingNormals += FaceNormals[iFace];
					}
				}
				SmoothingNormals.Normalize();
				pGeneratedNormalValues[index4+iSmooth] = ( ( m_ccw ) ? SmoothingNormals : ( -SmoothingNormals ) );
			}

			pNormalIndex[index5] = iNextGroup;
			for( iFace=0; iFace<4; iFace++ ) {
				pNormalIndex[index5+1+iFace] = iSmoothingGroup[iFace];
			}
		}
	}

	*ppIndexes = pNormalIndex;
	return pGeneratedNormalValues;
}




// Just generate the VertexCoords
//
Point3* CAnmExtrusion::GenerateVertexCoords( matrix4* pSPCMatrixArray )
{

	int nSpines = m_spine->size();
	int nCsx = m_crossSection->size();
	int nVerts = nSpines * nCsx;
	Point3* pNewVertArray = new Point3[nVerts];
	Point3* pVertTmp = pNewVertArray;
	Point3 pnt3;
	Point2 pnt2;


	for( int isp = 0; isp < nSpines; isp++, pSPCMatrixArray++ ) {
		for( int iCsx = 0; iCsx < nCsx; iCsx++ ) {

			pnt2 = (*m_crossSection)[iCsx];
			pnt3.x = pnt2.x;
			pnt3.y = 0.0;
			pnt3.z = pnt2.y;
			*(pVertTmp++) = ( (*pSPCMatrixArray)*pnt3 );
		}
	}

	return pNewVertArray;
}




// krv:  Gernate the SCP matricies:
// ( Spline Aligned Cross Section Plane )
//
// THis is adopted from the Extrusion Proto that I wrote for Don/NPS.
//
matrix4* CAnmExtrusion::GenerateSPCMatrixArray()
{
	// we already did out sainity checks.
	// no need to test again.

	int nSpines = m_spine->size();
	matrix4* pMatrixes = new matrix4[nSpines];

	// Allocate tmp vectors:
	//
	Point3* pSCPAxisY = new Point3[nSpines];
	Point3* pSCPAxisZ = new Point3[nSpines];

	int scalesize = m_scale->size();
	int orienationsize = m_orientation->size();

	int iLastValidYAxis = -1;
	int iLastValidZAxis = -1;

	bool bSpineIsClosed = ( nSpines > 2 && ((*m_spine)[nSpines - 1] == (*m_spine)[0]) );

	// Initialize our SCP vectors.
	for ( int isp = 0; isp < nSpines ; isp++ ) {
		pSCPAxisY[isp] = Point3::j;
		pSCPAxisZ[isp] = Point3::k;

		pMatrixes[isp] = matrix4::Identity;
	}


	// Our first time we loop through the Spine Vertebrs, we are just calulating the 
	// SCP vectors, SCPYAxisArray, and SCPZAxisArray.
	// Along the way, we are keeping track of iLastValidYAxis and iLastValidZAxis.
	for ( isp = 0; isp < nSpines ; isp++ ) {

		bool bGotYAxis = false;
		bool bGotZAxis = false;

		Point3 SCPyAxis( Point3::j );
		Point3 SCPzAxis( Point3::k );

		// so far, these crossSection faces are perpendicular to x axis.
		// must align each crossSection with spine-aligned cross-section plane (SCP),
		// which bisects incoming and outgoing spine tangents at each control point
		if ( ( isp == 0 ) && (bSpineIsClosed) ) { 
			// first, closed
			SCPyAxis = ( (*m_spine)[ 1 ] - (*m_spine)[ nSpines-2 ] );
			if( SCPyAxis.Mag() > ZeroExt ) {
				bGotYAxis = true;
				SCPzAxis = ( (*m_spine)[ 1 ] - ( (*m_spine)[ 0 ]) ) ^ ( (*m_spine)[ nSpines-2 ] - ( (*m_spine)[ 0 ] ) );
				if( SCPzAxis.Mag() > ZeroExt  ) {
					bGotZAxis = true;
				} 
			}
		}
		else if ( isp == 0 ) {
			// first, open
			SCPyAxis = ((*m_spine)[ 1 ] - ((*m_spine)[ 0 ]));
			if( SCPyAxis.Mag() > ZeroExt ) {
				bGotYAxis = true;
			}
		}
		else if ((isp == nSpines-1) && (bSpineIsClosed)) {
			// last, closed
			SCPyAxis = ((*m_spine)[ 1 ] - ((*m_spine)[ nSpines-2 ]));
			if( SCPyAxis.Mag() > ZeroExt ) {
				bGotYAxis = true;
				SCPzAxis = ((*m_spine)[ 1 ] - ((*m_spine)[ 0 ])) ^ ((*m_spine)[ nSpines-2 ] - ((*m_spine)[ 0 ]));
				if( SCPzAxis.Mag() > ZeroExt ) {
					bGotZAxis = true;
				} 
			}
		}
		else if ( ( isp == nSpines-1) ) {
			// last, open
			SCPyAxis = ((*m_spine)[ nSpines-1 ] - ((*m_spine)[ nSpines-2 ]));
			if( SCPyAxis.Mag() > ZeroExt ) {
				bGotYAxis = true;
			}
		}
		else {
			// intermediate Spine
			SCPyAxis = ((*m_spine)[ isp+1 ] - ((*m_spine)[ isp-1 ]));
			if( SCPyAxis.Mag() > ZeroExt ) {
				bGotYAxis = true;

				SCPzAxis = ((*m_spine)[ isp+1 ] - ((*m_spine)[ isp ])) ^ ((*m_spine)[ isp-1 ] - ((*m_spine)[ isp ]));
				if( SCPzAxis.Mag() > ZeroExt  ) {
					bGotZAxis = true;
				} 
			}
		}
		

		// OK, Calculated our Y and Z vectors. IF we could.
		// Now deal with the specail cases if we could not generate the vectors.



		 // If we could not calulate a Y axis, use the last valid one, if it exists.
		 //
		if( !bGotYAxis && iLastValidYAxis >= 0 ) {
			SCPyAxis = pSCPAxisY[iLastValidYAxis];
			bGotYAxis = true;
		}

		// If we could not calulate a Z axis, use the last valid one, if it exists.
		//
		if( bGotYAxis && !bGotZAxis && iLastValidZAxis >= 0 ) {
			SCPzAxis = pSCPAxisZ[iLastValidZAxis];
			bGotZAxis = true;
		}

		// If we have a previous Z axis, and a current Z axis, do the Z axis flip flop test.
		//     
		if( bGotZAxis && iLastValidZAxis >= 0 ) {
			// make sure the dot product is Pos.
			//
			if( ( SCPzAxis * pSCPAxisZ[iLastValidZAxis] ) < 0.0 ) {
				SCPzAxis = -SCPzAxis;
			}
		}

		// If we have a Y axis, then make update any previously undefined Y axis
		//
		if( isp > 0 && bGotYAxis && iLastValidYAxis < isp-1 ) {
			for( int iY=iLastValidYAxis+1; iY<isp; iY++ ) {
				pSCPAxisY[iY] = SCPyAxis;
			}
			iLastValidYAxis = isp-1;
		}

		// If we have a Z axis, then make update any previously undefined Z axis
		//
		if( isp > 0 && bGotZAxis && iLastValidZAxis < isp-1 ) {
			for( int iZ=iLastValidZAxis+1; iZ<isp; iZ++ ) {
				pSCPAxisZ[iZ] = SCPzAxis;
			}
			iLastValidZAxis = isp-1;
		}

		// If we have a Y axis, save it in the array, update iLastValidYAxis, and celebrate appropriately.
		// 
		if( bGotYAxis ) {
			iLastValidYAxis = isp;
			SCPyAxis.Normalize();
			pSCPAxisY[isp] = SCPyAxis;
		}

		// If we have a Z axis, save it in the array, update iLastValidZAxis, and celebrate appropriately.
		// 
		if( bGotZAxis ) {
			iLastValidZAxis = isp;
			SCPzAxis.Normalize();
			pSCPAxisZ[isp] = SCPzAxis;
		}

	}

	Rotation rot;

	// Now we are done with the SCP calulations ( unless we are 100% coolinear )
	// We can not loop through the Spine Vertebras again, and apply the transforms to the cross section.
	for ( isp = 0; isp < nSpines; isp++ ) {


		// get the Scale for this spine
		//
		float scaleX, scaleZ;
		if ( isp >= scalesize ) {
			scaleX = (* m_scale )[scalesize-1].x;
			scaleZ = (* m_scale )[scalesize-1].y;
		}
		else {
			scaleX = (* m_scale )[isp].x;
			scaleZ = (* m_scale )[isp].y;
		}

		// Get the orienations and generate the Matrix.
		//
		if ( isp >= orienationsize ) {
			rot = (* m_orientation )[orienationsize-1];
		}
		else {
			rot = (* m_orientation )[isp];
		}
		matrix4 RotMatrix;
		RotMatrix.ToAxisAngle( rot.vec, rot.theta);


		matrix4 TmpMatrix( matrix4::Identity );
		// calc the matrix using the two vectors
		//
		if( iLastValidZAxis < 0 && iLastValidYAxis >= 0 ) {
			// 100% colinear spine, 
			//

			if( iLastValidYAxis < nSpines-1 ) {
				// SHould never happen
				//
				assert( 0 );
				return NULL;
			}

			// Get the cross product of the Y vector, and the Spine Vector.
			Point3 tmpCrossOfYs = Point3::j ^ pSCPAxisY[isp];
			float lenghtOfCross = tmpCrossOfYs.Mag(); 
			float angle = asin( lenghtOfCross );

			// If our cross product is ZeroExt, we need to create a valid axis of rotation,
			// Because we may be rotating about it 180 degrees if the Y axis point in opposite directiosn.
			//
			if( lenghtOfCross < ZeroExt ) {
				tmpCrossOfYs = Point3::k;
			}
			else {
				tmpCrossOfYs.Normalize();
			}

			// If the Dot Protduct of the two Y Vectors is negative, our angle is larger than 90.
			// angle = atan2( lenghtOfCross, dotOfYAxis ); 
			if( pSCPAxisY[isp].y < 0.0 ) {
				angle = PI - angle;
			}

			// Apply rotation to Matrix.
			//
			TmpMatrix.ToAxisAngle( tmpCrossOfYs,  angle );
		}
		else
		{
			// Not Colinear SPine, build the rotation Matrix from the Y and Z axis that we
			// Calulated last time we looped.
			// Do some cross producting to generate the X axis, then
			// regenerate a Z axis to ensure orthogonality.

			Point3 SCPxAxis( pSCPAxisY[isp] ^ pSCPAxisZ[isp] );
			SCPxAxis.Normalize();
			Point3 SCPzAxis( SCPxAxis ^ pSCPAxisY[isp] );
			SCPzAxis.Normalize();


			TmpMatrix._11 = SCPxAxis.x;
			TmpMatrix._12 = SCPxAxis.y;
			TmpMatrix._13 = SCPxAxis.z;

			TmpMatrix._21 = pSCPAxisY[isp].x;
			TmpMatrix._22 = pSCPAxisY[isp].y;
			TmpMatrix._23 = pSCPAxisY[isp].z;

			TmpMatrix._31 = SCPzAxis.x;
			TmpMatrix._32 = SCPzAxis.y;
			TmpMatrix._33 = SCPzAxis.z;
		}
		// now rotate the matrix with the orientation.
		//
		pMatrixes[isp] = RotMatrix * TmpMatrix;
		// Do the Translation
		//
		pMatrixes[isp]._11 *= scaleX;
		pMatrixes[isp]._12 *= scaleX;
		pMatrixes[isp]._13 *= scaleX;

		pMatrixes[isp]._31 *= scaleZ;
		pMatrixes[isp]._32 *= scaleZ;
		pMatrixes[isp]._33 *= scaleZ;

		pMatrixes[isp].Place( (*m_spine)[ isp ] );
	}

	delete [] pSCPAxisY;
	delete [] pSCPAxisZ;

	return pMatrixes;
}



float* CAnmExtrusion::ComputeCsxTextureCoords()
{
	int size = m_crossSection->size();
	float* pUCsxTexCoords = new float[size];
	// get total length
	float totalLength = 0.0;
	for( int i=1; i<size; i++ ) {
		totalLength += ( (*m_crossSection)[i] - (*m_crossSection)[i-1] ).Mag();
	}

	if( totalLength > 1.0e-6 ) {
		float tmpLength = 0.0;
		pUCsxTexCoords[0] = 0.0;
		for( int i=1; i<size; i++ ) {
			tmpLength += ( (*m_crossSection)[i] - (*m_crossSection)[i-1] ).Mag();
			pUCsxTexCoords[i] = tmpLength / totalLength;
		}
	}
	else {
		pUCsxTexCoords[0] = 0.0;
		for( int i=1; i<size; i++ ) {
			pUCsxTexCoords[i] = ( ( float ) i ) / ( ( float ) size );
		}
	}

	return pUCsxTexCoords;
}



float* CAnmExtrusion::ComputeSpineTextureCoords()
{
	int size = m_spine->size();
	float* pVSpineTexCoords = new float[size];
	// get total length
	float totalLength = 0.0;
	for( int i=1; i<size; i++ ) {
		totalLength += ( (*m_spine)[i] - (*m_spine)[i-1] ).Mag();
	}

	if( totalLength > 1.0e-6 ) {
		float tmpLength = 0.0;
		pVSpineTexCoords[0] = 0.0;
		for( int i=1; i<size; i++ ) {
			tmpLength += ( (*m_spine)[i] - (*m_spine)[i-1] ).Mag();
			pVSpineTexCoords[i] = tmpLength / totalLength;
		}
	}
	else {
		pVSpineTexCoords[0] = 0.0;
		for( int i=1; i<size; i++ ) {
			pVSpineTexCoords[i] = ( ( float ) i ) / ( ( float ) size );
		}
	}

	return pVSpineTexCoords;
}




// CAnmGeometry overrides
void CAnmExtrusion::Realize()
{
	CreateMesh();
	CAnmGeometry::Realize();		// chain down

	ClearStateDirty();				// we're built, so we're clean
}

void CAnmExtrusion::SetBeginCap(Boolean beginCap)
{
	m_beginCap = beginCap;
}

void CAnmExtrusion::SetCCW(Boolean ccw)
{
	m_ccw = ccw;
}

void CAnmExtrusion::SetConvex(Boolean convex)
{
	m_convex = convex;
}

void CAnmExtrusion::SetCreaseAngle(Float creaseAngle)
{
	m_creaseAngle = creaseAngle;
}

void CAnmExtrusion::SetCrossSection(Point2Array *pCrossSection)
{
	assert(pCrossSection != NULL);

	SafeUnRef(m_crossSection);
	m_crossSection = pCrossSection;
	m_crossSection->Ref();

	SetStateDirty();
}

void CAnmExtrusion::SetEndCap(Boolean endCap)
{
	m_endCap = endCap;
}


void CAnmExtrusion::SetOrientation(RotationArray *pOrientation)
{
	assert(pOrientation != NULL);

	SafeUnRef(m_orientation);
	m_orientation = pOrientation;
	m_orientation->Ref();

	SetStateDirty();
}

void CAnmExtrusion::SetScale(Point2Array *pScale)
{
	assert(pScale != NULL);

	SafeUnRef(m_scale);
	m_scale = pScale;
	m_scale->Ref();

	SetStateDirty();
}

void CAnmExtrusion::SetSpine(Point3Array *pSpine)
{
	assert(pSpine != NULL);

	SafeUnRef(m_spine);
	m_spine = pSpine;
	m_spine->Ref();

	SetStateDirty();
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmExtrusion, beginCap, eValueBoolean, Boolean, GetBeginCap, SetBeginCap);
DEFINE_VALUE(CAnmExtrusion, ccw, eValueBoolean, Boolean, GetCCW, SetCCW);
DEFINE_VALUE(CAnmExtrusion, convex, eValueBoolean, Boolean, GetConvex, SetConvex);
DEFINE_VALUE(CAnmExtrusion, creaseAngle, eValueFloat, Float, GetCreaseAngle, SetCreaseAngle);
DEFINE_VALUE(CAnmExtrusion, crossSection, eValuePoint2Array, Point2Array*, GetCrossSection, SetCrossSection);
DEFINE_VALUE(CAnmExtrusion, endCap, eValueBoolean, Boolean, GetEndCap, SetEndCap);
DEFINE_VALUE(CAnmExtrusion, orientation, eValueRotationArray, RotationArray*, GetOrientation, SetOrientation);
DEFINE_VALUE(CAnmExtrusion, scale, eValuePoint2Array, Point2Array*, GetScale, SetScale);
DEFINE_INHERITED_VALUE(CAnmExtrusion, solid, CAnmGeometry);
DEFINE_VALUE(CAnmExtrusion, spine, eValuePoint3Array, Point3Array*, GetSpine, SetSpine);

DEFINE_METHOD(CAnmExtrusion, set_crossSection, eValuePoint2Array, Point2Array*, SetCrossSection);
DEFINE_METHOD(CAnmExtrusion, set_orientation, eValueRotationArray, RotationArray*, SetOrientation);
DEFINE_METHOD(CAnmExtrusion, set_scale, eValuePoint2Array, Point2Array*, SetScale);
DEFINE_METHOD(CAnmExtrusion, set_spine, eValuePoint3Array, Point3Array*, SetSpine);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmExtrusion)
VALUEID(CAnmExtrusion, beginCap), 
VALUEID(CAnmExtrusion, ccw), 
VALUEID(CAnmExtrusion, convex), 
VALUEID(CAnmExtrusion, creaseAngle), 
VALUEID(CAnmExtrusion, crossSection), 
VALUEID(CAnmExtrusion, endCap), 
VALUEID(CAnmExtrusion, orientation), 
VALUEID(CAnmExtrusion, scale), 
VALUEID(CAnmExtrusion, solid),
VALUEID(CAnmExtrusion, spine), 

METHODID(CAnmExtrusion, set_crossSection), 
METHODID(CAnmExtrusion, set_orientation), 
METHODID(CAnmExtrusion, set_scale), 
METHODID(CAnmExtrusion, set_spine), 

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmExtrusion, CAnmGeometry);


