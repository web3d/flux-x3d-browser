/********************************************************************************
 * Flux
 *
 * File: anmvrml97reader.h
 * Description: VRML97 reader class
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

#ifndef _anmvrml97reader_h
#define _anmvrml97reader_h

#include "anmfilereader.h"

#include <string>
#include <stack>

// YACC stuff
#define YYSTYPE VRML97YYSTYPE
typedef union  {
	int intVal;
	double doubleVal;
	String strVal;
	class CAnmSymbol *pSymbol;
	class CAnmNodeSymbol *pNodeSymbol;
	class CAnmAttributeSymbol *pAttrSymbol;
	eValueType attrType;
	eAnmClassMemberType memberType;
} YYSTYPE;
#define YYMAXDEPTH 1024

#define yychar ANMVRML97yychar
#define yynerrs ANMVRML97nerrs

class CAnmVRML97Reader : public CAnmFileReader
{
protected :

	class CAnmVRML97Lexer		*m_lexer;

	// Field type support
	TimeArray					m_doubleaccum;
	IntegerArray				m_intaccum;
	StringArray					m_stringaccum;

	// YACC data type support
	YYSTYPE						m_Yylval;
	YYSTYPE						m_Yyval;
	int							m_Pcyytoken;
	int							m_Pcyyerrfl;
	int							m_Pcyyerrct;
	YYSTYPE						m_Yyv[YYMAXDEPTH];


	void SetCurrentBool(Boolean b);

	void AccumDouble(double d)
	{
		m_doubleaccum.push_back(d);
	}

	void ClearDoubleAccum()
	{
		m_doubleaccum.clear();
	}

	void AccumInteger(int i)
	{
		m_intaccum.push_back(i);
	}

	void ClearIntegerAccum()
	{
		m_intaccum.clear();
	}

	void AccumString(String s)
	{
		m_stringaccum.push_back(s);
	}

	void ClearStringAccum()
	{
		// N.B.: do we need to unref each string pointer?
		m_stringaccum.clear();
	}

	void ClearAccums()
	{
		ClearDoubleAccum();
		ClearIntegerAccum();
		ClearStringAccum();
	}

	Boolean *GetCurrentBool();
	Color *ColorFromFloats();
	ColorArray **ColorArrayFromFloats();
	Double *DoubleFromFloats();
	DoubleArray **DoubleArrayFromFloats();
	Float *FloatFromFloats();
	FloatArray **FloatArrayFromFloats();
	Image **ImageFromInts();
	Integer *IntegerFromInts();
	IntegerArray **IntegerArrayFromInts();
	Rotation *RotationFromFloats();
	RotationArray **RotationArrayFromFloats();
	String *StringFromStrings();
	StringArray **StringArrayFromStrings();
	Time *TimeFromFloats();
	TimeArray **TimeArrayFromFloats();
	Point2 *Point2FromFloats();
	Point2Array **Point2ArrayFromFloats();
	Point3 *Point3FromFloats();
	Point3Array **Point3ArrayFromFloats();

	void BeginMakeNode(String nodetypename);
	void EndMakeNode();

	void DefNode(String defname);
	void UseNode(String usename);

	void NullNode();

	void MakeRoute(String fromName, String fromAttr,
		String toName, String toAttr);

	void BeginMakeScript();
	void EndMakeScript();

	void BeginMakeShader();
	void EndMakeShader();

	void SetValue(class CAnmAttributeSymbol *pAttrSym,
		void *pData);

	void IgnoreValue(String attrName, eValueType valueType);

	class CAnmAttributeSymbol *LookupAttribute(char *name);

	void BeginProtoDeclaration(String prototypename, BOOL isExternProto = FALSE);
	void EndProtoDeclaration(StringArray **ppExternProtoUrl = NULL);
	
	void SetFieldAccess(eAnmClassMemberType memberType);

	void AddField(eValueType fieldType, String fieldName);
	void AddField(eValueType fieldType, String fieldName, void *defaultValue);
	void AddField(eValueType fieldType, String fieldName, void *defaultValue, String ISName);
	void AddEventIn(eValueType fieldType, String eventInName);
	void AddEventOut(eValueType fieldType, String eventOutName);

	void AddIS(String name, String ISName);
	void AddIS(class CAnmAttributeSymbol *pAttrSym, String ISName);
	void AddIS(class CAnmAttributeSymbol *pAttrSym, class CAnmAttributeSymbol *pISAttrSym);
	void AddISField(eValueType fieldType, String name, String ISName);
	void AddISEventIn(eValueType fieldType, String name, String ISName);
	void AddISEventOut(eValueType fieldType, String name, String ISName);
	void AddMetaData(String key, String value);

	int Yyparse();					// defined in yacc-generated file

	int Yyerror(char* message);
	int Yylex();

public :

	CAnmVRML97Reader(class CAnmFileLoader *pLoader);
	virtual ~CAnmVRML97Reader();

	virtual eAnmReturnStatus Read(const char *source, bool fromFile);

	virtual int GetLineNumber();

	friend class CAnmVRML97Lexer;
};

#endif // _anmvrml97reader_h


