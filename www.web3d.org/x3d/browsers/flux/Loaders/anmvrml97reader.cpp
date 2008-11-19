/********************************************************************************
 * Flux
 *
 * File: anmvrml97reader.cpp
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

#include "stdafx.h"
#include "anmvrml97reader.h"
#include "anmvrml97lexer.h"
#include "anmexecutioncontext.h"
#include "anmfileloader.h"
#include "anmmessage.h"
#include "anmsymbol.h"
#include "anmnode.h"
#include "anmgroup.h"

CAnmVRML97Reader::CAnmVRML97Reader(CAnmFileLoader *pLoader)
: CAnmFileReader(pLoader)
{
	m_lexer = NULL;

	m_doubleaccum.reserve(DOUBLEARRAY_ALLOC);
	m_intaccum.reserve(INTARRAY_ALLOC);
	m_stringaccum.reserve(STRINGARRAY_ALLOC);
}

CAnmVRML97Reader::~CAnmVRML97Reader()
{
}

eAnmReturnStatus CAnmVRML97Reader::Read(const char *source, bool fromFile)
{
	eAnmReturnStatus retstatus = eAnmReturnAllGood;
		
	m_lexer = new CAnmVRML97Lexer(source, this, fromFile);

	try {

		eAnmVrmlReaderType vrmlFileType = eAnmVrmlReaderTypeX3DVrmlClassic;
		if (fromFile)
		{
			vrmlFileType = m_lexer->ReadHeaderLine();
			if (vrmlFileType == eAnmVrmlReaderTypeUnknown)
				Yyerror("Unrecognized file header");
		}
			
		// assume 
		m_loader->SetEncodingType(eAnmX3DClassicVrmlEncoding);
		if (vrmlFileType == eAnmVrmlReaderTypeVrml97)
			m_loader->SetSpecificationVersion(ANMX3DSPEC_VERSION_2_0);
		else
			m_loader->SetSpecificationVersion(ANMX3DSPEC_VERSION_3_0);

		Yyparse();
	}
	catch( CAnmError err ) // ... )
	{		
		WriteToConsole((char *) err.GetText());

		// ANMMSG_DONEPARSINGWITHERRORS : "Done parsing with errors."
		WriteToConsole(ANMMSG_DONEPARSINGWITHERRORS);

		SafeDelete(m_lexer);

		return(eAnmReturnAllBad);
	}

	SafeDelete(m_lexer);

	return retstatus;
}

int CAnmVRML97Reader::Yylex(void)
{
	return m_lexer->Lex();
}

CAnmVRML97Reader::Yyerror(char *msg)
{
	// ANMMSG_VRML97LOADERROR : "VRML97 Load error: [%s] at line %d"
	char *errmsg = (char *) malloc(strlen(ANMMSG_VRML97LOADERROR) + 
		strlen(msg) +
		ANMMAXDECIMALLENGTH +
		1);

	sprintf(errmsg, ANMMSG_VRML97LOADERROR, msg, m_lexer->GetLineNumber());

	CAnmError err(errmsg);

	delete errmsg;

	throw err;
}

void CAnmVRML97Reader::BeginMakeNode(String nodetypename)
{
	m_loader->BeginMakeNode(nodetypename);
	nodetypename->UnRef();
}

void CAnmVRML97Reader::EndMakeNode(void)
{
	m_loader->EndMakeNode();
}

void CAnmVRML97Reader::BeginMakeScript()
{
	m_loader->BeginMakeScript();
}

void CAnmVRML97Reader::EndMakeScript()
{
	m_loader->EndMakeScript();
}

void CAnmVRML97Reader::BeginMakeShader()
{
	m_loader->BeginMakeShader();
}

void CAnmVRML97Reader::EndMakeShader()
{
	m_loader->EndMakeShader();
}
void CAnmVRML97Reader::BeginProtoDeclaration(String prototypename, BOOL isExternProto)
{
	m_loader->BeginProtoDeclaration(prototypename, isExternProto);
	prototypename->UnRef();
}

void CAnmVRML97Reader::EndProtoDeclaration(StringArray **ppExternProtoUrl)
{
	m_loader->EndProtoDeclaration(ppExternProtoUrl);
}

void CAnmVRML97Reader::DefNode(String nodename)
{
	m_loader->DefNode(nodename);
	nodename->UnRef();
}

void CAnmVRML97Reader::UseNode(String nodename)
{
	m_loader->UseNode(nodename);
	nodename->UnRef();
}

void CAnmVRML97Reader::NullNode()
{
	m_loader->NullNode();
}

void CAnmVRML97Reader::SetValue(CAnmAttributeSymbol *pAttrSymbol,void *pData)
{
	m_loader->SetValue(pAttrSymbol, pData);
}


void CAnmVRML97Reader::MakeRoute(String fromnode, String fromfield,
								 String tonode, String tofield)
{
	m_loader->MakeRoute(fromnode, fromfield, tonode, tofield);
	fromnode->UnRef();
	fromfield->UnRef();
	tonode->UnRef();
	tofield->UnRef();
}


CAnmAttributeSymbol *CAnmVRML97Reader::LookupAttribute(char *name)
{
	return m_loader->LookupAttribute(name);
}

void CAnmVRML97Reader::AddIS(String name, String ISName)
{
	m_loader->AddIS(name, ISName);
	name->UnRef();
	ISName->UnRef();
}

void CAnmVRML97Reader::AddISField(eValueType fieldType, String name, String ISName)
{
	m_loader->AddISField(fieldType, name, ISName);
	name->UnRef();
	ISName->UnRef();
}

void CAnmVRML97Reader::AddISEventIn(eValueType fieldType, String name, String ISName)
{
	m_loader->AddISEventIn(fieldType, name, ISName);
	name->UnRef();
	ISName->UnRef();
}

void CAnmVRML97Reader::AddISEventOut(eValueType fieldType, String name, String ISName)
{
	m_loader->AddISEventOut(fieldType, name, ISName);
	name->UnRef();
	ISName->UnRef();
}

void CAnmVRML97Reader::AddIS(class CAnmAttributeSymbol *pAttrSym, String ISName)
{
	m_loader->AddIS(pAttrSym, ISName);
	ISName->UnRef();
}

void CAnmVRML97Reader::AddIS(class CAnmAttributeSymbol *pAttrSym, class CAnmAttributeSymbol *pISAttrSym)
{
	m_loader->AddIS(pAttrSym, pISAttrSym);
}

void CAnmVRML97Reader::SetFieldAccess(eAnmClassMemberType memberType)
{
	m_loader->SetFieldAccess(memberType);
}

void CAnmVRML97Reader::AddField(eValueType fieldType, String fieldName)
{
	m_loader->AddField(fieldType, fieldName, NULL);
	fieldName->UnRef();
}

void CAnmVRML97Reader::AddField(eValueType fieldType, String fieldName, void *defaultValue)
{
	m_loader->AddField(fieldType, fieldName, defaultValue);
	fieldName->UnRef();
}

void CAnmVRML97Reader::AddField(eValueType fieldType, String fieldName, void *defaultValue, String ISName)
{
	m_loader->AddField(fieldType, fieldName, defaultValue, ISName);
	fieldName->UnRef();
	ISName->UnRef();
}

void CAnmVRML97Reader::AddEventIn(eValueType fieldType, String eventInName)
{
	m_loader->AddEventIn(fieldType, eventInName);
	eventInName->UnRef();
}

void CAnmVRML97Reader::AddEventOut(eValueType fieldType, String eventOutName)
{
	m_loader->AddEventOut(fieldType, eventOutName);
	eventOutName->UnRef();
}

void CAnmVRML97Reader::AddMetaData(String key, String value)
{
	m_loader->AddMetaData(key, value);
	key->UnRef();
	value->UnRef();
}

// methods to deal with value stack/lists
void CAnmVRML97Reader::SetCurrentBool(Boolean b)
{
	m_booleanvalues.push_back(b);
}

Boolean *CAnmVRML97Reader::GetCurrentBool()
{
	int sz = m_booleanvalues.size();
	return &m_booleanvalues[sz - 1];
}

Color *CAnmVRML97Reader::ColorFromFloats()
{
	Color c;
	c.r = (float) m_doubleaccum[0];
	c.g = (float) m_doubleaccum[1];
	c.b = (float) m_doubleaccum[2];

	ClearAccums();

	int sz = m_colorvalues.size();
	m_colorvalues.push_back(c);

	return &m_colorvalues[sz];
}

ColorArray **CAnmVRML97Reader::ColorArrayFromFloats()
{
	Color c;
	
	SafeUnRef(m_colorarray);
	m_colorarray = new ColorArray;
	m_colorarray->reserve(m_doubleaccum.size() / 3);

	for (int i = 0; i < m_doubleaccum.size() / 3; i++)
	{
		c.r = (float) m_doubleaccum[i*3];
		c.g = (float) m_doubleaccum[i*3 + 1];
		c.b = (float) m_doubleaccum[i*3 + 2];

		m_colorarray->push_back(c);
	}

	ClearAccums();

	return &m_colorarray;
}

Double *CAnmVRML97Reader::DoubleFromFloats()
{
	Double d = m_doubleaccum[0];

	ClearAccums();

	int sz = m_floatvalues.size();
	m_doublevalues.push_back(d);

	return &m_doublevalues[sz];
}

DoubleArray **CAnmVRML97Reader::DoubleArrayFromFloats()
{
	SafeUnRef(m_doublearray);
	m_doublearray = new DoubleArray;
	m_doublearray->reserve(m_doubleaccum.size());

	for (int i = 0; i < m_doubleaccum.size(); i++)
	{
		m_doublearray->push_back(m_doubleaccum[i]);
	}

	ClearAccums();

	return &m_doublearray;
}

Float *CAnmVRML97Reader::FloatFromFloats()
{
	Float f = (Float) m_doubleaccum[0];

	ClearAccums();

	int sz = m_floatvalues.size();
	m_floatvalues.push_back(f);

	return &m_floatvalues[sz];
}

FloatArray **CAnmVRML97Reader::FloatArrayFromFloats()
{
	SafeUnRef(m_floatarray);
	m_floatarray = new FloatArray;
	m_floatarray->reserve(m_doubleaccum.size());

	for (int i = 0; i < m_doubleaccum.size(); i++)
	{
		m_floatarray->push_back((float) m_doubleaccum[i]);
	}

	ClearAccums();

	return &m_floatarray;
}

Image **CAnmVRML97Reader::ImageFromInts()
{
	SafeUnRef(m_image);
	m_image = new Image;
	
	int iasize = m_intaccum.size();

	if (iasize > 0)
		m_image->width = m_intaccum[0];
	if (iasize > 1)
		m_image->height = m_intaccum[1];
	if (iasize > 2)
		m_image->numComponents = m_intaccum[2];

	IntegerArray *pPixels = new IntegerArray;
	pPixels->reserve(m_image->width * m_image->height);

	for (int i = 3; i < iasize; i++)
		pPixels->push_back(m_intaccum[i]);

	m_image->SetPixels(pPixels);

	ClearAccums();

	return &m_image;
}

Integer *CAnmVRML97Reader::IntegerFromInts()
{
	Integer i = m_intaccum[0];

	ClearAccums();

	int sz = m_integervalues.size();
	m_integervalues.push_back(i);

	return &m_integervalues[sz];
}

IntegerArray **CAnmVRML97Reader::IntegerArrayFromInts()
{
	SafeUnRef(m_intarray);
	m_intarray = new IntegerArray;
	m_intarray->reserve(m_intaccum.size());

	for (int i = 0; i < m_intaccum.size(); i++)
	{
		m_intarray->push_back(m_intaccum[i]);
	}

	ClearAccums();

	return &m_intarray;
}

Rotation *CAnmVRML97Reader::RotationFromFloats()
{
	Rotation r;
	
	r.vec.x = (float) m_doubleaccum[0];
	r.vec.y = (float) m_doubleaccum[1];
	r.vec.z = (float) m_doubleaccum[2];
	r.theta = (float) m_doubleaccum[3];

	ClearAccums();

	int sz = m_rotationvalues.size();
	m_rotationvalues.push_back(r);

	return &m_rotationvalues[sz];
}

RotationArray **CAnmVRML97Reader::RotationArrayFromFloats()
{
	Rotation r;

	SafeUnRef(m_rotationarray);
	m_rotationarray = new RotationArray;
	m_rotationarray->reserve(m_doubleaccum.size() / 4 );

	for (int i = 0; i < m_doubleaccum.size() / 4; i++)
	{
		r.vec.x = (float) m_doubleaccum[i*4];
		r.vec.y = (float) m_doubleaccum[i*4 + 1];
		r.vec.z = (float) m_doubleaccum[i*4 + 2];
		r.theta = (float) m_doubleaccum[i*4 + 3];

		m_rotationarray->push_back(r);
	}

	ClearAccums();

	return &m_rotationarray;
}

String *CAnmVRML97Reader::StringFromStrings()
{
	String s = m_stringaccum[0];

	ClearAccums();

	int sz = m_stringvalues.size();
	m_stringvalues.push_back(s);

	return &m_stringvalues[sz];
}

StringArray **CAnmVRML97Reader::StringArrayFromStrings()
{
	SafeUnRef(m_stringarray);
	m_stringarray = new StringArray;
	m_stringarray->reserve(m_stringaccum.size());

	for (int i = 0; i < m_stringaccum.size(); i++)
	{
		m_stringarray->push_back(m_stringaccum[i]);
	}

	ClearAccums();

	return &m_stringarray;
}

Time *CAnmVRML97Reader::TimeFromFloats()
{
	Time t = m_doubleaccum[0];

	ClearAccums();

	int sz = m_timevalues.size();
	m_timevalues.push_back(t);

	return &m_timevalues[sz];
}

TimeArray **CAnmVRML97Reader::TimeArrayFromFloats()
{
	SafeUnRef(m_timearray);
	m_timearray = new TimeArray;
	m_timearray->reserve(m_doubleaccum.size());

	for (int i = 0; i < m_doubleaccum.size(); i++)
	{
		m_timearray->push_back(m_doubleaccum[i]);
	}

	ClearAccums();

	return &m_timearray;
}

Point2 *CAnmVRML97Reader::Point2FromFloats()
{
	Point2 p2;
	p2.x = (float) m_doubleaccum[0];
	p2.y = (float) m_doubleaccum[1];

	ClearAccums();

	int sz = m_point2values.size();
	m_point2values.push_back(p2);

	return &m_point2values[sz];
}

Point2Array **CAnmVRML97Reader::Point2ArrayFromFloats()
{
	Point2 p2;

	SafeUnRef(m_point2array);
	m_point2array = new Point2Array;
	m_point2array->reserve(m_doubleaccum.size() / 2);

	for (int i = 0; i < m_doubleaccum.size() / 2; i++)
	{
		p2.x = (float) m_doubleaccum[i*2];
		p2.y = (float) m_doubleaccum[i*2 + 1];

		m_point2array->push_back(p2);
	}

	ClearAccums();

	return &m_point2array;
}

Point3 *CAnmVRML97Reader::Point3FromFloats()
{
	Point3 p3;

	p3.x = (float) m_doubleaccum[0];
	p3.y = (float) m_doubleaccum[1];
	p3.z = (float) m_doubleaccum[2];

	ClearAccums();

	int sz = m_point3values.size();
	m_point3values.push_back(p3);

	return &m_point3values[sz];
}

Point3Array **CAnmVRML97Reader::Point3ArrayFromFloats()
{
	Point3 p3;

	SafeUnRef(m_point3array);
	m_point3array = new Point3Array;
	m_point3array->reserve(m_doubleaccum.size() / 3);

	for (int i = 0; i < m_doubleaccum.size() / 3; i++)
	{
		p3.x = (float) m_doubleaccum[i*3];
		p3.y = (float) m_doubleaccum[i*3 + 1];
		p3.z = (float) m_doubleaccum[i*3 + 2];

		m_point3array->push_back(p3);
	}

	ClearAccums();

	return &m_point3array;
}

void CAnmVRML97Reader::IgnoreValue(String attrName, eValueType valueType)
{
	FloatArray **ppFloats = NULL;
	IntegerArray **ppInts = NULL;
	StringArray **ppStrings = NULL;

	switch (valueType)
	{
		case eValueBoolean :
			break;

		case eValueFloatArray :
			ppFloats = FloatArrayFromFloats();
			// dump it
			SafeUnRef((*ppFloats));
			break;

		case eValueIntegerArray :
			ppInts  = IntegerArrayFromInts();
			// dump it
			SafeUnRef((*ppInts));
			break;

		case eValueNodeArray :
			// anything?
			break;

		case eValueImage :
			ppInts  = IntegerArrayFromInts();
			// dump it
			SafeUnRef((*ppInts));
			break;

		case eValueStringArray :
			ppStrings = StringArrayFromStrings();
			// dump it
			SafeUnRef((*ppStrings));
			break;

		default :
			assert(FALSE);
			break;
	}
}


int CAnmVRML97Reader::GetLineNumber()
{
	return m_lexer->GetLineNumber();
}

