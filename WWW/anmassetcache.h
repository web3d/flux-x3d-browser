/********************************************************************************
 * Flux
 *
 * File: anmassetcache.h
 * Description: Cached web asset support
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

#ifndef _anmassetcache_h
#define _anmassetcache_h

#include "anmsymbol.h"

typedef enum eAssetType {
	eSceneAsset,
	eImageAsset,
	eSoundAsset,
	eVideoAsset,
	eScriptAsset,
	eBadAssetType
} eAssetType ;

class CAnmAsset : public CAnmSymbol
{
protected :

	eAssetType				m_type;

public :

	CAnmAsset(String name, eAssetType assetType) : CAnmSymbol(name)
	{
		m_type = assetType;
	}

	~CAnmAsset()
	{
	}

	// Accessors
	virtual void SetType(eAssetType type)
	{
		m_type = type;
	}

	virtual eAssetType GetType()
	{
		return m_type;
	}
};

class CAnmSceneAsset : public CAnmAsset
{
protected :

	class CAnmSceneSymbol			*m_sceneSymbol;
	String							 m_requestUrl;

public :

	CAnmSceneAsset(String name, class CAnmSceneSymbol *pSceneSymbol) :
	   CAnmAsset(name, eSceneAsset)
	{
		m_sceneSymbol = pSceneSymbol;
	}

	~CAnmSceneAsset()
	{
	}

	// Accessors
	virtual void SetSceneSymbol(class CAnmSceneSymbol *pSceneSymbol)
	{
		m_sceneSymbol = pSceneSymbol;
	}

	virtual class CAnmSceneSymbol *GetSceneSymbol()
	{
		return m_sceneSymbol;
	}
};

class CAnmImageAsset : public CAnmAsset
{
protected :

	class CAnmTextureData			*m_textureData;

public :

	CAnmImageAsset(String name, class CAnmTextureData *pTextureData) :
	   CAnmAsset(name, eImageAsset)
	{
		m_textureData = pTextureData;
	}

	~CAnmImageAsset()
	{
	}

	// Accessors
	virtual void SetTextureData(class CAnmTextureData *pTextureData)
	{
		m_textureData = pTextureData;
	}

	virtual class CAnmTextureData *GetTextureData()
	{
		return m_textureData;
	}
};

class CAnmAssetCache : public CAnmSymbolTable
{
public :
	
	void AddAsset(CAnmAsset *pAsset)
	{
		assert(pAsset);

		CAnmSymbolTable::AddSymbol(pAsset);
	}

	void RemoveAsset(CAnmAsset *pAsset)
	{
		CAnmSymbolTable::RemoveSymbol(pAsset);
	}

	CAnmAsset *FindAsset(const char * str)
	{
		return (CAnmAsset *) CAnmSymbolTable::FindSymbol(str);
	}
};

#endif // _anmassetcache_h

