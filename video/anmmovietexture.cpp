/********************************************************************************
 * Flux
 *
 * File: anmmovietexture.cpp
 * Description: MovieTexture node
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

#include "anmmovietexture.h"
#include "anmassetcache.h"
#include "anmfilemanager.h"
#include "anmmediasensor.h"
#include "anmmovietexturerenderer.h"
#include "anmworld.h"
#include "anmproximitysensor.h"
#include "anmupdatevisitor.h"
#include "anmSound.h"

#define ANMSOUND_DEFAULT_MAXBACK		10.f
#define ANMSOUND_DEFAULT_MAXFRONT		10.f
#define ANMSOUND_DEFAULT_MINBACK		1.f
#define ANMSOUND_DEFAULT_MINFRONT		1.f


CAnmMovieTexture::CAnmMovieTexture()
		: CAnmTexture(),						// Parent Appearance or Sound will add me
		CAnmMediaSourceObject(),
	CAnmUrlObject()
{
	m_loop = ANMMOVIETEXTURE_DEFAULT_LOOP;
	m_speed = ANMMOVIETEXTURE_DEFAULT_SPEED;
	m_intensity = ANMMOVIETEXTURE_BADINTENSITY;

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

	m_movie = NULL;
	m_spatialize = false;
	m_UrlToLoadChars[0] = '\0';
}


CAnmMovieTexture::~CAnmMovieTexture()
{
	SafeDelete(m_movie);
	SafeUnRef(m_proxSensor);
}

void CAnmMovieTexture::NotifyRendererDestroyed()
{
	m_movie = NULL;
}

void CAnmMovieTexture::Realize()
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	Load( pApp, true );


	m_proxSensor->Realize();

	CAnmTexture::Realize();

	

	ClearStateDirty();
}

void CAnmMovieTexture::Update()
{
	// krv: First we need to see if we need to Create the MovieTextureRenderer.
	// We need to wait till we traverse, so we can look through our parents for 
	// Sound or Appreance, so we know which pins and renderers to hook up.
	if (m_UrlToLoadChars[0] &&
		TestDirtyBits(eAnmMovieTextureUrlDirty)){
		InitTextureRenderer();
	}
	
	if (!StateDirty())
		return;

	if (m_movie && m_intensity != ANMMOVIETEXTURE_BADINTENSITY) {
		m_movie->SetIntensity(m_intensity);
	}

	CAnmTexture::Update();
	CAnmMediaSourceObject::Update();
}

void CAnmMovieTexture::Traverse(CAnmUpdateVisitor *pVisitor)
{
	if (m_mediaSensor)
		m_mediaSensor->Traverse(pVisitor);

	if( m_spatialize ) {

		// krv:  TONY!!!!!
		//  How do I get the tail of the Node path so I can test if the
		// path is going through a SOund Node.
		//  pVisitor->GetNodePath().GetTail()  ???????
		// I only want to do this if the Traverse is coming from the SOund Node.
		// If we are Traverseing from an Appearance, we dont want to do this.
		bool pVisitor_NodePath_Tail_is_Sound_Node = true;

		if( pVisitor_NodePath_Tail_is_Sound_Node ) {

			// update the proximity sensor
			if (m_proxSensor)
				m_proxSensor->Traverse(pVisitor);

			// N.B.: Set sound params; this is a crock for multi-instanced
			Point3 pos(0, 0, 0);
			Point3 dir(0, 0, 1);	// RH
			float minFront = ANMSOUND_DEFAULT_MINFRONT;
			float maxFront = ANMSOUND_DEFAULT_MAXFRONT;


			CAnmSound *pSound = NULL;
			std::list< CAnmGroup* >::iterator iter;
			for( 
				iter = m_parents.begin(); 
				iter != m_parents.end() && !pSound;
				iter++ )
			{
				pSound = GETIMPLEMENTATION(CAnmSound, *iter ); 
			}

			// krv:  If we can find a Sound Node in our ParentList, then dont bother
			// doing any of this Sound3D sutff.
			//
			if (pSound) {
				pos = pSound->GetLocation();
				dir = pSound->GetDirection();
				minFront = pSound->GetMinFront();
				maxFront = pSound->GetMaxFront();


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

				// N.B.: this won't handle multi-instancing; we need to define some ->
				// kind of CAnmRenderObject for audio clip instances (probably video too)
				if (m_movie)
				{
					m_movie->SetPosition(pos);
					m_movie->SetOrientation(dir);
					m_movie->SetMinDistance(minFront);
					m_movie->SetMaxDistance(maxFront);
					m_movie->SetListenerPosition(listenerpos);
					m_movie->SetListenerOrientation(listenerDir, listenerUp);
					m_movie->CommitSound3DChanges();
				}
			}
		}
	}

	CAnmNode::Traverse(pVisitor);
}



void CAnmMovieTexture::PreTraverse()
{
	CAnmNode::PreTraverse();

	m_proxSensor->PreTraverse();
}

void CAnmMovieTexture::PostTraverse()
{
	CAnmNode::PostTraverse();

	m_proxSensor->PostTraverse();
}



class CAnmAsset *CAnmMovieTexture::handleUrlLoaded(String pRequestUrl, String pCacheFileName)
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
	SetDirtyBits( eAnmMovieTextureUrlDirty );
#else

	InitTextureRenderer();

#endif

	return NULL;
}

bool CAnmMovieTexture::InitTextureRenderer( )
{

	ClearDirtyBits(eAnmMovieTextureUrlDirty);


	SafeDelete(m_movie);

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	bool bTryAudio = false;
	bool bTryVideo = false;


	// We need to look through our parent list to see if we have a Sound Node, or
	// if we have a Appearance Node ( or MultiTexture ).
	//

	CAnmSound *pSound = NULL;

	std::list< CAnmGroup* >::iterator iter;
	for( 
		iter = m_parents.begin(); 
		iter != m_parents.end();
		iter++ )
	{
		if( pSound = GETIMPLEMENTATION(CAnmSound, *iter ) ) {
			m_spatialize = pSound->GetSpatialize();
			bTryAudio = true;
		}
		else {
			// we assume that if its not sound, then it must be video.
			bTryVideo = true;
		}
	}

	if( bTryAudio || bTryVideo ) {

		String UrlToLoad = new CAnmString( m_UrlToLoadChars );
		m_UrlToLoadChars[0] = '\0';

		m_movie = CAnmMovieTextureRenderer::Create(this, pApp->GetDevice(), pWorld, 
				UrlToLoad, bTryAudio, bTryVideo, m_RendererList );

		if (m_movie){

			pWorld->Lock();

			m_textureSurface = m_movie->GetTextureSurface();

			// Tell my MediaSource superclass how long I play, and set its clock
			SetDuration(m_movie->GetDuration());
			SetClock(pApp->GetClock());

			// Get us hooked into the app's time base
			ConnectSensor(this);

			// Make sure all other values get updated
			SetStateDirty();
			SetInstanceDirty();

			// release the lock
			pWorld->Unlock();	
		}
		delete UrlToLoad;
	}
	return ( m_movie!=NULL );
}




void CAnmMovieTexture::HandleCameraPositionChanged(Point3 *pCameraPosition)
{
	m_cameraPos = *pCameraPosition;
	SetMatrixDirty();
}

void CAnmMovieTexture::HandleCameraOrientationChanged(Rotation *pCameraOrientation)
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

void CAnmMovieTexture::CameraPositionChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData)
{
	CAnmProximitySensor *pPS = (CAnmProximitySensor *) sourceObject;
	CAnmMovieTexture *pMT = (CAnmMovieTexture *) userData;
	Point3 *pp =  (Point3 *) callData;

	pMT->HandleCameraPositionChanged(pp);
}

void CAnmMovieTexture::CameraOrientationChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData)
{
	CAnmProximitySensor *pPS = (CAnmProximitySensor *) sourceObject;
	CAnmMovieTexture *pMT = (CAnmMovieTexture *) userData;
	Rotation *rp =  (Rotation *) callData;

	pMT->HandleCameraOrientationChanged(rp);
}








void CAnmMovieTexture::handleUrlLoaded(String pRequestUrl, CAnmAsset *pAsset)
{
#if 0
	assert(pAsset);
	assert(pAsset->GetType() == eImageAsset);

	CAnmImageAsset *pImageAsset = (CAnmImageAsset *) pAsset;
	handleTextureLoaded(pImageAsset->GetTextureData());
#endif
}

void CAnmMovieTexture::HandleStart(Time startTime)
{
	if (m_movie ) {
		m_movie->PlayStream( m_loop, startTime );
	}
}

void CAnmMovieTexture::HandleStop()
{
	if (m_movie ) {
		m_movie->PauseStream();
	}
}

void CAnmMovieTexture::HandleResume()
{
	if (m_movie ) {
		m_movie->ResumeStream();
	}
}

void CAnmMovieTexture::HandlePause()
{
	if (m_movie ) {
		m_movie->PauseStream();
	}
}

void CAnmMovieTexture::HandleInit( )
{
	if (m_movie ) {
		m_movie->InitStream();
	}
}


void CAnmMovieTexture::HandleActive(Boolean isActive)
{
	CallCallbacks(CALLBACKID(CAnmMovieTexture, isActive), &isActive);
}

void CAnmMovieTexture::HandleDurationChanged(Time newDuration)
{
	CallCallbacks(CALLBACKID(CAnmMovieTexture, duration_changed), &newDuration);
}

void CAnmMovieTexture::HandleElapsedTimeChanged(Time eTime )
{
	CallCallbacks(CALLBACKID(CAnmMovieTexture, elapsedTime_changed), &eTime);
}
void CAnmMovieTexture::HandlePaused(Boolean isPaused )
{
	CallCallbacks(CALLBACKID(CAnmMovieTexture, isPaused), &isPaused);
}



void CAnmMovieTexture::HandleTimeChanged(Time newTime)
{
	// Contine will return true if the clip has reached its end.
	// this can only happen if Loop == false.
	if( m_movie->Continue() ) {
		m_isActive = false;
	}
}

// Accessors
void CAnmMovieTexture::SetSpeed(Float speed)
{
	m_speed = speed;

	SetStateDirty();
}

void CAnmMovieTexture::SetUrl(StringArray *pUrl)
{
	CAnmUrlObject::SetUrl(pUrl);

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMovieTexture, url_changed), &pUrl);
}



void CAnmMovieTexture::SetIntensity(float intensity)
{
	m_intensity = intensity;
	SetStateDirty();
}


// Tables for Callbacks, Methods and Set/GetValues


DEFINE_VALUE(CAnmMovieTexture, loop, eValueBoolean, Boolean, GetLoop, SetLoop);
DEFINE_VALUE(CAnmMovieTexture, speed, eValueFloat, Float, GetSpeed, SetSpeed);
DEFINE_VALUE(CAnmMovieTexture, startTime, eValueTime, Time, GetStartTime, SetStartTime);
DEFINE_VALUE(CAnmMovieTexture, stopTime, eValueTime, Time, GetStopTime, SetStopTime);
DEFINE_VALUE(CAnmMovieTexture, url, eValueStringArray, StringArray*, GetUrl, SetUrl);
DEFINE_INHERITED_VALUE(CAnmMovieTexture, repeatS, CAnmTexture);
DEFINE_INHERITED_VALUE(CAnmMovieTexture, repeatT, CAnmTexture);
DEFINE_VALUE(CAnmMovieTexture, resumeTime, eValueTime, Time, GetResumeTime, SetResumeTime);
DEFINE_VALUE(CAnmMovieTexture, pauseTime, eValueTime, Time, GetPauseTime, SetPauseTime);

DEFINE_METHOD(CAnmMovieTexture, set_loop, eValueBoolean, Boolean, SetLoop);
DEFINE_METHOD(CAnmMovieTexture, set_speed, eValueFloat, Float, SetSpeed);
DEFINE_METHOD(CAnmMovieTexture, set_startTime, eValueTime, Time, SetStartTime);
DEFINE_METHOD(CAnmMovieTexture, set_stopTime, eValueTime, Time, SetStopTime);
DEFINE_METHOD(CAnmMovieTexture, set_url, eValueStringArray, StringArray*, SetUrl);
DEFINE_METHOD(CAnmMovieTexture, set_resumeTime, eValueTime, Time, SetResumeTime);
DEFINE_METHOD(CAnmMovieTexture, set_pauseTime, eValueTime, Time, SetPauseTime);

DEFINE_CALLBACK(CAnmMovieTexture, duration_changed, eValueTime);
DEFINE_CALLBACK(CAnmMovieTexture, isActive, eValueBoolean);
DEFINE_CALLBACK(CAnmMovieTexture, loop_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmMovieTexture, speed_changed, eValueFloat);
DEFINE_CALLBACK(CAnmMovieTexture, startTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmMovieTexture, stopTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmMovieTexture, url_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmMovieTexture, resumeTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmMovieTexture, pauseTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmMovieTexture, isPaused, eValueBoolean);
DEFINE_CALLBACK(CAnmMovieTexture, elapsedTime_changed, eValueBoolean);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmMovieTexture)
VALUEID(CAnmMovieTexture, loop),
VALUEID(CAnmMovieTexture, speed),
VALUEID(CAnmMovieTexture, startTime),
VALUEID(CAnmMovieTexture, stopTime),
VALUEID(CAnmMovieTexture, url),
VALUEID(CAnmMovieTexture, repeatS),
VALUEID(CAnmMovieTexture, repeatT),
VALUEID(CAnmMovieTexture, resumeTime),
VALUEID(CAnmMovieTexture, pauseTime),

METHODID(CAnmMovieTexture, set_loop),
METHODID(CAnmMovieTexture, set_speed),
METHODID(CAnmMovieTexture, set_startTime),
METHODID(CAnmMovieTexture, set_stopTime),
METHODID(CAnmMovieTexture, set_url),
METHODID(CAnmMovieTexture, set_resumeTime),
METHODID(CAnmMovieTexture, set_pauseTime),

CALLBACKID(CAnmMovieTexture, duration_changed),
CALLBACKID(CAnmMovieTexture, isActive),
CALLBACKID(CAnmMovieTexture, loop_changed),
CALLBACKID(CAnmMovieTexture, speed_changed),
CALLBACKID(CAnmMovieTexture, startTime_changed),
CALLBACKID(CAnmMovieTexture, stopTime_changed),
CALLBACKID(CAnmMovieTexture, url_changed),
CALLBACKID(CAnmMovieTexture, resumeTime_changed),
CALLBACKID(CAnmMovieTexture, pauseTime_changed),
CALLBACKID(CAnmMovieTexture, isPaused),
CALLBACKID(CAnmMovieTexture, elapsedTime_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmMovieTexture, CAnmTexture);
