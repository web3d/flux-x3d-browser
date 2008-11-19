/********************************************************************************
 * Flux
 *
 * File: anmgraphics.cpp
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
 
#include "stdafx.h"
#include "anmdevice.h"
#include "anmprimitive.h"
#include "anmplatform.h"

CAnmGraphics *CAnmGraphics::CreateGraphics(cApplication *pApp)
{
	assert(pApp);

	CAnmPlatform *pPlatform = pApp->GetPlatform();
	assert(pPlatform);

	return pPlatform->CreateGraphics(pApp);
}

CAnmGraphics::CAnmGraphics(cApplication *pApp)
{
	m_app = pApp;
}

CAnmGraphics::~CAnmGraphics()
{
}


// Texture cache management - texture surface level (also cached at the TextureData level)

CAnmGraphics::Texture *CAnmGraphics::Texture::FindTexture(class CAnmTextureData *pTextureData,
		class CAnmDevice *pDevice, int stage)
{
	cApplication *pApp = pDevice->GetApp();
	assert(pApp);

	tAnmTextureArray *pTextureArray = pApp->GetCachedTextures();
	assert(pTextureArray);

	for (int i = 0; i < pTextureArray->size(); i++)
	{
		if ((*pTextureArray)[i]->GetTextureData() == pTextureData &&
			(*pTextureArray)[i]->GetDevice() == pDevice &&
			(*pTextureArray)[i]->GetStage() == stage)
			return (*pTextureArray)[i];
	}

	return NULL;
}


// Texture cache management - texture surface level (also cached at the TextureData level)

CAnmGraphics::Texture *CAnmGraphics::Texture::FindCubeTexture(class CAnmTextureData *pTextureData[6],
		class CAnmDevice *pDevice, int stage)
{
	cApplication *pApp = pDevice->GetApp();
	assert(pApp);

	tAnmTextureArray *pTextureArray = pApp->GetCachedCubeTextures();
	assert(pTextureArray);

	for (int i = 0; i < pTextureArray->size(); i++)
	{
		if (
			(*pTextureArray)[i]->GetCubeTextureData(0) == pTextureData[0] &&
			(*pTextureArray)[i]->GetCubeTextureData(1) == pTextureData[1] &&
			(*pTextureArray)[i]->GetCubeTextureData(2) == pTextureData[2] &&
			(*pTextureArray)[i]->GetCubeTextureData(3) == pTextureData[3] &&
			(*pTextureArray)[i]->GetCubeTextureData(4) == pTextureData[4] &&
			(*pTextureArray)[i]->GetCubeTextureData(5) == pTextureData[5] &&
			(*pTextureArray)[i]->GetDevice() == pDevice &&
			(*pTextureArray)[i]->GetStage() == stage)
			return (*pTextureArray)[i];
	}

	return NULL;
}



CAnmGraphics::Texture *CAnmGraphics::Texture::LoadTexture(class CAnmTextureData *pTextureData,
		class CAnmDevice *pDevice, int stage, int width, int height)
{
	CAnmGraphics::Texture *pTexture = NULL;

	if ((pTexture = FindTexture(pTextureData, pDevice, stage)) != NULL)
		return pTexture;
	else
	{

		assert(pDevice);

		cApplication *pApp = pDevice->GetApp();
		assert(pApp);

		CAnmGraphics *pGraphics = pApp->GetGraphics();
		assert(pGraphics);

		pTexture = pGraphics->CreateTexture(pDevice, pTextureData, stage, width, height);

		if (pTexture)
		{
			cApplication *pApp = pDevice->GetApp();
			assert(pApp);

			tAnmTextureArray *pTextureArray = pApp->GetCachedTextures();
			assert(pTextureArray);

			pTextureArray->push_back(pTexture);
		}

		return pTexture;
	}
}


bool CAnmGraphics::Texture::AddTextureDataToCubeTexture( 
					CAnmGraphics::Texture *pTextureSurface,
					int iFace,
					class CAnmTextureData *pTextureData,
					class CAnmDevice *pDevice )
{
	bool bTextureAdded = false;

	if( pDevice &&
		pTextureData && 
		pTextureSurface ) {


		assert(pDevice);

		cApplication *pApp = pDevice->GetApp();
		assert(pApp);

		CAnmGraphics *pGraphics = pApp->GetGraphics();
		assert(pGraphics);

		bTextureAdded = pGraphics->AddTextureDataToCubeTexture( pDevice, pTextureSurface, iFace, pTextureData );
	}
	return bTextureAdded;
}





CAnmGraphics::Texture *CAnmGraphics::Texture::LoadCubeTexture(class CAnmTextureData *pTextureData[6],
		class CAnmDevice *pDevice, int stage, int width, int height)
{
	CAnmGraphics::Texture *pTexture = NULL;

	if ((pTexture = FindCubeTexture( pTextureData, pDevice, stage)) != NULL)
		return pTexture;
	else
	{

		assert(pDevice);

		cApplication *pApp = pDevice->GetApp();
		assert(pApp);

		CAnmGraphics *pGraphics = pApp->GetGraphics();
		assert(pGraphics);

		pTexture = pGraphics->CreateCubeTexture( pDevice, pTextureData, stage, width );

		if (pTexture)
		{
			cApplication *pApp = pDevice->GetApp();
			assert(pApp);

			tAnmTextureArray *pTextureArray = pApp->GetCachedCubeTextures();
			assert(pTextureArray);
			pTextureArray->push_back(pTexture);
		}

		return pTexture;
	}
}




// Shader help

BOOL CAnmGraphics::Shader::IsInlineShaderProtocol(const char *url)
{
	char *skurl = AnmSkipLeadingSpace(url);

	if (!strncmp(skurl, ANMHLSLPROTOCOL, strlen(ANMHLSLPROTOCOL)))
		return TRUE;
	else if (!strncmp(skurl, ANMCGSHADERPROTOCOL, strlen(ANMCGSHADERPROTOCOL)))
		return TRUE;
	else if (!strncmp(skurl, ANMGLSLANGPROTOCOL, strlen(ANMGLSLANGPROTOCOL)))
		return TRUE;
	else
		return FALSE;
}

CAnmGraphics::Shader *CAnmGraphics::Shader::CreateFromString(CAnmShaderProgram *pShaderProgram, cApplication *pApp,
																	   const char *url)
{
	assert(pShaderProgram);
	assert(pApp);

	char *skurl = AnmSkipLeadingSpace(url);

	if (!strncmp(skurl, ANMHLSLPROTOCOL, strlen(ANMHLSLPROTOCOL)))
	{
		char *cp = (char *) skurl + strlen(ANMHLSLPROTOCOL);

		CAnmGraphics *pGraphics = pApp->GetGraphics();

		CAnmGraphics::Shader *pShader = pGraphics->CreateShader(pShaderProgram);

		pShader->CompileString(pApp, cp);

		return pShader;
	}
	else
	{
		// N.B.: should warn about unimplemented shader language...
	}

	return NULL;
}

CAnmGraphics::Shader *CAnmGraphics::Shader::CreateFromFile(CAnmShaderProgram *pShaderProgram, cApplication *pApp,
																	 const char *url)
{
	assert(pShaderProgram);
	assert(pApp);

	const char *ext;

	ext = strrchr(url, '.');

	if (!stricmp(ext, ANMHLSLVERTEXSHADEREXT))
	{
		CAnmGraphics *pGraphics = pApp->GetGraphics();

		CAnmGraphics::Shader *pShader = pGraphics->CreateShader(pShaderProgram);

		pShader->CompileFile(pApp, url);

		return pShader;
	}
	if (!stricmp(ext, ANMHLSLPIXELSHADEREXT))
	{
		CAnmGraphics *pGraphics = pApp->GetGraphics();

		CAnmGraphics::Shader *pShader = pGraphics->CreateShader(pShaderProgram);

		pShader->CompileFile(pApp, url);

		return pShader;
	}
	else
	{
		// N.B.: should warn about unimplemented shader language...
	}

	return NULL;
}


