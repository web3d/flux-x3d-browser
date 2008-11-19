/********************************************************************************
 * Flux
 *
 * File: anmelevationgrid.cpp
 * Description: ElevationGrid node
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
#include "anmelevationgrid.h"
#include "application.h"

#include "anmcolor.h"
#include "anmcoordinate.h"
#include "anmnormal.h"
#include "anmtexturecoordinate.h"
#include "anmtexturetransform.h"
#include "anmprimitive.h"
#include "anmmesh.h"
#include "anmdevice.h"
#include "anmworld.h"

#define MAX_VERTS_IN_INDEXED_TRI_LIST		( 64*1024 )


CAnmElevationGrid::CAnmElevationGrid() : CAnmComposedGeometry(),
	m_creaseAngle(ANMELV_DEFAULT_CREASEANGLE),
	m_xDimension(ANMELV_DEFAULT_XDIMENSION),
	m_xSpacing(ANMELV_DEFAULT_XSPACING),
	m_zDimension(ANMELV_DEFAULT_ZDIMENSION),
	m_zSpacing(ANMELV_DEFAULT_ZSPACING)
{
	m_ccw = ANMELV_DEFAULT_CCW;

	m_color = NULL;
	m_normal = NULL;
	m_texCoord = NULL;

	m_height = new FloatArray();
}

CAnmElevationGrid::~CAnmElevationGrid()
{
	SafeUnRef(m_height);
}

void CAnmElevationGrid::CreateMesh()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	// Create a new empty mesh
	m_mesh = new CAnmMesh();
	assert(m_mesh);

	// Build the grid
	if( BuildGrid() ) {
		// Set a default color if we don't have one. do it before normal generation
		if (m_color == NULL) {
			m_mesh->SetDiffuseColor(color4::White);
		}
	}
}

void CAnmElevationGrid::AddTriListToMesh( CAnmTriangleList *pTriangleList, bool bGenNormals, bool bBufferVerts )
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

bool CAnmElevationGrid::BuildGrid()
{
	bool bOK = false;
	Point3Array *pNormalValues = NULL;
	ColorArray *pColorValues = NULL;
	int nNormals = 0;
	int nColors = 0;
	int nTexCoords = 0;
	CAnmColor *pColorNode = GETIMPLEMENTATION(CAnmColor, m_color);
	CAnmNormal *pNormalNode = GETIMPLEMENTATION(CAnmNormal, m_normal);

	if (m_xDimension < 2 || m_zDimension < 2)
	{
		// N.B.: warning here?
		return bOK;
	}

	if (m_height->size() < m_xDimension * m_zDimension)
	{
		// N.B.: warning here?
		return bOK;
	}

	if (pNormalNode)
	{
		pNormalValues = pNormalNode->GetVector();
		nNormals = pNormalValues->size();
	}

	if (pColorNode)
	{
		pColorValues = pColorNode->GetColor();
		nColors = pColorValues->size();
	}

	int xdim = m_xDimension;
	int zdim = m_zDimension;
	int nheights = m_height->size();
	int nQuads = (xdim - 1) * (zdim - 1);
	int nTriangles = nQuads * 2;

	BOOL onevertex = TRUE;
	if (nNormals && !m_normalPerVertex && nColors && !m_colorPerVertex) {
		onevertex = FALSE;
	}

	bool bDoIndexedTriList = true;

	if( !onevertex ||
		nheights*4 > MAX_VERTS_IN_INDEXED_TRI_LIST ) {
		bDoIndexedTriList = false;
	}

	if( bDoIndexedTriList ) {
		bOK = BuildGridIndexedTriSet();
	}
	else {
		bOK = BuildGridNonIndexedTriSet();
	}
	return bOK;
}




bool CAnmElevationGrid::BuildGridNonIndexedTriSet()
{
	int nTriListsAdded = 0;

	Point3Array *pNormalValues = NULL;
	ColorArray *pColorValues = NULL;
	Point2Array *pTexCoordValues = NULL;
	int nNormals = 0;
	int nColors = 0;
	int nTexCoords = 0;
	CAnmColor *pColorNode = GETIMPLEMENTATION(CAnmColor, m_color);
	CAnmNormal *pNormalNode = GETIMPLEMENTATION(CAnmNormal, m_normal);
	CAnmTextureCoordinate *pTexCoordNode = GETIMPLEMENTATION(CAnmTextureCoordinate, m_texCoord);

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	bool bPostGenerateNormals = false;
	Point3Array* pGeneratedNormals = NULL;

	int iLocToGenNormalOffset[4] = { 1, 2, 0, 3 };

	if (m_xDimension < 2 || m_zDimension < 2)
	{
		// N.B.: warning here?
		return NULL;
	}

	if (m_height->size() < m_xDimension * m_zDimension)
	{
		// N.B.: warning here?
		return NULL;
	}

	if (pNormalNode)
	{
		pNormalValues = pNormalNode->GetVector();
		nNormals = pNormalValues->size();
	}

	if( nNormals == 0 ) {
		if( m_creaseAngle < 0.05 ) {
			// we will post calculate the normals.  Its faster for the simple case.
			//
			bPostGenerateNormals = true;
		}
		else {
			pGeneratedNormals = CalculateNormals();
		}
	}


	bool bGenNormals = ( nNormals == 0 );



	if (pColorNode)
	{
		pColorValues = pColorNode->GetColor();
		nColors = pColorValues->size();
	}

	if (pTexCoordNode)
	{
		pTexCoordValues = pTexCoordNode->GetPoint();
		nTexCoords = pTexCoordValues->size();
	}

	int xdim = m_xDimension;
	int zdim = m_zDimension;
	float xspacing = m_xSpacing;
	float zspacing = m_zSpacing;
	Point3 *normals = pNormalValues ? &(*pNormalValues)[0] : NULL;
	Color *colors = pColorValues ? &(*pColorValues)[0] : NULL;
	Point2 *uvs = pTexCoordValues ? &(*pTexCoordValues)[0] : NULL;
	float *heights = &(*m_height)[0];
	int nheights = m_height->size();

	int nQuads = (xdim - 1) * (zdim - 1);
	float x, y, z;

	int tmpIndex;

	int MaxTrianglesInTriList = pDevice->GetMaxPrimitiveCount() - 20;

	// Create a new triangle list; the NON indexed kind..
	//
	CAnmTriangleList *pTriangleList = new CAnmTriangleList(pDevice);
	assert(pTriangleList);

	tAnmVertexArray *pVerts = pTriangleList->GetVertices();

	// Reserve either one vertex or replicate for each shared face,
	// depending on per-vertex color and normal choices

	int iVertsLeftToAdd = nQuads * 6;
	pVerts->reserve(min( iVertsLeftToAdd, MaxTrianglesInTriList*3 + 6 )); 

	// Build the triangle list
	int vertCount = 0;
	int iPoly = 0;
	int index = 0;

	int iii, jjj, iThis, jThis, index0;
	sAnmVertex LocVertArray[4];

	int nTrisInCurrentList = 0;

	for (int i = 0; i < zdim-1; i++ )
	{
		for (int j = 0; j < xdim-1; j++, iPoly++ )
		{
			index0 = i * xdim + j;

			int iLoc = 0;
			for( iii=0; iii<2; iii++ ) {
				iThis = i + iii;
				z = iThis*zspacing;
				for( jjj=0; jjj<2; jjj++, iLoc++ ) {
					jThis = j + jjj;
					x = jThis*xspacing;
					index = index0 + iii*xdim + jjj;
					y = heights[index];

					// First set location.
					//
					LocVertArray[iLoc].vec = ( Point3(x,y,z) );

					// do texture coords next; they're the same
					// regardless of normal,color issues
					if (uvs && nTexCoords)
					{
						if (index < nTexCoords) {
							LocVertArray[iLoc].texCoord = uvs[index];
						}
						else {
							LocVertArray[iLoc].texCoord = uvs[nTexCoords - 1];
						}
					}
					else
					{
						// Default Tex Coords.
						//
						LocVertArray[iLoc].texCoord = Point2(   ( ( float ) jThis )/ ( xdim-1 ), ( ( float ) iThis ) / ( zdim-1 ) );
					}


					// DO Norms
					// If we dont have them, use the ones we calulated.
					//
					if( pGeneratedNormals ) {
						LocVertArray[iLoc].normal = ( (*pGeneratedNormals)[ 4*index + iLocToGenNormalOffset[iLoc] ] );
					}
					else if ( normals && nNormals)
					{
						tmpIndex = ( m_normalPerVertex ) ? index : iPoly;
						if (tmpIndex < nNormals) {
							LocVertArray[iLoc].normal = normals[tmpIndex];
						}
						else {
							LocVertArray[iLoc].normal = normals[nNormals - 1];
						}
					}
					else {
						// we will post calculate the normals.  Its faster for the simple case.
						//
						assert( bPostGenerateNormals );
						LocVertArray[iLoc].normal = Point3( 0, 1.0, 0 );
					}

					// Do Colors
					//
					if (colors && nColors)
					{
						tmpIndex = ( m_colorPerVertex ) ? index : iPoly;
						if (tmpIndex < nColors) {
							LocVertArray[iLoc].SetDiffuseColor( colors[tmpIndex] );
						}
						else {
							LocVertArray[iLoc].SetDiffuseColor( colors[nColors - 1] );
						}
					}
				}
			}

			if (m_ccw) {
				pVerts->push_back( LocVertArray[1] );
				pVerts->push_back( LocVertArray[0] );
				pVerts->push_back( LocVertArray[3] );

				pVerts->push_back( LocVertArray[0] );
				pVerts->push_back( LocVertArray[2] );
				pVerts->push_back( LocVertArray[3] );
			}
			else {
				pVerts->push_back( LocVertArray[0] );
				pVerts->push_back( LocVertArray[1] );
				pVerts->push_back( LocVertArray[3] );

				pVerts->push_back( LocVertArray[0] );
				pVerts->push_back( LocVertArray[3] );
				pVerts->push_back( LocVertArray[2] );
			}
			nTrisInCurrentList += 2;

			// CHeck if we need to wrap this up.
			//
			if( nTrisInCurrentList > MaxTrianglesInTriList ) {
				// FInish off the last list
				//
				AddTriListToMesh( pTriangleList, bPostGenerateNormals, true );

				// Create the New List
				//
				pTriangleList = new CAnmTriangleList(pDevice);
				assert(pTriangleList);
				pVerts = pTriangleList->GetVertices();

				iVertsLeftToAdd -= nTrisInCurrentList*3;

				pVerts->reserve(min( iVertsLeftToAdd, MaxTrianglesInTriList*3 + 6 )); 
				nTrisInCurrentList = 0;
			}
		}
	}

	delete pGeneratedNormals;

	AddTriListToMesh( pTriangleList, bPostGenerateNormals, true );
	return ( nTriListsAdded > 0 );
}























bool CAnmElevationGrid::BuildGridIndexedTriSet()
{

	Point3Array *pNormalValues = NULL;
	ColorArray *pColorValues = NULL;
	Point2Array *pTexCoordValues = NULL;
	int nNormals = 0;
	int nColors = 0;
	int nTexCoords = 0;
	CAnmColor *pColorNode = GETIMPLEMENTATION(CAnmColor, m_color);
	CAnmNormal *pNormalNode = GETIMPLEMENTATION(CAnmNormal, m_normal);
	CAnmTextureCoordinate *pTexCoordNode = GETIMPLEMENTATION(CAnmTextureCoordinate, m_texCoord);

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	if (m_xDimension < 2 || m_zDimension < 2)
	{
		// N.B.: warning here?
		return NULL;
	}

	if (m_height->size() < m_xDimension * m_zDimension)
	{
		// N.B.: warning here?
		return NULL;
	}

	if (pNormalNode)
	{
		pNormalValues = pNormalNode->GetVector();
		nNormals = pNormalValues->size();
	}
	bool bGenNormals = ( nNormals == 0 );



	if (pColorNode)
	{
		pColorValues = pColorNode->GetColor();
		nColors = pColorValues->size();
	}

	if (pTexCoordNode)
	{
		pTexCoordValues = pTexCoordNode->GetPoint();
		nTexCoords = pTexCoordValues->size();
	}

	int xdim = m_xDimension;
	int zdim = m_zDimension;
	float xspacing = m_xSpacing;
	float zspacing = m_zSpacing;
	Point3 *normals = pNormalValues ? &(*pNormalValues)[0] : NULL;
	Color *colors = pColorValues ? &(*pColorValues)[0] : NULL;
	Point2 *uvs = pTexCoordValues ? &(*pTexCoordValues)[0] : NULL;
	float *heights = &(*m_height)[0];
	int nheights = m_height->size();

	int nQuads = (xdim - 1) * (zdim - 1);
	float x, y, z;
	int index;

	BOOL onevertex = TRUE;
	if (nNormals && normals && !m_normalPerVertex && nColors && colors && !m_colorPerVertex) {
		onevertex = FALSE;
		// this shold not happen!
		// We build a non-indexed Tri list if this is the case.
		//
		assert( 0 );
	}

	// Create a new triangle list
	CAnmIndexedTriangleList *pTriangleList = new CAnmIndexedTriangleList(pDevice);
	assert(pTriangleList);

	tAnmVertexArray *pVerts = pTriangleList->GetVertices();
	tAnmIndexArray *pIndices = pTriangleList->GetIndices();

	// Reserve either one vertex or replicate for each shared face,
	// depending on per-vertex color and normal choices
	pVerts->reserve(onevertex ? nheights : nQuads * 4); 
	
	// Reserve two tris per quad
	pIndices->reserve(nQuads * 6); 

	// Build the triangle list
	int vertCount = 0;
	for (int i = 0; i < zdim; i++)
	{
		z = zspacing * i;

		for (int j = 0; j < xdim; j++)
		{
			x = xspacing * j;

			index = i * xdim + j;

			y = heights[index];

			sAnmVertex vert(Point3(x,y,z));

			// do texture coords first; they're the same
			// regardless of normal,color issues
			Point2 p2;
			if (uvs && nTexCoords)
			{
				if (index < nTexCoords)
					p2 = uvs[index];
				else
					p2 = uvs[nTexCoords - 1];
			}
			else
			{
				p2 = Point2(j * (1.0f / ( xdim-1 )), i * (1.0f / ( zdim-1 )));
			}
			vert.SetTexCoords(p2);


			if (normals && nNormals)
			{
				Point3 norm;

				if (index < nNormals)
					norm = normals[index];
				else
					norm = normals[nNormals - 1];

				vert.SetNormal(norm);
			}

			if (colors && nColors)
			{
				Color c;

				if (index < nColors)
					c = colors[index];
				else
					c = colors[nColors - 1];

				vert.SetDiffuseColor(c);
			}

			pVerts->push_back(vert);

			vertCount++;

			// now make the quad (shared vertices)
			if (i > 0 && j > 0)
			{
				pIndices->push_back(i * xdim + j);
				if (m_ccw)
				{
					pIndices->push_back((i - 1) * xdim + j - 1);
					pIndices->push_back(i * xdim + j - 1);
				}
				else
				{
					pIndices->push_back(i * xdim + j - 1);
					pIndices->push_back((i - 1) * xdim + j - 1);
				}

				pIndices->push_back(i * xdim + j);
				if (m_ccw)
				{
					pIndices->push_back((i - 1) * xdim + j);
					pIndices->push_back((i - 1) * xdim + j - 1);
				}
				else
				{
					pIndices->push_back((i - 1) * xdim + j - 1);
					pIndices->push_back((i - 1) * xdim + j);
				}
			}
		}
	}
	AddTriListToMesh( pTriangleList, bGenNormals, true );
	return ( pTriangleList != NULL );
}


static int Decriment4( int iThis )
{
	int iOut = iThis - 1;
	if( iOut < 0 ) {
		iOut=3;
	}
	return iOut;
}



Point3Array* CAnmElevationGrid::CalculateNormals()
{

	// krv
	// THis is a multipass approach as calculating the normslas
	// We go through each vertex, and calc four normals ( or less for edge cases )
	// We get one normal for each face that touches the vertex
	/************

	The Fave and Edge emueration looks like this:

	Face Enumeration:

        
        |
      3 | 0 
    ----o-----
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



	int xdim = m_xDimension;
	int zdim = m_zDimension;
	float xspacing = m_xSpacing;
	float zspacing = m_zSpacing;
	float *heights = &(*m_height)[0];
	int nheights = m_height->size();
	int nVerts = xdim * zdim;

	Point3Array *pGeneratedNormalValues = new Point3Array;
	pGeneratedNormalValues->reserve( 4 * nVerts );

	// ofsets for i and j to get us to the adjacent vertex,
	
	int jxOff[4] = { 0, 1, 0, -1 };
	int izOff[4] = { -1, 0, 1, 0 };
	
	Point3 FaceNormals[4];
	Point3 TmpVerts[4];
	int iFace, iNext, iii, jjj, index, iVertIndex, iEdge, iNextVert, iThisVert;
	int iPrevFace, iNextFace, iSmooth;
	float x, y, z, theDot;
	float crease_dot = cos( m_creaseAngle ) - 0.01;

	Point3 SmoothingNormals[4];
	Point3 DefNorm( 0, 1, 0 );

	// Loop thought each Vertex, I and J.
	//
	for (int i = 0; i < zdim; i++)
	{
		z = zspacing * i;

		for (int j = 0; j < xdim; j++)
		{

			// start the passes for each vertex.
			//

			bool bGotFace[4] = { false, false, false, false };
			bool bGotVert[4] = { false, false, false, false };
			bool bGotEdge[4] = { false, false, false, false };
			bool bSmoothEdge[4] = { false, false, false, false };

			int iSmoothingGroup[4] = { -1, -1, -1, -1 };

			x = xspacing * j;

			index = i * xdim + j;
			y = heights[index];

			Point3 PntCenter( x, y, z );
			// First calc all four adjacent verts coords
			//
			for( iEdge=0; iEdge<4; iEdge++ ) {
				iii = i + izOff[iEdge];
				jjj = j + jxOff[iEdge];
				if( iii >= 0 && iii < zdim &&
					jjj >= 0 && jjj < xdim ) {

					bGotVert[iEdge] = true;
					iVertIndex = iii * xdim + jjj;

					TmpVerts[iEdge].y = heights[iVertIndex];
					TmpVerts[iEdge].x = xspacing * jjj;
					TmpVerts[iEdge].z = zspacing * iii;
				}
			}


			// Now calc the four face normals
			//
			for( iFace=0; iFace<4; iFace++ ) {
				iThisVert = iFace;
				iNextVert = ( iFace+1 ) %4;
				if( bGotVert[iThisVert] && bGotVert[iNextVert] ) {
					bGotFace[iFace] = true;
					plane3 plane(
						TmpVerts[iThisVert], 
						PntCenter,
						TmpVerts[iNextVert] );
					FaceNormals[iFace] = plane.n;
				}
			}


			// Now get the dot product of adjacent face normals.
			// Dtermine if the Edge should be smoothed out.
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

			// Now group the Faces into Smoothing Groups\
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
						// Determine if we are smooth with Previous face3;
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
				// merge the first and last
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
			for( iSmooth=0; iSmooth<iNextGroup; iSmooth++ ) {
				SmoothingNormals[iSmooth] = Point3::Zero;
				for( iFace=0; iFace<4; iFace++ ) {
					if( iSmoothingGroup[iFace] == iSmooth ) {
						SmoothingNormals[iSmooth] += FaceNormals[iFace];
					}
				}
				SmoothingNormals[iSmooth].Normalize();
			}

			// I think we can finnaly set the normal into the big array.
			//
			for( iFace=0; iFace<4; iFace++ ) {
				if( iSmoothingGroup[iFace] >= 0 ) {
					if( m_ccw ) {
						pGeneratedNormalValues->push_back( SmoothingNormals[iSmoothingGroup[iFace]] );
					}
					else {
						pGeneratedNormalValues->push_back( -SmoothingNormals[iSmoothingGroup[iFace]] );
					}
				}
				else {
					pGeneratedNormalValues->push_back( DefNorm );
				}
			}
		}
	}
	return pGeneratedNormalValues;
}



// CAnmComposedGeometry overrides
void CAnmElevationGrid::Realize()
{
	CreateMesh();
	CAnmComposedGeometry::Realize();		// chain down

	ClearStateDirty();				// we're built, so we're clean
}

void CAnmElevationGrid::Update()
{
	// N.B.: worth the extra check here?
	if (!StateDirty())
		return;

	// N.B.: These could be optimizations someday
	if (TestDirtyBits(eAnmCGColorDirty))
	{
	}
	if (TestDirtyBits(eAnmCGNormalDirty))
	{
	}
	if (TestDirtyBits(eAnmCGTexCoordDirty))
	{
	}

	SafeUnRef(m_mesh);
	CreateMesh();

	ClearStateDirty();
}



// Accessors
void CAnmElevationGrid::SetCreaseAngle(Float creaseAngle)
{
	m_creaseAngle = creaseAngle;
}

void CAnmElevationGrid::SetHeight(FloatArray *pHeight)
{
	assert(pHeight);

	SafeUnRef(m_height);

	m_height = pHeight;
	m_height->Ref();

	SetDirtyBits(eAnmELVHeightDirty);
	SetStateDirty();
}

void CAnmElevationGrid::SetXDimension(Integer xDimension)
{
	m_xDimension = xDimension;
}

void CAnmElevationGrid::SetXSpacing(Float xSpacing)
{
	m_xSpacing = xSpacing;
}

void CAnmElevationGrid::SetZDimension(Integer zDimension)
{
	m_zDimension = zDimension;
}

void CAnmElevationGrid::SetZSpacing(Float zSpacing)
{
	m_zSpacing = zSpacing;
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmElevationGrid, ccw, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmElevationGrid, color, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmElevationGrid, colorPerVertex, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmElevationGrid, normal, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmElevationGrid, normalPerVertex, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmElevationGrid, solid, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmElevationGrid, texCoord, CAnmComposedGeometry);
DEFINE_VALUE(CAnmElevationGrid, creaseAngle, eValueFloat, Float, 
			 GetCreaseAngle, SetCreaseAngle);
DEFINE_VALUE(CAnmElevationGrid, height, eValueFloatArray, FloatArray *, 
			 GetHeight, SetHeight);
DEFINE_VALUE(CAnmElevationGrid, xDimension, eValueInteger, Integer, 
			 GetXDimension, SetXDimension);
DEFINE_VALUE(CAnmElevationGrid, xSpacing, eValueFloat, Float, 
			 GetXSpacing, SetXSpacing);
DEFINE_VALUE(CAnmElevationGrid, zDimension, eValueInteger, Integer, 
			 GetZDimension, SetZDimension);
DEFINE_VALUE(CAnmElevationGrid, zSpacing, eValueFloat, Float, 
			 GetZSpacing, SetZSpacing);

DEFINE_INHERITED_METHOD(CAnmElevationGrid, set_color, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmElevationGrid, set_normal, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmElevationGrid, set_texCoord, CAnmComposedGeometry);
DEFINE_METHOD(CAnmElevationGrid, set_height, eValueFloatArray, FloatArray *, SetHeight);


DEFINE_INHERITED_CALLBACK(CAnmElevationGrid, color_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmElevationGrid, normal_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmElevationGrid, texCoord_changed, CAnmComposedGeometry);



// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmElevationGrid)
VALUEID(CAnmElevationGrid, ccw), 
VALUEID(CAnmElevationGrid, color), 
VALUEID(CAnmElevationGrid, colorPerVertex), 
VALUEID(CAnmElevationGrid, creaseAngle), 
VALUEID(CAnmElevationGrid, height), 
VALUEID(CAnmElevationGrid, normal), 
VALUEID(CAnmElevationGrid, normalPerVertex),
VALUEID(CAnmElevationGrid, solid),
VALUEID(CAnmElevationGrid, texCoord),
VALUEID(CAnmElevationGrid, xDimension),
VALUEID(CAnmElevationGrid, xSpacing),
VALUEID(CAnmElevationGrid, zDimension),
VALUEID(CAnmElevationGrid, zSpacing),

METHODID(CAnmElevationGrid, set_color), 
METHODID(CAnmElevationGrid, set_height), 
METHODID(CAnmElevationGrid, set_normal), 
METHODID(CAnmElevationGrid, set_texCoord),

CALLBACKID(CAnmElevationGrid, color_changed),
CALLBACKID(CAnmElevationGrid, normal_changed),
CALLBACKID(CAnmElevationGrid, texCoord_changed),


END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmElevationGrid, CAnmComposedGeometry);


