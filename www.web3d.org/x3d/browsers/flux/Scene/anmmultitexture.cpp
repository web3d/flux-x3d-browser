/********************************************************************************
 * Flux
 *
 * File: anmmultitexture.cpp
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

#include "stdafx.h"
#include "anmmultitexture.h"
#include "anmdevice.h"
#include "anmworld.h"
				
CAnmMultiTexture::CAnmMultiTexture() : CAnmTexture(),
	m_alpha(ANMMULTITEXTURE_DEFAULT_ALPHA),
	m_color(ANMMULTITEXTURE_DEFAULT_COLOR),
	m_transparent(ANMMULTITEXTURE_DEFAULT_TRANSPARENT)
{		
	m_function = new StringArray;
	m_mode = new StringArray;
	m_source = new StringArray;
	m_texture = new NodeArray;
}

CAnmMultiTexture::~CAnmMultiTexture()
{
	SafeUnRef(m_function);
	SafeUnRef(m_mode);
	SafeUnRef(m_source);
	SafeUnRef(m_texture);
}

void CAnmMultiTexture::Realize()
{
	int sz = m_texture->size();

	for (int i = 0; i < sz; i++)
	{
		CAnmTexture *pTexture = GetTextureChild(i);
		if (pTexture)
			pTexture->SetStage(i);
	}

	// chain down for texture realize
	CAnmTexture::Realize();
	
	// N.B.: for now - flag everything dirty
	// SetDirtyBits(0xFFFFFFFF);
}

void CAnmMultiTexture::Update()
{
	if (!StateDirty())
		return;

	int sz = m_texture->size();

	for (int i = 0; i < sz; i++)
	{
		CAnmTexture *pTexture = GetTextureChild(i);
		if (pTexture)
			pTexture->Update();
		
		const char *cp;
		char *cp2;

		if (m_mode->size() > i)
		{
			cp = (*m_mode)[i]->GetBuf();
			m_modes[i] = String2Mode(cp);

			if (m_modes[i] == eAnmTextureModeBADMODE)
				m_modes[i] = eAnmTextureModeMODULATE;

			cp2 = strchr(cp, ',');
			if (cp2)
			{
				cp2++;
				m_alphamodes[i] = String2Mode((const char *) cp2);

				if (m_alphamodes[i] == eAnmTextureModeBADMODE)
					m_alphamodes[i] = eAnmTextureModeOFF;

			}
			else
			{
				//if (m_modes[i] == eAnmTextureModeMODULATE)
					m_alphamodes[i] = eAnmTextureModeMODULATE;
				//else
				//	m_alphamodes[i] = eAnmTextureModeOFF;
			}
		}
		else
		{
			m_modes[i] = eAnmTextureModeMODULATE;
			m_alphamodes[i] = eAnmTextureModeOFF;
		}

		if (m_source->size() > i)
		{
			cp = (*m_source)[i]->GetBuf();
			m_sources[i] = String2Source(cp);

			if (m_sources[i] == eAnmTextureSourceBADSOURCE)
				m_sources[i] = eAnmTextureSourceCURRENT;
		}
		else
			m_sources[i] = eAnmTextureSourceCURRENT;

		if (m_function->size() > i)
		{
			cp = (*m_function)[i]->GetBuf();
			m_functions[i] = String2Function(cp);

			if (m_functions[i] == eAnmTextureFunctionBADFUNCTION)
				m_functions[i] = eAnmTextureFunctionNONE;
		}
		else
			m_functions[i] = eAnmTextureFunctionNONE;
	}

	ClearStateDirty();
}

BOOL CAnmMultiTexture::HasAlpha()
{
	int sz = m_texture->size();

	for (int i = 0; i < sz; i++)
	{
		CAnmTexture *pTexture = GetTextureChild(i);
		if (pTexture && pTexture->HasAlpha())
			return TRUE;
	}

	return FALSE;
}

void CAnmMultiTexture::SetToDevice(class CAnmDevice *pDevice, bool alphablending,
								   bool lighting, int startLevel, int nLevels)
{
	int sz = m_texture->size();

	// do only one pass if the app has disabled multitexturing
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	if (pApp && !pApp->GetOptions(eAnmOptionsMultiTexturing))
		sz = 1;

	int endLevel = startLevel + nLevels;
	if (endLevel > sz)
		endLevel = sz;

	int i, l;
	for (i = 0, l = startLevel; l < endLevel; i++, l++)
	{
		CAnmTexture *pTexture = GetTextureChild(l);
		if (pTexture)
		{
			// only use passed-in alpha blending flag for base pass;
			// otherwise alpha blending values are explicit in stages?!
			//if (l == 0)
			//	pTexture->SetToDevice(pDevice, alphablending);
			//else
				SetStageToDevice(pDevice, pTexture, i, l, alphablending, lighting);
		}
	}
}

void CAnmMultiTexture::SetStageToDevice(class CAnmDevice *pDevice, CAnmTexture *pTexture,
										int stage, int level, bool alphablending, bool lighting)
{
	assert(pTexture);

	CAnmGraphics::Texture *pTextureSurface = pTexture->GetSurface();

	if (pTextureSurface)
	{
		pDevice->SetTexture(stage, pTextureSurface);
	
		// set some states
		eAnmTextureAddressMode addressU;
		eAnmTextureAddressMode addressV;
		
		// addressing modes (wrap or clamp)
		if (m_repeatS)
			addressU = eAnmTextureAddressWrap;
		else
			addressU = eAnmTextureAddressClamp;

		if (m_repeatT)
			addressV = eAnmTextureAddressWrap;
		else
			addressV = eAnmTextureAddressClamp;

		pDevice->SetTextureOption(stage, eAnmTextureAddressU, addressU);
		pDevice->SetTextureOption(stage, eAnmTextureAddressV, addressV);

		// for now
		pDevice->SetTextureOption(stage, eAnmTextureColorArg1, eAnmTextureArgCurrent);
		pDevice->SetTextureOption(stage, eAnmTextureColorArg2, eAnmTextureArgTexture);

		pDevice->SetTextureOption(stage, eAnmTextureColorOp, m_modes[level]);

		// N.B.: do some texture filtering (should this be settable as an option?)
		int width = pTextureSurface->GetWidth();
		int height = pTextureSurface->GetHeight();
		// a bit bogus right now...
		// pDevice->SetTextureOption(stage, eAnmTextureFilter, (width * height > ANMTEXTURE_MINSIZEFORFILTER));
		pDevice->SetTextureOption(stage, eAnmTextureFilter, TRUE);

		// 2D Texture coordinates for now, thank you
		// N.B.: this logic properly belongs with any geometry that uses ->
		// TextureCoordinate nodes because those are where the actual ->
		// coordinate space is defined...
		pDevice->SetTextureOption(stage, eAnmTextureNumTextureCoordinates, 
			( pTexture->GetTexCoordVectorSize() == 3 ) ? eAnmTextureCount3 : eAnmTextureCount2 );

		
		// set alpha ops if we're blending with previous stage
		if (alphablending && m_alphamodes[level] != eAnmTextureModeOFF)
		{
			pDevice->SetTextureOption(stage, eAnmTextureAlphaOp, m_alphamodes[level]);
			pDevice->SetTextureOption(stage, eAnmTextureAlphaArg1, eAnmTextureArgCurrent);	
			pDevice->SetTextureOption(stage, eAnmTextureAlphaArg2, eAnmTextureArgTexture);	
		}
	}
	else
	{
		pDevice->SetTexture(stage, NULL);
	}
}

int CAnmMultiTexture::NumLevels()
{
	// N.B.: might want to bolster this for NULL or non-texture children at some point
	return m_texture->size();
}

CAnmTexture *CAnmMultiTexture::GetTextureChild(int index)
{
	assert(index >= 0 && index < m_texture->size());

	CAnmNode *pChild = (*m_texture)[index];
	if (pChild)
	{
		CAnmTexture *pTexture = GETIMPLEMENTATION(CAnmTexture, pChild);
		return pTexture;
	}
	else
		return NULL;
}

eAnmTextureMode CAnmMultiTexture::String2Mode(const char *modename)
{
	if (!modename)
		return eAnmTextureModeBADMODE;

	eAnmTextureMode mode = eAnmTextureModeBADMODE;

	if (!strcmp(modename, "MODULATE"))
		mode = eAnmTextureModeMODULATE;
	else if (!strcmp(modename, "REPLACE"))
		mode = eAnmTextureModeREPLACE;
	else if (!strcmp(modename, "MODULATE2X"))
		mode = eAnmTextureModeMODULATE2X;
	else if (!strcmp(modename, "MODULATE4X"))
		mode = eAnmTextureModeMODULATE4X;
	else if (!strcmp(modename, "ADD"))
		mode = eAnmTextureModeADD;
	else if (!strcmp(modename, "ADDSIGNED"))
		mode = eAnmTextureModeADDSIGNED;
	else if (!strcmp(modename, "ADDSIGNED2X "))
		mode = eAnmTextureModeADDSIGNED2X;
	else if (!strcmp(modename, "SUBTRACT"))
		mode = eAnmTextureModeSUBTRACT;
	else if (!strcmp(modename, "ADDSMOOTH "))
		mode = eAnmTextureModeADDSMOOTH;
	else if (!strcmp(modename, "BLENDDIFFUSEALPHA"))
		mode = eAnmTextureModeBLENDDIFFUSEALPHA;
	else if (!strcmp(modename, "BLENDTEXTUREALPHA"))
		mode = eAnmTextureModeBLENDTEXTUREALPHA;
	else if (!strcmp(modename, "BLENDFACTORALPHA"))
		mode = eAnmTextureModeBLENDFACTORALPHA;
	else if (!strcmp(modename, "BLENDCURRENTALPHA"))
		mode = eAnmTextureModeBLENDCURRENTALPHA;
	else if (!strcmp(modename, "MODULATEALPHA_ADDCOLOR"))
		mode = eAnmTextureModeMODULATEALPHA_ADDCOLOR;
	else if (!strcmp(modename, "MODULATEINVALPHA_ADDCOLOR"))
		mode = eAnmTextureModeMODULATEINVALPHA_ADDCOLOR;
	else if (!strcmp(modename, "MODULATEINVCOLOR_ADDALPHA"))
		mode = eAnmTextureModeMODULATEINVCOLOR_ADDALPHA;
	else if (!strcmp(modename, "OFF"))
		mode = eAnmTextureModeOFF;
	else if (!strcmp(modename, "SELECTARG1"))
		mode = eAnmTextureModeSELECTARG1;
	else if (!strcmp(modename, "SELECTARG2"))
		mode = eAnmTextureModeSELECTARG2;
	else if (!strcmp(modename, "DOTPRODUCT3"))
		mode = eAnmTextureModeDOTPRODUCT3;

	return mode;
}

eAnmTextureSource CAnmMultiTexture::String2Source(const char *sourcename)
{
	if (!sourcename)
		return eAnmTextureSourceBADSOURCE;

	eAnmTextureSource source = eAnmTextureSourceBADSOURCE;

	if (!strlen(sourcename) || !strcmp(sourcename, "CURRENT"))
		source = eAnmTextureSourceCURRENT;
	else if (!strcmp(sourcename, "DIFFUSE"))
		source = eAnmTextureSourceDIFFUSE;
	else if (!strcmp(sourcename, "SPECULAR"))
		source = eAnmTextureSourceSPECULAR;
	else if (!strcmp(sourcename, "FACTOR"))
		source = eAnmTextureSourceFACTOR;

	return source;
}


eAnmTextureFunction CAnmMultiTexture::String2Function(const char *fnname)
{
	if (!fnname)
		return eAnmTextureFunctionBADFUNCTION;

	eAnmTextureFunction func = eAnmTextureFunctionBADFUNCTION;

	if (!strlen(fnname) || !strcmp(fnname, "NONE"))
		func = eAnmTextureFunctionNONE;
	else if (!strcmp(fnname, "COMPLEMENT"))
		func = eAnmTextureFunctionCOMPLEMENT;
	else if (!strcmp(fnname, "ALPHAREPLICATE"))
		func = eAnmTextureFunctionALPHAREPLICATE;

	return func;
}


// Accessors
void CAnmMultiTexture::SetAlpha(Float alpha)
{
	m_alpha = alpha;

	SetDirtyBits(eAnmMTAlphaDirty);
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMultiTexture, alpha_changed), &alpha);
}

void CAnmMultiTexture::SetColor(Color color)
{
	m_color = color;

	SetDirtyBits(eAnmMTColorDirty);
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMultiTexture, color_changed), &color);
}

void CAnmMultiTexture::SetFunction(StringArray *pFunction)
{
	assert(pFunction != NULL);

	SafeUnRef(m_function);
	m_function = pFunction;
	m_function->Ref();

	SetDirtyBits(eAnmMTFunctionDirty);
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMultiTexture, function_changed), &pFunction);
}

void CAnmMultiTexture::SetMode(StringArray *pMode)
{
	assert(pMode != NULL);

	SafeUnRef(m_mode);
	m_mode = pMode;
	m_mode->Ref();

	SetDirtyBits(eAnmMTModeDirty);
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMultiTexture, mode_changed), &pMode);
}

void CAnmMultiTexture::SetSource(StringArray *pSource)
{
	assert(pSource != NULL);

	SafeUnRef(m_source);
	m_source = pSource;
	m_source->Ref();

	SetDirtyBits(eAnmMTSourceDirty);
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMultiTexture, source_changed), &pSource);
}

void CAnmMultiTexture::SetTexture(NodeArray *pTexture)
{
	assert(pTexture != NULL);

	SafeUnRef(m_texture);
	m_texture = pTexture;
	m_texture->Ref();

	CAnmGroup::SetChildren(pTexture);

	SetDirtyBits(eAnmMTTextureDirty);
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMultiTexture, texture_changed), &pTexture);
}

void CAnmMultiTexture::SetTransparent(Boolean transparent)
{
	m_transparent = transparent;

	SetDirtyBits(eAnmMTTransparentDirty);
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMultiTexture, transparent_changed), &transparent);
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmMultiTexture, alpha, eValueFloat, Float, GetAlpha, SetAlpha);
DEFINE_VALUE(CAnmMultiTexture, color, eValueColor, Color, GetColor, SetColor);
DEFINE_VALUE(CAnmMultiTexture, function, eValueStringArray, StringArray*, GetFunction, SetFunction);
DEFINE_VALUE(CAnmMultiTexture, mode, eValueStringArray, StringArray*, GetMode, SetMode);
DEFINE_VALUE(CAnmMultiTexture, source, eValueStringArray, StringArray*, GetSource, SetSource);
DEFINE_VALUE(CAnmMultiTexture, texture, eValueNodeArray, NodeArray*, GetTexture, SetTexture);
DEFINE_VALUE(CAnmMultiTexture, transparent, eValueBoolean, Boolean, GetTransparent, SetTransparent);

DEFINE_METHOD(CAnmMultiTexture, set_alpha, eValueFloat, Float, SetAlpha);
DEFINE_METHOD(CAnmMultiTexture, set_color, eValueColor, Color, SetColor);
DEFINE_METHOD(CAnmMultiTexture, set_function, eValueStringArray, StringArray*, SetFunction);
DEFINE_METHOD(CAnmMultiTexture, set_mode, eValueStringArray, StringArray*, SetMode);
DEFINE_METHOD(CAnmMultiTexture, set_source, eValueStringArray, StringArray*, SetSource);
DEFINE_METHOD(CAnmMultiTexture, set_texture, eValueNodeArray, NodeArray*, SetTexture);
DEFINE_METHOD(CAnmMultiTexture, set_transparent, eValueBoolean, Boolean, SetTransparent);

DEFINE_CALLBACK(CAnmMultiTexture, alpha_changed, eValueFloat);
DEFINE_CALLBACK(CAnmMultiTexture, color_changed, eValueColor);
DEFINE_CALLBACK(CAnmMultiTexture, function_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmMultiTexture, mode_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmMultiTexture, source_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmMultiTexture, texture_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmMultiTexture, transparent_changed, eValueBoolean);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmMultiTexture)
VALUEID(CAnmMultiTexture, alpha),
VALUEID(CAnmMultiTexture, color),
VALUEID(CAnmMultiTexture, function),
VALUEID(CAnmMultiTexture, mode),
VALUEID(CAnmMultiTexture, source),
VALUEID(CAnmMultiTexture, texture),
VALUEID(CAnmMultiTexture, transparent),

METHODID(CAnmMultiTexture, set_alpha),
METHODID(CAnmMultiTexture, set_color),
METHODID(CAnmMultiTexture, set_function),
METHODID(CAnmMultiTexture, set_mode),
METHODID(CAnmMultiTexture, set_source),
METHODID(CAnmMultiTexture, set_texture),
METHODID(CAnmMultiTexture, set_transparent),

CALLBACKID(CAnmMultiTexture, alpha_changed),
CALLBACKID(CAnmMultiTexture, color_changed),
CALLBACKID(CAnmMultiTexture, function_changed),
CALLBACKID(CAnmMultiTexture, mode_changed),
CALLBACKID(CAnmMultiTexture, source_changed),
CALLBACKID(CAnmMultiTexture, texture_changed),
CALLBACKID(CAnmMultiTexture, transparent_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmMultiTexture, CAnmTexture);

