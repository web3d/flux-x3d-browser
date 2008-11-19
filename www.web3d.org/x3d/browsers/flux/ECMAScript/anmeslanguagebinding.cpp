/********************************************************************************
 * Flux
 *
 * File: ECMAScripting Language Binding class
 * Description: 
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
#include "anmeslanguagebinding.h"
#include "anmbrowser.h"
#include "anmesmessage.h"
#include "anmescompiler.h"
#include "anmessymbol.h"
#include "anmesnode.h"
#include "anmfields.h"
#include "anmsainode.h"
#include "anmscript.h"

CAnmECMAScriptLanguageBinding::CAnmECMAScriptLanguageBinding(CAnmScript *pScript, cApplication *pApp) :
CAnmScriptLanguageBinding(pScript, pApp)
{
	m_esScriptObject = NULL;
	m_scriptImpl = NULL;
}

CAnmECMAScriptLanguageBinding::~CAnmECMAScriptLanguageBinding()
{
	for (int i = 0; i < m_globals.size(); i++)
	{
		CAnmESObject *pObj = (CAnmESObject *) *(m_globals[i]);
		SafeDelete(pObj);
	}

	SafeDelete(m_scriptImpl);

}

void CAnmECMAScriptLanguageBinding::CompileString(const char *source)
{
	Compile(source, false);
}

void CAnmECMAScriptLanguageBinding::CompileFile(const char *filename)
{
	Compile(filename, true);
}

void CAnmECMAScriptLanguageBinding::Compile(const char *source, bool fromFile)
{
	m_scriptImpl = new CAnmESScript;
	CAnmECMAScriptCompiler *pCompiler = new CAnmECMAScriptCompiler(m_app, this);
	AddPredefinedMembers(m_scriptNode->GetScriptSymbol()->GetClass());

	eAnmReturnStatus retstat = fromFile ? 
		pCompiler->CompileFile(source) :
		pCompiler->CompileString(source);

	if (ANMSUCCEEDED(retstat))
	{
		CAnmSAINode *pSAINode = new CAnmSAINode(m_scriptNode);
		pSAINode->SetSink(m_scriptNode);
		m_esScriptObject = new CAnmESNodeObject(pSAINode, this);
		m_esScriptVar = m_esScriptObject;
		AddGlobal(&m_esScriptVar);

		// the esscript object will hold the ref for me, so decrement the counter
		pSAINode->UnRef();
	}
	else
		SafeDelete(m_scriptImpl);

	delete pCompiler;
}

eAnmReturnStatus CAnmECMAScriptLanguageBinding::CallMethod(CLASSMEMBERID mid, void *pValue,
														   ANMTIME timeStamp)
{
	return CallMethod(mid->name, mid->valueType, pValue, timeStamp);
}

eAnmReturnStatus CAnmECMAScriptLanguageBinding::CallMethod(char *methodName, eValueType valueType, void *pValue,
														   ANMTIME timeStamp)
{
	AnmESVar retval;
		
	if (m_scriptImpl == NULL)
		return eAnmReturnAllBad;

	CAnmESSymbol *pSymbol = m_scriptImpl->FindSymbol(methodName);
	if (pSymbol != NULL && pSymbol->GetSymType() == eFunctionSymbol)
	{
		CAnmESFunctionSymbol *pFuncSymbol = (CAnmESFunctionSymbol *) pSymbol;

		ESLOCALOFFSET procaddr = pFuncSymbol->GetProcAddress();
		if (procaddr != ESBADLOCALADDR)
		{

			AnmESVar v;

			if (valueType != eValueBadType)
			{
				CAnmField *pF = CAnmField::CreateField(valueType, pValue);
				v = AnmESVarFromField(pF, this);
				pF->UnRef();
			}
			else
				v = 0L;

			AnmESVar args[3];
			args[0] = m_esScriptObject;
			args[1] = v;
			args[2] = timeStamp;

			// Make sure our 'this' has all the newest stuff
			// m_esScriptObject->GetNode()->UpdateValues();
			try {
				retval = m_scriptImpl->GetCode()->CallFunction(this, procaddr, 3, args);
			}
			catch (CAnmScriptMethodError err)
			{
				throw err;
			}
			catch (CAnmError err)
			{
				m_app->WriteToConsole((char *) err.GetText());
				// ANMMSG_ESERRORSTATUS : "Flux: ECMAScript error"
				m_app->SetStatusText(ANMMSG_ESERRORSTATUS);
			}

			GarbageCollect();
			ClearNodeList();
		}
	}

	return eAnmReturnAllGood;
}


void CAnmECMAScriptLanguageBinding::AddPredefinedMembers(CAnmClass *pClass)
{
	assert (pClass != NULL);

	assert (pClass->IsA(GETCLASS(CAnmScript)));

	CAnmClassMember *pMember = NULL;
	CAnmESSymbol *pSymbol = NULL;

	for (int i = 0; i < pClass->m_numMembers; i++)
	{
		pMember = pClass->m_memberTable[i];

		if (pMember->memberType == eAnmMethodMember)
		{
			pSymbol = new CAnmESFunctionSymbol(new CAnmString(pMember->name));
		}
		else
		{
			pSymbol = new CAnmESVarSymbol(new CAnmString(pMember->name), eMemberVariable);
		}

		m_scriptImpl->AddSymbol(pSymbol);
	}
}

CLASSMEMBERID CAnmECMAScriptLanguageBinding::LookupMember(char *memberName)
{
	CAnmClass *pClass = m_scriptNode->GetScriptSymbol()->GetClass();
	assert(pClass);

	return pClass->FindClassMember(memberName);
}

void CAnmECMAScriptLanguageBinding::AddGlobal(AnmESVar *globalVar)
{
	m_globals.push_back(globalVar);
}

void CAnmECMAScriptLanguageBinding::AddObject(CAnmESObject *pObj)
{
	m_objects.push_back(pObj);
}

// Garbage collection
void CAnmECMAScriptLanguageBinding::GarbageCollect()
{
	Mark();
	Sweep();
	Unmark();
}

void CAnmECMAScriptLanguageBinding::Mark()
{
	for (int i = 0; i < m_globals.size(); i++)
	{
		CAnmESObject *pObj = (CAnmESObject *) *(m_globals[i]);
		if (pObj)
			pObj->Mark();
	}
}

void CAnmECMAScriptLanguageBinding::Unmark()
{
	for (int i = 0; i < m_globals.size(); i++)
	{
		CAnmESObject *pObj = (CAnmESObject *) *(m_globals[i]);
		if (pObj)
			pObj->Unmark();
	}
}

void CAnmECMAScriptLanguageBinding::Sweep()
{
	for (int i = 0; i < m_objects.size(); i++)
	{
		CAnmESObject *pObj = m_objects[i];
		if (!pObj->IsMarked())
		{
			delete pObj;
			m_objects[i] = NULL;
		}
	}

	m_objects.clear();
}

CAnmSAINode *CAnmECMAScriptLanguageBinding::LookupNode(CAnmNode *pNode)
{
	CAnmSAINode *pSAINode = NULL;

	int sz = m_nodes.size();

	// N.B.: linear search; should we hash?
	for (int i = 0; i < sz; i++)
	{
		if (m_nodes[i]->GetNode() == pNode)
		{
			pSAINode = m_nodes[i];
//			pSAINode->Ref();
			break;
		}
	}

	if (!pSAINode)
	{
		pSAINode = new CAnmSAINode(pNode);
		m_nodes.push_back(pSAINode);
	}

	return pSAINode;
}

void CAnmECMAScriptLanguageBinding::ClearNodeList()
{
	int sz = m_nodes.size();
	for (int i = 0; i < sz; i++)
	{
		SafeUnRef(m_nodes[i]);
	}

	m_nodes.clear();
}