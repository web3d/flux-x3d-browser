/********************************************************************************
 * Flux
 *
 * File: anmtexturecoordinategenerator.h
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

#ifndef _anmtexturecoordinategenerator_h
#define _anmtexturecoordinategenerator_h

#include "anmtexturecoordinate.h"

#define ANMTEXCOORDGEN_DEFAULT_MODE			"SPHERE"

class CAnmTextureCoordinateGenerator : public CAnmTextureCoordinate
{
protected :

	String							 m_mode;
	FloatArray						*m_parameter;
	
	eAnmTextureTexCoordIndexMode	 m_texcoordmode;

public:

	// constructor/destructor
	CAnmTextureCoordinateGenerator();
	virtual ~CAnmTextureCoordinateGenerator();

	// CAnmNode overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();								// re-render/reset rendering structures

	// New methods
	virtual bool SetToDevice(class CAnmDevice *pDevice, int stage);

	// Accessors
	void SetMode(String mode);
	String GetMode()
	{
		return m_mode;
	}
	void GetMode(String *pStr)
	{
		assert(pStr);
		*pStr = m_mode;
	}

	void SetParameter(FloatArray *pParameter);
	FloatArray *GetParameter() { return m_parameter; }
	void GetParameter(FloatArray **pVal)
	{
		assert(pVal);
		*pVal = m_parameter;
	}

	
	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmTextureCoordinateGenerator);

	DECLARE_VALUE(mode);
	DECLARE_VALUE(parameter);

	DECLARE_CALLBACK(mode_changed);
	DECLARE_CALLBACK(parameter_changed);


	DECLARE_METHOD(set_mode);
	DECLARE_METHOD(set_parameter);
};

DECLARE_GETIMPLEMENTATION(CAnmTextureCoordinateGenerator);

#endif _anmtexturecoordinategenerator_h
