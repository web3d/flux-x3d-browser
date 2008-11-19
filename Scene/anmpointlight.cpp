/********************************************************************************
 * Flux
 *
 * File: anmpointlight.cpp
 * Description: PointLight class
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

#include "anmpointlight.h"
#include "anmworld.h"

#define ANMPOINTLIGHT_NODIRECTION Point3(0, 0, 0)

CAnmPointLight::CAnmPointLight()
 : CAnmLight(eAnmLightPoint,
 ANMLIGHT_DEFAULT_COLOR, ANMPOINTLIGHT_NODIRECTION, ANMPOINTLIGHT_DEFAULT_LOCATION, ANMLIGHT_DEFAULT_INTENSITY, ANMLIGHT_DEFAULT_ON,
	ANMLIGHT_DEFAULT_AMBIENTINTENSITY),
 m_radius(ANMPOINTLIGHT_DEFAULT_RADIUS),
 m_attenuation(ANMPOINTLIGHT_DEFAULT_ATTENUATION)
{
}

CAnmPointLight::~CAnmPointLight()
{
}


void CAnmPointLight::SetAttenuation(Point3 attenuation)
{
	m_attenuation = attenuation;

	SetStateDirty();
}


void CAnmPointLight::SetRadius(Float radius)
{
	m_radius = radius;

	SetStateAndMatrixDirty();
}

void CAnmPointLight::Realize()
{
	CAnmLight::Realize();
}

void CAnmPointLight::CopyValuesToInstance(CAnmLightInstance *pLightInstance)
{
	CAnmGraphics::Light *pLight = pLightInstance->GetLight();

	// Set the attenuation value
	pLight->SetAttenuation(m_attenuation);

	// Call superclass to do the rest
	CAnmLight::CopyValuesToInstance(pLightInstance);
}

void CAnmPointLight::TransformInstance(CAnmLightInstance *pLightInstance, matrix4 &wmat)
{
	CAnmGraphics::Light *pLight = pLightInstance->GetLight();

	// Set the range
	// scale the radius to get range... somehow; just use Z scale factor for now
	float range = m_radius ; //  * fabs(wmat._33); N.B.: Flatout don't work; TP 07/24/04
	pLight->SetRange(range);

	// Call superclass to do the rest
	CAnmLight::TransformInstance(pLightInstance, wmat);
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmPointLight, ambientIntensity, CAnmLight);
DEFINE_INHERITED_VALUE(CAnmPointLight, color, CAnmLight);
DEFINE_INHERITED_VALUE(CAnmPointLight, direction, CAnmLight);
DEFINE_INHERITED_VALUE(CAnmPointLight, intensity, CAnmLight);
DEFINE_INHERITED_VALUE(CAnmPointLight, location, CAnmLight);
DEFINE_INHERITED_VALUE(CAnmPointLight, on, CAnmLight);
DEFINE_VALUE(CAnmPointLight, attenuation, eValuePoint3, Point3, GetAttenuation, SetAttenuation);
DEFINE_VALUE(CAnmPointLight, radius, eValueFloat, Float, GetRadius, SetRadius);


DEFINE_INHERITED_METHOD(CAnmPointLight, set_ambientIntensity, CAnmLight);
DEFINE_INHERITED_METHOD(CAnmPointLight, set_color, CAnmLight);
DEFINE_INHERITED_METHOD(CAnmPointLight, set_direction, CAnmLight);
DEFINE_INHERITED_METHOD(CAnmPointLight, set_intensity, CAnmLight);
DEFINE_INHERITED_METHOD(CAnmPointLight, set_location, CAnmLight);
DEFINE_INHERITED_METHOD(CAnmPointLight, set_on, CAnmLight);
DEFINE_METHOD(CAnmPointLight, set_attenuation, eValuePoint3, Point3, SetAttenuation);
DEFINE_METHOD(CAnmPointLight, set_radius, eValueFloat, Float, SetRadius);

DEFINE_INHERITED_CALLBACK(CAnmPointLight, ambientIntensity_changed, CAnmLight);
DEFINE_INHERITED_CALLBACK(CAnmPointLight, color_changed, CAnmLight);
DEFINE_INHERITED_CALLBACK(CAnmPointLight, direction_changed, CAnmLight);
DEFINE_INHERITED_CALLBACK(CAnmPointLight, intensity_changed, CAnmLight);
DEFINE_INHERITED_CALLBACK(CAnmPointLight, on_changed, CAnmLight);
DEFINE_INHERITED_CALLBACK(CAnmPointLight, location_changed, CAnmLight);
DEFINE_CALLBACK(CAnmPointLight, attenuation_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmPointLight, radius_changed, eValueFloat);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmPointLight)
VALUEID(CAnmPointLight, ambientIntensity),
VALUEID(CAnmPointLight, color),
VALUEID(CAnmPointLight, direction),
VALUEID(CAnmPointLight, intensity),
VALUEID(CAnmPointLight, on),
VALUEID(CAnmPointLight, location),
VALUEID(CAnmPointLight, attenuation),
VALUEID(CAnmPointLight, radius),

METHODID(CAnmPointLight, set_ambientIntensity),
METHODID(CAnmPointLight, set_color),
METHODID(CAnmPointLight, set_direction),
METHODID(CAnmPointLight, set_intensity),
METHODID(CAnmPointLight, set_on),
METHODID(CAnmPointLight, set_location),
METHODID(CAnmPointLight, set_attenuation),
METHODID(CAnmPointLight, set_radius),

CALLBACKID(CAnmPointLight, ambientIntensity_changed),
CALLBACKID(CAnmPointLight, color_changed),
CALLBACKID(CAnmPointLight, direction_changed),
CALLBACKID(CAnmPointLight, intensity_changed),
CALLBACKID(CAnmPointLight, on_changed),
CALLBACKID(CAnmPointLight, location_changed),
CALLBACKID(CAnmPointLight, attenuation_changed),
CALLBACKID(CAnmPointLight, radius_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmPointLight, CAnmLight);
