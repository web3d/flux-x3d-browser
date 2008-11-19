/********************************************************************************
 * Flux
 *
 * File: anmshape.cpp
 * Description: Shape node
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
#include "anmshape.h"
#include "anmgeometry.h"
#include "anmappearance.h"
#include "anmmaterial.h"
#include "anmshader.h"
#include "anmmultitexture.h"
#include "anmmultitexturecoordinate.h"
#include "anmtexture.h"
#include "anmtexturetransform.h"
#include "anmdevice.h"
#include "anmtext.h"
#include "anmupdatevisitor.h"
#include "anmworld.h"

CAnmShape::CAnmShape() : CAnmGroup()
{
	m_geometry = NULL;
	m_appearance = NULL;
}

CAnmShape::~CAnmShape()
{
}

void CAnmShape::Realize()
{
	CAnmGroup::Realize();	// realize the kids
}

void CAnmShape::GetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	if (m_geometry)
	{
		CAnmGeometry *pGeometry = GETIMPLEMENTATION(CAnmGeometry,m_geometry);
		if (pGeometry)
			pGeometry->GetBoundingBox(pBoundingBox);
	}
}

void CAnmShape::GetObjectCenter(Point3 *pCenter)
{
	if (m_geometry)
	{
		CAnmGeometry *pGeometry = GETIMPLEMENTATION(CAnmGeometry,m_geometry);
		if (pGeometry)
			pGeometry->GetObjectCenter(pCenter);
	}
}

void CAnmShape::PreTraverse()
{
	// This should get all my children up to date
	CAnmGroup::PreTraverse();

	// Copy new values to all our instances; only do it if we're not
	// rebuilding everything anyway
	if (!TestDirtyBits(eAnmNodeInstanceDirty | eAnmNodeChildInstanceDirty))
	{
		CopyToInstances();
	}
}

void CAnmShape::Traverse(CAnmUpdateVisitor *pVisitor)
{
	CAnmGeometry *pGeometry = GETIMPLEMENTATION(CAnmGeometry, m_geometry);

	if (pGeometry == NULL)
		return;

	// Get the mesh and appearance properties in there
	CAnmDrawable *pDrawable = NULL;
	if (TestDirtyBits(eAnmNodeInstanceDirty | eAnmNodeChildInstanceDirty))
		pDrawable = NextDrawable(pVisitor->GetNodePath());
	else
		pDrawable = FindDrawable(pVisitor->GetNodePath());

	assert(pDrawable);

	CopyValuesToInstance(pDrawable);

	// Set the world matrix from the current visitor's
	matrix4 wmat;
	pVisitor->GetWorldMatrix(&wmat);

	// Bake in any geometry scaling (e.g. for sized primitives)
	Point3 p = pGeometry->GetModelScale();

	if (Scaling(p))
	{
		matrix4 scalemat;
		scalemat.ToScale(p);
		wmat = scalemat * wmat;
	}

	pDrawable->SetWorldMatrix(&wmat);

	if (pVisitor->GetScreenMode())
	{
		matrix4 screenMat;
		pVisitor->GetScreenMatrix(&screenMat);
		pDrawable->SetScreenMatrix(&screenMat);
	}

	// Set the group lighting from the current visitor's light group
	pDrawable->SetLightGroup(pVisitor->GetCurrentLightGroup());

	// Set collision state
	pDrawable->SetCollide(pVisitor->GetCollide());

	// If we're rebuilding everything, add our instance to the current group
	if (TestDirtyBits(eAnmNodeInstanceDirty | eAnmNodeChildInstanceDirty))
	{
		pVisitor->AddDrawable(pDrawable);
	}

	// N.B.: this seems to do really bad things; any reason to do it?

#if 0
	CAnmAppearance *pAppearance = GETIMPLEMENTATION(CAnmAppearance,m_appearance);

	if (pAppearance)
		pAppearance->Traverse(pVisitor);
#else

	// Traverse the children
	// if we have a dirty matrix or child list, traverse all children;
	// otherwise traverse only them whut asked fer it
	std::list< CAnmNode* >::iterator iter;

	int childIndex = 0;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++, childIndex++ )
	{

		if (TestDirtyBits(ANMGROUPDIRTY_FORCETRAVERSECHILDREN))
			(*iter)->Traverse(pVisitor);
		else if ((*iter)->TestDirtyBits(ANMGROUPDIRTY_CHECKTRAVERSECHILDREN))
			(*iter)->Traverse(pVisitor);

		
	}
#endif

}

void CAnmShape::CopyValuesToInstance(CAnmDrawable *pDrawable)
{
	CAnmGeometry *pGeometry = GETIMPLEMENTATION(CAnmGeometry, m_geometry);
	assert (pGeometry != NULL);

	CAnmMultiTextureCoordinate *pMultiTextureCoordinate = 
		GETIMPLEMENTATION(CAnmMultiTextureCoordinate, pGeometry->GetTextureCoordinate());

	CAnmAppearance *pAppearance = GETIMPLEMENTATION(CAnmAppearance,m_appearance);

	CAnmMaterial *pMaterial = NULL;
	CAnmShader *pShader = NULL;
	CAnmTexture *pTexture = NULL;
	CAnmTextureTransform *pTextureTransform = NULL;

	if (pAppearance)
	{
		pMaterial = GETIMPLEMENTATION(CAnmMaterial, pAppearance->GetMaterial());
		pShader = GETIMPLEMENTATION(CAnmShader, pAppearance->GetShader());
		pTexture =  GETIMPLEMENTATION(CAnmTexture, pAppearance->GetTexture());
		pTextureTransform =  GETIMPLEMENTATION(CAnmTextureTransform, pAppearance->GetTextureTransform());
	}

	pDrawable->SetGeometry(pGeometry);
	pDrawable->SetMaterial(pMaterial ? pMaterial->GetRawMaterial() : NULL);
	pDrawable->SetShader(pShader);
	pDrawable->SetTexture(pTexture);
	pDrawable->SetTextureCoordinate(pMultiTextureCoordinate);
	pDrawable->SetTextureTransform(pTextureTransform);
	
	int renderOptions = (int) pGeometry->GetRenderOptions();
	
	bool alphablending = false;
	if (pMaterial && (pMaterial->GetTransparency() != 0.f))
		alphablending = true;
	else if (pTexture && pTexture->HasAlpha())
		alphablending = true;
	else if (pGeometry && pGeometry->HasAlpha())
		alphablending = true;


	pDrawable->SetAlphaBlending(alphablending);

	if (pMaterial != NULL)
	{
		renderOptions |= eAnmRenderOptionsLighting;

		// N.B.: this statement is to work around D3D limitations and should
		// probably be conditional based on platform info. It tells
		// the device to use a gray material as the color source 
		// and modulate because otherwise the texures won't light. The gray
		// material has an average of the r,b,g in its diffuse color, so
		// the lighting levels should appear as expected.

		if (pTexture != NULL && !pTexture->GetClass()->IsA(GETCLASS(CAnmMultiTexture)) &&
			!pTexture->IsGrayscale() && pTexture->GetSurface())
		{
			renderOptions &= ~eAnmRenderOptionsUseVertexForDiffuse;
			pDrawable->SetMaterial(pMaterial->GetRawMaterialWhiteDiffuse());
		}
	}


	pDrawable->SetRenderOptions((eAnmRenderOptions) renderOptions);

}

void CAnmShape::CopyToInstances()
{
	CAnmDrawable *pDrawable = NULL;

	int sz = NumInstances();
	for (int i = 0; i < sz; i++)
	{
		pDrawable = GetDrawable(i);
		assert(pDrawable);

		CopyValuesToInstance(pDrawable);
	}
}

eAnmReturnStatus CAnmShape::AddChild(CAnmNode *pChild)
{
	CAnmGroup::AddChild(pChild);

	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmAppearance)))
		m_appearance = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmGeometry)))
		m_geometry = pChild;

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmShape::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmAppearance)))
		m_appearance = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmGeometry)))
		m_geometry = NULL;

	CAnmGroup::RemoveChild(pChild);

	return eAnmReturnAllGood;
}


// Accessors
void CAnmShape::SetGeometry(CAnmNode *pGeometry)
{

	if (m_geometry == pGeometry)
		return;

	if (m_geometry)
		RemoveChild(m_geometry);

	if (pGeometry == NULL)
		return;

	AddChild(pGeometry);
	m_geometry = pGeometry;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmShape, geometry_changed), &m_geometry);
}

void CAnmShape::SetAppearance(CAnmNode *pAppearance)
{
	if (m_appearance == pAppearance)
		return;

	if (m_appearance)
		RemoveChild(m_appearance);

	if (pAppearance == NULL)
		return;

	AddChild(pAppearance);
	m_appearance = pAppearance;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmShape, appearance_changed), &m_appearance);

}


// Tables for Callbacks, Methods and Set/GetValues

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmShape, geometry, eValueNode, CAnmNode *, GetGeometry, SetGeometry);
DEFINE_VALUE(CAnmShape, appearance, eValueNode, CAnmNode *, GetAppearance, SetAppearance);

DEFINE_METHOD(CAnmShape, set_geometry, eValueNode, CAnmNode *, SetGeometry);
DEFINE_METHOD(CAnmShape, set_appearance, eValueNode, CAnmNode *, SetAppearance);

DEFINE_CALLBACK(CAnmShape, geometry_changed, eValueNode);
DEFINE_CALLBACK(CAnmShape, appearance_changed, eValueNode);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmShape)
VALUEID(CAnmShape, geometry),
VALUEID(CAnmShape, appearance),
METHODID(CAnmShape, set_geometry),
METHODID(CAnmShape, set_appearance),
CALLBACKID(CAnmShape, geometry_changed),
CALLBACKID(CAnmShape, appearance_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmShape, CAnmGroup);

