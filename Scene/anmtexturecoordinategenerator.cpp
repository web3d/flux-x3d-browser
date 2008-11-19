/********************************************************************************
 * Flux
 *
 * File: anmtexturecoordinategenerator.cpp
 * Description: TextureCoordinateGenerator node
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
#include "anmtexturecoordinategenerator.h"
#include "anmdevice.h"

CAnmTextureCoordinateGenerator::CAnmTextureCoordinateGenerator() : CAnmTextureCoordinate()
{
	m_mode = new CAnmString(ANMTEXCOORDGEN_DEFAULT_MODE);
	m_parameter = new FloatArray;

	m_texcoordmode = eAnmTextureTexCoordPassThru;
}

CAnmTextureCoordinateGenerator::~CAnmTextureCoordinateGenerator()
{
	SafeUnRef(m_mode);
	SafeUnRef(m_parameter);
}

void CAnmTextureCoordinateGenerator::Realize()
{
	CAnmTextureCoordinate::Realize();
}

void CAnmTextureCoordinateGenerator::Update()
{
	assert(m_mode);

	char *cp = m_mode->GetBuf();

	if (!strcmp(cp, "SPHERE"))
		m_texcoordmode = eAnmTextureTexCoordSphere;
	else if (!strcmp(cp, "CAMERASPACENORMAL"))
		m_texcoordmode = eAnmTextureCameraSpaceNormal;
	else if (!strcmp(cp, "CAMERASPACEPOSITION"))
		m_texcoordmode = eAnmTextureCameraSpacePosition;
	else if (!strcmp(cp, "CAMERASPACEREFLECTIONVECTOR"))
		m_texcoordmode = eAnmTextureCameraSpaceReflectionVector;
	else
		m_texcoordmode = eAnmTextureTexCoordPassThru;

	ClearStateDirty();
}

bool CAnmTextureCoordinateGenerator::SetToDevice(class CAnmDevice *pDevice, int stage)
{
	assert(pDevice);

	pDevice->SetTextureOption(stage, eAnmTextureTexCoordIndex, m_texcoordmode);

	// Camera space modes mess with the texture transform
	return (m_texcoordmode == eAnmTextureTexCoordSphere ||
			m_texcoordmode == eAnmTextureCameraSpaceReflectionVector );
}

// Accessors
void CAnmTextureCoordinateGenerator::SetMode(String mode)
{
	SafeUnRef(m_mode);
	m_mode = mode;
	m_mode->Ref();

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmTextureCoordinateGenerator, mode_changed), &mode);
}


void CAnmTextureCoordinateGenerator::SetParameter(FloatArray *pParameter)
{
	assert(pParameter != NULL);

	SafeUnRef(m_parameter);
	m_parameter = pParameter;
	m_parameter->Ref();

	CallCallbacks(CALLBACKID(CAnmTextureCoordinateGenerator, parameter_changed), &pParameter);

	SetStateDirty();
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmTextureCoordinateGenerator, mode, eValueString, String, GetMode, SetMode);
DEFINE_VALUE(CAnmTextureCoordinateGenerator, parameter, eValueFloatArray, FloatArray*, GetParameter, SetParameter);

DEFINE_METHOD(CAnmTextureCoordinateGenerator, set_mode, eValueString, String, SetMode);
DEFINE_METHOD(CAnmTextureCoordinateGenerator, set_parameter, eValueFloatArray, FloatArray*, SetParameter);

DEFINE_CALLBACK(CAnmTextureCoordinateGenerator, mode_changed, eValueString);
DEFINE_CALLBACK(CAnmTextureCoordinateGenerator, parameter_changed, eValueFloatArray);

BEGIN_DEFINE_NODE_MEMBERS(CAnmTextureCoordinateGenerator)
VALUEID(CAnmTextureCoordinateGenerator, mode), 
VALUEID(CAnmTextureCoordinateGenerator, parameter), 
METHODID(CAnmTextureCoordinateGenerator, set_mode), 
METHODID(CAnmTextureCoordinateGenerator, set_parameter), 
CALLBACKID(CAnmTextureCoordinateGenerator, mode_changed),
CALLBACKID(CAnmTextureCoordinateGenerator, parameter_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmTextureCoordinateGenerator, CAnmNode);
