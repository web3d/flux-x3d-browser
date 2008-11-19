/********************************************************************************
 * Flux
 *
 * File: anmdevice.h
 * Description: Base device class
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

#ifndef _anmdevice_h
#define _anmdevice_h

#include "anmobject.h"


#define ANMNUMDEFAULTACTIVELIGHTS			8
#define ANMDEVICE_MINDEVICEWIDTH			32
#define ANMDEVICE_MINDEVICEHEIGHT			32

class EXPORT CAnmDevice : public CAnmObject
{

protected:

	HWND					m_hWnd;
	class cApplication		*m_app;


	// Viewport object
	class CAnmViewport		*m_viewport;

	// hardware acceleration flag
	bool					m_bIsHardware;

	// mipmap support - basically a workaround for ATI driver crap
	bool					m_canMipMap;

	// multitexture capability
	int						m_maxTextureLevels;

	// rectangles we'll need
	RECT					m_rcScreenRect;
	RECT					m_rcViewportRect;

	// coordinate system to use
	bool					m_righthanded;

	bool					m_wireframe;
	bool					m_antialias;

	int						m_zbufferbytes;

	DWORD					m_maxActiveLights;
	std::vector<bool>		m_activeLights;
	int						m_numActiveLights;

	int						m_maxTextureSize;

	CAnmGraphics::Shader	*m_vertexShader;
	CAnmGraphics::Shader	*m_fragmentShader;

	// Helpers
	void MakeViewport();
	void AllocActiveLight(int index);
	BOOL UpdateRectangles();

	// Overridables
	virtual void LightEnable(int index, bool on) PURE;

public:

	CAnmDevice(HWND hWnd, class cApplication *pApp);
	~CAnmDevice();

	// Factory
	static CAnmDevice *CreateDevice(HWND hWnd, class cApplication *pApp);

	// Overridables - subclass MUST supply
	virtual void BeginScene() PURE;
	virtual void EndScene() PURE;

	virtual void GetWorldMatrix( matrix4* pMat ) PURE;
	virtual void SetWorldMatrix( const matrix4& mat ) PURE;

	virtual void GetViewMatrix( matrix4* pMat ) PURE;
	virtual void SetViewMatrix( const matrix4& mat ) PURE;

	virtual void GetProjectionMatrix( matrix4* pMat ) PURE;
	virtual void SetProjectionMatrix( const matrix4& mat ) PURE;
	virtual void SetOrthoMatrix( float x0, float x1, float y0, float y1, float z0, float z1 ) PURE;

	virtual void SetViewportParams(int x, int y, int width, int height, float minz, float maxz) PURE;

	// Drawing Routines
	virtual void Clear( 
		bool bClearFrame, 
		bool bClearZ, 
		DWORD frameColor = 0, 
		float zValue = 0.f ) PURE;

	virtual void Flip( bool bNoVSync = false ) PURE;

	virtual void DrawTextString( int x, int y, DWORD color, const char * str ) PURE;

	// Materials
	virtual void SetMaterial(CAnmGraphics::Material *pMaterial) PURE;

	// Textures
	virtual void SetTexture(int stage, class CAnmGraphics::Texture *pTexture) PURE;
	virtual void SetTextureOption(int stage, eAnmTextureOptions option,
		int value) PURE;
	virtual void SetTextureTransform(int stage, matrix4 *pTT) PURE;
	virtual void GetDefaultTextureTransform(matrix4 *pDefaultTT) PURE;

	// Lighting
	virtual void SetLight(int index, CAnmGraphics::Light *pLight) PURE;
	virtual void SetLighting(bool on) PURE;

	// N.B.: These aren't pure, they actually do something.
	virtual void EnableLight(int index, bool on);
	virtual void ClearLights();

	// Fog
	virtual void EnableFog(bool on) PURE;
	virtual void SetFog(CAnmGraphics::Fog *pFog) PURE;

	// Alpha blending
	virtual void SetAlphaBlending(bool alphablending) PURE;

	// Z-buffer on/off
	virtual void SetZBuffering(bool enabled) PURE;

	// Options
	virtual void SetRenderOptions(eAnmRenderOptions options) PURE;

	// Interface with windowing
	virtual void HandleSize(int newwidth, int newheight) PURE;
	virtual void HandleMove() PURE;
	virtual void Reset() PURE;
	virtual bool IsDeviceReady() PURE;

	// Shaders
	virtual void SetVertexShader(CAnmGraphics::Shader *pVertexShader) PURE;
	virtual void SetFragmentShader(CAnmGraphics::Shader *pFragmentShader) PURE;


	// Capabilities
	virtual DWORD GetMaxPrimitiveCount() PURE;


	// Accessors
	cApplication *GetApp()
	{
		return m_app;
	}

	HWND GetHWnd()
	{
		return m_hWnd;
	}

	int Width() const
	{
		return m_rcViewportRect.right;
	}

	int Height() const
	{
		return m_rcViewportRect.bottom;
	}

	class CAnmViewport *GetViewport()
	{
		return m_viewport;
	}

	bool IsHardwareAccelerated()
	{
		return m_bIsHardware;
	}

	int MaxTextureLevels()
	{
		return m_maxTextureLevels;
	}

	int GetZBufferBytes()
	{
		return m_zbufferbytes;
	}

	void SetMaxTextureSize(int maxTextureSize)
	{
		m_maxTextureSize = maxTextureSize;
	}

	int MaxTextureSize()
	{
		return m_maxTextureSize;
	}

	// Antialiasing
	virtual void SetAntialiasing(bool antialias)
	{
		m_antialias = antialias;
	}

	virtual bool GetAntialiasing()
	{
		return m_antialias;
	}

	// Wireframe rendering
	virtual void SetWireframe(bool wireframe)
	{
		m_wireframe = wireframe;
	}

	virtual bool GetWireframe()
	{
		return m_wireframe;
	}

	CAnmGraphics::Shader *GetVertexShader()
	{
		return m_vertexShader;
	}

	CAnmGraphics::Shader *GetFragmentShader()
	{
		return m_fragmentShader;
	}
};

inline void CAnmDevice::AllocActiveLight(int index)
{
	int sz = m_activeLights.size();
	while(index >= sz)
	{
		m_activeLights.push_back(false);
		sz++;
	}
}

#endif //_anmdevice_h
