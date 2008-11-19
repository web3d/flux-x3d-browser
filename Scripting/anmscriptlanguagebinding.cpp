/********************************************************************************
 * Flux
 *
 * File: anmscriptlanguagebinding.cpp
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

#include "stdafx.h"
#include "anmscriptlanguagebinding.h"
#include "anmeslanguagebinding.h"
#include "anmbrowser.h"
#include "anmscript.h"
#include "anmstring.h"

CAnmScriptLanguageBinding::CAnmScriptLanguageBinding(class CAnmScript *pScript, class cApplication *pApp)
{
	m_scriptNode = pScript;
	m_scriptNode->Ref();
	m_app = pApp;
	m_browser = new CAnmBrowser(m_app);
	m_browser->SetBaseUrl(m_scriptNode->GetBaseUrl()->GetBuf());
	m_browser->SetContainingNode(pScript);
}

CAnmScriptLanguageBinding::~CAnmScriptLanguageBinding()
{
	// tell the script node we're deleting, so we won't cycle back to here
	m_scriptNode->BindingDeleted();
	SafeUnRef(m_scriptNode);
	SafeUnRef(m_browser);
}

BOOL CAnmScriptLanguageBinding::IsInlineScriptProtocol(const char *url)
{
	char *skurl = AnmSkipLeadingSpace(url);

	if (!strncmp(skurl, ANMECMASCRIPTPROTOCOL, strlen(ANMECMASCRIPTPROTOCOL)))
		return TRUE;
	else if (!strncmp(skurl, ANMJAVASCRIPTPROTOCOL, strlen(ANMJAVASCRIPTPROTOCOL)))
		return TRUE;
	else if (!strncmp(skurl, ANMVRMLSCRIPTPROTOCOL, strlen(ANMJAVASCRIPTPROTOCOL)))
		return TRUE;
	else
		return FALSE;
}

CAnmScriptLanguageBinding *CAnmScriptLanguageBinding::CreateFromString(CAnmScript *pScript, cApplication *pApp,
																	   const char *url)
{
	char *cp = NULL;
	BOOL ecmascript = FALSE;

	char *skurl = AnmSkipLeadingSpace(url);
	if (!strncmp(skurl, ANMECMASCRIPTPROTOCOL, strlen(ANMECMASCRIPTPROTOCOL)))
	{
		cp = (char *) skurl + strlen(ANMECMASCRIPTPROTOCOL);
		ecmascript = TRUE;
	}
	else if (!strncmp(skurl, ANMJAVASCRIPTPROTOCOL, strlen(ANMJAVASCRIPTPROTOCOL)))
	{
		cp = (char *) skurl + strlen(ANMJAVASCRIPTPROTOCOL);
		ecmascript = TRUE;
	}
	else if (!strncmp(skurl, ANMVRMLSCRIPTPROTOCOL, strlen(ANMJAVASCRIPTPROTOCOL)))
	{
		cp = (char *) skurl + strlen(ANMVRMLSCRIPTPROTOCOL);
		ecmascript = TRUE;
	}

	if (ecmascript)
	{
		CAnmECMAScriptLanguageBinding *pESBinding =
			new CAnmECMAScriptLanguageBinding(pScript, pApp);

		// make sure this gets garbage collected
		pApp->AddGlobalObject(pESBinding);

		pESBinding->CompileString(cp);

		return pESBinding;
	}

	return NULL;
}

CAnmScriptLanguageBinding *CAnmScriptLanguageBinding::CreateFromFile(CAnmScript *pScript, cApplication *pApp,
																	 const char *url)
{
	// N.B.: use file extension to determine type; need MIME type too at some point
	char *ext = strrchr(url, '.');
	if (ext && (!stricmp(ext, ".es") || !stricmp(ext, ".js")))
	{
		CAnmECMAScriptLanguageBinding *pESBinding =
			new CAnmECMAScriptLanguageBinding(pScript, pApp);

		// make sure this gets garbage collected
		pApp->AddGlobalObject(pESBinding);

		pESBinding->CompileFile(url);

		return pESBinding;
	}

	return NULL;
}
