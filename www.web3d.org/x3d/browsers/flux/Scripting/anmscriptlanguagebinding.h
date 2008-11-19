/********************************************************************************
 * Flux
 *
 * File: anmscriptlanguagebinding.h
 * Description: Scripting Language Binding abstract
 *							base class
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

#ifndef _anmscriptlanguagebinding_h
#define _anmscriptlanguagebinding_h

#define ANMECMASCRIPTPROTOCOL "ecmascript:"
#define ANMJAVASCRIPTPROTOCOL "javascript:"
#define ANMVRMLSCRIPTPROTOCOL "vrmlscript:"

typedef void *ANMSCRIPTBINDING_METHODID;
#define ANMSCRIPTBINDING_METHODID_BADID ((ANMSCRIPTBINDING_METHODID) -1)

#include "anmobject.h"

class CAnmScriptLanguageBinding : public CAnmObject
{
protected:

	class CAnmScript			*m_scriptNode;
	class cApplication			*m_app;
	class CAnmBrowser			*m_browser;

public:

	// constructor/destructor
	CAnmScriptLanguageBinding(class CAnmScript *pScript, class cApplication *pApp);
	virtual ~CAnmScriptLanguageBinding();

	static BOOL IsInlineScriptProtocol(const char *url);
	static CAnmScriptLanguageBinding *CreateFromString(class CAnmScript *pScript, class cApplication *pApp,
		const char *url);
	static CAnmScriptLanguageBinding *CreateFromFile(class CAnmScript *pScript, class cApplication *pApp,
		const char *url);

	// these methods must be overridden
	virtual void CompileString(const char *source) PURE;
	virtual void CompileFile(const char *filename) PURE;

	virtual eAnmReturnStatus CallMethod(CLASSMEMBERID mid, void *pValue, ANMTIME timeStamp) PURE;
	virtual eAnmReturnStatus CallMethod(char *methodName, eValueType valueType, void *pValue, ANMTIME timeStamp) PURE;

	// Accessors
	class cApplication *GetApplication()
	{
		return m_app;
	}

	class CAnmScript *GetScriptNode()
	{
		return m_scriptNode;
	}

	class CAnmBrowser *GetBrowser()
	{
		return m_browser;
	}
};

#endif // _anmscriptlanguagebinding_h
