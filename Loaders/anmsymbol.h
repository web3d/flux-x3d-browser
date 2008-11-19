/********************************************************************************
 * Flux
 *
 * File: anmsymbol.h
 * Description: Symbol table management for file loaders, scripting
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

#ifndef _anmsymbol_h
#define _anmsymbol_h

#include "anmnode.h"
#include "anmcriticalsection.h"
#include "anmurlobject.h"

class CAnmSymbol
{
protected :

	String				m_name;

public :

	CAnmSymbol(String name)
	{
		m_name = name;
		m_name->Ref();
	}

	virtual ~CAnmSymbol()
	{
		SafeUnRef(m_name);
	}

	String GetName()
	{
		return m_name;
	}
};

class CAnmSymbolList : public std::list<CAnmSymbol *>
{
public :

	~CAnmSymbolList()
	{
		CAnmSymbol *pSymbol = NULL;
		iterator iter;

		for( 
			iter = begin(); 
			iter != end();
			iter++ )
		{
			pSymbol = *iter;
			delete pSymbol;
		}
	}

	virtual void Add(CAnmSymbol *pSymbol)
	{
		push_back(pSymbol);
	}

	virtual CAnmSymbol *Find(const char *name)
	{
		CAnmSymbol *pSymbol = NULL, *pFoundSymbol = NULL;
		iterator iter;

		for( 
			iter = begin(); 
			iter != end();
			iter++ )
		{
			pSymbol = *iter;
			if (!strcmp(pSymbol->GetName()->GetBuf(), name))
			{
				pFoundSymbol = pSymbol;
				break;
			}
		}

		return pFoundSymbol;
	}
};

#define NUMHASHBUCKETS 32

class CAnmSymbolTable
{
protected :

	class HashEntry
	{
	protected :
		CAnmSymbol		*m_symbol;
		unsigned		 m_hashvalue;

	public :
		HashEntry(CAnmSymbol *pSymbol, unsigned hashval)
		{
			m_symbol = pSymbol;
			m_hashvalue = hashval;
		}
		
		~HashEntry()
		{
			delete m_symbol;
		}

		CAnmSymbol		*GetSymbol()
		{
			return m_symbol;
		}

		unsigned		 GetHashValue()
		{
			return m_hashvalue;
		}

	};

	class HashBucket : public std::list<HashEntry *>
	{
	public :
		
		HashBucket() {}
		~HashBucket()
		{
			HashEntry *pEntry;
			iterator iter;

			for( 
				iter = begin(); 
				iter != end();
				iter++ )
			{
				pEntry = *iter;
				delete pEntry;
			}
		}

		void AddEntry(CAnmSymbol *pSymbol, unsigned hashval)
		{
			HashEntry *pEntry = new HashEntry(pSymbol, hashval);
			push_back(pEntry);
		}
	};

	int						 m_numBuckets;
	HashBucket				**m_buckets;
	std::list<CAnmSymbol *>	 m_symbols;

	unsigned Hash(const char* str);


public :

	CAnmSymbolTable(int nBuckets = NUMHASHBUCKETS);
	virtual ~CAnmSymbolTable();

	void AddSymbol(CAnmSymbol *pSymbol);
	void RemoveSymbol(CAnmSymbol *pSymbol);
	CAnmSymbol *FindSymbol(const char * str);
	std::list<CAnmSymbol *> *GetSymbols()
	{
		return &m_symbols;
	}
};

class CAnmAttributeSymbol : public CAnmSymbol
{
protected :

	CLASSMEMBERID		 m_mid;
	class CAnmField		*m_defaultValue;

public :

	CAnmAttributeSymbol(String attrName, CLASSMEMBERID mid)
		: CAnmSymbol(attrName)
	{
		m_mid = mid;
		m_defaultValue = NULL;
	}

	CAnmAttributeSymbol(String attrName, CLASSMEMBERID mid, void *defaultValue);
	~CAnmAttributeSymbol();

	CLASSMEMBERID GetClassMember()
	{
		return m_mid;
	}

	void SetDefaultValue(class CAnmField *defaultValue);
	CAnmField *GetDefaultValue()
	{
		return m_defaultValue;
	}

	eValueType GetValueType()
	{
		return ( ( m_mid ) ? m_mid->valueType : eValueBadType );
	}

};

class CAnmAttributeList : public CAnmSymbolList
{
public :

	virtual void Add(CAnmAttributeSymbol *pAttrSymbol)
	{
		assert (pAttrSymbol->GetClassMember());
		CAnmSymbolList::Add(pAttrSymbol);
	}

	virtual CAnmSymbol *Find(const char *name)
	{
		return CAnmSymbolList::Find(name);
	}

	virtual CAnmAttributeSymbol *Find(const char *name, eAnmClassMemberType memberType)
	{
		CAnmAttributeSymbol *pAttrSymbol, *pFoundSymbol = NULL;
		iterator iter;

		for( 
			iter = begin(); 
			iter != end();
			iter++ )
		{
			pAttrSymbol = (CAnmAttributeSymbol *) *iter;

			// do the assert here instead; we should never get here
			assert (pAttrSymbol->GetClassMember());

			if (!strcmp(pAttrSymbol->GetName()->GetBuf(), name)
				&& pAttrSymbol->GetClassMember()->memberType & memberType)
			{
				pFoundSymbol = pAttrSymbol;
				break;
			}
		}

		return pFoundSymbol;
	}

	virtual CAnmAttributeSymbol *Find(CLASSMEMBERID mid)
	{
		CAnmAttributeSymbol *pAttrSymbol, *pFoundSymbol = NULL;
		iterator iter;

		for( 
			iter = begin(); 
			iter != end();
			iter++ )
		{
			pAttrSymbol = (CAnmAttributeSymbol *) *iter;

			// do the assert here instead; we should never get here
			assert (pAttrSymbol->GetClassMember());

			if (pAttrSymbol->GetClassMember() == mid)
			{
				pFoundSymbol = pAttrSymbol;
				break;
			}
		}

		return pFoundSymbol;
	}
};

#define ANMDEFAULT_CONTAINERFIELD	"children"

class CAnmClassSymbol : public CAnmSymbol
{
protected :

	CAnmClass						*m_nodeclass;
	CAnmAttributeList				*m_attributeSymbols;
	String							 m_defaultContainerField;

public :

	// Constructor/destructor
	CAnmClassSymbol(String className, CAnmClass *pClass);

	~CAnmClassSymbol()
	{
		// don't delete nodeclass...
		delete m_attributeSymbols;
		SafeUnRef(m_defaultContainerField);
	}

	// Methods
	virtual void InitObject(CAnmObject *pObject)
	{
	}

	virtual void AddAttribute(const char *name, CLASSMEMBERID mid)
	{
		String s = new CAnmString(name);
		CAnmAttributeSymbol *pAttrSymbol = new CAnmAttributeSymbol(s, mid);
		s->UnRef();
		m_attributeSymbols->Add(pAttrSymbol);
	}

	virtual CAnmAttributeSymbol *FindAttribute(const char *name)
	{
		CAnmSymbol *pSym = m_attributeSymbols->Find(name);
		return (CAnmAttributeSymbol *) pSym;
	}

	virtual CAnmAttributeSymbol *FindAttribute(const char *name, eAnmClassMemberType memberType)
	{
		return m_attributeSymbols->Find(name, memberType);
	}

	virtual CAnmAttributeSymbol *FindAttribute(CLASSMEMBERID mid)
	{
		return m_attributeSymbols->Find(mid);
	}

	// Accessors
	virtual CAnmClass *GetClass()
	{ 
		return m_nodeclass;
	}

	virtual class CAnmSymbolList *GetAttributeSymbols()
	{
		return m_attributeSymbols;
	}

	virtual void SetDefaultContainerField(String containerField)
	{
		SafeUnRef(m_defaultContainerField);
		m_defaultContainerField = containerField;
		m_defaultContainerField->Ref();
	}

	virtual String GetDefaultContainerField()
	{
		return m_defaultContainerField;
	}
};

class CAnmNodeSymbol : public CAnmSymbol
{
protected :

	CAnmNode		*m_node;
	CAnmClassSymbol *m_classSymbol;
	int				 m_localOffset;		// local storage addr for DEF symbol

public :

	CAnmNodeSymbol(String nodeName, CAnmNode *pNode, CAnmClassSymbol *pClassSymbol)
		: CAnmSymbol(nodeName)
	{
		m_node = pNode;
		m_classSymbol = pClassSymbol;
		m_localOffset = -1;
	}

	~CAnmNodeSymbol()
	{
	}

	void SetNode(CAnmNode *pNode)
	{
		m_node = pNode;
	}

	CAnmNode *GetNode()
	{
		return m_node;
	}

	void SetClassSymbol(CAnmClassSymbol *pClassSymbol)
	{
		m_classSymbol = pClassSymbol;
	}

	CAnmClassSymbol *GetClassSymbol()
	{
		return m_classSymbol;
	}

	void SetLocalOffset(int offset)
	{
		m_localOffset = offset;
	}

	int GetLocalOffset()
	{
		return m_localOffset;
	}
};

class CAnmDynClassSymbol : public CAnmClassSymbol
{
protected :

	int									 m_curMemberOffset;

public :

	// Constructor/destructor
	CAnmDynClassSymbol(String className);
	virtual ~CAnmDynClassSymbol();

	// Methods
	virtual CAnmAttributeSymbol *AddAttribute(String name, eValueType valueType, eAnmClassMemberType memberType,
		void *defaultValue = NULL);

	// Declaration help-- need to know when all the fields have been declared
	virtual void EndDeclaration();

	// Accessors
};

class CAnmProtoSymbol : public CAnmDynClassSymbol, public CAnmUrlObject, public CAnmCriticalSection
{
protected :

	CAnmSymbolTable						*m_classSymbols;
	CAnmSymbolTable						*m_nodeSymbols;
	CAnmProtoSymbol						*m_parentScope;
	CAnmProtoSymbol						*m_protoSymbolForExtern;
	class CAnmProtoBuilder				*m_protoBuilder;
	class CAnmProtoBuilder				*m_protoBuilderForExtern;
	BOOL								 m_isExtern;
	StringArray							*m_urlForExtern;
	std::deque<class CAnmProto *>		 m_protosToFinalize;
	class cApplication					*m_app;

	void FinalizeProto(class CAnmProto *pProto, class CAnmProtoBuilder *pProtoBuilder);
	void InitProtoFields(class CAnmProto *pProto);

	// this is how we receive EXTERNPROTO declarations
	virtual class CAnmAsset *handleUrlLoaded(String pRequestUrl, String pCacheFileName);
	virtual void handleUrlLoaded(String pRequestUrl, class CAnmAsset *pAsset);
	void handleSceneLoaded(class CAnmSceneSymbol *pSceneSymbol, String pRequestUrl);
	String CAnmProtoSymbol::GetBaseUrlForChildren( String pRequestUrl );

public :

	// Constructor/destructor
	CAnmProtoSymbol(class cApplication *pApp, String protoName, CAnmProtoSymbol *pParentScope,
		BOOL isExtern = FALSE);
	virtual ~CAnmProtoSymbol();

	// Methods
	virtual void AddClassSymbol(CAnmClassSymbol *pClassSymbol);
	virtual void AddNodeSymbol(CAnmNodeSymbol *pNodeSymbol);

	virtual CAnmClassSymbol *FindClassSymbol(char *name);
	virtual CAnmNodeSymbol *FindNodeSymbol(char *name);
	virtual CAnmAttributeSymbol *FindAttribute(const char *name);
	virtual CAnmAttributeSymbol *FindAttribute(const char *name, eAnmClassMemberType memberType);
	virtual CAnmAttributeSymbol *FindAttribute(CLASSMEMBERID mid)
	{
		return CAnmDynClassSymbol::FindAttribute(mid);
	}


	// Proto instance creation
	virtual void InitObject(CAnmObject *pObject);

	// EXTERNPROTO support
	void SetUrl(String baseUrl, StringArray *pUrl);

	// Accessors
	virtual class CAnmSymbolTable *GetClassSymbols()
	{
		return m_classSymbols;
	}

	virtual class CAnmSymbolTable *GetNodeSymbols()
	{
		return m_nodeSymbols;
	}

	CAnmProtoSymbol *GetParentScope()
	{
		return m_parentScope;
	}

	class CAnmProtoBuilder *GetBuilder()
	{
		return m_protoBuilder;
	}

	BOOL IsExternProto()
	{
		return m_isExtern;
	}

	CAnmProtoSymbol *GetProtoSymbolForExtern()
	{
		return m_protoSymbolForExtern;
	}
};

class CAnmSceneSymbol : public CAnmProtoSymbol
{
protected :

	eAnmX3DEncoding							 m_encodingType;
	char									*m_specificationVersion;

	std::vector<String>						 m_metadatakeys;
	std::vector<String>						 m_metadatavalues;

public :

	// Constructor/destructor
	CAnmSceneSymbol(class cApplication *pApp, 
		String protoName, CAnmProtoSymbol *pParentScope);
	~CAnmSceneSymbol();

	// Methods
	class CAnmScene *CreateScene(class cApplication *pApp, String baseUrl);
	virtual void AddMetaData(String key, String value);

	// Accessors
	void SetEncodingType(eAnmX3DEncoding encodingType)
	{
		m_encodingType = encodingType;
	}

	eAnmX3DEncoding GetEncodingType()
	{
		return m_encodingType;
	}

	void SetSpecificationVersion(char *specificationVersion)
	{
		m_specificationVersion = specificationVersion;
	}

	char *GetSpecificationVersion()
	{
		return m_specificationVersion;
	}

	std::vector<String>	*GetMetaDataKeys()
	{
		return &m_metadatakeys;
	}

	std::vector<String> *GetMetaDataValues()
	{
		return &m_metadatavalues;
	}


};

class CAnmScriptSymbol : public CAnmDynClassSymbol
{
protected :

	void InitScriptFields(class CAnmScript *pScript);

public :

	// Constructor/destructor
	CAnmScriptSymbol(String scriptName);
	virtual ~CAnmScriptSymbol();

	// Script instance creation
	virtual void InitObject(CAnmObject *pObject);

	// Accessors
};

class CAnmShaderSymbol : public CAnmDynClassSymbol
{
protected :

	void InitShaderFields(class CAnmShaderProgram *pShader);

public :

	// Constructor/destructor
	CAnmShaderSymbol(String shaderName);
	virtual ~CAnmShaderSymbol();

	// Script instance creation
	virtual void InitObject(CAnmObject *pObject);

	// Accessors
};


#endif // _anmsymbol_h
