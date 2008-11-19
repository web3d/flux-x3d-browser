/********************************************************************************
 * Flux
 *
 * File: anmdirectionallight.cpp
 * Description: Directional light class
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

#include "anmdirectionallight.h"
#include "anmdevice.h"
#include "anmupdatevisitor.h"
#include "anmworld.h"

#define ANMDIRLIGHT_NOLOCATION Point3(0, 0, 0)


CAnmDirectionalLight::CAnmDirectionalLight()
 : CAnmLight(eAnmLightDirectional,
 ANMLIGHT_DEFAULT_COLOR, ANMLIGHT_DEFAULT_DIRECTION, ANMDIRLIGHT_NOLOCATION, ANMLIGHT_DEFAULT_INTENSITY, ANMLIGHT_DEFAULT_ON, 
 ANMLIGHT_DEFAULT_AMBIENTINTENSITY)
{
}

CAnmDirectionalLight::~CAnmDirectionalLight()
{
}

void CAnmDirectionalLight::Realize()
{
	CAnmLight::Realize();
}

void CAnmDirectionalLight::Update()
{
	CAnmLight::Update();
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmDirectionalLight, ambientIntensity, CAnmLight);
DEFINE_INHERITED_VALUE(CAnmDirectionalLight, color, CAnmLight);
DEFINE_INHERITED_VALUE(CAnmDirectionalLight, direction, CAnmLight);
DEFINE_INHERITED_VALUE(CAnmDirectionalLight, intensity, CAnmLight);
DEFINE_INHERITED_VALUE(CAnmDirectionalLight, on, CAnmLight);

DEFINE_INHERITED_METHOD(CAnmDirectionalLight, set_ambientIntensity, CAnmLight);
DEFINE_INHERITED_METHOD(CAnmDirectionalLight, set_color, CAnmLight);
DEFINE_INHERITED_METHOD(CAnmDirectionalLight, set_direction, CAnmLight);
DEFINE_INHERITED_METHOD(CAnmDirectionalLight, set_intensity, CAnmLight);
DEFINE_INHERITED_METHOD(CAnmDirectionalLight, set_on, CAnmLight);

DEFINE_INHERITED_CALLBACK(CAnmDirectionalLight, ambientIntensity_changed, CAnmLight);
DEFINE_INHERITED_CALLBACK(CAnmDirectionalLight, color_changed, CAnmLight);
DEFINE_INHERITED_CALLBACK(CAnmDirectionalLight, direction_changed, CAnmLight);
DEFINE_INHERITED_CALLBACK(CAnmDirectionalLight, intensity_changed, CAnmLight);
DEFINE_INHERITED_CALLBACK(CAnmDirectionalLight, on_changed, CAnmLight);

// member table


BEGIN_DEFINE_NODE_MEMBERS(CAnmDirectionalLight)
VALUEID(CAnmDirectionalLight, ambientIntensity),
VALUEID(CAnmDirectionalLight, color),
VALUEID(CAnmDirectionalLight, direction),
VALUEID(CAnmDirectionalLight, intensity),
VALUEID(CAnmDirectionalLight, on),

METHODID(CAnmDirectionalLight, set_ambientIntensity),
METHODID(CAnmDirectionalLight, set_color),
METHODID(CAnmDirectionalLight, set_direction),
METHODID(CAnmDirectionalLight, set_intensity),
METHODID(CAnmDirectionalLight, set_on),

CALLBACKID(CAnmDirectionalLight, ambientIntensity_changed),
CALLBACKID(CAnmDirectionalLight, color_changed),
CALLBACKID(CAnmDirectionalLight, direction_changed),
CALLBACKID(CAnmDirectionalLight, intensity_changed),
CALLBACKID(CAnmDirectionalLight, on_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmDirectionalLight, CAnmLight);
