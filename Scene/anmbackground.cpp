/********************************************************************************
 * Flux
 *
 * File: anmbackground.cpp
 * Description: Background node
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
#include "anmbackground.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmfilemanager.h"
#include "anminterpolator.h"
#include "anmlayer.h"
#include "anmmath.h"
#include "anmmesh.h"
#include "anmtexturedata.h"
#include "anmworld.h"

CAnmMesh *CAnmBackground::m_skyBox[6] =
{ 
	NULL, NULL, NULL, NULL, NULL, NULL
};

CAnmBackground::CAnmBackground() : CAnmBindableNode()
{		
	m_groundAngle = new FloatArray;
	m_groundColor = new ColorArray;
	m_backUrl = new StringArray;
	m_bottomUrl = new StringArray;
	m_frontUrl = new StringArray;
	m_leftUrl = new StringArray;
	m_rightUrl = new StringArray;
	m_topUrl = new StringArray;
	m_skyAngle = new FloatArray;
	m_skyColor = new ColorArray;

	m_camera = NULL;

	m_skyColor->push_back(ANMBACKGROUND_DEFAULT_SKYCOLOR);

	for (int i = 0; i < 6; i++)
		m_textureSurfaces[i] = NULL;

	m_groundSphere = NULL;
	m_skySphere = NULL;

	m_baseUrl = NULL;
}

CAnmBackground::~CAnmBackground()
{
	SafeUnRef(m_groundAngle);
	SafeUnRef(m_groundColor);
	SafeUnRef(m_backUrl);
	SafeUnRef(m_bottomUrl);
	SafeUnRef(m_frontUrl);
	SafeUnRef(m_leftUrl);
	SafeUnRef(m_rightUrl);
	SafeUnRef(m_topUrl);
	SafeUnRef(m_skyAngle);
	SafeUnRef(m_skyColor);

	for (int i = 0; i < 6; i++)
		SafeUnRef(m_textureSurfaces[i]);

	SafeUnRef(m_groundSphere);
	SafeUnRef(m_skySphere);

	SafeUnRef(m_baseUrl);
}

void CAnmBackground::Realize()
{
	if (m_realized)
		return;

	CAnmBindableNode::Realize();

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	CreateVolumes();

	LoadTextures();

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	m_camera = new CAnmCamera(pApp, pWorld->AllocLight(), false);

	m_camera->SetPosition(Point3(0, 0, 0));
	m_camera->SetFieldOfView(PI/2);
	m_camera->SetNearDistance(.125);
	m_camera->SetFarDistance(2.f);
}


void CAnmBackground::Update()
{
	// krv:
	// We need to update!

	if (!StateDirty())
		return;

	// we need to check dirty bits, and update textures or spheres if needed.
	if (TestDirtyBits(eAnmBackgroundSphereDirty)) {
		CreateSpheres();
	}

	if (TestDirtyBits(eAnmBackgroundBoxDirty) ) {
		CreateBox();
	}
	ClearStateDirty();
}

color4 CAnmBackground::GetSimpleBackgroundColor()
{
	color4 c = ANMBACKGROUND_DEFAULT_SKYCOLOR;

	if (m_skyColor->size() > 0)
		c = (*m_skyColor)[m_skyColor->size()-1];

	return c;
}

bool CAnmBackground::IsSimpleBackgroundColor()
{
	assert(m_skyColor && m_groundColor);

	return (m_skyColor->size() <= 1 && m_groundColor->size() <= 1 );
}

void CAnmBackground::SetBaseUrl(String baseUrl)
{
	SafeUnRef(m_baseUrl);
	m_baseUrl = baseUrl;
	m_baseUrl->Ref();
}

void CAnmBackground::CreateVolumes()
{
	if (!IsSimpleBackgroundColor())
		CreateSpheres();

	if (m_skyBox[0] == NULL)
		CreateBox();
}

// Ground and Sky Spheres:
// N.B.: Kind of copied from the Sphere node code; we should try to share this
#define BGSPHERE_NCROSSSECTIONS	18
#define BGSPHERE_NSECTIONS		18

CAnmMesh *CAnmBackground::CreateSphere(float radius, float lastAngle, bool up )
{
	int nSphereCrossSections = BGSPHERE_NCROSSSECTIONS;
	int nSphereSections = BGSPHERE_NSECTIONS;
	int nSphereVertices = (nSphereSections*nSphereCrossSections);

	int ncrossecs = nSphereCrossSections;

	int i, j;

	float *r = new float[ncrossecs];
	float *h = new float[ncrossecs];

	float phi;
	float dphi;

	if (up)
	{
		phi = 0;
		dphi = lastAngle / (float) (nSphereCrossSections-1);
	}
	else
	{
		phi = PI;
		dphi = -lastAngle / (float) (nSphereCrossSections-1);
	}

	for (i = 0; i < ncrossecs; i++)
	{
		if ( i == 0 )
		{
			r[i] = 0;
		}
		else
		{
			r[i] = sin(phi);
		}
		h[i] = cos(phi);

		phi += dphi;
	}

	float *x = new float[nSphereSections + 1];
	float *z = new float[nSphereSections + 1];

	// create angle multipliers for each section
	float theta = 1.5f * PI;
	float dtheta = 2.f * PI / (float) nSphereSections;

	for (i=0; i<nSphereSections+1; i++) {
		x[i] = cos(theta);
		z[i] = sin(theta);
		theta += dtheta;
	}

	// Create the mesh
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	CAnmMesh *pMesh = new CAnmMesh();
	CAnmIndexedTriangleList *pTriangleList = new CAnmIndexedTriangleList(pDevice);
	tAnmVertexArray *pVertices = pTriangleList->GetVertices();
	tAnmIndexArray *pIndices = pTriangleList->GetIndices();

	pVertices->reserve(nSphereVertices);
	pIndices->reserve( nSphereSections * ( ( ncrossecs-2 ) * 6 + 3 ) );

	for (i = 0; i < ncrossecs; i++)
	{
		for (j = 0; j < nSphereSections; j++)
		{
			Point3 vec;

			vec.x = x[j] * r[i];
			vec.y = h[i];
			vec.z = z[j] * r[i];

			// Scale vector by the desired radius
			vec = vec * radius;

			// N.B.: the normal is inward-pointing, the reverse of the vector location
			sAnmVertex vert(vec, -vec);
			pVertices->push_back(vert);
		}
	}

	int j1, j2;
	for (i = 1; i < ncrossecs; i++)
	{
		int offset1 = i * (nSphereSections);
		int offset2 = (i - 1) * (nSphereSections);
		for (j = 0; j < nSphereSections; j++)
		{

			j1 = j;
			j2 = ( j+1 ) % nSphereSections;
			// at the poles, we have 3-vertex facets. everywhere
			// else it's 4
			if (i == 1 )
			{

				pIndices->push_back(offset2 + j1);

				if (up)
				{
					pIndices->push_back(offset1 + j1);
					pIndices->push_back(offset1 + j2);
				}
				else
				{
					pIndices->push_back(offset1 + j2);
					pIndices->push_back(offset1 + j1);
				}
			} 
			else
			{
				pIndices->push_back(offset1 + j2);
				if (up)
				{
					pIndices->push_back(offset2 + j2);
					pIndices->push_back(offset2 + j1);
				}
				else
				{
					pIndices->push_back(offset2 + j1);
					pIndices->push_back(offset2 + j2);
				}

				pIndices->push_back(offset1 + j1);
				if (up)
				{
					pIndices->push_back(offset1 + j2);
					pIndices->push_back(offset2 + j1);
				}
				else
				{
					pIndices->push_back(offset2 + j1);
					pIndices->push_back(offset1 + j2);
				}

			}
		}
	}


	pMesh->AddPrimitive(pTriangleList);

	delete r;
	delete h;
	delete x;
	delete z;

	return pMesh;
}

void CAnmBackground::ApplyColors(CAnmMesh *pSphereMesh,
								 ColorArray *pColors, FloatArray *pAngles,
								 bool up )
{
	assert(pColors);
	assert(pAngles);

	int i, j;
	float fract;

	int nangles = pAngles->size();


	FloatArray *pKeys = new FloatArray;
	pKeys->reserve(nangles + 1);

	float lastAngle = (*pAngles)[nangles - 1];

	pKeys->push_back(0);
	for (i = 0; i < nangles; i++)
	{
		fract = (*pAngles)[i] / lastAngle;
		fract = ClampValue(0.f, fract, 1.f);
		pKeys->push_back(fract);
	}
	
	CAnmInterpolator<Color>	interp;
	interp.SetKey(pKeys);
	interp.SetKeyValue(pColors);

	int nSphereCrossSections = BGSPHERE_NCROSSSECTIONS;
	int nSphereSections = BGSPHERE_NSECTIONS;

	int ncrossecs = nSphereCrossSections;

	float phi;
	float dphi;

	if (up)
	{
		phi = 0;
		dphi = lastAngle / (float) (nSphereCrossSections-1);
	}
	else
	{
		phi = PI;
		dphi = -lastAngle / (float) (nSphereCrossSections-1);
	}

	tAnmPrimitiveArray *pPrimitives =
		pSphereMesh->GetPrimitives();

	CAnmIndexedTriangleList *pTriangleList = 
		(CAnmIndexedTriangleList *) (*pPrimitives)[0];

	tAnmVertexArray *pVertices = pTriangleList->GetVertices();

	int nvertices = pVertices->size();
	int vindex = 0;

	Color c;
	for (i = 0; i < ncrossecs; i++)
	{
		if (up)
			fract = phi / lastAngle;
		else
			fract = (PI - phi) / lastAngle;

		interp.RGBInterp(fract, &c);

		for (j = 0; j < nSphereSections; j++)
		{
			vindex = i * (nSphereSections) + j;
			sAnmVertex vert = (*pVertices)[vindex];

			vert.SetDiffuseColor(c);

			(*pVertices)[vindex] = vert;
		}
		phi += dphi;
	}

	SafeUnRef(pKeys);
}

#define SKYSPHEREOFFSET 0.0001f

void CAnmBackground::CreateSpheres()
{
	// krv;
	// Spec says MUST have one more color than angle.
	SafeUnRef(m_skySphere);
	SafeUnRef(m_groundSphere);
	if (m_skyColor &&
		m_skyAngle &&
		m_skyColor->size() > 1 &&
		m_skyAngle->size() + 1 == m_skyColor->size() )
	{
		m_skySphere = CreateSphere(ANM_SQRT2 + SKYSPHEREOFFSET, (*m_skyAngle)[m_skyAngle->size()-1], true );
		ApplyColors(m_skySphere, m_skyColor, m_skyAngle, true );
	}

	if (m_groundColor &&
		m_groundAngle &&
		m_groundColor->size() > 1 &&
		m_groundAngle->size() + 1 == m_groundColor->size() )
	{
		m_groundSphere = CreateSphere(ANM_SQRT2, (*m_groundAngle)[m_groundAngle->size()-1], false );
		ApplyColors(m_groundSphere, m_groundColor, m_groundAngle, false );
	}
}



// Skybox:
// vertices and normals of a 2-unit cube with inward-pointing faces 
// all data in RH coordinates

// N.B.: inward-pointing faces
static Point3 front_vertices[] = {
	Point3( 1.f, 1.f,-1.f),
	Point3(-1.f, 1.f,-1.f),
	Point3(-1.f,-1.f,-1.f),
	Point3( 1.f,-1.f,-1.f),
};

static Point3 front_normals[] = {
	Point3(0.0f,  0.0f,  1.0f),
};

static Point3 top_vertices[] = {
	Point3( 1.f, 1.f, 1.f),
	Point3(-1.f, 1.f, 1.f),
	Point3(-1.f, 1.f,-1.f),
	Point3( 1.f, 1.f,-1.f),
};

static Point3 top_normals[] = {
	Point3(0.0f,  -1.0f,  0.0f),
};

static Point3 right_vertices[] = {
	Point3(1.f, 1.f, 1.f),
	Point3(1.f, 1.f,-1.f),
	Point3(1.f,-1.f,-1.f),
	Point3(1.f,-1.f, 1.f),
};

static Point3 right_normals[] = {
	Point3(-1.0f,  0.0f,  0.0f),
};

static Point3 back_vertices[] = {
	Point3(-1.f, 1.f, 1.f),
	Point3( 1.f, 1.f, 1.f),
	Point3( 1.f,-1.f, 1.f),
	Point3(-1.f,-1.f, 1.f),
};

static Point3 back_normals[] = {
	Point3(0.0f,  0.0f,  -1.0f),
};

static Point3 bottom_vertices[] = {
	Point3( 1.f,-1.f,-1.f),
	Point3(-1.f,-1.f,-1.f),
	Point3(-1.f,-1.f, 1.f),
	Point3( 1.f,-1.f, 1.f),
};

static Point3 bottom_normals[] = {
	Point3(0.0f, 1.0f,  0.0f),
};

static Point3 left_vertices[] = {
	Point3(-1.f, 1.f,-1.f),
	Point3(-1.f, 1.f, 1.f),
	Point3(-1.f,-1.f, 1.f),
	Point3(-1.f,-1.f,-1.f),
};

static Point3 left_normals[] = {
	Point3(1.0f,  0.0f,  0.0f),
};

static int coordindices[] = {
	0, 1, 2, 3, -1,
};

static int normalindices[] = {
	0, 0, 0, 0, -1,
};

// Rectangular UVs
static Point2 rect_uvs[] = {
	Point2(1.0f, 1.0f),
	Point2(0.0f, 1.0f),
	Point2(0.0f, 0.0f),
	Point2(1.0f, 0.0f),
};

static int uvindices[] = {
	0, 1, 2, 3, -1,
};


void CAnmBackground::CreateBox()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	sAnmIndexedFaceList rect_facelist =
	{
		4,							// # of rectangle vertices
		NULL,						// coordinate data
		1,							// # of rectangle normals
		NULL,						// normal data
		0,							// colors; not used
		NULL,						// colors; not used
		4,							// # of UV coordinates
		rect_uvs,					// UV data; same for everyone
		5,							// # of coord indices (includes -1)
		coordindices,				// coord index data
		5,							// # of normal indices (includes -1)
		normalindices,				// normal index data
		0,							// colors; not used
		NULL,						// colors; not used
		5,							// # of UV indices (includes -1)
		uvindices,					// UV index data
		TRUE,						// ccw flag
	};

	for (int i = 0; i < 6; i++)
	{
		SafeUnRef( m_skyBox[i] );
		m_skyBox[i] = new CAnmMesh();

		switch (i)
		{
			case ANMBACKGROUND_BACKURL :
				rect_facelist.vertices = back_vertices;
				rect_facelist.normals = back_normals;
				break;

			case ANMBACKGROUND_BOTTOMURL :
				rect_facelist.vertices = bottom_vertices;
				rect_facelist.normals = bottom_normals;
				break;

			case ANMBACKGROUND_FRONTURL :
				rect_facelist.vertices = front_vertices;
				rect_facelist.normals = front_normals;
				break;

			case ANMBACKGROUND_LEFTURL :
				rect_facelist.vertices = left_vertices;
				rect_facelist.normals = left_normals;
				break;

			case ANMBACKGROUND_RIGHTURL :
				rect_facelist.vertices = right_vertices;
				rect_facelist.normals = right_normals;
				break;

			case ANMBACKGROUND_TOPURL :
				rect_facelist.vertices = top_vertices;
				rect_facelist.normals = top_normals;
				break;
		}

		m_skyBox[i]->AddFacesFromIndexedFaceList(pDevice, &rect_facelist, FALSE);
		m_skyBox[i]->SetDiffuseColor(color4::White);
	}
}

typedef struct sAnmBackgroundLoadUrlRequest {
	CAnmBackground *pBackground;
	int textureIndex;
} sAnmBackgroundLoadUrlRequest;

void CAnmBackground::LoadTextures()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	// Free old textures if we had them
	for (int i = 0; i < 6; i++)
		SafeUnRef(m_textureSurfaces[i]);

	for (i = 0; i < 6; i++)
	{
		StringArray *pUrl = NULL;
		switch (i)
		{
			case ANMBACKGROUND_BACKURL :
				pUrl = m_backUrl;
				break;

			case ANMBACKGROUND_BOTTOMURL :
				pUrl = m_bottomUrl;
				break;

			case ANMBACKGROUND_FRONTURL :
				pUrl = m_frontUrl;
				break;

			case ANMBACKGROUND_LEFTURL :
				pUrl = m_leftUrl;
				break;

			case ANMBACKGROUND_RIGHTURL :
				pUrl = m_rightUrl;
				break;

			case ANMBACKGROUND_TOPURL :
				pUrl = m_topUrl;
				break;
		}

		if (pUrl->size() > 0 && (*pUrl)[0]->length() > 0)
		{
			sAnmBackgroundLoadUrlRequest *pBackgroundRequest = new sAnmBackgroundLoadUrlRequest;
			pBackgroundRequest->pBackground = this;
			pBackgroundRequest->textureIndex = i;

			pApp->GetFileManager()->RequestLoadUrl(pUrl, m_baseUrl->GetBuf(), urlLoadedCallback, NULL,
				NULL, NULL, pBackgroundRequest);
		}

	}
}

class CAnmAsset *CAnmBackground::urlLoadedCallback(String pRequestUrl, String pCacheFileName, void *userData)
{
	sAnmBackgroundLoadUrlRequest *pBackgroundRequest = (sAnmBackgroundLoadUrlRequest *) userData;

	assert(pBackgroundRequest != NULL);
	assert(pBackgroundRequest->pBackground != NULL);
	pBackgroundRequest->pBackground->handleUrlLoaded(pRequestUrl, pCacheFileName, pBackgroundRequest->textureIndex);

	delete pBackgroundRequest;

	return NULL;
}

void CAnmBackground::handleUrlLoaded(String pRequestUrl, String pCacheFileName, int textureIndex)
{
	if( GetWorld() == NULL ) {
		return;
	}


	assert(pRequestUrl);
	assert(pCacheFileName);
	assert(textureIndex >= 0);
	assert(textureIndex < 6);

	// use file extension to determine type
	eAnmTextureDataFormat format = eAnmTextureBadFormat;

	const char *ext;
	const char *buf;

	buf = pCacheFileName->GetBuf();
	ext = strrchr(buf, '.');

	if (!stricmp(ext, ".bmp"))
	{
		format = eAnmTextureBMP;
	}
	else if (!stricmp(ext, ".jpg") || !stricmp(ext, ".jpeg") || !stricmp(ext, ".jpe"))
	{
		format = eAnmTextureJPG;
	}
	else if (!stricmp(ext, ".png"))
	{
		format = eAnmTexturePNG;
	}
	else if (!stricmp(ext, ".gif"))
	{
		format = eAnmTextureGIF;
	}
	else
	{
		// N.B.: ok, some nuttiness here: IE's cache returns weird file extension sometimes
		buf = pRequestUrl->GetBuf();
		ext = strrchr(buf, '.');

		if (!stricmp(ext, ".bmp"))
		{
			format = eAnmTextureBMP;
		}
		else if (!stricmp(ext, ".jpg") || !stricmp(ext, ".jpeg") || !stricmp(ext, ".jpe"))
		{
			format = eAnmTextureJPG;
		}
		else if (!stricmp(ext, ".png"))
		{
			format = eAnmTexturePNG;
		}
		else if (!stricmp(ext, ".gif"))
		{
			format = eAnmTextureGIF;
		}
	}

	if (format != eAnmTextureBadFormat)
	{
		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL

		cApplication *pApp = pWorld->GetApp();
		assert(pApp);

		CAnmTextureData *pTextureData = CAnmTextureData::LoadTextureData(pApp, pCacheFileName->GetBuf(), format);
		if (pTextureData)
		{
			// grab a lock; we don't want people trying to render this in a half-made state
			CAnmWorld *pWorld = GetWorld();
			assert(pWorld);

			cApplication *pApp = pWorld->GetApp();

			CAnmDevice *pDevice = pApp->GetDevice();

			pWorld->Lock();

			m_textureSurfaces[textureIndex] = CAnmGraphics::Texture::LoadTexture(pTextureData, pDevice);
			m_textureSurfaces[textureIndex]->Ref();

			// release the lock
			pWorld->Unlock();
		}
	}
}

void CAnmBackground::Render(CAnmDevice *pDevice)
{
	// Copy containing layer's orientation to my camera
	CAnmLayer *pLayer = GetLayer();
	assert(pLayer);

	CAnmCamera *pLayerCamera = pLayer->GetCamera();
	assert(pLayerCamera);

	Point3 dir, up;
	pLayerCamera->GetOrientation(&dir, &up);
	float fov = pLayerCamera->GetFieldOfView();
 
	m_camera->SetOrientation(dir, up);
	
	//Set the Fov.
	m_camera->SetFieldOfView(fov);

	// Set camera values to device
	m_camera->SetToDevice(pDevice);

	// Set world matrix to same as World
	matrix4 savedmat;
	pDevice->GetWorldMatrix(&savedmat);
	
	matrix4 *pWmat = pLayer->GetMatrix();
	pDevice->SetWorldMatrix(*pWmat);

	// Turn off z-buffering
	pDevice->SetZBuffering(false);

	// Turn off lighting
	pDevice->SetLighting(false);

	// Turn off fog
	pDevice->EnableFog(false);

	// Clear out textures for stages 1-N
	int n = pDevice->MaxTextureLevels();
	matrix4 defaultTTmat;
	pDevice->GetDefaultTextureTransform(&defaultTTmat);
	for (int i = 0; i < n; i++)
	{
		pDevice->SetTexture(i, NULL);
		pDevice->SetTextureTransform(i, &defaultTTmat);
		pDevice->SetTextureOption(i, eAnmTextureColorOp, eAnmTextureModeOFF);
		pDevice->SetTextureOption(i, eAnmTextureTexCoordIndex, eAnmTextureTexCoordPassThru);
	} 

	// Set up general texture states for stage 0
	// N.B.: do some texture filtering (should this be settable as an option?

	// krv:
	// Why was this set to False?
	// We need Texture Filtering for the Background Texture.
	// Changed to TRUE, 12/09/05
	pDevice->SetTextureOption(0, eAnmTextureFilter, TRUE );

	// 2D Texture coordinates for now, thank you
	pDevice->SetTextureOption(0, eAnmTextureNumTextureCoordinates, eAnmTextureCount2);

	// Default texture transform
	static matrix4 ttfm = matrix4::Identity;
	pDevice->SetTextureTransform(0, &ttfm);

	// Now, render the objects

	// Sky and ground spheres
	// If we only have on sky color, just clear the window

	// krv;
	// Clear with the background color so we dont have to draw the full sphere
	color4 bgColor = GetSimpleBackgroundColor();
	pDevice->Clear( true, true, bgColor.MakeDWord(), 1.f);

	if (m_skySphere)
		m_skySphere->Render(pDevice, 0);

	if (m_groundSphere)
		m_groundSphere->Render(pDevice, 0);

	// Sky box
	for (i = 0; i < 6; i++)
	{
		if (m_textureSurfaces[i])
		{
			pDevice->SetTexture(0, m_textureSurfaces[i]);
		
			// set some states
			int addressU;
			int addressV;
			
			// addressing modes (wrap or clamp)

			addressU = eAnmTextureAddressClamp;
			addressV = eAnmTextureAddressClamp;

			pDevice->SetTextureOption( 0, eAnmTextureAddressU, addressU);
			pDevice->SetTextureOption( 0, eAnmTextureAddressV, addressV);
			
			// color arguments (modulate or select for grayscale or RGB respectively)
			if (m_textureSurfaces[i]->IsGrayscale())
			{
				// grayscale textures are intensity maps; use modulate mode
				pDevice->SetTextureOption(0, eAnmTextureColorArg1, eAnmTextureArgCurrent);
				pDevice->SetTextureOption(0, eAnmTextureColorArg2, eAnmTextureArgTexture);
				pDevice->SetTextureOption(0, eAnmTextureColorOp, eAnmTextureModeMODULATE);
			}
			else
			{
				pDevice->SetTextureOption(0, eAnmTextureColorArg1, eAnmTextureArgTexture );
				pDevice->SetTextureOption(0, eAnmTextureColorOp, eAnmTextureModeSELECTARG1 );
			}

			if (m_textureSurfaces[i]->HasAlpha())
			{
				pDevice->SetAlphaBlending(true);
				pDevice->SetTextureOption(0, eAnmTextureAlphaOp, eAnmTextureModeMODULATE);
				pDevice->SetTextureOption(0, eAnmTextureAlphaArg1, eAnmTextureArgTexture);	
			}
			else
			{
				pDevice->SetAlphaBlending(false);
				pDevice->SetTextureOption(0, eAnmTextureAlphaOp, eAnmTextureModeOFF);
			}

			m_skyBox[i]->Render(pDevice, 0);
		}
	}

	// Restore world matrix
	pDevice->SetWorldMatrix(savedmat);
}

// Accessors

void CAnmBackground::SetGroundAngle(FloatArray *pGroundAngle)
{
	assert(pGroundAngle != NULL);

	SafeUnRef(m_groundAngle);
	m_groundAngle = pGroundAngle;
	m_groundAngle->Ref();

	CallCallbacks(CALLBACKID(CAnmBackground, groundAngle_changed), &pGroundAngle);

	SetDirtyBits(eAnmBackgroundSphereDirty);
	SetStateDirty();
}

void CAnmBackground::SetGroundColor(ColorArray *pGroundColor)
{
	assert(pGroundColor != NULL);

	SafeUnRef(m_groundColor);
	m_groundColor = pGroundColor;
	m_groundColor->Ref();

	CallCallbacks(CALLBACKID(CAnmBackground, groundColor_changed), &pGroundColor);

	SetDirtyBits(eAnmBackgroundSphereDirty);
	SetStateDirty();
}

void CAnmBackground::SetSkyAngle(FloatArray *pSkyAngle)
{
	assert(pSkyAngle != NULL);

	SafeUnRef(m_skyAngle);
	m_skyAngle = pSkyAngle;
	m_skyAngle->Ref();

	CallCallbacks(CALLBACKID(CAnmBackground, skyAngle_changed), &pSkyAngle);

	SetDirtyBits(eAnmBackgroundSphereDirty);
	SetStateDirty();
}

void CAnmBackground::SetSkyColor(ColorArray *pSkyColor)
{
	assert(pSkyColor != NULL);

	SafeUnRef(m_skyColor);
	m_skyColor = pSkyColor;
	m_skyColor->Ref();

	CallCallbacks(CALLBACKID(CAnmBackground, skyColor_changed), &pSkyColor);

	SetDirtyBits(eAnmBackgroundSphereDirty);
	SetStateDirty();
}

void CAnmBackground::SetBackUrl(StringArray *pBackUrl)
{
	assert(pBackUrl != NULL);

	SafeUnRef(m_backUrl);
	m_backUrl = pBackUrl;
	m_backUrl->Ref();

	CallCallbacks(CALLBACKID(CAnmBackground, backUrl_changed), &pBackUrl);

	SetDirtyBits(eAnmBackgroundBoxDirty);
	SetStateDirty();
}

void CAnmBackground::SetBottomUrl(StringArray *pBottomUrl)
{
	assert(pBottomUrl != NULL);

	SafeUnRef(m_bottomUrl);
	m_bottomUrl = pBottomUrl;
	m_bottomUrl->Ref();

	CallCallbacks(CALLBACKID(CAnmBackground, backUrl_changed), &pBottomUrl);

	SetDirtyBits(eAnmBackgroundBoxDirty);
	SetStateDirty();
}

void CAnmBackground::SetFrontUrl(StringArray *pFrontUrl)
{
	assert(pFrontUrl != NULL);

	SafeUnRef(m_frontUrl);
	m_frontUrl = pFrontUrl;
	m_frontUrl->Ref();

	CallCallbacks(CALLBACKID(CAnmBackground, backUrl_changed), &pFrontUrl);

	SetDirtyBits(eAnmBackgroundBoxDirty);
	SetStateDirty();
}

void CAnmBackground::SetLeftUrl(StringArray *pLeftUrl)
{
	assert(pLeftUrl != NULL);

	SafeUnRef(m_leftUrl);
	m_leftUrl = pLeftUrl;
	m_leftUrl->Ref();

	CallCallbacks(CALLBACKID(CAnmBackground, backUrl_changed), &pLeftUrl);

	SetDirtyBits(eAnmBackgroundBoxDirty);
	SetStateDirty();
}

void CAnmBackground::SetRightUrl(StringArray *pRightUrl)
{
	assert(pRightUrl != NULL);

	SafeUnRef(m_rightUrl);
	m_rightUrl = pRightUrl;
	m_rightUrl->Ref();

	CallCallbacks(CALLBACKID(CAnmBackground, backUrl_changed), &pRightUrl);

	SetDirtyBits(eAnmBackgroundBoxDirty);
	SetStateDirty();
}

void CAnmBackground::SetTopUrl(StringArray *pTopUrl)
{
	assert(pTopUrl != NULL);

	SafeUnRef(m_topUrl);
	m_topUrl = pTopUrl;
	m_topUrl->Ref();

	CallCallbacks(CALLBACKID(CAnmBackground, backUrl_changed), &pTopUrl);

	SetDirtyBits(eAnmBackgroundBoxDirty);
	SetStateDirty();
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmBackground, groundAngle, eValueFloatArray, FloatArray*, GetGroundAngle, SetGroundAngle);
DEFINE_VALUE(CAnmBackground, groundColor, eValueColorArray, ColorArray*, GetGroundColor, SetGroundColor);
DEFINE_VALUE(CAnmBackground, backUrl, eValueStringArray, StringArray*, GetBackUrl, SetBackUrl);
DEFINE_VALUE(CAnmBackground, bottomUrl, eValueStringArray, StringArray*, GetBottomUrl, SetBottomUrl);
DEFINE_VALUE(CAnmBackground, frontUrl, eValueStringArray, StringArray*, GetFrontUrl, SetFrontUrl);
DEFINE_VALUE(CAnmBackground, leftUrl, eValueStringArray, StringArray*, GetLeftUrl, SetLeftUrl);
DEFINE_VALUE(CAnmBackground, rightUrl, eValueStringArray, StringArray*, GetRightUrl, SetRightUrl);
DEFINE_VALUE(CAnmBackground, topUrl, eValueStringArray, StringArray*, GetTopUrl, SetTopUrl);
DEFINE_VALUE(CAnmBackground, skyAngle, eValueFloatArray, FloatArray*, GetSkyAngle, SetSkyAngle);
DEFINE_VALUE(CAnmBackground, skyColor, eValueColorArray, ColorArray*, GetSkyColor, SetSkyColor);

DEFINE_INHERITED_METHOD(CAnmBackground, set_bind, CAnmBindableNode);
DEFINE_METHOD(CAnmBackground, set_groundAngle, eValueFloatArray, FloatArray*, SetGroundAngle);
DEFINE_METHOD(CAnmBackground, set_groundColor, eValueColorArray, ColorArray*, SetGroundColor);
DEFINE_METHOD(CAnmBackground, set_backUrl, eValueStringArray, StringArray*, SetBackUrl);
DEFINE_METHOD(CAnmBackground, set_bottomUrl, eValueStringArray, StringArray*, SetBottomUrl);
DEFINE_METHOD(CAnmBackground, set_frontUrl, eValueStringArray, StringArray*, SetFrontUrl);
DEFINE_METHOD(CAnmBackground, set_leftUrl, eValueStringArray, StringArray*, SetLeftUrl);
DEFINE_METHOD(CAnmBackground, set_rightUrl, eValueStringArray, StringArray*, SetRightUrl);
DEFINE_METHOD(CAnmBackground, set_topUrl, eValueStringArray, StringArray*, SetTopUrl);
DEFINE_METHOD(CAnmBackground, set_skyAngle, eValueFloatArray, FloatArray*, SetSkyAngle);
DEFINE_METHOD(CAnmBackground, set_skyColor, eValueColorArray, ColorArray*, SetSkyColor);

DEFINE_INHERITED_CALLBACK(CAnmBackground, isBound, CAnmBindableNode);
DEFINE_CALLBACK(CAnmBackground, groundAngle_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmBackground, groundColor_changed, eValueColorArray);
DEFINE_CALLBACK(CAnmBackground, backUrl_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmBackground, bottomUrl_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmBackground, frontUrl_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmBackground, leftUrl_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmBackground, topUrl_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmBackground, rightUrl_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmBackground, skyAngle_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmBackground, skyColor_changed, eValueColorArray);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmBackground)
VALUEID(CAnmBackground, groundAngle),
VALUEID(CAnmBackground, groundColor),
VALUEID(CAnmBackground, backUrl),
VALUEID(CAnmBackground, bottomUrl),
VALUEID(CAnmBackground, frontUrl),
VALUEID(CAnmBackground, leftUrl),
VALUEID(CAnmBackground, rightUrl),
VALUEID(CAnmBackground, topUrl),
VALUEID(CAnmBackground, skyAngle),
VALUEID(CAnmBackground, skyColor),

METHODID(CAnmBackground, set_bind),
METHODID(CAnmBackground, set_groundAngle),
METHODID(CAnmBackground, set_groundColor),
METHODID(CAnmBackground, set_backUrl),
METHODID(CAnmBackground, set_bottomUrl),
METHODID(CAnmBackground, set_frontUrl),
METHODID(CAnmBackground, set_leftUrl),
METHODID(CAnmBackground, set_rightUrl),
METHODID(CAnmBackground, set_topUrl),
METHODID(CAnmBackground, set_skyAngle),
METHODID(CAnmBackground, set_skyColor),

CALLBACKID(CAnmBackground, groundAngle_changed),
CALLBACKID(CAnmBackground, groundColor_changed),
CALLBACKID(CAnmBackground, backUrl_changed),
CALLBACKID(CAnmBackground, bottomUrl_changed),
CALLBACKID(CAnmBackground, frontUrl_changed),
CALLBACKID(CAnmBackground, leftUrl_changed),
CALLBACKID(CAnmBackground, rightUrl_changed),
CALLBACKID(CAnmBackground, topUrl_changed),
CALLBACKID(CAnmBackground, skyAngle_changed),
CALLBACKID(CAnmBackground, skyColor_changed),
CALLBACKID(CAnmBackground, isBound),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmBackground, CAnmBindableNode);

