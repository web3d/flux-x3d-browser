/********************************************************************************
 * Flux
 *
 * File: anmxmldomreader.h
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

#ifndef _anmxmldomreader_h
#define _anmxmldomreader_h

#include "anmxmlreader.h"

class CAnmXMLDOMReader : public CAnmXMLReader
{
protected :

	IXMLDOMDocumentPtr m_pXMLDoc;


	class CAnmClassSymbol *ParseAttributes(IXMLDOMNodePtr pXMLNode,
		class CAnmClassSymbol *pParentClassSym, class CAnmAttributeSymbol **ppContainerAttrSym,
		BOOL *pGotScript);
	void WalkTree(IXMLDOMNodePtr pXMLNode,
		class CAnmClassSymbol *pParentClassSym, class CAnmAttributeSymbol **ppContainerAttrSym);
	void ParseRoute(IXMLDOMNodePtr pXMLNode);

public :

	CAnmXMLDOMReader(class CAnmFileLoader *pLoader);
	virtual ~CAnmXMLDOMReader();

	virtual eAnmReturnStatus Read(const char *source, bool fromFile);

	virtual int GetLineNumber();

};


#endif // _anmxmldomreader_h
