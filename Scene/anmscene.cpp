/********************************************************************************
 * Flux
 *
 * File: anmscene.cpp
 * Description: Scene instance; for top-level
 *							scenes only (worlds and inlines)
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
#include "anmscene.h"
#include "anmmetadatatable.h"
#include "anmworld.h"

// constructor/destructor
CAnmScene::CAnmScene() : CAnmProto()
{
	m_worldUrl = NULL;
	m_containingGroup = NULL;
	m_metadatatable = new CAnmMetadataTable();
}

CAnmObject *CAnmScene::Create()
{
	return new CAnmScene;
}


CAnmScene::~CAnmScene()
{
	SafeDelete(m_worldUrl);
	SafeDelete(m_metadatatable);
}


NodeArray *CAnmScene::GetRootNodes()
{
	NodeArray *pRootNodes = NULL;
	if (m_containingGroup)
		m_containingGroup->GetChildren(&pRootNodes);
	else
		GetChildren(&pRootNodes);
	return pRootNodes;
}

String CAnmScene::GetMetaData(const char *key)
{
	CAnmMetadataEntry *pEntry = m_metadatatable->FindEntry(key);
	if (pEntry)
		return pEntry->GetValue();
	else
		return NULL;
}

void CAnmScene::SetMetaData(String key, String value)
{
	CAnmMetadataEntry *pEntry = new CAnmMetadataEntry(key, value);
	m_metadatatable->AddEntry(pEntry);
}

CAnmNode *CAnmScene::GetExportedNode(const char *nodename)
{
	// N.B.: need to implement
	return NULL;
}

void CAnmScene::AddRootNode(class CAnmNode *pRootNode)
{
	// N.B.: root node can only be in one scene at a time. we should throw an exception?
	CAnmGroup *pParent = pRootNode->GetFirstParent();
	if (pParent)
	{
		pParent->RemoveChild(pRootNode);
	}

	if (m_containingGroup)
	{
		CAnmWorld *pWorld = m_containingGroup->GetWorld();
		pWorld->Lock();

		m_containingGroup->AddChild(pRootNode);

		pWorld->Unlock();
	}
	else
		AddChild(pRootNode);
}

void CAnmScene::RemoveRootNode(class CAnmNode *pRootNode)
{
	if(m_containingGroup)
	{
		CAnmWorld *pWorld = m_containingGroup->GetWorld();
		pWorld->Lock();

		m_containingGroup->RemoveChild(pRootNode);

		pWorld->Unlock();
	}
	else
		RemoveChild(pRootNode);
}


#define DEFINE_SCENEPROTO_CLASS(_cls_, _super_) \
static CAnmObject *create##_cls_() \
{ \
	return new _cls_(); \
} \
\
static CAnmClass classdata = \
{ \
	#_cls_, \
	create##_cls_, \
	NULL, \
	0, \
	_super_::m_classdata, \
	NULL, \
}; \
CAnmClass *_cls_::m_classdata = &classdata; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; }

DEFINE_SCENEPROTO_CLASS(CAnmScene, CAnmProto)


