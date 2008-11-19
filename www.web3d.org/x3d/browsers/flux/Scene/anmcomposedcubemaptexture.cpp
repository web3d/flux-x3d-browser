/********************************************************************************
 * Flux
 *
 * File: ComposedCubeMapTexture.cpp
 * Description: ComposedCubeMapTexture node
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
#include "anmcomposedcubemaptexture.h"
#include "anmassetcache.h"
#include "anmfilemanager.h"
#include "anmmessage.h"
#include "anmproto.h"
#include "anmtexturedata.h"
#include "anmworld.h"

CAnmComposedCubeMapTexture::CAnmComposedCubeMapTexture()
		: CAnmTexture()						// Parent Appearance will add me
{
	m_ppTextures[eAnmCFFront] = &m_front;
	m_ppTextures[eAnmCFBack] = &m_back;
	m_ppTextures[eAnmCFLeft] = &m_left;
	m_ppTextures[eAnmCFRight] = &m_right;
	m_ppTextures[eAnmCFTop] = &m_top;
	m_ppTextures[eAnmCFBottom] = &m_bottom;


	for( int iFace=0; iFace<ANM_N_CUBE_FACES; iFace++ ) {
		*m_ppTextures[iFace] = NULL;
		m_pTextureData[iFace] = NULL;
	}
}


CAnmComposedCubeMapTexture::~CAnmComposedCubeMapTexture()
{
	for( int iFace=0; iFace<ANM_N_CUBE_FACES; iFace++ ) {
		SafeUnRef( m_pTextureData[iFace] );
	}
}

void CAnmComposedCubeMapTexture::Realize()
{
	// chain down for texture realize
	CAnmTexture::Realize();
}

void CAnmComposedCubeMapTexture::Update()
{
//	if (!StateDirty())
//		return;

	for (int index = 0; index < 6; index++)
	{
		if (*m_ppTextures[index]) {
			(*m_ppTextures[index])->Update();
		}
	}
}


bool CAnmComposedCubeMapTexture::handleChildTextureLoaded( CAnmTexture* pSourceTexture, 
								CAnmTextureData *pTextureData )
{
	// Find the Texture in our child list, then try to load the cube map.
	//
	bool bConsumed = false;

	if( pSourceTexture && pTextureData ) {
		CAnmTexture* pKidTexture = NULL;
		for( int i=0; i<6; i++ ) {
			if( *m_ppTextures[i] &&
				( pKidTexture = GETIMPLEMENTATION(CAnmTexture, *m_ppTextures[i] ) ) ) {
				if( pKidTexture == pSourceTexture &&
					m_pTextureData[i] != pTextureData ) {
					// found mathcing child texture.
					// Assign it, and try to load the CubeMap.
					//
					SafeUnRef( m_pTextureData[i] );
					m_pTextureData[i] = pTextureData;
					pTextureData->Ref();

					// Now try to update an existing CubeMap Surface
					//

					bConsumed = TryToAddTextureToCubeMape( i );
					if( !bConsumed ) {
						// Cant update, try to create one
						//
						bConsumed = TryToLoadCubeMape( );
					}
					break;
				}
			}
		}
	}
	return bConsumed;
}


bool CAnmComposedCubeMapTexture::TryToAddTextureToCubeMape( int iFace  )
{
	bool bAddedTexture = false;

	if( m_textureSurface &&
		m_pTextureData[iFace] ) {
		
		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL_RTN_NULL

		cApplication *pApp = pWorld->GetApp();

		CAnmDevice *pDevice = pApp->GetDevice();

		pWorld->Lock();

		try {

			bAddedTexture = CAnmGraphics::Texture::AddTextureDataToCubeTexture( 
				m_textureSurface, iFace, m_pTextureData[iFace], pDevice );

		}
		catch (CAnmError &err)
		{

			// ANMMSG_ERRORLOADINGTEXTURE : "Error loading texture '%s' : %s; exiting"
			char *errmsg = (char *) malloc(strlen(ANMMSG_ERRORLOADINGTEXTURE) +
				strlen(err.GetText()) +
				1);

			sprintf(errmsg, ANMMSG_ERRORLOADINGTEXTURE, err.GetText());

			pApp->Warn(errmsg);

			CAnmError err2(errmsg);

			delete errmsg;

			throw err2;
		}

		// release the lock
		pWorld->Unlock();
	}
	return bAddedTexture;

}



bool CAnmComposedCubeMapTexture::TryToLoadCubeMape( )
{
	bool bGotNewTex = false;

	// Got through our 6 Textures.  Make sure they are all sqaure, and the same size.
	//
	int iDim = 0;
	int iGotCount = 0;
	CAnmNode *pImp = NULL;
	for( int i=0; i<6; i++ ) {

		if( m_pTextureData[i] ) {
			if( iDim == 0 ) {
				if( m_pTextureData[i]->GetWidth() == m_pTextureData[i]->GetHeight() ) {
					iDim = m_pTextureData[i]->GetWidth();
					iGotCount++;
				}
			}
			else {
				if( m_pTextureData[i]->GetWidth()  == iDim &&
					m_pTextureData[i]->GetHeight() == iDim ) {
					iGotCount++;
				}
			}
		}
	}

	if( iGotCount == 6 ) {

		// grab a lock; we don't want people trying to render this in a half-made state
		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL_RTN_NULL

		cApplication *pApp = pWorld->GetApp();

		CAnmDevice *pDevice = pApp->GetDevice();

		pWorld->Lock();

		try {

			SafeUnRef( m_textureSurface );
			m_textureSurface = CAnmGraphics::Texture::LoadCubeTexture(m_pTextureData, pDevice);
			if( m_textureSurface ) {
				m_textureSurface->Ref();

				SetDirtyBits(ANMTEXTURE_SURFACEDDIRTY);
				SetStateDirty();
				bGotNewTex = true;
			}
		}
		catch (CAnmError &err)
		{

			// ANMMSG_ERRORLOADINGTEXTURE : "Error loading texture '%s' : %s; exiting"
			char *errmsg = (char *) malloc(strlen(ANMMSG_ERRORLOADINGTEXTURE) +
				strlen(err.GetText()) +
				1);

			sprintf(errmsg, ANMMSG_ERRORLOADINGTEXTURE, err.GetText());

			pApp->Warn(errmsg);

			CAnmError err2(errmsg);

			delete errmsg;

			throw err2;
		}

		// release the lock
		pWorld->Unlock();

	}
	return bGotNewTex;
}




// Accessors
// Tables for Callbacks, Methods and Set/GetValues


void CAnmComposedCubeMapTexture::SetFront( CAnmNode *pTexture )
{
	SetTexture( eAnmCFFront, pTexture );
	CallCallbacks(CALLBACKID(CAnmComposedCubeMapTexture, front_changed), m_ppTextures[eAnmCFFront] );
}
void CAnmComposedCubeMapTexture::SetBack( CAnmNode *pTexture )
{
	SetTexture( eAnmCFBack, pTexture );
	CallCallbacks(CALLBACKID(CAnmComposedCubeMapTexture, back_changed), m_ppTextures[eAnmCFBack] );
}
void CAnmComposedCubeMapTexture::SetLeft( CAnmNode *pTexture )
{
	SetTexture( eAnmCFLeft, pTexture );
	CallCallbacks(CALLBACKID(CAnmComposedCubeMapTexture, left_changed), m_ppTextures[eAnmCFLeft] );
}
void CAnmComposedCubeMapTexture::SetRight( CAnmNode *pTexture )
{
	SetTexture( eAnmCFRight, pTexture );
	CallCallbacks(CALLBACKID(CAnmComposedCubeMapTexture, right_changed), m_ppTextures[eAnmCFRight] );
}
void CAnmComposedCubeMapTexture::SetTop( CAnmNode *pTexture )
{
	SetTexture( eAnmCFTop, pTexture );
	CallCallbacks(CALLBACKID(CAnmComposedCubeMapTexture, top_changed), m_ppTextures[eAnmCFTop] );
}
void CAnmComposedCubeMapTexture::SetBottom( CAnmNode *pTexture )
{
	SetTexture( eAnmCFBottom, pTexture );
	CallCallbacks(CALLBACKID(CAnmComposedCubeMapTexture, bottom_changed), m_ppTextures[eAnmCFBottom] );
}

void CAnmComposedCubeMapTexture::SetTexture( eAnmCubeFaces iFace, CAnmNode *pTexture)
{
	if (*m_ppTextures[iFace] == pTexture)
		return;

	if (*m_ppTextures[iFace]) {
		RemoveChild(*m_ppTextures[iFace]);
	}

	if (pTexture == NULL)
		return;

	AddChild(pTexture);
	*m_ppTextures[iFace] = pTexture;


	SetStateDirty();


}






DEFINE_VALUE(CAnmComposedCubeMapTexture, front, eValueNode, CAnmNode *, GetFront, SetFront);
DEFINE_VALUE(CAnmComposedCubeMapTexture, back, eValueNode, CAnmNode *, GetBack, SetBack);
DEFINE_VALUE(CAnmComposedCubeMapTexture, left, eValueNode, CAnmNode *, GetLeft, SetLeft);
DEFINE_VALUE(CAnmComposedCubeMapTexture, right, eValueNode, CAnmNode *, GetRight, SetRight);
DEFINE_VALUE(CAnmComposedCubeMapTexture, top, eValueNode, CAnmNode *, GetTop, SetTop);
DEFINE_VALUE(CAnmComposedCubeMapTexture, bottom, eValueNode, CAnmNode *, GetBottom, SetBottom);

DEFINE_METHOD(CAnmComposedCubeMapTexture, set_front, eValueNode, CAnmNode *, SetFront);
DEFINE_METHOD(CAnmComposedCubeMapTexture, set_back, eValueNode, CAnmNode *, SetBack);
DEFINE_METHOD(CAnmComposedCubeMapTexture, set_left, eValueNode, CAnmNode *, SetLeft);
DEFINE_METHOD(CAnmComposedCubeMapTexture, set_right, eValueNode, CAnmNode *, SetRight);
DEFINE_METHOD(CAnmComposedCubeMapTexture, set_top, eValueNode, CAnmNode *, SetTop);
DEFINE_METHOD(CAnmComposedCubeMapTexture, set_bottom, eValueNode, CAnmNode *, SetBottom);

DEFINE_CALLBACK(CAnmComposedCubeMapTexture, front_changed, eValueNode);
DEFINE_CALLBACK(CAnmComposedCubeMapTexture, back_changed, eValueNode);
DEFINE_CALLBACK(CAnmComposedCubeMapTexture, left_changed, eValueNode);
DEFINE_CALLBACK(CAnmComposedCubeMapTexture, right_changed, eValueNode);
DEFINE_CALLBACK(CAnmComposedCubeMapTexture, top_changed, eValueNode);
DEFINE_CALLBACK(CAnmComposedCubeMapTexture, bottom_changed, eValueNode);





// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmComposedCubeMapTexture)

VALUEID(CAnmComposedCubeMapTexture, front),
VALUEID(CAnmComposedCubeMapTexture, back),
VALUEID(CAnmComposedCubeMapTexture, left),
VALUEID(CAnmComposedCubeMapTexture, right),
VALUEID(CAnmComposedCubeMapTexture, top),
VALUEID(CAnmComposedCubeMapTexture, bottom),
METHODID(CAnmComposedCubeMapTexture, set_front),
METHODID(CAnmComposedCubeMapTexture, set_back),
METHODID(CAnmComposedCubeMapTexture, set_left),
METHODID(CAnmComposedCubeMapTexture, set_right),
METHODID(CAnmComposedCubeMapTexture, set_top),
METHODID(CAnmComposedCubeMapTexture, set_bottom),
CALLBACKID(CAnmComposedCubeMapTexture, front_changed),
CALLBACKID(CAnmComposedCubeMapTexture, back_changed),
CALLBACKID(CAnmComposedCubeMapTexture, left_changed),
CALLBACKID(CAnmComposedCubeMapTexture, right_changed),
CALLBACKID(CAnmComposedCubeMapTexture, top_changed),
CALLBACKID(CAnmComposedCubeMapTexture, bottom_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmComposedCubeMapTexture, CAnmTexture);
