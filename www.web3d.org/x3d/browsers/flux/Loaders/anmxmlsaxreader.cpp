/********************************************************************************
 * Flux
 *
 * File: anmxmlsaxreader.cpp
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

#include "stdafx.h"
#include "anmxmlsaxreader.h"
#include "anmexecutioncontext.h"
#include "anmfileloader.h"
#include "anmmessage.h"
#include "anmsymbol.h"
#include "anmnode.h"
#include "anmgroup.h"

CAnmXMLSAXReader::CAnmXMLSAXReader(CAnmFileLoader *pLoader)
: CAnmXMLReader(pLoader)
{
	m_inX3D = m_inHead = m_inScene = m_inScript = m_inShader =
		m_inProtoInterface = m_inIS = m_gotUse = 
		m_inProfile = m_inComponent = m_inImport = m_inExport = 
		m_headerDone = m_inCDATA = false;

	m_boolval = false;
	m_colorval = Color(0, 0, 0);
	m_doubleval = 0.0;
	m_floatval = 0.f;
	m_intval = 0;
	m_p3val = Point3(0,0,0);
	m_p2val = Point2(0,0);
	m_rotval = Rotation(0, 1, 0, 0);
	m_strval = new CAnmString("");
	m_cdatabuf = NULL;
	m_cdatabufsize = 0;
	m_cdatacharcount = 0;
	m_includeLevel = 0;
}

CAnmXMLSAXReader::~CAnmXMLSAXReader()
{
	SafeDelete(m_cdatabuf);
	m_cdatabufsize = 0;
	m_cdatacharcount = 0;
}

eAnmReturnStatus CAnmXMLSAXReader::Read(const char *source, bool fromFile)
{
	ISAXXMLReader* pRdr = NULL;

	HRESULT hr = CoCreateInstance(
								__uuidof(SAXXMLReader), 
								NULL, 
								CLSCTX_ALL, 
								__uuidof(ISAXXMLReader), 
								(void **)&pRdr);

	char buf[1024];
	
	if(!FAILED(hr)) 
	{
		hr = pRdr->putContentHandler(this);
		hr = pRdr->putErrorHandler(this);

		SAXLexicalHandlerImpl *pImp = this;

		VARIANT vt;
		vt.vt = VT_UNKNOWN;
		vt.punkVal = pImp;
		hr = pRdr->putProperty(L"http://xml.org/sax/properties/lexical-handler",
			vt);

		int len = strlen(source);

		wchar_t *pURL = new wchar_t[len + 1];

		mbstowcs( pURL, source, len + 1);
		
		if (fromFile)
			hr = pRdr->parseURL(pURL);
		else
		{
			variant_t vurl(pURL);
			hr = pRdr->parse(vurl);
		}

	
		delete pURL;

		pRdr->Release();
		if (SUCCEEDED(hr))
		{
			return eAnmReturnAllGood;
		}
		else
		{
			// ANMMSG_DONEPARSINGXMLWITHERRORS : "Done parsing XML with errors."
			WriteToConsole(ANMMSG_DONEPARSINGXMLWITHERRORS);
			return eAnmReturnAllBad;
		}
	}
	else 
	{
		// ANMMSG_COULDNOTCREATESAXREADER : "Could not create SAX reader to load XML."
		WriteToConsole(ANMMSG_COULDNOTCREATESAXREADER);
		return eAnmReturnAllBad;
	}
}

int CAnmXMLSAXReader::GetLineNumber()
{
	HRESULT hr = E_FAIL;
	
	assert(m_locator != NULL);

	int lnno;

	hr = m_locator->getLineNumber(&lnno);

	return lnno;
}

HRESULT CAnmXMLSAXReader::BeginProtoDeclare(ISAXAttributes __RPC_FAR *pAttributes,
		bool isExternProto)
{
	HRESULT hr;
	int len, clen;

	wchar_t *val;

	hr = pAttributes->getLength(&len);

	for (int i = 0; i < len; i++)
	{
		hr = pAttributes->getValue(i, &val, &clen);
		if (SUCCEEDED(hr))
		{
			BSTR uristr, nstr, qbstr;
			int urilen, nlen, qblen;

			hr = pAttributes->getName(i, &uristr, &urilen, &nstr, &nlen, &qbstr, &qblen);
			// search for the attribute name in the class symbol's list
			if (!wcsncmp(nstr, L"name", nlen))
			{
				String protoname = new CAnmString;
				protoname->alloc(clen + 1);
				bstr2cstr(protoname->GetBuf(), val, clen);
				m_loader->BeginProtoDeclaration(protoname, isExternProto);
				protoname->UnRef();
			}
			else if (isExternProto && !wcsncmp(nstr, L"url", nlen))
			{
				ParseStringArray(val, clen);
				m_stringarray->Ref();
				m_externprotourlstack.push(m_stringarray);
			}
		}
	}

	return S_OK;
}

HRESULT CAnmXMLSAXReader::EndProtoDeclare(bool isExternProto)
{
	StringArray *pUrl = NULL;

	if (isExternProto)
	{
		assert(!m_externprotourlstack.empty());
		pUrl = m_externprotourlstack.top();
		m_externprotourlstack.pop();
		m_loader->EndProtoDeclaration(&pUrl);
		pUrl->UnRef();
	}
	else
		m_loader->EndProtoDeclaration(&pUrl);

	return S_OK;
}


HRESULT CAnmXMLSAXReader::BeginProtoInstance(BSTR pwchUri,
											int cchUri,
									ISAXAttributes __RPC_FAR *pAttributes)
{
	char nodename[ANMMAXXMLSTRBUF];
	BSTR val;
	int len;

	HRESULT hr;
    hr = pAttributes->getValueFromName(pwchUri, cchUri, L"name", 4, &val, &len);
	if (SUCCEEDED(hr))
	{
		bstr2cstr(nodename, val, len);
		return BeginMakeNode(nodename, pwchUri, cchUri, pAttributes, true);
	}
	else
	{
	    hr = pAttributes->getValueFromName(pwchUri, cchUri, L"USE", 3, &val, &len);
		if (SUCCEEDED(hr))
			return BeginMakeNode(NULL, pwchUri, cchUri, pAttributes, true);
	}

	return E_FAIL;
}

HRESULT CAnmXMLSAXReader::EndProtoInstance()
{
	return EndMakeNode("ProtoInstance", true);
}

HRESULT CAnmXMLSAXReader::ParseConnect(BSTR pwchUri,
											int cchUri,
									ISAXAttributes __RPC_FAR *pAttributes)
{
	BSTR val;
	int len;
	String nodeField = NULL, protoField = NULL;

	HRESULT hr;
    hr = pAttributes->getValueFromName(pwchUri, cchUri, L"nodeField", 9, &val, &len);
	if (SUCCEEDED(hr))
	{
		nodeField = new CAnmString;
		nodeField->alloc(len + 1);
		bstr2cstr(nodeField->GetBuf(), val, len);
	}

    hr = pAttributes->getValueFromName(pwchUri, cchUri, L"protoField", 10, &val, &len);
	if (SUCCEEDED(hr))
	{
		protoField = new CAnmString;
		protoField->alloc(len + 1);
		bstr2cstr(protoField->GetBuf(), val, len);
	}

	if (nodeField == NULL || protoField == NULL)
	{
		// ANMMSG_CONNECTTAGERROR : "Connect tag requires nodeField and protoField values"
		WriteToConsole(ANMMSG_CONNECTTAGERROR);

		SafeUnRef(nodeField);
		SafeUnRef(protoField);
		return E_FAIL;
	}

	m_loader->AddIS(nodeField, protoField);
	SafeUnRef(nodeField);
	SafeUnRef(protoField);

	return S_OK;
}

typedef struct accessEntry {
	BSTR acctypename;
	eAnmClassMemberType acctype;
} accessEntry; 


const bool BE_STRICT_ABOUT_VRML_KEYWORDS = false;

inline eAnmClassMemberType AccessType(BSTR acctypename, int len)
{
	static accessEntry accessTable[] =
	{
		{ L"inputOnly", eAnmMethodMember },		
		{ L"outputOnly", eAnmCallbackMember },	
		{ L"initializeOnly", eAnmInitMember },
		{ L"inputOutput", eAnmValueMember },
	};

	static int nAccessEntries = sizeof(accessTable) / sizeof(accessEntry);

	for (int i = 0; i < nAccessEntries; i++)
	{
		if (!wcsncmp(acctypename, accessTable[i].acctypename, len))
			return accessTable[i].acctype;
	}


	// N.B.: I think we need some kind of warning here
	if (BE_STRICT_ABOUT_VRML_KEYWORDS)
		return (eAnmClassMemberType) ACC_UNKNOWN_ACCESS_TYPE;

	// For VRML keyword compatibility, you're welcome, Dave.
	static accessEntry vrmlAccessTable[] =
	{
		{ L"eventIn", eAnmMethodMember },		
		{ L"eventOut", eAnmCallbackMember },	
		{ L"field", eAnmInitMember },
		{ L"exposedField", eAnmValueMember },
	};

	static int nVRMLAccessEntries = sizeof(vrmlAccessTable) / sizeof(accessEntry);

	for (i = 0; i < nVRMLAccessEntries; i++)
	{
		if (!wcsncmp(acctypename, vrmlAccessTable[i].acctypename, len))
			return vrmlAccessTable[i].acctype;
	}

	// N.B.: I think we need some kind of warning here
	return (eAnmClassMemberType) ACC_UNKNOWN_ACCESS_TYPE;
}

typedef struct fieldTypeEntry {
	BSTR fieldtypename;
	eValueType fieldtype;
} fieldTypeEntry; 


inline eValueType FieldType(BSTR fieldtypename, int len)
{
	static fieldTypeEntry fieldTypeTable[] =
	{
		{ L"SFBool", eValueBoolean },
		{ L"MFBool", eValueBooleanArray },
		{ L"SFColor", eValueColor },
		{ L"MFColor", eValueColorArray },
		{ L"SFDouble", eValueDouble },
		{ L"MFDouble", eValueDoubleArray },
		{ L"SFFloat", eValueFloat },
		{ L"MFFloat", eValueFloatArray },
		{ L"SFImage", eValueImage },
		{ L"MFImage", eValueImageArray },
		{ L"SFInt32", eValueInteger },
		{ L"MFInt32", eValueIntegerArray },
		{ L"SFNode", eValueNode },
		{ L"MFNode", eValueNodeArray },
		{ L"SFRotation", eValueRotation },
		{ L"MFRotation", eValueRotationArray },
		{ L"SFString", eValueString },
		{ L"MFString", eValueStringArray },
		{ L"SFTime", eValueTime },
		{ L"MFTime", eValueTimeArray },
		{ L"SFVec2f", eValuePoint2 },
		{ L"MFVec2f", eValuePoint2Array },
		{ L"SFVec3f", eValuePoint3 },
		{ L"MFVec3f", eValuePoint3Array },
	};

	static int nFieldTypeEntries = sizeof(fieldTypeTable) / sizeof(fieldTypeEntry);

	for (int i = 0; i < nFieldTypeEntries; i++)
	{
		if (!wcsncmp(fieldtypename, fieldTypeTable[i].fieldtypename, len))
			return fieldTypeTable[i].fieldtype;
	}

	// N.B.: I think we need some kind of warning here
	return (eValueType) FIELD_UNKNOWN_FIELD_TYPE;
}


/*
<!ELEMENT field  ( %ProtoNodes; )* >
<!ATTLIST field
	name       	NMTOKEN  	#REQUIRED
	accessType (inputOnly | outputOnly | initializeOnly | inputOutput) #REQUIRED
	type 	   ( %X3dFieldTypes; ) #REQUIRED
	value      	CDATA   	#IMPLIED
  	xml:space  (default | preserve)	"default"
	appInfo 	%SFString;	#IMPLIED
	documentation	%SFString;	#IMPLIED>
*/

HRESULT CAnmXMLSAXReader::BeginFieldDeclare(ISAXAttributes __RPC_FAR *pAttributes)
{
	HRESULT hr;
	char attrbuf[1024];
	int len, clen;
	wchar_t *val;

	if (!m_inScript && !m_inShader && !m_inProtoInterface)
	{
		// ANMMSG_BADXMLFIELDDECLARATION : "Error at line %d: field declarations allowed only in Script, Shader or ProtoInterface"
		char *errmsg = (char *) malloc(strlen(ANMMSG_BADXMLFIELDDECLARATION ) + 
			ANMMAXDECIMALLENGTH +
			1);

		sprintf(errmsg, ANMMSG_BADXMLFIELDDECLARATION, GetLineNumber());

		WriteToConsole(errmsg);

		delete errmsg;

		return E_FAIL;
	}

	hr = pAttributes->getLength(&len);

	for (int i = 0; i < len; i++)
	{
		hr = pAttributes->getValue(i, &val, &clen);
		if (SUCCEEDED(hr))
		{
			BSTR uristr, nstr, qbstr;
			int urilen, nlen, qblen;

			hr = pAttributes->getName(i, &uristr, &urilen, &nstr, &nlen, &qbstr, &qblen);
			// search for the attribute name in the class symbol's list
			if (!wcsncmp(nstr, L"name", nlen))
			{
				m_fieldDecl.fieldName = new CAnmString();
				m_fieldDecl.fieldName->alloc(clen + 1);
				bstr2cstr(m_fieldDecl.fieldName->GetBuf(), val, clen);
			}
			else if (!wcsncmp(nstr, L"accessType", nlen))
			{
				m_fieldDecl.accessType = AccessType(val, clen);
				if (m_fieldDecl.accessType == ACC_UNKNOWN_ACCESS_TYPE)
				{
					bstr2cstr(attrbuf, val, min(clen, 1023));

					// ANMMSG_UNKNOWNFIELDACCESSXML : "Error at line %d: Unknown field access type %s"
					char *errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNFIELDACCESSXML) + 
						ANMMAXDECIMALLENGTH +
						strlen(attrbuf) +
						1);

					sprintf(errmsg, ANMMSG_UNKNOWNFIELDACCESSXML, GetLineNumber(), attrbuf);

					WriteToConsole(errmsg);

					delete errmsg;

					return E_FAIL;
				}
			}
			else if (!wcsncmp(nstr, L"type", nlen))
			{
				m_fieldDecl.fieldType = FieldType(val, clen);
				if (m_fieldDecl.fieldType == FIELD_UNKNOWN_FIELD_TYPE)
				{
					bstr2cstr(attrbuf, val, min(clen, 1023));

					// ANMMSG_UNKNOWNFIELDTYPEXML : "Error at line %d: Unknown field type %s"
					char *errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNFIELDTYPEXML) + 
						ANMMAXDECIMALLENGTH +
						strlen(attrbuf) +
						1);

					sprintf(errmsg, ANMMSG_UNKNOWNFIELDTYPEXML, GetLineNumber(), attrbuf);

					WriteToConsole(errmsg);

					delete errmsg;

					return E_FAIL;
				}
			}
		}
	}

	if (m_fieldDecl.fieldName == NULL ||
		m_fieldDecl.accessType == ACC_UNKNOWN_ACCESS_TYPE ||
		m_fieldDecl.fieldType == FIELD_UNKNOWN_FIELD_TYPE)
	{
		// ANMMSG_FIELDDECLMISSINGREQUIRED : "Error: field declaration is missing required values"
		WriteToConsole(ANMMSG_FIELDDECLMISSINGREQUIRED);
		return E_FAIL;
	}

    hr = pAttributes->getValueFromName(L"", 0, L"value", 5, &val, &clen);
	if (SUCCEEDED(hr))
		m_fieldDecl.defaultValue = ParseValue(m_fieldDecl.fieldType, val, clen);

	assert(m_fieldDecl.fieldName != NULL);
	assert(m_fieldDecl.accessType != ACC_UNKNOWN_ACCESS_TYPE);
	assert(m_fieldDecl.fieldType != FIELD_UNKNOWN_FIELD_TYPE);

	m_fieldDecl.gotNode = false;

	return S_OK;
}

HRESULT CAnmXMLSAXReader::EndFieldDeclare()
{
	// Supply null node if it wasn't found (but not for extern protos erf)
	if (m_fieldDecl.fieldType == eValueNode && !m_fieldDecl.gotNode &&
		m_externprotourlstack.empty())
		m_loader->NullNode();

	if (m_fieldDecl.accessType == eAnmMethodMember)
		m_loader->AddEventIn(m_fieldDecl.fieldType, m_fieldDecl.fieldName);
	else if (m_fieldDecl.accessType == eAnmCallbackMember)
		m_loader->AddEventOut(m_fieldDecl.fieldType, m_fieldDecl.fieldName);
	else
		m_loader->AddField(m_fieldDecl.fieldType, m_fieldDecl.fieldName, m_fieldDecl.defaultValue);

	SafeUnRef(m_fieldDecl.fieldName);
	m_fieldDecl.accessType = ACC_UNKNOWN_ACCESS_TYPE;
	m_fieldDecl.fieldType = FIELD_UNKNOWN_FIELD_TYPE;
	m_fieldDecl.defaultValue = NULL;
	m_fieldDecl.gotNode = false;

	return S_OK;
}

HRESULT CAnmXMLSAXReader::BeginFieldValue(ISAXAttributes __RPC_FAR *pAttributes)
{
	HRESULT hr;
	int clen;
	wchar_t *val;

	sAnmFieldValue fieldval;

    hr = pAttributes->getValueFromName(L"", 0, L"name", 4, &val, &clen);
	if (SUCCEEDED(hr))
	{
		fieldval.fieldName = AnmAllocString(clen + 1);
		bstr2cstr(fieldval.fieldName, val, clen);

		CAnmAttributeSymbol *pAttrSym = m_loader->LookupAttribute(fieldval.fieldName);
		if (pAttrSym == NULL)
		{
			// ANMMSG_UNKNOWNFIELDNAMEXML : "Unknown field name %s"
			char *errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNFIELDNAMEXML) + 
				strlen(fieldval.fieldName) +
				1);

			sprintf(errmsg, ANMMSG_UNKNOWNFIELDNAMEXML, fieldval.fieldName);

			WriteToConsole(errmsg);

			delete errmsg;

			return E_FAIL;
		}

		fieldval.attrSym = pAttrSym;
		CLASSMEMBERID mid = pAttrSym->GetClassMember();

		assert(mid != NULL);

		if (mid->valueType != eValueNode && mid->valueType != eValueNodeArray)
		{
			hr = pAttributes->getValueFromName(L"", 0, L"value", 5, &val, &clen);
			if (SUCCEEDED(hr))
					fieldval.value = ParseValue(mid->valueType, val, clen);
		}
	}
	else
	{
		// ANMMSG_FIELDVALUEREQNAMEXML		"Field value requires a name"
		WriteToConsole(ANMMSG_FIELDVALUEREQNAMEXML);
		return E_FAIL;
	}

	m_fieldvaluestack.push(fieldval);
	return S_OK;
}


HRESULT CAnmXMLSAXReader::EndFieldValue()
{
	assert(!m_fieldvaluestack.empty());
	sAnmFieldValue fieldval =
		m_fieldvaluestack.top();

	m_fieldvaluestack.pop();

	assert(fieldval.fieldName != NULL);
	assert(fieldval.attrSym != NULL);

	CLASSMEMBERID mid = fieldval.attrSym->GetClassMember();
	if (mid->valueType != eValueNode && mid->valueType != eValueNodeArray)
		m_loader->SetValue(fieldval.attrSym, fieldval.value);
	
	return S_OK;
}

HRESULT CAnmXMLSAXReader::ParseRoute(ISAXAttributes __RPC_FAR *pAttributes)
{
	HRESULT hr;
	String fromnode = NULL, tonode = NULL, fromfield = NULL, tofield = NULL;
	int len, clen;

	wchar_t *val;

	hr = pAttributes->getLength(&len);

	for (int i = 0; i < len; i++)
	{
		hr = pAttributes->getValue(i, &val, &clen);
		if (SUCCEEDED(hr))
		{
			BSTR uristr, nstr, qbstr;
			int urilen, nlen, qblen;

			hr = pAttributes->getName(i, &uristr, &urilen, &nstr, &nlen, &qbstr, &qblen);

			// search for the attribute name in the class symbol's list
			if (!wcsncmp(nstr, L"fromNode", nlen))
			{
				fromnode = new CAnmString();
				clen = wcslen(val);
				fromnode->alloc(clen + 1);
				bstr2cstr(fromnode->GetBuf(), val, clen);
			}
			else if (!wcsncmp(nstr, L"fromField", nlen))
			{
				fromfield = new CAnmString();
				clen = wcslen(val);
				fromfield->alloc(clen + 1);
				bstr2cstr(fromfield->GetBuf(), val, clen);
			}
			else if (!wcsncmp(nstr, L"toNode", nlen))
			{
				tonode = new CAnmString();
				clen = wcslen(val);
				tonode->alloc(clen + 1);
				bstr2cstr(tonode->GetBuf(), val, clen);
			}
			else if (!wcsncmp(nstr, L"toField", nlen))
			{
				tofield = new CAnmString();
				clen = wcslen(val);
				tofield->alloc(clen + 1);
				bstr2cstr(tofield->GetBuf(), val, clen);
			}
		}
	}

	m_loader->MakeRoute(fromnode, fromfield, tonode, tofield);
	SafeUnRef(fromnode);
	SafeUnRef(fromfield);
	SafeUnRef(tonode);
	SafeUnRef(tofield);

	return S_OK;
}

void *CAnmXMLSAXReader::ParseValue(eValueType valueType, BSTR bstr, int blen)
{
	BSTR outp;
	char attrvalue[ANMMAXXMLSTRBUF];
	void *pData = NULL;

	switch (valueType)
	{
		case eValueBoolean :
			m_boolval = false;
			bstr2cstr(attrvalue, bstr, blen);
			if (!strcmp(attrvalue, "TRUE")
				|| !strcmp(attrvalue, "True")
				|| !strcmp(attrvalue, "true"))
				m_boolval = true;

			pData = (void *) &m_boolval;

			break;

		case eValueBooleanArray :
			ParseBooleanArray(bstr, blen);
			pData = (void *) &m_booleanarray;
			break;

		case eValueColor :
			m_colorval.r = (float) wcstod(bstr, &outp);
			m_colorval.g = (float) wcstod(outp, &outp);
			m_colorval.b = (float) wcstod(outp, &outp);
			pData = (void *) &m_colorval;
			break;

		case eValueColorArray :
			ParseColorArray(bstr, blen);
			pData = (void *) &m_colorarray;
			break;

		case eValueDouble :
			m_doubleval = wcstod(bstr, &outp);
			pData = (void *) &m_doubleval;
			break;

		case eValueDoubleArray :
			ParseDoubleArray(bstr, blen);
			pData = (void *) &m_doublearray;
			break;

		case eValueInteger :
			m_intval = _wtol(bstr);
			pData = (void *) &m_intval;
			break;

		case eValueImage :
			ParseImage(bstr, blen);
			pData = (void *) &m_image;
			break;

		case eValueIntegerArray :
			ParseIntArray(bstr, blen);
			pData = (void *) &m_intarray;
			break;

		case eValueFloat :
			m_doubleval = wcstod(bstr, &outp);
			m_floatval = (Float) m_doubleval;
			pData = (void *) &m_floatval;
			break;

		case eValueFloatArray :
			ParseFloatArray(bstr, blen);
			pData = (void *) &m_floatarray;
			break;

		case eValuePoint2 :
			m_p2val.x = (float) wcstod(bstr, &outp);
			m_p2val.y = (float) wcstod(outp, &outp);
			pData = (void *) &m_p2val;
			break;

		case eValuePoint2Array :
			ParsePoint2Array(bstr, blen);
			pData = (void *) &m_point2array;
			break;

		case eValuePoint3 :
			m_p3val.x = (float) wcstod(bstr, &outp);
			m_p3val.y = (float) wcstod(outp, &outp);
			m_p3val.z = (float) wcstod(outp, &outp);
			pData = (void *) &m_p3val;
			break;

		case eValuePoint3Array :
			ParsePoint3Array(bstr, blen);
			pData = (void *) &m_point3array;
			break;

		case eValueRotation :
			m_rotval.vec.x = (float) wcstod(bstr, &outp);
			m_rotval.vec.y = (float) wcstod(outp, &outp);
			m_rotval.vec.z = (float) wcstod(outp, &outp);
			m_rotval.theta = (float) wcstod(outp, &outp);
			pData = (void *) &m_rotval;
			break;

		case eValueRotationArray :
			ParseRotationArray(bstr, blen);
			pData = (void *) &m_rotationarray;
			break;

		case eValueString :
			SafeUnRef(m_strval);
			m_strval = new CAnmString();
			m_strval->alloc(blen + 1);
			bstr2cstr(m_strval->GetBuf(), bstr, blen);
			pData = (void *) &m_strval;
			break;

		case eValueStringArray :
			ParseStringArray(bstr, blen);
			pData = (void *) &m_stringarray;
			break;

		case eValueTime :
			m_doubleval = wcstod(bstr, &outp);
			pData = (void *) &m_doubleval;
			break;

		case eValueNode :
			// N.B.: Just ignore this; if it's a NULL inside a
			// field declaration, then the defaultValue handling
			// will take care of it (see endFieldDeclare())
			break;

		default :
			assert (FALSE);
	}

	return pData;

}

CAnmClassSymbol *CAnmXMLSAXReader::ParseAttributes(char *nodename,
												   BSTR pwchUri,
												   int cchUri,
									ISAXAttributes __RPC_FAR *pAttributes,
									CAnmClassSymbol *pParentClassSym,
									CAnmAttributeSymbol **ppContainerAttrSym,
									bool isProtoInstance)
{
	HRESULT hr;
	BSTR bstr;
	// N.B.: length
	char attrname[ANMMAXSTRBUF];
	String attrvalue;
	char containerfield[ANMMAXSTRBUF];
	int isgroup = FALSE;
	int len, blen;
	int i;
	CAnmClassSymbol *pClassSym = NULL;
	CAnmClassSymbol *pClassSymForUse = NULL;
	void *pData;

	m_gotUse = false;

	// Try USE first
    hr = pAttributes->getValueFromName(pwchUri, cchUri, L"USE", 3, &bstr, &blen);

	if (SUCCEEDED(hr) && bstr && blen)
	{
		// we're USE-ing an existing node
		m_gotUse = true;

		attrvalue = new CAnmString;
		attrvalue->alloc(blen + 1);
		bstr2cstr(attrvalue->GetBuf(), bstr, blen);
		pClassSymForUse = m_loader->UseNode(attrvalue);
		attrvalue->UnRef();
	}
	else
	{
		// Set up DEF name first if there is one
		hr = pAttributes->getValueFromName(pwchUri, cchUri, L"DEF", 3, &bstr, &blen);

		if (SUCCEEDED(hr) && bstr && blen)
		{
			attrvalue = new CAnmString;
			attrvalue->alloc(blen + 1);
			bstr2cstr(attrvalue->GetBuf(), bstr, blen);
			m_loader->DefNode(attrvalue);
			attrvalue->UnRef();
		}

		// Now create the new node
		// ignore special XML tags
		// look for scripts, shaders or protos here
		if (!strcmp(nodename, "Script"))
		{
			pClassSym = m_loader->BeginMakeScript();
			m_inScript = TRUE;

			// N.B.: do this now, because we need to special-case if there's
			// a CDATA in-line script it will append that value to whatever's
			// in our string array; we don't want it to be the old value
			SafeUnRef(m_stringarray);
		}
		else if (!strcmp(nodename, "ShaderProgram"))
		{
			pClassSym = m_loader->BeginMakeShader();
			m_inShader = TRUE;

			// N.B.: do this now, because we need to special-case if there's
			// a CDATA in-line shader program it will append that value to whatever's
			// in our string array; we don't want it to be the old value
			SafeUnRef(m_stringarray);
		}
		else
		{
			String s = new CAnmString(nodename);
			pClassSym = m_loader->BeginMakeNode(s);
			s->UnRef();
		}
	}

	if ((pClassSym || pClassSymForUse) && pParentClassSym && ppContainerAttrSym)
	{
		// Try to find the crazy containerField attribute
	    hr = pAttributes->getValueFromName(pwchUri, cchUri, L"containerField", 
			wcslen(L"containerField"), &bstr, &blen);

		if( SUCCEEDED(hr) && bstr && blen)
		{
			bstr2cstr(containerfield, bstr, blen);
		}
		else if (pClassSym)
		{
			String s = pClassSym->GetDefaultContainerField();
			strcpy(containerfield, s->GetBuf());		// the default
		}
		else if (pClassSymForUse)
		{
			String s = pClassSymForUse->GetDefaultContainerField();
			strcpy(containerfield, s->GetBuf());		// the default
		}
		else
			strcpy(containerfield, "children");		// the default

		*ppContainerAttrSym = pParentClassSym->FindAttribute(containerfield,
			(eAnmClassMemberType) (eAnmValueMember | eAnmInitMember));
		if (!*ppContainerAttrSym)
		{
			// one last try: containing field or fieldValue
			if (!m_fieldvaluestack.empty())
			{
				sAnmFieldValue fieldval =
					m_fieldvaluestack.top();

				assert(fieldval.fieldName != NULL);

				*ppContainerAttrSym = pParentClassSym->FindAttribute(fieldval.fieldName,
					(eAnmClassMemberType) (eAnmValueMember | eAnmInitMember));
			}
			else if (m_fieldDecl.fieldType != FIELD_UNKNOWN_FIELD_TYPE)
			{
				assert(m_fieldDecl.fieldName != NULL);

				char enclosingContainerfield[ANMMAXSTRBUF];
				// N.B.: length
				strcpy(enclosingContainerfield, m_fieldDecl.fieldName->GetBuf());

				*ppContainerAttrSym = pParentClassSym->FindAttribute(enclosingContainerfield,
					(eAnmClassMemberType) (eAnmValueMember | eAnmInitMember));
			}

			if (!*ppContainerAttrSym)
			{
				// ANMMSG_UNKNOWNCONTAINERFIELD : "Warning: unknown container field: %s at line %d"
				char *errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNCONTAINERFIELD) + 
					strlen(containerfield) +
					ANMMAXDECIMALLENGTH +
					1);

				sprintf(errmsg, ANMMSG_UNKNOWNCONTAINERFIELD, containerfield, GetLineNumber());
				WriteToConsole(errmsg);

				delete errmsg;
			}
		}
	}

	// if we got a USE, ignore all the other attributes
	if (!m_gotUse)
	{
		hr = pAttributes->getLength(&len);

		for (i = 0; i < len; i++)
		{
			hr = pAttributes->getValue(i, &bstr, &blen);
			if (SUCCEEDED(hr))
			{
				// search for the attribute name in the class symbol's list

				BSTR uristr, nstr, qbstr;
				int urilen, nlen, qblen;

				hr = pAttributes->getName(i, &uristr, &urilen, &nstr, &nlen, &qbstr, &qblen);

				if (SUCCEEDED(hr) && nstr && nlen)
				{
					bstr2cstr(attrname, nstr, nlen);
				}
				else
					attrname[0] = '\0';

				// special case DEF/USE
				if (!strcmp(attrname, "DEF"))
				{
					// already done cf. above
				}
				else if (!strcmp(attrname, "USE"))
				{
					// N.B. we shouldn't be here, should be handled above
					// This means ignore in the case of an empty USE value -- TP 04/16/05
				}
				else if (!strcmp(attrname, "containerField"))
				{
					// already done cf. above
				}
				else
				{
					// it's a real field
					CAnmAttributeSymbol *pAttrSymbol =
						m_loader->LookupAttribute(attrname);

					if (pAttrSymbol)
					{

						CLASSMEMBERID mid = pAttrSymbol->GetClassMember();
						
						if (mid->valueType != eValueNode && mid->valueType != eValueNodeArray)
						{
							pData = ParseValue(mid->valueType, bstr, blen);
							m_loader->SetValue(pAttrSymbol, pData);
						}
					}
					else if (!(isProtoInstance && !strcmp(attrname, "name")))
					{
						// ANMMSG_UNKNOWNFIELD : "Warning: unknown field '%s' at line %d; skipping"
						char *errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNFIELD) + 
							strlen(attrname) +
							ANMMAXDECIMALLENGTH +
							1);

						sprintf(errmsg, ANMMSG_UNKNOWNFIELD, attrname, GetLineNumber());

						WriteToConsole(errmsg);

						delete errmsg;
					}
				}
			}

		}

	}
	
	return pClassSym;
}

HRESULT CAnmXMLSAXReader::BeginMakeNode(char *nodename,
												   BSTR pwchUri,
												   int cchUri,
									ISAXAttributes __RPC_FAR *pAttributes,
									bool isProtoInstance)
{
	CAnmAttributeSymbol *pContainerAttrSym = NULL;

	sAnmClassAndContainers curcc = CurrentClass();
	CAnmClassSymbol *pCurClass = curcc.pClassSymbol;

	CAnmClassSymbol *pClassSym = ParseAttributes(nodename, pwchUri, cchUri, pAttributes,
									pCurClass, &pContainerAttrSym, isProtoInstance);

	if (pCurClass && pContainerAttrSym == NULL)
	{
		// N.B.: actually we might be skipping an unknown or NIY node-- TP 3/31/03
		// sprintf(errbuf, "No valid containerField for element %s.", nodename);
		// WriteToConsole(errbuf);
		// return E_FAIL;
	}

	sAnmClassAndContainers newcc;
	newcc.pClassSymbol = pClassSym;
	newcc.pParentContainerField = pContainerAttrSym;
	PushClass(newcc);

	return S_OK;
}

HRESULT CAnmXMLSAXReader::EndMakeNode(char *nodename, bool isProto)
{
	// Special-case end of script declaration
	bool gotScript = false;
	if (m_inScript)
	{
		if (!strcmp(nodename, "Script") && !m_gotUse)
		{
			m_inScript = false;
			gotScript = true;
		}
	}

	// Special-case end of script declaration
	bool gotShader = false;
	if (m_inShader)
	{
		if (!strcmp(nodename, "ShaderProgram") && !m_gotUse)
		{
			m_inShader = false;
			gotShader = true;
		}
	}

	// Get current class and do finalizing work
	sAnmClassAndContainers curcc = CurrentClass();
	CAnmClassSymbol *pCurClass = curcc.pClassSymbol;
	CAnmAttributeSymbol *pContainerAttrSym = curcc.pParentContainerField;

	if (pCurClass)
	{
		// Need to match up type names with tags
		const char *classname = pCurClass->GetName()->GetBuf();

		// N.B.: we want to be able to match tags for user-defined
		// Script classes against just the
		// string 'Script'; however the type system creates class names
		// like 'Script_0' etc. for unique script instances since the user
		// only supplies the string Script. Get it?
		if (!gotScript && !gotShader && !isProto && strcmp(classname, nodename))
		{
			// ANMMSG_UNEXPECTEDENDTAGXML		"Unexpected end tag (%s); expected to match %s."
			char *errmsg = (char *) malloc(strlen(ANMMSG_UNEXPECTEDENDTAGXML) + 
				strlen(nodename) +
				strlen(classname) +
				1);

			sprintf(errmsg, ANMMSG_UNEXPECTEDENDTAGXML, nodename, classname);
			WriteToConsole(errmsg);

			delete errmsg;

			return E_FAIL;

		}

		// Generate code to finish the node
		if (gotScript)
			m_loader->EndMakeScript();
		else if (gotShader)
			m_loader->EndMakeShader();
		else
			m_loader->EndMakeNode();
	}
	else if (!m_gotUse)
	{
		// deal with skipped unknown node types
		m_loader->EndMakeNode();		
	}

	PopClass();

	// See if nutty container field needs to be set in parent
	// Get parent class info
	curcc = CurrentClass();

	// If we're not at the top of the class stack, we should have a
	// container symbol to set the child's value into in our node
	// N.B.: actually we might be skipping an unknown or NIY node-- TP 3/31/03
	if (curcc.pClassSymbol && pContainerAttrSym)
	{
		// assert(pContainerAttrSym != NULL);

		if (pContainerAttrSym->GetClassMember()->valueType == eValueNode)
		{
			// Force code generation of set on SFNode fields; this is the time to do it
			m_loader->SetValue(pContainerAttrSym, NULL);
		}
		else if (pContainerAttrSym->GetClassMember()->valueType == eValueNodeArray)
		{
			// Force code generation of node append on MFNode fields; this is the time to do it
			m_loader->AppendNode(pContainerAttrSym);
		}
	}

	// Replace top of stack with new info
	if (!m_classstack.empty())
	{
		PopClass();
		PushClass(curcc);
	}

	m_gotUse = false;
	m_fieldDecl.gotNode = true;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnmXMLSAXReader::startElement( 
            /* [in] */ wchar_t __RPC_FAR *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ wchar_t __RPC_FAR *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ wchar_t __RPC_FAR *pwchRawName,
            /* [in] */ int cchRawName,
            /* [in] */ ISAXAttributes __RPC_FAR *pAttributes)
{
	HRESULT hr = S_OK;
	char namebuf[ANMMAXXMLSTRBUF];

	// Copy BSTR to cstr
	wcstombs(namebuf, pwchLocalName, wcslen(pwchLocalName) + 1);

	// If we are in an include, ignore head, meta, scene, and root doc type tags
	if (m_includeLevel)
	{
		if (!strcmp(namebuf, ANMXMLHEADTAGNAME)
		||  !strcmp(namebuf, ANMXMLMETATAGNAME)
		||  !strcmp(namebuf, ANMXMLSCENETAGNAME)
		||  !strcmp(namebuf, ANMXMLDOCTYPENAME)
		||  !strcmp(namebuf, ANMXMLANIMADOCTYPENAME))

		return S_OK;
	}

	// First, look for X3D doc type
	if (!m_inX3D)
	{
		if (!strcmp(namebuf, ANMXMLDOCTYPENAME)
			|| !strcmp(namebuf, ANMXMLANIMADOCTYPENAME))
		{
			m_inX3D = true;
			return S_OK;
		}

		// Don't know what it is...
		return S_OK;
	}

	// Then, look for header
	if (!m_inHead && !m_headerDone)
	{
		if (!strcmp(namebuf, ANMXMLHEADTAGNAME))
		{
			m_inHead = true;
			return S_OK;
		}

		// didn't get a header, won't parse one later
		m_headerDone = true;
	}

	// Add meta tags to scene info
	if (m_inHead)
	{
		if (!strcmp(namebuf, ANMXMLMETATAGNAME))
		{
			BSTR namestr, contentstr;
			int namelen, contentlen;

			HRESULT mhr = 
				pAttributes->getValueFromName(pwchNamespaceUri, cchNamespaceUri,
					L"name", 4, &namestr, &namelen);

			if (SUCCEEDED(mhr) && namestr && namelen)
			{
				mhr = 
					pAttributes->getValueFromName(pwchNamespaceUri, cchNamespaceUri,
						L"content", 7, &contentstr, &contentlen);

				if (SUCCEEDED(mhr) && contentstr && contentlen)
				{
					String cpname = new CAnmString;
					cpname->alloc(namelen + 1);
					bstr2cstr(cpname->GetBuf(), namestr, namelen);
					
					String cpcontent = new CAnmString;
					cpcontent->alloc(contentlen + 1);
					bstr2cstr(cpcontent->GetBuf(), contentstr, contentlen);

					m_loader->AddMetaData(cpname, cpcontent);

					cpname->UnRef();
					cpcontent->UnRef();
				}
			}
		}

		return S_OK;
	}


	// Now look for root scene tag
	if (!m_inScene)
	{
		if (!strcmp(namebuf, ANMXMLSCENETAGNAME))
		{
			m_inScene = true;
			return S_OK;
		}
		else
		{
			// report unknown tag
			// ANMMSG_UNKNOWNSTARTTAGXML : "Unknown tag (%s); expected %s tag to start scene."
			char *errmsg = (char *) malloc(strlen(ANMMSG_UNKNOWNSTARTTAGXML) + 
				strlen(namebuf) +
				strlen(ANMXMLSCENETAGNAME) +
				1);

			sprintf(errmsg, ANMMSG_UNKNOWNSTARTTAGXML, namebuf, ANMXMLSCENETAGNAME);
			WriteToConsole(errmsg);

			delete errmsg;

			return E_FAIL;
		}
	}

	// Look for PROFILE and COMPONENT declarations
	if (!strcmp(namebuf, "component"))
	{
		m_inComponent = true;
		return S_OK;
	}

	// Look for special tags like ROUTE, IMPORT, EXPORT
	if (!strcmp(namebuf, "ROUTE"))
	{
		return ParseRoute(pAttributes);
	}

	if (!strcmp(namebuf, "IMPORT"))
	{
		m_inImport = true;
		return S_OK;
	}

	if (!strcmp(namebuf, "EXPORT"))
	{
		m_inExport = true;
		return S_OK;
	}

	if (!strcmp(namebuf, "INCLUDE"))
	{
		m_includeLevel++;

		BSTR filenamestr;
		int filenamelen;

		HRESULT mhr = 
			pAttributes->getValueFromName(pwchNamespaceUri, cchNamespaceUri,
				L"filename", 8, &filenamestr, &filenamelen);


		if (SUCCEEDED(mhr) && filenamestr && filenamelen)
		{
			String cpname = new CAnmString;
			cpname->alloc(filenamelen + 1);
			bstr2cstr(cpname->GetBuf(), filenamestr, filenamelen);
			
			Read(cpname->GetBuf(), true);

			cpname->UnRef();
		}

		return S_OK;
	}

	// Look for PROTO declarations
	bool protoDeclare = false;
	bool externProto = false;

	if (!strcmp(namebuf, "ProtoDeclare"))
		protoDeclare = true;
	else if (!strcmp(namebuf, "ExternProtoDeclare"))
	{
		protoDeclare = true;
		externProto = true;
		m_inProtoInterface = true;
	}

	if (protoDeclare)
	{
		return BeginProtoDeclare(pAttributes, externProto);
	}

	if (!strcmp(namebuf, "ProtoInstance"))
	{
		return BeginProtoInstance(pwchNamespaceUri, cchNamespaceUri, pAttributes);
	}

	if (!strcmp(namebuf, "ProtoInterface"))
	{
		m_inProtoInterface = true;
		return S_OK;
	}

	if (!strcmp(namebuf, "ProtoBody"))
	{
		// N.B.: purely structural; anything we should be validating here?
		return S_OK;
	}

	if (!strcmp(namebuf, "field"))
	{
		return BeginFieldDeclare(pAttributes);
	}

	if (!strcmp(namebuf, "fieldValue"))
	{
		return BeginFieldValue(pAttributes);
	}

	if (!strcmp(namebuf, "IS"))
	{
		m_inIS = true;
		return S_OK;
	}

	if (!strcmp(namebuf, "connect"))
	{
		if (!m_inIS)
		{
			// ANMMSG_CONNECTTAGREQUIRESIS : "Connect tags must appear within an IS element"
			WriteToConsole(ANMMSG_CONNECTTAGREQUIRESIS);
			return E_FAIL;
		}

		return ParseConnect(pwchNamespaceUri, cchNamespaceUri, pAttributes);
	}

	// If we're this far, it's a node type
	
	// If we got a USE, flag the error
	if (m_gotUse)
	{
		// report bad content
		// ANMMSG_CHILDELEMENTSUNDERUSE : "Warning: child elements under a USE"
		WriteToConsole(ANMMSG_CHILDELEMENTSUNDERUSE);
		return E_FAIL;
	}

	return BeginMakeNode(namebuf, pwchNamespaceUri, cchNamespaceUri, pAttributes);
}
        
       
HRESULT STDMETHODCALLTYPE CAnmXMLSAXReader::endElement( 
            /* [in] */ wchar_t __RPC_FAR *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ wchar_t __RPC_FAR *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ wchar_t __RPC_FAR *pwchRawName,
            /* [in] */ int cchRawName)
{
	HRESULT hr = S_OK;
	char namebuf[ANMMAXXMLSTRBUF];

	// Copy BSTR to cstr
	wcstombs(namebuf, pwchLocalName, wcslen(pwchLocalName) + 1);

	// Deal with include stuff first
	if (!strcmp(namebuf, "INCLUDE"))
	{
		--m_includeLevel;
		return S_OK;
	}

	// If we are in an include, ignore head, meta, scene, and root doc type tags
	if (m_includeLevel)
	{
		if (!strcmp(namebuf, ANMXMLHEADTAGNAME)
		||  !strcmp(namebuf, ANMXMLMETATAGNAME)
		||  !strcmp(namebuf, ANMXMLSCENETAGNAME)
		||  !strcmp(namebuf, ANMXMLDOCTYPENAME)
		||  !strcmp(namebuf, ANMXMLANIMADOCTYPENAME))

		return S_OK;
	}

 	// Try to match end of header
	if (m_inHead)
	{
		if (!strcmp(namebuf, ANMXMLHEADTAGNAME))
		{
			m_inHead = false;
			m_headerDone = true;
			return S_OK;
		}

		// meta tags are handled in-line in startElement
		return S_OK;
	}

	// Try to match end of scene
	if (m_inScene)
	{
		if (!strcmp(namebuf, ANMXMLSCENETAGNAME))
		{
			m_inScene = false;
			return S_OK;
		}
	}

	// Try to match end of doc
	if (m_inX3D)
	{
		if (!strcmp(namebuf, ANMXMLDOCTYPENAME)
			|| !strcmp(namebuf, ANMXMLANIMADOCTYPENAME))
		{
			m_inX3D = false;
			return S_OK;
		}
	}

	// Look for PROFILE and COMPONENT declarations
	if (!strcmp(namebuf, "component"))
	{
		m_inComponent = false;
		return S_OK;
	}

	// Look for special tags like ROUTE, IMPORT, EXPORT
	if (!strcmp(namebuf, "ROUTE"))
	{
		// just skip, it's already processed
		return S_OK;
	}

	if (!strcmp(namebuf, "IMPORT"))
	{
		m_inImport = false;
		return S_OK;
	}

	if (!strcmp(namebuf, "EXPORT"))
	{
		m_inExport = false;
		return S_OK;
	}

	// Look for end of PROTO declarations
	bool protoDeclare = false;
	bool externProto = false;

	if (!strcmp(namebuf, "ProtoDeclare"))
		protoDeclare = true;
	else if (!strcmp(namebuf, "ExternProtoDeclare"))
	{
		protoDeclare = true;
		externProto = true;
		m_inProtoInterface = false;
	}

	if (protoDeclare)
	{
		return EndProtoDeclare(externProto);
	}

	if (!strcmp(namebuf, "ProtoInstance"))
	{
		return EndProtoInstance();
	}

	if (!strcmp(namebuf, "ProtoInterface"))
	{
		m_inProtoInterface = false;
		return S_OK;
	}

	if (!strcmp(namebuf, "ProtoBody"))
	{
		// N.B.: purely structural; anything we should be validating here?
		return S_OK;
	}

	if (!strcmp(namebuf, "field"))
	{
		return EndFieldDeclare();
	}

	if (!strcmp(namebuf, "fieldValue"))
	{
		return EndFieldValue();
	}

	if (!strcmp(namebuf, "IS"))
	{
		m_inIS = false;
		return S_OK;
	}

	if (!strcmp(namebuf, "connect"))
	{
		// already handled in startElement
		return S_OK;
	}

	// If we're this far, it's a node type
	return EndMakeNode(namebuf);
}

HRESULT STDMETHODCALLTYPE CAnmXMLSAXReader::startDocument()
{
    // printf("<?xml version=\"1.0\" ?>");
    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE CAnmXMLSAXReader::putDocumentLocator( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator
            )
{
	m_locator = pLocator;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnmXMLSAXReader::characters( 
            /* [in] */ wchar_t __RPC_FAR *pwchChars,
            /* [in] */ int cchChars)
{
    if (m_inCDATA && pwchChars != NULL && cchChars > 0)
		BufferCData(pwchChars, cchChars);

	return S_OK;
}

void CAnmXMLSAXReader::BufferCData(wchar_t __RPC_FAR *pwchChars, int cchChars)
{
	// Expand buffer if needed
	if (m_cdatacharcount + cchChars > m_cdatabufsize)
	{
		// Do it in multiples of our preferred allocation size
		int reallocamount = ANMCDATABUFALLOC;
		while (cchChars > reallocamount)
			reallocamount += ANMCDATABUFALLOC;

		// N.B.: do we need to test for NULL return?
		m_cdatabufsize += reallocamount;
		m_cdatabuf = (wchar_t *) realloc(m_cdatabuf, m_cdatabufsize * sizeof(wchar_t));
	}

	// Copy chars
	wchar_t *cp = m_cdatabuf + m_cdatacharcount;
	wcsncpy(cp, pwchChars, cchChars);

	// Update real character count
	m_cdatacharcount += cchChars;
}

HRESULT STDMETHODCALLTYPE CAnmXMLSAXReader::startCDATA ( )
{
	m_inCDATA = true;
	m_cdatabuf = new wchar_t[ANMCDATABUFALLOC];
	m_cdatabufsize = ANMCDATABUFALLOC;
	m_cdatacharcount = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnmXMLSAXReader::endCDATA ( )
{
	if (m_inScript || m_inShader )
	{
		// N.B.: Assume CDATA within a script belongs in url field
		CAnmAttributeSymbol *pAttrSymbol =
			m_loader->LookupAttribute("url");

		assert (pAttrSymbol);

		CLASSMEMBERID mid = pAttrSymbol->GetClassMember();
			
		// void *pData = ParseValue(mid->valueType, m_cdatabuf, m_cdatacharcount);

		// Add the CDATA string as the last element of the accumulated
		// string array. N.B.: it was NULLed out before any attributes
		// of the Script were parsed, so it should only have clean
		// values as supplied in the url attribute of the Script element
		String s = new CAnmString;
		s->alloc(m_cdatacharcount + 1);
		bstr2cstr(s->GetBuf(), m_cdatabuf, m_cdatacharcount);

		// N.B.: Dude this is squirrely; check all the cases around m_stringarray
		// krv:  DONT do a SafeDelete!!!
		SafeUnRef(m_stringarray);

		m_stringarray = new StringArray;
		m_stringarray->Ref();

		m_stringarray->push_back(s);

		m_loader->SetValue(pAttrSymbol, (void *) &m_stringarray);
	}

	m_inCDATA = false;
	SafeDelete(m_cdatabuf);

	return S_OK;
} 

HRESULT STDMETHODCALLTYPE CAnmXMLSAXReader::error(
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode )
{
	char msg[ANMMAXXMLSTRBUF];

	int len = wcslen(pwchErrorMessage);

	if (len >= ANMMAXXMLSTRBUF)
		len = ANMMAXXMLSTRBUF - 1;

	bstr2cstr(msg, pwchErrorMessage, len);

	int lineno = 0;
	HRESULT hr = pLocator->getLineNumber(&lineno);

	// ANMMSG_XMLPARSEERROR : "XML parse error at line %d: %s"

	char *errmsg = (char *) malloc(strlen(ANMMSG_XMLPARSEERROR) + 
		ANMMAXDECIMALLENGTH +
		strlen(msg) +
		1);

	sprintf(errmsg, ANMMSG_XMLPARSEERROR, lineno, msg);

	WriteToConsole(errmsg);

	delete errmsg;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnmXMLSAXReader::fatalError(
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode )
{
	char msg[ANMMAXXMLSTRBUF];

	int len = wcslen(pwchErrorMessage);

	if (len >= ANMMAXXMLSTRBUF)
		len = ANMMAXXMLSTRBUF - 1;

	bstr2cstr(msg, pwchErrorMessage, len);

	int lineno = 0;
	HRESULT hr = pLocator->getLineNumber(&lineno);

	// ANMMSG_XMLPARSEERROR : "XML parse error at line %d: %s"

	char *errmsg = (char *) malloc(strlen(ANMMSG_XMLPARSEERROR) + 
		ANMMAXDECIMALLENGTH +
		strlen(msg) +
		1);

	sprintf(errmsg, ANMMSG_XMLPARSEERROR, lineno, msg);

	WriteToConsole(errmsg);

	delete errmsg;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAnmXMLSAXReader::ignorableWarning(
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode )
{
	char msg[ANMMAXXMLSTRBUF];

	int len = wcslen(pwchErrorMessage);

	if (len >= ANMMAXXMLSTRBUF)
		len = ANMMAXXMLSTRBUF - 1;

	bstr2cstr(msg, pwchErrorMessage, len);

	int lineno = 0;
	HRESULT hr = pLocator->getLineNumber(&lineno);

	// ANMMSG_XMLPARSEWARNING : "XML parse warning at line %d: %s"
	char *errmsg = (char *) malloc(strlen(ANMMSG_XMLPARSEWARNING) + 
		ANMMAXDECIMALLENGTH +
		strlen(msg) +
		1);

	sprintf(errmsg, ANMMSG_XMLPARSEWARNING, lineno, msg);

	WriteToConsole(errmsg);

	delete errmsg;
	return S_OK;
}