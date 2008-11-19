/********************************************************************************
 * Flux
 *
 * File: anmxmlreader.cpp
 * Description: XML reader base class
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
#include "anmxmlreader.h"
#include "anmfileloader.h"
#include "anmsymbol.h"
#include "anmnode.h"
#include "anmgroup.h"
#include "anmmessage.h"

CAnmXMLReader::CAnmXMLReader(CAnmFileLoader *pLoader)
: CAnmFileReader(pLoader)
{
	m_loader->SetEncodingType(eAnmX3DXMLEncoding);
	m_loader->SetSpecificationVersion(ANMX3DSPEC_VERSION_3_0);
}

CAnmXMLReader::~CAnmXMLReader()
{
}

static void stripStuff(BSTR *pBstr)
{
	BSTR pB;
	char c;

	pB = *pBstr;

	while (true)
	{
		c = (char) *pB++;

		if (!isspace(c) && c != ',')
			break;
	}

	*pBstr = pB -1;
}


static void stripWord(BSTR *pBstr)
{
	BSTR pB;
	char c;

	pB = *pBstr;

	while (true)
	{
		c = (char) *pB++;

		if (isspace(c) || c == ',' || c == '\0' )
			break;
	}

	*pBstr = pB -1;
}


static double parseDouble(BSTR *pBstr, bool *success)
{
	BSTR endPtr;
	double retval;
	char c;
	char buf[1024]; // oughtta be enuff

	stripStuff(pBstr);

	endPtr = *pBstr;

	int i = 0;
	while (!isspace(c = (char) *endPtr++) && c != '\0')
	{
		buf[i++] = c;
	}
	buf[i] = '\0';

	retval = atof(buf);

	*pBstr = endPtr;

	*success = (i > 0);

	return retval;
}



static Boolean parseBoolean (BSTR *pBstr, bool *success)
{
	Boolean boolval = false;
	char attrvalue[10];

	stripStuff(pBstr);

	bstr2cstr(attrvalue, *pBstr, 8);
	if (!strncmp(attrvalue, "TRUE", 4)
		|| !strncmp(attrvalue, "True", 4)
		|| !strncmp(attrvalue, "true", 4) ) {
		boolval = true;
	}

	stripWord( pBstr );
	return boolval;
}


static long parseInt (BSTR *pBstr, bool *success)
{
	BSTR endPtr;
	long retval;
	char c;
	char buf[1024]; // oughtta be enuff


	stripStuff(pBstr);

	endPtr = *pBstr;

	int i = 0;
	while (!isspace(c = (char) *endPtr++) && c != '\0')
	{
		buf[i++] = c;
	}
	buf[i] = '\0';

	if (i > 0)
	{
		if (i > 1)
		{
			if (!strnicmp(buf, "0x", 2))
				sscanf(buf, "%x", &retval);
			else
				retval = atol(buf);
		}
		else 
			retval = atol(buf);
	}

	*pBstr = endPtr;

	*success = (i > 0);
	return retval;
}

void CAnmXMLReader::ParsePoint2Array(BSTR str, int len)
{
	int i;
	Point2 p2(0,0);
	BSTR origstr = str;

	SafeUnRef(m_point2array);
	m_point2array = new Point2Array;
	m_point2array->Ref();

	m_point2array->reserve(FLOATARRAY_ALLOC);
	m_point2array->begin();

	for (i = 0; ; i++)
	{
		if (i >= m_point2array->capacity())
		{
			m_point2array->reserve(m_point2array->capacity() + FLOATARRAY_ALLOC);
		}

		bool stat;
		p2.x = (Float) parseDouble(&str, &stat);

		if (stat)
			p2.y = (Float) parseDouble(&str, &stat);

		if (stat)
			m_point2array->push_back(p2);
		else
			break;

		if (!str)
			break;

		if (str - origstr >= (len))
			break;

	}

}

void CAnmXMLReader::ParsePoint3Array(BSTR str, int len)
{
	int i;
	Point3 p;
	BSTR origstr = str;

	SafeUnRef(m_point3array);
	m_point3array = new Point3Array;
	m_point3array->Ref();

	m_point3array->reserve(FLOATARRAY_ALLOC);
	m_point3array->begin();

	for (i = 0; ; i++)
	{
		if (i >= m_point3array->capacity())
		{
			m_point3array->reserve(m_point3array->capacity() + FLOATARRAY_ALLOC);
		}

		bool stat;
		p.x = (Float) parseDouble(&str, &stat);
		
		if (stat)
			p.y = (Float) parseDouble(&str, &stat);

		if (stat)
			p.z = (Float) parseDouble(&str, &stat);

		if (stat)
			m_point3array->push_back(p);
		else
			break;

		if (!str)
			break;

		if (str - origstr >= (len))
			break;

	}
}



void CAnmXMLReader::ParseColorArray(BSTR str, int len)
{
	int i;
	Color c;
	BSTR origstr = str;

	SafeUnRef(m_colorarray);
	m_colorarray = new ColorArray;
	m_colorarray->Ref();

	m_colorarray->reserve(FLOATARRAY_ALLOC);
	m_colorarray->begin();

	for (i = 0; ; i++)
	{
		if (i >= m_colorarray->capacity())
		{
			m_colorarray->reserve(m_colorarray->capacity() + FLOATARRAY_ALLOC);
		}

		bool stat;
		c.r = (Float) parseDouble(&str, &stat);

		if (stat)
			c.g = (Float) parseDouble(&str, &stat);

		if (stat)
			c.b = (Float) parseDouble(&str, &stat);

		if (stat)
			m_colorarray->push_back(c);
		else
			break;

		if (!str)
			break;

		if (str - origstr >= (len))
			break;

	}

}
void CAnmXMLReader::ParseRotationArray(BSTR str, int len)
{
	int i;
	Rotation r(0, 1, 0, 0);
	BSTR origstr = str;

	SafeUnRef(m_rotationarray);
	m_rotationarray = new RotationArray;
	m_rotationarray->Ref();

	m_rotationarray->reserve(ROTARRAY_ALLOC);
	m_rotationarray->begin();

	for (i = 0; ; i++)
	{
		if (i >= m_rotationarray->capacity())
		{
			m_rotationarray->reserve(m_rotationarray->capacity() + ROTARRAY_ALLOC);
		}

		bool stat;
		r.vec.x = (Float) parseDouble(&str, &stat);

		if (stat)
			r.vec.y = (Float) parseDouble(&str, &stat);

		if (stat)
			r.vec.z = (Float) parseDouble(&str, &stat);

		if (stat)
			r.theta = (Float) parseDouble(&str, &stat);

		if (stat)
			m_rotationarray->push_back(r);
		else
			break;

		if (!str)
			break;

		if (str - origstr >= (len))
			break;

	}
}







void CAnmXMLReader::ParseBooleanArray(BSTR str, int len)
{
	int i;
	BSTR origstr = str;

	SafeUnRef(m_booleanarray);
	m_booleanarray = new BooleanArray;
	m_booleanarray->Ref();

	m_booleanarray->reserve(BOOLEANARRAY_ALLOC);
	m_booleanarray->begin();
	for (i = 0; ; i++)
	{
		if (i >= m_booleanarray->capacity())
		{
			m_booleanarray->reserve(m_booleanarray->capacity() + BOOLEANARRAY_ALLOC);
		}

		bool stat;
//		int i = parseInt(&str, &stat);
		Boolean b = parseBoolean(&str, &stat);

		if (stat)
			m_booleanarray->push_back(b);
		else
			break;

		if (!str)
			break;

		if (str - origstr >= (len))
			break;
	}
}




void CAnmXMLReader::ParseIntArray(BSTR str, int len)
{
	int i;
	BSTR origstr = str;

	SafeUnRef(m_intarray);
	m_intarray = new IntegerArray;
	m_intarray->Ref();

	m_intarray->reserve(INTARRAY_ALLOC);
	m_intarray->begin();
	for (i = 0; ; i++)
	{
		if (i >= m_intarray->capacity())
		{
			m_intarray->reserve(m_intarray->capacity() + INTARRAY_ALLOC);
		}

		bool stat;
		int i = parseInt(&str, &stat);

		if (stat)
			m_intarray->push_back(i);
		else
			break;

		if (!str)
			break;

		if (str - origstr >= (len))
			break;
	}
}

void CAnmXMLReader::ParseImage(BSTR str, int len)
{
	int i;
	BSTR origstr = str;

	int w, h, nC;

	bool stat;
	w = parseInt(&str, &stat);

	if (stat)
	{
		h = parseInt(&str, &stat);

		if (stat)
			nC = parseInt(&str, &stat);
	}

	int nchars = str - origstr;
	len -= nchars;

	if (!stat)
	{
		// ANMMSG_BADSFIMAGEXML : "Warning: malformed SFImage value at or near line %d"
		char *errmsg = (char *) malloc(strlen(ANMMSG_BADSFIMAGEXML) + 
			ANMMAXDECIMALLENGTH +
			1);

		sprintf(errmsg, ANMMSG_BADSFIMAGEXML, 
				GetLineNumber());

		WriteToConsole(errmsg);

		delete errmsg;
	}
	else
	{
		ParseIntArray(str, len);

		SafeUnRef(m_image);
		m_image = new Image(w, h, nC, m_intarray);
		m_image->Ref();

	}
}

static void parseString (BSTR *pBstr, char *outp, bool forceinnerquotes)
{
	BSTR endPtr;
	char c;
	char buf[1024]; // oughtta be enuff
	BOOL done;
	int i;
	char matchQuote;

	endPtr = *pBstr;
	buf[0] = '\0';

	while ((c = (char) *endPtr++) != '\"' && c != '\'' && c != '\0');

	if (c == '\0')
	{
#if 0
		*pBstr = endPtr;
		return;
#else
		endPtr = *pBstr;
#endif
	}

	matchQuote = c;

	// we got the leading quote or NUL char
	i = 0;
	done = FALSE;
	while (!done)
	{
		c = (char) *endPtr++;

		if (!forceinnerquotes || matchQuote != '\0')
		{
			if (c == '\\')
			{
				c = (char) *endPtr++;

				// Only \" and \\ are understood.  For all others, the
				// backslash sequence is inserted as a literal.
				// i.e. "\f" becomes the two characters \ and f
				if (c != '\"' && c != '\'' && c != '\\')
				{
					buf[i++] = '\\';
				}
				buf[i++] = c;
			}
			else if (c == matchQuote)
			{
				// got the closing quote
				done = TRUE;
			}
			else
				buf[i++] = c;
		}

		if (c == '\0')
			done = TRUE;
	}

	buf[i] = '\0';

	strcpy(outp, buf);

	*pBstr = endPtr;
}

void CAnmXMLReader::ParseStringArray(BSTR str, int len)
{
	int i;
	BSTR origstr = str;
	char buf[ANMMAXXMLSTRBUF];

	SafeUnRef(m_stringarray);
	m_stringarray = new StringArray;
	m_stringarray->Ref();

	m_stringarray->reserve(STRINGARRAY_ALLOC);
	m_stringarray->begin();

	for (i = 0; ; i++)
	{
		if (i >= m_stringarray->capacity())
		{
			m_stringarray->reserve(m_stringarray->capacity() + STRINGARRAY_ALLOC);
		}

		buf[0] = '\0';
		parseString(&str, buf, i > 0);
		if (true) // strlen(buf))
		{
			m_stringarray->push_back(new CAnmString(buf));
		}

		if (!str)
			break;

		if (str - origstr >= (len))
			break;
	}

	if (m_stringarray->size() == 0)
	{
		int minlen = min(ANMMAXXMLSTRBUF - 1, len);
		bstr2cstr(buf, origstr, minlen);

		m_stringarray->push_back(new CAnmString(buf));

		// ANMMSG_BADMFSTRINGXML : "Warning: malformed MFString value at or near line %d; missing interior quotes?"
		char *errmsg = (char *) malloc(strlen(ANMMSG_BADMFSTRINGXML) + 
			ANMMAXDECIMALLENGTH +
			1);

		sprintf(errmsg, ANMMSG_BADMFSTRINGXML, GetLineNumber());

		WriteToConsole(errmsg);

		delete errmsg;
	}
}

void CAnmXMLReader::ParseFloatArray(BSTR str, int len)
{
	int i;
	BSTR origstr = str;

	SafeUnRef(m_floatarray);
	m_floatarray = new FloatArray;
	m_floatarray->Ref();

	m_floatarray->reserve(INTARRAY_ALLOC);
	m_floatarray->begin();
	for (i = 0; ; i++)
	{
		if (i >= m_floatarray->capacity())
		{
			m_floatarray->reserve(m_floatarray->capacity() + FLOATARRAY_ALLOC);
		}

		bool stat;
		Float f = (Float) parseDouble(&str, &stat);
		
		if (stat)
			m_floatarray->push_back(f);
		else
			break;

		if (!str)
			break;

		if (str - origstr >= (len))
			break;
	}

}


void CAnmXMLReader::ParseDoubleArray(BSTR str, int len)
{
	int i;
	BSTR origstr = str;

	SafeUnRef(m_doublearray);
	m_doublearray = new DoubleArray;
	m_doublearray->Ref();

	m_doublearray->reserve(INTARRAY_ALLOC);
	m_doublearray->begin();
	for (i = 0; ; i++)
	{
		if (i >= m_doublearray->capacity())
		{
			m_doublearray->reserve(m_doublearray->capacity() + DOUBLEARRAY_ALLOC);
		}

		bool stat;
		Double d = parseDouble(&str, &stat);

		if (stat)
			m_doublearray->push_back(d);
		else
			break;

		if (!str)
			break;

		if (str - origstr >= (len))
			break;
	}

}