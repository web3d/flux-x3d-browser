/********************************************************************************
 * Flux
 *
 * File: anmvrml97lexer.cpp
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

#include "stdafx.h"
#include "anmvrml97lexer.h"
#include "anmvrml97reader.h"
#include "anmsymbol.h"
#include "anmnodedefs.h"
#include "anmvrmlgrammar.cpp.h"

CAnmVRML97Lexer::CAnmVRML97Lexer(const char *src, class CAnmVRML97Reader *pReader,
								 bool fromFile)
: CAnmLexer(src, fromFile, INTEGER_TOKEN, DOUBLE_TOKEN, STRING_TOKEN, '.')
{
	m_reader = pReader;

	// N.B.: enable block comment stripping (#/* */#);
	// not strictly VRML97 but ok in X3D "VRML classic"
	m_blockCommentsOK = FALSE;
}

CAnmVRML97Lexer::~CAnmVRML97Lexer()
{
	m_reader = NULL;
}

#define MAX_HEADER_LINE 2048

eAnmVrmlReaderType CAnmVRML97Lexer::ReadHeaderLine()
{
	char buf[MAX_HEADER_LINE];

	int i = 0;
	bool done = false;
	while (!done)
	{
		m_char = GetChar();

		if (m_char == EOF) return(eAnmVrmlReaderTypeUnknown);			// end of file

		if (m_char == '\n')
		{
			buf[i] = '\0';
			done = true;
			m_lineNumber++, m_charIndex = -1;
		}
		else
			buf[i] = m_char;

		i++;
	}

	if (!strncmp(buf, VRML97HEADERLINE, strlen(VRML97HEADERLINE)))
	{
		m_blockCommentsOK = FALSE;
		return eAnmVrmlReaderTypeVrml97;
	}
	else if (!strncmp(buf, X3DVRMLCLASSICHEADERLINE, strlen(X3DVRMLCLASSICHEADERLINE)))
	{
		m_blockCommentsOK = TRUE;
		return eAnmVrmlReaderTypeX3DVrmlClassic;
	}
	else
		return eAnmVrmlReaderTypeUnknown;
}

// Keyword table


struct Keyword { 
	char* name; 
	int token; 
	eValueType valueType;
};

static struct Keyword keywordTable[] =
{
	{ "SFBool", Boolean_Type, eValueBoolean },
	{ "MFBool", BooleanArray_Type, eValueBoolean },
	{ "SFColor", Color_Type, eValueColor },
	{ "MFColor", ColorArray_Type, eValueColorArray },
	{ "SFDouble", Double_Type, eValueDouble },
	{ "MFDouble", DoubleArray_Type, eValueDoubleArray },
	{ "SFFloat", Float_Type, eValueFloat },
	{ "MFFloat", FloatArray_Type, eValueFloatArray },
	{ "SFImage", Image_Type, eValueImage },
	{ "MFImage", ImageArray_Type, eValueImageArray },
	{ "SFInt32", Integer_Type, eValueInteger },
	{ "MFInt32", IntegerArray_Type, eValueIntegerArray },
	{ "SFNode", Node_Type, eValueNode },
	{ "MFNode", NodeArray_Type, eValueNodeArray },
	{ "SFRotation", Rotation_Type, eValueRotation },
	{ "MFRotation", RotationArray_Type, eValueRotationArray },
	{ "SFString", String_Type, eValueString },
	{ "MFString", StringArray_Type, eValueStringArray },
	{ "SFTime", Time_Type, eValueTime },
	{ "MFTime", TimeArray_Type, eValueTimeArray },
	{ "SFVec2f", Point2_Type, eValuePoint2 },
	{ "MFVec2f", Point2Array_Type, eValuePoint2Array },
	{ "SFVec3f", Point3_Type, eValuePoint3 },
	{ "MFVec3f", Point3Array_Type, eValuePoint3Array },
	{ "field", Field, (eValueType) 0 },
	{ "initializeOnly", Field, (eValueType) 0 },
	{ "exposedField", ExposedField, (eValueType) 0 },
	{ "inputOutput", ExposedField, (eValueType) 0 },
	{ "eventIn", EventIn, (eValueType) 0 },
	{ "inputOnly", EventIn, (eValueType) 0 },
	{ "eventOut", EventOut, (eValueType) 0 },
	{ "outputOnly", EventOut, (eValueType) 0 },
	{ "Script", Script, (eValueType) 0 },
	{ "ShaderProgram", ShaderProgram, (eValueType) 0 },
	{ "PROTO", PROTO, (eValueType) 0 },
	{ "EXTERNPROTO", EXTERNPROTO, (eValueType) 0 },
	{ "IS", IS, (eValueType) 0 },
	{ "DEF", DEF, (eValueType) 0 },
	{ "USE", USE, (eValueType) 0 },
	{ "ROUTE", ROUTE, (eValueType) 0 },
	{ "TO", TO, (eValueType) 0 },
	{ "TRUE", TRUE_TOKEN, (eValueType) 0 },
	{ "true", TRUE_TOKEN, (eValueType) 0 },
	{ "True", TRUE_TOKEN, (eValueType) 0 },
	{ "FALSE", FALSE_TOKEN, (eValueType) 0 },
	{ "false", FALSE_TOKEN, (eValueType) 0 },
	{ "False", FALSE_TOKEN, (eValueType) 0 },
	{ "NULL", NULL_TOKEN, (eValueType) 0 },
	{ "PROFILE", PROFILE, (eValueType) 0 },
	{ "COMPONENT", COMPONENT, (eValueType) 0 },
	{ "IMPORT", IMPORT_TOKEN, (eValueType) 0 },
	{ "EXPORT", EXPORT_TOKEN, (eValueType) 0 },
	{ "META", META, (eValueType) 0 },
};

const static int nkeywords = sizeof(keywordTable) / sizeof(struct Keyword);

bool CAnmVRML97Lexer::StripComment(int *pToken)
{	
	*pToken = 0;
	if (m_char == '#')
	{
		// Check for C-style block comments (#/* */#)
		m_char = GetChar();

		if (m_char == EOF)
			return true;			// end of file

		if (m_blockCommentsOK && m_char == '/')
		{	
			m_char = GetChar();
			if (m_char == EOF)
				return false;			// end of file

			if (m_char == '*')
			{
				// got the block comment begin
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
						if (m_char == EOF)
						{
							LexError("Encountered end of file within comment");
						}
						else if (m_char == '\n')
						{
							m_lineNumber++;
							m_charIndex = -1;
						}
						else if (m_char == '/')
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
							else if (m_char == '#')
							{
								m_char = GetChar();
								// whew, got block comment end
								break;
							}
						}
					}
				}

				// comment stripped; get out
				return true;
			}
		}

		// Block comment stripped, or just single-line comment
		while (TRUE)
		{
			if (m_char == EOF)
			{
				return true;					// end of file ok here
			}

			if (m_char == '\n')
				return true;

			m_char = GetChar();
		}

	}

	return false;
}

/* Search for keywords in the above list. */

int CAnmVRML97Lexer::LookupKeyword(char *name)
{
	int i;
	
	for (i = 0; i < nkeywords; i++)
	{
		if (!strcmp(keywordTable[i].name, name))
		{
			m_reader->m_Yylval.attrType = (eValueType) keywordTable[i].valueType;
			return keywordTable[i].token;
		}
	}

	return 0;
}

/* Search for valid identifiers */

int CAnmVRML97Lexer::LookupIdentifier(char *name)
{
	int i;
	CAnmAttributeSymbol *pAttrSymbol;
	
	if (pAttrSymbol = m_reader->LookupAttribute(name))
	{
		m_reader->m_Yylval.pAttrSymbol = pAttrSymbol;
		return ValueTypeToToken(pAttrSymbol->GetClassMember()->valueType);
	}
	
	m_reader->m_Yylval.strVal = new CAnmString(name);
	return IDENTIFIER;
}

int CAnmVRML97Lexer::ValueTypeToToken(eValueType valueType)
{
	switch (valueType)
	{
	case eValueBoolean :
		return BOOLEAN_ATTRIBUTE;
		break;
	case eValueColor :
		return COLOR_ATTRIBUTE;
		break;
	case eValueColorArray :
		return COLORARRAY_ATTRIBUTE;
		break;
	case eValueDouble :
		return DOUBLE_ATTRIBUTE;
		break;
	case eValueDoubleArray :
		return DOUBLEARRAY_ATTRIBUTE;
		break;
	case eValueFloat :
		return FLOAT_ATTRIBUTE;
		break;
	case eValueFloatArray :
		return FLOATARRAY_ATTRIBUTE;
		break;
	case eValueImage :
		return IMAGE_ATTRIBUTE;
		break;
	case eValueInteger :
		return INTEGER_ATTRIBUTE;
		break;
	case eValueIntegerArray :
		return INTEGERARRAY_ATTRIBUTE;
		break;
	case eValueNode :
		return NODE_ATTRIBUTE;
		break;
	case eValueNodeArray :
		return NODEARRAY_ATTRIBUTE;
		break;
	case eValuePoint2 :
		return POINT2_ATTRIBUTE;
		break;
	case eValuePoint2Array :
		return POINT2ARRAY_ATTRIBUTE;
		break;
	case eValuePoint3 :
		return POINT3_ATTRIBUTE;
		break;
	case eValuePoint3Array :
		return POINT3ARRAY_ATTRIBUTE;
		break;
	case eValueRotation :
		return ROTATION_ATTRIBUTE;
		break;
	case eValueRotationArray :
		return ROTATIONARRAY_ATTRIBUTE;
		break;
	case eValueString :
		return STRING_ATTRIBUTE;
		break;
	case eValueStringArray :
		return STRINGARRAY_ATTRIBUTE;
		break;
	case eValueTime :
		return TIME_ATTRIBUTE;
		break;
	case eValueTimeArray :
		return TIMEARRAY_ATTRIBUTE;
		break;
	default :
		return EOF;
	}

	return EOF;
}

void CAnmVRML97Lexer::SaveIntegerValue(int intval)
{
	m_reader->m_Yylval.intVal = intval;
}

void CAnmVRML97Lexer::SaveDoubleValue(double dblval)
{
	m_reader->m_Yylval.doubleVal = dblval;
}

void CAnmVRML97Lexer::SaveStringValue(char *strval)
{
	m_reader->m_Yylval.strVal = new CAnmString(strval);
}

void CAnmVRML97Lexer::LexError(char *errtxt)
{
	m_reader->Yyerror(errtxt);
}
