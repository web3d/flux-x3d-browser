/********************************************************************************
 * Flux
 *
 * File: anmd3dgraphics8.h
 * Description: Graphics layer definitions (DX8)
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


#ifndef _anmd3dgraphics8_h
#define _anmd3dgraphics8_h

#include "d3d8.h"
#include "dx7todx8.h" //helper header file for dx7->dx8 migration
#include "anmgraphics.h"
#include "anmprimitive.h"

class CAnmD3DGraphics8 : public CAnmGraphics
{
public :
	
	// Constructor/destructor
	CAnmD3DGraphics8(cApplication *pApp);
	virtual ~CAnmD3DGraphics8();

	// Factories
	static CAnmGraphics *CreateGraphics(cApplication *pApp);

	virtual Light *CreateLight(eAnmLightType lightType);
	virtual Material *CreateMaterial();
	virtual Fog *CreateFog();

	virtual Texture *CreateTexture(class CAnmDevice *pDevice, int stage = 0);
	virtual Texture *CreateTexture(class CAnmDevice *pDevice, class CAnmTextureData *pTextureData,
		int stage = 0, int width = 0, int height = 0);
	virtual Texture *CreateTexture(class CAnmDevice *pDevice, Image *pImage,
		int stage = 0);

	virtual Texture *CreateCubeTexture(class CAnmDevice *pDevice, int stage = 0);
	virtual Texture *CreateCubeTexture(class CAnmDevice *pDevice, class CAnmTextureData *pTextureData[6],
		int stage = 0, int pixelDim = 0);
//	virtual Texture *CreateCubeTexture(class CAnmDevice *pDevice, Image *pImage[6], int stage = 0);

	virtual bool  AddTextureDataToCubeTexture( CAnmDevice *pDevice, Texture *pTextureSurface, int iFace, 
		class CAnmTextureData *pTextureData );


	virtual RenderPrimitive *CreatePrimitive(class CAnmPrimitive *pPrimitive);

	virtual Shader *CreateShader(class CAnmShaderProgram *pShaderProgram);

	// Utility functions
	virtual void DrawBox(Point3 corners[8], Color c, class CAnmDevice *pDevice);
	virtual void DrawAxes(Point3 origin, Point3 axes[3], Color c, class CAnmDevice *pDevice);
};

class CAnmD3DLight8 : public CAnmGraphics::Light
{
protected :

	D3DLIGHT8		d3dlight;

public :

	CAnmD3DLight8()
	{
		memset(&d3dlight, 0, sizeof(D3DLIGHT8));
	}

	// CAnmGraphics::Light overrides
	virtual void SetLightType(eAnmLightType lightType)
	{
		switch (lightType)
		{
			case eAnmLightDirectional :
				d3dlight.Type = D3DLIGHT_DIRECTIONAL;
				break;
			case eAnmLightPoint :
				d3dlight.Type = D3DLIGHT_POINT;
				break;
			case eAnmLightSpot :
				d3dlight.Type = D3DLIGHT_SPOT;
				break;
			case eAnmLightBadType :
			default :
				assert(FALSE);
				break;
		}
	}

	virtual eAnmLightType GetLightType()
	{
		switch (d3dlight.Type)
		{
			case D3DLIGHT_DIRECTIONAL :
				return eAnmLightDirectional;
				break;
			case D3DLIGHT_POINT :
				return eAnmLightPoint;
				break;
			case D3DLIGHT_SPOT :
				return eAnmLightSpot;
				break;
			default :
				assert(FALSE);
				break;
		}

		return eAnmLightBadType;
	}

	virtual void SetDirection(const Point3 *pDirection)
	{
		d3dlight.Direction = * (D3DVECTOR *) pDirection;
	}

	virtual Point3 GetDirection()
	{
		return * (Point3 *) &d3dlight.Direction;
	}

	virtual void SetLocation(const Point3 *pLocation)
	{
		d3dlight.Position = *(D3DVECTOR *) pLocation;
	}

	virtual Point3 GetLocation()
	{
		return * (Point3 *) &d3dlight.Position;
	}

	virtual void SetDiffuseColor(const Color4 *pDiffuseColor)
	{
		d3dlight.Diffuse = * (D3DCOLORVALUE *) pDiffuseColor;
	}

	virtual Color4 GetDiffuseColor()
	{
		return * (Color4 *) &d3dlight.Diffuse;
	}

	virtual void SetSpecularColor(const Color4 *pSpecularColor)
	{
		d3dlight.Specular = * (D3DCOLORVALUE *) pSpecularColor;
	}

	virtual Color4 GetSpecularColor()
	{
		return * (Color4 *) &d3dlight.Specular;
	}

	virtual void SetAmbientColor(const Color4 *pAmbientColor)
	{
		d3dlight.Ambient = * (D3DCOLORVALUE *) pAmbientColor;
	}

	virtual Color4 GetAmbientColor()
	{
		return * (Color4 *) &d3dlight.Ambient;
	}

	virtual void SetRange(Float range)
	{
		d3dlight.Range = range;
	}

	virtual Float GetRange()
	{
		return d3dlight.Range;
	}

	virtual void SetAttenuation(Point3 attenuation)
	{
		d3dlight.Attenuation0 = attenuation.x;
		d3dlight.Attenuation1 = attenuation.y;
		d3dlight.Attenuation2 = attenuation.z;
	}

	virtual Point3 GetAttenuation()
	{
		return Point3(d3dlight.Attenuation0, d3dlight.Attenuation1, d3dlight.Attenuation2);
	}

	virtual void SetBeamWidth(Float beamWidth)
	{
		float theta = beamWidth * ANMLIGHT_X3DANGLEFACTOR;

		if (theta < ANMLIGHT_MINBEAMWIDTH)
			theta = ANMLIGHT_MINBEAMWIDTH;
		else if (theta > ANMLIGHT_MAXBEAMWIDTH)
			theta = ANMLIGHT_MAXBEAMWIDTH;

		d3dlight.Theta = theta;
	}

	virtual Float GetBeamWidth()
	{
		return d3dlight.Theta / ANMLIGHT_X3DANGLEFACTOR;
	}

	virtual void SetCutoffAngle(Float cutoffAngle)
	{
		float phi = cutoffAngle * ANMLIGHT_X3DANGLEFACTOR;

		if (phi < ANMLIGHT_MINBEAMWIDTH)
			phi = ANMLIGHT_MINBEAMWIDTH;
		else if (phi > ANMLIGHT_MAXBEAMWIDTH)
			phi = ANMLIGHT_MAXBEAMWIDTH;

		d3dlight.Phi = phi;
	}

	virtual Float GetCutoffAngle()
	{
		return d3dlight.Phi / ANMLIGHT_X3DANGLEFACTOR;
	}


	D3DLIGHT8 *GetD3DLight8()
	{
		return &d3dlight;
	}

} ;

class CAnmD3DMaterial8 : public CAnmGraphics::Material
{
protected :

	D3DMATERIAL8	d3dmaterial;

public :

	CAnmD3DMaterial8()
	{
		// why risk it?
		memset(&d3dmaterial, 0, sizeof(D3DMATERIAL8));
	}

	virtual void Copy(CAnmGraphics::Material *pMaterial)
	{
		CAnmD3DMaterial8 *pMaterial8 = (CAnmD3DMaterial8 *) pMaterial;

		memcpy(&d3dmaterial, pMaterial8->GetD3DMaterial8(), sizeof(D3DMATERIAL8));
	}

	virtual void SetDiffuseColor(const Color4 *pDiffuseColor)
	{
		d3dmaterial.Diffuse = *(D3DCOLORVALUE *) pDiffuseColor;
	}

	virtual Color4 GetDiffuseColor()
	{
		return * (Color4 *) &d3dmaterial.Diffuse;
	}

	virtual void SetSpecularColor(const Color4 *pSpecularColor)
	{
		d3dmaterial.Specular = *(D3DCOLORVALUE *) pSpecularColor;
	}

	virtual Color4 GetSpecularColor()
	{
		return * (Color4 *) &d3dmaterial.Specular;
	}

	virtual void SetEmissiveColor(const Color4 *pEmissiveColor)
	{
		d3dmaterial.Emissive = *(D3DCOLORVALUE *) pEmissiveColor;
	}

	virtual Color4 GetEmissiveColor()
	{
		return * (Color4 *) &d3dmaterial.Emissive;
	}

	virtual void SetAmbientColor(const Color4 *pAmbientColor)
	{
		d3dmaterial.Ambient = *(D3DCOLORVALUE *) pAmbientColor;
	}

	virtual Color4 GetAmbientColor()
	{
		return * (Color4 *) &d3dmaterial.Ambient;
	}

	virtual void SetPower(Float power)
	{
		d3dmaterial.Power = power;
	}

	virtual Float GetPower()
	{
		return d3dmaterial.Power;
	}

	D3DMATERIAL8 *GetD3DMaterial8()
	{
		return &d3dmaterial;
	}

} ;

class CAnmD3DFog8 : public CAnmGraphics::Fog 
{
public :
	
	DWORD fogcolor;
	DWORD fogmode;
	DWORD fogstart;
	DWORD fogend;
	DWORD fogdensity;


	CAnmD3DFog8()
	{
		fogcolor = 0L;
		fogmode = 0L;
		fogstart = 0L;
		fogend = 0L;
		fogdensity = 0L;
	}

	virtual void SetColor(Color fcolor)
	{
		fogcolor = fcolor.MakeDWord();
	}

	virtual void SetMode(eAnmFogMode fmode)
	{
		switch (fmode)
		{
			case eFogModeLinear :
				fogmode = D3DFOG_LINEAR;
				break;

			case eFogModeExponential :
				fogmode = D3DFOG_EXP;
				break;

			case eFogModeExponentialSquared :
				fogmode = D3DFOG_EXP2;
				break;

			default :
				assert(FALSE);
				break;
		}
	}

	virtual void SetStart(float fstart)
	{
		fogstart = *((DWORD *)&fstart);
	}

	virtual void SetEnd(float fend)
	{
		fogend = *((DWORD *)&fend);
	}

	virtual void SetDensity(float fdensity)
	{
		fogdensity = *((DWORD *)&fdensity);
	}

} ;

// CAnmTextureSurface - base texture map class

class CAnmD3DTextureSurface8 : public CAnmGraphics::Texture
{
protected :

	LPDIRECT3DTEXTURE8				 m_pTexture;

	void CopyPixels(Image *pImage);
	void CopyTextureData(class CAnmTextureData *pTextureData);

	void GenMipMap();
	void GenMipMapLevel(LPDIRECT3DSURFACE8 pParentSurface, LPDIRECT3DSURFACE8 pChildSurface);
	friend class CAnmDShowMovieTextureBlitter8;

public :

	// Constructor/destructor
	CAnmD3DTextureSurface8(class CAnmDevice *pDevice, class CAnmTextureData *pTextureData,
		int stage = 0, int width = 0, int height = 0);

	CAnmD3DTextureSurface8(class CAnmDevice *pDevice, int stage = 0);

	CAnmD3DTextureSurface8(class CAnmDevice *pDevice, Image *pImage,
		int stage = 0);
	
	~CAnmD3DTextureSurface8();

	// New methods
	static void CopyTextureDataToSurface( LPDIRECT3DSURFACE8 pSurface, CAnmTextureData *pTextureData,
			class CAnmDevice *pDevice,  bool bgrayscale = false, bool bhasalpha = false );
	virtual void InitSurface(int width, int height, bool mipmap = true);

	static int GetNumBitsInFormat(D3DFORMAT format);

	LPDIRECT3DTEXTURE8 GetTexture()
	{
		return m_pTexture;
	}
};


class CAnmD3DCubeTextureSurface8 : public CAnmGraphics::Texture
{
protected :

	LPDIRECT3DCUBETEXTURE8				 m_pTexture;
	int									 m_iPixelDim;

//	void CopyPixels(Image *pImage);

public :

	// Constructor/destructor
	CAnmD3DCubeTextureSurface8(class CAnmDevice *pDevice, class CAnmTextureData *pTextureData[6],
		int stage = 0, int pixelDim = 0 );

	CAnmD3DCubeTextureSurface8(class CAnmDevice *pDevice, int stage = 0);

//	CAnmD3DCubeTextureSurface8(class CAnmDevice *pDevice, Image *pImage[6],	int stage = 0);
	
	~CAnmD3DCubeTextureSurface8();

	// New methods
	virtual void InitSurface(int pixelDim, bool mipmap = true);
	virtual bool CopyTextureData( int iCube, class CAnmTextureData *pTextureData);

	LPDIRECT3DCUBETEXTURE8 GetTexture()
	{
		return m_pTexture;
	}
};




inline DWORD AnmNativeTextureMode(eAnmTextureMode mode)
{
	DWORD d3dmode;

	switch (mode)
	{
		case eAnmTextureModeMODULATE :
			d3dmode = D3DTOP_MODULATE;
			break;

		case eAnmTextureModeREPLACE :
			d3dmode = D3DTOP_SELECTARG2;
			break;

		case eAnmTextureModeMODULATE2X :
			d3dmode = D3DTOP_MODULATE2X;
			break;

		case eAnmTextureModeMODULATE4X :
			d3dmode = D3DTOP_MODULATE4X;
			break;

		case eAnmTextureModeADD :
			d3dmode = D3DTOP_ADD;
			break;

		case eAnmTextureModeADDSIGNED :
			d3dmode = D3DTOP_ADDSIGNED;
			break;

		case eAnmTextureModeADDSIGNED2X  :
			d3dmode = D3DTOP_ADDSIGNED2X;
			break;

		case eAnmTextureModeSUBTRACT :
			d3dmode = D3DTOP_SUBTRACT;
			break;

		case eAnmTextureModeADDSMOOTH  :
			d3dmode = D3DTOP_ADDSMOOTH;
			break;

		case eAnmTextureModeBLENDDIFFUSEALPHA :
			d3dmode = D3DTOP_BLENDDIFFUSEALPHA;
			break;

		case eAnmTextureModeBLENDTEXTUREALPHA :
			d3dmode = D3DTOP_BLENDTEXTUREALPHA;
			break;

		case eAnmTextureModeBLENDFACTORALPHA :
			d3dmode = D3DTOP_BLENDFACTORALPHA;
			break;

		case eAnmTextureModeBLENDCURRENTALPHA :
			d3dmode = D3DTOP_BLENDCURRENTALPHA;
			break;

		case eAnmTextureModeMODULATEALPHA_ADDCOLOR :
			d3dmode = D3DTOP_BLENDCURRENTALPHA;
			break;

		case eAnmTextureModeMODULATEINVALPHA_ADDCOLOR :
			d3dmode = D3DTOP_MODULATEINVALPHA_ADDCOLOR;
			break;

		case eAnmTextureModeMODULATEINVCOLOR_ADDALPHA :
			d3dmode = D3DTOP_MODULATEINVCOLOR_ADDALPHA;
			break;

		case eAnmTextureModeSELECTARG1 :
			d3dmode = D3DTOP_SELECTARG1;
			break;

		case eAnmTextureModeSELECTARG2 :
			d3dmode = D3DTOP_SELECTARG2;
			break;

		case eAnmTextureModeDOTPRODUCT3 :
			d3dmode = D3DTOP_DOTPRODUCT3;
			break;

		case eAnmTextureModeOFF :
		case eAnmTextureModeBADMODE :
		default :
			d3dmode = D3DTOP_DISABLE;
			break;
	}

	return d3dmode;
}

inline D3DTRANSFORMSTATETYPE AnmNativeTextureTransformState(int i)
{
	static D3DTRANSFORMSTATETYPE tstypes[] =
	{
		D3DTRANSFORMSTATE_TEXTURE0,
		D3DTRANSFORMSTATE_TEXTURE1,
		D3DTRANSFORMSTATE_TEXTURE2,
		D3DTRANSFORMSTATE_TEXTURE3,
		D3DTRANSFORMSTATE_TEXTURE4,
		D3DTRANSFORMSTATE_TEXTURE5,
		D3DTRANSFORMSTATE_TEXTURE6,
		D3DTRANSFORMSTATE_TEXTURE7,
	};

	assert(i >= 0 && i <= 7);

	return tstypes[i];
}

inline DWORD AnmNativeTextureAddressMode(eAnmTextureAddressMode mode)
{
	DWORD d3dmode;

	switch (mode)
	{
		case eAnmTextureAddressClamp :
			d3dmode = D3DTADDRESS_CLAMP;
			break;

		case eAnmTextureAddressWrap :
		default :
			d3dmode = D3DTADDRESS_WRAP;
			break;

	}

	return d3dmode;
}

inline DWORD AnmNativeTextureArgType(eAnmTextureArgType argtype)
{
	DWORD d3dtype;

	switch (argtype)
	{
		case eAnmTextureArgCurrent :
			d3dtype = D3DTA_CURRENT;
			break;

		case eAnmTextureArgTexture :
		default :
			d3dtype = D3DTA_TEXTURE;
			break;

	}

	return d3dtype;
}

inline DWORD AnmNativeTextureCoordinateCount(eAnmTextureCoordinateCount count)
{
	DWORD d3dcount;

	switch (count)
	{
		case eAnmTextureCount3 :
			d3dcount = D3DTTFF_COUNT3;
			break;

		case eAnmTextureCount2 :
		default :
			d3dcount = D3DTTFF_COUNT2;
			break;

	}

	return d3dcount;
}

class CAnmD3DRenderPrimitive8 : public CAnmGraphics::RenderPrimitive
{
protected :
	
	D3DPRIMITIVETYPE				 m_d3dprimtype;
	LPDIRECT3DVERTEXBUFFER8			 m_vertexBuffer;
	LPDIRECT3DINDEXBUFFER8			 m_indexBuffer;

	void CopyVerticesToBuffer(tAnmVertexArray *pVerts);

public :

	CAnmD3DRenderPrimitive8(class CAnmPrimitive *pPrimitive);
	~CAnmD3DRenderPrimitive8();

	// CAnmGraphics::RenderPrimitive overrides
	virtual void Buffer(class CAnmDevice *pDevice);
	virtual void UpdateBuffer();
	virtual void Render(class CAnmDevice *pDevice);
};

#endif //_anmd3dgraphics8_h
