/********************************************************************************
 * Flux
 *
 * File: anmsaiscene.cpp
 * Description: Scene object for SAI
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
#include "anmsaiscene.h"
#include "anmsainode.h"
#include "anmscene.h"

CAnmSAIScene::CAnmSAIScene(cApplication *pApp, CAnmScene *pScene,
		CAnmSAINodeEventSink *pSink) :
	CAnmExecutionContext(pApp, pScene, pSink)
{
}

CAnmSAIScene::~CAnmSAIScene()
{
}

String CAnmSAIScene::GetMetaData(const char *key)
{
	CAnmScene *pScene = (CAnmScene *) m_proto;

	return pScene->GetMetaData(key);
}

void CAnmSAIScene::SetMetaData(String key, String value)
{
	CAnmScene *pScene = (CAnmScene *) m_proto;

	pScene->SetMetaData(key, value);
}

CAnmSAINode *CAnmSAIScene::GetExportedNode(const char *nodename)
{
	CAnmScene *pScene = (CAnmScene *) m_proto;

	CAnmNode *pNode = pScene->GetExportedNode(nodename);

	if (pNode)
	{
		CAnmSAINode *pSAINode = new CAnmSAINode(pNode);
		pSAINode->SetSink(m_sink);
		return pSAINode;
	}
	else
		return NULL;
}

void CAnmSAIScene::AddRootNode(class CAnmSAINode *pSAIRootNode)
{
	CAnmScene *pScene = (CAnmScene *) m_proto;

	CAnmNode *pRootNode = pSAIRootNode->GetNode();

	pScene->AddRootNode(pRootNode);
}

void CAnmSAIScene::RemoveRootNode(class CAnmSAINode *pSAIRootNode)
{
	CAnmScene *pScene = (CAnmScene *) m_proto;

	CAnmNode *pRootNode = pSAIRootNode->GetNode();

	pScene->RemoveRootNode(pRootNode);
}

CAnmScene *CAnmSAIScene::GetScene()
{
	return (CAnmScene *) m_proto;
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_ABSTRACT_EMPTY_CLASS(CAnmSAIScene, CAnmExecutionContext)
