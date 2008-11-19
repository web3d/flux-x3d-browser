/********************************************************************************
 * Flux
 *
 * File: anmtexture.h
 * Description: Base Texture node
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

#ifndef _anmtexture_h
#define _anmtexture_h

#include "anmgroup.h"

#define ANMTEXTURE_MAXLEVELS		8

#define ANMTEXTURE_DEFAULT_REPEATS	TRUE
#define ANMTEXTURE_DEFAULT_REPEATT	TRUE

// N.B.: Minimum size for turning on texture filtering. Where should this define
// really live?
#define ANMTEXTURE_MINSIZEFORFILTER 1024

class CAnmTexture : public CAnmGroup
{

protected:
	
	Boolean							 m_repeatS;
	Boolean							 m_repeatT;

	class CAnmGraphics::Texture		*m_textureSurface;

	int								 m_stage;

	virtual void SetStage(int stage)
	{
		m_stage = stage;
	}

	virtual int GetStage()
	{
		return m_stage;
	}

	friend class CAnmMultiTexture;

	static bool m_drawTextures;

public:

	// constructor/destructor
	CAnmTexture();
	virtual ~CAnmTexture();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	// New methods
	virtual BOOL HasAlpha();
	virtual BOOL IsGrayscale();
	virtual void SetToDevice(class CAnmDevice *pDevice, bool alphablending,
		bool lighting);
	virtual void SetToDevice(class CAnmDevice *pDevice, bool alphablending,
		bool lighting, int startLevel, int nLevels)
	{
		SetToDevice(pDevice, alphablending, lighting);
	}
	virtual int GetTexCoordVectorSize(){ return 2; }  // Useually, we have 2 tex coords.  3 for cube maps
	virtual bool handleChildTextureLoaded( CAnmTexture* pSourceTexture, class CAnmTextureData *pTextureData ){ return false; }

	static void SetToDevice( CAnmDevice *pDevice, 
							  bool alphablending, bool lighting,
							  CAnmGraphics::Texture* pTextureSurface,
							  Boolean bRepeatS,
							  Boolean bRepeatT );


	virtual int NumLevels()
	{
		return 1;
	}

	virtual bool IsValid()
	{
		Lock();
		bool isvalid = (m_textureSurface != NULL);
		Unlock();
		return isvalid;
	}

	static void SetDrawTextures(bool drawTextures);
	static bool GetDrawTextures();

	// Accessors
	void SetRepeatS(Boolean repeatS);
	Boolean GetRepeatS() { return m_repeatS; }
	void GetRepeatS(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_repeatS;
	}

	void SetRepeatT(Boolean repeatT);
	Boolean GetRepeatT() { return m_repeatT; }
	void GetRepeatT(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_repeatT;
	}

	class CAnmGraphics::Texture *GetSurface()
	{
		Lock();
		CAnmGraphics::Texture *pTextureSurface = m_textureSurface;
		Unlock();

		return pTextureSurface;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmTexture);

	DECLARE_VALUE(repeatS);
	DECLARE_VALUE(repeatT);

};

DECLARE_GETIMPLEMENTATION(CAnmTexture);

#define ANMTEXTURE_SURFACEDDIRTY			(ANMGROUP_NDIRTYBITS)
#define ANMTEXTURE_URLDIRTY					(ANMTEXTURE_SURFACEDDIRTY + 1)
#define ANMTEXTURE_NDIRTYBITS				(ANMTEXTURE_URLDIRTY + 1)

typedef enum eAnmTextureDirtyFlags {
eAnmTextureSurfaceDirty				= ANMDIRTYBIT(ANMTEXTURE_SURFACEDDIRTY),
eAnmTexturUrlDirty					= ANMDIRTYBIT(ANMTEXTURE_URLDIRTY),
} eAnmTextureDirtyFlags;


#endif // _anmtexture_h
