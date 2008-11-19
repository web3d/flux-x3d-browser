/********************************************************************************
 * Flux
 *
 * File: anmmultitexture.h
 * Description: MultiTexture node
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

#ifndef _anmmultitexture_h
#define _anmmultitexture_h

#include "anmtexture.h"

#define ANMMULTITEXTURE_DEFAULT_ALPHA			1.f
#define ANMMULTITEXTURE_DEFAULT_COLOR			Color(1,1,1)
#define ANMMULTITEXTURE_DEFAULT_TRANSPARENT		false

#define ANMMT_ALPHADIRTY					ANMTEXTURE_NDIRTYBITS
#define ANMMT_COLORDIRTY					(ANMMT_ALPHADIRTY + 1)
#define ANMMT_FUNCTIONDIRTY					(ANMMT_ALPHADIRTY + 2)
#define ANMMT_MODEDIRTY						(ANMMT_ALPHADIRTY + 3)
#define ANMMT_SOURCEDIRTY					(ANMMT_ALPHADIRTY + 4)
#define ANMMT_TEXTUREDIRTY					(ANMMT_ALPHADIRTY + 5)
#define ANMMT_TRANSPARENTDIRTY				(ANMMT_ALPHADIRTY + 6)

typedef enum eAnmMTDirtyFlags {
eAnmMTAlphaDirty				= ANMDIRTYBIT(ANMMT_ALPHADIRTY),
eAnmMTColorDirty				= ANMDIRTYBIT(ANMMT_COLORDIRTY),
eAnmMTFunctionDirty				= ANMDIRTYBIT(ANMMT_FUNCTIONDIRTY),
eAnmMTModeDirty					= ANMDIRTYBIT(ANMMT_MODEDIRTY),
eAnmMTSourceDirty				= ANMDIRTYBIT(ANMMT_SOURCEDIRTY),
eAnmMTTextureDirty				= ANMDIRTYBIT(ANMMT_TEXTUREDIRTY),
eAnmMTTransparentDirty			= ANMDIRTYBIT(ANMMT_TRANSPARENTDIRTY),

} eAnmMTDirtyFlags;


class CAnmMultiTexture : public CAnmTexture
{

protected:
	
	Float						 m_alpha;
	Color						 m_color;
	StringArray					*m_function;
	StringArray					*m_mode;
	StringArray					*m_source;
	NodeArray					*m_texture;
	Boolean						 m_transparent;

	eAnmTextureMode				 m_modes[ANMTEXTURE_MAXLEVELS];
	eAnmTextureMode				 m_alphamodes[ANMTEXTURE_MAXLEVELS];
	eAnmTextureSource			 m_sources[ANMTEXTURE_MAXLEVELS];
	eAnmTextureFunction			 m_functions[ANMTEXTURE_MAXLEVELS];

	virtual CAnmTexture *GetTextureChild(int index);
	virtual void SetStageToDevice(class CAnmDevice *pDevice, CAnmTexture *pTexture, 
		int stage, int level, bool alphablending, bool lighting);

	virtual eAnmTextureMode String2Mode(const char *modename);
	virtual eAnmTextureSource String2Source(const char *sourcename);
	virtual eAnmTextureFunction String2Function(const char *fnname);

public:

	// constructor/destructor
	CAnmMultiTexture();
	virtual ~CAnmMultiTexture();

	// CAnmTexture overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual BOOL HasAlpha();
	virtual void SetToDevice(class CAnmDevice *pDevice, bool alphablending,
		bool lighting, int startLevel, int nLevels);
	virtual int NumLevels();


	// Accessors
	void SetAlpha(Float alpha);
	Float GetAlpha() { return m_alpha; }
	void GetAlpha(Float *pVal)
	{
		assert(pVal);
		*pVal = m_alpha;
	}

	void SetColor(Color color);
	Color GetColor() { return m_color; }
	void GetColor(Color *pVal)
	{
		assert(pVal);
		*pVal = m_color;
	}

	void SetFunction(StringArray *pFunction);
	StringArray *GetFunction() { return m_function; }
	void GetFunction(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_function;
	}

	void SetMode(StringArray *pMode);
	StringArray *GetMode() { return m_mode; }
	void GetMode(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_mode;
	}

	void SetSource(StringArray *pSource);
	StringArray *GetSource() { return m_source; }
	void GetSource(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_source;
	}

	void SetTexture(NodeArray *pTexture);
	NodeArray *GetTexture() { return m_texture; }
	void GetTexture(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_texture;
	}

	void SetTransparent(Boolean transparent);
	Boolean GetTransparent() { return m_transparent; }
	void GetTransparent(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_transparent;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmMultiTexture);

	DECLARE_VALUE(alpha);
	DECLARE_VALUE(color);
	DECLARE_VALUE(function);
	DECLARE_VALUE(mode);
	DECLARE_VALUE(source);
	DECLARE_VALUE(texture);
	DECLARE_VALUE(transparent);

	DECLARE_METHOD(set_alpha);
	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_function);
	DECLARE_METHOD(set_mode);
	DECLARE_METHOD(set_source);
	DECLARE_METHOD(set_texture);
	DECLARE_METHOD(set_transparent);

	DECLARE_CALLBACK(alpha_changed);
	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(function_changed);
	DECLARE_CALLBACK(mode_changed);
	DECLARE_CALLBACK(source_changed);
	DECLARE_CALLBACK(texture_changed);
	DECLARE_CALLBACK(transparent_changed);
};


#endif // _anmmultitexture_h
