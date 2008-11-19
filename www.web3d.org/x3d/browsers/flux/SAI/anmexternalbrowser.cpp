/********************************************************************************
 * Flux
 *
 * File: anmexternalbrowser.cpp
 * Description: Browser object (for SAI external access)
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
#include "../AnimaApp.h"
#include "anmexternalbrowser.h"
#include "anmsaiscene.h"
#include "anmscene.h"
#include "anmworld.h"

CAnmExternalBrowser::CAnmExternalBrowser(cApplication *pApp)
: CAnmBrowser(pApp)
{
	SetEventBuffering(false);

	pApp->AddExternalEventSource(this);
}

CAnmExternalBrowser::~CAnmExternalBrowser()
{
	m_app->RemoveExternalEventSource(this);
}

void CAnmExternalBrowser::NotifyNodeChanged(CAnmSAINode *pNode)
{
	Lock();
	CAnmSAINodeEventSink::NotifyNodeChanged(pNode);
	Unlock();
}

void CAnmExternalBrowser::ProcessEvents()
{
	Lock();
	if (!m_bufferevents)
	{
		CommitNodeChanges();
		ClearNodeChangedList();
	}
	Unlock();
}

CAnmExecutionContext *CAnmExternalBrowser::getExecutionContext()
{
	CAnmWorld *pWorld = ((cAnimaApp *)m_app)->GetWorld();
	if (pWorld == NULL)
		return NULL;

	CAnmScene *pScene = pWorld->GetScene();
	if (pScene == NULL)
		return NULL;

	return new CAnmSAIScene(m_app, pScene, this);
}

void CAnmExternalBrowser::beginUpdate()
{
	Lock();
	SetEventBuffering(true);
	Unlock();
}

void CAnmExternalBrowser::endUpdate()
{
	Lock();
	SetEventBuffering(false);
	Unlock();
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_ABSTRACT_EMPTY_CLASS(CAnmExternalBrowser, CAnmBrowser)
