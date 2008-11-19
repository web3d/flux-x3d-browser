/********************************************************************************
 * Flux
 *
 * File: anmsymbol.cpp
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

#include "stdafx.h"
#include "anmsymbol.h"
#include "anmassetcache.h"
#include "anmfields.h"
#include "anmfileloader.h"
#include "anmfilemanager.h"
#include "anmproto.h"
#include "anmprotobuilder.h"
#include "anmscene.h"
#include "anmscript.h"
#include "anmshaderprogram.h"
#include "anmworld.h"

// Attribute symbols
CAnmAttributeSymbol::CAnmAttributeSymbol(String attrName, CLASSMEMBERID mid, void *defaultValue)
	: CAnmSymbol(attrName)
{
	m_mid = mid;
	assert(m_mid != NULL);

	m_defaultValue = NULL;
	if (defaultValue != NULL)
		m_defaultValue = CAnmField::CreateField(mid->valueType, defaultValue);
}

CAnmAttributeSymbol::~CAnmAttributeSymbol()
{
	SafeUnRef(m_defaultValue);
}

void CAnmAttributeSymbol::SetDefaultValue(CAnmField *defaultValue)
{
	SafeUnRef(m_defaultValue);
	m_defaultValue = defaultValue;
	m_defaultValue->Ref();
}

// Class symbols
CAnmClassSymbol::CAnmClassSymbol(String className, CAnmClass *pClass)
	: CAnmSymbol(className)
{
	m_nodeclass = pClass;
	m_attributeSymbols = new CAnmAttributeList;
	m_defaultContainerField = new CAnmString(ANMDEFAULT_CONTAINERFIELD);
}

// Dynamic class symbol-- user defined such as Protos and Scripts
CAnmDynClassSymbol::CAnmDynClassSymbol(String className)
	: CAnmClassSymbol(className, NULL)
{
	m_nodeclass = new CAnmClass();
	memset(m_nodeclass, 0, sizeof(CAnmClass));
	m_nodeclass->m_classSymbol = this;
	m_nodeclass->m_className = (char *) className->GetBuf();	// N.B.: heap?
	m_nodeclass->m_createObject = NULL;
	m_nodeclass->m_superclass = NULL;
	m_nodeclass->m_numMembers = 0;
	m_curMemberOffset = 0;
}

CAnmDynClassSymbol::~CAnmDynClassSymbol()
{
	delete m_nodeclass;

	// N.B.: we'll need to delete the user-defined attrs on destruction
}

CAnmAttributeSymbol *CAnmDynClassSymbol::AddAttribute(String name, eValueType valueType,
											   eAnmClassMemberType memberType, void *defaultValue)
{
	// create a new class member
	CLASSMEMBERID mid = new CAnmClassMember();
	memset(mid, 0, sizeof(CAnmClassMember));
	mid->memberType = memberType;
	mid->memberOffset = m_curMemberOffset++;
	mid->name = name->GetBuf();				// N.B. string heap?
	mid->valueType = valueType;

	// make a new symbol for it
	CAnmAttributeSymbol *pAttrSymbol = new CAnmAttributeSymbol(name, mid, defaultValue);
	m_attributeSymbols->Add(pAttrSymbol);

	return pAttrSymbol;
}

void CAnmDynClassSymbol::EndDeclaration()
{
	int newnummembers = m_nodeclass->m_numMembers + m_curMemberOffset;

	m_nodeclass->m_memberTable = (CAnmClassMember **) realloc(m_nodeclass->m_memberTable,
		newnummembers * sizeof(CAnmClassMember *));

	// add newly defined members to the class's member table
	int i;
	CAnmAttributeList::iterator iter;
	CAnmAttributeSymbol *pAttrSym;

	// first, skip over the predefined ones
	for (i = 0, iter = m_attributeSymbols->begin();
		i < m_nodeclass->m_numMembers; iter++, i++)
			;

	// now add these to the end of the list
	for (i = m_nodeclass->m_numMembers; 
		iter != m_attributeSymbols->end();
		iter++, i++ )
	{
		pAttrSym = (CAnmAttributeSymbol *) *iter;
		m_nodeclass->m_memberTable[i] = pAttrSym->GetClassMember();
	}

	m_nodeclass->m_numMembers = newnummembers;
}


// Proto symbols
CAnmProtoSymbol::CAnmProtoSymbol(cApplication *pApp, String protoName, CAnmProtoSymbol *pParentScope, BOOL isExtern)
	: CAnmDynClassSymbol(protoName),
	m_app(pApp)
{
	// fill in node class structure to bootstrap object creation
	m_nodeclass->m_createObject = CAnmProto::Create;
	m_nodeclass->m_superclass = CAnmProto::m_classdata;

	m_parentScope = pParentScope;

	m_classSymbols = new CAnmSymbolTable;
	m_nodeSymbols = new CAnmSymbolTable;

	if (!isExtern)
		m_protoBuilder = new CAnmProtoBuilder(pApp, this);
	else
		m_protoBuilder = NULL;

	m_protoSymbolForExtern = NULL;
	m_protoBuilderForExtern = NULL;
	m_isExtern = isExtern;
	m_urlForExtern = NULL;
}

CAnmProtoSymbol::~CAnmProtoSymbol()
{
	delete m_classSymbols;
	delete m_nodeSymbols;
	SafeUnRef(m_urlForExtern);
}

void CAnmProtoSymbol::SetUrl(String baseUrl, StringArray *pUrl)
{
	CAnmUrlObject::SetBaseUrl(baseUrl);
	CAnmUrlObject::SetUrl(pUrl);
	Load(m_app);
}

class CAnmAsset *CAnmProtoSymbol::handleUrlLoaded(String pRequestUrl, String pCacheFileName)
{
	NodeArray *pNodesReturn = NULL;

	CAnmSceneSymbol *pSceneSymbol = NULL;

	BEGIN_PROFILE_BLOCK("EXTERNPROTO LOAD");

	pSceneSymbol = CAnmFileLoader::LoadScene(pCacheFileName->GetBuf(), 
		GetBaseUrl(), m_app);

	if (pSceneSymbol)
		handleSceneLoaded(pSceneSymbol, pRequestUrl);

	END_PROFILE_BLOCK("EXTERNPROTO LOAD");

	if (pSceneSymbol)
		return new CAnmSceneAsset(pCacheFileName, pSceneSymbol);
	else
		return NULL;
}





/**************

//  krv: we dont use this any more, but lets keep it here in case we need this again.


String CAnmProtoSymbol::GetBaseUrlForChildren( String pRequestUrl ) 
{
	String pBaseUrl = GetBaseUrl( );
	if( pRequestUrl && pBaseUrl ) {
		// Get just path of Request.
		//

		char BaseUrlOfRequest[ANMMAXSTRBUF];
		char FullUrlOfRequest[ANMMAXSTRBUF];

		ComputeFullName(pRequestUrl->GetBuf(), pBaseUrl->GetBuf(), FullUrlOfRequest );
		ComputeBaseUrl( FullUrlOfRequest, BaseUrlOfRequest, ANMMAXSTRBUF);
		return new CAnmString( BaseUrlOfRequest );
	}
	return pBaseUrl;
}
****/ 


void CAnmProtoSymbol::handleUrlLoaded(String pRequestUrl, CAnmAsset *pAsset)
{
	assert(pAsset);
	assert(pAsset->GetType() == eSceneAsset);

	CAnmSceneAsset *pSceneAsset = (CAnmSceneAsset *) pAsset;
	handleSceneLoaded(pSceneAsset->GetSceneSymbol(), pRequestUrl);
}

void CAnmProtoSymbol::handleSceneLoaded(CAnmSceneSymbol *pSceneSymbol, String pRequestUrl)
{
	assert(pSceneSymbol);

	CAnmSymbolTable *pClassSymbols = pSceneSymbol->GetClassSymbols();
	CAnmProtoSymbol *pExternProtoSym = NULL;
	if (pClassSymbols)
	{
		// N.B.: we should get some RTTI going on this; what if it's not a Proto in the file?!
		assert(pRequestUrl);

		// look for #ProtoName library thingie
		const char *cp = pRequestUrl->GetBuf();
		char *protoname = strchr(cp, '#');

		if (protoname)
			protoname++;
		else
			protoname = (char *) m_name->GetBuf();

		if (strlen(protoname))
			pExternProtoSym = (CAnmProtoSymbol *) pClassSymbols->FindSymbol(protoname);
	}
	else
	{
		int debug = 1;
	}

	if (pExternProtoSym)
	{
		Lock();

		m_protoSymbolForExtern = pExternProtoSym;
		m_protoBuilderForExtern = pExternProtoSym->GetBuilder();
		if (m_protoBuilderForExtern)
		{
			while (!m_protosToFinalize.empty())
			{
				CAnmProto *pProto = m_protosToFinalize.front();
				CAnmWorld *pWorld = pProto->GetWorld();

				m_protosToFinalize.pop_front();
				FinalizeProto(pProto, m_protoBuilderForExtern);

				pProto->SetBaseUrl( GetBaseUrl() );
				pProto->UnRef();

				if (pWorld)
				{
					pWorld->Lock();
					pProto->Realize();
					pWorld->Unlock();
				}
			}
		}

		Unlock();
	}
}

void CAnmProtoSymbol::InitObject(CAnmObject *pObject)
{
	assert(pObject != NULL);
	CAnmProto *pProto = (CAnmProto *) pObject;

	// Tell the proto object about my type
	pProto->SetProtoSymbol(this);
	
	// Tell the proto how many members it has
	pProto->AllocMembers(m_curMemberOffset);

	CAnmClassMember *pMember = NULL;
	for (int i = 0; i < m_nodeclass->m_numMembers; i++ )
	{
		pMember = m_nodeclass->m_memberTable[i];
		if (pMember->memberType == eAnmCallbackMember)
			pProto->InitCallbackList(pMember);
	}

	if (m_protoBuilder)
		FinalizeProto(pProto, m_protoBuilder);
	else
	{
		Lock();
		if (m_protoBuilderForExtern)
		{
			FinalizeProto(pProto, m_protoBuilderForExtern);
		}
		else
		{
			pProto->Ref();
			m_protosToFinalize.push_back(pProto);
		}
		Unlock();
	}
}

void CAnmProtoSymbol::FinalizeProto(CAnmProto *pProto, CAnmProtoBuilder *pProtoBuilder)
{
	// Execute proto builder code
	assert(pProtoBuilder != NULL);
	if (pProtoBuilder == m_protoBuilderForExtern)
		pProtoBuilder->SetProtoSymForExtern(this);

	NodeArray *pNodes = NULL;
	
	pProtoBuilder->Build(pProto, &pNodes);
	if (pNodes)
		pProto->SetSceneGraph(pNodes);

	// Set field values from the defaults (maybe had to wait till extern load)
	if (m_protoBuilderForExtern)
		m_protoBuilderForExtern->GetProtoSym()->InitProtoFields(pProto);
	else
		InitProtoFields(pProto);

	// Now finalize will commit all deferred default and user-set values
	pProto->Finalize();
}

void CAnmProtoSymbol::InitProtoFields(CAnmProto *pProto)
{
	CAnmAttributeList::iterator iter;
	CAnmAttributeSymbol *pAttrSym = NULL;
	CAnmClassMember *pMember = NULL;
	CAnmField *defaultValue;

	int i;
	for (i = 0, iter = m_attributeSymbols->begin();
		iter != m_attributeSymbols->end();
		iter++, i++)
	{
		pAttrSym = (CAnmAttributeSymbol *) *iter;
		pMember = pAttrSym->GetClassMember();
		defaultValue = pAttrSym->GetDefaultValue();
		if (defaultValue)
			pProto->SetValueFromDefault(pMember, defaultValue->GetData());
	}
}

void CAnmProtoSymbol::AddClassSymbol(CAnmClassSymbol *pClassSymbol)
{
	m_classSymbols->AddSymbol(pClassSymbol);
}

void CAnmProtoSymbol::AddNodeSymbol(CAnmNodeSymbol *pNodeSymbol)
{
	// if duplicate DEF, remove the old one first
	CAnmSymbol *pSym = m_nodeSymbols->FindSymbol((char *) pNodeSymbol->GetName()->GetBuf());
	if (pSym)
	{
		m_nodeSymbols->RemoveSymbol(pSym);
	}
	m_nodeSymbols->AddSymbol(pNodeSymbol);
}

CAnmClassSymbol *CAnmProtoSymbol::FindClassSymbol(char *name)
{
	if (m_protoSymbolForExtern)
		return m_protoSymbolForExtern->FindClassSymbol(name);

	CAnmClassSymbol *pClassSym = (CAnmClassSymbol *) m_classSymbols->FindSymbol(name);

	if (pClassSym == NULL && m_parentScope != NULL)
		return m_parentScope->FindClassSymbol(name);
	
	return pClassSym;
}

CAnmNodeSymbol *CAnmProtoSymbol::FindNodeSymbol(char *name)
{
	CAnmNodeSymbol *pNodeSym = (CAnmNodeSymbol *) m_nodeSymbols->FindSymbol(name);

	if (pNodeSym == NULL && m_parentScope != NULL)
		return m_parentScope->FindNodeSymbol(name);
	
	return pNodeSym;
}

CAnmAttributeSymbol *CAnmProtoSymbol::FindAttribute(const char *name)
{
	CAnmAttributeSymbol *pAttrSym = CAnmClassSymbol::FindAttribute(name);

	if (pAttrSym == NULL && m_parentScope != NULL)
		return m_parentScope->FindAttribute(name);
	
	return pAttrSym;
}


CAnmAttributeSymbol *CAnmProtoSymbol::FindAttribute(const char *name, eAnmClassMemberType memberType)
{
	CAnmAttributeSymbol *pAttrSym = CAnmClassSymbol::FindAttribute(name, memberType);

	if (pAttrSym == NULL && m_parentScope != NULL)
		return m_parentScope->FindAttribute(name, memberType);
	
	return pAttrSym;
}

// Scene symbols-- for top-level scenes and inlines only
CAnmSceneSymbol::CAnmSceneSymbol(cApplication *pApp, String sceneName, 
								 CAnmProtoSymbol *pParentScope)
	: CAnmProtoSymbol(pApp, sceneName, pParentScope)
{
	// It's a trick; we're just overriding the create function 
	// and class info to build a subclass of the base Proto object
	m_nodeclass->m_createObject = CAnmScene::Create;
	m_nodeclass->m_superclass = CAnmScene::m_classdata;

	m_encodingType = eAnmX3DBadEncodingValue;

}

CAnmSceneSymbol::~CAnmSceneSymbol()
{
	int sz = m_metadatakeys.size();
	for (int i = 0; i < sz; i++)
	{
		m_metadatakeys[i]->UnRef();
		m_metadatavalues[i]->UnRef();
	}
}

CAnmScene *CAnmSceneSymbol::CreateScene(cApplication *pApp, String baseUrl)
{
	// Create a new scene object
	CAnmScene *pScene = (CAnmScene *) this->GetClass()->Create();

	// Bind new scene to the app
	pScene->SetApp(pApp);

	// Set file header values
	pScene->SetEncoding(m_encodingType);
	pScene->SetSpecificationVersion(m_specificationVersion);
	
	int sz = m_metadatakeys.size();
	int sz2 = m_metadatavalues.size();
	if (sz > sz2)
		sz = sz2;

	for (int i = 0; i < sz; i++)
		pScene->SetMetaData(m_metadatakeys[i], m_metadatavalues[i]);

	// Set the base url for descendants
	pScene->SetBaseUrl(baseUrl);

	// Done
	return pScene;
}


void CAnmSceneSymbol::AddMetaData(String key, String value)
{
	m_metadatakeys.push_back(key);
	m_metadatavalues.push_back(value);

	key->Ref();
	value->Ref();
}

// Script symbols
CAnmScriptSymbol::CAnmScriptSymbol(String scriptName)
	: CAnmDynClassSymbol(scriptName)
{
	// fill in node class structure to bootstrap object creation
	m_nodeclass->m_createObject = CAnmScript::Create;
	m_nodeclass->m_superclass = CAnmScript::m_classdata;

	// copy the base script node class's member table
	CAnmClass *pClass = CAnmScript::m_classdata;
	m_nodeclass->m_numMembers = pClass->m_numMembers;
	m_nodeclass->m_memberTable = new CAnmClassMember *[m_nodeclass->m_numMembers];

	for (int i = 0; i < m_nodeclass->m_numMembers; i++)
	{
		// copy each member
		m_nodeclass->m_memberTable[i] = pClass->m_memberTable[i];

		// also make an attribute symbol for each one; blech
		CAnmClassMember *pMember = pClass->m_memberTable[i];
		CAnmClassSymbol::AddAttribute(pMember->name, pMember);
	}
}

CAnmScriptSymbol::~CAnmScriptSymbol()
{
}


void CAnmScriptSymbol::InitObject(CAnmObject *pObject)
{
	assert(pObject != NULL);
	CAnmScript *pScript = (CAnmScript *) pObject;

	// Tell the proto object about my type
	pScript->SetScriptSymbol(this);
	
	// Tell the proto how many members it has
	pScript->AllocMembers(m_curMemberOffset);

	CAnmClassMember *pMember = NULL;
	for (int i = 0; i < m_nodeclass->m_numMembers; i++ )
	{
		pMember = m_nodeclass->m_memberTable[i];
		if (pMember->memberType == eAnmCallbackMember)
			pScript->InitCallbackList(pMember);
	}

	// Set field defaults from the default values
	InitScriptFields(pScript);
}

void CAnmScriptSymbol::InitScriptFields(CAnmScript *pScript)
{
	CAnmAttributeList::iterator iter;
	CAnmAttributeSymbol *pAttrSym = NULL;
	CAnmClassMember *pMember = NULL;
	CAnmField *defaultValue;

	int i;
	for (i = 0, iter = m_attributeSymbols->begin();
		iter != m_attributeSymbols->end();
		iter++, i++)
	{
		pAttrSym = (CAnmAttributeSymbol *) *iter;
		pMember = pAttrSym->GetClassMember();
		defaultValue = pAttrSym->GetDefaultValue();
		if (defaultValue)
			pScript->SetValue(pMember, defaultValue->GetData());
	}
}

// Shader symbols
CAnmShaderSymbol::CAnmShaderSymbol(String shaderName)
	: CAnmDynClassSymbol(shaderName)
{
	// fill in node class structure to bootstrap object creation
	m_nodeclass->m_createObject = CAnmShaderProgram::Create;
	m_nodeclass->m_superclass = CAnmShaderProgram::m_classdata;

	// copy the base node class's member table
	CAnmClass *pClass = CAnmShaderProgram::m_classdata;
	m_nodeclass->m_numMembers = pClass->m_numMembers;
	m_nodeclass->m_memberTable = new CAnmClassMember *[m_nodeclass->m_numMembers];

	for (int i = 0; i < m_nodeclass->m_numMembers; i++)
	{
		// copy each member
		m_nodeclass->m_memberTable[i] = pClass->m_memberTable[i];

		// also make an attribute symbol for each one; blech
		CAnmClassMember *pMember = pClass->m_memberTable[i];
		CAnmClassSymbol::AddAttribute(pMember->name, pMember);
	}

	m_defaultContainerField = new CAnmString("programs");
}

CAnmShaderSymbol::~CAnmShaderSymbol()
{
}


void CAnmShaderSymbol::InitObject(CAnmObject *pObject)
{
	assert(pObject != NULL);
	CAnmShaderProgram *pShader = (CAnmShaderProgram *) pObject;

	// Tell the proto object about my type
	pShader->SetShaderSymbol(this);
	
	// Tell the proto how many members it has
	pShader->AllocMembers(m_curMemberOffset);

	CAnmClassMember *pMember = NULL;
	for (int i = 0; i < m_nodeclass->m_numMembers; i++ )
	{
		pMember = m_nodeclass->m_memberTable[i];
		if (pMember->memberType == eAnmCallbackMember)
			pShader->InitCallbackList(pMember);
	}

	// Set field defaults from the default values
	InitShaderFields(pShader);
}

void CAnmShaderSymbol::InitShaderFields(CAnmShaderProgram *pShader)
{
	CAnmAttributeList::iterator iter;
	CAnmAttributeSymbol *pAttrSym = NULL;
	CAnmClassMember *pMember = NULL;
	CAnmField *defaultValue;

	int i;
	for (i = 0, iter = m_attributeSymbols->begin();
		iter != m_attributeSymbols->end();
		iter++, i++)
	{
		pAttrSym = (CAnmAttributeSymbol *) *iter;
		pMember = pAttrSym->GetClassMember();
		defaultValue = pAttrSym->GetDefaultValue();
		if (defaultValue)
			pShader->SetValue(pMember, defaultValue->GetData());
	}
}


// Symbol table management

CAnmSymbolTable::CAnmSymbolTable(int numBuckets)
{
	m_buckets = new HashBucket *[numBuckets];
	for (int i = 0; i < numBuckets; i++)
		m_buckets[i] = new HashBucket();

	m_numBuckets = numBuckets;
}

CAnmSymbolTable::~CAnmSymbolTable()
{
	for (int i = 0; i < m_numBuckets; i++)
		delete m_buckets[i];
}

unsigned CAnmSymbolTable::Hash(const char* str)
{
	unsigned hashValue = 0;
	char c;

	// classic hashing function; bitwise Xor followed by shift. intended to jumble
	// the bits as much as possible
	while (c = *str++) {
		hashValue <<= 1;
		hashValue ^= c;
	}
	return hashValue;
}

void CAnmSymbolTable::AddSymbol(CAnmSymbol *pSymbol)
{
	unsigned hashValue = Hash(pSymbol->GetName()->GetBuf());
	HashBucket *pBucket;

	int index;

	index = hashValue % m_numBuckets;		// open hash; this will get us to
											// a list of entries
	pBucket = m_buckets[index];

	pBucket->AddEntry(pSymbol, hashValue);

	// Add to our linear list too
	m_symbols.push_back(pSymbol);
}

void CAnmSymbolTable::RemoveSymbol(CAnmSymbol *pSymbol)
{
	unsigned hashValue = Hash(pSymbol->GetName()->GetBuf());
	HashBucket *pBucket;
	HashBucket::iterator iter;
	HashEntry *pEntry ;

	int index;

	index = hashValue % m_numBuckets;		// open hash; this will get us to
											// a list of entries
	pBucket = m_buckets[index];

	for( 
		iter = pBucket->begin(); 
		iter != pBucket->end();
		iter++ )
		{
			pEntry = *iter;
			if (pEntry->GetHashValue() == hashValue
				&& !strcmp(pEntry->GetSymbol()->GetName()->GetBuf(), pSymbol->GetName()->GetBuf()))
			{
				pBucket->remove(pEntry);
				break;
			}
		}

	m_symbols.remove(pSymbol);

}

CAnmSymbol *CAnmSymbolTable::FindSymbol(const char * str)
{
	unsigned hashValue = Hash(str);
	HashBucket *pBucket;
	HashBucket::iterator iter;
	HashEntry *pEntry ;
	CAnmSymbol *pSymbol = NULL;

	int index;

	index = hashValue % m_numBuckets;		// open hash; this will get us to
											// a list of entries
	pBucket = m_buckets[index];

	for( 
		iter = pBucket->begin(); 
		iter != pBucket->end();
		iter++ )
		{
			pEntry = *iter;
			if (pEntry->GetHashValue() == hashValue
				&& !strcmp(pEntry->GetSymbol()->GetName()->GetBuf(), str))
			{
				pSymbol = pEntry->GetSymbol();
				break;
			}
		}

	return pSymbol;
}

