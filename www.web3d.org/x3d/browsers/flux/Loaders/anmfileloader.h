/********************************************************************************
 * Flux
 *
 * File: anmfileloader.h
 * Description: File loader class
 *						Dispatches to specific types: X3D, VRML, Binary
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

#ifndef _anmfileloader_h
#define _anmfileloader_h

#include "anmnodedefs.h"
#include "anmsymbol.h"

#include <stack>

#define BROWSER_PROTO_NAME	"#Browser"
#define SCENE_PROTO_NAME	"#Scene"

#define ANMX3DSPEC_VERSION_2_0	"2.0"
#define ANMX3DSPEC_VERSION_3_0	"3.0"

class EXPORT CAnmFileLoader
{
protected :

	class cApplication						*m_app;
	BOOL									 m_consoleOutput;
	String									 m_baseUrl;
	static class CAnmProtoSymbol			*m_globalNames;
	// static CAnmCriticalSection				 m_criticalSection;
	class CAnmSceneSymbol					*m_sceneNames;
	static std::vector<CAnmFileLoader *>			 m_loaders;

	class CAnmFileReader					*m_reader;
	std::stack<class CAnmClassSymbol *>		 m_classstack;

	class CAnmClassSymbol 					*m_currentClassSymbol;
	class CAnmNodeSymbol 					*m_nodeSymbolDefining;
	class CAnmProtoSymbol					*m_protoDeclaring;
	class CAnmScriptSymbol					*m_scriptDefining;
	class CAnmShaderSymbol					*m_shaderDefining;

	eAnmClassMemberType						 m_fieldAccess;
	int										 m_nodelevel;

	int										 m_skipLevel;
	eAnmX3DEncoding							 m_encodingType;
	char									*m_specificationVersion;

	std::vector<String>						 m_metadatakeys;
	std::vector<String>						 m_metadatavalues;

	void CreateClassSymbols();
	void InitClassSymbols(CAnmSymbolTable *pClassSymbols);
	void AddClassSymbol(CAnmSymbolTable *pClassSymbols, const char *classname,
									CAnmClass *pClass, const char *defContainerField =
									ANMDEFAULT_CONTAINERFIELD);

	void PushClass(class CAnmClassSymbol *pClassSymbol);
	class CAnmClassSymbol *PopClass();


	void DoAddIS(class CAnmAttributeSymbol *pAttrSym, class CAnmAttributeSymbol *pISAttrSym);

	static eAnmReturnStatus LoadFile(const char *filename,
		String baseUrl,
		class CAnmScene **ppSceneReturn,
		class CAnmSceneSymbol **ppSceneSymbol, class cApplication *pApp,
		BOOL consoleOutput = TRUE);

	static eAnmReturnStatus LoadFile(String source,
		String baseUrl,
		class CAnmScene **ppSceneReturn,
		class CAnmSceneSymbol **ppSceneSymbol, class cApplication *pApp,
		BOOL consoleOutput = TRUE);

	// Readers for different formats
	eAnmReturnStatus LoadVRML97(const char *source, bool fromFile);
	eAnmReturnStatus LoadXML(const char *source, bool fromFile);
	
public :

	// constructor/destructor
	CAnmFileLoader(class cApplication *pApp, String baseUrl, BOOL consoleOutput = TRUE);
	virtual ~CAnmFileLoader();


	// static functions
	static class CAnmScene *CreateScene(const char *filename,
		String baseUrl, cApplication *pApp, BOOL consoleOutput = TRUE);

	static class CAnmScene *CreateScene(String source,
		String baseUrl, cApplication *pApp, BOOL consoleOutput = TRUE);

	static class CAnmSceneSymbol *LoadScene(const char *filename,
		String baseUrl, cApplication *pApp, BOOL consoleOutput = TRUE);

	static CAnmSymbolTable *GetClassSymbols();

	static void UnloadAllClassSymbols();


	// Scene construction methods
	class CAnmClassSymbol *BeginMakeNode(String nodetypename);
	void EndMakeNode();

	void DefNode(String defname);
	CAnmClassSymbol *UseNode(String usename);

	void NullNode();

	void MakeRoute(String fromName, String fromAttr,
		String toName, String toAttr);

	class CAnmClassSymbol *BeginMakeScript();
	void EndMakeScript();

	class CAnmClassSymbol *BeginMakeShader();
	void EndMakeShader();

	void SetValue(class CAnmAttributeSymbol *pAttrSym,
		void *pData);

	void AppendNode(class CAnmAttributeSymbol *pAttrSym);

	class CAnmAttributeSymbol *LookupAttribute(char *name);

	void BeginProtoDeclaration(String prototypename, BOOL isExternProto = FALSE);
	void EndProtoDeclaration(StringArray **ppExternProtoUrl = NULL);

	void AddClassSymbol(class CAnmClassSymbol *pClassSymbol);
	class CAnmClassSymbol *LookupClassSymbol(char *classname);

	void AddNodeSymbol(class CAnmNodeSymbol *pNodeSymbol);
	class CAnmNodeSymbol *LookupNodeSymbol(char *nodename);
	
	void SetFieldAccess(eAnmClassMemberType memberType);
	
	CAnmAttributeSymbol *AddField(eValueType fieldType, String fieldName, void *defaultValue);
	CAnmAttributeSymbol *AddField(eValueType fieldType, String fieldName, void *defaultValue, String ISName);
	void AddEventIn(eValueType fieldType, String eventInName);
	void AddEventOut(eValueType fieldType, String eventOutName);

	void AddIS(String name, String ISName);
	void AddIS(class CAnmAttributeSymbol *pAttrSym, String ISName);
	void AddIS(class CAnmAttributeSymbol *pAttrSym, class CAnmAttributeSymbol *pISAttrSym);
	void AddISField(eValueType fieldType, String name, String ISName);
	void AddISEventIn(eValueType fieldType, String name, String ISName);
	void AddISEventOut(eValueType fieldType, String name, String ISName);

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

	virtual void AddMetaData(String key, String value);

	static class CAnmSceneSymbol *CreateNewTopLevelScene(class cApplication *pApp);

	// Utility methods
	virtual void WriteToConsole(char *str);

	String GetBaseUrl()
	{
		return m_baseUrl;
	}

};

#endif // _anmfileloader_h
