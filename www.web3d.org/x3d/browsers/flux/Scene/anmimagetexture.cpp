/********************************************************************************
 * Flux
 *
 * File: anmimagetexture.cpp
 * Description: ImageTexture node
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

#include "anmimagetexture.h"
#include "anmassetcache.h"
#include "anmfilemanager.h"
#include "anmmessage.h"
#include "anmproto.h"
#include "anmtexturedata.h"
#include "anmworld.h"

CAnmImageTexture::CAnmImageTexture()
		: CAnmTexture(),						// Parent Appearance will add me
	CAnmUrlObject()
{
}


CAnmImageTexture::~CAnmImageTexture()
{
}

void CAnmImageTexture::Realize()
{
	// get the file loaded
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	// See if the texture is already in our cache
	bool loaded = false;

	String requestUrl = NULL;
	String cachedFileName = NULL;

	if (!loaded)
	{
		if (!m_baseUrl)
		{
			CAnmProto *pProto = GetProtoContext();

			if (pProto)
			{
				SetBaseUrl(pProto->GetBaseUrl());
			}
		}
		Load(pApp);
	}

	CAnmTexture::Realize();

	ClearStateDirty();
}

void CAnmImageTexture::Update()
{
	if (!StateDirty())
		return;

	if (!m_realized)
		return;

	if (TestDirtyBits(ANMTEXTURE_URLDIRTY))
	{
		// Must be the URL that's dirty, right?
		SafeUnRef(m_textureSurface);

		// get the file loaded
		CAnmWorld *pWorld = GetWorld();
		assert(pWorld);
	
		if( pWorld ) {
			cApplication *pApp = pWorld->GetApp();
			assert(pApp);
			Load(pApp);

			ClearDirtyBits(ANMTEXTURE_URLDIRTY);
		}
	}

	CAnmTexture::Update();
}

class CAnmAsset *CAnmImageTexture::handleUrlLoaded(String pRequestUrl, String pCacheFileName)
{

	if( GetWorld() == NULL ) {
		return NULL;
	}
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

	CAnmTextureData *pTextureData = NULL;

	if (format != eAnmTextureBadFormat)
	{
		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL_RTN_NULL

		cApplication *pApp = pWorld->GetApp();
		assert(pApp);

		pTextureData = CAnmTextureData::LoadTextureData(pApp, pCacheFileName->GetBuf(), format);
		if (pTextureData)
		{
			handleTextureLoaded(pTextureData);
		}
	}

	return new CAnmImageAsset(pCacheFileName, pTextureData);
}

void CAnmImageTexture::handleUrlLoaded(String pRequestUrl, CAnmAsset *pAsset)
{
	assert(pAsset);
	assert(pAsset->GetType() == eImageAsset);

	CAnmImageAsset *pImageAsset = (CAnmImageAsset *) pAsset;
	handleTextureLoaded(pImageAsset->GetTextureData());
}

void CAnmImageTexture::handleTextureLoaded(CAnmTextureData *pTextureData)
{
	// Maybe this is a bad format and we don't want to do this?
	if (pTextureData == NULL)
		return;

	bool bNeedSurface = false;
	bool bGotCubeMap = false;
	// krv:
	// Give the Parents a callback about our new texture.
	//
	std::list< CAnmGroup* >::iterator iter;
	for( 
		iter = m_parents.begin(); 
		iter != m_parents.end();
		iter++ )
	{
		bool bConsumedTexture = false;
		CAnmTexture* pParentTexture = GETIMPLEMENTATION(CAnmTexture, *iter ); 
		if( pParentTexture ) {
			bConsumedTexture = pParentTexture->handleChildTextureLoaded( this, pTextureData );
		}
		if( !bConsumedTexture ) {
			bNeedSurface = true;
		}
	}

	// theoretically, if all the Textures are consumed by the parents, then we dont need to
	// generate the native texture surface.
	//
	if( !bNeedSurface ) {
//		return;
	}

	// grab a lock; we don't want people trying to render this in a half-made state
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();

	CAnmDevice *pDevice = pApp->GetDevice();

	pWorld->Lock();

	try {

		// Lock();
		m_textureSurface = CAnmGraphics::Texture::LoadTexture(pTextureData, pDevice);
		m_textureSurface->Ref();

		SetDirtyBits(ANMTEXTURE_SURFACEDDIRTY);
		SetStateDirty();

		ClearDirtyBits(ANMTEXTURE_URLDIRTY);
		// Unlock();
	}
	catch (CAnmError &err)
	{
		assert(m_url);

		int sz = m_url->size();

		char *filename = "unknown";
		if (sz > 0)
		{
			String s = (*m_url)[0];
			filename  = s->GetBuf();
		}

		// ANMMSG_ERRORLOADINGTEXTURE : "Error loading texture '%s' : %s; exiting"
		char *errmsg = (char *) malloc(strlen(ANMMSG_ERRORLOADINGTEXTURE) + 
			strlen(filename) +
			strlen(err.GetText()) +
			1);

		sprintf(errmsg, ANMMSG_ERRORLOADINGTEXTURE, filename, err.GetText());

		pApp->Warn(errmsg);

		CAnmError err2(errmsg);

		delete errmsg;

		throw err2;
	}
	// release the lock
	pWorld->Unlock();
}

// Accessors
void CAnmImageTexture::SetUrl(StringArray *pUrl)
{
	CAnmUrlObject::SetUrl(pUrl);

	SetDirtyBits(ANMTEXTURE_URLDIRTY);
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmImageTexture, url_changed), &pUrl);
}

// Tables for Callbacks, Methods and Set/GetValues


DEFINE_VALUE(CAnmImageTexture, url, eValueStringArray, StringArray*, GetUrl, SetUrl);
DEFINE_INHERITED_VALUE(CAnmImageTexture, repeatS, CAnmTexture);
DEFINE_INHERITED_VALUE(CAnmImageTexture, repeatT, CAnmTexture);

DEFINE_METHOD(CAnmImageTexture, set_url, eValueStringArray, StringArray*, SetUrl);

DEFINE_CALLBACK(CAnmImageTexture, url_changed, eValueStringArray);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmImageTexture)
VALUEID(CAnmImageTexture, url),
VALUEID(CAnmImageTexture, repeatS),
VALUEID(CAnmImageTexture, repeatT),
METHODID(CAnmImageTexture, set_url),
CALLBACKID(CAnmImageTexture, url_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmImageTexture, CAnmTexture);
