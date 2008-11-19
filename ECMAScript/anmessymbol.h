/********************************************************************************
 * Flux
 *
 * File: anmessymbol.h
 * Description: ECMAScript symbol table support
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

#ifndef _anmessymbol_h
#define _anmessymbol_h

#include "anmecmascript.h"
#include "anmesstackmachine.h"
#include "anmsymbol.h"

// this one's special
#define THIS_NAME "this"

typedef enum eESSymbolType {
	eVarSymbol,
	eFunctionSymbol,
	eTypeSymbol,
} eESSymbolType ;

class CAnmESSymbol : public CAnmSymbol
{
protected :

	eESValueType			m_type;
	eESSymbolType			m_symType;

public :

	CAnmESSymbol(String name, eESSymbolType symType) : CAnmSymbol(name)
	{
		m_type = eUndeclared;
		m_symType = symType;
	}

	~CAnmESSymbol()
	{
	}

	// Accessors
	virtual void SetType(eESValueType type)
	{
		m_type = type;
	}

	virtual eESValueType GetType()
	{
		return m_type;
	}

	virtual void SetSymType(eESSymbolType symType)
	{
		m_symType = symType;
	}

	virtual eESSymbolType GetSymType()
	{
		return m_symType;
	}
};

typedef enum eESVarSymbolType {
	eLocalVariable,
	eGlobalVariable,
	eMemberVariable,
} eESVarSymbolType ;

class CAnmESVarSymbol : public CAnmESSymbol
{
protected :

	eESVarSymbolType		 m_vartype;
	union
	{
	ESLOCALOFFSET			 m_localOffset;
	AnmESVar				*m_globalAddr;
	CLASSMEMBERID			 m_mid;
	};

public :

	CAnmESVarSymbol(String name, eESVarSymbolType vartype) : CAnmESSymbol(name, eVarSymbol)
	{
		m_vartype = vartype;
		m_localOffset = ESBADLOCALADDR;
	}

	// Accessors
	virtual eESVarSymbolType GetVarType()
	{
		return m_vartype;
	}

	virtual void SetLocalOffset(ESLOCALOFFSET localOffset)
	{
		m_localOffset = localOffset;
	}

	virtual ESLOCALOFFSET GetLocalOffset()
	{
		return m_localOffset;
	}

	virtual void SetGlobalAddress(AnmESVar *pGlobalAddr)
	{
		m_globalAddr = pGlobalAddr;
	}

	virtual AnmESVar *GetGlobalAddress()
	{
		return m_globalAddr;
	}

	virtual void SetClassMember(CLASSMEMBERID mid)
	{
		m_mid = mid;
	}

	virtual CLASSMEMBERID GetClassMember()
	{
		return m_mid;
	}
};

#define MAXTMPNAMEBUF 64
#define TMPNAMEPREFIX "__tmp"

class CAnmESFunctionSymbol : public CAnmESSymbol
{
protected :

	CAnmSymbolList			m_locals;
	ESLOCALOFFSET			m_procaddress;
	int						m_localOffset;
	int						m_nargs;
	int						m_ntemps;

public :

	CAnmESFunctionSymbol(String name) : CAnmESSymbol(name, eFunctionSymbol)
	{
		m_type = eObject;
		m_procaddress = ESBADLOCALADDR;
		m_localOffset = 0;
		m_nargs = 0;
		m_ntemps = 0;
	}

	virtual CAnmESVarSymbol *AddLocal(String name)
	{
		CAnmESVarSymbol *pVarSymbol = new CAnmESVarSymbol(name, eLocalVariable);
		pVarSymbol->SetLocalOffset(m_localOffset++);
		m_locals.Add(pVarSymbol);
		return pVarSymbol;
	}

	virtual int AddTemp()
	{
		char buf[MAXTMPNAMEBUF];
		sprintf(buf, "%s%d", TMPNAMEPREFIX, m_ntemps);

		AddLocal(new CAnmString(buf));

		m_ntemps++;
		return (m_localOffset - 1);
	}

	virtual CAnmESVarSymbol *FindLocal(char *name)
	{
		return (CAnmESVarSymbol *) m_locals.Find(name);
	}

	virtual CAnmESVarSymbol *AddParam(String name)
	{
		m_nargs++;
		return AddLocal(name);
	}

	// Accessors
	virtual void SetProcAddress(ESLOCALOFFSET addr)
	{
		m_procaddress = addr;
	}

	virtual ESLOCALOFFSET GetProcAddress()
	{
		return m_procaddress;
	}

	virtual int GetNLocals()
	{
		return m_localOffset;
	}

	virtual int GetNArgs()
	{
		return m_nargs;
	}
};

typedef class CAnmESObject *((*pFnCreateESObject)(class CAnmECMAScriptLanguageBinding *pOwner,
							 int nargs, AnmESVar *args));

class CAnmESTypeSymbol : public CAnmESSymbol
{
protected :

	pFnCreateESObject			 m_createObject;

public :

	CAnmESTypeSymbol(String name, pFnCreateESObject createObject)
		: CAnmESSymbol(name, eTypeSymbol)
	{
		m_createObject = createObject;
	}

	AnmESVar CreateObject(class CAnmECMAScriptLanguageBinding *pOwner, int args, AnmESVar *nargs);

	BOOL CanCreate()
	{
		return (m_createObject != NULL);
	}
};


class CAnmESCallReferences : public CAnmSymbol
{
protected :

	std::vector<int>		m_callrefs;

public :

	CAnmESCallReferences(String name)
		: CAnmSymbol(name)
	{
	}

	virtual void AddCallRef(int jumpaddr)
	{
		m_callrefs.push_back(jumpaddr);
	}

	virtual std::vector<int> *GetCallRefs()
	{
		return &m_callrefs;
	}
};

class CAnmESScript
{
protected :

	CAnmSymbolTable				*m_symbolTable;
	CAnmSymbolTable				*m_callRefs;
	CAnmESCode					 m_code;

public :

	CAnmESScript()
	{
		m_symbolTable = new CAnmSymbolTable();
		m_callRefs = new CAnmSymbolTable();
	}

	~CAnmESScript()
	{
		SafeDelete(m_symbolTable);
		SafeDelete(m_callRefs);
	}

	// Methods
	virtual void AddSymbol(CAnmESSymbol *pSymbol)
	{
		m_symbolTable->AddSymbol(pSymbol);
	}

	virtual CAnmESSymbol *FindSymbol(char* str)
	{
		return (CAnmESSymbol *) m_symbolTable->FindSymbol(str);
	}

	virtual void AddCallRef(String subName, int jumpsubaddr)
	{
		assert(subName);

		CAnmESCallReferences *pCallRef = (CAnmESCallReferences *) m_callRefs->FindSymbol(subName->GetBuf());

		if (pCallRef == NULL)
		{
			pCallRef = new CAnmESCallReferences(subName);
			m_callRefs->AddSymbol(pCallRef);
		}

		pCallRef->AddCallRef(jumpsubaddr);
	}

	virtual std::vector <int> *GetCallRefs(String subName)
	{
		assert(subName);

		CAnmESCallReferences *pCallRef = (CAnmESCallReferences *) m_callRefs->FindSymbol(subName->GetBuf());

		if (pCallRef)
			return pCallRef->GetCallRefs();
		else
			return NULL;
	}

	// Accessors
	virtual CAnmESCode *GetCode()
	{
		return &m_code;
	}

};

#endif // _anmessymbol_h

