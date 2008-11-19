/********************************************************************************
 * Flux
 *
 * File: anmspotlight.cpp
 * Description: SpotLight class
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

#include "anmspotlight.h"
#include "anmworld.h"


CAnmSpotLight::CAnmSpotLight()
 : CAnmLight(eAnmLightSpot,
 ANMLIGHT_DEFAULT_COLOR, ANMLIGHT_DEFAULT_DIRECTION, ANMSPOTLIGHT_DEFAULT_LOCATION, ANMLIGHT_DEFAULT_INTENSITY, ANMLIGHT_DEFAULT_ON,
	ANMLIGHT_DEFAULT_AMBIENTINTENSITY),
 m_radius(ANMSPOTLIGHT_DEFAULT_RADIUS),
 m_attenuation(ANMSPOTLIGHT_DEFAULT_ATTENUATION),
 m_beamWidth(ANMSPOTLIGHT_DEFAULT_BEAMWIDTH),
 m_cutOffAngle(ANMSPOTLIGHT_DEFAULT_CUTOFFANGLE)
{
}

CAnmSpotLight::~CAnmSpotLight()
{
}


void CAnmSpotLight::SetAttenuation(Point3 attenuation)
{
	m_attenuation = attenuation;

	SetStateDirty();
}

void CAnmSpotLight::SetBeamWidth(Float beamWidth)
{
	m_beamWidth = beamWidth;

	SetStateDirty();
}

void CAnmSpotLight::SetCutOffAngle(Float cutOffAngle)
{
	m_cutOffAngle = cutOffAngle;

	SetStateDirty();
}


void CAnmSpotLight::SetRadius(Float radius)
{
	m_radius = radius;

	SetStateAndMatrixDirty();
}

void CAnmSpotLight::Realize()
{
	CAnmLight::Realize();
}

void CAnmSpotLight::CopyValuesToInstance(CAnmLightInstance *pLightInstance)
{
	CAnmGraphics::Light *pLight = pLightInstance->GetLight();

	// Set attenuation, theta, phi, falloff
	pLight->SetAttenuation(m_attenuation);

	// constrain things: beamWidth has to be <= cutOffAngle
	pLight->SetBeamWidth((m_beamWidth > m_cutOffAngle) ? m_cutOffAngle : m_beamWidth);
	pLight->SetCutoffAngle(m_cutOffAngle);

	// Call superclass to do the rest
	CAnmLight::CopyValuesToInstance(pLightInstance);
}

void CAnmSpotLight::TransformInstance(CAnmLightInstance *pLightInstance, matrix4 &wmat)
{
	CAnmGraphics::Light *pLight = pLightInstance->GetLight();

	// Set the range
	// scale the radius to get range... somehow; just use Z scale factor for now
	float range = m_radius  ; //  * fabs(wmat._33); N.B.: Flatout don't work; TP 07/24/04
	pLight->SetRange(range);

	// Call superclass to do the rest
	CAnmLight::TransformInstance(pLightInstance, wmat);
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmSpotLight, ambientIntensity, eValueFloat, Float, GetAmbientIntensity, SetAmbientIntensity);
DEFINE_VALUE(CAnmSpotLight, attenuation, eValuePoint3, Point3, GetAttenuation, SetAttenuation);
DEFINE_VALUE(CAnmSpotLight, beamWidth, eValueFloat, Float, GetBeamWidth, SetBeamWidth);
DEFINE_VALUE(CAnmSpotLight, color, eValueColor, Color, GetColor, SetColor);
DEFINE_VALUE(CAnmSpotLight, cutOffAngle, eValueFloat, Float, GetCutOffAngle, SetCutOffAngle);
DEFINE_VALUE(CAnmSpotLight, direction, eValuePoint3, Point3, GetDirection, SetDirection);
DEFINE_VALUE(CAnmSpotLight, intensity, eValueFloat, Float, GetIntensity, SetIntensity);
DEFINE_VALUE(CAnmSpotLight, location, eValuePoint3, Point3, GetLocation, SetLocation);
DEFINE_VALUE(CAnmSpotLight, on, eValueBoolean, Boolean, GetOn, SetOn);
DEFINE_VALUE(CAnmSpotLight, radius, eValueFloat, Float, GetRadius, SetRadius);


DEFINE_METHOD(CAnmSpotLight, set_ambientIntensity, eValueFloat, Float, SetAmbientIntensity);
DEFINE_METHOD(CAnmSpotLight, set_attenuation, eValuePoint3, Point3, SetAttenuation);
DEFINE_METHOD(CAnmSpotLight, set_beamWidth, eValueFloat, Float, SetBeamWidth);
DEFINE_METHOD(CAnmSpotLight, set_color, eValueColor, Color, SetColor);
DEFINE_METHOD(CAnmSpotLight, set_cutOffAngle, eValueFloat, Float, SetCutOffAngle);
DEFINE_METHOD(CAnmSpotLight, set_direction, eValuePoint3, Point3, SetDirection);
DEFINE_METHOD(CAnmSpotLight, set_intensity, eValueFloat, Float, SetIntensity);
DEFINE_METHOD(CAnmSpotLight, set_location, eValuePoint3, Point3, SetLocation);
DEFINE_METHOD(CAnmSpotLight, set_on, eValueBoolean, Boolean, SetOn);
DEFINE_METHOD(CAnmSpotLight, set_radius, eValueFloat, Float, SetRadius);

DEFINE_CALLBACK(CAnmSpotLight, ambientIntensity_changed, eValueFloat);
DEFINE_CALLBACK(CAnmSpotLight, attenuation_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmSpotLight, beamWidth_changed, eValueFloat);
DEFINE_CALLBACK(CAnmSpotLight, color_changed, eValueColor);
DEFINE_CALLBACK(CAnmSpotLight, cutOffAngle_changed, eValueFloat);
DEFINE_CALLBACK(CAnmSpotLight, direction_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmSpotLight, intensity_changed, eValueFloat);
DEFINE_CALLBACK(CAnmSpotLight, location_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmSpotLight, on_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmSpotLight, radius_changed, eValueFloat);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmSpotLight)
VALUEID(CAnmSpotLight, ambientIntensity),
VALUEID(CAnmSpotLight, attenuation),
VALUEID(CAnmSpotLight, beamWidth),
VALUEID(CAnmSpotLight, color),
VALUEID(CAnmSpotLight, cutOffAngle),
VALUEID(CAnmSpotLight, direction),
VALUEID(CAnmSpotLight, intensity),
VALUEID(CAnmSpotLight, location),
VALUEID(CAnmSpotLight, on),
VALUEID(CAnmSpotLight, radius),

METHODID(CAnmSpotLight, set_ambientIntensity),
METHODID(CAnmSpotLight, set_attenuation),
METHODID(CAnmSpotLight, set_beamWidth),
METHODID(CAnmSpotLight, set_color),
METHODID(CAnmSpotLight, set_cutOffAngle),
METHODID(CAnmSpotLight, set_direction),
METHODID(CAnmSpotLight, set_intensity),
METHODID(CAnmSpotLight, set_location),
METHODID(CAnmSpotLight, set_on),
METHODID(CAnmSpotLight, set_radius),

CALLBACKID(CAnmSpotLight, ambientIntensity_changed),
CALLBACKID(CAnmSpotLight, attenuation_changed),
CALLBACKID(CAnmSpotLight, beamWidth_changed),
CALLBACKID(CAnmSpotLight, color_changed),
CALLBACKID(CAnmSpotLight, cutOffAngle_changed),
CALLBACKID(CAnmSpotLight, direction_changed),
CALLBACKID(CAnmSpotLight, intensity_changed),
CALLBACKID(CAnmSpotLight, location_changed),
CALLBACKID(CAnmSpotLight, on_changed),
CALLBACKID(CAnmSpotLight, radius_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmSpotLight, CAnmLight);
