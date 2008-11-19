/********************************************************************************
 * Flux
 *
 * File: anmcomposedcubemaptexture.h
 * Description: anmcomposedcubemaptexture node
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

#ifndef _anmcomposedcubemaptexture_h
#define _anmcomposedcubemaptexture_h

#include "anmtexture.h"
//  #include "anmurlobject.h"

#define		ANM_N_CUBE_FACES		6


typedef enum eAnmCubeFaces {
eAnmCFRight,
eAnmCFLeft,
eAnmCFTop,
eAnmCFBottom,
eAnmCFFront,
eAnmCFBack
} eAnmCubeFaces;



class CAnmComposedCubeMapTexture : public CAnmTexture
{

protected:
	


	CAnmNode						*m_front;
	CAnmNode						*m_back;
	CAnmNode						*m_left;
	CAnmNode						*m_right;
	CAnmNode						*m_top;
	CAnmNode						*m_bottom;

	CAnmNode						**m_ppTextures[ANM_N_CUBE_FACES];
	CAnmTextureData					*m_pTextureData[6];

	// helpers for async texture file loading
//	virtual class CAnmAsset *handleUrlLoaded(String pRequestUrl, String pCacheFileName);
//	virtual void handleUrlLoaded(String pRequestUrl, class CAnmAsset *pAsset);
//	virtual void handleTextureLoaded(class CAnmTextureData *pTextureData);

	void SetTexture( eAnmCubeFaces iFace, CAnmNode *pTexture);


public:

	// constructor/destructor
	CAnmComposedCubeMapTexture();
	virtual ~CAnmComposedCubeMapTexture();

	// CAnmTexture overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual int GetTexCoordVectorSize(){ return 3; }  // Ussually, we have 2 tex coords.  3 for cube maps

	bool TryToLoadCubeMape( );
	bool TryToAddTextureToCubeMape( int iFace  );

	virtual bool handleChildTextureLoaded( CAnmTexture* pSourceTexture, class CAnmTextureData *pTextureData );


	virtual void SetBaseUrl(String baseUrl)
	{
//		CAnmUrlObject::SetBaseUrl(baseUrl);
	}

//	virtual String GetBaseUrl()
//	{
//		return CAnmUrlObject::GetBaseUrl();
//	}

	// basic QueryInterface-like capability
	virtual bool GetInterface(ANMINTERFACE ival, void *pvoid)
	{
		return false;
	}


	// Accessors

	void SetFront(CAnmNode *pTex);
	CAnmNode *GetFront()
	{
		return m_front;
	}
	void GetFront(CAnmNode **ppTex)
	{
		assert(ppTex);
		*ppTex = m_front;
	}

	void SetBack(CAnmNode *pTex);
	CAnmNode *GetBack()
	{
		return m_back;
	}
	void GetBack(CAnmNode **ppTex)
	{
		assert(ppTex);
		*ppTex = m_back;
	}


	void SetLeft(CAnmNode *pTex);
	CAnmNode *GetLeft()
	{
		return m_left;
	}
	void GetLeft(CAnmNode **ppTex)
	{
		assert(ppTex);
		*ppTex = m_left;
	}


	void SetRight(CAnmNode *pTex);
	CAnmNode *GetRight()
	{
		return m_right;
	}
	void GetRight(CAnmNode **ppTex)
	{
		assert(ppTex);
		*ppTex = m_right;
	}


	void SetTop(CAnmNode *pTex);
	CAnmNode *GetTop()
	{
		return m_top;
	}
	void GetTop(CAnmNode **ppTex)
	{
		assert(ppTex);
		*ppTex = m_top;
	}


	void SetBottom(CAnmNode *pTex);
	CAnmNode *GetBottom()
	{
		return m_bottom;
	}
	void GetBottom(CAnmNode **ppTex)
	{
		assert(ppTex);
		*ppTex = m_bottom;
	}


	// N.B.: GetUrl methods defined in CAnmUrlObject superclass

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmComposedCubeMapTexture);

	DECLARE_VALUE(front);
	DECLARE_VALUE(back);
	DECLARE_VALUE(left);
	DECLARE_VALUE(right);
	DECLARE_VALUE(top);
	DECLARE_VALUE(bottom);

	DECLARE_METHOD(set_front);
	DECLARE_METHOD(set_back);
	DECLARE_METHOD(set_left);
	DECLARE_METHOD(set_right);
	DECLARE_METHOD(set_top);
	DECLARE_METHOD(set_bottom);

	DECLARE_CALLBACK(front_changed);
	DECLARE_CALLBACK(back_changed);
	DECLARE_CALLBACK(left_changed);
	DECLARE_CALLBACK(right_changed);
	DECLARE_CALLBACK(top_changed);
	DECLARE_CALLBACK(bottom_changed);
};



DECLARE_GETIMPLEMENTATION(CAnmComposedCubeMapTexture);



#define ANMCOMPOSEDCUBEMAPTEXTURE_CHILDDIRTY				ANMTEXTURE_NDIRTYBITS
#define ANMCOMPOSEDCUBEMAPTEXTURE_NDIRTYBITS				(ANMCOMPOSEDCUBEMAPTEXTURE_CHILDDIRTY + 1)

typedef enum eAnmComposedCubeMapTextureDirtyFlags {
eAnmComposedCubeMapTextureChildDirty				= ANMDIRTYBIT(ANMCOMPOSEDCUBEMAPTEXTURE_CHILDDIRTY),
} eAnmComposedCubeMapTextureDirtyFlags;


#endif // _anmcomposedcubemaptexture_h

