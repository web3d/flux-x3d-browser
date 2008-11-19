/********************************************************************************
 * Flux
 *
 * File: anmaudioclip.cpp
 * Description: AudioClip class
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
#include "application.h"
#include "anmaudioclip.h"
#include "anmfilemanager.h"
#include "anmmediasensor.h"
#include "anmproximitysensor.h"
#include "soundlayer.h"
#include "sound.h"
#include "anmproximitysensor.h"
#include "anmsound.h"
#include "anmtime.h"
#include "anmupdatevisitor.h"
#include "anmworld.h"
#include "anmmovietexturerenderer.h"

CAnmAudioClip::CAnmAudioClip()
 : CAnmNode(), CAnmMediaSourceObject(), CAnmUrlObject(),
 m_pitch(ANMAUDIOCLIP_DEFAULT_PITCH)
{
	m_description = new CAnmString(ANMAUDIOCLIP_DEFAULT_DESCRIPTION);
	m_intensity = ANMAUDIOCLIP_BADINTENSITY;
	m_loop = ANMAUDIOCLIP_DEFAULT_LOOP;
	m_pSoundDirShow = NULL;
	m_pSoundDirSound = NULL;
	m_proxSensor = new CAnmProximitySensor;
	m_proxSensor->SetOwner(this);
	m_proxSensor->AddCallback(CALLBACKID(CAnmProximitySensor, position_changed), CameraPositionChangedCallback, this);
	m_proxSensor->AddCallback(CALLBACKID(CAnmProximitySensor, orientation_changed), CameraOrientationChangedCallback, this);
	m_proxSensor->SetSize(Point3(FLT_MAX, FLT_MAX, FLT_MAX));
	m_cameraPos = point3::Zero;
	m_cameraDir = point3::Zero;
	m_cameraUp = point3::Zero;
	m_cameraUp.y = 1.0;
	m_cameraDir.z = -1.0;
	m_spatialize = false;
	m_UrlToLoadChars[0] = '\0';
	m_bUseDS = false;
} 


CAnmAudioClip::~CAnmAudioClip()
{
	SafeUnRef(m_description);
	SafeUnRef(m_proxSensor);
}

void CAnmAudioClip::NotifyRendererDestroyed()
{
	m_pSoundDirShow = NULL;
	m_pSoundDirSound = NULL;
}


void CAnmAudioClip::Realize()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	// if we dont have a direct sound device, then dont load the sound file.
	//
	if( pApp->GetSound() &&
		pApp->GetSound()->GetDSound() ) {

		Load(pApp, m_bUseDS );
	}

	CAnmNode::Realize();

	m_proxSensor->Realize();
	ClearStateDirty();
}




bool CAnmAudioClip::InitAudioRenderer( )
{
	bool bRtn = false;

	ClearDirtyBits(eAnmAudioClipUrlDirty);


	SafeDelete(m_pSoundDirShow);
	SafeDelete(m_pSoundDirSound);

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	bool bTryAudio = true;
	bool bTryVideo = false;

	String fullNameStr = new CAnmString( m_UrlToLoadChars );

	// krv: The Create emthod will fail if it cant find the file, or if the file can't be rendered.
	// 

	if( !m_bUseDS ) {
		m_pSoundDirSound = new cSound(pApp, m_UrlToLoadChars );
	}
	else {
		m_pSoundDirShow = CAnmMovieTextureRenderer::Create( this, pApp->GetDevice(), pWorld, 
				fullNameStr, bTryAudio, bTryVideo, m_RendererList );
	}
	m_UrlToLoadChars[0] = '\0';
	


	if ( m_pSoundDirShow || m_pSoundDirSound ){

		pWorld->Lock();

		// Tell my MediaSource superclass how long I play, and set its clock
		if( m_pSoundDirShow ) {
			SetDuration(m_pSoundDirShow->GetDuration());
		}
		if( m_pSoundDirSound ) {
			SetDuration(m_pSoundDirSound->GetDuration());
		}
		SetClock(pApp->GetClock());

		// Get us hooked into the app's time base
		ConnectSensor(this);

		// Make sure all other values get updated
		SetStateDirty();
		SetInstanceDirty();

		// N.B.: Set spatialize flag; this is a crock for multi-instanced
		CAnmGroup *pParent = GetFirstParent();
		CAnmSound *pSoundNode = GETIMPLEMENTATION(CAnmSound, pParent); 

		if (pSoundNode) {
			m_spatialize = pSoundNode->GetSpatialize();
		}


		// release the lock
		pWorld->Unlock();	
		bRtn = TRUE;
	}

	return ( bRtn );
}




class CAnmAsset *CAnmAudioClip::handleUrlLoaded(String pRequestUrl, String pCacheFileName)
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	strcpy( m_UrlToLoadChars, pCacheFileName->GetBuf() );

#if 0
	// krv:  Just save the url, and set the bit dirty.
	// Must wait till Update to Create the renderer.
	// We need to wait till we know if we need Audio and/or Video
	SetStateDirty();
	SetDirtyBits( eAnmAudioClipUrlDirty );
#else

	InitAudioRenderer();

#endif

	return NULL;
}

void CAnmAudioClip::handleUrlLoaded(String pRequestUrl, CAnmAsset *pAsset)
{
}

void CAnmAudioClip::Update()
{
	CAnmNode::Update();
	CAnmMediaSourceObject::Update();

	if ( m_UrlToLoadChars[0] && TestDirtyBits(eAnmAudioClipUrlDirty)){
		InitAudioRenderer();
	}

	if (m_pSoundDirShow && m_intensity != ANMAUDIOCLIP_BADINTENSITY) {
		m_pSoundDirShow->SetIntensity(m_intensity);
	}
	if (m_pSoundDirSound && m_intensity != ANMAUDIOCLIP_BADINTENSITY) {
		m_pSoundDirSound->SetIntensity(m_intensity);
	}

	// should check pitch here
}

void CAnmAudioClip::Traverse(CAnmUpdateVisitor *pVisitor)
{
	if (m_mediaSensor)
		m_mediaSensor->Traverse(pVisitor);

	// update the proximity sensor
	if (m_proxSensor)
		m_proxSensor->Traverse(pVisitor);



	if( m_spatialize ) {
		// N.B.: Set sound params; this is a crock for multi-instanced
		Point3 pos(0, 0, 0);
		Point3 dir(0, 0, 1);	// RH
		float minFront = ANMSOUND_DEFAULT_MINFRONT;
		float maxFront = ANMSOUND_DEFAULT_MAXFRONT;

		CAnmGroup *pParent = GetFirstParent();
		CAnmSound *pSound = GETIMPLEMENTATION(CAnmSound, pParent); 
		if (pSound)
		{
			pos = pSound->GetLocation();
			dir = pSound->GetDirection();
			minFront = pSound->GetMinFront();
			maxFront = pSound->GetMaxFront();
		}

		matrix4 wmat;
		pVisitor->GetWorldMatrix(&wmat);
		pos = wmat * pos;
		dir = wmat * dir;
		// pesky origin
		Point3 org = wmat * point3::Zero;
		dir = dir - org;

		Point3 listenerpos = wmat * m_cameraPos;

		
		wmat._41 = 
		wmat._42 = 
		wmat._43 = 0.0;
		Point3 listenerDir = wmat * m_cameraDir;
		Point3 listenerUp = wmat * m_cameraUp;

		dir.Normalize();
		listenerDir.Normalize();
		listenerUp.Normalize();

		// N.B.: this won't handle multi-instancing; we need to define some ->
		// kind of CAnmRenderObject for audio clip instances (probably video too)
		if ( m_pSoundDirShow )
		{
			m_pSoundDirShow->SetPosition(pos);
			m_pSoundDirShow->SetOrientation(dir);
			m_pSoundDirShow->SetMinDistance(minFront);
			m_pSoundDirShow->SetMaxDistance(maxFront);
			m_pSoundDirShow->SetListenerPosition(listenerpos);
			m_pSoundDirShow->SetListenerOrientation(listenerDir, listenerUp);
			m_pSoundDirShow->CommitSound3DChanges();
		}
		else if( m_pSoundDirSound )
		{
			m_pSoundDirSound->SetPosition(pos);
			m_pSoundDirSound->SetOrientation(dir);
			m_pSoundDirSound->SetMinDistance(minFront);
			m_pSoundDirSound->SetMaxDistance(maxFront);
			m_pSoundDirSound->SetListenerPosition(listenerpos);
			m_pSoundDirSound->SetListenerOrientation(listenerDir, listenerUp);
		}
	}

	CAnmNode::Traverse(pVisitor);
}

void CAnmAudioClip::PreTraverse()
{
	CAnmNode::PreTraverse();
	m_proxSensor->PreTraverse();
}

void CAnmAudioClip::PostTraverse()
{
	CAnmNode::PostTraverse();
	m_proxSensor->PostTraverse();
}

void CAnmAudioClip::HandleStart(Time startTime)
{
	if ( m_pSoundDirShow ) {
		m_pSoundDirShow->PlayStream(m_loop, startTime);
	}
	if (m_pSoundDirSound && !m_pSoundDirSound->IsPlaying()) {
		m_pSoundDirSound->Play(m_loop, startTime );
	}
}

void CAnmAudioClip::HandleStop()
{
	if ( m_pSoundDirShow ) {
		m_pSoundDirShow->PauseStream();
	}
	if (m_pSoundDirSound && m_pSoundDirSound->IsPlaying()) {
		m_pSoundDirSound->Stop();
	}
}
void CAnmAudioClip::HandleResume()
{
	if (m_pSoundDirShow ) {
		m_pSoundDirShow->ResumeStream();
	}
	if (m_pSoundDirSound ) {
		m_pSoundDirSound->Resume( m_loop );
	}
}

void CAnmAudioClip::HandlePause()
{
	if (m_pSoundDirShow ) {
		m_pSoundDirShow->PauseStream();
	}
	if (m_pSoundDirSound && m_pSoundDirSound->IsPlaying()) {
		m_pSoundDirSound->Stop();
	}
}

void CAnmAudioClip::HandleInit( )
{
	if (m_pSoundDirShow ) {
		m_pSoundDirShow->InitStream();
	}
}


void CAnmAudioClip::HandleActive(Boolean isActive)
{
	CallCallbacks(CALLBACKID(CAnmAudioClip, isActive), &isActive);
}

void CAnmAudioClip::HandleDurationChanged(Time newDuration)
{
	CallCallbacks(CALLBACKID(CAnmAudioClip, duration_changed), &newDuration);
}
void CAnmAudioClip::HandleElapsedTimeChanged(Time eTime )
{
	CallCallbacks(CALLBACKID(CAnmAudioClip, elapsedTime_changed), &eTime);
}
void CAnmAudioClip::HandlePaused(Boolean isPaused )
{
	CallCallbacks(CALLBACKID(CAnmAudioClip, isPaused), &isPaused);
}

void CAnmAudioClip::HandleTimeChanged(Time newTime)
{
	// we only need to do this for DirectShow.
	//
	// Contine will return true if the clip has reached its end.
	// this can only happen if Loop == false.
	if( m_pSoundDirShow && m_pSoundDirShow->Continue() ) {
		m_isActive = false;
	}
}

void CAnmAudioClip::HandleCameraPositionChanged(Point3 *pCameraPosition)
{
	m_cameraPos = *pCameraPosition;
	SetMatrixDirty();
}

void CAnmAudioClip::HandleCameraOrientationChanged(Rotation *pCameraOrientation)
{
	// Convert ROtation to Up and Dir:
	matrix4 Mtx = matrix4::AxisAngle( pCameraOrientation->vec, pCameraOrientation->theta );
		
	m_cameraDir.x = -Mtx._31;
	m_cameraDir.y = -Mtx._32;
	m_cameraDir.z = -Mtx._33;

	m_cameraUp.x = Mtx._21;
	m_cameraUp.y = Mtx._22;
	m_cameraUp.z = Mtx._23;
	SetMatrixDirty();
}

void CAnmAudioClip::CameraPositionChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData)
{
	CAnmProximitySensor *pPS = (CAnmProximitySensor *) sourceObject;
	CAnmAudioClip *pClip = (CAnmAudioClip *) userData;
	Point3 *pp =  (Point3 *) callData;

	pClip->HandleCameraPositionChanged(pp);
}

void CAnmAudioClip::CameraOrientationChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData)
{
	CAnmProximitySensor *pPS = (CAnmProximitySensor *) sourceObject;
	CAnmAudioClip *pClip = (CAnmAudioClip *) userData;
	Rotation *rp =  (Rotation *) callData;

	pClip->HandleCameraOrientationChanged(rp);
}

// Accessors
void CAnmAudioClip::SetDescription(String description)
{
	SafeUnRef(m_description);
	m_description = description;
	m_description->Ref();
}

void CAnmAudioClip::SetIntensity(Float intensity)
{
	m_intensity = intensity;
	SetStateDirty();
}

void CAnmAudioClip::SetPitch(Float pitch)
{
	m_pitch = pitch;

	SetStateDirty();
}

void CAnmAudioClip::SetUrl(StringArray *pUrl)
{
	CAnmUrlObject::SetUrl(pUrl);

	SetStateDirty();

	// Determine if this is a wav file.
	//
	m_bUseDS = false;
	if( pUrl && pUrl->size() > 0 ) {
		String str = (*pUrl)[0];
		int len = str->length();
		if( len > 4 ) {
			if( strnicmp( str->GetBuf() + len - 3, "wav", 3 ) != 0 ) {
				m_bUseDS = true;
			}
		}
	}


	CallCallbacks(CALLBACKID(CAnmAudioClip, url_changed), &pUrl);
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmAudioClip, description, eValueString, String, GetDescription, SetDescription);
DEFINE_VALUE(CAnmAudioClip, loop, eValueBoolean, Boolean, GetLoop, SetLoop);
DEFINE_VALUE(CAnmAudioClip, pitch, eValueFloat, Float, GetPitch, SetPitch);
DEFINE_VALUE(CAnmAudioClip, startTime, eValueTime, Time, GetStartTime, SetStartTime);
DEFINE_VALUE(CAnmAudioClip, stopTime, eValueTime, Time, GetStopTime, SetStopTime);
DEFINE_VALUE(CAnmAudioClip, url, eValueStringArray, StringArray*, GetUrl, SetUrl);
DEFINE_VALUE(CAnmAudioClip, resumeTime, eValueTime, Time, GetResumeTime, SetResumeTime);
DEFINE_VALUE(CAnmAudioClip, pauseTime, eValueTime, Time, GetPauseTime, SetPauseTime);




DEFINE_METHOD(CAnmAudioClip, set_description, eValueString, String, SetDescription);
DEFINE_METHOD(CAnmAudioClip, set_loop, eValueBoolean, Boolean, SetLoop);
DEFINE_METHOD(CAnmAudioClip, set_pitch, eValueFloat, Float, SetPitch);
DEFINE_METHOD(CAnmAudioClip, set_startTime, eValueTime, Time, SetStartTime);
DEFINE_METHOD(CAnmAudioClip, set_stopTime, eValueTime, Time, SetStopTime);
DEFINE_METHOD(CAnmAudioClip, set_url, eValueStringArray, StringArray*, SetUrl);
DEFINE_METHOD(CAnmAudioClip, set_resumeTime, eValueTime, Time, SetResumeTime);
DEFINE_METHOD(CAnmAudioClip, set_pauseTime, eValueTime, Time, SetPauseTime);




DEFINE_CALLBACK(CAnmAudioClip, description_changed, eValueString);
DEFINE_CALLBACK(CAnmAudioClip, duration_changed, eValueTime);
DEFINE_CALLBACK(CAnmAudioClip, isActive, eValueBoolean);
DEFINE_CALLBACK(CAnmAudioClip, loop_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmAudioClip, pitch_changed, eValueFloat);
DEFINE_CALLBACK(CAnmAudioClip, startTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmAudioClip, stopTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmAudioClip, url_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmAudioClip, resumeTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmAudioClip, pauseTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmAudioClip, isPaused, eValueBoolean);
DEFINE_CALLBACK(CAnmAudioClip, elapsedTime_changed, eValueBoolean);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmAudioClip)
VALUEID(CAnmAudioClip, description),
VALUEID(CAnmAudioClip, loop),
VALUEID(CAnmAudioClip, pitch),
VALUEID(CAnmAudioClip, startTime),
VALUEID(CAnmAudioClip, stopTime),
VALUEID(CAnmAudioClip, url),
VALUEID(CAnmAudioClip, resumeTime),
VALUEID(CAnmAudioClip, pauseTime),

METHODID(CAnmAudioClip, set_description),
METHODID(CAnmAudioClip, set_loop),
METHODID(CAnmAudioClip, set_pitch),
METHODID(CAnmAudioClip, set_startTime),
METHODID(CAnmAudioClip, set_stopTime),
METHODID(CAnmAudioClip, set_url),
METHODID(CAnmAudioClip, set_resumeTime),
METHODID(CAnmAudioClip, set_pauseTime),

CALLBACKID(CAnmAudioClip, description_changed),
CALLBACKID(CAnmAudioClip, duration_changed),
CALLBACKID(CAnmAudioClip, isActive),
CALLBACKID(CAnmAudioClip, loop_changed),
CALLBACKID(CAnmAudioClip, pitch_changed),
CALLBACKID(CAnmAudioClip, startTime_changed),
CALLBACKID(CAnmAudioClip, stopTime_changed),
CALLBACKID(CAnmAudioClip, url_changed),
CALLBACKID(CAnmAudioClip, resumeTime_changed),
CALLBACKID(CAnmAudioClip, pauseTime_changed),
CALLBACKID(CAnmAudioClip, isPaused),
CALLBACKID(CAnmAudioClip, elapsedTime_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmAudioClip, CAnmNode);


