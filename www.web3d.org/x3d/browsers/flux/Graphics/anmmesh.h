/********************************************************************************
 * Flux
 *
 * File: anmmesh.h
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

#ifndef _anmmesh_h
#define _anmmesh_h

#include "anmprimitive.h"

class CAnmMesh : public CAnmRefObject
{

protected:
	
	tAnmPrimitiveArray					m_primitives;
	int									m_maxTextureStage;


public:

	// constructor/destructor
	CAnmMesh();
	virtual ~CAnmMesh();

	// Methods
	virtual void Render(class CAnmDevice *pDevice, int texturestage);

	virtual bool TestPick(Point3 start, Point3 end, struct sAnmRenderGraphPickList *pPickList);
	virtual bool SphereCollide(Point3 center, float radius, struct sAnmRenderGraphPickList *pPickList);
	virtual bool CapsuleCollide(const Point3 &capsulePt0, const Point3 &capsulePt1, const Point3& capsuleRadius,
		struct sAnmRenderGraphPickList *pPickList);

	virtual void GetBoundingBox(class CAnmBoundingBox *pBoundingBox);
	virtual void Scale(Float scalefactor);

	virtual void AddPrimitive(CAnmPrimitive *pPrimitive);

	virtual void AddFacesFromIndexedFaceList(class CAnmDevice *pDevice, sAnmIndexedFaceList *pFaceList,
		BOOL generateNormals);

	virtual eAnmReturnStatus GenerateNormals(Float creaseAngle);

	virtual void SetDiffuseColor(ULONG diffuseColor);
	virtual void SetDiffuseColor(color4 diffuseColor);

	virtual long GetPolyCount();

	virtual void AddTextureStage(int texturestage);

	virtual CAnmGraphics::Texture* GetTextureSurface(){ return NULL; }

	virtual bool HasAlpha();

	// Accessors
	virtual tAnmPrimitiveArray *GetPrimitives()
	{
		return &m_primitives;
	}

	virtual int NumTextureLevels()
	{
		return m_maxTextureStage + 1;
	}
};


#endif // _anmmesh_h
