/********************************************************************************
 * Flux
 *
 * File: anmeslexer.h
 * Description: ECMAScript lexer class
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

#ifndef _anmeslexer_h
#define _anmeslexer_h

#include "anmlexer.h"

class CAnmECMAScriptLexer : public CAnmLexer
{

protected :
	class CAnmECMAScriptCompiler	*m_compiler;

	// Virtual methods that we must supply on top of Lexer base class

	// Lexical elements
	virtual bool IsWhiteSpace(int c)
	{
		return isspace(c);
	}

	virtual bool IsFirstIDChar(int c)
	{
		return isalpha(c) || c == '_' || c == '$';
	}

	virtual bool IsIDChar(int c)
	{
		return isalnum(c) || c == '_' || c == '$';
	}

	virtual bool IsLeadNumeric(int c)
	{
		return isdigit(c) || (c== '.');
	}

	virtual bool IsQuoteChar(int c)
	{
		return c == '\"' || c == '\'';
	}

	// Comments; returns true if stripped, possibly valid token
	virtual bool StripComment(int *pToken);

	// Find keyword from language keyword set (0 = not found)
	virtual int LookupKeyword(char *name);

	// Find generic identifier or data member
	virtual int LookupIdentifier(char *name);

	// Look for operator character(s)
	virtual int TryOperator();

	// Interface to outside
	virtual void SaveIntegerValue(int intval);
	virtual void SaveDoubleValue(double dblval);
	virtual void SaveStringValue(char *strval);
	virtual void LexError(char *errtxt);


public :
	
	CAnmECMAScriptLexer(const char *str, class CAnmECMAScriptCompiler *pCompiler,
		bool fromFile);
	~CAnmECMAScriptLexer();

};

#endif // _anmeslexer_h
