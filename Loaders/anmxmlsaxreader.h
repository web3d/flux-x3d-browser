/********************************************************************************
 * Flux
 *
 * File: anmxmlsaxreader.h
 * Description: XML SAX reader class
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

#ifndef _anmxmlsaxreader_h
#define _anmxmlsaxreader_h

#include "anmxmlreader.h"
#include "SAXContentHandlerImpl.h"
#include "SAXErrorHandlerImpl.h"
#include "SAXLexicalHandlerImpl.h"

#include <stack>

typedef struct sAnmClassAndContainers {
	class CAnmClassSymbol			*pClassSymbol;
	class CAnmAttributeSymbol		*pParentContainerField;

	sAnmClassAndContainers()
	{
		pClassSymbol = NULL;
		pParentContainerField = NULL;
	}

} sAnmClassAndContainers ;

#define FIELD_UNKNOWN_FIELD_TYPE ((eValueType) -1)
#define ACC_UNKNOWN_ACCESS_TYPE ((eAnmClassMemberType) -1)

typedef struct sAnmFieldDeclaration {
	String						 fieldName;
	eValueType					 fieldType;
	eAnmClassMemberType			 accessType;
	void						*defaultValue;
	bool						 gotNode;

	sAnmFieldDeclaration()
	{
		fieldName = NULL;
		fieldType = FIELD_UNKNOWN_FIELD_TYPE;
		accessType = ACC_UNKNOWN_ACCESS_TYPE;
		defaultValue = NULL;
		gotNode = false;
	}

} sAnmFieldDeclaration ;

typedef struct sAnmFieldValue {
	char						*fieldName;
	void						*value;
	class CAnmAttributeSymbol	*attrSym;

	sAnmFieldValue()
	{
		fieldName = NULL;
		value = NULL;
		attrSym = NULL;
	}

} sAnmFieldValue ;

#define ANMCDATABUFALLOC 2048

class CAnmXMLSAXReader : public CAnmXMLReader,
	public SAXContentHandlerImpl,
	public SAXLexicalHandlerImpl, 
	public SAXErrorHandlerImpl
{
protected :

	bool							 m_inX3D;
	bool							 m_inHead;
	bool							 m_headerDone;
	bool							 m_inScene;
	bool							 m_inScript;
	bool							 m_inShader;
	bool							 m_inProtoInterface;
	bool							 m_inIS;
	bool							 m_inProfile;
	bool							 m_inComponent;
	bool							 m_inImport;
	bool							 m_inExport;
	bool							 m_gotUse;
	bool							 m_inCDATA;
	sAnmFieldDeclaration			 m_fieldDecl;
	ISAXLocator						*m_locator;
	bool							 m_boolval;
	Color							 m_colorval;
	double							 m_doubleval;
	Float							 m_floatval;
	Integer							 m_intval;
	Point2							 m_p2val;
	Point3							 m_p3val;
	Rotation						 m_rotval;
	String							 m_strval;
	wchar_t							*m_cdatabuf;
	int								 m_cdatabufsize;
	int								 m_cdatacharcount;
	int								 m_includeLevel;

	std::stack<sAnmClassAndContainers> m_classstack;
	std::stack<sAnmFieldValue>		   m_fieldvaluestack;
	std::stack<StringArray *>		   m_externprotourlstack;

	// SAXContentHandlerImpl overrides
   virtual HRESULT STDMETHODCALLTYPE startElement( 
        /* [in] */ wchar_t __RPC_FAR *pwchNamespaceUri,
        /* [in] */ int cchNamespaceUri,
        /* [in] */ wchar_t __RPC_FAR *pwchLocalName,
        /* [in] */ int cchLocalName,
        /* [in] */ wchar_t __RPC_FAR *pwchRawName,
        /* [in] */ int cchRawName,
        /* [in] */ ISAXAttributes __RPC_FAR *pAttributes);
    
    virtual HRESULT STDMETHODCALLTYPE endElement( 
        /* [in] */ wchar_t __RPC_FAR *pwchNamespaceUri,
        /* [in] */ int cchNamespaceUri,
        /* [in] */ wchar_t __RPC_FAR *pwchLocalName,
        /* [in] */ int cchLocalName,
        /* [in] */ wchar_t __RPC_FAR *pwchRawName,
        /* [in] */ int cchRawName);

    virtual HRESULT STDMETHODCALLTYPE startDocument();

    virtual HRESULT STDMETHODCALLTYPE putDocumentLocator( 
        /* [in] */ ISAXLocator __RPC_FAR *pLocator);

    virtual HRESULT STDMETHODCALLTYPE characters( 
        /* [in] */ wchar_t __RPC_FAR *pwchChars,
        /* [in] */ int cchChars);

	// SAXLexicalHandlerImpl overrides
    virtual HRESULT STDMETHODCALLTYPE startCDATA ( );
    virtual HRESULT STDMETHODCALLTYPE endCDATA ( );

	// SAXErrorHandlerImpl overrides
    virtual HRESULT __stdcall error (
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode );
    virtual HRESULT __stdcall fatalError (
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode );
    virtual HRESULT __stdcall ignorableWarning (
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode );

	// Helpers
	virtual HRESULT BeginProtoDeclare(ISAXAttributes __RPC_FAR *pAttributes,
		bool isExternProto);
	virtual HRESULT EndProtoDeclare(bool isExternProto);
	virtual HRESULT BeginProtoInstance(BSTR pwchUri, int cchUri,
		ISAXAttributes __RPC_FAR *pAttributes);
	virtual HRESULT EndProtoInstance();
	virtual HRESULT ParseConnect(BSTR pwchUri, int cchUri,
		ISAXAttributes __RPC_FAR *pAttributes);
	virtual HRESULT BeginFieldDeclare(ISAXAttributes __RPC_FAR *pAttributes);
	virtual HRESULT EndFieldDeclare();
	virtual HRESULT BeginFieldValue(ISAXAttributes __RPC_FAR *pAttributes);
	virtual HRESULT EndFieldValue();
	virtual HRESULT ParseRoute(ISAXAttributes __RPC_FAR *pAttributes);
	virtual HRESULT BeginMakeNode(char *nodename,
		BSTR pwchUri,
		int cchUri,
		ISAXAttributes __RPC_FAR *pAttributes,
		bool isProtoInstance = false);
	virtual HRESULT EndMakeNode(char *nodename, bool isProto = false);
	virtual void *ParseValue(eValueType valueType, BSTR bstr, int blen);
	virtual class CAnmClassSymbol *ParseAttributes(char *nodename,
		BSTR pwchUri,
		int cchUri,
		ISAXAttributes __RPC_FAR *pAttributes,
		class CAnmClassSymbol *pParentClassSym,
		class CAnmAttributeSymbol **ppContainerAttrSym,
		bool isProtoInstance = false);

	virtual void PushClass(sAnmClassAndContainers classAndContainers)
	{
		m_classstack.push(classAndContainers);
	}

	virtual void PopClass()
	{
		assert(!m_classstack.empty());
		m_classstack.pop();
	}

	virtual sAnmClassAndContainers CurrentClass()
	{
		sAnmClassAndContainers cc;

		if (!m_classstack.empty())
			return m_classstack.top();
		else
			return cc;
	}

	void BufferCData(wchar_t __RPC_FAR *pwchChars, int cchChars);

public :

	CAnmXMLSAXReader(class CAnmFileLoader *pLoader);
	virtual ~CAnmXMLSAXReader();

	virtual eAnmReturnStatus Read(const char *source, bool fromFile);

	virtual int GetLineNumber();

};


#endif // _anmxmlsaxreader_h
