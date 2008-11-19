/********************************************************************************
 * Flux
 *
 * File: anmaudioclip.h
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

#ifndef _anmaudioclip_h
#define _anmaudioclip_h

#include "anmnode.h"
#include "anmmediasourceobject.h"
#include "anmurlobject.h"
#include "anmtexture.h"

#define ANMAUDIOCLIP_DEFAULT_DESCRIPTION			""
#define ANMAUDIOCLIP_DEFAULT_LOOP					FALSE
#define ANMAUDIOCLIP_DEFAULT_PITCH					1.0f
#define ANMAUDIOCLIP_BADINTENSITY					-1.f

class CAnmAudioClip : public CAnmNode, public CAnmMediaSourceObject, public CAnmUrlObject
{
protected:

	String			m_description;
	Float			m_pitch;

	Float			m_intensity;

	// N.B.: these all need to move up to CAnmSound to handle multi-instancing

	class cSound					*m_pSoundDirSound;
	class CAnmMovieTextureRenderer	*m_pSoundDirShow;


	class CAnmProximitySensor	*m_proxSensor;
	Point3			m_cameraPos;
	Point3			m_cameraDir;
	Point3			m_cameraUp;
	bool			m_spatialize;

	char								m_UrlToLoadChars[400];
	

	bool			m_bUseDS;

	// CAnmMediaSourceObject overrides
	virtual void HandleStart(Time startTime);
	virtual void HandleStop();
	virtual void HandleActive(Boolean isActive);
	virtual void HandleDurationChanged(Time newDuration);
	virtual void HandleTimeChanged(Time newtime);

	virtual void HandleResume();
	virtual void HandlePause();
	virtual void HandlePaused(Boolean isPaused);
	virtual void HandleElapsedTimeChanged(Time elapsedTime);
	virtual void HandleInit( );


	// CAnmUrlObject overrides
	// Helper for async sound file loading
	virtual class CAnmAsset *handleUrlLoaded(String pRequestUrl, String pCacheFileName);
	virtual void handleUrlLoaded(String pRequestUrl, class CAnmAsset *pAsset);

	// helpers
	void HandleCameraPositionChanged(Point3 *pCameraPosition);
	void HandleCameraOrientationChanged(Rotation *pCameraOrientation);
	bool InitAudioRenderer( );

	// static members
	static void CameraPositionChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData);
	static void CameraOrientationChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData);

	virtual void NotifyRendererDestroyed();


public:

	// constructor/destructor
	CAnmAudioClip();
	virtual ~CAnmAudioClip();

	// CAnmNode overrides
	virtual void Realize();
	virtual void Update();
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual void PreTraverse();
	virtual void PostTraverse();

	// CAnmUrlObject overrides
	virtual void SetBaseUrl(String baseUrl)
	{
		CAnmUrlObject::SetBaseUrl(baseUrl);
	}

	virtual String GetBaseUrl()
	{
		return CAnmUrlObject::GetBaseUrl();
	}


	// basic QueryInterface-like capability
	virtual bool GetInterface(ANMINTERFACE ival, void *pvoid)
	{
		if (ival == ANMINTERFACEID(CAnmUrlObject))
		{
			CAnmUrlObject **ppUrlObj = (CAnmUrlObject **) pvoid;
			*ppUrlObj = this;
			return true;
		}
		else if (ival == ANMINTERFACEID(CAnmMediaSourceObject))
		{
			CAnmMediaSourceObject **ppUrlObj = (CAnmMediaSourceObject **) pvoid;
			*ppUrlObj = this;
			return true;
		}

		return false;
	}


//	if( pImp && pImp->GetInterface(ANMINTERFACEID(CAnmMediaSourceObject), &pMSO) ) {


	// New methods
	virtual void SetIntensity(Float intensity);

	// Accessors

	void SetDescription(String description);
	String GetDescription() { return m_description; }
	void GetDescription(String *pVal)
	{
		if (pVal)
			*pVal = m_description; //N.B.: heap!
	}

	void SetPitch(Float pitch);
	Float GetPitch() { return m_pitch; }
	void GetPitch(Float *pVal)
	{
		if (pVal)
			*pVal = m_pitch;
	}

	void SetUrl(StringArray *pUrl);
	// N.B.: GetUrl methods defined in CAnmUrlObject superclass

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmAudioClip);

	DECLARE_VALUE(description);
	DECLARE_VALUE(loop);
	DECLARE_VALUE(pitch);
	DECLARE_VALUE(startTime);
	DECLARE_VALUE(stopTime);
	DECLARE_VALUE(url);
	DECLARE_VALUE(pauseTime);
	DECLARE_VALUE(resumeTime);

	DECLARE_METHOD(set_description);
	DECLARE_METHOD(set_loop);
	DECLARE_METHOD(set_pitch);
	DECLARE_METHOD(set_startTime);
	DECLARE_METHOD(set_stopTime);
	DECLARE_METHOD(set_url);
	DECLARE_METHOD(set_resumeTime);
	DECLARE_METHOD(set_pauseTime);

	DECLARE_CALLBACK(description_changed);
	DECLARE_CALLBACK(duration_changed);
	DECLARE_CALLBACK(isActive);
	DECLARE_CALLBACK(loop_changed);
	DECLARE_CALLBACK(pitch_changed);
	DECLARE_CALLBACK(startTime_changed);
	DECLARE_CALLBACK(stopTime_changed);
	DECLARE_CALLBACK(url_changed);
	DECLARE_CALLBACK(resumeTime_changed);
	DECLARE_CALLBACK(pauseTime_changed);
	DECLARE_CALLBACK(isPaused);
	DECLARE_CALLBACK(elapsedTime_changed);

};

DECLARE_GETIMPLEMENTATION(CAnmAudioClip);

#define ANMAUDIOCLIP_URLDIRTY				1 
#define ANMAUDIOCLIP_NDIRTYBITS				(ANMAUDIOCLIP_URLDIRTY + 1)

typedef enum eAnmAudioClipDirtyFlags {
eAnmAudioClipUrlDirty				= ANMDIRTYBIT(ANMAUDIOCLIP_URLDIRTY),
} eAnmAudioClipDirtyFlags;


#endif // _anmaudioclip_h
