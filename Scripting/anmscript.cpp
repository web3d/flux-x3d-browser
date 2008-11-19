/********************************************************************************
 * Flux
 *
 * File: anmscript.cpp
 * Description: Script node
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
#include "anmscript.h"
#include "anmfields.h"
#include "anmmessage.h"
#include "anmsainode.h"
#include "anmscriptlanguagebinding.h"
#include "anmsymbol.h"
#include "anmworld.h"

CAnmScript::CAnmScript()
		: CAnmNode(),
	CAnmUrlObject(),
	CAnmSAINodeEventSink()
{
	m_languageBinding = NULL;
	m_scriptsymbol = NULL;
	m_members = NULL;
	m_nMembers = 0;
	m_methodCallLevel = 0;
	m_bufferevents = true;		// make sure all event processing is deferred
}


CAnmScript::~CAnmScript()
{
	for (int i = 0; i < m_nMembers; i++)
	{
		// m_members[i].Destroy();
		SafeUnRef(m_members[i]);
	}
	delete [] m_members;

	SafeDelete(m_languageBinding);
}

CAnmObject *CAnmScript::Create()
{
	return new CAnmScript;
}

void CAnmScript::AllocMembers(int nMembers)
{
	// not sure what to do here, yet
	m_members = new CAnmField *[nMembers];
	m_nMembers = nMembers;
	for (int i = 0; i < m_nMembers; i++)
	{
		m_members[i] = NULL;
	}
}


void CAnmScript::Realize()
{
	CAnmNode::Realize();

	if (IsInlineScript())
	{
		CompileInlineScript();
	}
	else
	{
		// get the file loaded
		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL

		cApplication *pApp = pWorld->GetApp();
		assert(pApp);

		Load(pApp);
	}

}


void CAnmScript::Update()
{
	if (StateDirty())
	{
		// here's where we'll reload the script code
		ClearStateDirty();
	}
}

void CAnmScript::SetValue(CLASSMEMBERID mid, void *value)
{
	if (mid == VALUEID(CAnmScript, directOutput) ||
		mid == VALUEID(CAnmScript, mustEvaluate) ||
		mid == VALUEID(CAnmScript, url) ||
		mid == VALUEID(CAnmScript, metadata))
	{
		CAnmNode::SetValue(mid, value);
		return;
	}

	int offset = mid->memberOffset;
	assert (offset >= 0 && offset < m_nMembers);

	if (m_members[offset] == NULL)
	{
		m_members[offset] = CAnmField::CreateField(mid->valueType, value);
	}
	else
	{
		assert(mid->valueType == m_members[offset]->GetType());
		m_members[offset]->SetData(value);
	}

	// N.B.: call set_ handler and _changed callback for value members; ->
	// Watch out for recursion and stack blowout - we need to deal with ->
	// that as a general case anyway

	// Only do this if we're realized
	if (!m_realized)
		return;

	if (mid->memberType == eAnmValueMember)
	{
		char eventbuf[ANMMAXSTRBUF];		// N.B.: length

		sprintf(eventbuf, "set_%s", mid->name);
		CAnmAttributeSymbol *pSetAttrSym = m_scriptsymbol->FindAttribute(eventbuf, eAnmMethodMember);

		if (pSetAttrSym)
			CallMethod(pSetAttrSym->GetClassMember()->name, pSetAttrSym->GetClassMember()->valueType, value,
				m_methodCallLevel == 0);

		sprintf(eventbuf, "%s_changed", mid->name);
		CAnmAttributeSymbol *pChangedAttrSym = m_scriptsymbol->FindAttribute(eventbuf, eAnmCallbackMember);

		if (pChangedAttrSym)
			CallCallbacks(pChangedAttrSym->GetClassMember(), value);	
	}

}

void CAnmScript::GetValue(CLASSMEMBERID mid, void **pValue)
{
	if (mid == VALUEID(CAnmScript, directOutput) ||
		mid == VALUEID(CAnmScript, mustEvaluate) ||
		mid == VALUEID(CAnmScript, url) ||
		mid == VALUEID(CAnmScript, metadata))
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

void CAnmScript::CallMethod(CLASSMEMBERID mid, void *value)
{
	if (mid == METHODID(CAnmScript, set_directOutput) ||
		mid == METHODID(CAnmScript, set_mustEvaluate) ||
		mid == METHODID(CAnmScript, set_url) ||
		mid == METHODID(CAnmScript, set_metadata))
	{
		CAnmNode::CallMethod(mid, value);
		return;
	}

	CAnmWorld *pWorld = GetWorld();
	if (pWorld == NULL)
		return;

	cApplication *pApp = pWorld->GetApp();

	if (pApp == NULL)
		return;

	CAnmTime *pClock = pApp->GetClock();
	Time timeStamp = pClock->GetCurrentTime();

	CallMethod(mid->name, mid->valueType, value, timeStamp, true);
}

void CAnmScript::CallMethod(char *methodName, eValueType valueType, void *pValue, ANMTIME timeStamp,
							bool clearChanges)
{
	if (m_languageBinding != NULL)
	{
		if (clearChanges)
			ClearNodeChangedList();

		if (m_methodCallLevel > ANMSCRIPT_MAXMETHODCALLLEVEL)
			// ANMMSG_SCRIPTCALLOVERFLOW : "Script method call stack overflow"
			throw CAnmScriptMethodError(ANMMSG_SCRIPTCALLOVERFLOW);

		try {
			m_methodCallLevel++;
			m_languageBinding->CallMethod(methodName, valueType, pValue, timeStamp);
			--m_methodCallLevel;
		}
		catch (CAnmScriptMethodError err)
		{
			--m_methodCallLevel;

			if (m_methodCallLevel > 0)
				throw err;
			else
			{
			CAnmWorld *pWorld = GetWorld();
//			assert(pWorld);
			ABORT_IF_WORLD_IS_NULL

			cApplication *pApp = pWorld->GetApp();
			assert(pApp);
			
			pApp->WriteToConsole((char *) err.GetText());
				
			// ANMMSG_SCRIPTERRORSTATUS : "Flux: Script error"
			pApp->SetStatusText(ANMMSG_SCRIPTERRORSTATUS);

			ClearNodeChangedList();
			return;

			}
		}

		if (clearChanges)
			CommitNodeChanges();
	}
}

void CAnmScript::CallMethod(char *methodName, eValueType valueType, void *pValue, 
							bool clearChanges)
{
	CAnmWorld *pWorld = GetWorld();
	if (pWorld == NULL)
		return;

	cApplication *pApp = pWorld->GetApp();

	if (pApp == NULL)
		return;

	CAnmTime *pClock = pApp->GetClock();
	Time timeStamp = pClock->GetCurrentTime();

	CallMethod(methodName, valueType, pValue, timeStamp, clearChanges);
}

void CAnmScript::InitCallbackList(CLASSMEMBERID mid)
{
	sAnmScriptCallbackList cblist;
	cblist.memberOffset = (int) mid->memberOffset;
	m_callbackLists.push_back(cblist);
}

sAnmScriptCallbackList *CAnmScript::LookupCallbackList(CLASSMEMBERID mid)
{
	for (int i = 0; i < m_callbackLists.size(); i++)
	{
		if (m_callbackLists[i].memberOffset == mid->memberOffset)
			return &m_callbackLists[i];
	}

	return NULL;
}

CAnmCallback *CAnmScript::AddCallback(CLASSMEMBERID mid, CAnmCallbackFunction userFunction, void *userData)
{
	sAnmScriptCallbackList *pCBList = LookupCallbackList(mid);
	assert(pCBList != NULL);

	CAnmCallback *pCB = new CAnmCallback(userFunction, this, mid, userData);

	pCBList->callbackList.AddCallback(pCB);

	return pCB;
}

void CAnmScript::RemoveCallback(CLASSMEMBERID mid, CAnmCallback *pCB)
{
	sAnmScriptCallbackList *pCBList = LookupCallbackList(mid);
	assert(pCBList != NULL);

	pCBList->callbackList.RemoveCallback(pCB);

	delete pCB;
}

void CAnmScript::CallCallbacks(CLASSMEMBERID mid, void *cbdata)
{
	sAnmScriptCallbackList *pCBList = LookupCallbackList(mid);
	assert(pCBList != NULL);

	pCBList->callbackList.CallCallbacks(cbdata);
}


CAnmAsset *CAnmScript::handleUrlLoaded(String pRequestUrl, String pCacheFileName)
{
	CompileScript(pCacheFileName);
	return NULL;
}


void CAnmScript::handleUrlLoaded(String pRequestUrl, CAnmAsset *pAsset)
{
}

BOOL CAnmScript::IsInlineScript()
{
	if (m_url->size() <= 0)
		return FALSE;

	const char *str = (*m_url)[0]->GetBuf();

	return CAnmScriptLanguageBinding::IsInlineScriptProtocol(str);
}

void CAnmScript::CompileInlineScript()
{
	assert(IsInlineScript());

	const char *str = (*m_url)[0]->GetBuf();

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld != NULL);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp != NULL);

	m_languageBinding = CAnmScriptLanguageBinding::CreateFromString(this, pApp, str);

	CAnmTime *pClock = pApp->GetClock();
	Time timeStamp = pClock->GetCurrentTime();

	CallMethod("initialize", eValueBadType, NULL, timeStamp, true);
}

void CAnmScript::CompileScript(String filename)
{
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld != NULL);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp != NULL);

	m_languageBinding = CAnmScriptLanguageBinding::CreateFromFile(this, pApp, filename->GetBuf());

	CAnmTime *pClock = pApp->GetClock();
	Time timeStamp = pClock->GetCurrentTime();

	CallMethod("initialize", eValueBadType, NULL, timeStamp, true);
}

// Called from the language binding when it's doing the deleting, so we won't unref it
void CAnmScript::BindingDeleted()
{
	m_languageBinding = NULL;
}

CAnmNode *CAnmScript::GetSinkNode()
{
	return this;
}

// Accessors
void CAnmScript::SetDirectOutput(Boolean directOutput)
{
	m_directOutput = directOutput;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmScript, directOutput_changed), &directOutput);
}

void CAnmScript::SetMustEvaluate(Boolean mustEvaluate)
{
	m_mustEvaluate = mustEvaluate;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmScript, mustEvaluate_changed), &mustEvaluate);
}

void CAnmScript::SetUrl(StringArray *pUrl)
{
	CAnmUrlObject::SetUrl(pUrl);

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmScript, url_changed), &pUrl);
}

// Tables for Callbacks, Methods and Set/GetValues


DEFINE_VALUE(CAnmScript, directOutput, eValueBoolean, Boolean, GetDirectOutput, SetDirectOutput);
DEFINE_VALUE(CAnmScript, mustEvaluate, eValueBoolean, Boolean, GetMustEvaluate, SetMustEvaluate);
DEFINE_VALUE(CAnmScript, url, eValueStringArray, StringArray*, GetUrl, SetUrl);

DEFINE_METHOD(CAnmScript, set_directOutput, eValueBoolean, Boolean, SetDirectOutput);
DEFINE_METHOD(CAnmScript, set_mustEvaluate, eValueBoolean, Boolean, SetMustEvaluate);
DEFINE_METHOD(CAnmScript, set_url, eValueStringArray, StringArray*, SetUrl);

DEFINE_CALLBACK(CAnmScript, directOutput_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmScript, mustEvaluate_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmScript, url_changed, eValueStringArray);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmScript)
VALUEID(CAnmScript, directOutput),
VALUEID(CAnmScript, mustEvaluate),
VALUEID(CAnmScript, url),
METHODID(CAnmScript, set_directOutput),
METHODID(CAnmScript, set_mustEvaluate),
METHODID(CAnmScript, set_url),
CALLBACKID(CAnmScript, directOutput_changed),
CALLBACKID(CAnmScript, mustEvaluate_changed),
CALLBACKID(CAnmScript, url_changed),

END_DEFINE_MEMBERS

// Linkage for callbacks, methods, values. include in all classes derived from this one
// Special version of gunk for Script classes... cf. anmclass.h in core for basis of these
// macros... TP

#define DEFINE_SCRIPT_CLASS(_cls_, _super_) \
static CAnmObject *create##_cls_() \
{ \
	return new _cls_(); \
} \
\
static CAnmClass classdata = \
{ \
	#_cls_, \
	create##_cls_, \
	memberTable##_cls_, \
	sizeof(memberTable##_cls_) / sizeof(CAnmClassMember *), \
	_super_::m_classdata, \
	NULL, \
}; \
CAnmClass *_cls_::m_classdata = &classdata; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; }

DEFINE_SCRIPT_CLASS(CAnmScript, CAnmNode)

