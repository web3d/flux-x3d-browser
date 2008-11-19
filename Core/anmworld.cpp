/********************************************************************************
 * Flux
 *
 * File: anmworld.cpp
 * Description: World object
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
#include "anmworld.h"
#include "anmbackground.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmfog.h"
#include "anmlayer.h"
#include "anmmainlayer.h"
#include "anmnavigationinfo.h"
#include "anmpickmanager.h"
#include "anmpicksensor.h"
#include "anmproto.h"
#include "anmroute.h"
#include "anmscene.h"
#include "anmshape.h"
#include "anmsymbol.h"
#include "anmtransform.h"
#include "anmviewport.h"
#include "anmviewpoint.h"
#include "anmviewer.h"
#include "anmvisitor.h"
#include "anmupdatevisitor.h"
#include "anmworld.h"
#include "anmMatrixInvert.h"

CAnmWorld::CAnmWorld(cApplication *pApp, CAnmScene *pScene) :
	m_app(pApp),
	m_scene(pScene)
{
	CAnmLayer *pAppLayer = pApp->CreateAppLayer();
	if (pAppLayer)
	{
		m_applayer = pAppLayer;
		AddChild(m_applayer);
		AddLayer(m_applayer);
	}

	m_mainlayer = new CAnmMainLayer(pApp);
	
	AddChild(m_mainlayer);

	// Clear out proto's child list; I'll take it
	NodeArray *pChildren = NULL;
	m_scene->GetChildren(&pChildren);
	m_mainlayer->AddChildren(pChildren);

	m_scene->RemoveAllChildren();

	m_scene->SetContainingGroup(m_mainlayer);

	m_bgColor = m_app->GetBackgroundColor();
	m_polycount = 0L;

	m_nextlightindex = 0;

	CAnmDevice *pDevice = m_app->GetDevice();
	assert(pDevice);

	CAnmViewport *pViewport = pDevice->GetViewport();
	assert(pViewport);

	m_nextRenderGraphIndex = 0;

	m_seekMode = false;
}
 
CAnmWorld::~CAnmWorld()
{
	int sz = m_renderGraphs.size();

	for (int i = 0; i < sz; i++)
		SafeDelete(m_renderGraphs[i]);

	sz = m_backgroundlayers.size();
	for (i = 0; i < sz; i++)
		m_backgroundlayers[i]->FreeResources();

	m_mainlayer->FreeResources();

	sz = m_foregroundlayers.size();
	for (i = 0; i < sz; i++)
		m_foregroundlayers[i]->FreeResources();

	//krv:  Moved this from the CanmApp destructor, so that the thread can be shut down
	// when we do a restart.
	// Free external resources (like MovieTexture threads, deadly otherwise)
	m_app->FreeExternalResources();

 
	SafeUnRef(m_mainlayer); 
	SafeUnRef(m_scene);

/**************
************/
}

CAnmProto *CAnmWorld::GetProtoContext()
{
	return m_scene;
}

void CAnmWorld::Realize()
{
	// chain the realize down to the children
	CAnmGroup::Realize();

	Update();

	SetDirtyBits(eAnmNodeChildrenDirty | eAnmNodeMatrixDirty);
}

void CAnmWorld::Update()
{
	// update fields like background color etc.
	m_polycount = GetPolyCount();

	ClearStateDirty();
}

static double GetFrameRate( float timeDelta )
{
	//if( timeDelta < 1/100.f )
	//	timeDelta = 1/100.f;

	double fps = (int)(1.f/timeDelta);

	return fps;
}

void CAnmWorld::Traverse(CAnmUpdateVisitor *pVisitor)
{
	if (!m_realized)
		return;

	// make sure all the layers' bindable stuff is updated
	int sz = m_backgroundlayers.size();
	for (int i = 0; i < sz; i++)
		m_backgroundlayers[i]->Traverse(pVisitor);

	m_mainlayer->Traverse(pVisitor);

	sz = m_foregroundlayers.size();
	for (i = 0; i < sz; i++)
		m_foregroundlayers[i]->Traverse(pVisitor);

	// Set up pick data for next frame. Go from front to back
	bool somethingpicked = false;
	for (i = sz - 1; i >= 0; --i)
	{
		if (m_foregroundlayers[i]->TryPick())
		{
			somethingpicked = true;
			break;
		}
	}

	if (!somethingpicked)
	{
		if (!m_mainlayer->TryPick())
		{
			sz = m_backgroundlayers.size();
			for (i = sz - 1; i >= 0; --i)
			{
				if (m_backgroundlayers[i]->TryPick())
				{
					somethingpicked = true;
					break;
				}
			}
		}
	}
}

void CAnmWorld::Render(CAnmDevice *pDevice)
{
	assert(pDevice);

	// N.B.: WM_MOVE isn't happening when the parent window moves; so we need to update the device's
	// rectangles every render tick
	pDevice->HandleMove();

	// Begin drawing
	pDevice->BeginScene();

	// Paint the background and clear the z and color buffers
	pDevice->Clear( true, true, m_bgColor, 1.f);

	// Render the background layers
	int nLayers = m_backgroundlayers.size();
	if( nLayers) {
		for( int iLayer = 0; iLayer < nLayers; iLayer++ ) {
			m_backgroundlayers[iLayer]->Render(pDevice);
		}
	}


	// Render the main layer
	m_mainlayer->Render(pDevice);

	// Render the foreground layers
	nLayers = m_foregroundlayers.size();
	if( nLayers) {
		for( int iLayer = 0; iLayer < nLayers; iLayer++ ) {
			m_foregroundlayers[iLayer]->Render(pDevice);
		}
	}

	// End drawing
	pDevice->EndScene();
}

void CAnmWorld::PreTraverse()
{
	// If anything's dirty, clear out my global light instances; the lights will re-add
	// as we traverse
	if (TestDirtyBits(eAnmNodeChildrenDirty))
	{
		CAnmLightGroup *pLightGroup = m_renderGraphs[0]->GetGlobalLightGroup();
		pLightGroup->ClearLights();
	}
	
	CAnmGroup::PreTraverse();
}

CAnmLayer *CAnmWorld::GetLayer()
{
	return m_mainlayer;
}

void CAnmWorld::AddViewpoint(CAnmViewpoint *pViewpoint)
{
	Lock();
	m_mainlayer->AddViewpoint(pViewpoint);
	Unlock();
}

std::list<class CAnmViewpoint *> *CAnmWorld::GetViewpoints()
{
	Lock();
	std::list<class CAnmViewpoint *> *pVpList =
		m_mainlayer->GetViewpoints();
	Unlock();

	return pVpList;
}

CAnmCamera *CAnmWorld::GetCamera()
{
	return m_mainlayer->GetCamera();
}

CAnmBackground *CAnmWorld::GetBackground()
{
	return m_mainlayer->GetBackground();
}

CAnmFog *CAnmWorld::GetFog()
{
	return m_mainlayer->GetFog();
}

CAnmNavigationInfo *CAnmWorld::GetNavigationInfo()
{
	return m_mainlayer->GetNavigationInfo();
}

CAnmViewpoint *CAnmWorld::GetViewpoint()
{
	return m_mainlayer->GetViewpoint();
}

void CAnmWorld::FirstViewpoint()
{
	m_mainlayer->FirstViewpoint();
}

void CAnmWorld::LastViewpoint()
{
	m_mainlayer->LastViewpoint();
}

void CAnmWorld::NextViewpoint()
{
	m_mainlayer->NextViewpoint();
}

void CAnmWorld::PreviousViewpoint()
{
	m_mainlayer->PreviousViewpoint();
}

CAnmPickManager *CAnmWorld::GetPickManager()
{
	return m_mainlayer->GetPickManager();
}

CAnmDevice *CAnmWorld::GetDevice()
{
	return m_app->GetDevice();
}

void CAnmWorld::BindToViewpoint(CAnmViewpoint *pViewpoint)
{
	m_mainlayer->BindToViewpoint(pViewpoint);
}



void CAnmWorld::SetBackgroundColor(ulong bgcolor)
{
	m_bgColor = bgcolor;
	Update();
}

void CAnmWorld::GetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	// We could recursively traverse the whole scene... but why?
	m_renderGraphs[0]->GetBoundingBox(&m_bbox);
	*pBoundingBox = m_bbox;
}

float CAnmWorld::GetRadius()
{
	// We could recursively traverse the whole scene... but why?
	m_renderGraphs[0]->GetBoundingBox(&m_bbox);
	return m_bbox.GetRadius();
}

matrix4 *CAnmWorld::GetMatrix()
{
	assert(m_mainlayer);

	return m_mainlayer->GetMatrix();
}

void CAnmWorld::GetCenter(Point3 *pp)
{
	assert(m_mainlayer);

	m_mainlayer->GetCenter(pp);
}

void CAnmWorld::GetObjectCenter(Point3 *pCenter)
{
	// We could recursively traverse the whole scene... but why?
	m_renderGraphs[0]->GetBoundingBox(&m_bbox);
	Point3 size;
	m_bbox.GetCenterSize(pCenter, &size);
}

bool CAnmWorld::TestCollision(Point3 oldpos, Point3 newpos, float radius, Point3 &hitPoint, Point3 &hitNormal)
{
	return m_mainlayer->TestCollision(m_app->GetDevice(), oldpos, newpos, radius, hitPoint, hitNormal);
}

bool CAnmWorld::TestPick(int mousex, int mousey, Point3 &hitPoint, CAnmDrawable **ppDrawable)
{
	return m_mainlayer->TestPick(m_app->GetDevice(), mousex, mousey, hitPoint, ppDrawable);
}

bool CAnmWorld::TestTerrain(Point3 oldpos, Point3 newpos, float radius, Point3 &hitPoint)
{
	return m_mainlayer->TestTerrain(m_app->GetDevice(), oldpos, newpos, radius, hitPoint);
}

int CAnmWorld::AllocLight()
{
	// N.B.: we should have a reclaim list so that we can re-use light indices
	return m_nextlightindex++;
}

void CAnmWorld::FreeLight(int index)
{
	// N.B.: we should have a reclaim list so that we can re-use light indices
}

int CAnmWorld::AllocRenderGraph()
{
	// N.B.: we should have a reclaim list so that we can re-use light indices
	m_renderGraphs.push_back(new CAnmRenderGraph());
	m_renderGraphs[m_nextRenderGraphIndex]->SetUserData(this);

	return m_nextRenderGraphIndex++;
}

void CAnmWorld::AddLayer(class CAnmLayer *pLayer)
{
	assert(pLayer);

	// Don't add to our list if it's the main layer; we've got a pointer already
	if (pLayer->IsMainLayer())
		return;

	eAnmLayerType layertype = pLayer->GetLayerType();

	if (layertype == eAnmLayerForeground)
		m_foregroundlayers.push_back( pLayer);
	else if (layertype == eAnmLayerBackground)
		m_backgroundlayers.push_back( pLayer);
	else
		; // what to do?
}
