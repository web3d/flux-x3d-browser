/********************************************************************************
 * Flux
 *
 * File: anmmesh.cpp
 * Description: Mesh class - support for triangle
 *							meshes
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
#include "anmmesh.h"
#include "anmvolumes.h"
#include "anmdevice.h"
#include "anmtexture.h"
#include "anmtexturetransform.h"

CAnmMesh::CAnmMesh()
{
	// we might want to pre-reserve space in the primitive list;
	// but I'm not sure what's a good number; our objects range from 1 to a few dozen
	// primitives

	m_maxTextureStage = 0;
}

CAnmMesh::~CAnmMesh()
{
	for (int i = 0; i < m_primitives.size(); i++)
	{
		SafeDelete(m_primitives[i]);
	}
}

void CAnmMesh::Render(class CAnmDevice *pDevice, int texturestage)
{


	// test if this Mesh has a Texture attached to it.
	//
	CAnmGraphics::Texture* pTextureSurface = GetTextureSurface();
	bool oldal;

	if( pTextureSurface ) {
		// if there is one, clear the textures state.
		//
		int maxTextureLevels = pDevice->MaxTextureLevels();
		for ( int i = 0; i < maxTextureLevels; i++) {
			pDevice->SetTexture(i, NULL);
		}
		// Now set the texture.
		bool alphablending = true;
		bool lighting = true;
		bool bRepeatS = true;
		bool bRepeatT = true;


		oldal = pDevice->GetAntialiasing();
		pDevice->SetAntialiasing(false);

		CAnmTexture::SetToDevice( pDevice, 
							  alphablending, lighting,
							  pTextureSurface,
							  bRepeatS,
							  bRepeatT );
// 		matrix4 myMat( matrix4::Identity );

		matrix4 defaultTTmat;
		pDevice->GetDefaultTextureTransform(&defaultTTmat);


		CAnmTextureTransform::SetToDevice( pDevice, &defaultTTmat );


		pDevice->SetTexture(0, pTextureSurface);

	}


int iLen= m_primitives.size();
	for ( int i = 0; i < m_primitives.size(); i++)
	{
		CAnmPrimitive* pPrim = m_primitives[i];
		if (m_maxTextureStage == 0 || (m_primitives[i]->GetTextureStage() == texturestage))
			m_primitives[i]->Render(pDevice);
	}

	if( pTextureSurface ) {
		pDevice->SetTexture(0, NULL);
		pDevice->SetAntialiasing(oldal);
	}
}


bool CAnmMesh::TestPick(Point3 start, Point3 end, sAnmRenderGraphPickList *pPickList)
{
	bool picked = false;
	for (int i = 0; i < m_primitives.size(); i++)
	{
		if (m_primitives[i]->TestPick(start, end, pPickList))
			picked = true;
	}

	return picked;
}

bool CAnmMesh::SphereCollide(Point3 center, float radius, sAnmRenderGraphPickList *pPickList)
{
	bool collide = false;
	for (int i = 0; i < m_primitives.size(); i++)
	{
		if (m_primitives[i]->SphereCollide(center, radius, pPickList))
			collide = true;
	}

	return collide;
}

bool CAnmMesh::CapsuleCollide(const Point3 &capsulePt0, const Point3 &capsulePt1, 
							  const Point3 &capsuleRadius, sAnmRenderGraphPickList *pPickList)
{
	bool collide = false;
	for (int i = 0; i < m_primitives.size(); i++)
	{
		if (m_primitives[i]->CapsuleCollide(capsulePt0, capsulePt1, capsuleRadius, pPickList))
			collide = true;
	}

	return collide;
}


void CAnmMesh::GetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	CAnmBoundingBox b;
	BOOL oncethru = FALSE;

	for (int i = 0; i < m_primitives.size(); i++)
	{
		m_primitives[i]->GetBoundingBox(&b);
		assert(b.IsValid());

		if (!oncethru)
		{
			*pBoundingBox = b;
			oncethru = TRUE;
		}
		else
		{
			pBoundingBox->Merge(b);
		}

	}
}

void CAnmMesh::Scale(Float scalefactor)
{
	for (int i = 0; i < m_primitives.size(); i++)
	{
		m_primitives[i]->Scale(scalefactor);
	}
}

void CAnmMesh::AddPrimitive(CAnmPrimitive *pPrimitive)
{
	m_primitives.push_back(pPrimitive);
}

void CAnmMesh::SetDiffuseColor(ULONG c)
{
	for (int i = 0; i < m_primitives.size(); i++)
	{
		m_primitives[i]->SetDiffuseColor(c);
	}
}

void CAnmMesh::SetDiffuseColor(color4 c)
{
	for (int i = 0; i < m_primitives.size(); i++)
	{
		m_primitives[i]->SetDiffuseColor(c);
	}
}

void CAnmMesh::AddFacesFromIndexedFaceList(CAnmDevice *pDevice, sAnmIndexedFaceList *pFaceList,
		BOOL generateNormals)
{
	// lot of work to do; got to go through the list and make a TriangleFan
	// for each face

	int nindices = pFaceList->vindexcount;
	int *pindices = pFaceList->vindices;
	Point3 *pVertices = pFaceList->vertices;
	
	int *pface = pindices;
	for (int i = 0; i < nindices; )
	{
		int vertexcount = 0;		
		CAnmTriangleFan *pTriangleFan = new CAnmTriangleFan(pDevice);
		tAnmVertexArray *pTriVertices = pTriangleFan->GetVertices();

		while(pindices[i] != -1)
		{
			vertexcount++;
			i++;
		}

		pTriVertices->reserve(vertexcount);

		// make the fan
		for (int j = 0; j < vertexcount; j++)
		{
			// get the coordinate info
			int vindex = pface[j];
			Point3 p3 = pVertices[vindex];
			sAnmVertex vert(p3);

			// get normals, colors, UVs
			if (pFaceList->nindices != NULL)
			{
				assert(i < pFaceList->nindexcount);

				int nindex = pFaceList->nindices[i - (j + 1)];
				
				assert(nindex >= 0);
				assert(nindex < pFaceList->normalcount);

				vert.normal = pFaceList->normals[nindex];

			}

			if (pFaceList->cindices != NULL)
			{
				assert(i < pFaceList->cindexcount);

				int cindex = pFaceList->cindices[i - (j + 1)];
				
				assert(cindex >= 0);
				assert(cindex < pFaceList->colorcount);

				vert.SetDiffuseColor(pFaceList->colors[cindex]);

			}

			if (pFaceList->tindices != NULL)
			{
				assert(i < pFaceList->tindexcount);

				int tindex = pFaceList->tindices[i - (j + 1)];
				
				assert(tindex >= 0);
				assert(tindex < pFaceList->uvcount);

				vert.texCoord.x = pFaceList->uvs[tindex].x;
				vert.texCoord.y = /* 1.f - */ pFaceList->uvs[tindex].y;
			}

			// add the vertex to the list
			pTriVertices->push_back(vert);
		}

		m_primitives.push_back(pTriangleFan);

		pface = &pindices[++i];
	}

	// N.B.: ignore the normal generation flag until we figure out what to do with it
}


eAnmReturnStatus CAnmMesh::GenerateNormals(Float creaseAngle)
{
	// N.B.: how do we do vertex sharing among separate primitives?

	if (m_primitives.size() == 1)
	{
		return m_primitives[0]->GenerateNormals(creaseAngle);
	}

	return eAnmReturnAllBad;
}

long CAnmMesh::GetPolyCount()
{
	ulong pcount = 0;

	for (int i = 0; i < m_primitives.size(); i++)
	{
		pcount += m_primitives[i]->NumTriangles();
	}

	return pcount;
}

void CAnmMesh::AddTextureStage(int texturestage)
{
	if (texturestage > m_maxTextureStage)
		m_maxTextureStage = texturestage;
}

bool CAnmMesh::HasAlpha()
{
	return false;
}
