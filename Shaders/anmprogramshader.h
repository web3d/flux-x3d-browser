/********************************************************************************
 * Flux
 *
 * File: anmprogramshader.h
 * Description: Container class for Flux Programmable Shaders
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

#ifndef _anmprogramshader_h
#define _anmprogramshader_h

#include "anmshader.h"

class CAnmProgramShader : public CAnmShader
{

protected:

	NodeArray						*m_programs;

	class CAnmShaderProgram			*m_fragmentShader;
	class CAnmShaderProgram			*m_vertexShader;

	void SetFragmentShader(class CAnmShaderProgram *pFragmentShader);
	void SetVertexShader(class CAnmShaderProgram *pVertexShader);

public:

	// constructor/destructor
	CAnmProgramShader();
	virtual ~CAnmProgramShader();

	// CAnmShader overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual eAnmReturnStatus AddChild(CAnmNode *pChild);
	virtual eAnmReturnStatus RemoveChild(CAnmNode *pChild);

	virtual void SetToDevice(class CAnmDevice *pDevice);

	// Accessors
	void SetPrograms(NodeArray *pPrograms);
	NodeArray *GetPrograms() { return m_programs; }
	void GetPrograms(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_programs;
	}

	class CAnmShaderProgram *GetFragmentShader()
	{
		return m_fragmentShader;
	}

	class CAnmShaderProgram *GetVertexShader()
	{
		return m_vertexShader;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmProgramShader);

	DECLARE_VALUE(programs);

	DECLARE_METHOD(set_programs);

	DECLARE_CALLBACK(programs_changed);
};


DECLARE_GETIMPLEMENTATION(CAnmProgramShader);

#endif // _anmprogramshader_h
