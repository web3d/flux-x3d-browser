/********************************************************************************
 * Flux
 *
 * File: anmvrml97lexer.h
 * Description: VRML97 lexer class
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

#ifndef _anmvrml97lexer_h
#define _anmvrml97lexer_h

// N.B.: length
#define STRINGBUFFERSIZE 1024
#define MAXINPUTBUFFER 1024

#define VRML97HEADERLINE				"#VRML V2.0 utf8"
#define X3DVRMLCLASSICHEADERLINE		"#X3D V3.0 utf8"

typedef enum eAnmVrmlReaderType {
	eAnmVrmlReaderTypeVrml97,
	eAnmVrmlReaderTypeX3DVrmlClassic,
	eAnmVrmlReaderTypeUnknown
} eAnmVrmlReaderType;

#include "anmlexer.h"

class CAnmVRML97Lexer : public CAnmLexer
{

protected :
	class CAnmVRML97Reader			*m_reader;
	BOOL							 m_blockCommentsOK;

	// Helpers
	int ValueTypeToToken(eValueType valueType);

	// Virtual methods that we must supply on top of Lexer base class

	// Lexical elements
	virtual bool IsWhiteSpace(int c)
	{
		return isspace(c) || c == ',';	// skip all those commas
	}

#if 0
	// trying to fix up Identifier recognition; people use some funky names
	virtual bool IsIDChar(int c)
	{
		return isalnum(c) || c == '-' || c == '_' || c == '%' || c == '/' ||
			c == '(' || c == ')' || c == '?';
	}
#else

	virtual bool IsFirstIDChar(int c)
	{
		// From the VRML97 spec:
		// Any ISO-10646 character encoded using UTF-8 except: 
		// 0x30-0x39, 0x0-0x20, 0x22, 0x23, 0x27, 0x2b, 0x2c, 0x2d, 0x2e,
		// 0x5b, 0x5c, 0x5d, 0x7b, 0x7d, 0x7f ; 

		if (c <= 0x20)	// N.B.: if c is negative it's probably not a good char anyway?
			return false;

		if (c <= 0x39 && c >= 0x30)
			return false;

		switch (c)
		{
	
			case 0x22 :
			case 0x23 :
			case 0x27 :					
			case 0x2b :
			case 0x2c :
			case 0x2d :
			case 0x2e :
			case 0x5b :
			case 0x5c :
			case 0x5d :
			case 0x7b :
			case 0x7d :
			case 0x7f :
				return false;
			default :
				return true;
		}
	
		return true;
	}

	virtual bool IsIDChar(int c)
	{
		// From the VRML97 spec:
		// Any number of ISO-10646 characters except: 0x0-0x20, 0x22, 0x23, 0x27, 0x2c, 0x2e,
		// 0x5b, 0x5c, 0x5d, 0x7b, 0x7d, 0x7f

		if (c <= 0x20)	// N.B.: if c is negative it's probably not a good char anyway?
			return false;

		switch (c)
		{
	
			case 0x22 :
			case 0x23 :
			case 0x27 :					
			case 0x2c :
			case 0x2e :
			case 0x5b :
			case 0x5c :
			case 0x5d :
			case 0x7b :
			case 0x7d :
			case 0x7f :
				return false;
			default :
				return true;
		}
	
		return true;
	}

#endif

	virtual bool IsLeadNumeric(int c)
	{
		return isdigit(c) || (c == '.') || (c == '-');
	}

	virtual bool IsQuoteChar(int c)
	{
		return c == '\"';
	}

	// Comments; returns true if stripped, possibly valid token
	virtual bool StripComment(int *pToken);

	// Find keyword from language keyword set (0 = not found)
	virtual int LookupKeyword(char *name);

	// Find generic identifier or data member
	virtual int LookupIdentifier(char *name);

	// Look for operator character(s); no-op for VRML parse. just advance char
	virtual int TryOperator()
	{
		int token = m_char;
		m_char = GetChar();

		return token;
	}

	// Interface to outside
	virtual void SaveIntegerValue(int intval);
	virtual void SaveDoubleValue(double dblval);
	virtual void SaveStringValue(char *strval);
	virtual void LexError(char *errtxt);


public :
	
	CAnmVRML97Lexer(const char *src, class CAnmVRML97Reader *pReader,
		bool fromFile);
	~CAnmVRML97Lexer();

	eAnmVrmlReaderType ReadHeaderLine();

	int GetLineNumber()
	{
		return m_lineNumber;
	}

};

#endif // _anmvrml97lexer_h
