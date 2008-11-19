/********************************************************************************
 * Flux
 *
 * File: anmanchor.cpp
 * Description: WWW Anchor node class
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

#include "anmanchor.h"
#include "anmtouchsensor.h"
#include "anmfilemanager.h"
#include "anmscene.h"
#include "anmviewpoint.h"
#include "anmworld.h"

CAnmAnchor::CAnmAnchor() : CAnmGroup (), CAnmUrlObject()
{
	m_description = new CAnmString(ANMANCHOR_DEFAULT_DESCRIPTION);
	m_parameter = new StringArray;

	m_anchorSensor = new CAnmTouchSensor;
	m_anchorSensor->SetOwner(this);

	// m_anchorSensor->AddCallback(CALLBACKID(CAnmTouchSensor, touchTime), AnchorClickCallback, this);
	m_anchorSensor->AddCallback(CALLBACKID(CAnmPickSensor, isOver), AnchorOverCallback, this);
	m_anchorSensor->AddCallback(CALLBACKID(CAnmPickSensor, isActive), AnchorActiveCallback, this);

	m_anchorSensor->Ref();

	m_groupPicking = TRUE;	// turn on sensor-less picking for pick manager

}

CAnmAnchor::~CAnmAnchor()
{
	SafeUnRef(m_anchorSensor);
	SafeUnRef(m_description);
	SafeUnRef(m_parameter);
}

void CAnmAnchor::Realize()
{
	// create the frame, and chain the realize down to the children
	CAnmGroup::Realize();

	// realize my weird touchsensor
	m_anchorSensor->Realize();
}

void CAnmAnchor::Traverse(CAnmUpdateVisitor *pVisitor)
{
	CAnmGroup::Traverse(pVisitor);

	m_anchorSensor->Traverse(pVisitor);
}

void CAnmAnchor::PreTraverse()
{
	CAnmGroup::PreTraverse();

	m_anchorSensor->PreTraverse();
}

void CAnmAnchor::PostTraverse()
{
	CAnmGroup::PostTraverse();

	m_anchorSensor->PostTraverse();
}

void CAnmAnchor::HandleClick(Time t)
{
	int foo = 1;
}

void CAnmAnchor::HandleOver(Boolean isOver)
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL
	cApplication *pApp = pWorld->GetApp();

	if (isOver)
	{
		const char *descrip = m_description->GetBuf();

		if (strlen(descrip))
		{
			pApp->SetStatusText((char *) descrip);
		}
		else if (m_url->size() > 0)
		{
			const char *url = (*m_url)[0]->GetBuf();
			pApp->SetStatusText((char *) url);
		}
		else
			pApp->SetStatusText("");		
	}
	else
	{
		pApp->SetStatusText("");		
	}

}

void CAnmAnchor::HandleActive(Boolean isActive)
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL
	cApplication *pApp = pWorld->GetApp();

	if (!isActive && (m_url->size() > 0))
	{
		char *vpname;

		if (vpname = GetViewpointAnchor((*m_url)[0]))
			BindToViewpoint(vpname);
		else
			pApp->GetFileManager()->RequestLinkToUrl(m_url, m_baseUrl->GetBuf(), m_parameter);
	}
}

void CAnmAnchor::AnchorClickCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData)
{
	CAnmTouchSensor *pTS = (CAnmTouchSensor *) sourceObject;
	CAnmAnchor *pAnchor = (CAnmAnchor *) userData;
	Time *pTime =  (Time *) callData;

	pAnchor->HandleClick(*pTime);
}

void CAnmAnchor::AnchorOverCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData)
{
	CAnmTouchSensor *pTS = (CAnmTouchSensor *) sourceObject;
	CAnmAnchor *pAnchor = (CAnmAnchor *) userData;
	Boolean *pOver =  (Boolean *) callData;

	pAnchor->HandleOver(*pOver);
}

void CAnmAnchor::AnchorActiveCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData)
{
	CAnmTouchSensor *pTS = (CAnmTouchSensor *) sourceObject;
	CAnmAnchor *pAnchor = (CAnmAnchor *) userData;
	Boolean *pActive =  (Boolean *) callData;

	pAnchor->HandleActive(*pActive);
}

class CAnmAsset *CAnmAnchor::handleUrlLoaded(String pRequestUrl, String pCacheFileName)
{
	// we should never be here; just need to supply this to satisfy PURE decl in base CAnmUrlObject class
	assert(FALSE);

	return NULL;
}

void CAnmAnchor::handleUrlLoaded(String pRequestUrl, CAnmAsset *pAsset)
{
	// we should never be here; just need to supply this to satisfy PURE decl in base CAnmUrlObject class
	assert(FALSE);
}

// Accessors

void CAnmAnchor::SetParameter(StringArray *pParameter)
{
	assert(pParameter != NULL);

	SafeUnRef(m_parameter);
	m_parameter = pParameter;
	m_parameter->Ref();

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmAnchor, parameter_changed), &pParameter);
}

void CAnmAnchor::SetUrl(StringArray *pUrl)
{
	CAnmUrlObject::SetUrl(pUrl);

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmAnchor, url_changed), &pUrl);
}

void CAnmAnchor::SetDescription(String description)
{
	SafeUnRef(m_description);
	m_description = description;
	m_description->Ref();

	SetStateDirty();
}

void CAnmAnchor::Update()
{
	m_anchorSensor->Update();

	ClearStateDirty();
}

void CAnmAnchor::BindToViewpoint(char *viewpointName)
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	CAnmScene *pScene = pWorld->GetScene();
	assert(pScene);

	CAnmNode *pNode = pScene->GetNode(viewpointName);
	if (pNode)
	{
		CAnmViewpoint *pViewpoint = GETIMPLEMENTATION(CAnmViewpoint, pNode);
		if (pViewpoint)
			pViewpoint->SetBind(true);
	}
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmAnchor, bboxCenter, CAnmGroup);
DEFINE_INHERITED_VALUE(CAnmAnchor, bboxSize, CAnmGroup);
DEFINE_VALUE(CAnmAnchor, children, eValueNodeArray, NodeArray*, GetChildren, SetChildren);
DEFINE_VALUE(CAnmAnchor, description, eValueString, String, GetDescription, SetDescription);
DEFINE_VALUE(CAnmAnchor, parameter, eValueStringArray, StringArray *, GetParameter, SetParameter);
DEFINE_VALUE(CAnmAnchor, url, eValueStringArray, StringArray *, GetUrl, SetUrl);


DEFINE_INHERITED_METHOD(CAnmAnchor, addChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmAnchor, removeChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmAnchor, set_bboxCenter, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmAnchor, set_bboxSize, CAnmGroup);
DEFINE_METHOD(CAnmAnchor, set_children, eValueNodeArray, NodeArray*, SetChildren);
DEFINE_METHOD(CAnmAnchor, set_description, eValueString, String, SetDescription);
DEFINE_METHOD(CAnmAnchor, set_parameter, eValueStringArray, StringArray *, SetParameter);
DEFINE_METHOD(CAnmAnchor, set_url, eValueStringArray, StringArray *, SetUrl);

DEFINE_INHERITED_CALLBACK(CAnmAnchor, bboxCenter_changed, CAnmGroup);
DEFINE_INHERITED_CALLBACK(CAnmAnchor, bboxSize_changed, CAnmGroup);
DEFINE_CALLBACK(CAnmAnchor, children_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmAnchor, description_changed, eValueString);
DEFINE_CALLBACK(CAnmAnchor, parameter_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmAnchor, url_changed, eValueStringArray);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmAnchor)
VALUEID(CAnmAnchor, bboxCenter),
VALUEID(CAnmAnchor, bboxSize),
VALUEID(CAnmAnchor, children),
VALUEID(CAnmAnchor, description),
VALUEID(CAnmAnchor, parameter),
VALUEID(CAnmAnchor, url),

METHODID(CAnmAnchor, addChildren),
METHODID(CAnmAnchor, removeChildren),
METHODID(CAnmAnchor, set_bboxCenter),
METHODID(CAnmAnchor, set_bboxSize),
METHODID(CAnmAnchor, set_children),
METHODID(CAnmAnchor, set_description),
METHODID(CAnmAnchor, set_parameter),
METHODID(CAnmAnchor, set_url),

CALLBACKID(CAnmAnchor, bboxCenter_changed),
CALLBACKID(CAnmAnchor, bboxSize_changed),
CALLBACKID(CAnmAnchor, children_changed),
CALLBACKID(CAnmAnchor, description_changed),
CALLBACKID(CAnmAnchor, parameter_changed),
CALLBACKID(CAnmAnchor, url_changed),




END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmAnchor, CAnmGroup);

