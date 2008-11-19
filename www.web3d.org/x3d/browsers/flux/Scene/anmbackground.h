/********************************************************************************
 * Flux
 *
 * File: anmbackground.h
 * Description: Background node
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

#ifndef _anmbackground_h
#define _anmbackground_h

#include "anmbindablenode.h"



#define ANMBACKGROUND_SPHEREDIRTY			(ANMNODE_NDIRTYBITS )
#define ANMBACKGROUND_BOXDIRTY				(ANMBACKGROUND_SPHEREDIRTY + 1)
#define ANMBACKGROUND_NDIRTYBITS			(ANMBACKGROUND_BOXDIRTY + 1)

typedef enum eAnmBackgroundDirtyFlags {
eAnmBackgroundSphereDirty				= ANMDIRTYBIT(ANMBACKGROUND_SPHEREDIRTY),
eAnmBackgroundBoxDirty					= ANMDIRTYBIT(ANMBACKGROUND_BOXDIRTY),
} eAnmBackgroundDirtyFlags;


#define ANMBACKGROUND_DEFAULT_SKYCOLOR	Color(0, 0, 0)

#define ANMBACKGROUND_BACKURL	0
#define ANMBACKGROUND_BOTTOMURL	1
#define ANMBACKGROUND_FRONTURL	2
#define ANMBACKGROUND_LEFTURL	3
#define ANMBACKGROUND_RIGHTURL	4
#define ANMBACKGROUND_TOPURL	5

class CAnmBackground : public CAnmBindableNode
{

protected:

	FloatArray						*m_groundAngle;
	ColorArray						*m_groundColor;
	StringArray						*m_backUrl;
	StringArray						*m_bottomUrl;
	StringArray						*m_frontUrl;
	StringArray						*m_leftUrl;
	StringArray						*m_rightUrl;
	StringArray						*m_topUrl;
	FloatArray						*m_skyAngle;
	ColorArray						*m_skyColor;

	class CAnmGraphics::Texture		*m_textureSurfaces[6];
	String							 m_baseUrl;

	class CAnmCamera				*m_camera;

	class CAnmMesh					*m_groundSphere;
	class CAnmMesh					*m_skySphere;
	static class CAnmMesh			*m_skyBox[6];


	void CreateVolumes();
	void CreateSpheres();
	class CAnmMesh *CreateSphere(float radius, float lastAngle, bool up );
	void CAnmBackground::ApplyColors(class CAnmMesh *pSphereMesh, ColorArray *pColors,
		FloatArray *pAngles, bool up );
	void CreateBox();

	void handleUrlLoaded(String pRequestUrl, String pCacheFileName, int textureIndex);
	static class CAnmAsset *urlLoadedCallback(String pRequestUrl, String pCacheFileName, void *userData);

	virtual void LoadTextures();

public:

	// Constructor/Destructor
	CAnmBackground();

	virtual ~CAnmBackground();

	// Node class overrides
	virtual void Realize();		// build lower-level rendering structures
	virtual void Update();		// re-render/reset rendering structures

	// New methods
	virtual void Render(class CAnmDevice *pDevice);

	virtual bool IsSimpleBackgroundColor();
	virtual color4 GetSimpleBackgroundColor();

	virtual void SetBaseUrl(String baseUrl);
	virtual String GetBaseUrl()
	{
		return m_baseUrl;
	}

	// Accessors
	void SetGroundAngle(FloatArray *pGroundAngle);
	FloatArray *GetGroundAngle() { return m_groundAngle; }
	void GetGroundAngle(FloatArray **pVal)
	{
		assert(pVal);
		*pVal = m_groundAngle;
	}

	void SetGroundColor(ColorArray *pGroundColor);
	ColorArray *GetGroundColor() { return m_groundColor; }
	void GetGroundColor(ColorArray **pVal)
	{
		assert(pVal);
		*pVal = m_groundColor;
	}

	void SetSkyAngle(FloatArray *pSkyAngle);
	FloatArray *GetSkyAngle() { return m_skyAngle; }
	void GetSkyAngle(FloatArray **pVal)
	{
		assert(pVal);
		*pVal = m_skyAngle;
	}

	void SetSkyColor(ColorArray *pSkyColor);
	ColorArray *GetSkyColor() { return m_skyColor; }
	void GetSkyColor(ColorArray **pVal)
	{
		assert(pVal);
		*pVal = m_skyColor;
	}

	void SetBackUrl(StringArray *pBackUrl);
	StringArray *GetBackUrl() { return m_backUrl; }
	void GetBackUrl(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_backUrl;
	}

	void SetBottomUrl(StringArray *pBottomUrl);
	StringArray *GetBottomUrl() { return m_bottomUrl; }
	void GetBottomUrl(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_bottomUrl;
	}

	void SetFrontUrl(StringArray *pFrontUrl);
	StringArray *GetFrontUrl() { return m_frontUrl; }
	void GetFrontUrl(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_frontUrl;
	}

	void SetLeftUrl(StringArray *pLeftUrl);
	StringArray *GetLeftUrl() { return m_leftUrl; }
	void GetLeftUrl(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_leftUrl;
	}

	void SetRightUrl(StringArray *pRightUrl);
	StringArray *GetRightUrl() { return m_rightUrl; }
	void GetRightUrl(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_rightUrl;
	}
	
	void SetTopUrl(StringArray *pTopUrl);
	StringArray *GetTopUrl() { return m_topUrl; }
	void GetTopUrl(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_topUrl;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmBackground);

	DECLARE_VALUE(groundAngle);
	DECLARE_VALUE(groundColor);
	DECLARE_VALUE(backUrl);
	DECLARE_VALUE(bottomUrl);
	DECLARE_VALUE(frontUrl);
	DECLARE_VALUE(leftUrl);
	DECLARE_VALUE(rightUrl);
	DECLARE_VALUE(topUrl);
	DECLARE_VALUE(skyAngle);
	DECLARE_VALUE(skyColor);

	DECLARE_METHOD(set_bind);
	DECLARE_METHOD(set_groundAngle);
	DECLARE_METHOD(set_groundColor);
	DECLARE_METHOD(set_backUrl);
	DECLARE_METHOD(set_bottomUrl);
	DECLARE_METHOD(set_frontUrl);
	DECLARE_METHOD(set_leftUrl);
	DECLARE_METHOD(set_rightUrl);
	DECLARE_METHOD(set_topUrl);
	DECLARE_METHOD(set_skyAngle);
	DECLARE_METHOD(set_skyColor);

	DECLARE_CALLBACK(isBound);
	DECLARE_CALLBACK(groundAngle_changed);
	DECLARE_CALLBACK(groundColor_changed);
	DECLARE_CALLBACK(backUrl_changed);
	DECLARE_CALLBACK(bottomUrl_changed);
	DECLARE_CALLBACK(frontUrl_changed);
	DECLARE_CALLBACK(leftUrl_changed);
	DECLARE_CALLBACK(rightUrl_changed);
	DECLARE_CALLBACK(topUrl_changed);
	DECLARE_CALLBACK(skyAngle_changed);
	DECLARE_CALLBACK(skyColor_changed);
};


#endif // _anmbackground_h
