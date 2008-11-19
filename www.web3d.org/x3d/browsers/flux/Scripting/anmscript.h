/********************************************************************************
 * Flux
 *
 * File: anmscript.h
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

#ifndef _anmscript_h
#define _anmscript_h

#include "anmnode.h"
#include "anmurlobject.h"
#include "anmsainodeventsink.h"
#include "anmsymbol.h"

// callback handling-- have to do it differently from core objects
typedef struct sAnmScriptCallbackList {
	int memberOffset;
	CAnmCallbackList callbackList;
} sAnmScriptCallbackList ;

typedef std::vector<sAnmScriptCallbackList> tAnmScriptCallbackLists;

class CAnmScriptMethodError : public CAnmError
{
public :
	
	CAnmScriptMethodError(const char *errText) : CAnmError(errText)
	{
	}
};

#define ANMSCRIPT_MAXMETHODCALLLEVEL 32

class CAnmScript : public CAnmNode, public CAnmUrlObject, public CAnmSAINodeEventSink
{

protected:

	Boolean									 m_directOutput;
	Boolean									 m_mustEvaluate;

	class CAnmScriptLanguageBinding			*m_languageBinding;	// binding to specific language
																// implementation

	class CAnmScriptSymbol					*m_scriptsymbol;	// type info for this script

	class CAnmField							**m_members;
	int										 m_nMembers;

	tAnmScriptCallbackLists					 m_callbackLists;

	std::vector<class CAnmSAINode	*>			 m_changedNodes;

	int										 m_methodCallLevel;

	// helpers for async script file loading
	virtual class CAnmAsset *handleUrlLoaded(String pRequestUrl, String pCacheFileName);
	virtual void handleUrlLoaded(String pRequestUrl, class CAnmAsset *pAsset);

	virtual BOOL IsInlineScript();
	virtual void CompileScript(String filename);
	virtual void CompileInlineScript();

	virtual void AllocMembers(int nMembers);
	virtual void InitCallbackList(CLASSMEMBERID mid);
	virtual sAnmScriptCallbackList *LookupCallbackList(CLASSMEMBERID mid);

	virtual void CallMethod(char *methodName, eValueType valueType, void *pValue, 
		bool clearChanges);

	virtual void CallMethod(char *methodName, eValueType valueType, void *pValue, ANMTIME timeStamp,
		bool clearChanges);

	virtual void BindingDeleted();

	// CAnmSAINodeEventSink overrides
	virtual class CAnmNode *GetSinkNode();

public:

	// constructor/destructor
	CAnmScript();
	virtual ~CAnmScript();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual void SetValue(CLASSMEMBERID mid, void *value);
	virtual void GetValue(CLASSMEMBERID mid, void **pValue);
	virtual void CallMethod(CLASSMEMBERID mid, void *value);

	virtual CAnmCallback *AddCallback(CLASSMEMBERID mid, CAnmCallbackFunction userFunction, void *userData);
	virtual void RemoveCallback(CLASSMEMBERID mid, CAnmCallback *pCB);
	virtual void CallCallbacks(CLASSMEMBERID mid, void *cbdata);

	virtual void SetBaseUrl(String baseUrl)
	{
		CAnmUrlObject::SetBaseUrl(baseUrl);
	}

	virtual String GetBaseUrl()
	{
		return CAnmUrlObject::GetBaseUrl();
	}

	// basic QueryInterface-like capability
	virtual bool GetInterface(ANMINTERFACE ival, void *pvoid)
	{
		if (ival == ANMINTERFACEID(CAnmUrlObject))
		{
			CAnmUrlObject **ppUrlObj = (CAnmUrlObject **) pvoid;
			*ppUrlObj = this;
			return true;
		}

		return false;
	}


	// New methods
	static CAnmObject *Create();

	// Accessors

	virtual void SetScriptSymbol(class CAnmScriptSymbol *pScriptSym)
	{
		m_scriptsymbol = pScriptSym;
	}

	virtual class CAnmScriptSymbol *GetScriptSymbol()
	{
		return m_scriptsymbol;
	}

	void SetDirectOutput(Boolean directOutputs);
	Boolean GetDirectOutput()
	{
		return m_directOutput;
	}
	void GetDirectOutput(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_directOutput;
	}

	void SetMustEvaluate(Boolean mustEvaluate);
	Boolean GetMustEvaluate()
	{
		return m_mustEvaluate;
	}
	void GetMustEvaluate(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_mustEvaluate;
	}

	void SetUrl(StringArray *pUrl);
	// N.B.: GetUrl methods defined in CAnmUrlObject superclass

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmScript);

	DECLARE_VALUE(directOutput);
	DECLARE_VALUE(mustEvaluate);
	DECLARE_VALUE(url);

	DECLARE_METHOD(set_directOutput);
	DECLARE_METHOD(set_mustEvaluate);
	DECLARE_METHOD(set_url);

	DECLARE_CALLBACK(directOutput_changed);
	DECLARE_CALLBACK(mustEvaluate_changed);
	DECLARE_CALLBACK(url_changed);

	friend class CAnmScriptSymbol;
	friend class CAnmScriptLanguageBinding;
};

inline CAnmClass *CAnmScript::GetClass()
{
	if (m_scriptsymbol)
		return m_scriptsymbol->GetClass();
	else
		return CAnmScript::m_classdata;
}

#endif // _anmscript_h
