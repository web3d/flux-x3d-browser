/********************************************************************************
 * Flux
 *
 * File: anmgraphics.h
 * Description: Graphics layer utilities
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

#ifndef _anmgraphics_h
#define _anmgraphics_h

// Light types and values
typedef enum eAnmLightType
{
	eAnmLightDirectional,
	eAnmLightPoint,
	eAnmLightSpot,
	eAnmLightBadType,
} eAnmLightType;

#define ANMLIGHTINDEX_BADINDEX -1
#define ANMLIGHT_DEFAULT_FALLOFF 1.0f	// D3D recommended value
#define ANMLIGHT_MINBEAMWIDTH 0.001f	// A safe minimum?
#define ANMLIGHT_MAXBEAMWIDTH PI		// The maximum
#define ANMLIGHT_X3DANGLEFACTOR 2.f

// Fog mode types
typedef enum eAnmFogMode {
	eFogModeLinear,
	eFogModeExponential,
	eFogModeExponentialSquared,
	eFogModeBadMode,
} eAnmFogMode ;

// Mesh primitive types
typedef enum eAnmPrimitiveType
{
	eAnmPrimitiveTriangleList,
	eAnmPrimitiveTriangleStrip,
	eAnmPrimitiveTriangleFan,
	eAnmPrimitivePolyLine,
	eAnmPrimitivePointList,
	eAnmPrimitiveBadType,
} eAnmPrimitiveType;

typedef tri<Point3> tAnmTriangle;

// Shader types

typedef enum eAnmShaderType {
	eAnmFragmentShader,
	eAnmVertexShader,
	eAnmShaderTypeBadType,
} eAnmShaderType ;

#define ANMCGSHADERPROTOCOL "cg:"
#define ANMGLSLANGPROTOCOL	"glslang:"
#define ANMHLSLPROTOCOL		"hlsl:"

#define ANMCGSHADEREXTENSION ".cg"
#define ANMHLSLVERTEXSHADEREXT ".vhl"
#define ANMHLSLPIXELSHADEREXT ".phl"

// The Graphics namespace

class EXPORT CAnmGraphics {
protected :

	class cApplication				*m_app;

public :

class Light
{
protected :

	// Protect the constructor; only factories can create this
	Light()
	{
	}

public :

	virtual ~Light() {}

	// Overridables - subclass MUST supply
	virtual void SetLightType(eAnmLightType lightType) PURE;
	virtual eAnmLightType GetLightType() PURE;

	virtual void SetDirection(const Point3 *pDirection) PURE;
	virtual Point3 GetDirection() PURE;

	virtual void SetLocation(const Point3 *pLocation) PURE;
	virtual Point3 GetLocation() PURE;

	virtual void SetDiffuseColor(const Color4 *pDiffuseColor) PURE;
	virtual Color4 GetDiffuseColor() PURE;

	virtual void SetSpecularColor(const Color4 *pSpecularColor) PURE;
	virtual Color4 GetSpecularColor() PURE;

	virtual void SetAmbientColor(const Color4 *pAmbientColor) PURE;
	virtual Color4 GetAmbientColor() PURE;

	virtual void SetRange(Float range) PURE;
	virtual Float GetRange() PURE;

	virtual void SetAttenuation(Point3 attenuation) PURE;
	virtual Point3 GetAttenuation() PURE;

	virtual void SetBeamWidth(Float beamWidth) PURE;
	virtual Float GetBeamWidth() PURE;

	virtual void SetCutoffAngle(Float cutoffAngle) PURE;
	virtual Float GetCutoffAngle() PURE;

} ;


class Material
{
protected :
	
	// Protect the constructor; only factories can create this
	Material()
	{
	}

public :

	virtual ~Material() {}

	// Overridables - subclass MUST supply
	virtual void Copy(Material *pMaterial) PURE;

	virtual void SetDiffuseColor(const Color4 *pDiffuseColor) PURE;
	virtual Color4 GetDiffuseColor() PURE;

	virtual void SetSpecularColor(const Color4 *pSpecularColor) PURE;
	virtual Color4 GetSpecularColor() PURE;

	virtual void SetEmissiveColor(const Color4 *pEmissiveColor) PURE;
	virtual Color4 GetEmissiveColor() PURE;

	virtual void SetAmbientColor(const Color4 *pAmbientColor) PURE;
	virtual Color4 GetAmbientColor() PURE;

	virtual void SetPower(Float power) PURE;
	virtual Float GetPower() PURE;

} ;

class Fog
{
protected :
	
	// Protect the constructor; only factories can create this
	Fog()
	{
	}

public :

	virtual ~Fog() {}

	// Overridables - subclass MUST supply
	virtual void SetColor(Color fcolor) PURE;
	virtual void SetMode(eAnmFogMode fmode) PURE;
	virtual void SetStart(float fstart) PURE;
	virtual void SetEnd(float fend) PURE;
	virtual void SetDensity(float fdensity) PURE;

} ;

class Texture : public CAnmRefObject
{
protected :

	class CAnmDevice				*m_device;
	class CAnmTextureData			*m_texturedata;
	class CAnmTextureData			*m_cubetexturedata[6];
	int								 m_stage;
	bool							 m_hasalpha;
	bool							 m_grayscale;
	int								 m_width;
	int								 m_height;

	static Texture *FindTexture(class CAnmTextureData *pTextureData,
		class CAnmDevice *pDevice, int stage = 0);
	static Texture *FindCubeTexture(class CAnmTextureData *pTextureData[6],
		class CAnmDevice *pDevice, int stage = 0);

	// Protect the constructors; only factories can create this
	Texture()
	{
		m_device = NULL;
		m_texturedata = NULL;
		for( int i=0; i<6; i++ ) { m_cubetexturedata[i] = NULL; }
		m_stage = 0;
		m_hasalpha = false;
		m_grayscale = false;
		m_width = m_height = 0;
	}

	Texture(class CAnmDevice *pDevice, int stage = 0)
	{
		m_device = pDevice;
		m_stage = stage;
		m_hasalpha = false;
		m_grayscale = false;
		for( int i=0; i<6; i++ ) { m_cubetexturedata[i] = NULL; }
		m_texturedata = NULL;
		m_width = m_height = 0;
	}

public :
	
	virtual ~Texture() {}

	// Methods
	static Texture *LoadTexture(class CAnmTextureData *pTextureData,
		class CAnmDevice *pDevice, int stage = 0, int width = 0, int height = 0);

	static Texture *LoadCubeTexture(class CAnmTextureData *pTextureData[6],
		class CAnmDevice *pDevice, int stage = 0, int width = 0, int height = 0);

	static bool AddTextureDataToCubeTexture( 
					Texture *pTextureSurface,
					int iFace,
					class CAnmTextureData *pTextureData,
					class CAnmDevice *pDevice );


	static void UnloadAllTextures();

	// Accessors
	class CAnmDevice *GetDevice()
	{
		return m_device;
	}

	class CAnmTextureData *GetTextureData()
	{
		return m_texturedata;
	}

	class CAnmTextureData *GetCubeTextureData(int i)
	{
		return m_cubetexturedata[i];
	}

	int GetStage()
	{
		return m_stage;
	}

	void SetHasAlpha(bool hasalpha)
	{
		m_hasalpha = hasalpha;
	}

	bool HasAlpha()
	{
		return m_hasalpha;
	}

	void SetIsGrayscale(bool isGrayScale)
	{
		m_grayscale = isGrayScale;
	}

	bool IsGrayscale()
	{
		return m_grayscale;
	}

	int GetWidth()
	{
		return m_width;
	}

	int GetHeight()
	{
		return m_height;
	}
};

class RenderPrimitive
{
protected :
	
	class CAnmPrimitive			*m_primitive;

	// Protect the constructor; only factories can create this
	RenderPrimitive(class CAnmPrimitive *pPrimitive)
	{
		m_primitive = pPrimitive;
	}

public :

	virtual ~RenderPrimitive() {}

	// Overridables - subclass MUST supply
	virtual void Buffer(class CAnmDevice *pDevice) PURE;
	virtual void UpdateBuffer() PURE;
	virtual void Render(class CAnmDevice *pDevice) PURE;
};


class Shader
{
protected :

	class CAnmShaderProgram				*m_shaderProgram;

	// Protect the constructor; only factories can create this
	Shader(class CAnmShaderProgram *pShaderProgram)
	{
		// Don't get into ref/unref with this, then we'd
		// have to worry about mutual unrefing
		m_shaderProgram = pShaderProgram;
	}

public :

	virtual ~Shader() {}

	static BOOL IsInlineShaderProtocol(const char *url);
	static Shader *CreateFromString(class CAnmShaderProgram *pShader, class cApplication *pApp,
		const char *url);
	static Shader *CreateFromFile(class CAnmShaderProgram *pShader, class cApplication *pApp,
		const char *url);

	// these methods must be overridden
	virtual void CompileString(class cApplication *pApp, const char *source) PURE;
	virtual void CompileFile(class cApplication *pApp, const char *filename) PURE;

	virtual eAnmReturnStatus SetValue(char *valuename, eValueType valueType, void *pValue) PURE;
};

class FragmentShader : public Shader
{
protected :

	// Protect the constructor; only factories can create this
	FragmentShader(class CAnmShaderProgram *pShaderProgram) : Shader(pShaderProgram)
	{
	}

public :

	virtual ~FragmentShader() {}

};

class VertexShader : public Shader
{
protected :

	// Protect the constructor; only factories can create this
	VertexShader(class CAnmShaderProgram *pShaderProgram) : Shader(pShaderProgram)
	{
	}

public :

	virtual ~VertexShader() {}

};

	// Constructor/destructor
	CAnmGraphics(cApplication *pApp);
	virtual ~CAnmGraphics();

	// Factories
	static CAnmGraphics *CreateGraphics(cApplication *pApp);

	virtual Light *CreateLight(eAnmLightType lightType) PURE;
	virtual Material *CreateMaterial() PURE;
	virtual Fog *CreateFog() PURE;





	virtual Texture *CreateTexture(class CAnmDevice *pDevice, int stage = 0) PURE;
	virtual Texture *CreateTexture(class CAnmDevice *pDevice, class CAnmTextureData *pTextureData,
		int stage = 0, int width = 0, int height = 0) PURE;
	virtual Texture *CreateTexture(class CAnmDevice *pDevice, Image *pImage,
		int stage = 0) PURE;


	virtual Texture *CreateCubeTexture(class CAnmDevice *pDevice, int stage = 0) PURE;
	virtual Texture *CreateCubeTexture(class CAnmDevice *pDevice, class CAnmTextureData *pTextureData[6],
		int stage = 0, int pixelDim = 0) PURE;
//	virtual Texture *CreateCubeTexture(class CAnmDevice *pDevice, Image *pImage, int stage = 0) PURE;

	virtual bool  AddTextureDataToCubeTexture( CAnmDevice *pDevice, Texture *pTextureSurface, int iFace, 
		class CAnmTextureData *pTextureData ) PURE;

	virtual RenderPrimitive *CreatePrimitive(class CAnmPrimitive *pPrimitive) PURE;

	virtual Shader *CreateShader(class CAnmShaderProgram *pShaderProgram) PURE;

	// Utility functions
	virtual void DrawBox(Point3 corners[8], Color c, class CAnmDevice *pDevice) PURE;
	virtual void DrawAxes(Point3 origin, Point3 axes[3], Color c, class CAnmDevice *pDevice) PURE;

};		// end namespace CAnmGraphics

typedef enum eAnmTextureMode {
	eAnmTextureModeMODULATE,
	eAnmTextureModeREPLACE,
	eAnmTextureModeMODULATE2X,
	eAnmTextureModeMODULATE4X,
	eAnmTextureModeADD,
	eAnmTextureModeADDSIGNED,
	eAnmTextureModeADDSIGNED2X ,
	eAnmTextureModeSUBTRACT,
	eAnmTextureModeADDSMOOTH ,
	eAnmTextureModeBLENDDIFFUSEALPHA,
	eAnmTextureModeBLENDTEXTUREALPHA,
	eAnmTextureModeBLENDFACTORALPHA,
	eAnmTextureModeBLENDCURRENTALPHA,
	eAnmTextureModeMODULATEALPHA_ADDCOLOR,
	eAnmTextureModeMODULATEINVALPHA_ADDCOLOR,
	eAnmTextureModeMODULATEINVCOLOR_ADDALPHA,
	eAnmTextureModeOFF,
	eAnmTextureModeSELECTARG1,
	eAnmTextureModeSELECTARG2,
	eAnmTextureModeDOTPRODUCT3,
	eAnmTextureModeBADMODE,
} eAnmTextureMode ;

typedef enum eAnmTextureSource {
	eAnmTextureSourceCURRENT,
	eAnmTextureSourceDIFFUSE,
	eAnmTextureSourceSPECULAR,
	eAnmTextureSourceFACTOR,
	eAnmTextureSourceBADSOURCE,
} eAnmTextureSource ;

typedef enum eAnmTextureFunction {
	eAnmTextureFunctionNONE,
	eAnmTextureFunctionCOMPLEMENT,
	eAnmTextureFunctionALPHAREPLICATE,
	eAnmTextureFunctionBADFUNCTION,
} eAnmTextureFunction ;


typedef enum eAnmTextureTexCoordIndexMode {
	eAnmTextureTexCoordPassThru,
	eAnmTextureTexCoordSphere,
	eAnmTextureCameraSpaceNormal,
	eAnmTextureCameraSpacePosition,
	eAnmTextureCameraSpaceReflectionVector,
} eAnmTextureTexCoordIndexMode ;

typedef enum eAnmTextureOptions {
	eAnmTextureAddressU,
	eAnmTextureAddressV,
	eAnmTextureFilter,
	eAnmTextureNumTextureCoordinates,
	eAnmTextureColorArg1,
	eAnmTextureColorArg2,
	eAnmTextureAlphaArg1,
	eAnmTextureAlphaArg2,
	eAnmTextureColorOp,
	eAnmTextureAlphaOp,
	eAnmTextureTexCoordIndex,
} eAnmTextureOptions ;

typedef enum eAnmTextureAddressMode {
	eAnmTextureAddressWrap,
	eAnmTextureAddressClamp,
} eAnmTextureAddressMode ;

typedef enum eAnmTextureArgType {
	eAnmTextureArgTexture,
	eAnmTextureArgCurrent,
} eAnmTextureArgType ;

typedef enum eAnmTextureCoordinateCount {
	eAnmTextureCount2,
	eAnmTextureCount3,
} eAnmTextureCoordinateCount ;


typedef enum eAnmRenderOptions {
	eAnmRenderOptionsNone					=	0x0,
	eAnmRenderOptionsUseVertexForDiffuse	=	0x1,
	eAnmRenderOptionsUseVertexForEmissive	=	0x2,
	eAnmRenderOptionsUseVertexForAmbient	=	0x4,
	eAnmRenderOptionsLighting				=	0x8,
} eAnmRenderOptions ; 



#endif //_anmgraphics_h
