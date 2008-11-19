/********************************************************************************
 * Flux
 *
 * File: anmxmlreader.h
 * Description: XML reader base class
 *						Abstract class used by both DOM and SAX readers
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

#ifndef _anmxmlreader_h
#define _anmxmlreader_h

#include "anmfilereader.h"

#define ANMXMLDOCTYPENAME		"X3D"
#define ANMXMLANIMADOCTYPENAME	"ANIMA"
#define ANMXMLSCENETAGNAME		"Scene"
#define ANMXMLHEADTAGNAME		"head"
#define ANMXMLMETATAGNAME		"meta"

class CAnmXMLReader : public CAnmFileReader
{
protected :

	void ParseColorArray(BSTR str, int len);
	void ParseDoubleArray(BSTR str, int len);
	void ParsePoint3Array(BSTR str, int len);
	void ParsePoint2Array(BSTR str, int len);
	void ParseImage(BSTR str, int len);
	void ParseIntArray(BSTR str, int len);
	void ParseStringArray(BSTR str, int len);
	void ParseFloatArray(BSTR str, int len);
	void ParseRotationArray(BSTR str, int len);
	void ParseBooleanArray(BSTR str, int len);

public :

	CAnmXMLReader(class CAnmFileLoader *pLoader);
	virtual ~CAnmXMLReader();

};

#define ANMMAXXMLSTRBUF 2048



#endif // _anmxmlreader_h
