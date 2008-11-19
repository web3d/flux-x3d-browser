/********************************************************************************
 * Flux
 *
 * File: anmd3ddevice8.h
 * Description: Device class for Direct3D Version 8
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

#ifndef _anmd3ddevice8_h
#define _anmd3ddevice8_h

#include "d3d8.h"
#include "dx7todx8.h" //helper header file for dx7->dx8 migration
#include "anmdevice.h"
#include "d3dx8.h"

//-----------------------------------------------------------------------------
// Name: struct D3DModeInfo
// Desc: Structure for holding information about a display mode - DX8 Common Files Framework
//-----------------------------------------------------------------------------
struct D3DModeInfo
{
    DWORD      Width;      // Screen width in this mode
    DWORD      Height;     // Screen height in this mode
    D3DFORMAT  Format;     // Pixel format in this mode
    DWORD      dwBehavior; // Hardware / Software / Mixed vertex processing
    D3DFORMAT  DepthStencilFormat; // Which depth/stencil format to use with this mode
};

//-----------------------------------------------------------------------------
// Name: struct D3DDeviceInfo
// Desc: Structure for holding information about a Direct3D device, including
//       a list of modes compatible with this device - DX8 Common Files Framework
//-----------------------------------------------------------------------------
struct D3DDeviceInfo
{
    // Device data
    D3DDEVTYPE   DeviceType;      // Reference, HAL, etc.
    D3DCAPS8     d3dCaps;         // Capabilities of this device
    const TCHAR* strDesc;         // Name of this device
    BOOL         bCanDoWindowed;  // Whether this device can work in windowed mode

    // Modes for this device
    DWORD        dwNumModes;
    D3DModeInfo  modes[150];

    // Current state
    DWORD        dwCurrentMode;
    BOOL         bWindowed;
    D3DMULTISAMPLE_TYPE MultiSampleTypeWindowed;
    D3DMULTISAMPLE_TYPE MultiSampleTypeFullscreen;
};


//-----------------------------------------------------------------------------
// Name: struct D3DAdapterInfo
// Desc: Structure for holding information about an adapter, including a list
//       of devices available on this adapter - DX8 Common Files Framework
//-----------------------------------------------------------------------------
struct D3DAdapterInfo
{
    // Adapter data
    D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
    D3DDISPLAYMODE d3ddmDesktop;      // Desktop display mode for this adapter

    // Devices for this adapter
    DWORD          dwNumDevices;
    D3DDeviceInfo  devices[5];

    // Current state
    DWORD          dwCurrentDevice;
};


//-----------------------------------------------------------------------------
// Name: struct sAnmD3DFontResource8 - encapsulates fonts for status display
//-----------------------------------------------------------------------------

struct sAnmD3DFont8
{
public :

	HFONT m_hFont;
	LPD3DXFONT m_pD3DXFont;


	sAnmD3DFont8()
	{
		m_hFont = NULL;
		m_pD3DXFont = NULL;
	}

	~sAnmD3DFont8()
	{
		if (m_hFont != NULL)
			DeleteObject(m_hFont);

		SafeRelease(m_pD3DXFont);
	}

	// Hardcode this for just right now...
	void Init(LPDIRECT3DDEVICE8 pDevice)
	{
		if (m_hFont == NULL)
			m_hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
								  ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								  ANTIALIASED_QUALITY, FF_DONTCARE, "Arial" );      

		if (m_hFont == NULL)
			return;

		HRESULT hr;
		if (m_pD3DXFont == NULL)
			hr = D3DXCreateFont( pDevice, m_hFont, &m_pD3DXFont );
	}

	bool IsValid()
	{
		return m_pD3DXFont != NULL;
	}

	LPD3DXFONT GetD3DXFont()
	{
		return m_pD3DXFont;
	}

	HRESULT HandleLostDevice()
	{
		if (m_pD3DXFont)
		{
			HRESULT hr = m_pD3DXFont->OnLostDevice();
			return hr;
		}

		return D3DERR_INVALIDCALL;
	}

	HRESULT HandleResetDevice()
	{
		if (m_pD3DXFont)
		{
			return m_pD3DXFont->OnResetDevice();
		}

		return D3DERR_INVALIDCALL;
	}
} ;


class CAnmD3DDevice8 : public CAnmDevice
{

protected:

	// main D3D objects
    LPDIRECT3DDEVICE8       m_pDevice;
	LPDIRECT3D8				m_pD3D;
	D3DDEVTYPE				m_devType;

	D3DCAPS8				m_d3dCaps;
	D3DPRESENT_PARAMETERS   m_d3dpp;
	DWORD					m_createDeviceFlags;

	//Device enumeration stuff
	D3DAdapterInfo    m_Adapters[10];
    DWORD             m_dwNumAdapters;
    DWORD             m_dwAdapter;
	HRESULT ConfirmDevice(D3DCAPS8*,DWORD,D3DFORMAT);	
	BOOL m_bWindowed;
	BOOL FindDepthStencilFormat( UINT, D3DDEVTYPE, D3DFORMAT, D3DFORMAT*);	
	char* m_strWindowTitle;
	DWORD m_dwMinDepthBits;
    DWORD m_dwMinStencilBits;
	DWORD             m_dwCreateFlags;     // Indicate sw or hw vertex processing
	D3DSURFACE_DESC   m_d3dsdBackBuffer;   // Surface desc of the backbuffer
	D3DDISPLAYMODE	  m_d3ddm;
	sAnmD3DFont8			m_font;

	D3DMULTISAMPLE_TYPE   GetMultiSampleType();
	D3DFORMAT GetDepthStencilFormat();

	// helpers to make/update/destroy our D3D objects
	void InitDevice();
	void DestroyAll();
	void HandleDeviceLost();

	// initialization
	void InitD3D();

	// Overridables
	virtual void LightEnable(int index, bool on);

public:

	CAnmD3DDevice8(HWND hWnd, class cApplication *pApp);
	~CAnmD3DDevice8();

	// CAnmDevice overrides
	virtual void BeginScene();
	virtual void EndScene();

	virtual void GetWorldMatrix( matrix4* pMat );
	virtual void SetWorldMatrix( const matrix4& mat );

	virtual void GetViewMatrix( matrix4* pMat );
	virtual void SetViewMatrix( const matrix4& mat );

	virtual void GetProjectionMatrix( matrix4* pMat );
	virtual void SetProjectionMatrix( const matrix4& mat );
	virtual void SetOrthoMatrix( float x0, float x1, float y0, float y1, float z0, float z1 );

	virtual void SetViewportParams(int x, int y, int width, int height, float minz, float maxz);

	// Drawing Routines
	virtual void Clear( 
		bool bClearFrame, 
		bool bClearZ, 
		DWORD frameColor = 0, 
		float zValue = 0.f );

	virtual void Flip( bool bNoVSync = false );

	virtual void DrawTextString( int x, int y, DWORD color, const char * str );

	// Materials
	virtual void SetMaterial(CAnmGraphics::Material *pMaterial);

	// Textures
	virtual void SetTexture(int stage, class CAnmGraphics::Texture *pTexture);
	virtual void SetTextureOption(int stage, eAnmTextureOptions option,
		int value);
	virtual void SetTextureTransform(int stage, matrix4 *pTT);
	virtual void GetDefaultTextureTransform(matrix4 *pDefaultTT);

	// Lighting
	virtual void SetLight(int index, CAnmGraphics::Light *pLight);
	virtual void SetLighting(bool on);

	// Fog
	virtual void EnableFog(bool on);
	virtual void SetFog(CAnmGraphics::Fog *pFog);

	// Alpha blending
	virtual void SetAlphaBlending(bool alphablending);

	// Z-buffer on/off
	virtual void SetZBuffering(bool enabled);

	// Antialiasing
	virtual void SetAntialiasing(bool antialias);

	// Wireframe rendering
	virtual void SetWireframe(bool wireframe);

	// Options
	virtual void SetRenderOptions(eAnmRenderOptions options);

	// Interface with windowing
	virtual void HandleSize(int newwidth, int newheight);
	virtual void HandleMove();
	virtual void Reset();
	virtual bool IsDeviceReady();

	// Shaders
	virtual void SetVertexShader(CAnmGraphics::Shader *pVertexShader);
	virtual void SetFragmentShader(CAnmGraphics::Shader *pFragmentShader);

	// Capabilities
	virtual DWORD GetMaxPrimitiveCount()
	{
		return m_d3dCaps.MaxPrimitiveCount;
	}

	// Accessors
	LPDIRECT3DDEVICE8 GetD3DDevice()
	{
		return m_pDevice;
	}	

	LPDIRECT3D8 GetD3D()
	{
		return m_pD3D;
	}

	D3DCAPS8& GetD3DDesc()
	{
		return m_d3dCaps;
	}

	D3DFORMAT GetAdapterFormat()
	{
		return m_d3dpp.BackBufferFormat;
	}


	DWORD GetCreateDeviceFlags();

	void GetWorldMatrix(D3DXMATRIX*);
	void GetViewMatrix(D3DXMATRIX*);
	void GetProjectionMatrix(D3DXMATRIX*);

};

#endif //_anmd3ddevice8_h
