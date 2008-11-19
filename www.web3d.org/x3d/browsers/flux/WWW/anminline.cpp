/********************************************************************************
 * Flux
 *
 * File: anminline.cpp
 * Description: WWW Inline node class
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

#include "anminline.h"
#include "anmassetcache.h"
#include "anmfilemanager.h"
#include "anmfileloader.h"
#include "anmproto.h"
#include "anmscene.h"
#include "anmworld.h"

CAnmInline::CAnmInline() : CAnmGroup (),
	CAnmUrlObject()
{
	m_loading = false;
	m_inlineScene = NULL;
	m_baseUrlForChildren = NULL;
}

CAnmInline::~CAnmInline()
{
	SafeUnRef(m_inlineScene);
	SafeUnRef(m_baseUrlForChildren);
}

#define MAXBUFLEN 2048

void CAnmInline::Realize()
{
	if (m_realized)
		return;

	Lock();

	if (m_url->size() <= 0)
	{
		m_realized = TRUE;
	}

	if (!m_loading && !m_realized) 
	{
		// N.B.: these string lengths are limited; should go to allocated scheme
		// set up base Url for when children are loaded
		char fullnamebuf[MAXBUFLEN];

		if ( m_baseUrl && m_baseUrl->length())
		{
			ComputeFullName((*m_url)[0]->GetBuf(), m_baseUrl->GetBuf(), fullnamebuf );
		}
		else
		{
			strcpy(fullnamebuf, (*m_url)[0]->GetBuf());
		}

		char *fullname = fullnamebuf;

		// N.B.: heap
		char *cp = new char[MAXBUFLEN];
		CAnmUrlObject::ComputeBaseUrl(fullname, cp, MAXBUFLEN);
		m_baseUrlForChildren = new CAnmString(cp);

		delete cp;

		// set loading flag so we don't try to load twice if we're multiply instanced
		m_loading = true;

		// don't let us traverse until we're fully loaded
		m_realized = FALSE;

		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL_UNLOCK

		cApplication *pApp = pWorld->GetApp();
		assert(pApp);

		// get the file loaded
		Load(pApp);
	}

	Unlock();

}

void CAnmInline::Traverse(CAnmUpdateVisitor *pVisitor)
{
	Lock();

	BOOL realized = m_realized;
	bool loading = m_loading;

	Unlock();

	if (!realized || loading)
		return;

	CAnmGroup::Traverse(pVisitor);
}

class CAnmAsset *CAnmInline::handleUrlLoaded(String pRequestUrl, String pCacheFileName)
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();

	NodeArray *pNodesReturn = NULL;

	CAnmSceneSymbol *pSceneSymbol = NULL;

	// N.B.: use master scene for now but should really use innermost containing
	// scene
	BEGIN_PROFILE_BLOCK("INLINE LOAD");
	
	pSceneSymbol = CAnmFileLoader::LoadScene(pCacheFileName->GetBuf(), 
		m_baseUrlForChildren, pApp);

	END_PROFILE_BLOCK("INLINE LOAD");

	if (pSceneSymbol)
	{
		handleSceneLoaded(pSceneSymbol, pApp, pWorld);

		return new CAnmSceneAsset(pCacheFileName, pSceneSymbol);
	}
	else
		return NULL;
}

void CAnmInline::handleUrlLoaded(String pRequestUrl, CAnmAsset *pAsset)
{
	assert(pAsset);
	assert(pAsset->GetType() == eSceneAsset);

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();

	CAnmSceneAsset *pSceneAsset = (CAnmSceneAsset *) pAsset;
	handleSceneLoaded(pSceneAsset->GetSceneSymbol(), pApp, pWorld);
}

void CAnmInline::handleSceneLoaded(CAnmSceneSymbol *pSceneSymbol, cApplication *pApp, CAnmWorld *pWorld)
{
	// get a World lock since we're doing this from a background thread
	pWorld->Lock();

	BEGIN_PROFILE_BLOCK("INLINE INSTANCE");

	CAnmScene *pInlineScene = pSceneSymbol->CreateScene(pApp, m_baseUrlForChildren);

	if (pInlineScene)
	{
		m_inlineScene = pInlineScene;

		// Add scene's children to my list
		NodeArray *pChildren = NULL;
		m_inlineScene->GetChildren(&pChildren);
		AddChildren(pChildren);

		m_inlineScene->RemoveAllChildren();

		m_inlineScene->SetContainingGroup(this);
	}

	END_PROFILE_BLOCK("INLINE INSTANCE");

	// turn off loading flag
	Lock();
	m_loading = false;	
	Unlock();

	// make sure it all gets rebuilt
	SetChildrenDirty();
	SetStateAndMatrixDirty();

	// N.B.: this is just to update the polycount, duh
	pWorld->SetStateDirty();

	// make sure the children are realized;
	// the Group class's realize will do it
	CAnmGroup::Realize();

	// release the scene lock
	pWorld->Unlock();
}

CAnmProto *CAnmInline::GetProtoContext()
{
	return m_inlineScene;
}

// Accessors
void CAnmInline::SetUrl(StringArray *pUrl)
{
	CAnmUrlObject::SetUrl(pUrl);

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmInline, url_changed), &pUrl);
}

void CAnmInline::Update()
{
	// at some point, do ugly stuff
	if (StateDirty())
	{
		// N.B.: this is just to update the polycount, duh
		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL

		pWorld->SetStateDirty();

		ClearStateDirty();
	}
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_INHERITED_VALUE(CAnmInline, bboxCenter, CAnmGroup);
DEFINE_INHERITED_VALUE(CAnmInline, bboxSize, CAnmGroup);
DEFINE_VALUE(CAnmInline, url, eValueStringArray, StringArray *, GetUrl, SetUrl);

DEFINE_INHERITED_METHOD(CAnmInline, set_bboxCenter, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmInline, set_bboxSize, CAnmGroup);
DEFINE_METHOD(CAnmInline, set_url, eValueStringArray, StringArray *, SetUrl);

DEFINE_INHERITED_CALLBACK(CAnmInline, bboxCenter_changed, CAnmGroup);
DEFINE_INHERITED_CALLBACK(CAnmInline, bboxSize_changed, CAnmGroup);
DEFINE_CALLBACK(CAnmInline, url_changed, eValueStringArray);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmInline)
VALUEID(CAnmInline, bboxCenter),
VALUEID(CAnmInline, bboxSize),
VALUEID(CAnmInline, url),

METHODID(CAnmInline, set_bboxCenter),
METHODID(CAnmInline, set_bboxSize),
METHODID(CAnmInline, set_url),

CALLBACKID(CAnmInline, bboxCenter_changed),
CALLBACKID(CAnmInline, bboxSize_changed),
CALLBACKID(CAnmInline, url_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmInline, CAnmGroup);



