/********************************************************************************
 * Flux
 *
 * File: anmtext.h
 * Description: Text node
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

#ifndef _anmtext_h
#define _anmtext_h

#include "anmgeometry.h"

#define ANMTEXT_DEFAULT_MAXEXTENT	0.f
#define ANMTEXT_DEFAULT_SOLID		FALSE		// overrides default geometry class value

class CAnmText : public CAnmGeometry
{

protected:

	StringArray						*m_strings;
	FloatArray						*m_lengths;
	Float							 m_maxextent;
	CAnmNode						*m_fontstyle;


	class CAnmFont					*m_font;

	virtual void CreateMesh();
	virtual void FindFont();

	virtual float GenRadius();

	virtual void CheckFontStyleParams(CAnmNode *pFontStyleNode);

public:

	// constructor/destructor
	CAnmText();
	virtual ~CAnmText();

	// CAnmGeometry overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual eAnmReturnStatus AddChild(CAnmNode *pChild);


	// New methods

	// Accessors
	void SetStrings(StringArray *strings);
	StringArray *GetStrings() { return m_strings; }
	void GetStrings(StringArray **pVal)
	{
		if (pVal)
			*pVal = m_strings;
	}

	void SetLengths(FloatArray *lengths);
	FloatArray *GetLengths() { return m_lengths; }
	void GetLengths(FloatArray **pVal)
	{
		if (pVal)
			*pVal = m_lengths;
	}

	void SetMaxExtent(Float maxExtent);
	Float GetMaxExtent() { return m_maxextent; }
	void GetMaxExtent(Float *pVal)
	{
		if (pVal)
			*pVal = m_maxextent;
	}

	void SetFontStyle(class CAnmNode *pFontStyle);
	CAnmNode *GetFontStyle() { return m_fontstyle; }
	void GetFontStyle(class CAnmNode **pVal)
	{
		if (pVal)
			*pVal = m_fontstyle;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmText);

	DECLARE_VALUE(solid);
	DECLARE_VALUE(string);
	DECLARE_VALUE(length);
	DECLARE_VALUE(maxExtent);
	DECLARE_VALUE(fontStyle);

	DECLARE_METHOD(set_string);
	DECLARE_METHOD(set_length);
	DECLARE_METHOD(set_maxExtent);
	DECLARE_METHOD(set_fontStyle);

	DECLARE_CALLBACK(string_changed);
	DECLARE_CALLBACK(length_changed);
	DECLARE_CALLBACK(maxExtent_changed);
	DECLARE_CALLBACK(fontStyle_changed);

};

#define ANMTEXT_STRINGSDIRTY			ANMGEOMETRY_NDIRTYBITS
#define ANMTEXT_LENGTHSDIRTY			(ANMTEXT_STRINGSDIRTY + 1)
#define ANMTEXT_MAXEXTENTDIRTY			(ANMTEXT_STRINGSDIRTY + 2)
#define ANMTEXT_FONTSTYLEDIRTY			(ANMTEXT_STRINGSDIRTY + 3)


typedef enum eAnmTextDirtyFlags {
eAnmTextStringsDirty				= ANMDIRTYBIT(ANMTEXT_STRINGSDIRTY),
eAnmTextLengthsDirty				= ANMDIRTYBIT(ANMTEXT_LENGTHSDIRTY),
eAnmTextMaxExtentDirty				= ANMDIRTYBIT(ANMTEXT_MAXEXTENTDIRTY),
eAnmTextFontStyleDirty				= ANMDIRTYBIT(ANMTEXT_FONTSTYLEDIRTY),
} eAnmTextDirtyFlags;

#endif // _anmtext_h
