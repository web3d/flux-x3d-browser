/********************************************************************************
 * Flux
 *
 * File: anmmaterial.h
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

#ifndef _anmmaterial_h
#define _anmmaterial_h

#include "anmnode.h"

#define ANMMATERIAL_DEFAULT_AMBIENTINTENSITY 0.2f
#define ANMMATERIAL_DEFAULT_DIFFUSECOLOR Color(0.8f, 0.8f, 0.8f)
#define ANMMATERIAL_DEFAULT_EMISSIVECOLOR Color(0.f, 0.f, 0.f)
#define ANMMATERIAL_DEFAULT_SHININESS 0.2f
#define ANMMATERIAL_DEFAULT_SPECULARCOLOR Color(0.f, 0.f, 0.f)
#define ANMMATERIAL_DEFAULT_TRANSPARENCY 0.f

class CAnmMaterial : public CAnmNode
{

protected:
	
	CAnmGraphics::Material		*m_material;				// native rendering structure
	CAnmGraphics::Material		*m_materialWhiteDiffuse;	// help with lit texture limitations

	Float				 m_ambientIntensity;
	Color				 m_diffuseColor;
	Color				 m_emissiveColor;
	Float				 m_shininess;
	Color				 m_specularColor;
	Float				 m_transparency;

public:

	// constructor/destructor
	CAnmMaterial();
	virtual ~CAnmMaterial();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	// New Methods
	Float	CalcPower(Float shininess);		// convert shininess to D3D specular power
	Float	CalcAlpha(Float transparency);	// convert transparency to D3D alpha value
	Float	GetAlpha()
	{
		return CalcAlpha(m_transparency);
	}

	CAnmGraphics::Material *GetRawMaterialWhiteDiffuse();

	// Accessors
	CAnmGraphics::Material *GetRawMaterial()
	{
		return m_material;
	}

	void SetAmbientIntensity(Float ambientIntensity);
	Float GetAmbientIntensity()
	{
		return m_ambientIntensity;
	}
	void GetAmbientIntensity(Float *pVal)
	{
		assert(pVal);
		*pVal = m_ambientIntensity;
	}

	void SetDiffuseColor(Color diffuseColor);
	Color GetDiffuseColor()
	{
		return m_diffuseColor;
	}
	void GetDiffuseColor(Color *pVal)
	{
		assert(pVal);
		*pVal = m_diffuseColor;
	}

	void SetEmissiveColor(Color emissiveColor);
	Color GetEmissiveColor()
	{
		return m_emissiveColor;
	}
	void GetEmissiveColor(Color *pVal)
	{
		assert(pVal);
		*pVal = m_emissiveColor;
	}

	void SetShininess(Float shininess);
	Float GetShininess()
	{
		return m_shininess;
	}
	void GetShininess(Float *pVal)
	{
		assert(pVal);
		*pVal = m_shininess;
	}

	void SetSpecularColor(Color specularColor);
	Color GetSpecularColor()
	{
		return m_specularColor;
	}
	void GetSpecularColor(Color *pVal)
	{
		assert(pVal);
		*pVal = m_specularColor;
	}

	void SetTransparency(Float transparency);
	Float GetTransparency()
	{
		return m_transparency;
	}
	void GetTransparency(Float *pVal)
	{
		assert(pVal);
		*pVal = m_transparency;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmMaterial);

	DECLARE_VALUE(ambientIntensity);
	DECLARE_VALUE(diffuseColor);
	DECLARE_VALUE(emissiveColor);
	DECLARE_VALUE(shininess);
	DECLARE_VALUE(specularColor);
	DECLARE_VALUE(transparency);

	DECLARE_METHOD(set_ambientIntensity);
	DECLARE_METHOD(set_diffuseColor);
	DECLARE_METHOD(set_emissiveColor);
	DECLARE_METHOD(set_shininess);
	DECLARE_METHOD(set_specularColor);
	DECLARE_METHOD(set_transparency);

	DECLARE_CALLBACK(ambientIntensity_changed);
	DECLARE_CALLBACK(diffuseColor_changed);
	DECLARE_CALLBACK(emissiveColor_changed);
	DECLARE_CALLBACK(shininess_changed);
	DECLARE_CALLBACK(specularColor_changed);
	DECLARE_CALLBACK(transparency_changed);


};

DECLARE_GETIMPLEMENTATION(CAnmMaterial);

#endif // _anmmaterial_h
