/********************************************************************************
 * Flux
 *
 * File: anmmovietexture.h
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

#ifndef _anmmovietexture_h
#define _anmmovietexture_h

#include "anmmediasourceobject.h"
#include "anmtexture.h"
#include "anmurlobject.h"

#define ANMMOVIETEXTURE_DEFAULT_LOOP				FALSE
#define ANMMOVIETEXTURE_DEFAULT_SPEED				1.f
#define ANMMOVIETEXTURE_BADINTENSITY					-1.f

class CAnmMovieTexture: public CAnmTexture, public CAnmMediaSourceObject, public CAnmUrlObject
{

protected:

	Float								 m_speed;
	Point3								m_cameraPos;
	Point3								m_cameraDir;
	Point3								m_cameraUp;
	
	bool								m_spatialize;
	class CAnmMovieTextureRenderer		*m_movie;
	class CAnmProximitySensor			*m_proxSensor;

	char								m_UrlToLoadChars[400];

	Float								m_intensity;

	// helpers for async movie file loading
	virtual class CAnmAsset *handleUrlLoaded(String pRequestUrl, String pCacheFileName);
	virtual void handleUrlLoaded(String pRequestUrl, class CAnmAsset *pAsset);
	// helpers
	void HandleCameraPositionChanged(Point3 *pCameraPosition);
	void HandleCameraOrientationChanged(Rotation *pCameraOrientation);
	bool CAnmMovieTexture::InitTextureRenderer( );

	// static members
	static void CameraPositionChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData);
	static void CameraOrientationChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData);


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
	virtual void HandleInit();

	virtual void NotifyRendererDestroyed();

public:

	// constructor/destructor
	CAnmMovieTexture();
	virtual ~CAnmMovieTexture();

	// CAnmTexture overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual void PreTraverse();
	virtual void PostTraverse();

	virtual void Ref()
	{
		m_refcount++;
	}


	void SetIntensity(float intensity);


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


	// Accessors
	void SetUrl(StringArray *pUrl);
	// N.B.: GetUrl methods defined in CAnmUrlObject superclass


	void SetSpeed(Float speed);
	Float GetSpeed() { return m_speed; }
	void GetSpeed(Float *pVal)
	{
		if (pVal)
			*pVal = m_speed;
	}
	
	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmMovieTexture);

	DECLARE_VALUE(loop);
	DECLARE_VALUE(repeatS);
	DECLARE_VALUE(repeatT);
	DECLARE_VALUE(speed);
	DECLARE_VALUE(startTime);
	DECLARE_VALUE(stopTime);
	DECLARE_VALUE(url);
	DECLARE_VALUE(pauseTime);
	DECLARE_VALUE(resumeTime);

	DECLARE_METHOD(set_loop);
	DECLARE_METHOD(set_speed);
	DECLARE_METHOD(set_startTime);
	DECLARE_METHOD(set_stopTime);
	DECLARE_METHOD(set_url);
	DECLARE_METHOD(set_resumeTime);
	DECLARE_METHOD(set_pauseTime);

	DECLARE_CALLBACK(duration_changed);
	DECLARE_CALLBACK(isActive);
	DECLARE_CALLBACK(loop_changed);
	DECLARE_CALLBACK(speed_changed);
	DECLARE_CALLBACK(startTime_changed);
	DECLARE_CALLBACK(stopTime_changed);
	DECLARE_CALLBACK(url_changed);
	DECLARE_CALLBACK(resumeTime_changed);
	DECLARE_CALLBACK(pauseTime_changed);
	DECLARE_CALLBACK(isPaused);
	DECLARE_CALLBACK(elapsedTime_changed);

};

DECLARE_GETIMPLEMENTATION(CAnmMovieTexture);


#define ANMMOVIETEXTURE_URLDIRTY				ANMTEXTURE_NDIRTYBITS
#define ANMMOVIETEXTURE_NDIRTYBITS				(ANMMOVIETEXTURE_URLDIRTY + 1)

typedef enum eAnmMovieTextureDirtyFlags {
eAnmMovieTextureUrlDirty				= ANMDIRTYBIT(ANMMOVIETEXTURE_URLDIRTY),
} eAnmMovieTextureDirtyFlags;


#endif // _anmmovietexture_h

