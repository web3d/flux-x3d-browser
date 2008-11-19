/********************************************************************************
 * Flux
 *
 * File: anmlight.cpp
 * Description: Base light class
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

#include "anmlight.h"
#include "anmdevice.h"
#include "anmupdatevisitor.h"
#include "anmworld.h"

CAnmLight::CAnmLight(eAnmLightType lightType,
	Color color, Point3 direction, Point3 location, Float intensity, Boolean on, Float ambientIntensity)
 : CAnmNode(),
 m_color(color),
 m_direction(direction),
 m_location(location),
 m_intensity(intensity),
 m_on(on),
 m_ambientIntensity(ambientIntensity),
 m_lightType(lightType)
{
}


CAnmLight::~CAnmLight()
{
}


void CAnmLight::SetAmbientIntensity(Float ambientIntensity)
{
	m_ambientIntensity = ambientIntensity;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmLight, ambientIntensity_changed), &ambientIntensity);
}

void CAnmLight::SetColor(Color color)
{
	m_color = color;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmLight, color_changed), &color);

}


void CAnmLight::SetDirection(Point3 direction)
{
	m_direction = direction;

	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmLight, direction_changed), &direction);
}

void CAnmLight::SetLocation(Point3 location)
{
	m_location = location;

	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmLight, location_changed), &location);
}

void CAnmLight::SetIntensity(Float intensity)
{
	m_intensity = intensity;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmLight, intensity_changed), &intensity);

}


void CAnmLight::SetOn(Boolean on)
{
	m_on = on;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmLight, on_changed), &on);

}


void CAnmLight::Realize()
{
	CAnmWorld *pWorld= GetWorld();
	assert (pWorld);

	CAnmNode::Realize();

	SetStateDirty();		// trigger initial update


}

void CAnmLight::Update()
{
	// Copy new values to all our instances; only do it if we're not
	// rebuilding everything anyway
	if (!TestDirtyBits(eAnmNodeMatrixDirty | eAnmNodeInstanceDirty))
	{
		CopyToInstances();
	}

	ClearStateDirty();
}

void CAnmLight::Traverse(CAnmUpdateVisitor *pVisitor)
{
	CAnmLightInstance *pLightInstance = NULL;
	
	if (TestDirtyBits(eAnmNodeInstanceDirty))
		pLightInstance = NextLightInstance(pVisitor->GetNodePath());
	else
		pLightInstance = FindLightInstance(pVisitor->GetNodePath());

	// If the platform light hasn't been created before, do it now
	CAnmGraphics::Light *pLight = pLightInstance->GetLight();
	if (pLight == NULL)
	{
		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL

		cApplication *pApp = pWorld->GetApp();
		assert(pApp);

		pLightInstance->CreateLight(pApp, m_lightType);
	}

	// If this is a new light object, alloc ID from global pool
	int index = pLightInstance->GetLightIndex();
	if (index == ANMLIGHTINDEX_BADINDEX)
	{
		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL
		index = pWorld->AllocLight();
		pLightInstance->SetLightIndex(index);
	}

	// Scoped light thing; if this is a directional light add to the current
	// traversal group; for points and spots, add to the global list
	CAnmLightGroup *pLightGroup;
	if (m_lightType == eAnmLightDirectional)
		pLightGroup = pVisitor->GetCurrentLightGroup();
	else
		pLightGroup = pVisitor->GetGlobalLightGroup();

	// Only add the light if the hierarchy is getting rebuilt
	if (TestDirtyBits(eAnmNodeInstanceDirty))
	{
		// Gotta set the type because of constructor screwaround
		pLightInstance->SetLightType(m_lightType);

		pLightGroup->AddLight(pLightInstance);
	}

	// Copy new values in
	CopyValuesToInstance(pLightInstance);

	// Transform it; N.B.: we're only in this method if there was a dirty matrix ->
	// or instance somewhere; otherwise this is handled in CopyToInstances during ->
	// the PreTraverse step
	matrix4 wmat;
	pVisitor->GetWorldMatrix(&wmat);

	TransformInstance(pLightInstance, wmat);

}

void CAnmLight::CopyValuesToInstance(CAnmLightInstance *pLightInstance)
{
	CAnmGraphics::Light *pLight = pLightInstance->GetLight();
	assert(pLight);

	// Copy color values into graphics list
	// N.B.: need to convert to using Color4 everywhere

	color4 diffuseColor = m_color * m_intensity;
	color4 specularColor = color4::White;
	color4 ambientColor = m_color * m_ambientIntensity;

	pLight->SetDiffuseColor(&diffuseColor);
	pLight->SetSpecularColor(&specularColor);
	pLight->SetAmbientColor(&ambientColor);

	// Set on/off flag in instance object
	pLightInstance->SetOn(m_on);

}

void CAnmLight::TransformInstance(CAnmLightInstance *pLightInstance, matrix4 &wmat)
{
	CAnmGraphics::Light *pLight = pLightInstance->GetLight();

	// put my position and direction vectors into world coordinates
	Point3 pos = wmat * m_location;

	// zero out translation values to get proper direction
	Point3 origin = wmat * Point3(0, 0, 0);
	Point3 dir = wmat * m_direction;
	dir = dir - origin;
	if (dir.Mag())
		dir.Normalize();

	pLight->SetDirection(&dir);
	pLight->SetLocation(&pos);
}

void CAnmLight::CopyToInstances()
{
	CAnmLightInstance *pLightInstance = NULL;

	int sz = NumInstances();
	for (int i = 0; i < sz; i++)
	{
		pLightInstance = GetLightInstance(i);
		assert(pLightInstance);

		CopyValuesToInstance(pLightInstance);
	}
}

bool CAnmLight::m_showLights = true;

void CAnmLight::SetShowLights(bool showLights)
{
	m_showLights = showLights;
}

bool CAnmLight::GetShowLights()
{
	return m_showLights;
}


// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmLight, ambientIntensity, eValueFloat, Float, GetAmbientIntensity, SetAmbientIntensity);
DEFINE_VALUE(CAnmLight, color, eValueColor, Color, GetColor, SetColor);
DEFINE_VALUE(CAnmLight, direction, eValuePoint3, Point3, GetDirection, SetDirection);
DEFINE_VALUE(CAnmLight, intensity, eValueFloat, Float, GetIntensity, SetIntensity);
DEFINE_VALUE(CAnmLight, location, eValuePoint3, Point3, GetLocation, SetLocation);
DEFINE_VALUE(CAnmLight, on, eValueBoolean, Boolean, GetOn, SetOn);

DEFINE_METHOD(CAnmLight, set_ambientIntensity, eValueFloat, Float, SetAmbientIntensity);
DEFINE_METHOD(CAnmLight, set_color, eValueColor, Color, SetColor);
DEFINE_METHOD(CAnmLight, set_direction, eValuePoint3, Point3, SetDirection);
DEFINE_METHOD(CAnmLight, set_intensity, eValueFloat, Float, SetIntensity);
DEFINE_METHOD(CAnmLight, set_location, eValuePoint3, Point3, SetLocation);
DEFINE_METHOD(CAnmLight, set_on, eValueBoolean, Boolean, SetOn);

DEFINE_CALLBACK(CAnmLight, ambientIntensity_changed, eValueFloat);
DEFINE_CALLBACK(CAnmLight, color_changed, eValueColor);
DEFINE_CALLBACK(CAnmLight, direction_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmLight, intensity_changed, eValueFloat);
DEFINE_CALLBACK(CAnmLight, location_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmLight, on_changed, eValueBoolean);

// member table


BEGIN_DEFINE_MEMBERS(CAnmLight)
VALUEID(CAnmLight, ambientIntensity),
VALUEID(CAnmLight, color),
VALUEID(CAnmLight, direction),
VALUEID(CAnmLight, intensity),
VALUEID(CAnmLight, location),
VALUEID(CAnmLight, on),

METHODID(CAnmLight, set_ambientIntensity),
METHODID(CAnmLight, set_color),
METHODID(CAnmLight, set_direction),
METHODID(CAnmLight, set_intensity),
METHODID(CAnmLight, set_location),
METHODID(CAnmLight, set_on),

CALLBACKID(CAnmLight, ambientIntensity_changed),
CALLBACKID(CAnmLight, color_changed),
CALLBACKID(CAnmLight, direction_changed),
CALLBACKID(CAnmLight, intensity_changed),
CALLBACKID(CAnmLight, location_changed),
CALLBACKID(CAnmLight, on_changed),

END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmLight, CAnmNode);


