/********************************************************************************
 * Flux
 *
 * File: anmshaderprogram.h
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

#ifndef _anmshaderprogram_h
#define _anmshaderprogram_h

#include "anmnode.h"
#include "anmurlobject.h"
#include "anmsymbol.h"

#define ANMSHADERPROGRAM_DEFAULT_TYPE		"VERTEX"

// callback handling-- have to do it differently from core objects
typedef struct sAnmShaderCallbackList {
	int memberOffset;
	CAnmCallbackList callbackList;
} sAnmShaderCallbackList ;

typedef std::vector<sAnmShaderCallbackList> tAnmShaderCallbackLists;


class CAnmShaderProgram : public CAnmNode, public CAnmUrlObject
{

protected:

	String									 m_type;
	eAnmShaderType							 m_shaderType;
	CAnmGraphics::Shader					*m_shader;

	class CAnmShaderSymbol					*m_shadersymbol;	// type info for this
																// shader

	class CAnmField							**m_members;
	int										 m_nMembers;
	CLASSMEMBERID							*m_memberIDs;

	tAnmShaderCallbackLists					 m_callbackLists;

	// helpers for async shader file loading
	virtual class CAnmAsset *handleUrlLoaded(String pRequestUrl, String pCacheFileName);
	virtual void handleUrlLoaded(String pRequestUrl, class CAnmAsset *pAsset);

	virtual BOOL IsInlineShader();
	virtual void CompileShader(String filename);
	virtual void CompileInlineShader();

	virtual void AllocMembers(int nMembers);
	virtual void InitCallbackList(CLASSMEMBERID mid);
	virtual sAnmShaderCallbackList *LookupCallbackList(CLASSMEMBERID mid);

	virtual eAnmShaderType CalcShaderType();


public:

	// constructor/destructor
	CAnmShaderProgram();
	virtual ~CAnmShaderProgram();

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
	// New methods
	static CAnmObject *Create();
	virtual void CopyValuesToShader();
	virtual CAnmGraphics::Shader *GetRawShader();

	// Accessors
	void SetType(String type);
	String GetType()
	{
		return m_type;
	}
	void GetType(String *pStr)
	{
		assert(pStr);
		*pStr = m_type;
	}

	virtual void SetShaderSymbol(class CAnmShaderSymbol *pShaderSym)
	{
		m_shadersymbol = pShaderSym;
	}

	virtual class CAnmShaderSymbol *GetShaderSymbol()
	{
		return m_shadersymbol;
	}

	virtual eAnmShaderType GetShaderType()
	{
		return m_shaderType;
	}

	void SetUrl(StringArray *pUrl);
	// N.B.: GetUrl methods defined in CAnmUrlObject superclass

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmShaderProgram);

	DECLARE_VALUE(url);
	DECLARE_INITVALUE(type);

	DECLARE_METHOD(set_url);

	DECLARE_CALLBACK(url_changed);

	friend class CAnmShaderSymbol;
};

inline CAnmClass *CAnmShaderProgram::GetClass()
{
	if (m_shadersymbol)
		return m_shadersymbol->GetClass();
	else
		return CAnmShaderProgram::m_classdata;
}

DECLARE_GETIMPLEMENTATION(CAnmShaderProgram);

#endif // _anmshaderprogram_h
