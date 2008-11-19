/********************************************************************************
 * Flux
 *
 * File: anmeslanguagebinding.h
 * Description: ECMAScript Language Binding class
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

#ifndef _anmeslanguagebinding_h
#define _anmeslanguagebinding_h

#include "anmscriptlanguagebinding.h"
#include "anmecmascript.h"

class CAnmECMAScriptLanguageBinding : public CAnmScriptLanguageBinding
{
protected :

	class CAnmESScript							*m_scriptImpl;
	class CAnmESNodeObject						*m_esScriptObject;
	AnmESVar									 m_esScriptVar;
	std::vector<AnmESVar *>						 m_globals;
	std::vector<class CAnmESObject *>			 m_objects;
	std::vector<class CAnmSAINode *>			 m_nodes;

	virtual void AddPredefinedMembers(class CAnmClass *pClass);

	void GarbageCollect();
	void Mark();
	void Unmark();
	void Sweep();

	virtual void Compile(const char *source, bool fromFile);
	virtual void ClearNodeList();

public :

	// Constructor/destructor
	CAnmECMAScriptLanguageBinding(class CAnmScript *pScript, class cApplication *pApp);
	virtual ~CAnmECMAScriptLanguageBinding();

	// CAnmScriptLanguageBinding overrides
	virtual void CompileString(const char *source);
	virtual void CompileFile(const char *filename);

	virtual eAnmReturnStatus CallMethod(CLASSMEMBERID mid, void *pValue, ANMTIME timeStamp);
	virtual eAnmReturnStatus CallMethod(char *methodName, eValueType valueType, void *pValue, ANMTIME timeStamp);

	// New methods
	virtual void AddGlobal(AnmESVar *globalVar);
	virtual void AddObject(class CAnmESObject *pObj);

	virtual CLASSMEMBERID LookupMember(char *memberName);

	virtual class CAnmESScript *GetScriptImplementation()
	{
		return m_scriptImpl;
	}

	virtual class CAnmSAINode *LookupNode(CAnmNode *pNode);

};

#endif // _anmeslanguagebinding_h
