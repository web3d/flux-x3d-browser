/********************************************************************************
 * Flux
 *
 * File: anmsound.cpp
 * Description: Spatialized sound node
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

 #include "anmsound.h"
 #include "anmaudioclip.h"
 #include "anmmovieTexture.h"
 
CAnmSound::CAnmSound()
	: CAnmGroup(),
	m_source(NULL),
	m_direction(ANMSOUND_DEFAULT_DIRECTION),
	m_intensity(ANMSOUND_DEFAULT_INTENSITY),
	m_location(ANMSOUND_DEFAULT_LOCATION),
	m_maxBack(ANMSOUND_DEFAULT_MAXBACK),
	m_maxFront(ANMSOUND_DEFAULT_MAXFRONT),
	m_minBack(ANMSOUND_DEFAULT_MINBACK),
	m_minFront(ANMSOUND_DEFAULT_MINFRONT),
	m_priority(ANMSOUND_DEFAULT_PRIORITY),
	m_spatialize(ANMSOUND_DEFAULT_SPATIALIZE)
{
}

CAnmSound::~CAnmSound()
{
}

void CAnmSound::Realize()
{
	// get the audio clip realized
	CAnmGroup::Realize();
}

void CAnmSound::Update()
{
	if (StateDirty())
	{
		if (m_source)
		{
			CAnmAudioClip *pAudioClip = GETIMPLEMENTATION(CAnmAudioClip, m_source);
			if (pAudioClip) {
				pAudioClip->SetIntensity(m_intensity);
			}
			else {
				CAnmMovieTexture *pMovie = GETIMPLEMENTATION(CAnmMovieTexture, m_source);
				if (pMovie) {
					pMovie->SetIntensity(m_intensity);
				}
			}
		}

		ClearStateDirty();
	}
}

eAnmReturnStatus CAnmSound::AddChild(CAnmNode *pChild)
{
	CAnmGroup::AddChild(pChild);

	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmAudioClip))) {
		m_source = pChild;
	}
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmMovieTexture))) {
		m_source = pChild;
	}

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmSound::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmAudioClip))) {
		m_source = NULL;
	}
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmMovieTexture))) {
		m_source = NULL;
	}

	CAnmGroup::RemoveChild(pChild);

	return eAnmReturnAllGood;
}

// Accessors
void CAnmSound::SetDirection(Point3 direction)
{
	m_direction = direction;
	SetMatrixDirty();
}

void CAnmSound::SetIntensity(Float intensity)
{
	m_intensity = intensity;
	SetStateDirty();
}

void CAnmSound::SetLocation(Point3 location)
{
	m_location = location;
	SetMatrixDirty();
}

void CAnmSound::SetMaxBack(Float maxBack)
{
	m_maxBack = maxBack;
	SetMatrixDirty();
}

void CAnmSound::SetMaxFront(Float maxFront)
{
	m_maxFront = maxFront;
	SetMatrixDirty();
}

void CAnmSound::SetMinBack(Float minBack)
{
	m_minBack = minBack;
	SetStateDirty();
}

void CAnmSound::SetMinFront(Float minFront)
{
	m_minFront = minFront;
	SetStateDirty();
}

void CAnmSound::SetPriority(Float priority)
{
	m_priority = priority;
	SetStateDirty();
}

void CAnmSound::SetSource(CAnmNode *pSource)
{
	if (m_source == pSource)
		return;

	if (m_source)
		RemoveChild(m_source);

	if (pSource == NULL)
		return;

	AddChild(pSource);
	m_source = pSource;
}

void CAnmSound::SetSpatialize(Boolean spatialize)
{
	m_spatialize = spatialize;
	SetStateDirty();
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmSound, direction, eValuePoint3, Point3, GetDirection, SetDirection);
DEFINE_VALUE(CAnmSound, intensity, eValueFloat, Float, GetIntensity, SetIntensity);
DEFINE_VALUE(CAnmSound, location, eValuePoint3, Point3, GetLocation, SetLocation);
DEFINE_VALUE(CAnmSound, maxBack, eValueFloat, Float, GetMaxBack, SetMaxBack);
DEFINE_VALUE(CAnmSound, maxFront, eValueFloat, Float, GetMaxFront, SetMaxFront);
DEFINE_VALUE(CAnmSound, minBack, eValueFloat, Float, GetMinBack, SetMinBack);
DEFINE_VALUE(CAnmSound, minFront, eValueFloat, Float, GetMinFront, SetMinFront);
DEFINE_VALUE(CAnmSound, priority, eValueFloat, Float, GetPriority, SetPriority);
DEFINE_VALUE(CAnmSound, source, eValueNode, CAnmNode *, GetSource, SetSource);
DEFINE_VALUE(CAnmSound, spatialize, eValueBoolean, Boolean, GetSpatialize, SetSpatialize);

DEFINE_METHOD(CAnmSound, set_direction, eValuePoint3, Point3, SetDirection);
DEFINE_METHOD(CAnmSound, set_intensity, eValueFloat, Float, SetIntensity);
DEFINE_METHOD(CAnmSound, set_location, eValuePoint3, Point3, SetLocation);
DEFINE_METHOD(CAnmSound, set_maxBack, eValueFloat, Float, SetMaxBack);
DEFINE_METHOD(CAnmSound, set_maxFront, eValueFloat, Float, SetMaxFront);
DEFINE_METHOD(CAnmSound, set_minBack, eValueFloat, Float, SetMinBack);
DEFINE_METHOD(CAnmSound, set_minFront, eValueFloat, Float, SetMinFront);
DEFINE_METHOD(CAnmSound, set_priority, eValueFloat, Float, SetPriority);
DEFINE_METHOD(CAnmSound, set_source, eValueNode, CAnmNode *, SetSource);
DEFINE_METHOD(CAnmSound, set_spatialize, eValueBoolean, Boolean, SetSpatialize);

DEFINE_CALLBACK(CAnmSound, direction_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmSound, intensity_changed, eValueFloat);
DEFINE_CALLBACK(CAnmSound, location_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmSound, maxBack_changed, eValueFloat);
DEFINE_CALLBACK(CAnmSound, maxFront_changed, eValueFloat);
DEFINE_CALLBACK(CAnmSound, minBack_changed, eValueFloat);
DEFINE_CALLBACK(CAnmSound, minFront_changed, eValueFloat);
DEFINE_CALLBACK(CAnmSound, priority_changed, eValueFloat);
DEFINE_CALLBACK(CAnmSound, source_changed, eValueNode);
DEFINE_CALLBACK(CAnmSound, spatialize_changed, eValueBoolean);

// member table


BEGIN_DEFINE_NODE_MEMBERS(CAnmSound)
VALUEID(CAnmSound, direction),
VALUEID(CAnmSound, intensity),
VALUEID(CAnmSound, location),
VALUEID(CAnmSound, maxBack),
VALUEID(CAnmSound, maxFront),
VALUEID(CAnmSound, minBack),
VALUEID(CAnmSound, minFront),
VALUEID(CAnmSound, priority),
VALUEID(CAnmSound, source),
VALUEID(CAnmSound, spatialize),

METHODID(CAnmSound, set_direction),
METHODID(CAnmSound, set_intensity),
METHODID(CAnmSound, set_location),
METHODID(CAnmSound, set_maxBack),
METHODID(CAnmSound, set_maxFront),
METHODID(CAnmSound, set_minBack),
METHODID(CAnmSound, set_minFront),
METHODID(CAnmSound, set_priority),
METHODID(CAnmSound, set_source),
METHODID(CAnmSound, set_spatialize),

CALLBACKID(CAnmSound, direction_changed),
CALLBACKID(CAnmSound, intensity_changed),
CALLBACKID(CAnmSound, location_changed),
CALLBACKID(CAnmSound, maxBack_changed),
CALLBACKID(CAnmSound, maxFront_changed),
CALLBACKID(CAnmSound, minBack_changed),
CALLBACKID(CAnmSound, minFront_changed),
CALLBACKID(CAnmSound, priority_changed),
CALLBACKID(CAnmSound, source_changed),
CALLBACKID(CAnmSound, spatialize_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmSound, CAnmGroup);
