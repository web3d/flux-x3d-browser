/********************************************************************************
 * Flux
 *
 * File: anmlexer.cpp
 * Description: Abstract base lexer class
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
#include "anmlexer.h"

CAnmLexer::CAnmLexer(const char *src, bool fromFile, int itoken, int dtoken, int stoken,
					 int dottoken) : 
	m_integerToken(itoken),
	m_doubleToken(dtoken),
	m_stringToken(stoken),
	m_dotToken(dottoken)
{
	m_fromFile = fromFile;

	// Set up input stream (file or string)
	if (m_fromFile)
	{
		// open the file for read
		m_fp = fopen(src, "r");
		m_cp = NULL;
		m_fromCharLen = 0;
		m_fromCharIndex = 0;
	}
	else
	{
		m_cp = (char *) src;
		m_fromCharLen = strlen(src);
		m_fromCharIndex = 0;
		m_fp = NULL;
	}

	m_char = '\n';
	m_lastChar = -1;
	m_lineNumber = 0;
	m_charIndex = 0;

	m_ibindex = 0;
	m_curinputlinelength = 0;

	// Set up buffer for string literals
	m_stringBuffer = new char[STRINGBUFFERSIZE];
	m_curstringbuffersize = STRINGBUFFERSIZE;

}

CAnmLexer::~CAnmLexer()
{
	if (m_fp != NULL)
		fclose(m_fp);

	SafeDelete(m_stringBuffer);
}

void CAnmLexer::ExpandStringBuffer()
{
	m_curstringbuffersize += STRINGBUFFERSIZE;
	m_stringBuffer = (char *) realloc(m_stringBuffer, m_curstringbuffersize);
}

const int MAXINT = 0x7FFFFFFF;

int CAnmLexer::Lex()
{
	int   i, done;
	int   token;
	unsigned uintval;
	int intval;
	double dblval, finaldouble;
	int exponent;
	char matchquote;

	// First, try to strip out whitespace and/or comments
	bool stripping = true;
	while (stripping)
	{
		// Test EOF
		if (m_char == EOF) return(0);			// end of file

		// Try stripping out whitespace
		while (IsWhiteSpace(m_char))
		{		
			// strip whitespace
			if (m_char == '\n') m_lineNumber++, m_charIndex = -1;
			m_char = GetChar();
			m_charIndex++;
		}
		
		// Test EOF again
		if (m_char == EOF) return(0);			// end of file
	
		// Try stripping out comments
		bool commentstripped = StripComment(&token);

		if (token)
		{
			// We might have found a real token while trying to strip comments
			return(token);
		}
		else if (!commentstripped)
		{
			// If we stripped a comment, continue looking for whitespace and more comments;
			// otherwise we hit some other character and want to try lexing it. get out
			// of this loop.
			stripping = false;
		}
	}

	if (IsQuoteChar(m_char))
	{				
		// String constant
		matchquote = m_char;

		i = 0;
		done = FALSE;
		while (!done)
		{
			m_char = GetChar();
			if (m_char == EOF)
			{
				LexError("Encountered end of file within string literal");
			}
			else if (m_char == '\\')
			{
				m_char = GetChar();
				// Only \" (maybe \') and \\ are understood.  For all others, the
				// backslash sequence is inserted as a literal.
				// i.e. "\f" becomes the two characters \ and f
				if (m_char != matchquote && m_char != '\\')
				{
					if (i >= m_curstringbuffersize - 1)
						ExpandStringBuffer();

					m_stringBuffer[i++] = '\\';
				}

				if (i >= m_curstringbuffersize - 1)
					ExpandStringBuffer();
				
				m_stringBuffer[i++] = m_char;
			}
			else if (m_char == matchquote)
			{
				done = TRUE;
				m_char = GetChar();
			}
			else
			{
				if (m_char == '\n')
				{
					m_lineNumber++;
					m_charIndex = -1;
				}
				if (i >= m_curstringbuffersize - 1)
					ExpandStringBuffer();
	
				m_stringBuffer[i++] = m_char;
			}
		}

		m_stringBuffer[i++] = '\0';
		SaveStringValue(m_stringBuffer);

		token = m_stringToken;
	}
	else if (IsLeadNumeric(m_char))
	{
		token = m_integerToken;
		uintval = 0;
		exponent = 0;
		int minusflag = FALSE;
		int isint = TRUE;
		dblval = 0.0;

		if (m_char == '.')
		{
			m_char = GetChar();
			if (!isdigit(m_char))
				return m_dotToken;

			// if we're here the number is of the form .ddddd
			token = m_doubleToken;
			isint = FALSE;
			double power = 0.1;
			
			while (isdigit(m_char))
			{
				dblval = dblval + (m_char - '0') * power;
				power *= 0.1;
				m_char = GetChar();
			}
		}

		// This deals with whether or not '-' is an operator or lexical element
		if (IsLeadNumeric('-') && m_char == '-')
		{
			m_char = GetChar();
			minusflag = TRUE;
		}

		// Check if this is a hexadecimal number
		if (m_char == '0')
		{
			// First character is a leading zero, we can discard it
			// so we can check if the 2nd char is an 'x'
			m_char = GetChar();
			if (m_char == 'x' || m_char == 'X') {
				// It's hex, parse it.
				m_char = GetChar();
				while (isxdigit(m_char))
				{
					uintval <<= 4;
					if (isdigit(m_char))
						uintval += m_char - '0';
					else
						uintval += toupper(m_char) - 'A' + 10;
					m_char = GetChar();
				}
				intval = (int) uintval;
				if (minusflag)
					intval = -intval;

				SaveIntegerValue(intval);

				return m_integerToken;
			}
		}

		BOOL overflow = FALSE;

		// Read in the decimal number
		while (isdigit(m_char))
		{
			uintval = uintval * 10 + (m_char - '0');
			m_char = GetChar();
			// If uintval has gone over the limits of ints, go into REAL mode.
			if (uintval > MAXINT)
			{
				overflow = TRUE;
				break;
			}
		}

		if (overflow)
		{
			token = m_doubleToken;
			isint = FALSE;
			dblval = (double) uintval;
			uintval = 0;

			while (isdigit(m_char))
			{
				dblval = dblval * 10.0 + (double) (m_char - '0');
				m_char = GetChar();
			}
		}

		if (m_char == '.')
		{
			token = m_doubleToken;
			isint = FALSE;
			double power = 0.1;
			m_char = GetChar();
			
			while (isdigit(m_char))
			{
				dblval = dblval + (m_char - '0') * power;
				power *= 0.1;
				m_char = GetChar();
			}
		}
		
		// got this far; check for scientific notation
		if (m_char == 'e' || m_char == 'E')
		{
			int posexp;

			posexp = TRUE;

			m_char = GetChar();
			if (m_char == '-' || m_char == '+')
			{
				posexp = (m_char == '+');
				m_char = GetChar();
			}
			
			while (isdigit(m_char))
			{
				exponent = exponent * 10 + (m_char - '0');
				m_char = GetChar();
			}

			if (!posexp)
				exponent = -exponent;
		}

		if (exponent != 0)
		{
			token = m_doubleToken;
			isint = FALSE;
		}

		if (isint)
		{
			intval = (int) uintval;
			if (minusflag)
				intval = -intval;

			SaveIntegerValue(intval);
		}
		else
		{
			finaldouble = dblval + uintval;
			if (minusflag)
				finaldouble = -finaldouble;
			if (exponent != 0)
				finaldouble *= pow(10.0, exponent);

			SaveDoubleValue(finaldouble);
		}
	}
	else if (IsFirstIDChar(m_char))
	{
		// must be a name of some kind...
		i = 0;
		while (IsIDChar(m_char))
		{
			m_stringBuffer[i++] = m_char;
			m_char = GetChar();
		}
		
		m_stringBuffer[i++] = '\0';

		if (!(token = LookupKeyword(m_stringBuffer)))
			token = LookupIdentifier(m_stringBuffer);
	}
	else
	{
		token = TryOperator();
	}

	return(token);
}

