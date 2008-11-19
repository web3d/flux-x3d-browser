/********************************************************************************
 * Flux
 *
 * File: anmlayer.cpp
 * Description: Layer node - base class for layers
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

#include "anmlayer.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmbackground.h"
#include "anmfog.h"
#include "anmnavigationinfo.h"
#include "anmupdatevisitor.h"
#include "anmviewpoint.h"
#include "anmviewport.h"
#include "anmworld.h"
#include "anmMatrixInvert.h"

CAnmLayer::CAnmLayer() : CAnmTransform(),
	m_renderGraphIndex( -1 )
{
	m_camera = NULL;
	m_transparency = ANMLAYER_DEFAULT_TRANSPARENCY;
	m_type = new CAnmString(ANMLAYER_DEFAULT_TYPE);
	m_layerType = ANMLAYER_DEFAULT_LAYERTYPE;
	m_background = NULL;
	m_fog = NULL;
	m_navigationInfo = NULL;
	m_viewpoint = NULL;
}

CAnmLayer::~CAnmLayer()
{
	SafeDelete(m_camera);
	SafeUnRef(m_type);

	std::list<CAnmViewpoint *>::iterator iter;
	for (iter = m_viewpoints.begin(); ( iter != m_viewpoints.end() ); iter++) {
		SafeUnRef(*iter);
	}


	FreeResources();
}

void CAnmLayer::FreeResources()
{
	SafeDelete(m_pickmanager);
}

void CAnmLayer::Realize()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	// make a render graph
	m_renderGraphIndex = pWorld->AllocRenderGraph();
	m_renderGraph = pWorld->GetRenderGraph(m_renderGraphIndex);

	m_camera = new CAnmCamera(pApp, pWorld->AllocLight(), true );
	m_pickmanager = new CAnmPickManager(pApp);

	m_layerType = CalcLayerType();

	// get all the children realized
	if (pApp->GetOptions(eAnmOptionsCoordinateSystem) == eAnmCoordinatesRightHanded)
		SetScale(Point3(1, 1, -1));
	
	CAnmTransform::Realize();

	// deal with dirty states
	Update();

	SetDirtyBits(eAnmNodeChildrenDirty | eAnmNodeMatrixDirty);

	// add me to the world
	pWorld->AddLayer(this);

}

eAnmLayerType CAnmLayer::CalcLayerType()
{
	if (!stricmp(m_type->GetBuf(), "BACKGROUND"))
		return eAnmLayerBackground;
	else if (!stricmp(m_type->GetBuf(), "FOREGROUND"))
		return eAnmLayerForeground;
	else
		return ANMLAYER_DEFAULT_LAYERTYPE;
}

void CAnmLayer::Render(CAnmDevice *pDevice)
{
	// Paint the background first
	if (m_background)
	{
		m_background->Render(pDevice);
	}

	// Set up fog state
	if (m_fog)
	{
		if (m_fog->FogEnabled())
			m_renderGraph->SetFog(m_fog->GetRawFog());
		else
			m_renderGraph->SetFog(NULL);
	}
	else
	{
		m_renderGraph->SetFog(NULL);
	}
	
	// Clear out lighting
	// N.B.: Do this BEFORE you set up the camera, else it will
	// blow the lighting away
	pDevice->ClearLights();

	// Set up camera values 
	m_camera->SetToDevice(pDevice);

	// N.B.: Always z-buffer?
	pDevice->SetZBuffering(true);

	// Clear z-buffer for each layer
	pDevice->Clear( false, true, 0, 1.f);

	// Render the thing
	m_renderGraph->Render(pDevice, m_camera);
}

void CAnmLayer::Traverse(CAnmUpdateVisitor *pVisitor)
{
	if (!m_realized)
		return;

	// Pre-traversal initialization (clear ephemeral instance data, etc.)
	PreTraverse();

	// Use our own visitor (ignore passed-in one)
	CAnmUpdateVisitor layerVisitor;
	
	layerVisitor.Init(m_renderGraph, m_camera);

	// Set up collision (ON by default unless we have some UI to toggle it someday)
	if (IsMainLayer())
		layerVisitor.SetCollide(true);
	else
		layerVisitor.SetCollide(false);

	// Traverse it
	CAnmTransform::Traverse(&layerVisitor);

	// Clean up (clear dirty flags, etc)
	PostTraverse();

	// Get our bindables up to date
	UpdateBindables();

	// Get view up to date
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL


	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	RecalculateCameraParams(pDevice);

	InitPick();
}


void CAnmLayer::PreTraverse()
{
	if (!m_realized)
		return;

#if 0
	// If anything's dirty, clear out my global light instances; the lights will re-add
	// as we traverse
	if (TestDirtyBits(eAnmNodeChildrenDirty))
	{
		CAnmLightGroup *pLightGroup = m_renderGraph->GetGlobalLightGroup();
		pLightGroup->ClearLights();
	}
#endif
	
	CAnmGroup::PreTraverse();
}

void CAnmLayer::InitPick()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	int mousex = pApp->MouseX();
	int mousey = pApp->MouseY();
	Point3 pickfrom, pickto;
	m_camera->GetPickRay(pDevice, mousex, mousey, &pickfrom, &pickto);
	m_pickmanager->Init(mousex, mousey, pickfrom, pickto);
}

bool CAnmLayer::TryPick()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	int mousex = pApp->MouseX();
	int mousey = pApp->MouseY();
	Point3 pickfrom, pickto;
	m_camera->GetPickRay(pDevice, mousex, mousey, &pickfrom, &pickto);

	sAnmRenderGraphPickList renderpicklist;
	m_renderGraph->TestPick(pDevice, m_camera, pickfrom, pickto, eAnmPickSortZ, false, &renderpicklist);

	sAnmSceneGraphPickList *pScenePickList = 
		m_pickmanager->GetPickList();

	int sz = renderpicklist.pickList.size();
	for (int i = 0; i < sz; i++)
	{
		sAnmSceneGraphPickDesc scenePickDesc;

		sAnmPickDesc<CAnmDrawable, CAnmRenderGroup> renderPickDesc  = renderpicklist.pickList[i];
		scenePickDesc.hitNormal = renderPickDesc.hitNormal;
		scenePickDesc.hitPoint = renderPickDesc.hitPoint;
		scenePickDesc.hitUV = renderPickDesc.hitUV;
		
		CAnmNode *pNode = (CAnmNode *) renderPickDesc.pLeaf->GetUserData();
		assert(pNode);
		
		scenePickDesc.pLeaf = pNode;
		pNode->Ref();

		scenePickDesc.pParentArray = new CAnmGroup *[renderPickDesc.nParents - 1];

		scenePickDesc.nParents = renderPickDesc.nParents - 1;
		for (int j = 0; j < renderPickDesc.nParents - 1; j++)
		{
			// Hack! ignore the first one
			CAnmGroup *pGroup = (CAnmGroup *) renderPickDesc.pParentArray[j+1]->GetUserData();
			assert(pGroup);

			scenePickDesc.pParentArray[j] = pGroup;
			scenePickDesc.pParentArray[j]->Ref(); // ref parents since they're pointed to
		}

		pScenePickList->pickList.push_back(scenePickDesc);
	}

	renderpicklist.Clear();

	return sz > 0;
}


bool CAnmLayer::TestCollision(CAnmDevice *pDevice, Point3 oldpos, Point3 newpos, float radius, Point3 &hitPoint, Point3 &hitNormal)
{
	// This capsule stuff ain't working yet
	sAnmRenderGraphPickList renderpicklist;
	m_renderGraph->CapsuleCollide(pDevice, m_camera, oldpos, newpos, radius, eAnmPickSortZ, true, &renderpicklist);

	int sz = renderpicklist.pickList.size();
	if (sz > 0) {
		hitPoint = renderpicklist.pickList[0].hitPoint;
		hitNormal = renderpicklist.pickList[0].hitNormal;
		return true;
	}
	else
		return false;

	return (sz > 0);
}

bool CAnmLayer::TestPick(CAnmDevice *pDevice, int mousex, int mousey, Point3 &hitPoint, CAnmDrawable **ppDrawable)
{
	Point3 pickfrom, pickto;
	m_camera->GetPickRay(pDevice, mousex, mousey, &pickfrom, &pickto);

	sAnmRenderGraphPickList renderpicklist;
	m_renderGraph->TestPick(pDevice, m_camera, pickfrom, pickto, eAnmPickSortZ, false, &renderpicklist);

	int sz = renderpicklist.pickList.size();
	if (sz > 0) {
		hitPoint = renderpicklist.pickList[0].hitPoint;
		*ppDrawable = renderpicklist.pickList[0].pLeaf;
		renderpicklist.Clear();
		return true;
	}
	else
	{
		*ppDrawable = NULL;
		return false;
	}

	return (sz > 0);
}

bool CAnmLayer::TestTerrain(CAnmDevice *pDevice, Point3 oldpos, Point3 newpos, float radius, Point3 &hitPoint)
{
	// This capsule stuff ain't working yet
	sAnmRenderGraphPickList renderpicklist;
	m_renderGraph->CapsuleCollide(pDevice, m_camera, oldpos, newpos, radius, eAnmPickSortY, true, &renderpicklist);

	int sz = renderpicklist.pickList.size();
	if (sz > 0) {
		hitPoint = renderpicklist.pickList[0].hitPoint;
		return true;
	}
	else
		return false;

	return (sz > 0);

}

// Binding stack support
void CAnmLayer::BindNode(CAnmBindableNode *pBindableNode, CAnmClass *pClass)
{
	assert(pBindableNode);

	CAnmBindingStack *pBindingStack = FindBindingStack(pClass);
	
	if (pBindingStack == NULL)
	{
		pBindingStack = AddBindingStack(pClass);
		assert(pBindingStack);
	}

	pBindingStack->BindNode(pBindableNode);

	UpdateBoundNode(pClass);
}

void CAnmLayer::TryUnbindNode(CAnmBindableNode *pBindableNode, CAnmClass *pClass)
{
	assert(pBindableNode);

	CAnmBindingStack *pBindingStack = FindBindingStack(pClass);
	
	if (pBindingStack)
		pBindingStack->TryUnbindNode(pBindableNode);

	UpdateBoundNode(pClass);
}

CAnmBindableNode *CAnmLayer::GetBoundNode(CAnmClass *pClass)
{
	CAnmBindingStack *pBindingStack = FindBindingStack(pClass);

	if (pBindingStack == NULL)
	{
		pBindingStack = AddBindingStack(pClass);
		assert(pBindingStack);
	}

	return pBindingStack->GetBoundNode();
}

void CAnmLayer::UpdateBoundNode(CAnmClass *pClass)
{
	if (pClass == GETCLASS(CAnmBackground))
		SetBackground(GetBoundBackground());
	else if (pClass == GETCLASS(CAnmFog))
		SetFog(GetBoundFog());
	else if (pClass == GETCLASS(CAnmNavigationInfo))
		SetNavigationInfo(GetBoundNavigationInfo());
	else if (pClass == GETCLASS(CAnmViewpoint))
		SetViewpoint(GetBoundViewpoint());
}

CAnmBindingStack *CAnmLayer::FindBindingStack(CAnmClass *pClass)
{
	int sz = m_bindingstacks.size();
	
	for (int i = 0; i < sz; i++)
		if (m_bindingstacks[i].GetClass() == pClass)
			return &m_bindingstacks[i];

	return NULL;
}

CAnmBindingStack *CAnmLayer::AddBindingStack(CAnmClass *pClass)
{
	CAnmBindingStack *pBindingStack;
	if ((pBindingStack = FindBindingStack(pClass)))
		return pBindingStack;

	CAnmBindingStack bstk(pClass);
	m_bindingstacks.push_back(bstk);

	int sz = m_bindingstacks.size();
	assert(sz > 0);

	return &m_bindingstacks[sz - 1];
}

CAnmBackground *CAnmLayer::GetBoundBackground()
{
	CAnmBindableNode *pBoundNode = GetBoundNode(GETCLASS(CAnmBackground));
	if (pBoundNode && pBoundNode->GetClass()->IsA(GETCLASS(CAnmBackground)))
	{
		return (CAnmBackground *) pBoundNode;
	}
	else
		return NULL;
}

CAnmFog *CAnmLayer::GetBoundFog()
{
	CAnmBindableNode *pBoundNode = GetBoundNode(GETCLASS(CAnmFog));
	if (pBoundNode && pBoundNode->GetClass()->IsA(GETCLASS(CAnmFog)))
	{
		return (CAnmFog *) pBoundNode;
	}
	else
		return NULL;
}

CAnmNavigationInfo *CAnmLayer::GetBoundNavigationInfo()
{
	CAnmBindableNode *pBoundNode = GetBoundNode(GETCLASS(CAnmNavigationInfo));
	if (pBoundNode && pBoundNode->GetClass()->IsA(GETCLASS(CAnmNavigationInfo)))
	{
		return (CAnmNavigationInfo *) pBoundNode;
	}
	else
		return NULL;
}

CAnmViewpoint *CAnmLayer::GetBoundViewpoint()
{
	CAnmBindableNode *pBoundNode = GetBoundNode(GETCLASS(CAnmViewpoint));
	if (pBoundNode && pBoundNode->GetClass()->IsA(GETCLASS(CAnmViewpoint)))
	{
		return (CAnmViewpoint *) pBoundNode;
	}
	else
		return NULL;
}

void CAnmLayer::BindToViewpoint(CAnmViewpoint *pViewpoint)
{
	if (pViewpoint && pViewpoint == m_viewpoint)
		return;

	pViewpoint->SetBind(TRUE);

}

void CAnmLayer::UpdateBindables()
{
	// Update current background node
	if (m_background)
		m_background->Update();

	// Update current fog node
	if (m_fog)
		m_fog->Update();

	// Make sure the camera is in the right place
	if (m_viewpoint)
	{
		m_viewpoint->Update();
	}	

	// Make sure nav info is up to date in the camera
	if (m_navigationInfo)
	{
		m_camera->SetHeadlightOn(m_navigationInfo->GetHeadlight());
	}
	else
	{
		m_camera->SetHeadlightOn(ANMNAVINFO_DEFAULT_HEADLIGHT);
	}
}

void CAnmLayer::AddViewpoint(CAnmViewpoint *pViewpoint)
{
	Lock();
	pViewpoint->Ref();
	m_viewpoints.push_back(pViewpoint);
	Unlock();
}

std::list<class CAnmViewpoint *> *CAnmLayer::GetViewpoints()
{
	Lock();
	RemoveViewpointOrphans();
	std::list<class CAnmViewpoint *> *pVpList =
		&m_viewpoints;
	Unlock();

	return pVpList;
}

// krv
// We must ensure that all the VPs in the list actually live in the layer.
// A call to RemoveChild will leave orphans in the VP list, causing problems for Dave.
// 

void CAnmLayer::RemoveViewpointOrphans()
{
	bool bFound = true;
	while( bFound ) {
		bFound = false;

		std::list<CAnmViewpoint *>::iterator iter;
		for (iter = m_viewpoints.begin(); ( iter != m_viewpoints.end() ) && !bFound; iter++) {
			CAnmViewpoint* pVP = (*iter);
			if( pVP && pVP->GetLayer() != this ) {
				pVP->UnRef();
				m_viewpoints.remove( pVP );

				// Restart the process so the Iter does not get messed up/
				// THere has got to be a better way.
				//
				bFound = true;
				break;
			}
		}
	}
}


// Accessors

void CAnmLayer::SetTransparency(Float transparency)
{
	m_transparency = transparency;
	SetStateDirty();
	CallCallbacks(CALLBACKID(CAnmLayer, transparency_changed), &transparency);
}

void CAnmLayer::SetType(String type)
{
	SafeUnRef(m_type);
	m_type = type;
	m_type->Ref();
}

void CAnmLayer::SetBackground(class CAnmBackground *pBackground)
{
	m_background = pBackground;
}

void CAnmLayer::SetFog(class CAnmFog *pFog)
{
	m_fog = pFog;
}

void CAnmLayer::SetNavigationInfo(class CAnmNavigationInfo *pNavigationInfo)
{
	m_navigationInfo = pNavigationInfo;
}

void CAnmLayer::SetViewpoint(class CAnmViewpoint *pViewpoint)
{
	m_viewpoint = pViewpoint;
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmLayer, children, CAnmGroup);

DEFINE_INHERITED_METHOD(CAnmLayer, addChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmLayer, removeChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmLayer, set_children, CAnmGroup);

DEFINE_INHERITED_CALLBACK(CAnmLayer, children_changed, CAnmGroup);

DEFINE_VALUE(CAnmLayer, transparency, eValueFloat, Float, GetTransparency, SetTransparency);
DEFINE_METHOD(CAnmLayer, set_transparency, eValueFloat, Float, SetTransparency);
DEFINE_CALLBACK(CAnmLayer, transparency_changed, eValueFloat);

DEFINE_INITVALUE(CAnmLayer, type, eValueString, String, SetType);

// member table
BEGIN_DEFINE_MEMBERS(CAnmLayer)
VALUEID(CAnmLayer, children),
METHODID(CAnmLayer, addChildren),
METHODID(CAnmLayer, removeChildren),
METHODID(CAnmLayer, set_children),
CALLBACKID(CAnmLayer, children_changed),

VALUEID(CAnmLayer, transparency),
METHODID(CAnmLayer, set_transparency),
CALLBACKID(CAnmLayer, transparency_changed),

VALUEID(CAnmLayer, type),

END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmLayer, CAnmTransform);
