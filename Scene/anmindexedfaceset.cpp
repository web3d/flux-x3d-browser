/********************************************************************************
 * Flux
 *
 * File: anmindexedfaceset.cpp
 * Description: IndexedFaceSet node
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
#include "anmindexedfaceset.h"
#include "application.h"

#include "anmcolor.h"
#include "anmcoordinate.h"
#include "anmdevice.h"
#include "anmmesh.h"
#include "anmmultitexturecoordinate.h"
#include "anmnormal.h"
#include "anmprimitive.h"
#include "anmtesselator.h"
#include "anmtexturecoordinate.h"
#include "anmtexturecoordinategenerator.h"
#include "anmtexturetransform.h"
#include "anmworld.h"


#define ZEROF	( 1.0e-20 ) 

// krv Define this to get timing results for the CreateFaces method.
// #define TIMECREATEFACES


CAnmIndexedFaceSet::CAnmIndexedFaceSet() : CAnmComposedGeometry(),
	m_creaseAngle(ANMIFS_DEFAULT_CREASEANGLE),
	m_convex(ANMIFS_DEFAULT_CONVEX)
{
	m_ccw = ANMIFS_DEFAULT_CCW;

	m_color = NULL;
	m_coord = NULL;
	m_normal = NULL;
	m_texCoord = NULL;

	m_colorIndex = new IntegerArray;
	m_coordIndex = new IntegerArray;
	m_normalIndex = new IntegerArray;
	m_texCoordIndex = new IntegerArray;
}

CAnmIndexedFaceSet::~CAnmIndexedFaceSet()
{
	SafeUnRef(m_colorIndex);
	SafeUnRef(m_coordIndex);
	SafeUnRef(m_normalIndex);
	SafeUnRef(m_texCoordIndex);
}

void CAnmIndexedFaceSet::Update()
{
	// N.B.: worth the extra check here?
	if (!TestDirtyBits(eAnmNodeStateDirty | eAnmNodeMatrixDirty))
		return;

	// N.B.: maybe we can do all kinds of optimizations... someday. but for ->
	// now just rebuild the whole thing
	if (TestDirtyBits(eAnmCGColorDirty))
	{
	}
	if (TestDirtyBits(eAnmCGCoordDirty))
	{
	}
	if (TestDirtyBits(eAnmCGNormalDirty))
	{
	}
	if (TestDirtyBits(eAnmCGTexCoordDirty))
	{
	}

	// chuck the old mesh
	SafeUnRef(m_mesh);

	// build a new one
	CreateMesh();
	
	// update the scene poly count
	if (m_mesh)
	{
		GenBoundingVolumes();
		m_polycount = m_mesh->GetPolyCount();

		CAnmWorld *pWorld = GetWorld();
		if (pWorld)
			// N.B.: this is just to update the polycount, duh
			pWorld->SetStateDirty();
	}

	// clear state flag
	ClearStateDirty();
}

void CAnmIndexedFaceSet::CreateMesh()
{
	// Create a new empty mesh
	m_mesh = new CAnmMesh();
	assert(m_mesh);

	// Create the faces from all those fields


// krv  Time this method of define.
//
#ifdef TIMECREATEFACES 
	double dStartTime = ( (double)clock() );
#endif
	CreateFaces();

#ifdef TIMECREATEFACES
	double dEndTime = ( (double)clock() );
	TRACE( "Create Face:  %10.3f secs\n", ( dEndTime - dStartTime ) / ( ( double ) CLOCKS_PER_SEC ) );
#endif

}

#ifdef DEBUG
inline void dump(char *format, ... )
{
	char buf[1024];

	va_list arglist;
	va_start (arglist, format);
	vsprintf (buf, format, arglist);
	va_end (arglist);


	DP(buf);
}
#endif // DEBUG

// N.B.: length
#define MAXBUFLEN 2048

void CAnmIndexedFaceSet::CreateFaces()
{
	Point3Array *pCoordinates = NULL;
	Point3Array *pNormalValues = NULL;
	// krv:  Added pGeneratedNormalValues to keep pointers to temp data.
	//
	float* pGeneratedNormalValues = NULL;
	int*   pGeneratedNormalIndex = NULL;
	ColorArray *pColorValues = NULL;
	Point2Array *pTexCoordValues = NULL;
	int nVerts = 0; 
	int nNormals = 0;
	int nColors = 0;
	int nTexCoords = 0;
	int nIndices = m_coordIndex->size();
	int nNormalIndices = m_normalIndex->size();
	int nColorIndices = m_colorIndex->size();
	int nTexCoordIndices = m_texCoordIndex->size();
	int *pNormalIndices = NULL;
	int *pColorIndices = NULL;
	int *pTexCoordIndices = NULL;
	CAnmColor *pColorNode = GETIMPLEMENTATION(CAnmColor, m_color);
	CAnmCoordinate *pCoordNode = GETIMPLEMENTATION(CAnmCoordinate, m_coord);
	CAnmNormal *pNormalNode = GETIMPLEMENTATION(CAnmNormal, m_normal);
	CAnmTextureCoordinate *pTexCoordNode = GETIMPLEMENTATION(CAnmTextureCoordinate, m_texCoord);
	CAnmMultiTextureCoordinate *pMTCoordNode = NULL;
	CAnmTextureCoordinateGenerator *pTexCoordGenNode = NULL;
	NodeArray *pTexCoordNodes = NULL;
	int i, j;

	// Do some sanity checks
	char msgbuf[MAXBUFLEN];
	char *nodename = m_name ? m_name->GetBuf() : "" ;

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	if (pCoordNode == NULL || nIndices == 0)
	{
		sprintf(msgbuf, "No coordinates found in IFS '%s'; skipping", nodename);
		pApp->Warn(msgbuf);
		return;								// what else to do?
	}

	pCoordinates = pCoordNode->GetPoint();
	nVerts = pCoordinates->size();

	Boolean normalPerVertex = m_normalPerVertex;

	// Do some setup
	if (pNormalNode)
	{
		pNormalValues = pNormalNode->GetVector();
		nNormals = pNormalValues->size();
		if (nNormalIndices > 0)
		{
			if (m_normalPerVertex && nNormalIndices < nIndices)
			{
				sprintf(msgbuf, "Too few normal indices in IFS '%s'; skipping", nodename);
				pApp->Warn(msgbuf);
				return;
			}

			pNormalIndices = &(*m_normalIndex)[0];
		}
		else if (m_normalPerVertex)
		{
			nNormalIndices = nIndices;
			pNormalIndices = &(*m_coordIndex)[0];
		}
	}
	else {
		// krv:  If we dont have normal data, then we generate it now.
		//
		int nGeneratedNormals = 0;
		if( !PreGenerateNormals( &pGeneratedNormalValues, &pGeneratedNormalIndex, nGeneratedNormals ) ) {
			ASSERT( 0 );
			return;
		}
		nNormals = nGeneratedNormals;
	}




	if (pColorNode)
	{
		pColorValues = pColorNode->GetColor();
		nColors = pColorValues->size();
		if (nColorIndices > 0)
		{
			if (m_colorPerVertex && nColorIndices < nIndices)
			{
				sprintf(msgbuf, "Too few color indices in IFS '%s'; skipping", nodename);
				pApp->Warn(msgbuf);
				return;
			}

			pColorIndices = &(*m_colorIndex)[0];
		}
		else if (m_colorPerVertex)
		{
			nColorIndices = nIndices;
			pColorIndices = &(*m_coordIndex)[0];
		}
	}

	int nTextureCoordinateNodes = 0;

	if (pTexCoordNode == NULL)
	{
		pTexCoordGenNode = GETIMPLEMENTATION(CAnmTextureCoordinateGenerator, m_texCoord);

		if (pTexCoordGenNode)
			nTextureCoordinateNodes = 1;
		else
		{
			pMTCoordNode = GETIMPLEMENTATION(CAnmMultiTextureCoordinate, m_texCoord);
			if (pMTCoordNode)
			{
				pTexCoordNodes = pMTCoordNode->GetTexCoord();
				int sz = pTexCoordNodes->size();

				// We're going to store each real texture coordinate node in an array
				// and look for dups so that we can avoid creating extra mesh primitives
				CAnmTextureCoordinate **pTmpArray = NULL;

				// Allocate and initialize tmp array
				if (sz > 0)
				{
					pTmpArray = new CAnmTextureCoordinate *[sz];
					for (i = 0; i < sz; i++)
						pTmpArray[i] = NULL;
				}

				// Count up real texture coordinate nodes. Don't count texture coordinate generator
				// nodes or duplicate texture coordinate nodes.
				nTextureCoordinateNodes = 0;
				for (i = 0; i < sz; i++)
				{
					CAnmNode *pNode = (*pTexCoordNodes)[i];
					CAnmTextureCoordinate *pTmpTC = GETIMPLEMENTATION(CAnmTextureCoordinate, pNode);
					if (pTmpTC)
					{
						bool found = false;
						for (j = 0; j < i; j++)
						{
							if (pTmpTC == pTmpArray[j])
							{
								found = true;
								break;
							}
						}
						if (!found)
						{
							pTmpArray[i] = pTmpTC;
							nTextureCoordinateNodes++;

							// If there's only one we'll use it momentarily
							if (nTextureCoordinateNodes == 1)
								pTexCoordNode = pTmpTC;
							else
								pTexCoordNode = NULL;
						}
					}
				}

				SafeDelete(pTmpArray);
			}
		}
	}

	if (pTexCoordNode)
	{
		pTexCoordValues = pTexCoordNode->GetPoint();
		nTexCoords = pTexCoordValues->size();
		if (nTexCoordIndices > 0)
		{
			if (nTexCoordIndices < nIndices)
			{
				sprintf(msgbuf, "Too few texture coordinate indices in IFS '%s'; skipping", nodename);
				pApp->Warn(msgbuf);
				return;
			}

			 pTexCoordIndices = &(*m_texCoordIndex)[0];
		}
		else
		{
			nTexCoordIndices = nIndices;
			pTexCoordIndices = &(*m_coordIndex)[0];
		}

		nTextureCoordinateNodes = 1;
	}

	if (nTextureCoordinateNodes <= 1)
	{
		CreateFaces(0, pCoordinates, pNormalValues, pColorValues, pTexCoordValues, pGeneratedNormalValues,
			nVerts, nNormals, nColors, nTexCoords, pNormalIndices, pColorIndices, pTexCoordIndices, pGeneratedNormalIndex,
			nIndices, nNormalIndices, nColorIndices, nTexCoordIndices, pTexCoordGenNode == NULL);
	}
	else
	{
		int sz = pTexCoordNodes->size();
		for (i = 0; i < sz; i++)
		{
			CAnmNode *pNode = (*pTexCoordNodes)[i];
			pTexCoordNode = GETIMPLEMENTATION(CAnmTextureCoordinate, pNode);

			if (pTexCoordNode)
			{
				pTexCoordValues = pTexCoordNode->GetPoint();
				nTexCoords = pTexCoordValues->size();
				if (nTexCoordIndices > 0)
				{
					 pTexCoordIndices = &(*m_texCoordIndex)[0];
				}
				else
				{
					nTexCoordIndices = nIndices;
					pTexCoordIndices = &(*m_coordIndex)[0];
				}
			}
			else
			{
				pTexCoordValues = NULL;
				nTexCoords = 0;
				pTexCoordIndices = NULL;
				nTexCoordIndices = 0;

				pTexCoordGenNode = GETIMPLEMENTATION(CAnmTextureCoordinateGenerator, m_texCoord);
			}

			if (pTexCoordGenNode == NULL)
			{
				CreateFaces(i, pCoordinates, pNormalValues, pColorValues, pTexCoordValues, pGeneratedNormalValues,
					nVerts, nNormals, nColors, nTexCoords, pNormalIndices, pColorIndices, pTexCoordIndices, pGeneratedNormalIndex,
					nIndices, nNormalIndices, nColorIndices, nTexCoordIndices, true);
			}
		}
	}
	// krv:  Delete temp data for Generated Normals
	//
	if( pGeneratedNormalValues ) {
		delete [] pGeneratedNormalValues;
		delete [] pGeneratedNormalIndex;
	}


}






void CAnmIndexedFaceSet::CreateFaces(
									 int texturestage, 
									 Point3Array *pCoordinates, Point3Array *pNormalValues, ColorArray *pColorValues, Point2Array *pTexCoordValues, 
									 float* pGeneratedNormalValues,
									 int nVerts, int nNormals, int nColors, int nTexCoords, 
									 int *pNormalIndices, int *pColorIndices, int *pTexCoordIndices, int* pGeneratedNormalIndex,
									 int nIndices, int nNormalIndices, int nColorIndices, int nTexCoordIndices,
									 bool generateUVs)
{
	bool bNonIndexedTriList = ( nVerts > ANMMAXVERTICES / 3 );
	if( bNonIndexedTriList ) {
		CreateFacesNonIndexedTriList(
									 texturestage, 
									 pCoordinates, pNormalValues, pColorValues, pTexCoordValues, 
									 pGeneratedNormalValues,
									 nVerts, nNormals, nColors, nTexCoords, 
									 pNormalIndices, pColorIndices, pTexCoordIndices, pGeneratedNormalIndex,
									 nIndices, nNormalIndices, nColorIndices, nTexCoordIndices,
									 generateUVs);
	}
	else {
		CreateFacesIndexedTriList(
									 texturestage, 
									 pCoordinates, pNormalValues, pColorValues, pTexCoordValues, 
									 pGeneratedNormalValues,
									 nVerts, nNormals, nColors, nTexCoords, 
									 pNormalIndices, pColorIndices, pTexCoordIndices, pGeneratedNormalIndex,
									 nIndices, nNormalIndices, nColorIndices, nTexCoordIndices,
									 generateUVs);
	}
}














void CAnmIndexedFaceSet::CreateFacesNonIndexedTriList(
									 int texturestage, 
									 Point3Array *pCoordinates, Point3Array *pNormalValues, ColorArray *pColorValues, Point2Array *pTexCoordValues, 
									 float* pGeneratedNormalValues,
									 int nVerts, int nNormals, int nColors, int nTexCoords, 
									 int *pNormalIndices, int *pColorIndices, int *pTexCoordIndices, int* pGeneratedNormalIndex,
									 int nIndices, int nNormalIndices, int nColorIndices, int nTexCoordIndices,
									 bool generateUVs)
{

	// krv:  if we dont have any Texture COords, then go get some.
	// ??? DO we need to check if we have a texture.  No need to generate coords if we dont got no texture.
	//

	bool bNukeTextureCoords = false;
	if( pTexCoordValues == NULL ||
		nTexCoords == 0 ) {
		GenerateTextureCoordinates( pTexCoordValues, nTexCoords, 
									pCoordinates, nVerts );
		pTexCoordIndices = &(*m_coordIndex)[0];
		nTexCoordIndices = nIndices;


		bNukeTextureCoords = true;
	}

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	int i, j, vi, iVertexIndex;

	int MaxVertsInTriList = 3 * ( pDevice->GetMaxPrimitiveCount() - 100 );


	CAnmTriangleList *pTriangleList = NULL;

	tAnmVertexArray *pVerts = NULL;

	pTriangleList = new CAnmTriangleList(pDevice);
	assert(pTriangleList);
	pVerts = pTriangleList->GetVertices();



	Point3 * pvertices = &(*pCoordinates)[0];


	// Set up the face indices
	int *pSrcIndices = &(*m_coordIndex)[0];
	Point3 *normals = pNormalValues ? &(*pNormalValues)[0] : NULL;
	Color *colors = pColorValues ? &(*pColorValues)[0] : NULL;
	Point2 *uvs = pTexCoordValues ? &(*pTexCoordValues)[0] : NULL;

	// We'll pick a likely number of indices to pre-allocate based on
	// 3 indices per face; the array will expand as needed
	int maxfaces = nIndices / 4 + 1;
	
//	pIndices->reserve(maxfaces * 3);

	int *pface = pSrcIndices;
	int *pnorm = pNormalIndices;
	int *pcolor = pColorIndices;
	int *ptex = pTexCoordIndices;
	if( pGeneratedNormalIndex ) {
		pnorm = pGeneratedNormalIndex;
	}

	// krv Create UsageArray to determine if a particular vertex has Been set before.


	// krv:  Create buffers to track indecies of Texture, color, and Normal
	// Get Normal Buffer
	// convex = TRUE;
	int nVertAdded=0;

	int faceindex = 0;
	for (i = 0; i < nIndices ; )
	{
		int vertexcount = 0;
		while(pSrcIndices[i] != -1 && i < nIndices)
		{
			vertexcount++;
			i++;

			//N.B.: should issue a warning if we never find a -1
		}

		if (vertexcount >= 3)
		{
			int iOriginalVertexCount = vertexcount;
			int *pfacedata = pface;
			int *tesseldata = NULL;
			bool bUsingTessData = false;
			if (!m_convex && vertexcount > 3 )
			{
				tesseldata = Tesselate(pCoordinates, pface, vertexcount); // vertexcount might change
				assert( tesseldata );
				if (tesseldata) {
					pfacedata = tesseldata;
					bUsingTessData = true;
				}
			}

			// make a fan, or use triangulated data
			for (j = 0; ; j++)
			{
				int v[3], vn[3], vc[3], vt[3];
				int i0, i1, i2;

				if (!bUsingTessData && j >= vertexcount - 2) {
					break;
				}
				else if (bUsingTessData && j >= vertexcount / 3) {
					break;
				}


				if (!bUsingTessData) {
					// local indecies for data generated by tesselate
					i0 = 0, i1 = j + 1, i2 = j + 2;
				}
				else {
					i0 = j * 3, i1 = j * 3 + 1, i2 = j * 3 + 2;
					i0 = pfacedata[j * 3];
					i1 = pfacedata[j * 3 + 1];
					i2 = pfacedata[j * 3 + 2];
					if( i0 < 0 || i0 >= iOriginalVertexCount || pface[i0] < 0 ) {
						i0 = 0;
						assert( 0 );
					}
					if( i1 < 0 || i1 >= iOriginalVertexCount || pface[i1] < 0 ) {
						i1 = 0;
						assert( 0 );
					}
					if( i2 < 0 || i2 >= iOriginalVertexCount || pface[i2] < 0 ) {
						i2 = 0;
						assert( 0 );
					}
				}

				v[0] = pface[i0];
				v[1] = pface[i1];
				v[2] = pface[i2];

				if ( pnorm && ( m_normalPerVertex || pGeneratedNormalIndex ) )
				{
					vn[0] = pnorm[i0];
					vn[1] = pnorm[i1];
					vn[2] = pnorm[i2];
				}

				if (pcolor && m_colorPerVertex)
				{
					vc[0] = pcolor[i0];
					vc[1] = pcolor[i1];
					vc[2] = pcolor[i2];
				}

				if (ptex)
				{
					vt[0] = ptex[i0];
					vt[1] = ptex[i1];
					vt[2] = ptex[i2];
				}

				if (v[0] >= nVerts || v[0] < 0 ) {
					v[0] = nVerts - 1;
					assert( 0 );
				}
				if (v[1] >= nVerts || v[1] < 0 ) {
					v[1] = nVerts - 1;
					assert( 0 );
				}
				if (v[2] >= nVerts || v[2] < 0 ) {
					v[2] = nVerts - 1;
					assert( 0 );
				}


				// now get the normals, colors and texture coords

				for (vi = 0; vi < 3; vi++)
				{
					iVertexIndex = v[vi];

					sAnmVertex vert(pvertices[iVertexIndex], Point3(0, 0, 0), Point2(-1, -1));

					// krv:  If we have normals that were generated, then use these
					//
					if( pGeneratedNormalValues &&
						pGeneratedNormalIndex ) {

						int nindex = vn[vi];
						if (nindex < 0 || nindex >= nNormals ) {
							nindex = 0;
						}


						float* pNThis = &pGeneratedNormalValues[3*nindex];
						Point3 normal( pNThis[0], pNThis[1], pNThis[2] );
						vert.normal = normal;

					}
					else if (normals) {
						int nindex;

						if (pnorm)
						{
							if (m_normalPerVertex) {
								nindex = vn[vi];
							}
							else {
								nindex = pnorm[faceindex];
							}
						}
						else {
							nindex = faceindex;
						}

						if (nindex < 0 || nindex >= nNormals) {
							nindex = 0;
						}

						vert.normal = normals[nindex];
						vert.normal.Normalize();
					}
					else {
						// krv:  We must have normals!
						ASSERT( 0 );
						return;
					}




					if (colors)
					{
						int cindex;

						if (pcolor)
						{
							if (m_colorPerVertex)
							{
								cindex = vc[vi];
							}
							else
								cindex = pcolor[faceindex];

						}
						else
							cindex = faceindex;

						if (cindex < 0 || cindex >= nColors) {
							cindex = 0;
						}

						vert.SetDiffuseColor(colors[cindex]);
					}

					if (ptex && uvs)
					{
						int tindex = vt[vi];

						if (tindex < 0 || tindex >= nTexCoords) {
							tindex = 0;
						}
						vert.texCoord = uvs[tindex];
					}
					pVerts->push_back(vert);
					nVertAdded++;
				}
				if( nVertAdded > MaxVertsInTriList ) {
					
					if (!m_solid) {
						pTriangleList->GenerateBackfaces(pDevice);
					}

					
					// Sort the thing for picking
					pTriangleList->Sort();

					// Try to buffer the vertex list in hardware; may or may not succeed but ok either way
					eAnmReturnStatus retval = pTriangleList->BufferVertices(pApp->GetDevice());


					m_mesh->AddPrimitive(pTriangleList);

					// Set stage information
					pTriangleList->SetTextureStage(texturestage);


					pTriangleList = new CAnmTriangleList(pDevice);
					assert(pTriangleList);
					pVerts = pTriangleList->GetVertices();
					nVertAdded= 0;
				}


			}

			if (tesseldata != NULL) {
				delete tesseldata;
			}

		}
		else
		{
			// N.B.: junk face; < 3 vertices. issue warning?
			int err = 1;
		}

		pface = &pSrcIndices[++i];
		faceindex++;

		if (pNormalIndices && m_normalPerVertex)
			pnorm = &pNormalIndices[i];
		if( pGeneratedNormalIndex ) 
			pnorm = &pGeneratedNormalIndex[i];
		if (pColorIndices && m_colorPerVertex)
			pcolor = &pColorIndices[i];
		if (pTexCoordIndices)
			ptex = &pTexCoordIndices[i];

	}

	// Generate backfaces if the thing isn't solid

	if (!m_solid)
		pTriangleList->GenerateBackfaces(pDevice);

	// Sort the thing for picking
	pTriangleList->Sort();

	// Try to buffer the vertex list in hardware; may or may not succeed but ok either way
	eAnmReturnStatus retval = pTriangleList->BufferVertices(pApp->GetDevice());

	// Add the faces to our mesh
	m_mesh->AddPrimitive(pTriangleList);

	// Set stage information
	pTriangleList->SetTextureStage(texturestage);


	m_mesh->AddTextureStage(texturestage);

	// krv:  Delete our usageMask
	if( bNukeTextureCoords ) {
		delete pTexCoordValues;
	}
}
















void CAnmIndexedFaceSet::CreateFacesIndexedTriList(
									 int texturestage, 
									 Point3Array *pCoordinates, Point3Array *pNormalValues, ColorArray *pColorValues, Point2Array *pTexCoordValues, 
									 float* pGeneratedNormalValues,
									 int nVerts, int nNormals, int nColors, int nTexCoords, 
									 int *pNormalIndices, int *pColorIndices, int *pTexCoordIndices, int* pGeneratedNormalIndex,
									 int nIndices, int nNormalIndices, int nColorIndices, int nTexCoordIndices,
									 bool generateUVs)
{

	// krv:  if we dont have any Texture COords, then go get some.
	// ??? DO we need to check if we have a texture.  No need to generate coords if we dont got no texture.
	//

	bool bNukeTextureCoords = false;
	if( pTexCoordValues == NULL ||
		nTexCoords == 0 ) {
		GenerateTextureCoordinates( pTexCoordValues, nTexCoords, 
									pCoordinates, nVerts );
		pTexCoordIndices = &(*m_coordIndex)[0];
		nTexCoordIndices = nIndices;


		bNukeTextureCoords = true;
	}

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	int i, j, vi, iVertexIndex;



	CAnmTriangleList *pTriangleList = NULL;
	CAnmIndexedTriangleList *pIndexedTriangleList = NULL;

	tAnmVertexArray *pVerts = NULL;
	tAnmIndexArray *pIndices = NULL;


	pIndexedTriangleList = new CAnmIndexedTriangleList(pDevice);
	pTriangleList = pIndexedTriangleList;

	assert(pIndexedTriangleList);

	pVerts = pIndexedTriangleList->GetVertices();
	pIndices = pIndexedTriangleList->GetIndices();

	// Copy the vertices, with bad normal, tex coord, color values (well, color not really bad)
	Point3 * pvertices = &(*pCoordinates)[0];
	pVerts->reserve(nVerts);



	for (i = 0; i < nVerts; i++)
	{
		sAnmVertex v(pvertices[i], Point3(0, 0, 0), Point2(-1, -1));
		pVerts->push_back(v);
	}


	// Set up the face indices
	int *pSrcIndices = &(*m_coordIndex)[0];
	Point3 *normals = pNormalValues ? &(*pNormalValues)[0] : NULL;
	Color *colors = pColorValues ? &(*pColorValues)[0] : NULL;
	Point2 *uvs = pTexCoordValues ? &(*pTexCoordValues)[0] : NULL;

	// We'll pick a likely number of indices to pre-allocate based on
	// 3 indices per face; the array will expand as needed
	int maxfaces = nIndices / 4 + 1;
	
	pIndices->reserve(maxfaces * 3);

	int *pface = pSrcIndices;
	int *pnorm = pNormalIndices;
	int *pcolor = pColorIndices;
	int *ptex = pTexCoordIndices;
	if( pGeneratedNormalIndex ) {
		pnorm = pGeneratedNormalIndex;
	}

	// krv Create UsageArray to determine if a particular vertex has Been set before.

	unsigned char* pUsageArray = new unsigned char[nVerts];
	if( !pUsageArray ) {
		return;
	}

	unsigned char* pUsageArrayTmp = pUsageArray;
	for( int iUse=0; iUse<nVerts; iUse++ ) {
		*pUsageArrayTmp++ = 0;
	}


	// krv:  Create buffers to track indecies of Texture, color, and Normal
	// pNormalIndexOfVertex[iVertex] = the Index of the Normal that is used by the original Vertex.
	// pColorIndexOfVertex[iVertex] = the Index of the Color that is used by the original Vertex.
	// pTextureIndexOfVertex[iVertex] = the Index of the Texture Coordinate that is used by the original Vertex.

	int* pNormalIndexOfVertex = NULL;
	int* pColorIndexOfVertex =  NULL;
	int* pTextureIndexOfVertex = NULL;

	// Get Normal Buffer
	if( pGeneratedNormalValues || ( normals && pnorm && m_normalPerVertex ) ) {
		pNormalIndexOfVertex = new int[nVerts];
		if( !pNormalIndexOfVertex ) {
			return;
		}
		int* pTmp = pNormalIndexOfVertex;
		for( int i=0; i<nVerts; i++ ) {
			*pTmp++ = 0;
		}
	}

	if( ptex && uvs ) {
		pTextureIndexOfVertex = new int[nVerts];
		if( !pTextureIndexOfVertex ) {
			return;
		}
		int* pTmp = pTextureIndexOfVertex;
		for( int i=0; i<nVerts; i++ ) {
			*pTmp++ = 0;
		}
	}

	if( colors && pcolor && m_colorPerVertex ) {
		pColorIndexOfVertex = new int[nVerts];
		if( !pColorIndexOfVertex ) {
			return;
		}
		int* pTmp = pColorIndexOfVertex;
		for( int i=0; i<nVerts; i++ ) {
			*pTmp++ = 0;
		}
	}


	// convex = TRUE;

	int faceindex = 0;
	for (i = 0; i < nIndices; )
	{
		int vertexcount = 0;
		while(pSrcIndices[i] != -1 && i < nIndices)
		{
			vertexcount++;
			i++;

			//N.B.: should issue a warning if we never find a -1
		}

		if (vertexcount >= 3)
		{
			int iOriginalVertexCount = vertexcount;
			int *pfacedata = pface;
			int *tesseldata = NULL;
			bool bUsingTessData = false;
			if (!m_convex && vertexcount > 3 )
			{
				tesseldata = Tesselate(pCoordinates, pface, vertexcount); // vertexcount might change
				assert( tesseldata );
				if (tesseldata) {
					pfacedata = tesseldata;
					bUsingTessData = true;
				}
			}

			// make a fan, or use triangulated data
			for (j = 0; ; j++)
			{
				int v[3], vn[3], vc[3], vt[3];
				int i0, i1, i2;

				if (!bUsingTessData && j >= vertexcount - 2) {
					break;
				}
				else if (bUsingTessData && j >= vertexcount / 3) {
					break;
				}


				if (!bUsingTessData) {
					// local indecies for data generated by tesselate
					i0 = 0, i1 = j + 1, i2 = j + 2;
				}
				else {
					i0 = j * 3, i1 = j * 3 + 1, i2 = j * 3 + 2;
					i0 = pfacedata[j * 3];
					i1 = pfacedata[j * 3 + 1];
					i2 = pfacedata[j * 3 + 2];
					if( i0 < 0 || i0 >= iOriginalVertexCount || pface[i0] < 0 ) {
						i0 = 0;
						assert( 0 );
					}
					if( i1 < 0 || i1 >= iOriginalVertexCount || pface[i1] < 0 ) {
						i1 = 0;
						assert( 0 );
					}
					if( i2 < 0 || i2 >= iOriginalVertexCount || pface[i2] < 0 ) {
						i2 = 0;
						assert( 0 );
					}
				}

				v[0] = pface[i0];
				v[1] = pface[i1];
				v[2] = pface[i2];

				if ( pnorm && ( m_normalPerVertex || pGeneratedNormalIndex ) )
				{
					vn[0] = pnorm[i0];
					vn[1] = pnorm[i1];
					vn[2] = pnorm[i2];
				}

				if (pcolor && m_colorPerVertex)
				{
					vc[0] = pcolor[i0];
					vc[1] = pcolor[i1];
					vc[2] = pcolor[i2];
				}

				if (ptex)
				{
					vt[0] = ptex[i0];
					vt[1] = ptex[i1];
					vt[2] = ptex[i2];
				}

				if (v[0] >= nVerts || v[0] < 0 ) {
					v[0] = nVerts - 1;
					assert( 0 );
				}
				if (v[1] >= nVerts || v[1] < 0 ) {
					v[1] = nVerts - 1;
					assert( 0 );
				}
				if (v[2] >= nVerts || v[2] < 0 ) {
					v[2] = nVerts - 1;
					assert( 0 );
				}


				// now get the normals, colors and texture coords

				sAnmVertex vert;

				for (vi = 0; vi < 3; vi++)
				{
					iVertexIndex = v[vi];
					vert = (*pVerts)[ iVertexIndex ];

					BOOL somethingchanged = FALSE;


					// krv:  If we have normals that were generated, then use these
					//
					if( pGeneratedNormalValues &&
						pGeneratedNormalIndex ) {

						int nindex = vn[vi];
						if (nindex < 0 || nindex >= nNormals ) {
							nindex = 0;
						}

						if ( pUsageArray[iVertexIndex] ) {
							// Test for the same index
							//
							if( nindex != pNormalIndexOfVertex[iVertexIndex] ) {
								// We must set the Vertex Normal.
								float* pNThis = &pGeneratedNormalValues[3*nindex];
								Point3 normal( pNThis[0], pNThis[1], pNThis[2] );
								vert.normal = normal;
								somethingchanged = TRUE;
							}
						}
						else {
							// Virgin Vertex.  Set the Normal.
							float* pNThis = &pGeneratedNormalValues[3*nindex];
							Point3 normal( pNThis[0], pNThis[1], pNThis[2] );
							vert.normal = normal;
							pNormalIndexOfVertex[iVertexIndex] = nindex;
						}

					}
					else if (normals) {
						int nindex;

						if (pnorm)
						{
							if (m_normalPerVertex)
							{
								nindex = vn[vi];
							}
							else
								nindex = pnorm[faceindex];
						}
						else
							nindex = faceindex;

						if (nindex < 0 || nindex >= nNormals)
							nindex = 0;

						// Check if the Vertex has been set before.
						//
						if( pUsageArray[iVertexIndex] ) {
							if( pNormalIndexOfVertex ) {
								// Do test with index.
								if( nindex != pNormalIndexOfVertex[iVertexIndex] ) {
									// Different Index, Set and Normal, and somethingchanged.
									Point3 normal = normals[nindex];
									// if (!m_ccw)
									//	normal = -normal;
									normal.Normalize();
									vert.normal = normal;
									somethingchanged = TRUE;
								}
							}
							else {
								// Do test with Normal.
								Point3 normal = normals[nindex];
								// if (!m_ccw)
								//	normal = -normal;
								normal.Normalize();
								if( !( vert.normal == normal ) ) {
									vert.normal = normal;
									somethingchanged = TRUE;
								}
							}
						}
						else {
							// Virgin Vertex.
							Point3 normal = normals[nindex];
							// if (!m_ccw)
							//	normal = -normal;
							normal.Normalize();
							vert.normal = normal;
							if( pNormalIndexOfVertex ) {
								pNormalIndexOfVertex[iVertexIndex] = nindex;
							}
						}








						Point3 normal = normals[nindex];
						// if (!m_ccw)
						//	normal = -normal;

						// krv:  New place for normalize, before compare.
						//
						normal.Normalize();

						if ( ( pUsageArray[iVertexIndex] ) &&
							!( vert.normal == normal)) {
							somethingchanged = TRUE;
						}

						vert.normal = normal;
					}
					else {
						// krv:  We must have normals!
						ASSERT( 0 );
						return;
					}




					if (colors)
					{
						int cindex;

						if (pcolor)
						{
							if (m_colorPerVertex)
							{
								cindex = vc[vi];
							}
							else
								cindex = pcolor[faceindex];

						}
						else
							cindex = faceindex;

						if (cindex < 0 || cindex >= nColors)
							cindex = 0;


						// this should work ok; none of our X3D colors
						// have alpha < 1, so a color 0 should never happen in the content

						// Check if the Vertex has been set before.
						//
						if( pUsageArray[iVertexIndex] ) {
							if( pColorIndexOfVertex ) {
								// Do test with index.
								if( cindex != pColorIndexOfVertex[iVertexIndex] ) {
									vert.SetDiffuseColor(colors[cindex]);
									somethingchanged = TRUE;
								}
							}
							else {
								// Do test with Color.
								color4 c = colors[cindex];
								if( vert.diffuseColor != c.MakeDWord() ) {
									vert.SetDiffuseColor(c);
									somethingchanged = TRUE;
								}
							}
						}
						else {
							// Virgin Vertex.
							vert.SetDiffuseColor(colors[cindex]);
							if( pColorIndexOfVertex ) {
								pColorIndexOfVertex[iVertexIndex] = cindex;
							}
						}
					}

					if (ptex && uvs)
					{
						int tindex = vt[vi];

						if (tindex < 0 || tindex >= nTexCoords)
							tindex = 0;

						if( pUsageArray[iVertexIndex] ) {
							// Test if tindex is the same.
							if( tindex != pTextureIndexOfVertex[iVertexIndex] ) {
								vert.texCoord = uvs[tindex];
								somethingchanged = true;
							}
						}
						else {
							// virgin vertex.  Set Tex Coord.
							vert.texCoord = uvs[tindex];
							pTextureIndexOfVertex[iVertexIndex] = tindex;
						}


						// X3D->D3D
						// tc.y = 1.f - tc.y;
/************
						if ( !somethingchanged &&
							  pUsageArray[iVertexIndex] &&
							  (vert.texCoord.x != tc.x || vert.texCoord.y != tc.y )) {
							somethingchanged = TRUE;
						}
						vert.texCoord = tc;
**************/
					}
					pUsageArray[iVertexIndex] = 1;

					// let's have a sane normal
					// krv:  THis is a very inefficet place to do this!
//					vert.normal.Normalize();

					// somethingchanged = FALSE;

					// set the data back into the vertex array. if the same
					// coordinate has a different color, normal or UV create a
					// new vertex; otherwise overwrite the old one with the
					// color, normal, UV info
					if (somethingchanged)
					{
						int newvindex = pVerts->size();
						pVerts->push_back(vert);
						v[vi] = newvindex;
					}
					else
					{
						(*pVerts)[ v[vi] ] = vert;
					}
				}

				pIndices->push_back(v[0]);
				if (m_ccw)
				{
					pIndices->push_back(v[1]);
					pIndices->push_back(v[2]);
				}
				else
				{
					pIndices->push_back(v[2]);
					pIndices->push_back(v[1]);
				}

			}

			if (tesseldata != NULL)
				delete tesseldata;

		}
		else
		{
			// N.B.: junk face; < 3 vertices. issue warning?
			int err = 1;
		}

		pface = &pSrcIndices[++i];
		faceindex++;

		if (pNormalIndices && m_normalPerVertex)
			pnorm = &pNormalIndices[i];
		if( pGeneratedNormalIndex ) 
			pnorm = &pGeneratedNormalIndex[i];
		if (pColorIndices && m_colorPerVertex)
			pcolor = &pColorIndices[i];
		if (pTexCoordIndices)
			ptex = &pTexCoordIndices[i];

	}

	// Generate backfaces if the thing isn't solid
	if (!m_solid)
		pTriangleList->GenerateBackfaces(pDevice);

	// Sort the thing for picking
	pTriangleList->Sort();

	// Try to buffer the vertex list in hardware; may or may not succeed but ok either way
	eAnmReturnStatus retval = pTriangleList->BufferVertices(pApp->GetDevice());

	// Add the faces to our mesh
	m_mesh->AddPrimitive(pTriangleList);

	// Set stage information
	pTriangleList->SetTextureStage(texturestage);
	m_mesh->AddTextureStage(texturestage);

	// krv:  Delete our usageMask
	delete [] pUsageArray;
	if( pNormalIndexOfVertex ) {
		delete [] pNormalIndexOfVertex;
	}
	if( pTextureIndexOfVertex ) {
		delete [] pTextureIndexOfVertex;
	}
	if( pColorIndexOfVertex ) {
		delete [] pColorIndexOfVertex;
	}

	if( bNukeTextureCoords ) {
		delete pTexCoordValues;
	}
}



// krv: clear will zero out an array.
//
static void clear( int n, float pval[] )
{
	int i;
	for(i=0; i<n; i++, pval++)
		*pval = 0.0f;
}


// krv: vcrosu:  Cross Multiply two vectors
//
void vcrosu( float vec1[], float vec2[], float vec[] )
{
	vec[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
	vec[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
	vec[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];
}

// krv: vplusu:  Add two vectors
//
void vplusu( int n, float R1[], float R2[], float R3[] )
{
	int i;
	for(i=0; i<n; i++)
		R3[i] = R2[i] + R1[i];
}


// krv: absvec:  Normalize vector
//
float absvec( int n, float vec[] )
{
	float mag=0.0f;
	int i;

	for(i=0; i<n; i++) {
		mag += vec[i]*vec[i];
	}

	if(mag < ZEROF) {
		return 0.0f;
	}

	mag = (float) sqrt(mag);
	for(i=0; i<n; i++) {
		vec[i] /= mag;
	}

	return mag;
}


// krv: vdotu:; get dot product of two vectors.
//
float vdotu( int n, float vec1[], float vec2[] )
{
	float mag=0.0;
	int i;

	for(i=0; i<n; i++)
		mag += vec1[i]*vec2[i];
	return mag;
}

// krv: vequ:  Copy an array.
//
void vequ( int n, const float R1[], float R2[] )
{
	int i;

	for(i=0; i<n; i++)
		R2[i] = R1[i];
}



// krv:  A structure used to store information for a Face that is attached to a Vertex.
//       For each Vertex, and array of thses is generated, one Element for each Face that touches the Vertex.
typedef struct tagVertexFaceInfo
{
	int iPrev;  // Index of Previous Vertex on this face.
	int iNext;  // Index of Next Vertex on this face.
	int iFace;  // Index of this Face
	int iIndexOfIndex;  // Index of Index of this Vertex
	int iGroup;			// Group Index.
} VertexFaceInfo;



#define MAXFACEPERVERT		300
#define MAXGROUP		    300



bool CAnmIndexedFaceSet::PreGenerateNormals( float** ppNormalArray, int** ppNormIndexArray, int &nGeneratedNormals )
{

	// Extract Coordinate Data.
	CAnmCoordinate *pCoordNode = GETIMPLEMENTATION(CAnmCoordinate, m_coord);
	if( pCoordNode ) {

		Point3Array *pCoordinates = pCoordNode->GetPoint();

		if( pCoordinates ) {

			return PreGenerateNormals( ppNormalArray, ppNormIndexArray, nGeneratedNormals,
					 pCoordinates,
					 m_coordIndex,
					 m_ccw,
					 m_creaseAngle );
		}
	}
	return false;
}



// krv  PreGenerateNormals will calculate an array of Normal Vectors for Each Vertex, for each face.
//      
bool CAnmIndexedFaceSet::PreGenerateNormals( float** ppNormalArray, int** ppNormIndexArray, int &nGeneratedNormals,
		 Point3Array*  pCoordinates,
		 IntegerArray* coordIndex,
		 bool ccw,
		 float creaseAngle )
{

	float * pNormalArray = NULL;
	int*    pNormalIndexArray = NULL;

	// Extract CoordinateIndex array.
	int *pSrcIndices = &(*coordIndex)[0];
	int nIndices = coordIndex->size();

	// Extract Coordinate Data.
	if( pSrcIndices && pCoordinates ) {

		int i;
		int index;
		int iOffset;
		int* pFaceData;

		int nVerts = pCoordinates->size();
		Point3 *pvertices = &(*pCoordinates)[0];


		// Make room for output array of Normals.
		float * pNormalArray = new float[nIndices*3];
		int*    pNormalIndexArray = new int[nIndices];

		if( !pNormalArray || !pNormalIndexArray ) {
			return false;
		}

		// Init the Norm Index Array with -1 for end of face.
		//
		for( int iNormIndex=0; iNormIndex<nIndices; iNormIndex++ ) {
			pNormalIndexArray[iNormIndex] = ( ( pSrcIndices[iNormIndex] >= 0 ) ? 0 : -1 );
		}


		int     nNormalsInOutputArray = 0;

		float* pNormalArrayTmp = pNormalArray;


		// Count faces in mesh,
		// Also build array of the useage count for ach vertex.
		int nFacesInMesh = 0;
		int* pVertToFaceIndex = new int[nVerts+1];
		if( !pVertToFaceIndex ) {
			return false;
		}


		int* pTmpVertMap = pVertToFaceIndex;
		// Clear backpointer list.
		//
		for( i=0; i<nVerts; i++ ) {
			*pTmpVertMap++ = 0;
		}

		int* pInd = pSrcIndices;
		for (i = 0; i < nIndices; )
		{
			int vertexcount = 0;
			while( ( index = *pInd ) != -1 && i < nIndices)
			{
				pVertToFaceIndex[index]++;
				vertexcount++;
				i++;
				pInd++;
			}
			nFacesInMesh++;
			i++;
			pInd++;
		}

		// Convert usage count array to offset array.
		int iLastCum = 0;
		int iLastValue = 0;
		pTmpVertMap = pVertToFaceIndex;
		for( i=0; i<nVerts; i++, pTmpVertMap++ ) {
			iLastValue = *pTmpVertMap;
			*pTmpVertMap = iLastCum;
			iLastCum += iLastValue;
		}
		pVertToFaceIndex[nVerts] = iLastCum;

		int nSizeOfVertToFaceData = iLastCum;

		int* pVertexToFaceData = new int[nSizeOfVertToFaceData];
		if( !pVertexToFaceData ) {
			return false;
		}


		int* pTmp = pVertexToFaceData;
		for( i=0; i<nSizeOfVertToFaceData; i++ ) {
			*pTmp++ = -1;
		}

		int* pFaceToFirstIndex = new int[nFacesInMesh];
		if( !pFaceToFirstIndex ) {
			return false;
		}


		// Now populate the pVertexToFaceData array.
		pInd = pSrcIndices;
		int iFace = 0;
		for (i = 0; i < nIndices; )
		{
			pFaceToFirstIndex[iFace] = i;
			int vertexcount = 0;
			while( ( index = *pInd ) != -1 && i < nIndices)
			{
				iOffset = pVertToFaceIndex[index];
				pFaceData = pVertexToFaceData + iOffset;

				// Find the next empty slot, and set the FaceIndex.
				//
				while( *pFaceData >= 0 ) {
					pFaceData++;
				}
				*pFaceData = iFace;


				vertexcount++;
				i++;
				pInd++;
			}
			iFace++;
			i++;
			pInd++;
		}









		// Calcualte the Face Normals.
		float* pFaceNormals = new float [nFacesInMesh*3];
		if( !pFaceNormals ) {
			return false;
		}


		int i0, i1, i2, iLast, iThis, iFirst;

		float v1[3], v2[3], dot;
		int iface = 0;
		float pfacenrm[3];
		float* pThisFaceNormal;

		iFace = 0;
		for (i = 0; i < nIndices; )
		{
			int iFirstVert = i;
			int vertexcount = 0;
			while(pSrcIndices[i] != -1 && i < nIndices)
			{
				vertexcount++;
				i++;
			}
			pThisFaceNormal = &pFaceNormals[iFace*3];

			if (vertexcount >= 3)
			{
				clear( 3, pThisFaceNormal );
				iFirst = pSrcIndices[iFirstVert];
				for( int iVert=2; iVert<vertexcount; iVert++ ) {
					// Calualte the area of this triangle.
					//
					iLast = pSrcIndices[iFirstVert + iVert-1];
					iThis = pSrcIndices[iFirstVert + iVert];
					Point3 dpnt1 = pvertices[iLast] - pvertices[iFirst];
					Point3 dpnt2 = pvertices[iThis] - pvertices[iFirst];
					v1[0] = dpnt1.x;
					v1[1] = dpnt1.y;
					v1[2] = dpnt1.z;
					v2[0] = dpnt2.x;
					v2[1] = dpnt2.y;
					v2[2] = dpnt2.z;
					if( ccw ) {
						vcrosu( v1,v2,pfacenrm );
					}
					else {
						vcrosu( v2,v1,pfacenrm );
					}
					// Add normal of this triangle to sum.

					vplusu( 3, pfacenrm, pThisFaceNormal, pThisFaceNormal );
				}
				if( absvec( 3, pThisFaceNormal ) < ZEROF ) {
					pThisFaceNormal[0] = 0.0f;
					pThisFaceNormal[1] = 0.0f;
					pThisFaceNormal[2] = 1.0f;
				}
			}
			else {
				pThisFaceNormal[0] = 0.0f;
				pThisFaceNormal[1] = 0.0f;
				pThisFaceNormal[2] = 1.0f;
			}
			i++;
			iFace++;
		}



		VertexFaceInfo VertexFaceInfoList[MAXFACEPERVERT];
		float avGroupNorm[MAXGROUP][3];
		int	  nGroupUsedCount[MAXGROUP];
		int* pind;
		int iind, inext, iprev, ifirst, ilast;

		int in1, if1;

		int nFacesOnThisVertex;
		int iIndexOfFirstFace;
		int iFaceOnThisVertex;
		int iIndexOfFirstIndex;
		int iIndexOfThisIndex;
		int iLoc;


		float crease_dot = cos( creaseAngle ) - 0.01;

		bool bAllFlat = ( creaseAngle < 0.001 );
		bool bAllSmooth = ( creaseAngle > 3.1 );


		for( int ivert=0; ivert<nVerts; ivert++ ) {
			ilast = -1;
			iface = 0;

			//
			nFacesOnThisVertex = pVertToFaceIndex[ivert+1] - pVertToFaceIndex[ivert];
			iIndexOfFirstFace = pVertToFaceIndex[ivert];


			// Now fill our VertextListData with the four key values;
			// One item for each Face touching this vertex.
			// 0 : LPREV ; Index of Previous Vertex on the Face.
			// 1 : LNEXT ; Index of Previous Vertex on the Face.
			// 2 : LFACE ; Index of Previous Vertex on the Face.
			// 3 : LINDEX ; Index of Index of this vertex the face.
			// 4 : LGROUP : Group Index of this Vertex on the Face ( Each Group has a unique normal )

			for( iFaceOnThisVertex=0; iFaceOnThisVertex<nFacesOnThisVertex; iFaceOnThisVertex++ ) {
				iFace = pVertexToFaceData[iIndexOfFirstFace + iFaceOnThisVertex ];
				iIndexOfFirstIndex = pFaceToFirstIndex[iFace]; 

				iprev = -1;
				iIndexOfThisIndex = -1;
				for( iLoc=0, pind = &pSrcIndices[iIndexOfFirstIndex]; pind && *pind != -1; iLoc++ ) {
					// If the Vertex is in the face twice, we find the wronge one.
					//
					if( *pind == ivert ) {

						inext = pind[1];
						// If this is the Last Vert On Face, then we need to get the First Vertext to use as the 
						//  Next vert.
						if( inext == -1  || iLoc + iIndexOfFirstIndex >= nIndices ) {
							inext = pSrcIndices[iIndexOfFirstIndex];
						}
						// If this is the First Vert On Face, then we need to find the Last vert to use as
						// the Prev Vert.
						if( iprev == -1 ) {
							while( *pind != -1 && ( pind - pSrcIndices ) < nIndices ) {
								iprev = *pind++;
							}
						}
						iIndexOfThisIndex = iIndexOfFirstIndex + iLoc;
						pind = NULL;
					}
					else {
						iprev = *pind;
						pind++;
					}
				}

				if( iIndexOfThisIndex < 0 ) {
					ASSERT( 0 );
					return false;
				}
				
				VertexFaceInfo* pData = &VertexFaceInfoList[iFaceOnThisVertex];
				pData->iPrev = iprev;
				pData->iNext = inext;
				pData->iFace = iFace;
				pData->iIndexOfIndex = iIndexOfThisIndex;
				pData->iGroup = -1;
			}



			int iface1;
			int iIndexOfIndex;

			int iNextNormGroup = 0;

			// Traverse the Edges attached to this vertes.
			// Need to find two faces that share both this vertex, AND another vertex.
			// 
			for( iface1=0; iface1<nFacesOnThisVertex; iface1++ ) {
				iprev = VertexFaceInfoList[iface1].iPrev;
				inext = VertexFaceInfoList[iface1].iNext;
				for( int iface2=iface1+1; iface2<nFacesOnThisVertex; iface2++ ) {
					if( iprev == VertexFaceInfoList[iface2].iNext ||
							inext == VertexFaceInfoList[iface2].iPrev ) {
						// Found an Edge.
						// Get Dot for crease angle comparison.
						//
						dot = vdotu( 3, pFaceNormals+3*VertexFaceInfoList[iface1].iFace,
											 pFaceNormals+3*VertexFaceInfoList[iface2].iFace );

						if( dot > crease_dot )
						{   

							// Need to Blend the edge from Face1 to Face2.
							// That means that Face1 and Face2 must go in a common GROUP.
							if( VertexFaceInfoList[iface1].iGroup >= 0 &&
								 VertexFaceInfoList[iface2].iGroup >= 0 &&
								 VertexFaceInfoList[iface1].iGroup != VertexFaceInfoList[iface2].iGroup ) {
								// Both Vertecies are already in two different groups
								// We need to Merge the two groups to one.
								// ALL verts in the old groups must move over.
								int iOldGroup = VertexFaceInfoList[iface2].iGroup;
								int iNewGroup = VertexFaceInfoList[iface1].iGroup;
								for( int iface3=0; iface3<nFacesOnThisVertex; iface3++ ) {
									if( VertexFaceInfoList[iface3].iGroup == iOldGroup ) {
										VertexFaceInfoList[iface3].iGroup = iNewGroup;
									}
								}
							}
							else if( VertexFaceInfoList[iface1].iGroup >= 0 &&
										VertexFaceInfoList[iface2].iGroup < 0 ) {
								// Face1 is in a group, face2 is not in a group yet.
								// So, put Face2 in the Face1 group
								VertexFaceInfoList[iface2].iGroup = VertexFaceInfoList[iface1].iGroup;
							}
							else if( VertexFaceInfoList[iface2].iGroup >= 0 &&
										VertexFaceInfoList[iface1].iGroup < 0 ) {
								// Face2 is in a group, face1 is not in a group yet.
								// So, put Face1 in the Face2 group
								VertexFaceInfoList[iface1].iGroup = VertexFaceInfoList[iface2].iGroup;
							}
							else if( VertexFaceInfoList[iface2].iGroup < 0 &&
										VertexFaceInfoList[iface1].iGroup < 0 &&
										iNextNormGroup < MAXGROUP) {
								// Neither face is in a Group.
								// Create a new Group.  Add both to it.
								VertexFaceInfoList[iface1].iGroup = iNextNormGroup;
								VertexFaceInfoList[iface2].iGroup = iNextNormGroup;
								iNextNormGroup++;
							}
						}
					}
				}
			}


			// Clear Vectors
			//
			for( int igroup=0; igroup<iNextNormGroup; igroup++ ) {
				clear( 3, avGroupNorm[igroup] );
				nGroupUsedCount[igroup] = 0;
			}
			// Sum normals for all groups to get an average normal
			//
			for( iface1=0; iface1<nFacesOnThisVertex; iface1++ ) {
				igroup = VertexFaceInfoList[iface1].iGroup;
				if( igroup >= 0 ) {
					if1 = VertexFaceInfoList[iface1].iFace;
					ASSERT( if1 >= 0 && if1 < nFacesInMesh );
					if( if1 >= 0 && if1 < nFacesInMesh ) {
						nGroupUsedCount[igroup]++;
						for( i=0; i<3; i++) {
							avGroupNorm[igroup][i] += (*( pFaceNormals+3*if1+i));
						}
					}
				}
			}
			// Force normals to unit vectors. 
			// Add them to our output array of normals.
			// ( Note, this does not include the faces that do not share a
			//    normal with another face, they will have -1 as Group Number, and will
			// use the Face Normal below.
			//

			int iFirstNorm = nNormalsInOutputArray;
			for( igroup=0; igroup<iNextNormGroup; igroup++ ) {
				if( nGroupUsedCount[igroup] > 1 ) {
					absvec( 3, avGroupNorm[igroup] );
				}
				vequ( 3, avGroupNorm[igroup], pNormalArrayTmp );
				pNormalArrayTmp+=3;
				nNormalsInOutputArray++;
			}

			// Lastly, place the normals in our output array.
			for( iface1=0; iface1<nFacesOnThisVertex; iface1++ ) {
				igroup = VertexFaceInfoList[iface1].iGroup;
				iIndexOfIndex = VertexFaceInfoList[iface1].iIndexOfIndex;
				if( igroup >= 0 ) {
					pNormalIndexArray[iIndexOfIndex] = iFirstNorm + igroup;
				}
				else {
					// Get the Face Normal, and add it to our list.
					//
					if1 = VertexFaceInfoList[iface1].iFace;
					float* pFaceNorm = &pFaceNormals[3*if1];
					vequ( 3, pFaceNorm, pNormalArrayTmp );

					pNormalIndexArray[iIndexOfIndex] = nNormalsInOutputArray;
					pNormalArrayTmp+=3;
					nNormalsInOutputArray++;

				}

			}
		}

		delete [] pFaceNormals;
		delete [] pVertToFaceIndex;
		delete [] pVertexToFaceData;
		delete [] pFaceToFirstIndex;


		*ppNormalArray = pNormalArray;
		*ppNormIndexArray = pNormalIndexArray;
		nGeneratedNormals = nNormalsInOutputArray;
		return true;
	}

	return false;
}


int *CAnmIndexedFaceSet::Tesselate(Point3Array *pCoords, int *indices, int &numIndices)
{
	// copy vertices 1 for 1 into new array
	std::vector<double> coords;
	for (int i = 0; i < numIndices; i++)
	{
		coords.push_back((double) (*pCoords)[indices[i]].x);
		coords.push_back((double) (*pCoords)[indices[i]].y);
		coords.push_back((double) (*pCoords)[indices[i]].z);
	}

	int nindices = numIndices;

	// tesselate; this will create a new index array and update the count
	CAnmTesselator t;
	int *newindices = t.Tesselate(&coords[0], nindices);

	if (!newindices)
		return NULL;

	// krv:  NO!  Dont do this!
	/********************************
	// now translate per-face indices back to global index offsets
	for (i = 0; i < nindices; i++)
	{
		newindices[i] = indices[newindices[i]];
	}
	******************************/

	if (nindices) {
		numIndices = nindices;
	}

	return nindices ? newindices : NULL;
}




void CAnmIndexedFaceSet::GenerateTextureCoordinates( 
				Point2Array* &pTexCoordValues, 
				int& nTexCoords, 
				Point3Array* pCoordinates, 
				int& nVerts ) 
{
	if( nVerts > 0 ) {
		pTexCoordValues = new Point2Array;
		pTexCoordValues->reserve(nVerts);

		float vMin[3];
		float vMax[3];
		float delta[3];

		vMin[0] = vMax[0] = (*pCoordinates)[0].x;
		vMin[1] = vMax[1] = (*pCoordinates)[0].y;
		vMin[2] = vMax[2] = (*pCoordinates)[0].z;

		for( int i=1; i<nVerts; i++ ) {
			for( int ixyz=0; ixyz<3; ixyz++ ) {
				if( vMin[ixyz] > ( *pCoordinates)[i].v[ixyz] ) {
					vMin[ixyz] = (*pCoordinates)[i].v[ixyz];
				}
				if( vMax[ixyz] < ( *pCoordinates)[i].v[ixyz] ) {
					vMax[ixyz] = (*pCoordinates)[i].v[ixyz];
				}
			}
		}


		int iMax = 0;
		int iNext = -1;
		float fMax = 0.0f;
		float fNext = 0.0f;

		// Calc the deltas, and find the Max Delta.
		//
		for( i=0; i<3; i++ ) {
			delta[i] = ( vMax[i] - vMin[i] );
			if( delta[i] == 0.0f ) {
				delta[i] = 0.000000001f;
			}
			if( i == 0 ) {
				iMax = 0;
				fMax = delta[i];
			}
			else {
				if( fMax < delta[i] ) {
					fMax = delta[i];
					iMax = i;
				}
			}
		}

		// find the Next largest Delta.
		//
		for( i=0; i<3; i++ ) {
			if( i != iMax ) {
				if( iNext < 0 ) {
					iNext = i;
					fNext = delta[i];
				}
				else {
					if( fNext < delta[i] ) {
						fNext = delta[i];
						iNext = i;
					}
				}
			}
		}

		// u goes from zero to one.
		// v goes from zero to vRatio.  ( that what the spec says! )
		//
		float vRatio = fNext / fMax;

		float uVal;
		float vVal;

		float fMaxDelta = delta[iMax];
		float fNextDelta = delta[iNext] / vRatio;

		float fMinOfMax = vMin[iMax];
		float fMinOfNext = vMin[iNext];


		for( i=0; i<nVerts; i++ ) {
			uVal = ( ( *pCoordinates)[i].v[iMax] -  fMinOfMax )  / fMaxDelta;
			vVal = ( ( *pCoordinates)[i].v[iNext] - fMinOfNext ) / fNextDelta;

			pTexCoordValues->push_back( Point2( uVal, vVal ) );
		}
		nTexCoords = nVerts;
	}
}





// CAnmComposedGeometry overrides
void CAnmIndexedFaceSet::Realize()
{
	CreateMesh();
	CAnmComposedGeometry::Realize();		// chain down

	ClearStateDirty();				// we're built, so we're clean
}

void CAnmIndexedFaceSet::SetColorIndices(IntegerArray *pColorIndices)
{
	assert(pColorIndices != NULL);

	SafeUnRef(m_colorIndex);
	m_colorIndex = pColorIndices;
	m_colorIndex->Ref();

	SetDirtyBits(eAnmIFSColorIndexDirty);
	SetStateDirty();
}

void CAnmIndexedFaceSet::SetConvex(Boolean convex)
{
	m_convex = convex;
	SetDirtyBits(eAnmIFSConvexDirty);
	SetStateDirty();
}

void CAnmIndexedFaceSet::SetCoordIndices(IntegerArray *pCoordIndices)
{
	assert(pCoordIndices != NULL);

	SafeUnRef(m_coordIndex);
	m_coordIndex = pCoordIndices;
	m_coordIndex->Ref();

	SetDirtyBits(eAnmIFSCoordIndexDirty);
	SetStateDirty();
}

void CAnmIndexedFaceSet::SetCreaseAngle(Float creaseAngle)
{
	m_creaseAngle = creaseAngle;
	SetDirtyBits(eAnmIFSCreaseAngleDirty);
	SetStateDirty();
}

void CAnmIndexedFaceSet::SetNormalIndices(IntegerArray *pNormalIndices)
{
	assert(pNormalIndices != NULL);

	SafeUnRef(m_normalIndex);
	m_normalIndex = pNormalIndices;
	m_normalIndex->Ref();

	SetDirtyBits(eAnmIFSNormalIndexDirty);
	SetStateDirty();
}

void CAnmIndexedFaceSet::SetTexCoordIndices(IntegerArray *pTexCoordIndices)
{
	assert(pTexCoordIndices != NULL);

	SafeUnRef(m_texCoordIndex);
	m_texCoordIndex = pTexCoordIndices;
	m_texCoordIndex->Ref();

	SetDirtyBits(eAnmIFSTexCoordIndexDirty);
	SetStateDirty();
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmIndexedFaceSet, ccw, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedFaceSet, color, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedFaceSet, coord, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedFaceSet, colorPerVertex, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedFaceSet, normal, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedFaceSet, normalPerVertex, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedFaceSet, solid, CAnmComposedGeometry);
DEFINE_INHERITED_VALUE(CAnmIndexedFaceSet, texCoord, CAnmComposedGeometry);

DEFINE_VALUE(CAnmIndexedFaceSet, colorIndex, eValueIntegerArray, IntegerArray*, 
			 GetColorIndices, SetColorIndices);
DEFINE_VALUE(CAnmIndexedFaceSet, convex, eValueBoolean, Boolean, 
			 GetConvex, SetConvex);
DEFINE_VALUE(CAnmIndexedFaceSet, coordIndex, eValueIntegerArray, IntegerArray*, 
			 GetCoordIndices, SetCoordIndices);
DEFINE_VALUE(CAnmIndexedFaceSet, creaseAngle, eValueFloat, Float, 
			 GetCreaseAngle, SetCreaseAngle);
DEFINE_VALUE(CAnmIndexedFaceSet, normalIndex, eValueIntegerArray, IntegerArray*, 
			 GetNormalIndices, SetNormalIndices);
DEFINE_VALUE(CAnmIndexedFaceSet, texCoordIndex, eValueIntegerArray, IntegerArray*,
			 GetTexCoordIndices, SetTexCoordIndices);

DEFINE_INHERITED_METHOD(CAnmIndexedFaceSet, set_color, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmIndexedFaceSet, set_coord, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmIndexedFaceSet, set_normal, CAnmComposedGeometry);
DEFINE_INHERITED_METHOD(CAnmIndexedFaceSet, set_texCoord, CAnmComposedGeometry);

DEFINE_METHOD(CAnmIndexedFaceSet, set_colorIndex, eValueIntegerArray, IntegerArray*, SetColorIndices);
DEFINE_METHOD(CAnmIndexedFaceSet, set_coordIndex, eValueIntegerArray, IntegerArray*, SetCoordIndices);
DEFINE_METHOD(CAnmIndexedFaceSet, set_normalIndex, eValueIntegerArray, IntegerArray*, SetNormalIndices);
DEFINE_METHOD(CAnmIndexedFaceSet, set_texCoordIndex, eValueIntegerArray, IntegerArray*, SetTexCoordIndices);

DEFINE_INHERITED_CALLBACK(CAnmIndexedFaceSet, color_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmIndexedFaceSet, coord_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmIndexedFaceSet, normal_changed, CAnmComposedGeometry);
DEFINE_INHERITED_CALLBACK(CAnmIndexedFaceSet, texCoord_changed, CAnmComposedGeometry);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmIndexedFaceSet)
VALUEID(CAnmIndexedFaceSet, ccw), 
VALUEID(CAnmIndexedFaceSet, color), 
VALUEID(CAnmIndexedFaceSet, colorIndex), 
VALUEID(CAnmIndexedFaceSet, colorPerVertex), 
VALUEID(CAnmIndexedFaceSet, convex), 
VALUEID(CAnmIndexedFaceSet, coord), 
VALUEID(CAnmIndexedFaceSet, coordIndex), 
VALUEID(CAnmIndexedFaceSet, creaseAngle), 
VALUEID(CAnmIndexedFaceSet, normal), 
VALUEID(CAnmIndexedFaceSet, normalIndex), 
VALUEID(CAnmIndexedFaceSet, normalPerVertex),
VALUEID(CAnmIndexedFaceSet, solid),
VALUEID(CAnmIndexedFaceSet, texCoord),
VALUEID(CAnmIndexedFaceSet, texCoordIndex),

METHODID(CAnmIndexedFaceSet, set_color), 
METHODID(CAnmIndexedFaceSet, set_colorIndex), 
METHODID(CAnmIndexedFaceSet, set_coord), 
METHODID(CAnmIndexedFaceSet, set_coordIndex), 
METHODID(CAnmIndexedFaceSet, set_normal), 
METHODID(CAnmIndexedFaceSet, set_normalIndex), 
METHODID(CAnmIndexedFaceSet, set_texCoord),
METHODID(CAnmIndexedFaceSet, set_texCoordIndex),

CALLBACKID(CAnmIndexedFaceSet, color_changed),
CALLBACKID(CAnmIndexedFaceSet, coord_changed),
CALLBACKID(CAnmIndexedFaceSet, normal_changed),
CALLBACKID(CAnmIndexedFaceSet, texCoord_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmIndexedFaceSet, CAnmComposedGeometry);


