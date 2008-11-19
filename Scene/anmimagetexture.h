/********************************************************************************
 * Flux
 *
 * File: anmimagetexture.h
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

#ifndef _anmimagetexture_h
#define _anmimagetexture_h

#include "anmtexture.h"
#include "anmurlobject.h"

class CAnmImageTexture : public CAnmTexture, public CAnmUrlObject
{

protected:
	
	// helpers for async texture file loading
	virtual class CAnmAsset *handleUrlLoaded(String pRequestUrl, String pCacheFileName);
	virtual void handleUrlLoaded(String pRequestUrl, class CAnmAsset *pAsset);
	virtual void handleTextureLoaded(class CAnmTextureData *pTextureData);

public:

	// constructor/destructor
	CAnmImageTexture();
	virtual ~CAnmImageTexture();

	// CAnmTexture overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual void SetBaseUrl(String baseUrl)
	{
		CAnmUrlObject::SetBaseUrl(baseUrl);
	}

	virtual String GetBaseUrl()
	{
		return CAnmUrlObject::GetBaseUrl();
	}

	// basic QueryInterface-like capability
	virtual bool GetInterface(ANMINTERFACE ival, void *pvoid)
	{
		if (ival == ANMINTERFACEID(CAnmUrlObject))
		{
			CAnmUrlObject **ppUrlObj = (CAnmUrlObject **) pvoid;
			*ppUrlObj = this;
			return true;
		}

		return false;
	}


	// Accessors
	void SetUrl(StringArray *pUrl);
	// N.B.: GetUrl methods defined in CAnmUrlObject superclass

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmImageTexture);

	DECLARE_VALUE(url);
	DECLARE_VALUE(repeatS);
	DECLARE_VALUE(repeatT);

	DECLARE_METHOD(set_url);

	DECLARE_CALLBACK(url_changed);
};


#define ANMIMAGETEXTURE_URLDIRTY				ANMTEXTURE_NDIRTYBITS
#define ANMIMAGETEXTURE_NDIRTYBITS				(ANMIMAGETEXTURE_URLDIRTY + 1)

typedef enum eAnmImageTextureDirtyFlags {
eAnmImageTextureUrlDirty				= ANMDIRTYBIT(ANMIMAGETEXTURE_URLDIRTY),
} eAnmImageTextureDirtyFlags;


#endif // _anmimagetexture_h

