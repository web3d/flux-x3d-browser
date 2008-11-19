/********************************************************************************
 * Flux
 *
 * File: anmrenderobject.cpp
 * Description: Render graph classes
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

#include "anmrenderobject.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmfrustum.h"
#include "anmgeometry.h"
#include "anmintersection.h"
#include "anmlight.h"
#include "anmmesh.h"
#include "anmmultitexturecoordinate.h"
#include "anmpickmanager.h"
#include "anmshader.h"
#include "anmtexture.h"
#include "anmtexturetransform.h"
#include "anmtransform.h"
#include "anmviewport.h"
#include "anmMatrixInvert.h"



// RenderObject
CAnmRenderGraph *CAnmRenderObject::GetRoot()
{
	if (m_parent)
		return m_parent->GetRoot();
	else
		return NULL;
}

void CAnmRenderObject::InvalidateBBox()
{
	m_bbox.Invalidate();
	if (m_parent)
		m_parent->InvalidateBBox();
}

void CAnmRenderObject::SetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	// assert(pBoundingBox->IsValid());
	m_bbox = *pBoundingBox;
}

void CAnmRenderObject::GetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	// base class doesn't do anything since it's geometry-free
	assert(FALSE);
}

void CAnmRenderObject::DrawBBox(CAnmDevice *pDevice)
{
	cApplication *pApp = pDevice->GetApp();
	assert(pApp);

	CAnmGraphics *pGraphics = pApp->GetGraphics();
	assert(pGraphics);

	if (m_bbox.IsValid())
	{
		Point3 corners[8];
		m_bbox.GetCorners(corners);
		pGraphics->DrawBox(corners, color3::White, pDevice);
		Point3 org;
		Point3 axes[3];
		m_bbox.GetOriginAxes(&org, axes);
		pGraphics->DrawAxes(org, axes, color3::Cyan, pDevice);
	}
}

void CAnmRenderObject::TestPick(matrix4 &ivmat, Point3 from, Point3 to, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
}

void CAnmRenderObject::SphereCollide(matrix4 &ivmat, Point3 center, float radius, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
}

void CAnmRenderObject::CapsuleCollide(matrix4 &ivmat, const Point3 &capsulePt0, const Point3 &capsulePt1, 
		float capsuleRadius, eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
}

int CAnmRenderObject::NumParents()
{
	if (m_parent == NULL)
		return 0;
	else
		return m_parent->NumParents() + 1;
}

void CAnmRenderObject::GetTopDownPath(CAnmRenderGroup **pParents, int nParents)
{
	assert(pParents != NULL);
	assert(nParents > 0);

	pParents[--nParents] = m_parent;

	if (nParents > 0)
		m_parent->GetTopDownPath(pParents, nParents);
}

void CAnmRenderObject::Cull(CAnmFrustum *pFrustum, 
		std::vector<CAnmDrawable *>	*pRenderList)
{
}


// StateSet
CAnmStateSet::CAnmStateSet()
{
	m_worldmatrix = matrix4::Identity;
	m_material = NULL;
	m_texture = NULL;
	m_textureCoordinate = NULL;
	m_textureTransform = NULL;
	m_lightGroup = NULL;
	m_shader = NULL;
	m_renderOptions = eAnmRenderOptionsNone;
	m_alphablending = false;
	m_screenMatrix = NULL;
	m_combinedScreenMatrix = NULL;
}

CAnmStateSet::~CAnmStateSet()
{
	SafeDelete(m_screenMatrix);
	SafeDelete(m_combinedScreenMatrix);

	SafeUnRef(m_texture);
	SafeUnRef(m_textureCoordinate);
	SafeUnRef(m_textureTransform);
}

void CAnmStateSet::SetState(CAnmDevice *pDevice, CAnmCamera *pCamera)
{
	if (m_combinedScreenMatrix)
	{
		pDevice->SetWorldMatrix(*m_combinedScreenMatrix);
	}
	else
		pDevice->SetWorldMatrix(m_worldmatrix);

	// Lighting is a bit involved
	CAnmLightGroup::ChangeLighting(pDevice, NULL, m_lightGroup);

	// Set the material
	if (m_material)
		pDevice->SetMaterial(m_material);

	// Set the shader
	if (m_shader)
		m_shader->SetToDevice(pDevice);

	// Set random flags
	pDevice->SetRenderOptions(m_renderOptions);
	pDevice->SetAlphaBlending(m_alphablending);

	// Zero out textures and transforms for all hardware stages
	int maxTextureLevels = pDevice->MaxTextureLevels();
	matrix4 defaultTTmat;
	pDevice->GetDefaultTextureTransform(&defaultTTmat);
	for (int i = 0; i < maxTextureLevels; i++)
	{
		pDevice->SetTexture(i, NULL);
		pDevice->SetTextureTransform(i, &defaultTTmat);
		pDevice->SetTextureOption(i, eAnmTextureColorOp, eAnmTextureModeOFF);
		pDevice->SetTextureOption(i, eAnmTextureTexCoordIndex, eAnmTextureTexCoordPassThru);
	}

}



void CAnmStateSet::UpdateScreenGroupMatrix(CAnmDevice *pDevice, CAnmCamera *pCamera)
{
	assert(m_screenMatrix);
	assert(m_combinedScreenMatrix);

	CAnmViewport *pViewport = pDevice->GetViewport();

	int width = pViewport->Width();
	int height = pViewport->Height();

	float fov = pCamera->GetFieldOfView();

	int minOfVPDim = ( width < height ) ? width : height;
	float halfMinVPDim = 0.5f * ( ( float ) minOfVPDim );


	matrix4 cameraMatrix = pCamera->GetMatrix();
	Point3 cameraLocation = pCamera->GetPosition();
	Point3 cameraDirection;
	Point3 cameraUp;
	pCamera->GetOrientation( &cameraDirection, &cameraUp );

	// $$$ Keith this should be some function of the screen matrix and viewport
	// NOTE that this does *not* have the culling logic in it yet; i.e. culling
	// could be off because of this

	float focalLength = ( m_worldmatrix._41 - cameraLocation.x ) * cameraDirection.x + 
						( m_worldmatrix._42 - cameraLocation.y ) * cameraDirection.y + 
						( m_worldmatrix._43 - cameraLocation.z ) * cameraDirection.z;

	float halfHeight = tan( fov / 2.0f ) * focalLength;

	// halfMinVPDim is the actual number of pixels in half the screen.
	// halfHeight is the desired number of pixels in half the screen.
	// so if we scale by the ratio, then we should get the desired values.

	float scaleAdjustment = halfHeight / halfMinVPDim;

	// get the current scale of the matrix, since we dont need that scale.
	//

	// If we have a scale in the matrix, we better extract it, and divide by it
	//

	float currentScaleX = sqrt( m_worldmatrix._11*m_worldmatrix._11 + 
								m_worldmatrix._12*m_worldmatrix._12 + 
								m_worldmatrix._13*m_worldmatrix._13 );

	float currentScaleY = sqrt( m_worldmatrix._21*m_worldmatrix._21 + 
								m_worldmatrix._22*m_worldmatrix._22 + 
								m_worldmatrix._23*m_worldmatrix._23 );


	matrix4 scaleMat( matrix4::Identity );
	scaleMat._11 = scaleAdjustment / currentScaleX;
	scaleMat._22 = scaleAdjustment / currentScaleY;
	scaleMat._33 = 0.5f * ( scaleMat._11 + scaleMat._22 );


	*m_combinedScreenMatrix = *m_screenMatrix * scaleMat * m_worldmatrix;
}




void CAnmStateSet::ChangeState(CAnmDevice *pDevice, CAnmCamera *pCamera, CAnmStateSet *prevstate)
{
	// at the moment, no way to compare matrices so we'll just set it

	if (m_combinedScreenMatrix)
	{
		pDevice->SetWorldMatrix(*m_combinedScreenMatrix);

	}
	else
		pDevice->SetWorldMatrix(m_worldmatrix);


	// Call helper to change lighting
	CAnmLightGroup *pOldLighting = prevstate->GetLightGroup();
	CAnmLightGroup::ChangeLighting(pDevice, pOldLighting, m_lightGroup);

	// Simple states, just blast through them
	if (m_material != prevstate->GetMaterial())
	{
		if (m_material)
			pDevice->SetMaterial(m_material);
	}
	if (m_renderOptions != prevstate->GetRenderOptions())
	{
		pDevice->SetRenderOptions(m_renderOptions);
	}
	if (m_alphablending != prevstate->GetAlphaBlending())
	{
		pDevice->SetAlphaBlending(m_alphablending);
	}

	// Slightly complicated logic for shaders because the matrix
	// state has to be taken into account with every draw-- TP 080604
	if (m_shader)
	{
		m_shader->SetToDevice(pDevice);
	}
	else if (m_shader != prevstate->GetShader()) {
		pDevice->SetFragmentShader(NULL);
		pDevice->SetVertexShader(NULL);
	}

}

void CAnmStateSet::SetWorldMatrix(matrix4 *pMat)
{
	m_worldmatrix = *pMat;
}

void CAnmStateSet::SetMaterial(CAnmGraphics::Material *pMaterial)
{
	m_material = pMaterial;
}

void CAnmStateSet::SetTexture(class CAnmTexture *pTexture)
{
	SafeUnRef(m_texture);
	m_texture = pTexture;
	if (m_texture)
		m_texture->Ref();
}

void CAnmStateSet::SetTextureCoordinate(class CAnmMultiTextureCoordinate *pTextureCoordinate)
{
	SafeUnRef(m_textureCoordinate);
	m_textureCoordinate = pTextureCoordinate;
	if (m_textureCoordinate)
		m_textureCoordinate->Ref();
}

void CAnmStateSet::SetTextureTransform(class CAnmTextureTransform *pTextureTransform)
{
	SafeUnRef(m_textureTransform);
	m_textureTransform = pTextureTransform;
	if (m_textureTransform)
		m_textureTransform->Ref();
}

void CAnmStateSet::SetLightGroup(class CAnmLightGroup *pLightGroup)
{
	m_lightGroup = pLightGroup;	
}

void CAnmStateSet::SetShader(class CAnmShader *pShader)
{
	m_shader = pShader;
}

void CAnmStateSet::SetRenderOptions(eAnmRenderOptions renderOptions)
{
	// includes material color use (diffuse vs. material), zbuffering, edge antialias etc.
	m_renderOptions = renderOptions;
}

void CAnmStateSet::SetAlphaBlending(bool alphablending)
{
	// for now just set the flag; eventually this will trigger some kind of re-sort
	m_alphablending = alphablending;
}

void CAnmStateSet::SetScreenMatrix(matrix4 *pMat)
{
	if (m_screenMatrix == NULL)
		m_screenMatrix = new matrix4;

	if (m_combinedScreenMatrix == NULL)
		m_combinedScreenMatrix = new matrix4;

	*m_screenMatrix = *pMat;
	*m_combinedScreenMatrix = matrix4::Identity;
}

// Drawable
CAnmDrawable::CAnmDrawable() : 
CAnmRenderObject(),
CAnmStateSet()
{
	m_geometry = NULL;
	m_collide = false;
}

CAnmDrawable::~CAnmDrawable()
{
	SafeUnRef(m_geometry);
}

void CAnmDrawable::SetGeometry(class CAnmGeometry *pGeometry)
{
	if (pGeometry != m_geometry)
	{
		SafeUnRef(m_geometry);
		m_geometry = pGeometry;
		if (m_geometry)
			m_geometry->Ref();

	}
		
	InvalidateBBox();
}

void CAnmDrawable::SetWorldMatrix(matrix4 *pMat)
{
	CAnmStateSet::SetWorldMatrix(pMat);
	InvalidateBBox();
}

void CAnmDrawable::GetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	if (!m_bbox.IsValid() && m_geometry != NULL)
	{
		// krv:  Need to get the unscaled Bounding Box, since our primitives will get scaled
		// by the world matrix below, which includes the "size" of the four simple primitive geo nodes.
		m_geometry->GetUnscaledBoundingBox(&m_bbox);

		if (m_combinedScreenMatrix)
			m_bbox = m_bbox.Transform(*m_combinedScreenMatrix);
		else
			m_bbox = m_bbox.Transform(m_worldmatrix);
	}

	*pBoundingBox = m_bbox;
}

// N.B.: we're requerying the texture for its alphaness. do we want
// to do this every frame?
bool CAnmDrawable::GetAlphaBlending()
{
	if (m_alphablending)
		return true;
	else if (m_texture)
		return (m_alphablending = m_texture->HasAlpha());	// note single '='
	else
		return false;
}


void CAnmDrawable::TestPick(matrix4 &ivmat, Point3 from, Point3 to, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	if (collideTest && !m_collide)
		return;

	// Try trivial reject on bounding box
	if (!m_bbox.IsValid())
		return;

	Point3 p, center, size;
	m_bbox.GetCenterSize(&center, &size);
	if (!CAnmIntersection::RayIntersectBox(from, to, center, size, &p))
		return;

	if (m_geometry)
	{
		CAnmMesh *mesh = m_geometry->GetMesh();

		if (mesh)
		{

			// krv:  If you have a combined matrix, use it.
			//
			matrix4* pWorldmatrix;
			if (m_combinedScreenMatrix) {
				pWorldmatrix = m_combinedScreenMatrix;
			}
			else {
				pWorldmatrix = &m_worldmatrix;
			}


			matrix4 imat;
			MatrixInvert(imat, *pWorldmatrix);

			// convert pick ray to model coords for testing
			Point3 lfrom = imat * from;
			Point3 lto = imat * to;

			sAnmRenderGraphPickList meshPickList;

			if (mesh->TestPick(lfrom, lto, &meshPickList))
			{
				int nParents = NumParents();

				int nPicked = meshPickList.Size();

				for (int i = 0; i < nPicked; i++)
				{
					sAnmRenderGraphPickDesc pickDesc;

					meshPickList.GetItem(i, pickDesc);

					// save the parent stack (it's the top-down path to this node)
					pickDesc.pLeaf = this;
					pickDesc.nParents =  nParents;
					pickDesc.pParentArray = new CAnmRenderGroup *[nParents];

					GetTopDownPath(pickDesc.pParentArray, nParents);

					// transform hitPoint and hitNormal into world coords
					pickDesc.hitPoint = (*pWorldmatrix) * pickDesc.hitPoint;
					pickDesc.hitNormal = (*pWorldmatrix) * pickDesc.hitNormal;
					// pesky origin
					Point3 org = (*pWorldmatrix) * point3::Zero;
					pickDesc.hitNormal = pickDesc.hitNormal - org;

					pPickList->Insert(ivmat, pickDesc, sortType);
				}
			}
		}	
	}

}

void CAnmDrawable::SphereCollide(matrix4 &ivmat, Point3 center, float radius, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	if (collideTest && !m_collide)
		return;

	// Try trivial reject on bounding box
	if (!m_bbox.IsValid())
		return;

	Point3 p, bcenter, bsize;
	m_bbox.GetCenterSize(&bcenter, &bsize);
	if (!CAnmIntersection::SphereIntersectBox(center, radius, bcenter, bsize))
		return;

	if (m_geometry)
	{
		CAnmMesh *mesh = m_geometry->GetMesh();

		assert(mesh);


		// krv:  If you have a combined matrix, use it.
		//
		matrix4* pWorldmatrix;
		if (m_combinedScreenMatrix) {
			pWorldmatrix = m_combinedScreenMatrix;
		}
		else {
			pWorldmatrix = &m_worldmatrix;
		}

		matrix4 imat;
		
		MatrixInvert(imat, (*pWorldmatrix) );

		// convert pick ray to model coords for testing
		Point3 lcenter = imat * center;
		float lradius = imat._11 * radius;	// N.B. might be a problem with non-uniform scale

		sAnmRenderGraphPickList meshPickList;

		if (mesh->SphereCollide(lcenter, lradius, &meshPickList))
		{
			int nParents = NumParents();

			int nPicked = meshPickList.Size();

			for (int i = 0; i < nPicked; i++)
			{
				sAnmRenderGraphPickDesc pickDesc;

				meshPickList.GetItem(i, pickDesc);

				// save the parent stack (it's the top-down path to this node)
				pickDesc.pLeaf = this;
				pickDesc.nParents =  nParents;
				pickDesc.pParentArray = new CAnmRenderGroup *[nParents];

				GetTopDownPath(pickDesc.pParentArray, nParents);

				// transform hitPoint and hitNormal into world coords
				pickDesc.hitPoint = (*pWorldmatrix) * pickDesc.hitPoint;
				pickDesc.hitNormal = (*pWorldmatrix) * pickDesc.hitNormal;
				// pesky origin
				Point3 org = (*pWorldmatrix) * point3::Zero;
				pickDesc.hitNormal = pickDesc.hitNormal - org;

				pPickList->Insert(ivmat, pickDesc, sortType);
			}
		}	
	}
}

void CAnmDrawable::CapsuleCollide(matrix4 &ivmat, const Point3 &capsulePt0, const Point3 &capsulePt1, 
		float capsuleRadius, eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	if (collideTest && !m_collide)
		return;

	// Try trivial reject on bounding box
	if (!m_bbox.IsValid())
		return;

	Point3 p, bmin, bmax;
	m_bbox.GetMinMax(&bmin, &bmax);
	if (!CAnmIntersection::CapsuleIntersectBox(capsulePt0, capsulePt1, capsuleRadius, bmin, bmax))
		return;

	if (m_geometry)
	{
		CAnmMesh *mesh = m_geometry->GetMesh();

		assert(mesh);


		// krv:  If you have a combined matrix, use it.
		//
		matrix4* pWorldmatrix;
		if (m_combinedScreenMatrix) {
			pWorldmatrix = m_combinedScreenMatrix;
		}
		else {
			pWorldmatrix = &m_worldmatrix;
		}

		matrix4 imat;
		MatrixInvert(imat, (*pWorldmatrix));

		// convert pick ray to model coords for testing
		Point3 lpt0 = imat * capsulePt0;
		Point3 lpt1 = imat * capsulePt1;

		// krv:  Create a point3 nonuniform scaled radius.
		//
		imat._41 = 
		imat._42 = 
		imat._43 = 0.0f;
		Point3 lradius = imat * Point3( capsuleRadius, capsuleRadius, capsuleRadius );

		lradius.x = fabs( lradius.x );
		lradius.y = fabs( lradius.y );
		lradius.z = fabs( lradius.z );

		sAnmRenderGraphPickDesc pickDesc;

		sAnmRenderGraphPickList meshPickList;

		if (mesh->CapsuleCollide(lpt0, lpt1, lradius, &meshPickList))
		{
			int nParents = NumParents();

			int nPicked = meshPickList.Size();

			for (int i = 0; i < nPicked; i++)
			{
				sAnmRenderGraphPickDesc pickDesc;

				meshPickList.GetItem(i, pickDesc);

				// save the parent stack (it's the top-down path to this node)
				pickDesc.pLeaf = this;
				pickDesc.nParents =  nParents;
				pickDesc.pParentArray = new CAnmRenderGroup *[nParents];

				GetTopDownPath(pickDesc.pParentArray, nParents);

				// transform hitPoint and hitNormal into world coords
				pickDesc.hitPoint = (*pWorldmatrix) * pickDesc.hitPoint;
				pickDesc.hitNormal = (*pWorldmatrix) * pickDesc.hitNormal;
				// pesky origin
				Point3 org = (*pWorldmatrix) * point3::Zero;
				pickDesc.hitNormal = pickDesc.hitNormal - org;

				pPickList->Insert(ivmat, pickDesc, sortType);
			}
		}	
	}
}

void CAnmDrawable::Cull(CAnmFrustum *pFrustum, 
		std::vector<CAnmDrawable *>	*pRenderList)
{
	bool culled = pFrustum->Cull(m_bbox);
	if (!culled) {
		pRenderList->push_back(this);
	}
}


void CAnmDrawable::Render(CAnmDevice *pDevice, CAnmStateSet *&prevstate,
						  CAnmCamera *pCamera, CAnmPrimitive *pPrimitive, int triangleIndex)

{
	if (m_geometry == NULL)
		return;

	CAnmMesh *mesh = m_geometry->GetMesh();

	if (mesh == NULL)
		return;

	// Get basic state set up. this includes zeroing out old texture stages
	ChangeState(pDevice, pCamera, prevstate);

	// Render the drawable; this accounts for multiple passes in a multitexture
	int maxTextureLevels = pDevice->MaxTextureLevels();

	int nTextureLevels = 1;
	if (m_texture)
		nTextureLevels = m_texture->NumLevels();

	// First, zero out any texture stages we're not going to use
	int i;
	if (m_texture != prevstate->GetTexture())
	{
		CAnmTexture *pOldTexture = prevstate->GetTexture();
		if (pOldTexture)
		{
			int nNewLevels = 0;
			if (m_texture)
				nNewLevels = m_texture->NumLevels();
			if (nNewLevels > maxTextureLevels)
				nNewLevels = maxTextureLevels;

			int nOldLevels = pOldTexture->NumLevels();
			if (nOldLevels > maxTextureLevels)
				nOldLevels = maxTextureLevels;

			matrix4 defaultTTmat;
			pDevice->GetDefaultTextureTransform(&defaultTTmat);
			for (i = nNewLevels; i < nOldLevels; i++)
			{
				pDevice->SetTexture(i, NULL);
				pDevice->SetTextureTransform(i, &defaultTTmat);
				pDevice->SetTextureOption(i, eAnmTextureColorOp, eAnmTextureModeOFF);
				pDevice->SetTextureOption(i, eAnmTextureTexCoordIndex, eAnmTextureTexCoordPassThru);
			}
		}
	}

	// Loop through texture levels. Do as many stages as possible per pass,
	// based on hardware capabilities
	bool lighting = (m_renderOptions & eAnmRenderOptionsLighting);

	// Don't try multi-stage if the geometry itself has multiple texture coordinates


	int nTexturesPerPass = maxTextureLevels;
	int nGeomTextureLevels = mesh->NumTextureLevels();
	if (nGeomTextureLevels > 1)
		nTexturesPerPass = 1;
	
	int startLevel = 0;
	while (startLevel < nTextureLevels)
	{
		// set new texture stage(s) and state(s). test against previous ones
		// to avoid unnecessary state changes
		if (m_texture != prevstate->GetTexture())
		{
			if (m_texture)
			{
#ifdef DEBUG
				if (CAnmTexture::GetDrawTextures())
#endif
					m_texture->SetToDevice(pDevice, m_alphablending, lighting, startLevel, nTexturesPerPass);
			}
			else if (startLevel < maxTextureLevels)
			{
				// no texture? zero out all hardware levels
				for (i = 0; i < maxTextureLevels; i++)
					pDevice->SetTexture(i, NULL);
			}
		}

		// set new texture transform(s). test against previous ones
		// to avoid unnecessary state changes
		if (m_textureTransform != prevstate->GetTextureTransform())
		{
			if (m_textureTransform)
			{
				m_textureTransform->SetToDevice(pDevice, startLevel, nTexturesPerPass);
			}
			else if (startLevel < maxTextureLevels)
			{
				// no texture transform? use default matrix at all hardware levels
				matrix4 defTT;
				pDevice->GetDefaultTextureTransform(&defTT);

				for (i = 0; i < maxTextureLevels; i++)
					pDevice->SetTextureTransform(i, &defTT);
			}
		}

		// set new texture coordinate(s). test against previous ones
		// to avoid unnecessary state changes

		bool coordsettransform = false;
		if (m_textureCoordinate != prevstate->GetTextureCoordinate())
		{
			if (m_textureCoordinate)
			{
				coordsettransform =
					m_textureCoordinate->SetToDevice(pDevice, startLevel, nTexturesPerPass);

			}
			else if (startLevel < maxTextureLevels)
			{
				// no multi-texture coordinates? zero out all hardware levels
				for (i = 0; i < maxTextureLevels; i++)
					pDevice->SetTextureOption(i, eAnmTextureTexCoordIndex, eAnmTextureTexCoordPassThru);
			}
		}
		
		// Render our geometry
		m_geometry->Render(pDevice, startLevel, pCamera, m_worldmatrix);

		// Restore possibly messed up texture transform matrix
		// N.B.: Annoying; but I think we need to do this because of the tweaking
		// that happens to texture transforms with camera-relative modes
		if (coordsettransform)
		{
			if (m_textureTransform)
			{
				m_textureTransform->SetToDevice(pDevice, startLevel, nTexturesPerPass);
			}
			else if (startLevel < maxTextureLevels)
			{
				// no texture transform? use default matrix at all hardware levels
				matrix4 defTT;
				pDevice->GetDefaultTextureTransform(&defTT);

				for (i = 0; i < maxTextureLevels; i++)
					pDevice->SetTextureTransform(i, &defTT);
			}
		}

		// Bump level count
		startLevel += nTexturesPerPass;
	}


	prevstate = this;
}

void CAnmDrawable::handleAdded(CAnmRenderGroup *pParent)
{
	CAnmRenderObject::handleAdded(pParent);

	if (m_screenMatrix)
	{
		CAnmRenderGraph *pRenderGraph = GetRoot();
		assert(pRenderGraph);

		pRenderGraph->AddScreenObject(this);
	}
}

void CAnmDrawable::handleRemoved()
{
	if (m_screenMatrix)
	{
		CAnmRenderGraph *pRenderGraph = GetRoot();
		assert(pRenderGraph);

		pRenderGraph->RemoveScreenObject(this);
	}

	CAnmRenderObject::handleRemoved();
}

void CAnmDrawable::UpdateScreenGroupMatrix(CAnmDevice *pDevice, CAnmCamera *pCamera)
{
	CAnmStateSet::UpdateScreenGroupMatrix(pDevice, pCamera);

	InvalidateBBox();
}

// RenderGroup
CAnmRenderGroup::CAnmRenderGroup()
{
}

CAnmRenderGroup::~CAnmRenderGroup()
{
}

void CAnmRenderGroup::AddChild(CAnmRenderObject *pChild)
{


#ifdef DEBUG

	std::list< CAnmRenderObject* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ ) {
		assert( ( *iter ) != pChild );
	}
#endif

	m_children.push_back(pChild);
	pChild->handleAdded(this);
	InvalidateBBox();
}

void CAnmRenderGroup::RemoveChild(CAnmRenderObject *pChild)
{
	m_children.remove(pChild);
	pChild->handleRemoved();
	InvalidateBBox();
}

void CAnmRenderGroup::ClearChildren(void)
{
	// N.B.: someone else is responsible for deleting the instances... I think
	std::list< CAnmRenderObject* >::iterator iter;

	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		CAnmRenderObject *pChild = (*iter);
		pChild->handleRemoved();
	}

	m_children.clear();
	InvalidateBBox();
}

void CAnmRenderGroup::GetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	if (!m_bbox.IsValid())
	{
		CAnmBoundingBox b;
		BOOL oncethru = FALSE;
		std::list< CAnmRenderObject* >::iterator iter;

		for( 
			iter = m_children.begin(); 
			iter != m_children.end();
			iter++ )
		{
			(*iter)->GetBoundingBox(&b);

			if (b.IsValid())
			{
				if (!oncethru)
				{
					m_bbox = b;
					oncethru = TRUE;
				}
				else
				{
					m_bbox.Merge(b);
				}
			}
		}
	}

	*pBoundingBox = m_bbox;
}

void CAnmRenderGroup::DrawBBox(CAnmDevice *pDevice)
{
	if (m_bbox.IsValid())
	{
		CAnmRenderObject::DrawBBox(pDevice);

		std::list< CAnmRenderObject* >::iterator iter;

		for( 
			iter = m_children.begin(); 
			iter != m_children.end();
			iter++ )
		{
			(*iter)->DrawBBox(pDevice);
		}

	}
}

void CAnmRenderGroup::TestPick(matrix4 &ivmat, Point3 from, Point3 to, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	// Try trivial reject on bounding box
	if (!m_bbox.IsValid())
		return;

	Point3 p, center, size;
	m_bbox.GetCenterSize(&center, &size);
	if (!CAnmIntersection::RayIntersectBox(from, to, center, size, &p))
		return;

	std::list< CAnmRenderObject* >::iterator iter;

	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		CAnmRenderObject *pRenderObj = (*iter);
		(*iter)->TestPick(ivmat, from, to, sortType, collideTest, pPickList);
	}
}

void CAnmRenderGroup::SphereCollide(matrix4 &ivmat, Point3 center, float radius, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	// Try trivial reject on bounding box
	if (!m_bbox.IsValid())
		return;

	Point3 p, bcenter, bsize;
	m_bbox.GetCenterSize(&bcenter, &bsize);
	if (!CAnmIntersection::SphereIntersectBox(center, radius, bcenter, bsize))
		return;

	std::list< CAnmRenderObject* >::iterator iter;

	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		CAnmRenderObject *pRenderObj = (*iter);
		(*iter)->SphereCollide(ivmat, center, radius, sortType, collideTest, pPickList);
	}
}

void CAnmRenderGroup::CapsuleCollide(matrix4 &ivmat, const Point3 &capsulePt0, const Point3 &capsulePt1, 
		float capsuleRadius, eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	// Try trivial reject on bounding box
	if (!m_bbox.IsValid())
		return;

	Point3 p, bmin, bmax;
	m_bbox.GetMinMax(&bmin, &bmax);
	if (!CAnmIntersection::CapsuleIntersectBox(capsulePt0, capsulePt1, capsuleRadius, bmin, bmax))
		return;

	std::list< CAnmRenderObject* >::iterator iter;

	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		CAnmRenderObject *pRenderObj = (*iter);
		(*iter)->CapsuleCollide(ivmat, capsulePt0, capsulePt1, capsuleRadius, sortType, collideTest, pPickList);
	}
}

void CAnmRenderGroup::Cull(CAnmFrustum *pFrustum, 
		std::vector<CAnmDrawable *>	*pRenderList)
{
	bool culled = pFrustum->Cull(m_bbox);
	if (!culled)
	{
		std::list< CAnmRenderObject* >::iterator iter;

		for( 
			iter = m_children.begin(); 
			iter != m_children.end();
			iter++ )
		{
			(*iter)->Cull(pFrustum, pRenderList);
		}	
	}
}


// RenderGraph: the root
CAnmRenderGraph::CAnmRenderGraph() :
CAnmRenderGroup(),
CAnmStateSet()
{
	// N.B.: light 0 reserved for headlight. we should make this softer somehow
	m_globalLightGroup = new CAnmLightGroup;
	m_renderlist.reserve(ANMRENDERGRAPH_RENDERLISTALLOC);
	m_zbufferenabled = true;
	m_fog = NULL;
	m_screenobjectsupdated = false;
	m_lastcameraposforscreen = Point3(FLT_MAX, FLT_MAX, FLT_MAX);
}
	
CAnmRenderGraph::~CAnmRenderGraph()
{
}

#define USE_QSORT

#ifdef USE_QSORT


class CAnmAlphaShape
{
	CAnmDrawable	*m_drawable;
	matrix4			*m_ivmat;

protected :
public :

	CAnmAlphaShape(CAnmDrawable *pDrawable, matrix4 *pIVmat)
	{
		m_drawable = pDrawable;
		m_ivmat = pIVmat;
	}

	CAnmDrawable *GetDrawable()
	{
		return m_drawable;
	}

	matrix4 *GetIVMat()
	{
		return m_ivmat;
	}
};

static int alphasort(const void * elem1, const void * elem2 )
{
	CAnmAlphaShape *pShape1 = (CAnmAlphaShape *) elem1;
	CAnmAlphaShape *pShape2 = (CAnmAlphaShape *) elem2;

	assert(pShape1);
	assert(pShape2);

	CAnmDrawable *pDrawable1 = pShape1->GetDrawable();
	CAnmDrawable *pDrawable2 = pShape2->GetDrawable();

	matrix4 *pIVmat = pShape1->GetIVMat();

	assert(pDrawable1);
	assert(pDrawable2);
	assert(pIVmat);

	CAnmBoundingBox bbox1, bbox2;
	pDrawable1->GetBoundingBox(&bbox1);
	pDrawable2->GetBoundingBox(&bbox2);

	CAnmBoundingBox vb1 = bbox1.Transform(*pIVmat);
	CAnmBoundingBox vb2 = bbox2.Transform(*pIVmat);

	Point3 ctr1, ctr2, size1, size2;

	vb1.GetCenterSize(&ctr1, &size1);
	vb2.GetCenterSize(&ctr2, &size2);

	if (ctr1.z > ctr2.z)
		return -1;
	else if (ctr1.z < ctr2.z)
		return 1;
	else
		return 0;
}

inline void SortAlphaShapes(CAnmAlphaShape *pAlphaShapes, int nAlphaShapes)
{
	qsort(pAlphaShapes, nAlphaShapes, sizeof(CAnmAlphaShape), alphasort);
}

#else // USE_QSORT

inline void InsertAlphaShape(matrix4 &ivmat,
							 std::list<CAnmDrawable *> &alphashapes,
							 CAnmDrawable *pDrawable)
{
	// get new Drawable's bounding box, min and max
	// get model space bbox...
	CAnmBoundingBox bbox;
	pDrawable->GetBoundingBox(&bbox);

	// transform bbox from model space to view space and get min/max
	CAnmBoundingBox vb = bbox.Transform(ivmat);
	Point3 ctr, size;
	vb.GetCenterSize(&ctr, &size);

	// find the place to insert
	std::list<CAnmDrawable *>::iterator iter;
	for (iter = alphashapes.begin(); iter != alphashapes.end(); iter++)
	{
		// get current Drawable's bounding box, min and max
		CAnmDrawable *pCurrent = *iter;

		// get model space bbox...
		pCurrent->GetBoundingBox(&bbox);

		// ...transform bbox from model space to view space and get min/max
		vb = bbox.Transform(ivmat);
		Point3 curctr, cursize;
		vb.GetCenterSize(&curctr, &cursize);

		// greater than the current one? we're done; else keep looking
		if (ctr.z > curctr.z)
			break;
	}

	// now insert it
	alphashapes.insert(iter, pDrawable);
}

#endif // USE_QSORT

void CAnmRenderGraph::Render(CAnmDevice *pDevice, CAnmCamera *pCamera)
{
	// Get global state set up
	SetState(pDevice, pCamera);

	// Set up global lighting (StateSet won't do this)
	CAnmLightGroup::ChangeLighting(pDevice, NULL, m_globalLightGroup);

	// Set up global zbuffering and fog states (StateSet won't do this)
	pDevice->SetZBuffering(m_zbufferenabled);
	if (m_fog)
	{
		pDevice->EnableFog(true);
		pDevice->SetFog(m_fog);
	}
	else
	{
		pDevice->EnableFog(false);
	}

	// Make sure camera-dependent objects are up to date
	// N.B.: do this before getting bounding boxes!
	UpdateScreenObjects(pDevice, pCamera);

	// Make sure our bounding boxes are up to date
	if (!m_bbox.IsValid())
	{
		CAnmBoundingBox dummy;
		GetBoundingBox(&dummy);
	}

#ifdef DEBUG
	// Draw our silly bounding boxes if asked (uses Identity matrix so do this first)
	if (CAnmTransform::GetDrawBoxes())
	{
		DrawBBox(pDevice);
	}
#endif

	// Cull step
	// Clear out the render list
	m_renderlist.clear();

	// Now populate it based on the frustum
	// Get frustum for cull
	CAnmFrustum *pFrustum = pCamera->GetFrustum();
	assert(pFrustum);

	Cull(pFrustum, &m_renderlist);


	// List cursors for drawables and alpha-sort list
	std::vector< CAnmDrawable * >::iterator iter;
#ifdef USE_QSORT
	std::vector<CAnmAlphaShape> alphashapes;
#else
	std::list<CAnmDrawable *> alphashapes;
#endif

	// Get camera matrix before loop
	matrix4 vmat = pCamera->GetMatrix();
	matrix4 ivmat;
	MatrixInvert(ivmat, vmat);

	// init counter
	m_visibleobjects = 0;

	// First value for state-change list
	CAnmStateSet *pCurrent = this;

	for( 
		iter = m_renderlist.begin(); 
		iter != m_renderlist.end();
		iter++ )
	{
		if ((*iter)->GetAlphaBlending())
		{
			// N.B.: These will be drawn back-to-front depth-sorted
#ifdef USE_QSORT
			alphashapes.push_back(CAnmAlphaShape(*iter, &ivmat));
#else
			InsertAlphaShape(ivmat, alphashapes, (*iter));
#endif
		}
		else
		{
			(*iter)->Render(pDevice, pCurrent, pCamera);
		}

		m_visibleobjects++;
	}	

	// Draw any shapes with alpha
#ifdef USE_QSORT
	int nalphashapes = alphashapes.size();
	SortAlphaShapes(&alphashapes[0], nalphashapes);
	for (int i = 0; i < nalphashapes; i++)
	{
		CAnmDrawable *pDrawable = alphashapes[i].GetDrawable();
		pDrawable->Render(pDevice, pCurrent, pCamera);
	}

#else
	std::list<CAnmDrawable *>::iterator alphaiter;
	int nalphashapes = 0;
	for (alphaiter = alphashapes.begin(); alphaiter != alphashapes.end(); alphaiter++)
	{
		nalphashapes++;
		CAnmDrawable *pDrawable = *alphaiter;
		pDrawable->Render(pDevice, pCurrent, pCamera);
	}
#endif

	// Flag that we'll need a new screen update
	m_screenobjectsupdated = false;

}

void CAnmRenderGraph::TestPick(matrix4 &ivmat, Point3 from, Point3 to, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	// Make sure our bounding boxes are up to date
	if (!m_bbox.IsValid())
	{
		CAnmBoundingBox dummy;
		GetBoundingBox(&dummy);
	}

	CAnmRenderGroup::TestPick(ivmat, from, to, sortType, collideTest, pPickList);
}


void CAnmRenderGraph::SphereCollide(matrix4 &ivmat, Point3 center, float radius, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	// Make sure our bounding boxes are up to date
	if (!m_bbox.IsValid())
	{
		CAnmBoundingBox dummy;
		GetBoundingBox(&dummy);
	}

	CAnmRenderGroup::SphereCollide(ivmat, center, radius, sortType, collideTest, pPickList);
}

void CAnmRenderGraph::CapsuleCollide(matrix4 &ivmat, const Point3 &capsulePt0, const Point3 &capsulePt1, 
		float capsuleRadius, eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	// Make sure our bounding boxes are up to date
	if (!m_bbox.IsValid())
	{
		CAnmBoundingBox dummy;
		GetBoundingBox(&dummy);
	}

	CAnmRenderGroup::CapsuleCollide(ivmat, capsulePt0, capsulePt1, capsuleRadius, sortType, collideTest, pPickList);
}

void CAnmRenderGraph::TestPick(CAnmDevice *pDevice, CAnmCamera *pCamera, Point3 from, Point3 to, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	// Make sure camera-dependent objects are up to date
	UpdateScreenObjects(pDevice, pCamera);

	matrix4 vmat = pCamera->GetMatrix();
	matrix4 ivmat;
	MatrixInvert(ivmat, vmat);

	TestPick(ivmat, from, to, sortType, collideTest, pPickList);
}


void CAnmRenderGraph::SphereCollide(CAnmDevice *pDevice, CAnmCamera *pCamera, Point3 center, float radius, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	// Make sure camera-dependent objects are up to date
	UpdateScreenObjects(pDevice, pCamera);

	matrix4 vmat = pCamera->GetMatrix();
	matrix4 ivmat;
	MatrixInvert(ivmat, vmat);

	SphereCollide(ivmat, center, radius, sortType, collideTest, pPickList);
}

void CAnmRenderGraph::CapsuleCollide(CAnmDevice *pDevice, CAnmCamera *pCamera, const Point3 &capsulePt0, const Point3 &capsulePt1, 
		float capsuleRadius, eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList)
{
	// Make sure camera-dependent objects are up to date
	UpdateScreenObjects(pDevice, pCamera);

	matrix4 vmat = pCamera->GetMatrix();
	matrix4 ivmat;
	MatrixInvert(ivmat, vmat);

	CapsuleCollide(ivmat, capsulePt0, capsulePt1, capsuleRadius, sortType, collideTest, pPickList);
}

void CAnmRenderGraph::AddScreenObject(CAnmDrawable *pDrawable)
{
	m_screenobjects.push_back(pDrawable);
}

void CAnmRenderGraph::RemoveScreenObject(CAnmDrawable *pDrawable)
{
	m_screenobjects.remove(pDrawable);
}

void CAnmRenderGraph::UpdateScreenObjects(class CAnmDevice *pDevice, CAnmCamera *pCamera)
{
	Point3 camerapos = pCamera->GetPosition();

	if (m_screenobjectsupdated)
	{
		if ((camerapos == m_lastcameraposforscreen)) {
			return;
		}
	}

	std::list<CAnmDrawable *>::iterator iter;
	for (iter = m_screenobjects.begin(); iter != m_screenobjects.end(); iter++)
	{
		CAnmDrawable *pDrawable = *iter;

		pDrawable->UpdateScreenGroupMatrix(pDevice, pCamera);
	}


	m_screenobjectsupdated = true;
	m_lastcameraposforscreen = camerapos;
}


// LightInstance
CAnmLightInstance::CAnmLightInstance(cApplication *pApp, eAnmLightType lightType)
{
	assert(pApp);

	CAnmGraphics *pGraphics = pApp->GetGraphics();

	m_light = pGraphics->CreateLight(lightType);
	m_lightIndex = ANMLIGHTINDEX_BADINDEX;
}

CAnmLightInstance::CAnmLightInstance()
{
	m_light = NULL;
	m_lightIndex = ANMLIGHTINDEX_BADINDEX;
}

void CAnmLightInstance::CreateLight(cApplication *pApp, eAnmLightType lightType)
{
	assert(pApp);

	CAnmGraphics *pGraphics = pApp->GetGraphics();

	m_light = pGraphics->CreateLight(lightType);
}


CAnmLightInstance::~CAnmLightInstance()
{
	SafeDelete(m_light);
}

// LightGroup

CAnmLightGroup::CAnmLightGroup()
{
	m_parentgroup = NULL;
}

CAnmLightGroup::~CAnmLightGroup()
{
}

void CAnmLightGroup::AddLight(CAnmLightInstance *pLight)
{
	assert(pLight);
	m_lights.push_back(pLight);
}

void CAnmLightGroup::RemoveLight(CAnmLightInstance *pLight)
{
	assert(pLight);
	m_lights.remove(pLight);
}

void CAnmLightGroup::SetParentGroup(CAnmLightGroup *pLightGroup)
{
	assert(pLightGroup);

	m_parentgroup = pLightGroup;
}

void CAnmLightGroup::ClearLights()
{
	m_lights.clear();
}

void CAnmLightGroup::ClearLighting(CAnmDevice *pDevice)
{
	// Clear out my lights
	std::list<CAnmLightInstance *>::iterator lightiter;

	for (lightiter = m_lights.begin();
		lightiter != m_lights.end();
		lightiter++)
	{
			CAnmLightInstance *pLight = (*lightiter);
			if (pLight)
			{
				pDevice->EnableLight(pLight->GetLightIndex(), false);
			}
	}

	// Now recurse
	if (m_parentgroup)
		m_parentgroup->ClearLighting(pDevice);
}

void CAnmLightGroup::SetLighting(CAnmDevice *pDevice)
{
	// Set my lights
	std::list<CAnmLightInstance *>::iterator lightiter;

	for (lightiter = m_lights.begin();
		lightiter != m_lights.end();
		lightiter++)
	{
			CAnmLightInstance *pLight = (*lightiter);
			if (pLight)
			{
				pDevice->SetLight(pLight->GetLightIndex(), pLight->GetLight());
#ifdef DEBUG
				if (CAnmLight::GetShowLights())
					pDevice->EnableLight(pLight->GetLightIndex(), pLight->GetOn());
				else
					pDevice->EnableLight(pLight->GetLightIndex(), false);
#else
				pDevice->EnableLight(pLight->GetLightIndex(), pLight->GetOn());
#endif
			}
	}

	// Now recurse
	if (m_parentgroup)
		m_parentgroup->SetLighting(pDevice);
}

void CAnmLightGroup::ChangeLighting(class CAnmDevice *pDevice, 
		CAnmLightGroup *pOldLighting, CAnmLightGroup *pNewLighting)
{
	// turn old lights off
	if (pOldLighting)
		pOldLighting->ClearLighting(pDevice);

	// turn new lights on
	if (pNewLighting)
		pNewLighting->SetLighting(pDevice);
}

void sAnmPickList<CAnmDrawable, CAnmRenderGroup>::Clear()
{
	for (int i = 0; i < pickList.size(); i++)
	{
		sAnmPickDesc<CAnmDrawable, CAnmRenderGroup> pickDesc = pickList[i];

		if (pickDesc.pParentArray)
			delete pickDesc.pParentArray;
	}
	pickList.clear();
}

void sAnmPickList<CAnmDrawable, CAnmRenderGroup>::Clear1(sAnmPickDesc<CAnmDrawable, CAnmRenderGroup> &pickDesc)
{
	if (pickDesc.pParentArray)
		delete pickDesc.pParentArray;
}
