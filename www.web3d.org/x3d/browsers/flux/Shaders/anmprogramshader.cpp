/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			anmprogramshader.cpp
 *		Description:	Container class for Flux Programmable Shaders
 *
 * (C) 2001-2004 by Media Machines
 *
 *******************************************************************/

#include "stdafx.h"
#include "anmprogramshader.h"
#include "anmdevice.h"
#include "anmshaderprogram.h"

CAnmProgramShader::CAnmProgramShader() : CAnmShader()
{
	m_programs = new NodeArray;
	m_fragmentShader = NULL;
	m_vertexShader = NULL;
}

CAnmProgramShader::~CAnmProgramShader()
{
	SafeUnRef(m_programs);
}

void CAnmProgramShader::Realize()
{
	CAnmGroup::Realize();	// realize the kids
}

void CAnmProgramShader::SetToDevice(class CAnmDevice *pDevice)
{
	assert(pDevice);

	if (m_vertexShader)
		pDevice->SetVertexShader(m_vertexShader->GetRawShader());
	if (m_fragmentShader)
		pDevice->SetFragmentShader(m_fragmentShader->GetRawShader());
}

void CAnmProgramShader::Update()
{
	if (m_fragmentShader && m_fragmentShader->StateDirty())
		GETIMPLEMENTATION(CAnmShaderProgram, m_fragmentShader)->Update();

	if (m_vertexShader && m_vertexShader->StateDirty())
		GETIMPLEMENTATION(CAnmShaderProgram, m_vertexShader)->Update();
}

eAnmReturnStatus CAnmProgramShader::AddChild(CAnmNode *pChild)
{
	CAnmGroup::AddChild(pChild);

	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmShaderProgram)))
		
	{
		CAnmShaderProgram *pShaderProgram = (CAnmShaderProgram *) pImp;

		if (pShaderProgram->GetShaderType() == eAnmFragmentShader)
			m_fragmentShader = pShaderProgram;
		else if (pShaderProgram->GetShaderType() == eAnmVertexShader)
			m_vertexShader = pShaderProgram;
	}

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmProgramShader::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmShaderProgram)))
		
	{
		CAnmShaderProgram *pShaderProgram = (CAnmShaderProgram *) pImp;

		if (pShaderProgram->GetShaderType() == eAnmFragmentShader)
			m_fragmentShader = NULL;
		else if (pShaderProgram->GetShaderType() == eAnmVertexShader)
			m_vertexShader = NULL;
	}

	CAnmGroup::RemoveChild(pChild);

	return eAnmReturnAllGood;
}

// Accessors
void CAnmProgramShader::SetFragmentShader(CAnmShaderProgram *pFragmentShader)
{
	if (m_fragmentShader == pFragmentShader)
		return;

	if (m_fragmentShader)
		RemoveChild(m_fragmentShader);

	if (pFragmentShader == NULL)
		return;

	AddChild(pFragmentShader);
	m_fragmentShader = pFragmentShader;
}

void CAnmProgramShader::SetVertexShader(CAnmShaderProgram *pVertexShader)
{
	if (m_vertexShader == pVertexShader)
		return;

	if (m_vertexShader)
		RemoveChild(pVertexShader);

	if (pVertexShader == NULL)
		return;

	AddChild(pVertexShader);
	m_vertexShader = pVertexShader;
}

void CAnmProgramShader::SetPrograms(NodeArray *pPrograms)
{
	assert(pPrograms != NULL);

	SafeUnRef(m_programs);
	m_programs = pPrograms;
	m_programs->Ref();

	for (int i = 0; i < m_programs->size(); i++)
	{
		CAnmNode *pChild = (*m_programs)[i];
		if (pChild)
		{
			CAnmNode *pImp = pChild->GetImplementationNode();
			if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmShaderProgram)))
				
			{
				CAnmShaderProgram *pShaderProgram = (CAnmShaderProgram *) pImp;

				if (pShaderProgram->GetShaderType() == eAnmFragmentShader)
					SetFragmentShader(pShaderProgram);
				else if (pShaderProgram->GetShaderType() == eAnmVertexShader)
					SetVertexShader(pShaderProgram);
			}
		}
	}

	// Make sure updates happen next tick
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmProgramShader, programs_changed), &pPrograms);
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmProgramShader, programs, eValueNodeArray, NodeArray *, GetPrograms, SetPrograms);

DEFINE_METHOD(CAnmProgramShader, set_programs, eValueNodeArray, NodeArray *, SetPrograms);

DEFINE_CALLBACK(CAnmProgramShader, programs_changed, eValueNodeArray);

// member table
BEGIN_DEFINE_MEMBERS(CAnmProgramShader)
VALUEID(CAnmProgramShader, programs),
METHODID(CAnmProgramShader, set_programs),
CALLBACKID(CAnmProgramShader, programs_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmProgramShader, CAnmGroup);