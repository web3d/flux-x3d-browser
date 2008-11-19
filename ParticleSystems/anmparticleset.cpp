/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			anmparticleset.cpp
 *		Description:	Anima ParticleSet node
 *
 *
 *
 * (C) 2001-2004 by Media Machines
 *
 *		Revision history:
 *			$Log: anmparticleset.cpp,v $
 *			Revision 1.1.1.1  2003/09/10 02:18:12  turk
 *			build466
 *			
 *
 ******************************************************************/

#include "stdafx.h"
#include "anmparticleset.h"
#include "application.h"
#include "anmcamera.h"
#include "anmcolor.h"
#include "anmcoordinate.h"
#include "anmdevice.h"
#include "anmfrustum.h"
#include "anmoctree.h"
#include "anmprimitive.h"
#include "anmmesh.h"
#include "anmviewport.h"
#include "anmworld.h"
#include "anmmath.h"

CAnmParticleSet::CAnmParticleSet() : 
	
	CAnmGeometry()
{
	m_color = NULL;
	m_coord = NULL;
	m_size = new FloatArray;

	m_primsupdated = false;
	m_prevcamerapos = point3::Zero;
	m_prevcameradir = point3::Zero;
	m_prevcameraup = point3::Zero;

	m_noalphamesh = NULL;
}

CAnmParticleSet::~CAnmParticleSet()
{
	SafeUnRef(m_size);
	SafeUnRef(m_noalphamesh);
}

eAnmReturnStatus CAnmParticleSet::AddChild(CAnmNode *pChild)
{
	CAnmGroup::AddChild(pChild);

	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmColor)))
		m_color = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_coord = pChild;

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmParticleSet::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmColor)))
		m_color = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_coord = NULL;

	CAnmGroup::RemoveChild(pChild);

	return eAnmReturnAllGood;
}

void CAnmParticleSet::ChildChanged(CAnmNode *pChild)
{
	if (pChild == m_color)
	{
		SetDirtyBits(eAnmParticleSetColorDirty);
	}
	else if (pChild == m_coord)
	{
		SetDirtyBits(eAnmParticleSetCoordDirty);
	}

	SetStateDirty();
}

void CAnmParticleSet::Update()
{
	if (!TestDirtyBits(eAnmNodeStateDirty | eAnmNodeMatrixDirty))
		return;

	assert(m_mesh);

	tAnmPrimitiveArray *pPrimitives = m_mesh->GetPrimitives();
	CAnmPointList *pPointList =
		(CAnmPointList *) (*pPrimitives)[0];

	if (TestDirtyBits(eAnmParticleSetColorDirty))
	{
	}
	if (TestDirtyBits(eAnmParticleSetCoordDirty))
	{
		CAnmCoordinate *pCoord = GETIMPLEMENTATION(CAnmCoordinate, m_coord);

		pCoord->Update();
		pPointList->SetCoordinates(pCoord->GetPoint());
	}

	ClearStateDirty();
}

void CAnmParticleSet::CreateMesh()
{
	Point3Array *pCoordinates = NULL;
	ColorArray *pColors = NULL;
	int nCoords = 0; 
	int nColors = 0;
	int nSizes = 0;
	CAnmColor *pColorNode = GETIMPLEMENTATION(CAnmColor, m_color);
	CAnmCoordinate *pCoordNode = GETIMPLEMENTATION(CAnmCoordinate, m_coord);

	if (pCoordNode == NULL)
	{
		// N.B.: I think we should issue a warning here, no?
		return;								// what else to do?
	}

	pCoordinates = pCoordNode->GetPoint();
	assert(pCoordinates);

	nCoords = pCoordinates->size();
	nSizes = m_size->size();

	if (pColorNode)
	{
		pColors = pColorNode->GetColor();
		nColors = pColors->size();
	}

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	m_mesh = new CAnmMesh();
	assert(m_mesh);

	m_noalphamesh = new CAnmMesh();
	assert(m_noalphamesh);

	int chunksize = ANMMAXVERTICES / 6;
	int nCoordsRemaining = nCoords;
	Point3 *pCoords = &(*pCoordinates)[0];

	int index = 0;
	while (nCoordsRemaining > 0)
	{
		int nCoords = min(nCoordsRemaining, chunksize);

		CAnmTriangleList *pTriangleList = new CAnmTriangleList(pDevice);
		tAnmVertexArray *pVerts = pTriangleList->GetVertices();

		Point3 normal = point3::k;
		for (int i = 0; i < nCoords; i++, index++)
		{
			// Make first tri of quad center at the current vertex
				
			Point3 center = pCoords[i];

			float quadsize = 1.f;
			if (m_size && nSizes > index)
				quadsize = (*m_size)[index];

			float quadoffset = quadsize * .5;

			Color c;
			if (pColors && nColors > index)
				c = (*pColors)[index];

			sAnmVertex v;
			Point2 uv;

			v.Assign(center.x - quadoffset, center.y + quadoffset, center.z);
			v.SetDiffuseColor(c);
			v.SetNormal(normal);
			uv.x = 0, uv.y = 1;
			v.SetTexCoords(uv);
			pVerts->push_back(v);

			v.Assign(center.x - quadoffset, center.y - quadoffset, center.z);
			v.SetDiffuseColor(c);
			v.SetNormal(normal);
			uv.x = 0, uv.y = 0;
			v.SetTexCoords(uv);
			pVerts->push_back(v);

			v.Assign(center.x + quadoffset, center.y - quadoffset, center.z);
			v.SetDiffuseColor(c);
			v.SetNormal(normal);
			uv.x = 1, uv.y = 0;
			v.SetTexCoords(uv);
			pVerts->push_back(v);


			// Make second tri of quad center at the current vertex
				
			v.Assign(center.x + quadoffset, center.y - quadoffset, center.z);
			v.SetDiffuseColor(c);
			v.SetNormal(normal);
			uv.x = 1, uv.y = 0;
			v.SetTexCoords(uv);
			pVerts->push_back(v);

			v.Assign(center.x + quadoffset, center.y + quadoffset, center.z);
			v.SetDiffuseColor(c);
			v.SetNormal(normal);
			uv.x = 1, uv.y = 1;
			v.SetTexCoords(uv);
			pVerts->push_back(v);

			v.Assign(center.x - quadoffset, center.y + quadoffset, center.z);
			v.SetDiffuseColor(c);
			v.SetNormal(normal);
			uv.x = 0, uv.y = 1;
			v.SetTexCoords(uv);
			pVerts->push_back(v);
		
		}

		m_mesh->AddPrimitive(pTriangleList);

		// Sort it if it's big
		pTriangleList->Sort();

		// Create another mesh for the non-alpha-sorted stuff
		CAnmTriangleList *pNoAlphaTriangleList = new CAnmTriangleList(pDevice);
		m_noalphamesh->AddPrimitive(pNoAlphaTriangleList );

		// increment pointers
		nCoordsRemaining -= chunksize;
		pCoords += chunksize;
	}

	// set a default color if we don't have one. do it before normal generation
	if (m_color == NULL)
		m_mesh->SetDiffuseColor(color4::White);

	int sz = pCoordinates->size() * 6;
}

static float CalcPixelSize(CAnmDevice *pDevice, CAnmCamera *pCamera)
{
	CAnmViewport *pViewport = pDevice->GetViewport();
	assert(pViewport);

	float fov = pCamera->GetFieldOfView();
	int windowHeight = pViewport->Height();

	// N.B.: If you don't know why this works, email Chris Laurel...
	return (float) tan(fov / 2.0) / (float) windowHeight;
}


void CAnmParticleSet::Render(CAnmDevice *pDevice, int texturestage, CAnmCamera *pCamera,
						  const matrix4 &worldmat)
{

	if (!m_primsupdated)
		UpdatePrimitives(pDevice, pCamera, worldmat, NULL);

	// Render the non-alpha blended stuff
	pDevice->SetAlphaBlending(false);

#if 0
	LPDIRECT3DDEVICE7 pD3DDevice = pDevice->GetD3DDevice();
	LPDIRECTDRAWSURFACE7 pD3DSurface = NULL;
	pD3DDevice->GetTexture(0, &pD3DSurface);
	pD3DDevice->SetTexture(0, NULL);
#endif

	m_noalphamesh->Render(pDevice, texturestage);
	
	pDevice->SetAlphaBlending(true);

#if 0
	pD3DDevice->SetTexture(0, pD3DSurface);
#endif

	// Now render the rest
	m_mesh->Render(pDevice, texturestage);

	m_primsupdated = false;
}

void CAnmParticleSet::UpdatePrimitives(CAnmDevice *pDevice, CAnmCamera *pCamera,
						  const matrix4 &worldmat, class CAnmAlphaBuffer *pAlphaBuffer)
{
	Point3Array *pCoordinates = NULL;
	ColorArray *pColors = NULL;
	int nCoords = 0;
	int nSizes = 0;
	CAnmColor *pColorNode = GETIMPLEMENTATION(CAnmColor, m_color);
	CAnmCoordinate *pCoordNode = GETIMPLEMENTATION(CAnmCoordinate, m_coord);

	if (pCoordNode == NULL)
	{
		// N.B.: I think we should issue a warning here, no?
		return;								// what else to do?
	}

	pCoordinates = pCoordNode->GetPoint();
	assert(pCoordinates);

	nCoords = pCoordinates->size();
	nSizes = m_size->size();

	assert(m_mesh);

	// Get up vector from camera (world space)
	Point3 cameradir, cameraup;
	pCamera->GetOrientation(&cameradir, &cameraup);

	// Get camera position (world space)
	Point3 camerapos;
	pCamera->GetPosition(&camerapos);

	if (m_prevcamerapos == camerapos &&
		m_prevcameradir == cameradir &&
		m_prevcameraup == cameraup)
	{
		m_primsupdated = true;
		return;
	}

	// Translate camera pos, dir, up into model space
	matrix4 w2m;
	MatrixInvert(w2m, (matrix4 &) worldmat);

	Point3 mcamerapos = w2m * camerapos;
	Point3 mcameradir = w2m * cameradir;
	Point3 mcameraup = w2m * cameraup;

	// Substract out pesky origin for vectors
	mcameradir = mcameradir - (w2m * Point3::Zero);
	mcameraup = mcameraup - (w2m * Point3::Zero);

	// Now we have camera dir and up in model space
	// BB2V is opposite camera direction
	Point3 bb2v = -mcameradir;
	bb2v.Normalize();

	float theta = acos(bb2v * mcameraup);

	matrix4 bbmat;

	// Point at camera but with same up vector as camera (unless the two vectors parallel)
	if (fabs(theta) > EPSILON)
	{
		bbmat.ToObjectLookAt(Point3::Zero, bb2v, mcameraup);
	}
	else
	{
		bbmat = matrix4::Identity;
	}


	float pixelSize = CalcPixelSize(pDevice, pCamera);

	int chunksize = ANMMAXVERTICES / 6;
	int nCoordsRemaining = nCoords;
	int nSizesRemaining = nSizes;
	Point3 *pCoords = &(*pCoordinates)[0];
	float *pSizes = &(*m_size)[0];

	tAnmPrimitiveArray *pPrims = m_mesh->GetPrimitives();
	tAnmPrimitiveArray *pNoAlphaPrims = m_noalphamesh->GetPrimitives();

	int index = 0;
	int chunkindex = 0;

	while (nCoordsRemaining > 0)
	{
		int nCoords = min(nCoordsRemaining, chunksize);
		int nSizes = min(nSizesRemaining, chunksize);
		if (nSizes <= 0)
		{
			pSizes = NULL;
			nSizes = 0;
		}

		CAnmTriangleList *pTriangleList = (CAnmTriangleList *) (*pPrims)[chunkindex];
		CAnmTriangleList *pNoAlphaTriangleList = (CAnmTriangleList *) (*pNoAlphaPrims)[chunkindex];
		tAnmVertexArray *pNoAlphaVerts = pNoAlphaTriangleList->GetVertices();
		pNoAlphaVerts->clear();

		UpdatePrimitive(pTriangleList, pNoAlphaTriangleList, pDevice, pCamera, bbmat, pCoords, nCoords,
			pSizes, nSizes, mcamerapos, worldmat, pixelSize);

		// increment pointers
		nCoordsRemaining -= chunksize;
		nSizesRemaining -= chunksize;
		pCoords += chunksize;
		pSizes += chunksize;
		chunkindex++;
	}


	m_primsupdated = true;
	m_prevcamerapos = camerapos;
	m_prevcameradir = cameradir;
	m_prevcameraup = cameraup;
}

void CAnmParticleSet::UpdatePrimitive(CAnmTriangleList *pTriangleList, CAnmTriangleList *pNoAlphaTriangleList,
									  CAnmDevice *pDevice, CAnmCamera *pCamera, CAnmOctree *pOctree,
									  Point3 *pCoords, Point3 *bbvec, Point3 &camerapos, const matrix4 &worldmat,
									  float pixelSize, class CAnmAlphaBuffer *pAlphaBuffer)
{
	assert(pOctree);
	
	int nSizes = m_size->size();

	if (pOctree->IsLeaf())
	{
		tAnmVertexArray *pVerts = pTriangleList->GetVertices();

		int nTriangleIndices = pOctree->GetTriangleIndexCount();
		int *pTriangleIndices = pOctree->GetTriangleIndices();

		for (int i = 0; i < nTriangleIndices; i++)
		{
			// N.B.: 2 triangles generated from each center, so ->
			// the coordinate index is half the triangle index...
			int triangleindex = pTriangleIndices[i];

			int coordindex = triangleindex / 2;

			float quadsize = 1.f;
			if (m_size && nSizes > coordindex)
				quadsize = (*m_size)[coordindex] * 1.f;

			float quadoffset = quadsize * .5;

			// ... now we have a decent coord index for the center point.

			// Update first tri of quad centered at the current vertex
			
			Point3 center = pCoords[coordindex];

			Point3 cameratocenter = center - camerapos;
			float dist = cameratocenter.Mag();

			float sizeInPixels = quadsize / (dist * pixelSize);
			
			(*pVerts)[coordindex * 6].SetVec(center + bbvec[0] * quadoffset);

			(*pVerts)[coordindex * 6 + 1].SetVec(center + bbvec[1] * quadoffset);

			(*pVerts)[coordindex * 6 + 2].SetVec(center + bbvec[2] * quadoffset);

			// Update second tri of quad centered at the current vertex
				
			(*pVerts)[coordindex * 6 + 3].SetVec(center + bbvec[2] * quadoffset);

			(*pVerts)[coordindex * 6 + 4].SetVec(center + bbvec[3] * quadoffset);

			(*pVerts)[coordindex * 6 + 5].SetVec(center + bbvec[0] * quadoffset);

			if (sizeInPixels < 1)
			{
				sAnmVertex v[6];
				v[0] = (*pVerts)[coordindex * 6];
				v[1] = (*pVerts)[coordindex * 6 + 1];
				v[2] = (*pVerts)[coordindex * 6 + 2];
				v[3] = (*pVerts)[coordindex * 6 + 3];
				v[4] = (*pVerts)[coordindex * 6 + 4];
				v[5] = (*pVerts)[coordindex * 6 + 5];

				pNoAlphaTriangleList->AddVertices(6, &v[0]);
			}
		}

	}
	else
	{
		CAnmOctree *octants[8];

		pOctree->GetOctants(octants);

		for (int i = 0; i < 8; i++)
		{
			if (octants[i])
			{
				CAnmOctree *pChildOct = octants[i];
				CAnmBoundingBox bbox;

				pChildOct->GetBoundingBox(&bbox);
				CAnmBoundingBox vsbbox = bbox.Transform((matrix4 &) worldmat);

				CAnmFrustum *pFrustum = pCamera->GetFrustum();
				assert(pFrustum);

				bool culled = pFrustum->Cull(vsbbox);
				if (!culled)
				{
					UpdatePrimitive(pTriangleList, pNoAlphaTriangleList, pDevice, pCamera, pChildOct, pCoords, bbvec,
						camerapos, worldmat, pixelSize);
				}
			}
		}
	}
}

void CAnmParticleSet::UpdatePrimitive(CAnmTriangleList *pTriangleList, CAnmTriangleList *pNoAlphaTriangleList,
									  CAnmDevice *pDevice, CAnmCamera *pCamera, const matrix4 &bbmat,
									  Point3 *pCoords, int nCoords, float *pSizes, int nSizes, Point3 &camerapos, const matrix4 &worldmat, 
									  float pixelSize, class CAnmAlphaBuffer *pAlphaBuffer)
{
	Point3 bbvec[4];

	bbvec[0] = bbmat * Point3(-1, 1, 0);
	bbvec[1] = bbmat * Point3(-1, -1, 0);
	bbvec[2] = bbmat * Point3(1, -1, 0);
	bbvec[3] = bbmat * Point3(1, 1, 0);

	CAnmOctree *pOctree = pTriangleList->GetOctree();

	CAnmFrustum *pFrustum = pCamera->GetFrustum();

	if (pOctree)
	{
		// Get frustum for cull
		CAnmViewport *pViewport = pDevice->GetViewport();
		assert(pViewport);

		UpdatePrimitive(pTriangleList, pNoAlphaTriangleList, pDevice, pCamera, pOctree, pCoords, bbvec, camerapos, worldmat, pixelSize);
		return;
	}

	tAnmVertexArray *pVerts = pTriangleList->GetVertices();

	for (int i = 0; i < nCoords; i++)
	{
		float quadsize = 1.f;
		if (pSizes && nSizes > i)
			quadsize = pSizes[i] * 1.f;

		float quadoffset = quadsize * .5;

		// Update first tri of quad centered at the current vertex
			
		Point3 center = pCoords[i];

		(*pVerts)[i * 6].SetVec(center + bbvec[0] * quadoffset);

		(*pVerts)[i * 6 + 1].SetVec(center + bbvec[1] * quadoffset);

		(*pVerts)[i * 6 + 2].SetVec(center + bbvec[2] * quadoffset);

		// Update second tri of quad centered at the current vertex
			
		(*pVerts)[i * 6 + 3].SetVec(center + bbvec[2] * quadoffset);

		(*pVerts)[i * 6 + 4].SetVec(center + bbvec[3] * quadoffset);

		(*pVerts)[i * 6 + 5].SetVec(center + bbvec[0] * quadoffset);

	}
}


// CAnmGeometry overrides
void CAnmParticleSet::Realize()
{
	CreateMesh();
	CAnmGeometry::Realize();		// chain down
}

eAnmRenderOptions CAnmParticleSet::GetRenderOptions()
{
	if (m_color)
		return eAnmRenderOptionsUseVertexForEmissive;
	else
		return eAnmRenderOptionsNone;
}


// Accessors

void CAnmParticleSet::SetColor(CAnmNode *pColor)
{
	if (m_color == pColor)
		return;

	if (m_color)
		RemoveChild(m_color);

	if (pColor == NULL)
		return;

	AddChild(pColor);
	m_color = pColor;

	SetDirtyBits(eAnmParticleSetColorDirty);

	CallCallbacks(CALLBACKID(CAnmParticleSet, color_changed), &m_color);
}


void CAnmParticleSet::SetCoord(CAnmNode *pCoord)
{
	if (m_coord == pCoord)
		return;

	if (m_coord)
		RemoveChild(m_coord);

	if (pCoord == NULL)
		return;

	AddChild(pCoord);
	m_coord = pCoord;

	SetDirtyBits(eAnmParticleSetCoordDirty);

	CallCallbacks(CALLBACKID(CAnmParticleSet, coord_changed), &m_coord);
}

void CAnmParticleSet::SetSize(FloatArray *pSize)
{
	assert(pSize != NULL);

	SafeUnRef(m_size);
	m_size = pSize;
	m_size->Ref();

	SetDirtyBits(eAnmParticleSetSizeDirty);

	CallCallbacks(CALLBACKID(CAnmParticleSet, size_changed), &pSize);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmParticleSet, color, eValueNode, CAnmNode*, 
			 GetColor, SetColor);
DEFINE_VALUE(CAnmParticleSet, coord, eValueNode, CAnmNode*, 
			 GetCoord, SetCoord);
DEFINE_VALUE(CAnmParticleSet, size, eValueFloatArray, FloatArray*, 
			 GetSize, SetSize);

DEFINE_METHOD(CAnmParticleSet, set_color, eValueNode, CAnmNode*, SetColor);
DEFINE_METHOD(CAnmParticleSet, set_coord, eValueNode, CAnmNode*, SetCoord);
DEFINE_METHOD(CAnmParticleSet, set_size, eValueFloatArray, FloatArray*, SetSize);


DEFINE_CALLBACK(CAnmParticleSet, color_changed, eValueNode);
DEFINE_CALLBACK(CAnmParticleSet, coord_changed, eValueNode);
DEFINE_CALLBACK(CAnmParticleSet, size_changed, eValueFloatArray);



// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmParticleSet)
VALUEID(CAnmParticleSet, color), 
VALUEID(CAnmParticleSet, coord), 
VALUEID(CAnmParticleSet, size), 

METHODID(CAnmParticleSet, set_color), 
METHODID(CAnmParticleSet, set_coord), 
METHODID(CAnmParticleSet, set_size), 

CALLBACKID(CAnmParticleSet, color_changed),
CALLBACKID(CAnmParticleSet, coord_changed),
CALLBACKID(CAnmParticleSet, size_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmParticleSet, CAnmGeometry);


