/********************************************************************************
 * Flux
 *
 * File: anmxmldomreader.cpp
 * Description: XML DOM reader class
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
#include "anmxmldomreader.h"
#include "anmexecutioncontext.h"
#include "anmfileloader.h"
#include "anmsymbol.h"
#include "anmnode.h"
#include "anmgroup.h"

CAnmXMLDOMReader::CAnmXMLDOMReader(CAnmFileLoader *pLoader)
: CAnmXMLReader(pLoader)
{
}

CAnmXMLDOMReader::~CAnmXMLDOMReader()
{
}

void CAnmXMLDOMReader::ParseRoute(IXMLDOMNodePtr pXMLNode)
{
	IXMLDOMNamedNodeMapPtr pXnodemap;
	IXMLDOMNodePtr pXattr;
	VARIANT nodeval;
	BSTR bstr;
	HRESULT hr;
	char attrname[1024];
	String fromnode = NULL, tonode = NULL, fromfield = NULL, tofield = NULL;
	long len, slen;

	hr = pXMLNode->get_attributes(&pXnodemap);
	if (pXnodemap)
	{
		hr = pXnodemap->get_length(&len);

		for (int i=0; i<len; i++)
		{
			hr = pXnodemap->get_item(i, &pXattr);
			if (pXattr)
			{
				// search for the attribute name in the class symbol's list

				hr = pXattr->get_nodeName(&bstr);
				wcstombs(attrname, bstr, wcslen(bstr) + 1);

				hr = pXattr->get_nodeTypedValue(&nodeval);

				if (!strcmp(attrname, "fromNode"))
				{
					fromnode = new CAnmString();
					slen = wcslen(nodeval.bstrVal);
					fromnode->alloc(slen + 1);
					bstr2cstr(fromnode->GetBuf(), nodeval.bstrVal, slen);
				}
				else if (!strcmp(attrname, "fromField"))
				{
					fromfield = new CAnmString();
					slen = wcslen(nodeval.bstrVal);
					fromfield->alloc(slen + 1);
					bstr2cstr(fromfield->GetBuf(), nodeval.bstrVal, slen);
				}
				else if (!strcmp(attrname, "toNode"))
				{
					tonode = new CAnmString();
					slen = wcslen(nodeval.bstrVal);
					tonode->alloc(slen + 1);
					bstr2cstr(tonode->GetBuf(), nodeval.bstrVal, slen);
				}
				else if (!strcmp(attrname, "toField"))
				{
					tofield = new CAnmString();
					slen = wcslen(nodeval.bstrVal);
					tofield->alloc(slen + 1);
					bstr2cstr(tofield->GetBuf(), nodeval.bstrVal, slen);
				}
				
				pXattr.Release();
			}
		}

		// now we should have all the required ROUTE names
		m_loader->MakeRoute(fromnode, fromfield, tonode, tofield);
		SafeUnRef(fromnode);
		SafeUnRef(fromfield);
		SafeUnRef(tonode);
		SafeUnRef(tofield);

		pXnodemap.Release();
	}
}


CAnmClassSymbol *CAnmXMLDOMReader::ParseAttributes(IXMLDOMNodePtr pXMLNode,
									CAnmClassSymbol *pParentClassSym,
									CAnmAttributeSymbol **ppContainerAttrSym,
									BOOL *pGotScript)
{
	IXMLDOMNamedNodeMapPtr pXnodemap;
	IXMLDOMNodePtr pXattr;
	VARIANT nodeval;
	HRESULT hr;
	BSTR bstr, outp;
	char nodename[1024], attrname[1024];
	String attrvalue;
	char nodetext[1024]; // for debugging
	char containerfield[1024];
	char boolbuf[1024];
	char errbuf[1024];
	int isgroup = FALSE;
	long len, blen;
	int i;
	CAnmClassSymbol *pClassSym = NULL;
	Point3 p(0,0,0);
	Point2 p2(0,0);
	sAnmRotation rot(0, 1, 0, 0);
	double doubleval = 0.0;
	Float floatval;
	Integer intval;
	Color color(0, 0, 0);
	String strval;
	Boolean boolval;
	BOOL gotuse = FALSE;
	void *pData;

	assert(pGotScript);
	*pGotScript = FALSE;

	hr = pXMLNode->get_nodeName(&bstr);
	bstr2cstr(nodename, bstr, wcslen(bstr));

	pXMLNode->get_text(&bstr);
	bstr2cstr(nodetext, bstr, wcslen(bstr));

	// special case routes
	if (!strcmp(nodename, "ROUTE"))
	{
		ParseRoute(pXMLNode);
		return NULL;
	}
	

	hr = pXMLNode->get_attributes(&pXnodemap);
	
	if (pXnodemap)
	{
        hr = pXnodemap->getNamedItem(L"USE", &pXattr);
		gotuse = FALSE;

        if( SUCCEEDED(hr) && (pXattr != NULL))
        {
			// we're USE-ing an existing node
			gotuse = TRUE;

			hr = pXattr->get_nodeName(&bstr);
			bstr2cstr(attrname, bstr, wcslen(bstr));

			hr = pXattr->get_nodeTypedValue(&nodeval);
			blen = wcslen(nodeval.bstrVal);
			attrvalue = new CAnmString();
			attrvalue->alloc(blen + 1);
			bstr2cstr(attrvalue->GetBuf(), nodeval.bstrVal, blen);
			m_loader->UseNode(attrvalue);
			attrvalue->UnRef();

			pXattr.Release();
		}
		else
		{
			// Set up DEF name first if there is one
			hr = pXnodemap->getNamedItem(L"DEF", &pXattr);
			if( SUCCEEDED(hr) && (pXattr != NULL))
			{
				hr = pXattr->get_nodeName(&bstr);
				bstr2cstr(attrname, bstr, wcslen(bstr));

				hr = pXattr->get_nodeTypedValue(&nodeval);
				blen = wcslen(nodeval.bstrVal);
				attrvalue = new CAnmString();
				attrvalue->alloc(blen + 1);
				bstr2cstr(attrvalue->GetBuf(), nodeval.bstrVal, blen);
				m_loader->DefNode(attrvalue);
				attrvalue->UnRef();

				pXattr.Release();
			}

			// Now create the new node
			// ignore special XML tags
			if (strcmp(nodename, ANMXMLDOCTYPENAME)
				&& strcmp(nodename, ANMXMLANIMADOCTYPENAME)
				&& strcmp(nodename, ANMXMLSCENETAGNAME)
				&& strcmp(nodename, ANMXMLHEADTAGNAME)
				&& strcmp(nodename, ANMXMLMETATAGNAME))
			{
				// look for scripts or protos here
				if (!strcmp(nodename, "Script"))
				{
					m_loader->BeginMakeScript();
					*pGotScript = TRUE;
					pClassSym = NULL;
				}
				else
				{
					String s = new CAnmString(nodename);
					pClassSym = m_loader->BeginMakeNode(s);
					s->UnRef();
				}
			}
			else
				pClassSym = NULL;
		}
	
		if (pParentClassSym && ppContainerAttrSym)
		{
			// Try to find the crazy containerField attribute
			hr = pXnodemap->getNamedItem(L"containerField", &pXattr);
			if( SUCCEEDED(hr) && (pXattr != NULL))
			{
				hr = pXattr->get_nodeName(&bstr);
				wcstombs(attrname, bstr, wcslen(bstr) + 1);

				hr = pXattr->get_nodeTypedValue(&nodeval);
				wcstombs(containerfield, nodeval.bstrVal, wcslen(nodeval.bstrVal) + 1);

				pXattr.Release();
			}
			else
				strcpy(containerfield, "children");		// the default

			*ppContainerAttrSym = pParentClassSym->FindAttribute(containerfield,
				(eAnmClassMemberType) (eAnmValueMember | eAnmInitMember));
			if (!*ppContainerAttrSym)
			{
				sprintf(errbuf, "Unknown container field: %s", containerfield);
				WriteToConsole(errbuf);
			}
		}

		// if we got a USE, ignore all the other attributes
		if (!gotuse)
		{
			hr = pXnodemap->get_length(&len);

			for (i=0; i<len; i++)
			{
				hr = pXnodemap->get_item(i, &pXattr);
				if (pXattr)
				{
					// search for the attribute name in the class symbol's list

					hr = pXattr->get_nodeName(&bstr);
					wcstombs(attrname, bstr, wcslen(bstr) + 1);

					hr = pXattr->get_nodeTypedValue(&nodeval);
					blen = wcslen(nodeval.bstrVal);

					// special case DEF/USE
					if (!strcmp(attrname, "DEF"))
					{
						// already done cf. above
					}
					else if (!strcmp(attrname, "USE"))
					{
						// N.B. we shouldn't be here, should be handled above
						assert(FALSE);
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

							switch (mid->valueType)
							{
							case eValueBoolean :
								bstr2cstr(boolbuf, nodeval.bstrVal, wcslen(nodeval.bstrVal));
								boolval = FALSE;
								if (!strcmp(boolbuf, "TRUE")
									|| !strcmp(boolbuf, "True")
									|| !strcmp(boolbuf, "true"))
									boolval = TRUE;

								pData = (void *) &boolval;

								break;

							case eValueColor :
								color.r = (float) wcstod(nodeval.bstrVal, &outp);
								color.g = (float) wcstod(outp, &outp);
								color.b = (float) wcstod(outp, &outp);
								pData = (void *) &color;
								break;

							case eValueColorArray :
								ParseColorArray(nodeval.bstrVal, wcslen(nodeval.bstrVal));
								pData = (void *) &m_colorarray;
								break;

							case eValueInteger :
								intval = _wtol(nodeval.bstrVal);
								pData = (void *) &intval;
								break;

							case eValueIntegerArray :
								ParseIntArray(nodeval.bstrVal, wcslen(nodeval.bstrVal));
								pData = (void *) &m_intarray;
								break;

							case eValueFloat :
								doubleval = wcstod(nodeval.bstrVal, &outp);
								floatval = (Float) doubleval;
								pData = (void *) &floatval;
								break;

							case eValueFloatArray :
								ParseFloatArray(nodeval.bstrVal, wcslen(nodeval.bstrVal));
								pData = (void *) &m_floatarray;
								break;

							case eValuePoint2 :
								p2.x = (float) wcstod(nodeval.bstrVal, &outp);
								p2.y = (float) wcstod(outp, &outp);
								pData = (void *) &p2;
								break;

							case eValuePoint2Array :
								ParsePoint2Array(nodeval.bstrVal, wcslen(nodeval.bstrVal));
								pData = (void *) &m_point2array;
								break;

							case eValuePoint3 :
								p.x = (float) wcstod(nodeval.bstrVal, &outp);
								p.y = (float) wcstod(outp, &outp);
								p.z = (float) wcstod(outp, &outp);
								pData = (void *) &p;
								break;

							case eValuePoint3Array :
								ParsePoint3Array(nodeval.bstrVal, wcslen(nodeval.bstrVal));
								pData = (void *) &m_point3array;
								break;

							case eValueRotation :
								rot.vec.x = (float) wcstod(nodeval.bstrVal, &outp);
								rot.vec.y = (float) wcstod(outp, &outp);
								rot.vec.z = (float) wcstod(outp, &outp);
								rot.theta = (float) wcstod(outp, &outp);
								pData = (void *) &rot;
								break;

							case eValueRotationArray :
								ParseRotationArray(nodeval.bstrVal, wcslen(nodeval.bstrVal));
								pData = (void *) &m_rotationarray;
								break;

							case eValueString :
								blen = wcslen(nodeval.bstrVal);
								strval = new CAnmString();
								strval->alloc(blen + 1);
								bstr2cstr(strval->GetBuf(), nodeval.bstrVal, blen);
								pData = (void *) &strval;
								break;

							case eValueStringArray :
								ParseStringArray(nodeval.bstrVal, wcslen(nodeval.bstrVal));
								pData = (void *) &m_stringarray;
								break;

							case eValueTime :
								doubleval = wcstod(nodeval.bstrVal, &outp);
								pData = (void *) &doubleval;
								break;

							default :
								assert (FALSE);
							}

							m_loader->SetValue(pAttrSymbol, pData);
							
							if (mid->valueType == eValueString)
								strval->UnRef();
						}
					}

					pXattr.Release();
				}

			}

		}
	

		pXnodemap.Release();

	}

	return pClassSym;
}


void CAnmXMLDOMReader::WalkTree(IXMLDOMNodePtr pXMLNode,
		CAnmClassSymbol *pParentClassSym, CAnmAttributeSymbol **ppContainerAttrSym)
{
	IXMLDOMNodePtr pXDN;
	IXMLDOMNodeListPtr pChildList;
	VARIANT_BOOL haschildren;
	HRESULT hr;
	BSTR bstr;
	char buf[1024];
	long len;
	BOOL gotScript;

	hr = pXMLNode->get_nodeName(&bstr);
	wcstombs(buf, bstr, wcslen(bstr) + 1);

	CAnmClassSymbol *pClassSym = ParseAttributes(pXMLNode, pParentClassSym, ppContainerAttrSym, &gotScript);

	hr = pXMLNode->hasChildNodes(&haschildren);
	hr = pXMLNode->get_childNodes(&pChildList);
	hr = pChildList->get_length(&len);

	CAnmAttributeSymbol *pPrevContainerAttrSym = NULL;

	while (pChildList->nextNode(&pXDN) != S_FALSE)
	{
		CAnmAttributeSymbol *pContainerAttrSym = NULL;

		WalkTree(pXDN, pClassSym, &pContainerAttrSym);

		if (pContainerAttrSym != NULL)
		{
			// force code generation of set's on SFNode fields
			if (pContainerAttrSym->GetClassMember()->valueType == eValueNode)
				m_loader->SetValue(pContainerAttrSym, NULL);
			else if (pContainerAttrSym->GetClassMember()->valueType == eValueNodeArray)
			{
				// force code generation of set's on MFNode fields;
				// collect up set on an MF until they're all done
				if (pPrevContainerAttrSym && (pContainerAttrSym != pPrevContainerAttrSym))
					m_loader->SetValue(pPrevContainerAttrSym, NULL);

				pPrevContainerAttrSym = pContainerAttrSym;
			}
		}

		pXDN.Release();
	}

	if (pPrevContainerAttrSym != NULL)
		m_loader->SetValue(pPrevContainerAttrSym, NULL);

	if (pClassSym != NULL)
		m_loader->EndMakeNode();

	if (gotScript)
		m_loader->EndMakeScript();

	pChildList.Release();
}


eAnmReturnStatus CAnmXMLDOMReader::Read(const char *source, bool fromFile)
{
	IXMLDOMElementPtr pXelt;
	BSTR bstr;
	char buf[1024];
	HRESULT hr;
	CAnmNode *pRetNode = NULL;

	hr = m_pXMLDoc.CreateInstance(("msxml2.domdocument"));


	if(!FAILED(hr)) 
	{
		int len = strlen(source);

		wchar_t * pURL = new wchar_t[len + 1];
		
		mbstowcs( pURL, source, len + 1 );
		wprintf(L"\nParsing document: %s\n", pURL);
		
		variant_t url(pURL);
		VARIANT_BOOL loadresult;

		if (fromFile)
		{
			hr = m_pXMLDoc->load(url, &loadresult);
			if (SUCCEEDED(hr))
				hr = m_pXMLDoc->get_nodeName(&bstr);
		}
		else
		{
			m_pXMLDoc->loadXML(pURL, &loadresult);
			if (SUCCEEDED(hr))
				hr = m_pXMLDoc->get_nodeName(&bstr);
		}

		delete pURL;

		printf("\n Load result: %s.\n\n", (loadresult == VARIANT_TRUE ? "OK" : "FAILED"));

		hr = m_pXMLDoc->get_documentElement(&pXelt);

		if (pXelt)
		{
			hr = pXelt->get_nodeName(&bstr);
			wcstombs(buf, bstr, wcslen(bstr) + 1);
			WalkTree(pXelt, NULL, NULL);
			pXelt.Release();
		}
		else
		{

			IXMLDOMParseErrorPtr errPtr;
			
			HRESULT hr = m_pXMLDoc->get_parseError(&errPtr);
			
			if (SUCCEEDED(hr))
			{
				long linepos = 0;
				BSTR reason;

				errPtr->get_linepos(&linepos);
				errPtr->get_reason(&reason);
				if (reason)
				{
				sprintf(buf, "XML parse error at line %ld:", linepos);
      			WriteToConsole(buf);
				wcstombs(buf, reason, wcslen(reason) + 1);
				buf[wcslen(reason)] = '\0';
      			WriteToConsole(buf);
				}
				else
	      			WriteToConsole("Unknown error parsing XML document.");

			}
			else
			{
      			WriteToConsole("Unknown error parsing XML document.");
			}
		}

		m_pXMLDoc.Release();
	}
	else 
	{
		sprintf(buf, "\nUnknown XML load error: %08X\n\n", hr);
		WriteToConsole(buf);
	}

	WriteToConsole("Done parsing XML.");

	return eAnmReturnAllGood;
}

int CAnmXMLDOMReader::GetLineNumber()
{
	return 0; // for now
}
