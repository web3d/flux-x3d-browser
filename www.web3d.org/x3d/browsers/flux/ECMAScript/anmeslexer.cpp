/********************************************************************************
 * Flux
 *
 * File: anmeslexer.cpp
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

#include "stdafx.h"
#include "anmeslexer.h"
#include "anmescompiler.h"
#include "anmesgrammar.cpp.h"
#include "anmeslanguagebinding.h"
#include "anmesmessage.h"
#include "anmesstring.h"
#include "anmessymbol.h"

CAnmECMAScriptLexer::CAnmECMAScriptLexer(const char *str, CAnmECMAScriptCompiler *pCompiler,
										 bool fromFile) :
CAnmLexer(str, fromFile, NUMERIC, NUMERIC, STRING, DOT)
{
	m_compiler = pCompiler;
}

CAnmECMAScriptLexer::~CAnmECMAScriptLexer()
{
	m_compiler = NULL;
}


// Keyword table


struct Keyword { 
	char* name; 
	int token;
	BOOL reserved;
};

static struct Keyword keywordTable[] =
{
	{ "true", TRUE_TOKEN},
	{ "false", FALSE_TOKEN},
	{ "TRUE", TRUE_TOKEN},
	{ "FALSE", FALSE_TOKEN},
	{ "null", NULL_TOKEN},
	{ "undefined", UNDEFINED_TOKEN},
	{ "Infinity", INFINITY}, 
	{ "break", BREAK},
	{ "case", CASE},
	{ "continue", CONTINUE},
	{ "default", DEFAULT},
	{ "delete", DELETE_TOKEN},
	{ "else", ELSE},
	{ "for", FOR},
	{ "function", FUNCTION},
	{ "if", IF},
	{ "in", IN_TOKEN},
	{ "new", NEW},
	{ "return", RETURN},
	{ "switch", SWITCH},
	{ "this", THIS_TOKEN},
	{ "typeof", TYPEOF},
	{ "var", VAR},
	{ "void", VOID_SYMBOL},
	{ "while", WHILE},
	{ "with", WITH}, 
};

const static int nkeywords = sizeof(keywordTable) / sizeof(struct Keyword);

static const char *reservedWordTable[] =
{
	"catch",
	"class",
	"const",
	"debugger",
	"do",
	"enum",
	"export",
	"extends",
	"finally",
	"import",
	"super",
	"throw",
	"try", 
};

const static int nreservedwords = sizeof(reservedWordTable) / sizeof(char *);

bool CAnmECMAScriptLexer::StripComment(int *pToken)
{
	*pToken = 0;

	if (m_char == '/')
	{	
		// possible comment
		m_char = GetChar();
		if (m_char == '/')
		{
			// C++-style comment; skip to end of line
			while (TRUE)
			{
				m_char = GetChar();
				if (m_char == EOF)
				{
					LexError("Encountered end of file within comment");
				}
				else if (m_char == '\n')
					break;
			}

			// comment stripped
			return true;
		}
		else if (m_char == '*')
		{
			// Old C-style comment.
			while (TRUE)
			{
				m_char = GetChar();
				if (m_char == EOF)
				{
					LexError("Encountered end of file within comment");
				}
				else if (m_char == '\n')
				{
					m_lineNumber++;
					m_charIndex = -1;
				}
				else if (m_char == '*')
				{
					m_char = GetChar();
					if (m_char == '/')
					{
						m_char = GetChar();
						break;
					}
				}
			}

			// comment stripped
			return true;
		}
		else
		{
			*pToken = DIV;
		}

	}

	return false;
}

// TryOperator() - this is where we really really want Lex!

int CAnmECMAScriptLexer::TryOperator()
{
	char theChar = m_char;

	// read one ahead
	m_char = GetChar();

	switch (theChar)
	{
		// Single char punctuation
		case '?' :
			return QUERY;
			break;

		case  ':':
			return COLON;
			break;

		case  ';':
			return SEMICOLON;
			break;

		case  '.':
			return DOT;
			break;

		case  ',':
			return COMMA;
			break;

		case  '{':
			return START_BLOCK;
			break;

		case  '}':
			return END_BLOCK;
			break;

		case  '(':
			return OPEN_PARENTHESIS;
			break;

		case  ')':
			return CLOSE_PARENTHESIS;
			break;

		case  '[':
			return OPEN_SQ_BRACKETS;
			break;

		case  ']':
			return CLOSE_SQ_BRACKETS;
			break;

		case  '~':
			return ONES_COMPLIMENT;
			break;

		// One- or two-char punctuation; read ahead if two-char match

		case  '!':
			if (m_char == '=')
			{
				m_char = GetChar();
				return NOT_EQUAL;
			}
			else
				return LOGICAL_NOT;
			break;

		case  '=':
			if (m_char == '=')
			{
				m_char = GetChar();
				return EQUALS ;
			}
			else
				return ASSIGN_SYMBOL;
			break;

		case  '&':
			if (m_char == '=')
			{
				m_char = GetChar();
				return BITWISE_AND_EQUALS;
			}
			else if (m_char == '&')
			{
				m_char = GetChar();
				return LOGICAL_AND;
			}
			else
				return BITWISE_AND;
			break;

		case  '|':
			if (m_char == '=')
			{
				m_char = GetChar();
				return BITWISE_OR_EQUALS;
			}
			else if (m_char == '|')
			{
				m_char = GetChar();
				return LOGICAL_OR;
			}
			else
				return BITWISE_OR;
			break;

		case  '-':
			if (m_char == '=')
			{
				m_char = GetChar();
				return MINUS_EQUALS;
			}
			else if (m_char == '-')
			{
				m_char = GetChar();
				return DECREMENT;
			}
			else
				return MINUS;
			break;

		case  '+':
			if (m_char == '=')
			{
				m_char = GetChar();
				return PLUS_EQUALS;
			}
			else if (m_char == '+')
			{
				m_char = GetChar();
				return INCREMENT;
			}
			else
				return PLUS;
			break;

		case  '*':
			if (m_char == '=')
			{
				m_char = GetChar();
				return MULTIPLY_EQUALS;
			}
			else
				return MULTIPLY;
			break;

		case  '/':
			if (m_char == '=')
			{
				m_char = GetChar();
				return DIV_EQUALS;
			}
			else
				return DIV;
			break;

		case  '%':
			if (m_char == '=')
			{
				m_char = GetChar();
				return MOD_EQUALS;
			}
			else
				return MOD;
			break;

		case  '^':
			if (m_char == '=')
			{
				m_char = GetChar();
				return BITWISE_EXCLUSIVE_OR_EQUALS;
			}
			else
				return BITWISE_EXCLUSIVE_OR;
			break;

		// Real crazy stuff: one-, two- or up to four-character punctuation

		case  '<':
			if (m_char == '=')
			{
				m_char = GetChar();
				return LS_EQUAL;
			}
			else if (m_char == '<')
			{
				// read ahead at least one more
				m_char = GetChar();
				if (m_char == '=')
				{
					m_char = GetChar();
					return BITWISE_SHIFT_LEFT_EQUALS;
				}
				else
					return BITWISE_SHIFT_LEFT ;
			}
			else
				return LESS_THAN;
			break;

		case  '>':
			if (m_char == '=')
			{
				m_char = GetChar();
				return GT_EQUAL;
			}
			else if (m_char == '>')
			{
				// read ahead at least one more
				m_char = GetChar();
				if (m_char == '=')
				{
					m_char = GetChar();
					return BITWISE_SHIFT_RIGHT_EQUALS;
				}
				else if (m_char == '>')
				{
					// yet another readahead, looking for '='
					m_char = GetChar();
					if (m_char == '=')
					{
						m_char = GetChar();
						return BITWISE_SHIFT_RIGHT_ZERO_FILL_EQUALS;
					}
					else
						return BITWISE_SHIFT_RIGHT_ZERO_FILL ;
				}
				else
					return BITWISE_SHIFT_RIGHT ;
			}
			else
				return GREATER_THAN;
			break;
	}

	return theChar;
}


/* Search for keywords in the above list.  If none found, it's an IDENTIFIER */
int CAnmECMAScriptLexer::LookupKeyword(char *name)
{
	int i;
	
	// search for keywords
	for (i = 0; i < nkeywords; i++)
	{
		if (!strcmp(keywordTable[i].name, name))
		{
			return keywordTable[i].token;
		}
	}

	// try looking for reserved words
	for (i = 0; i < nreservedwords; i++)
	{
		if (!strcmp(reservedWordTable[i], name))
		{
			// ANMMSG_ESRESERVEDWORD : "ECMAScript warning: reserved word '%s' used at line '%d"
			char *errmsg = (char *) malloc(strlen(ANMMSG_ESRESERVEDWORD) + 
				strlen(name) +
				ANMMAXDECIMALLENGTH +
				1);

			sprintf(errmsg, ANMMSG_ESRESERVEDWORD, name, m_lineNumber);

			CAnmError err(errmsg);

			delete errmsg;

			throw err;

		}
	}
	
	return 0;
}

int CAnmECMAScriptLexer::LookupIdentifier(char *name)
{
	m_compiler->m_Yylval.idVal = new CAnmString(name); 
	return IDENTIFIER;
}

void CAnmECMAScriptLexer::SaveIntegerValue(int intval)
{
	m_compiler->m_Yylval.doubleVal = (double) intval;
}

void CAnmECMAScriptLexer::SaveDoubleValue(double dblval)
{
	m_compiler->m_Yylval.doubleVal = dblval;
}

void CAnmECMAScriptLexer::SaveStringValue(char *strval)
{
	CAnmECMAScriptLanguageBinding *pOwner = m_compiler->GetOwner();

	CAnmESString *pStr = new CAnmESString(new CAnmString(strval), pOwner);
	m_compiler->m_Yylval.strVal = pStr;

	AnmESVar *pStrVar = new AnmESVar(pStr);
	pOwner->AddGlobal(pStrVar);
}

void CAnmECMAScriptLexer::LexError(char *errtxt)
{
	m_compiler->Yyerror(errtxt);
}
