/********************************************************************************
 * Flux
 *
 * File: anmd3ddevice9.cpp
 * Description: Device class for Direct3D Version 9
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
#include "anmd3ddevice9.h"
#include "anmd3dgraphics9.h"
#include "anmd3derrors.h"
#include "anmcamera.h"
#include "anmviewport.h"
#include "application.h"
#include "anmfilemanager.h"		// N.B. For WM_USERANMRESETDEVICE; really doesn't belong there
#include "d3dx9.h"
#include "anmdx9message.h"

CAnmD3DDevice9::CAnmD3DDevice9(HWND hWnd, cApplication *pApp)
: CAnmDevice(hWnd, pApp)
{
    m_pD3D           = NULL;
    m_pDevice	     = NULL;	
	m_devType		 = D3DDEVTYPE_HAL;
	m_bWindowed = TRUE;
	m_strWindowTitle = _T("Flux X3D Browser");
	m_dwMinDepthBits    = 16;
    m_dwMinStencilBits  = 0;
	m_dwCreateFlags     = 0L;
	m_dwNumAdapters		= 0;
	m_vertexShaderProfile = NULL;
	m_pixelShaderProfile = NULL;

	InitD3D();

	InitDevice();

}


void CAnmD3DDevice9::DestroyAll()
{
 	SafeUnRef(m_vertexShaderProfile);
	SafeUnRef(m_pixelShaderProfile);

	if( m_pDevice )
	{
		// Flush the textures out, causing releases we need.
		for( int i=0; i<8; i++ )
		{
			m_pDevice->SetTexture( i, NULL );
		}
		if (ExtraSafeRelease( m_pDevice ))
		{
			// ::MessageBox(NULL, "In ExtraSafeRelease; device not cleaned up", "Debug", 0);
		}
	}
	
    //SafeUnRef( m_zbuffer );
	SafeUnRef( m_viewport );

    SafeRelease( m_pD3D );

}

CAnmD3DDevice9::~CAnmD3DDevice9()
{
	DestroyAll();
}


void CAnmD3DDevice9::Clear( 
	bool bClearFrame, 
	bool bClearZ, 
	DWORD frameColor, 
	float zValue )
{
	DWORD flags = bClearFrame ? D3DCLEAR_TARGET : 0L;
	flags |= bClearZ? D3DCLEAR_ZBUFFER : 0L;

	HRESULT hr;
	hr = m_pDevice->Clear( 0, 0, flags, frameColor, zValue, 0);

	if( FAILED( hr ))
	{
		DP0("[CAnmD3DDevice9::Clear]: Clear failed\n");
	}
}

void CAnmD3DDevice9::Flip( bool bNoVSync )
{
	HRESULT hr;

	hr = m_pDevice->Present(NULL, NULL, NULL, NULL);	

	if(FAILED(hr)) {
		if(hr == D3DERR_DEVICELOST) {
			HandleDeviceLost();
		}
		if(hr == D3DERR_DRIVERINTERNALERROR) {
			m_app->PostMessage(eAppMsgResetDevice, NULL);
		}		
		//and D3DERR_INVALIDCALL
	}
}


void CAnmD3DDevice9::GetWorldMatrix( matrix4* pMat )
{
	// Pass the call to the Direct3D Device
	m_pDevice->GetTransform( 
		D3DTRANSFORMSTATE_WORLD, 
		(D3DMATRIX*)pMat );
}

void CAnmD3DDevice9::SetWorldMatrix( const matrix4& mat )
{
	// Pass the call to the Direct3D Device
	m_pDevice->SetTransform( 
		D3DTRANSFORMSTATE_WORLD, 
		(D3DMATRIX*)&mat );
}

void CAnmD3DDevice9::GetViewMatrix( matrix4* pMat )
{
	// Pass the call to the Direct3D Device
	m_pDevice->GetTransform( 
		D3DTRANSFORMSTATE_VIEW, 
		(D3DMATRIX*)pMat );
}

void CAnmD3DDevice9::SetViewMatrix( const matrix4& mat )
{
	// Pass the call to the Direct3D Device
	m_pDevice->SetTransform( 
		D3DTRANSFORMSTATE_VIEW, 
		(D3DMATRIX*)&mat );
}

void CAnmD3DDevice9::GetProjectionMatrix( matrix4* pMat )
{
	// Pass the call to the Direct3D Device
	m_pDevice->GetTransform( 
		D3DTRANSFORMSTATE_PROJECTION, 
		(D3DMATRIX*)pMat );
}

void CAnmD3DDevice9::SetProjectionMatrix( const matrix4& mat )
{
	// Pass the call to the Direct3D Device
	m_pDevice->SetTransform( 
		D3DTRANSFORMSTATE_PROJECTION, 
		(D3DMATRIX*)&mat );
}

void CAnmD3DDevice9::SetOrthoMatrix( float x0, float x1, float y0, float y1, float z0, float z1 )
{
	D3DXMATRIX matView;
	D3DXMatrixOrthoOffCenterLH( &matView, x0, x1, y0, y1, z0, z1 );
	m_pDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matView );
}

void CAnmD3DDevice9::SetViewportParams(int x, int y, int width, int height, float minz, float maxz)
{
    D3DVIEWPORT9 vp = { x, y, width, height, minz, maxz };

    HRESULT hr = m_pDevice->SetViewport( &vp );

    if( FAILED( hr ) )
		// ANMMSG_VIEWPORTSETFAILED : "Viewport setting failed"
        throw CAnmError(ANMMSG_VIEWPORTSETFAILED);

}

void CAnmD3DDevice9::BeginScene()
{
	// Pass the call to the Direct3D Device
	m_pDevice->BeginScene();
}

void CAnmD3DDevice9::EndScene()
{
	// Pass the call to the Direct3D Device
	m_pDevice->EndScene();
}

HRESULT CAnmD3DDevice9::CreateDevice()
{
	return S_OK;
}


void CAnmD3DDevice9::InitDevice()
{
	HRESULT hr;

	//store the capabilities of the created device:
	m_pDevice->GetDeviceCaps(&m_d3dCaps);
	
	m_maxTextureLevels = m_d3dCaps.MaxSimultaneousTextures;
	m_maxActiveLights = m_d3dCaps.MaxActiveLights;

	// Get shader capability info
	// N.B.: DX9 docs say that D3DX will let us query this,
	// but that must be a later SDK version than we have right now; could
	// not find the functions D3DXGetVertexShaderProfile() and
	// D3DXGetPixelShaderProfile(). So let's hard-code these and
	// hope for good results...

#if 0
	LPCSTR cp = D3DXGetVertexShaderProfile(pDevice);
	if (cp)
		m_vertexShaderProfile = new CAnmString(cp);

	cp = D3DXGetPixelShaderProfile(pDevice);
	if (cp)
		m_pixelShaderProfile = new CAnmString(cp);
#else

	m_vertexShaderProfile = new CAnmString("vs_2_0");
	m_pixelShaderProfile = new CAnmString("ps_2_0");

#endif


	// set some sensible defaults
	int coordsystem =
		m_app->GetOptions(eAnmOptionsCoordinateSystem);

	if (coordsystem == eAnmCoordinatesLeftHanded)
		m_pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);
	else
		m_pDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);

	m_maxTextureSize = 
		m_app->GetOptions(eAnmOptionsMaxTextureSize);

	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, TRUE);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_COLORVERTEX, TRUE);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_NORMALIZENORMALS, TRUE);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD);

	SetAntialiasing(m_antialias);
	SetWireframe(m_wireframe);

	matrix4 ttxfmmat;
	GetDefaultTextureTransform(&ttxfmmat);

	int maxlevels = MaxTextureLevels();
	for (int i = 0; i < maxlevels; i++)
	{
		m_pDevice->SetTexture(i, NULL);
		m_pDevice->SetTransform(AnmNativeTextureTransformState(i), (D3DMATRIX *) &ttxfmmat);
	}

	ClearLights();

	// set up the viewport
	MakeViewport();

	SetViewMatrix( matrix4::GetIdentity() );

}

DWORD
CAnmD3DDevice9::GetCreateDeviceFlags()
{
	return m_createDeviceFlags;
}

void
CAnmD3DDevice9::GetViewMatrix(D3DXMATRIX* viewMatrix)
{
	m_pDevice->GetTransform(D3DTRANSFORMSTATE_VIEW, viewMatrix);
}

void
CAnmD3DDevice9::GetProjectionMatrix(D3DXMATRIX* projMatrix)
{
	m_pDevice->GetTransform(D3DTRANSFORMSTATE_PROJECTION, projMatrix);
}

void
CAnmD3DDevice9::GetWorldMatrix(D3DXMATRIX* worldMatrix)
{
	m_pDevice->GetTransform(D3DTRANSFORMSTATE_WORLD, worldMatrix);
}

D3DMULTISAMPLE_TYPE CAnmD3DDevice9::GetMultiSampleType() 
{
	D3DMULTISAMPLE_TYPE mType = D3DMULTISAMPLE_2_SAMPLES;
	HRESULT hr = m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, m_devType,
													m_d3ddm.Format, TRUE, mType, NULL);
	if(SUCCEEDED(hr)) {
		return mType;
	}

	return D3DMULTISAMPLE_NONE;
}

//-----------------------------------------------------------------------------
// Name: GetDepthStencilFormat()
// Desc: Finds a depth/stencil format for the given device that is compatible
//       with the render target format and meets the needs of the app.
//-----------------------------------------------------------------------------
D3DFORMAT CAnmD3DDevice9::GetDepthStencilFormat()
{
	D3DFORMAT fmt = D3DFMT_UNKNOWN;

	D3DFORMAT TargetFormat = m_d3dpp.BackBufferFormat;
	INT iAdapter = D3DADAPTER_DEFAULT;

    
    if( m_dwMinDepthBits <= 15 && m_dwMinStencilBits <= 1 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, m_devType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, m_devType,
                TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
            {
				fmt = D3DFMT_D15S1;
				m_zbufferbytes = 2;
            }
        }
    }

	if( m_dwMinDepthBits <= 16 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, m_devType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, m_devType,
                TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
            {
				fmt = D3DFMT_D16;                
				m_zbufferbytes = 2;
            }
        }
    }


    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, m_devType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, m_devType,
                TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
            {
				fmt = D3DFMT_D24X8;
				m_zbufferbytes = 3;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 8 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, m_devType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, m_devType,
                TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
            {
				fmt = D3DFMT_D24S8;
				m_zbufferbytes = 3;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 4 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, m_devType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, m_devType,
                TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
            {
				fmt = D3DFMT_D24X4S4;                
				m_zbufferbytes = 3;
            }
        }
    }

    if( m_dwMinDepthBits <= 32 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, m_devType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, m_devType,
                TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
            {
				fmt = D3DFMT_D32;                
				m_zbufferbytes = 4;
            }
        }
    }

    return fmt;
}

//-----------------------------------------------------------------------------
// Name: SortModesCallback()
// Desc: Callback function for sorting display modes (used by BuildDeviceList) - DX8 Common Files Framework
//-----------------------------------------------------------------------------
static int SortModesCallback( const VOID* arg1, const VOID* arg2 )
{
    D3DDISPLAYMODE* p1 = (D3DDISPLAYMODE*)arg1;
    D3DDISPLAYMODE* p2 = (D3DDISPLAYMODE*)arg2;

    if( p1->Format > p2->Format )   return -1;
    if( p1->Format < p2->Format )   return +1;
    if( p1->Width  < p2->Width )    return -1;
    if( p1->Width  > p2->Width )    return +1;
    if( p1->Height < p2->Height )   return -1;
    if( p1->Height > p2->Height )   return +1;

    return 0;
}

//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device initialization, this code checks the display device
//       for some minimum set of capabilities
//
//
// pCaps are the capabilities of the device that is to be created (has not been created yet)
// dwBehaviour are the behaviour flags for the type of vertex processing to be created, i.e. software, mixed, hardware or pure
// format is the currently selected desktop display mode format.
//-----------------------------------------------------------------------------
HRESULT CAnmD3DDevice9::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
	UNREFERENCED_PARAMETER( Format );
    UNREFERENCED_PARAMETER( dwBehavior );
    UNREFERENCED_PARAMETER( pCaps );

	BOOL bCapsAcceptable = TRUE;

	if (dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING ||
					  dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING)
	{
		if (pCaps->VertexProcessingCaps != 0)
			return S_OK;
		else
			return E_FAIL;
	}
	else
		return S_OK;

//	else if (dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING)
//	{
//		return S_OK;
//	}

	// bCapsAcceptable = pCaps->VertexShaderVersion >= D3DVS_VERSION(1, 0) && bCapsAcceptable;
	// bCapsAcceptable = pCaps->VertexShaderVersion >= D3DVS_VERSION(2, 0) && bCapsAcceptable;
	// bCapsAcceptable = pCaps->PixelShaderVersion  >= D3DPS_VERSION(2, 0) && bCapsAcceptable; 

	return E_FAIL;
}

//-----------------------------------------------------------------------------
// Name: FindDepthStencilFormat()
// Desc: Finds a depth/stencil format for the given device that is compatible
//       with the render target format and meets the needs of the app.
//-----------------------------------------------------------------------------
BOOL CAnmD3DDevice9::FindDepthStencilFormat( UINT iAdapter, D3DDEVTYPE DeviceType,
    D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat )
{
    if( m_dwMinDepthBits <= 16 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D16;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 15 && m_dwMinStencilBits <= 1 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D15S1;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 8 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24S8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 4 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X4S4;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 32 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
        {
            if( SUCCEEDED( m_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D32;
                return TRUE;
            }
        }
    }

    return FALSE;
}


void CAnmD3DDevice9::DrawTextString( 
	int x, int y, 
	DWORD color, const char * str )
{
	if (!m_font.IsValid())
		m_font.Init(m_pDevice);

	if (!m_font.IsValid())
		return;

	LPDIRECT3DPIXELSHADER9 curPixelShader;
	m_pDevice->GetPixelShader(&curPixelShader);
	if (curPixelShader != NULL)
	{
		m_pDevice->SetPixelShader(NULL);
	}

	RECT rct;
    ZeroMemory( &rct, sizeof(rct) );       

	LPD3DXFONT pD3DXFont = m_font.GetD3DXFont();


#ifdef UNKNOWN_DX9_SDK
	pD3DXFont->Begin();
#else
	BeginScene();
#endif

	rct.left   = 2;
	static char buff[1024];
	LPDIRECT3DSURFACE9 pBackBuffer;
		if(SUCCEEDED(m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer))) {
		D3DSURFACE_DESC desc;
		pBackBuffer->GetDesc(&desc);
		rct.right  = desc.Width - 20;
		rct.top = y;
		rct.bottom = y+20;
		strcpy(buff, str);
		char* pTok = strtok(buff, "\n");
		while(pTok)
		{			
#ifdef UNKNOWN_DX9_SDK
			pD3DXFont->DrawText(pTok, strlen(pTok), &rct, 0, color);
#else
			pD3DXFont->DrawText( NULL, pTok, strlen(pTok), &rct, 0, color);

#endif


			rct.top += 20;
			rct.bottom += 20;
			pTok = strtok(NULL, "\n");
		}
		pBackBuffer->Release();
	}

#ifdef UNKNOWN_DX9_SDK
	pD3DXFont->End();
#else
	EndScene();
#endif


	if (curPixelShader != NULL)
		m_pDevice->SetPixelShader(curPixelShader);

}


void CAnmD3DDevice9::InitD3D()
{
	UpdateRectangles();

	SafeRelease( m_pD3D );
	SafeRelease( m_pDevice );

	HRESULT hr;

	// Get a pointer to the Direct3D api
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if( m_pD3D == NULL )
	{
		// ANMMSG_D3DCREATE9FAILED : "Direct3DCreate9 failed"
        throw CAnmError(ANMMSG_D3DCREATE9FAILED);
	}

	//Select the current display mode from the default device.	
	hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_d3ddm);
	if(FAILED(hr)) {
		// ANMMSG_GETADAPTERDISPLAYMODEFAILED : "GetAdapterDisplayMode failed"
		throw CAnmError(ANMMSG_GETADAPTERDISPLAYMODEFAILED);
	}

	//Set up the application behaviour
	memset(&m_d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.BackBufferHeight = 0;//use values from hWnd
	m_d3dpp.BackBufferWidth = 0;//use values from hWnd
	m_d3dpp.BackBufferFormat = m_d3ddm.Format;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.MultiSampleType = GetMultiSampleType();
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.hDeviceWindow = m_hWnd;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = GetDepthStencilFormat();
	m_d3dpp.Flags = 0;//Back Buffer is not lockable.

	//check to see if we can expect hardware acceleration using the current device parameters
	m_bIsHardware = true;
	hr = m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, m_devType, m_d3dpp.BackBufferFormat,
									m_d3dpp.BackBufferFormat, TRUE);
	if(FAILED(hr)) {
		//goto the reference rasterizer
		m_devType = D3DDEVTYPE_REF;
		hr = m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, m_devType, m_d3dpp.BackBufferFormat,
									m_d3dpp.BackBufferFormat, TRUE);
		if(FAILED(hr)) {
			//device cannot be created
			// ANMMSG_NOAPPROPRIATEDEVICE : "An appropriate device cannot be created"
			throw CAnmError(ANMMSG_NOAPPROPRIATEDEVICE);
		}

		m_bIsHardware = false;
	}

	//Confirm that we can do the things we want to do with the proposed device:
	//Get the capabilities of the proposed device
	D3DCAPS9 pCaps;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, m_devType, &pCaps);
	m_createDeviceFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING  | D3DCREATE_MULTITHREADED;

	hr = ConfirmDevice(&pCaps, m_createDeviceFlags, m_d3dpp.BackBufferFormat);
	if(FAILED(hr)) {
		m_createDeviceFlags = D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
		hr = ConfirmDevice(&pCaps, m_createDeviceFlags, m_d3dpp.BackBufferFormat);
		if(FAILED(hr)) {
			m_createDeviceFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
			hr = ConfirmDevice(&pCaps, m_createDeviceFlags, m_d3dpp.BackBufferFormat);
			if(FAILED(hr)) {
				if(m_devType == D3DDEVTYPE_HAL) {
					//do software vertex processing with REF device
					m_devType = D3DDEVTYPE_REF;
					m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, m_devType, &pCaps);
					hr = ConfirmDevice(&pCaps, m_createDeviceFlags, m_d3dpp.BackBufferFormat);
					if(FAILED(hr)) {
						// ANMMSG_COULDNTCONFIRMDEVICE : "Could not confirm device"
						throw CAnmError(ANMMSG_COULDNTCONFIRMDEVICE);
					}
				}					
			}
		}
	}
	
	hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, m_devType, m_hWnd,
			m_createDeviceFlags, &m_d3dpp, &m_pDevice);

	if(FAILED(hr)) {
			char *cp = AnmD3DError(hr);

			// ANMMSG_ERRORCREATINGDEVICE : "Error creating device: %s"
			char *errmsg = (char *) malloc(strlen(ANMMSG_ERRORCREATINGDEVICE) + 
				strlen(cp) + 
				1);

			sprintf(errmsg, ANMMSG_ERRORCREATINGDEVICE, cp);

			CAnmError err(errmsg);

			delete errmsg;

			throw err;
	}	
}

void CAnmD3DDevice9::HandleSize(int newwidth, int newheight)
{
	// Update the screen/viewport rectangles with 
	// the new window
	if (!UpdateRectangles())
		return;
	
	m_viewport->HandleSize(newwidth, newheight);

	if (!m_app->PausedForResize())
	{
		Reset();
	}
}


void CAnmD3DDevice9::HandleMove()
{
	// Update the rectangles used in blitting
	// UpdateRectangles();
}

void CAnmD3DDevice9::HandleDeviceLost()
{
	// reinitialize font resource
	m_font.HandleLostDevice();
	
	// wait a bit and try again
	Sleep(50);
	m_app->PostMessage(eAppMsgResetDevice, NULL);
}

void CAnmD3DDevice9::Reset()
{
	m_app->Pause();

	HRESULT hr = m_pDevice->TestCooperativeLevel();

	if (FAILED(hr))
	{
		if (hr == D3DERR_DEVICELOST)
		{
			HandleDeviceLost();
			return;
		}
		else
		{
			int debug = 1;
		}
	}

	// N.B.: we seem to need to call this *before* reset, despite the DX documentation
	m_font.HandleLostDevice();

	m_d3dpp.BackBufferWidth = m_rcViewportRect.right - m_rcViewportRect.left;
	m_d3dpp.BackBufferHeight = m_rcViewportRect.bottom - m_rcViewportRect.top;

	hr = m_pDevice->Reset(&m_d3dpp);

	if (FAILED(hr))
	{
		char *cp = AnmD3DError(hr);
		if(hr == D3DERR_DEVICELOST) {
				HandleDeviceLost();
		}
		else {
			//DX9 documentation says that the application should pop-up message
			//saying that user should contact adapter manufacturer and that 
			//the application should end immediately.
			// ANMMSG_ERRORRESETDEVICE : "Error on Reset Device: %s"
			char *errmsg = (char *) malloc(strlen(ANMMSG_ERRORRESETDEVICE) + 
				strlen(cp) + 
				1);


			sprintf(errmsg, ANMMSG_ERRORRESETDEVICE, cp);

			CAnmError err(errmsg);

			delete errmsg;

			throw err;
		}
	}
	else
	{
		m_font.HandleResetDevice();

		InitDevice();

		m_app->UnPause();
	}	
}

bool CAnmD3DDevice9::IsDeviceReady()
{
	return m_pDevice != NULL;
}


void CAnmD3DDevice9::SetAntialiasing(bool antialias)
{
	HRESULT hr = S_FALSE;

	if (m_pDevice)
	{
		hr = m_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, (BOOL) antialias);
	}

	CAnmDevice::SetAntialiasing(antialias);
}

void CAnmD3DDevice9::SetWireframe(bool wireframe)
{
	HRESULT hr = S_FALSE;

	if (m_pDevice)
	{
		hr = m_pDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, wireframe ?
			D3DFILL_WIREFRAME : D3DFILL_SOLID);
	}

	CAnmDevice::SetWireframe(wireframe);
}


// Materials help
void CAnmD3DDevice9::SetMaterial(CAnmGraphics::Material *pMaterial)
{
	CAnmD3DMaterial9 *pMaterial9 = (CAnmD3DMaterial9 *) pMaterial;

	D3DMATERIAL9 *pD3DMaterial9 = pMaterial9->GetD3DMaterial9();

	HRESULT hr = m_pDevice->SetMaterial(pD3DMaterial9);
}


// Textures
void CAnmD3DDevice9::SetTexture(int stage, CAnmGraphics::Texture *pTexture)
{
	CAnmD3DTextureSurface9 *pTextureSurface = (CAnmD3DTextureSurface9 *) pTexture;

	HRESULT hr = m_pDevice->SetTexture(stage, pTextureSurface ? pTextureSurface->GetTexture() : NULL);

	if (!SUCCEEDED(hr))
	{
		int foo = 1;
	}
}

void CAnmD3DDevice9::SetTextureOption(int stage, eAnmTextureOptions option, int value)
{
	DWORD d3dmode = 0L;
	eAnmTextureTexCoordIndexMode texCoordMode = eAnmTextureTexCoordPassThru;

	switch (option)
	{
		case eAnmTextureAddressU :
			m_pDevice->SetSamplerState(stage, D3DSAMP_ADDRESSU, AnmNativeTextureAddressMode((eAnmTextureAddressMode)value));
			break;

		case eAnmTextureAddressV :
			m_pDevice->SetSamplerState(stage, D3DSAMP_ADDRESSV, AnmNativeTextureAddressMode((eAnmTextureAddressMode)value));
			break;

		case eAnmTextureFilter :
			if (value)
			{
				m_pDevice->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				m_pDevice->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
				m_pDevice->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			}
			else
			{
				m_pDevice->SetSamplerState(stage, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				m_pDevice->SetSamplerState(stage, D3DSAMP_MINFILTER, D3DTEXF_POINT);
				m_pDevice->SetSamplerState(stage, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			}
			break;

		case eAnmTextureNumTextureCoordinates :
			m_pDevice->SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, AnmNativeTextureCoordinateCount((eAnmTextureCoordinateCount)value));
			break;

		case eAnmTextureColorArg1 :
			m_pDevice->SetTextureStageState(stage, D3DTSS_COLORARG1, AnmNativeTextureArgType((eAnmTextureArgType)value));
			break;

		case eAnmTextureColorArg2 :
			m_pDevice->SetTextureStageState(stage, D3DTSS_COLORARG2, AnmNativeTextureArgType((eAnmTextureArgType)value));
			break;

		case eAnmTextureAlphaArg1 :
			m_pDevice->SetTextureStageState(stage, D3DTSS_ALPHAARG1, AnmNativeTextureArgType((eAnmTextureArgType)value));
			break;

		case eAnmTextureAlphaArg2 :
			m_pDevice->SetTextureStageState(stage, D3DTSS_ALPHAARG2, AnmNativeTextureArgType((eAnmTextureArgType)value));
			break;

		case eAnmTextureColorOp :
			d3dmode = AnmNativeTextureMode((eAnmTextureMode) value);
			m_pDevice->SetTextureStageState(stage, D3DTSS_COLOROP,   d3dmode );
			break;

		case eAnmTextureAlphaOp :
			d3dmode = AnmNativeTextureMode((eAnmTextureMode) value);
			m_pDevice->SetTextureStageState(stage, D3DTSS_ALPHAOP,  d3dmode );
			break;

		case eAnmTextureTexCoordIndex :

			texCoordMode = (eAnmTextureTexCoordIndexMode) value;

			switch (texCoordMode)
			{
				case eAnmTextureTexCoordPassThru :
					d3dmode = D3DTSS_TCI_PASSTHRU;
					break;

				// Both sphere and camera space normal use same D3D mode but different texture transform
				case eAnmTextureTexCoordSphere :
				case eAnmTextureCameraSpaceNormal :
					d3dmode = D3DTSS_TCI_CAMERASPACENORMAL;
					break;

				case eAnmTextureCameraSpacePosition :
					d3dmode = D3DTSS_TCI_CAMERASPACEPOSITION;
					break;
				case eAnmTextureCameraSpaceReflectionVector :
					d3dmode = D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR | stage;
					break;

				default :
					assert(FALSE);
					break;
			}

			m_pDevice->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, d3dmode);

			if (texCoordMode == eAnmTextureTexCoordSphere)
			{
				matrix4 texMat = matrix4::GetIdentity();
				texMat._11 = -0.5;		// X3D->D3D flip z
				texMat._22 = -0.5;		// X3D->D3D flip y
				texMat._41 = -0.5;		// X3D->D3D flip z
				texMat._42 = 0.5;		// X3D->D3D flip y

				m_pDevice->SetTransform(AnmNativeTextureTransformState(stage), (D3DMATRIX*)&texMat);
			}
			else if (texCoordMode == eAnmTextureCameraSpaceReflectionVector )
			{
				// For Cube Maps, we need to lay in the 3D Transform of the ViewMatrix, Inverted.
				//
				matrix4 texMat = matrix4::GetIdentity();

				GetViewMatrix( &texMat );

				texMat._41 = 0.0;
				texMat._42 = 0.0;
				texMat._43 = 0.0;

				// Invert it.
				//
				float tmp;
				tmp = texMat._21;
				texMat._21 = texMat._12;
				texMat._12 = tmp;
				tmp = texMat._23;
				texMat._23 = texMat._32;
				texMat._32 = tmp;
				tmp = texMat._31;
				texMat._31 = texMat._13;
				texMat._13 = tmp;
				m_pDevice->SetTransform(AnmNativeTextureTransformState(stage), (D3DMATRIX*)&texMat);
			}


			break;

		default :
			assert(FALSE);
			break;
	}

}

void CAnmD3DDevice9::SetTextureTransform(int stage, matrix4 *pTT)
{
	assert(pTT);

	m_pDevice->SetTransform(AnmNativeTextureTransformState(stage), 
			(D3DMATRIX*) pTT);
}


void CAnmD3DDevice9::GetDefaultTextureTransform(matrix4 *pDefaultTT)
{
	assert(pDefaultTT);

	// global texture transform matrix to flip V component of texture coords
	matrix4 ttxfmmat;
	ttxfmmat.ToScale(Point3(1, -1, 1));
	
	matrix4 mat;
	mat.ToTranslation(Point3(0, -1, 0));
	ttxfmmat = mat * ttxfmmat;
	// N.B.: don't ask
	ttxfmmat._31 = ttxfmmat._41;
	ttxfmmat._32 = ttxfmmat._42;

	*pDefaultTT = ttxfmmat;
}

// Lighting help
void CAnmD3DDevice9::SetLight(int index, CAnmGraphics::Light *pLight)
{
	CAnmD3DLight9 *pLight9 = (CAnmD3DLight9 *) pLight;

	D3DLIGHT9 *pD3DLight9= pLight9->GetD3DLight9();

	HRESULT hr = m_pDevice->SetLight(index, pD3DLight9);
}

void CAnmD3DDevice9::LightEnable(int index, bool on)
{
	HRESULT hr = m_pDevice->LightEnable(index, on);
}

void CAnmD3DDevice9::SetLighting(bool on)
{
	HRESULT hr = m_pDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, on ? TRUE : FALSE);
}

void CAnmD3DDevice9::EnableFog(bool on)
{
	HRESULT hr;

	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, on ? TRUE : FALSE);
	// hr = m_pDevice->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, on ? TRUE : FALSE);
}

void CAnmD3DDevice9::SetFog(CAnmGraphics::Fog *pFog)
{
	assert(pFog);

	CAnmD3DFog9 *pFog9 = (CAnmD3DFog9 *) pFog;

	HRESULT hr;

	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, pFog9->fogcolor);
	// hr = m_pDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, pFog7->fogmode);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_FOGVERTEXMODE, pFog9->fogmode);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_FOGSTART, pFog9->fogstart);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_FOGEND, pFog9->fogend);
	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_FOGDENSITY, pFog9->fogdensity);
}

// Render states
void CAnmD3DDevice9::SetZBuffering(bool enabled)
{
	HRESULT hr;

	hr = m_pDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, enabled ? TRUE : FALSE);
}

void CAnmD3DDevice9::SetRenderOptions(eAnmRenderOptions options)
{
	HRESULT hr;

	if (options & eAnmRenderOptionsUseVertexForDiffuse)
		hr = m_pDevice->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	else
		hr = m_pDevice->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

	if (options & eAnmRenderOptionsUseVertexForEmissive)
		hr = m_pDevice->SetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_COLOR1);
	else
		hr = m_pDevice->SetRenderState(D3DRENDERSTATE_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

	if (options & eAnmRenderOptionsUseVertexForAmbient)
		hr = m_pDevice->SetRenderState(D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
	else
		hr = m_pDevice->SetRenderState(D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

	if (options & eAnmRenderOptionsLighting)
		hr = m_pDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
	else
		hr = m_pDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
}

// Alpha blending
void CAnmD3DDevice9::SetAlphaBlending(bool alphablending)
{
	if (alphablending)
	{
		m_pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
	}
	else
	{
		m_pDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	}
}

// Programmable shaders
void CAnmD3DDevice9::SetVertexShader(CAnmGraphics::Shader *pVertexShader)
{
	m_vertexShader = pVertexShader;

	CAnmD3DVertexShader9 *pShader9 = (CAnmD3DVertexShader9 *) pVertexShader;

	if (pShader9)
	{
		LPDIRECT3DVERTEXSHADER9 pD3DShader9 = pShader9->GetVertexShader();

		HRESULT hr = m_pDevice->SetVertexShader(pD3DShader9);

		if (SUCCEEDED(hr))
		{
			pShader9->SetupConstants(this);
		}
	}
	else
		m_pDevice->SetVertexShader(NULL);
}

void CAnmD3DDevice9::SetFragmentShader(CAnmGraphics::Shader *pFragmentShader)
{
	m_fragmentShader = pFragmentShader;

	CAnmD3DFragmentShader9 *pShader9 = (CAnmD3DFragmentShader9 *) pFragmentShader;

	if (pShader9)
	{
		LPDIRECT3DPIXELSHADER9 pD3DShader9 = pShader9->GetPixelShader();

		HRESULT hr = m_pDevice->SetPixelShader(pD3DShader9);

		if (SUCCEEDED(hr))
		{
			pShader9->SetupConstants(this);
		}
	}
	else
		m_pDevice->SetPixelShader(NULL);
}
