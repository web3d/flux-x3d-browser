/********************************************************************************
 * Flux
 *
 * File: anmmaterial.cpp
 * Description: Material node
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

#include "anmmaterial.h"
#include "anmshape.h"
#include "anmgeometry.h"
#include "anmappearance.h"
#include "anmworld.h"

CAnmMaterial::CAnmMaterial() : 

	CAnmNode(),						// Parent Appearance will add me
	m_diffuseColor(ANMMATERIAL_DEFAULT_DIFFUSECOLOR),
	m_emissiveColor(ANMMATERIAL_DEFAULT_EMISSIVECOLOR),
	m_specularColor(ANMMATERIAL_DEFAULT_SPECULARCOLOR),
	m_shininess(ANMMATERIAL_DEFAULT_SHININESS),
	m_transparency(ANMMATERIAL_DEFAULT_TRANSPARENCY),
	m_ambientIntensity(ANMMATERIAL_DEFAULT_AMBIENTINTENSITY)	
{
	m_material = NULL;
	m_materialWhiteDiffuse = NULL;
}

CAnmMaterial::~CAnmMaterial()
{
	SafeDelete(m_material);
	SafeDelete(m_materialWhiteDiffuse);
}

// some helper methods
#define BASE_SHININESS 1.0f		// according to RM documentation anyway; ick

// calculate RM material power based on shininess value
Float CAnmMaterial::CalcPower(Float shininess)
{
	return BASE_SHININESS + (shininess * 32.f);
}

// calculate D3D alpha value based on our transparency value; basically
// it's the opposite
Float CAnmMaterial::CalcAlpha(Float transparency)
{
	return 1.0f - transparency;
}

// CAnmNode overrides
void CAnmMaterial::Realize()
{
	CAnmNode::Realize();


	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmGraphics *pGraphics = pApp->GetGraphics();
	assert(pGraphics);

	m_material = pGraphics->CreateMaterial();

	SetStateDirty();

	Update();
}

void CAnmMaterial::Update()
{
	// N.B.: the parent Appearance checks me for dirty before calling this;
	// don't test again

	// we just do all the fields if we're dirty; why test flags?-- this is cheap
	if (m_realized)
	{
		Float power = CalcPower(m_shininess);
		Float alpha = CalcAlpha(m_transparency);

		color4 diffuseColor = color4(m_diffuseColor, alpha);
		color4 specularColor = m_specularColor;
		color4 emissiveColor = m_emissiveColor;
		color4 ambientColor = m_diffuseColor * m_ambientIntensity;

		if( m_material == NULL ) {
			CAnmWorld *pWorld = GetWorld();
		//	assert(pWorld);
			ABORT_IF_WORLD_IS_NULL

			cApplication *pApp = pWorld->GetApp();
			assert(pApp);
			CAnmGraphics *pGraphics = pApp->GetGraphics();
			assert(pGraphics);
			m_material = pGraphics->CreateMaterial();
		}


		m_material->SetDiffuseColor(&diffuseColor);
		m_material->SetSpecularColor(&specularColor);
		m_material->SetEmissiveColor(&emissiveColor);
		m_material->SetAmbientColor(&ambientColor);
		m_material->SetPower(power);
	}

	ClearStateDirty();
}

// Return a material equivalent to our current values, but with white diffuse color
CAnmGraphics::Material *CAnmMaterial::GetRawMaterialWhiteDiffuse()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	if (m_materialWhiteDiffuse == NULL)
	{
		CAnmGraphics *pGraphics = pApp->GetGraphics();
		assert(pGraphics);

		m_materialWhiteDiffuse = pGraphics->CreateMaterial();
	}

	m_materialWhiteDiffuse->Copy(m_material);

	color4 whiteDiffuse = color4::White;
	whiteDiffuse.a = CalcAlpha(m_transparency);
	m_materialWhiteDiffuse->SetDiffuseColor(&whiteDiffuse);

	return m_materialWhiteDiffuse;
}

// Accessors
void CAnmMaterial::SetAmbientIntensity(Float ambientIntensity)
{
	m_ambientIntensity = ClampValue(0.f, ambientIntensity, 1.f);

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMaterial, ambientIntensity_changed), &ambientIntensity);
}

void CAnmMaterial::SetDiffuseColor(Color diffuseColor)
{
	m_diffuseColor = diffuseColor;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMaterial, diffuseColor_changed), &diffuseColor);
}

void CAnmMaterial::SetEmissiveColor(Color emissiveColor)
{
	m_emissiveColor = emissiveColor;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMaterial, emissiveColor_changed), &emissiveColor);
}

void CAnmMaterial::SetShininess(Float shininess)
{
	m_shininess = ClampValue(0.f, shininess, 1.f);

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMaterial, shininess_changed), &shininess);
}

void CAnmMaterial::SetSpecularColor(Color specularColor)
{
	m_specularColor = specularColor;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMaterial, specularColor_changed), &specularColor);
}

void CAnmMaterial::SetTransparency(Float transparency)
{
	m_transparency = ClampValue(0.f, transparency, 1.f);

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMaterial, transparency_changed), &transparency);
}



// Tables for Callbacks, Methods and Set/GetValues


DEFINE_VALUE(CAnmMaterial, ambientIntensity, eValueFloat, Float, GetAmbientIntensity, SetAmbientIntensity);
DEFINE_VALUE(CAnmMaterial, diffuseColor, eValueColor, Color, GetDiffuseColor, SetDiffuseColor);
DEFINE_VALUE(CAnmMaterial, emissiveColor, eValueColor, Color, GetEmissiveColor, SetEmissiveColor);
DEFINE_VALUE(CAnmMaterial, shininess, eValueFloat, Float, GetShininess, SetShininess);
DEFINE_VALUE(CAnmMaterial, specularColor, eValueColor, Color, GetSpecularColor, SetSpecularColor);
DEFINE_VALUE(CAnmMaterial, transparency, eValueFloat, Float, GetTransparency, SetTransparency);

DEFINE_METHOD(CAnmMaterial, set_ambientIntensity, eValueFloat, Float, SetAmbientIntensity);
DEFINE_METHOD(CAnmMaterial, set_diffuseColor, eValueColor, Color, SetDiffuseColor);
DEFINE_METHOD(CAnmMaterial, set_emissiveColor, eValueColor, Color, SetEmissiveColor);
DEFINE_METHOD(CAnmMaterial, set_shininess, eValueFloat, Float, SetShininess);
DEFINE_METHOD(CAnmMaterial, set_specularColor, eValueColor, Color, SetSpecularColor);
DEFINE_METHOD(CAnmMaterial, set_transparency, eValueFloat, Float, SetTransparency);

DEFINE_CALLBACK(CAnmMaterial, ambientIntensity_changed, eValueFloat);
DEFINE_CALLBACK(CAnmMaterial, diffuseColor_changed, eValueColor);
DEFINE_CALLBACK(CAnmMaterial, emissiveColor_changed, eValueColor);
DEFINE_CALLBACK(CAnmMaterial, shininess_changed, eValueFloat);
DEFINE_CALLBACK(CAnmMaterial, specularColor_changed, eValueColor);
DEFINE_CALLBACK(CAnmMaterial, transparency_changed, eValueFloat);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmMaterial)
VALUEID(CAnmMaterial, ambientIntensity),
VALUEID(CAnmMaterial, diffuseColor),
VALUEID(CAnmMaterial, emissiveColor),
VALUEID(CAnmMaterial, shininess),
VALUEID(CAnmMaterial, specularColor),
VALUEID(CAnmMaterial, transparency),

METHODID(CAnmMaterial, set_ambientIntensity),
METHODID(CAnmMaterial, set_diffuseColor),
METHODID(CAnmMaterial, set_emissiveColor),
METHODID(CAnmMaterial, set_shininess),
METHODID(CAnmMaterial, set_specularColor),
METHODID(CAnmMaterial, set_transparency),

CALLBACKID(CAnmMaterial, ambientIntensity_changed),
CALLBACKID(CAnmMaterial, diffuseColor_changed),
CALLBACKID(CAnmMaterial, emissiveColor_changed),
CALLBACKID(CAnmMaterial, shininess_changed),
CALLBACKID(CAnmMaterial, specularColor_changed),
CALLBACKID(CAnmMaterial, transparency_changed),


END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmMaterial, CAnmNode);

