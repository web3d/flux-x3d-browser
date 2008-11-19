/********************************************************************************
 * Flux
 *
 * File: anmlexer.h
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

#ifndef _anmlexer_h
#define _anmlexer_h

// N.B.: length
#define STRINGBUFFERSIZE 1024
#define MAXINPUTBUFFER 1024

class CAnmLexer
{

protected :
	FILE							*m_fp;
	bool							 m_fromFile;
	char							*m_cp;
	int								 m_fromCharLen;
	int								 m_fromCharIndex;
	char							 m_char;
	int								 m_lineNumber;
	int								 m_charIndex;
	char							 m_lastChar;
	char							*m_stringBuffer;
	int								 m_curstringbuffersize;
	char							 m_inputbuffer[MAXINPUTBUFFER];
	int								 m_curinputlinelength;
	int								 m_ibindex;
	int								 m_integerToken;
	int								 m_doubleToken;
	int								 m_stringToken;
	int								 m_dotToken;

	// String allocation helper
	void ExpandStringBuffer();

	// Pure virtual methods; subclass must supply

	// Lexical elements
	virtual bool IsWhiteSpace(int c) PURE;
	virtual bool IsFirstIDChar(int c) PURE;
	virtual bool IsIDChar(int c) PURE;
	virtual bool IsLeadNumeric(int c) PURE;
	virtual bool IsQuoteChar(int c) PURE;

	// Comments; returns true if stripped, possibly valid token
	virtual bool StripComment(int *pToken) PURE;

	// Find keyword from language keyword set (0 = not found)
	virtual int LookupKeyword(char *name) PURE;

	// Find generic identifier or data member
	virtual int LookupIdentifier(char *name) PURE;

	// Look for operator character(s)
	virtual int TryOperator() PURE;

	// Interface to outside
	virtual void SaveIntegerValue(int intval) PURE;
	virtual void SaveDoubleValue(double dblval) PURE;
	virtual void SaveStringValue(char *strval) PURE;
	virtual void LexError(char *errtxt) PURE;

public :
	
	CAnmLexer(const char *src, bool fromFile, int itoken, int dtoken, int stoken,
		int dottoken);

	virtual ~CAnmLexer();

	int Lex();

	int GetChar()
	{
		int theChar;

		if (m_fromFile)
		{
			if (m_ibindex < m_curinputlinelength)
				theChar = m_inputbuffer[m_ibindex++];
			else
			{
				m_curinputlinelength = fread(m_inputbuffer, sizeof(char), MAXINPUTBUFFER, m_fp);

				if (m_curinputlinelength == 0)
					theChar = EOF;
				else
				{
					m_ibindex = 1;
					theChar = m_inputbuffer[0];
				}
			}

			// Deal with all CRLF permutations, blech
			if (theChar == '\r')
			{
				int nextChar;

				if (m_ibindex < m_curinputlinelength)
					nextChar = m_inputbuffer[m_ibindex++];
				else
				{
					m_curinputlinelength = fread(m_inputbuffer, sizeof(char), MAXINPUTBUFFER, m_fp);

					if (m_curinputlinelength == 0)
						nextChar = EOF;
					else
					{
						m_ibindex = 1;
						nextChar = m_inputbuffer[0];
					}
				}

				if (nextChar != EOF)
				{
					// CRLF && CR-only -> newline; push back any other char
					theChar = '\n';

					if (nextChar != '\n')
						--m_ibindex;
				}
			}
		}
		else
		{
			if (m_fromCharIndex >= m_fromCharLen)
				theChar = EOF;
			else
				theChar = m_cp[m_fromCharIndex++];

			// Deal with all CRLF permutations, blech
			if (theChar == '\r')
			{
				int nextChar;

				if (m_fromCharIndex >= m_fromCharLen)
					nextChar = EOF;
				else
					nextChar = m_cp[m_fromCharIndex++];

				if (nextChar != EOF)
				{
					// CRLF && CR-only -> newline; push back any other char
					theChar = '\n';

					if (nextChar != '\n')
						--m_fromCharIndex;
				}
			}
		}

		return theChar;
	}

	int GetLineNumber()
	{
		return m_lineNumber;
	}

};

#endif // _anmlexer_h
