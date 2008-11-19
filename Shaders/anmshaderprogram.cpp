/********************************************************************************
 * Flux
 *
 * File: anmshaderprogram.cpp
 * Description: Programmable Shaders single program (Vertex/Pixel shader)
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
#include "anmshaderprogram.h"
#include "anmfields.h"
#include "anmmessage.h"
#include "anmsymbol.h"
#include "anmworld.h"

CAnmShaderProgram::CAnmShaderProgram()
		: CAnmNode(),
	CAnmUrlObject()
{
	m_type = new CAnmString(ANMSHADERPROGRAM_DEFAULT_TYPE);
	m_shaderType = eAnmShaderTypeBadType;

	m_shader = NULL;
	m_shadersymbol = NULL;
	m_members = NULL;
	m_nMembers = 0;
	m_memberIDs = NULL;

}


CAnmShaderProgram::~CAnmShaderProgram()
{
	for (int i = 0; i < m_nMembers; i++)
	{
		// m_members[i].Destroy();
		SafeUnRef(m_members[i]);
	}
	delete [] m_members;

	if (m_memberIDs)
		delete [] m_memberIDs;

	SafeDelete(m_shader);
}

CAnmObject *CAnmShaderProgram::Create()
{
	return new CAnmShaderProgram;
}


void CAnmShaderProgram::AllocMembers(int nMembers)
{
	// not sure what to do here, yet
	m_members = new CAnmField *[nMembers];
	m_memberIDs = new CLASSMEMBERID[nMembers];
	m_nMembers = nMembers;
	for (int i = 0; i < m_nMembers; i++)
	{
		m_members[i] = NULL;
		m_memberIDs[i] = NULL;
	}
}

eAnmShaderType CAnmShaderProgram::CalcShaderType()
{
	if (!strcmp(m_type->GetBuf(), "VERTEX"))
		return eAnmVertexShader;
	else if (!strcmp(m_type->GetBuf(), "FRAGMENT"))
		return eAnmFragmentShader;
	else
		return eAnmShaderTypeBadType;
}

void CAnmShaderProgram::Realize()
{
	// get the file loaded
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	if (m_shaderType == eAnmShaderTypeBadType)
	{
		// ANMMSG_UNKNOWNSHADERTYPE : "Unknown shader type: '%s'"
		char *msg = (char *) malloc(strlen(ANMMSG_UNKNOWNSHADERTYPE) + 
			strlen(m_type->GetBuf()) +
			1);

		sprintf(msg, ANMMSG_UNKNOWNSHADERTYPE, m_type->GetBuf());
		
		pApp->Warn(msg);

		delete msg;
		return;
	}

	if (IsInlineShader())
	{
		CompileInlineShader();
	}
	else
	{

		Load(pApp);
	}

	CAnmNode::Realize();
}


void CAnmShaderProgram::Update()
{
	if (StateDirty())
	{
		// here's where we'll reload the script code
		ClearStateDirty();
	}
}

void CAnmShaderProgram::SetValue(CLASSMEMBERID mid, void *value)
{
	if (mid == VALUEID(CAnmShaderProgram, url)  ||
		mid == VALUEID(CAnmShaderProgram, type))
	{
		CAnmNode::SetValue(mid, value);
		return;
	}

	// Save the value now; these will get set to the shader in batch
	// prior to rendering
	int offset = mid->memberOffset;
	assert (offset >= 0 && offset < m_nMembers);

	if (m_members[offset] == NULL)
	{
		m_members[offset] = CAnmField::CreateField(mid->valueType, value);
		m_memberIDs[offset] = mid;
	}
	else
	{
		assert(mid->valueType == m_members[offset]->GetType());
		m_members[offset]->SetData(value);
	}
}

void CAnmShaderProgram::GetValue(CLASSMEMBERID mid, void **pValue)
{
	if (mid == VALUEID(CAnmShaderProgram, url))
	{
		CAnmNode::GetValue(mid, pValue);
		return;
	}

	int offset = mid->memberOffset;
	assert (offset >= 0 && offset < m_nMembers);
	
	if (m_members[offset] == NULL)
	{
		m_members[offset] = CAnmField::CreateField(mid->valueType, NULL);
	}

	m_members[offset]->Copy(pValue);
}

void CAnmShaderProgram::CallMethod(CLASSMEMBERID mid, void *value)
{
	if (mid == METHODID(CAnmShaderProgram, set_url))
	{
		CAnmNode::CallMethod(mid, value);
		return;
	}

	// A method call is just the same as set_value from the POV of the shader.
	// Just save the value; it will get set into the shader in batch at
	// render time
	int offset = mid->memberOffset;
	assert (offset >= 0 && offset < m_nMembers);

	if (m_members[offset] == NULL)
	{
		m_members[offset] = CAnmField::CreateField(mid->valueType, value);
		m_memberIDs[offset] = mid;
	}
	else
	{
		assert(mid->valueType == m_members[offset]->GetType());
		m_members[offset]->SetData(value);
	}
}

void CAnmShaderProgram::InitCallbackList(CLASSMEMBERID mid)
{
	sAnmShaderCallbackList cblist;
	cblist.memberOffset = (int) mid->memberOffset;
	m_callbackLists.push_back(cblist);
}

sAnmShaderCallbackList *CAnmShaderProgram::LookupCallbackList(CLASSMEMBERID mid)
{
	for (int i = 0; i < m_callbackLists.size(); i++)
	{
		if (m_callbackLists[i].memberOffset == mid->memberOffset)
			return &m_callbackLists[i];
	}

	return NULL;
}

CAnmCallback *CAnmShaderProgram::AddCallback(CLASSMEMBERID mid, CAnmCallbackFunction userFunction, void *userData)
{
	sAnmShaderCallbackList *pCBList = LookupCallbackList(mid);
	assert(pCBList != NULL);

	CAnmCallback *pCB = new CAnmCallback(userFunction, this, mid, userData);

	pCBList->callbackList.AddCallback(pCB);

	return pCB;
}

void CAnmShaderProgram::RemoveCallback(CLASSMEMBERID mid, CAnmCallback *pCB)
{
	sAnmShaderCallbackList *pCBList = LookupCallbackList(mid);
	assert(pCBList != NULL);

	pCBList->callbackList.RemoveCallback(pCB);

	delete pCB;
}

void CAnmShaderProgram::CallCallbacks(CLASSMEMBERID mid, void *cbdata)
{
	sAnmShaderCallbackList *pCBList = LookupCallbackList(mid);
	assert(pCBList != NULL);

	pCBList->callbackList.CallCallbacks(cbdata);
}


class CAnmAsset *CAnmShaderProgram::handleUrlLoaded(String pRequestUrl, String pCacheFileName)
{
	CompileShader(pCacheFileName);

	return NULL;
}

void CAnmShaderProgram::handleUrlLoaded(String pRequestUrl, CAnmAsset *pAsset)
{
}

BOOL CAnmShaderProgram::IsInlineShader()
{
	if (m_url->size() <= 0)
		return FALSE;

	const char *str = (*m_url)[0]->GetBuf();

	return CAnmGraphics::Shader::IsInlineShaderProtocol(str);
}

void CAnmShaderProgram::CompileInlineShader()
{
	assert(IsInlineShader());

	const char *str = (*m_url)[0]->GetBuf();

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld != NULL);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp != NULL);

	m_shader = CAnmGraphics::Shader::CreateFromString(this, pApp, str);
}

void CAnmShaderProgram::CompileShader(String filename)
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld != NULL);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp != NULL);

	m_shader = CAnmGraphics::Shader::CreateFromFile(this, pApp, filename->GetBuf());
}

void CAnmShaderProgram::CopyValuesToShader()
{
	for (int i = 0; i < m_nMembers; i++)
	{
		CLASSMEMBERID mid = m_memberIDs[i];

		if (mid && (mid->memberType == eAnmValueMember || mid->memberType == eAnmMethodMember))
			m_shader->SetValue(mid->name, mid->valueType, m_members[i]->GetData());
	}
}

CAnmGraphics::Shader *CAnmShaderProgram::GetRawShader()
{
	return m_shader;
}

// Accessors
void CAnmShaderProgram::SetUrl(StringArray *pUrl)
{
	CAnmUrlObject::SetUrl(pUrl);

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmShaderProgram, url_changed), &pUrl);
}

void CAnmShaderProgram::SetType(String type)
{
	SafeUnRef(m_type);
	m_type = type;
	m_type->Ref();

	m_shaderType = CalcShaderType();

}


// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmShaderProgram, url, eValueStringArray, StringArray*, GetUrl, SetUrl);
DEFINE_INITVALUE(CAnmShaderProgram, type, eValueString, String, , SetType);

DEFINE_METHOD(CAnmShaderProgram, set_url, eValueStringArray, StringArray*, SetUrl);

DEFINE_CALLBACK(CAnmShaderProgram, url_changed, eValueStringArray);

// member table
BEGIN_DEFINE_MEMBERS(CAnmShaderProgram)
VALUEID(CAnmShaderProgram, url),
VALUEID(CAnmShaderProgram, type),
METHODID(CAnmShaderProgram, set_url),
CALLBACKID(CAnmShaderProgram, url_changed),
END_DEFINE_MEMBERS


// Linkage for callbacks, methods, values. include in all classes derived from this one
// Special version of gunk for Shader classes... cf. anmclass.h in core for basis of these
// macros. Note that this is an abstract class. See VertexShader and FragmentShader concrete
// classes. TP

#define DEFINE_SHADER_CLASS(_cls_, _super_) \
static CAnmClass classdata = \
{ \
	#_cls_, \
	NULL, \
	memberTable##_cls_, \
	sizeof(memberTable##_cls_) / sizeof(CAnmClassMember *), \
	_super_::m_classdata, \
	NULL, \
}; \
CAnmClass *_cls_::m_classdata = &classdata; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; }

DEFINE_SHADER_CLASS(CAnmShaderProgram, CAnmNode)

