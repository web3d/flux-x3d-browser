/********************************************************************************
 * Flux
 *
 * File: anmnavigationinfo.cpp
 * Description: NavigationInfo node
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
#include "anmnavigationinfo.h"
#include "anmworld.h"

#include "anmdevice.h"
#include "anmviewport.h"
#include "anmcamera.h"

CAnmNavigationInfo::CAnmNavigationInfo() : 
	CAnmBindableNode()
{
	m_avatarSize = new FloatArray;
	m_transitionType = new StringArray;
	m_type = new StringArray;

	m_avatarSize->push_back(ANMNAVINFO_DEFAULT_AVATAR_COLLISION_DISTANCE);
	m_avatarSize->push_back(ANMNAVINFO_DEFAULT_AVATAR_HEIGHT_ABOVE_TERRAIN);
	m_avatarSize->push_back(ANMNAVINFO_DEFAULT_AVATAR_TALLEST_OBJECT_HEIGHT);

	m_type->push_back(new CAnmString("WALK"));
	m_type->push_back(new CAnmString("ANY"));

	m_transitionType->push_back(new CAnmString("LINEAR"));

	m_headlight = ANMNAVINFO_DEFAULT_HEADLIGHT;
	m_speed = ANMNAVINFO_DEFAULT_SPEED;
	m_visibilityLimit = ANMNAVINFO_DEFAULT_VISIBILITYLIMIT;
	m_navigationMode = eNavigationModeFly;
	m_transitionMode = eNavigationTransitionLinear;
	m_lookAt = ANMNAVINFO_DEFAULT_LOOKAT;
}

CAnmNavigationInfo::~CAnmNavigationInfo()
{
	SafeUnRef(m_avatarSize);
	SafeUnRef(m_type);
}

void CAnmNavigationInfo::Realize()
{
	CAnmBindableNode::Realize();
}


void CAnmNavigationInfo::Update()
{
	if (!StateDirty())
		return;

	// figure out the nav mode; N.B.: we're still not maintaining list
	// of available modes; need to do that

	int sz = m_type->size();
	eNavigationMode navmode;

	for (int i = 0; i < sz; i++)
	{
		if (!strcmp((*m_type)[i]->GetBuf(), "NONE"))
			navmode = eNavigationModeNone;
		else if (!strcmp((*m_type)[i]->GetBuf(), "EXAMINE"))
			navmode = eNavigationModeExamine;
		else if (!strcmp((*m_type)[i]->GetBuf(), "FLY"))
			navmode = eNavigationModeFly;
		else if (!strcmp((*m_type)[i]->GetBuf(), "WALK"))
			navmode = eNavigationModeWalk;
		else if (!strcmp((*m_type)[i]->GetBuf(), "LOOKAT"))
			navmode = eNavigationModeLookAt;
		else
			navmode = eNavigationModeFly;

		if (i == 0)
			m_navigationMode = navmode;

		if (navmode == eNavigationModeLookAt)
			m_lookAt = true;
	}

	sz = m_transitionType->size();
	if (sz > 0)
	{
		if (!strcmp((*m_transitionType)[0]->GetBuf(), "LINEAR"))
			m_transitionMode = eNavigationTransitionLinear;
		else if (!strcmp((*m_transitionType)[0]->GetBuf(), "ANIMATE"))
			m_transitionMode = eNavigationTransitionAnimate;
		else if (!strcmp((*m_transitionType)[0]->GetBuf(), "TELEPORT"))
			m_transitionMode = eNavigationTransitionTeleport;
	}

	ClearStateDirty();
}

float CAnmNavigationInfo::GetCollisionRadius()
{
	// Collision radius is first value in avatarSize field
	if (m_avatarSize)
	{
		int sz = m_avatarSize->size();
		if (sz > 0)
			return (*m_avatarSize)[0];
	}
		
	return ANMNAVINFO_DEFAULT_AVATAR_COLLISION_DISTANCE;
}

float CAnmNavigationInfo::GetAvatarHeight()
{
	// Avatar height is second value in avatarSize field
	if (m_avatarSize)
	{
		int sz = m_avatarSize->size();
		if (sz > 1)
			return (*m_avatarSize)[1];
	}
		
	return ANMNAVINFO_DEFAULT_AVATAR_HEIGHT_ABOVE_TERRAIN;
}

float CAnmNavigationInfo::GetMaxStepHeight()
{
	// Max step height is third value in avatarSize field
	if (m_avatarSize)
	{
		int sz = m_avatarSize->size();
		if (sz > 2)
			return (*m_avatarSize)[2];
	}
		
	return ANMNAVINFO_DEFAULT_AVATAR_TALLEST_OBJECT_HEIGHT;
}

// Accessors
void CAnmNavigationInfo::SetAvatarSize(FloatArray *pAvatarSize)
{
	assert(pAvatarSize != NULL);

	SafeUnRef(m_avatarSize);
	m_avatarSize = pAvatarSize;
	m_avatarSize->Ref();

	SetStateDirty();
	
	CallCallbacks(CALLBACKID(CAnmNavigationInfo, avatarSize_changed), &pAvatarSize);
}

void CAnmNavigationInfo::SetHeadlight(Boolean headlight)
{
	m_headlight = headlight;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmNavigationInfo, headlight_changed), &headlight);
}

void CAnmNavigationInfo::SetSpeed(Float speed)
{
	m_speed = speed;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmNavigationInfo, speed_changed), &speed);
}

void CAnmNavigationInfo::SetTransitionType(StringArray *pTransitionType)
{
	assert(pTransitionType != NULL);

	SafeUnRef(m_transitionType);
	m_transitionType = pTransitionType;
	m_transitionType->Ref();

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmNavigationInfo, transitionType_changed), &pTransitionType);

}

void CAnmNavigationInfo::SetType(StringArray *pType)
{
	assert(pType != NULL);

	SafeUnRef(m_type);
	m_type = pType;
	m_type->Ref();

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmNavigationInfo, type_changed), &pType);

}

void CAnmNavigationInfo::SetVisibilityLimit(Float visibilityLimit)
{
	m_visibilityLimit = visibilityLimit;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmNavigationInfo, visibilityLimit_changed), &visibilityLimit);
}


// Accessors


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmNavigationInfo, avatarSize, eValueFloatArray, FloatArray *, GetAvatarSize, SetAvatarSize);
DEFINE_VALUE(CAnmNavigationInfo, headlight, eValueBoolean, Boolean, GetHeadlight, SetHeadlight);
DEFINE_VALUE(CAnmNavigationInfo, speed, eValueFloat, Float, GetSpeed, SetSpeed);
DEFINE_VALUE(CAnmNavigationInfo, transitionType, eValueStringArray, StringArray *, GetTransitionType, SetTransitionType);
DEFINE_VALUE(CAnmNavigationInfo, type, eValueStringArray, StringArray *, GetType, SetType);
DEFINE_VALUE(CAnmNavigationInfo, visibilityLimit, eValueFloat, Float, GetVisibilityLimit, SetVisibilityLimit);

DEFINE_INHERITED_METHOD(CAnmNavigationInfo, set_bind, CAnmBindableNode);
DEFINE_METHOD(CAnmNavigationInfo, set_avatarSize, eValueFloatArray, FloatArray *, SetAvatarSize);
DEFINE_METHOD(CAnmNavigationInfo, set_headlight, eValueBoolean, Boolean, SetHeadlight);
DEFINE_METHOD(CAnmNavigationInfo, set_speed, eValueFloat, Float, SetSpeed);
DEFINE_METHOD(CAnmNavigationInfo, set_transitionType, eValueStringArray, StringArray *, SetTransitionType);
DEFINE_METHOD(CAnmNavigationInfo, set_type, eValueStringArray, StringArray *, SetType);
DEFINE_METHOD(CAnmNavigationInfo, set_visibilityLimit, eValueFloat, Float, SetVisibilityLimit);


DEFINE_INHERITED_CALLBACK(CAnmNavigationInfo, isBound, CAnmBindableNode);
DEFINE_CALLBACK(CAnmNavigationInfo, avatarSize_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmNavigationInfo, headlight_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmNavigationInfo, speed_changed, eValueFloat);
DEFINE_CALLBACK(CAnmNavigationInfo, transitionType_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmNavigationInfo, type_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmNavigationInfo, visibilityLimit_changed, eValueFloat);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmNavigationInfo)
VALUEID(CAnmNavigationInfo, avatarSize),
VALUEID(CAnmNavigationInfo, headlight),
VALUEID(CAnmNavigationInfo, speed),
VALUEID(CAnmNavigationInfo, transitionType),
VALUEID(CAnmNavigationInfo, type),
VALUEID(CAnmNavigationInfo, visibilityLimit),

METHODID(CAnmNavigationInfo, set_bind),
METHODID(CAnmNavigationInfo, set_avatarSize),
METHODID(CAnmNavigationInfo, set_headlight),
METHODID(CAnmNavigationInfo, set_speed),
METHODID(CAnmNavigationInfo, set_transitionType),
METHODID(CAnmNavigationInfo, set_type),
METHODID(CAnmNavigationInfo, set_visibilityLimit),

CALLBACKID(CAnmNavigationInfo, isBound),
CALLBACKID(CAnmNavigationInfo, avatarSize_changed),
CALLBACKID(CAnmNavigationInfo, headlight_changed),
CALLBACKID(CAnmNavigationInfo, speed_changed),
CALLBACKID(CAnmNavigationInfo, transitionType_changed),
CALLBACKID(CAnmNavigationInfo, type_changed),
CALLBACKID(CAnmNavigationInfo, visibilityLimit_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmNavigationInfo, CAnmBindableNode);

