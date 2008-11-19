/********************************************************************************
 * Flux
 *
 * File: anmextrudedfontstyle.h
 * Description: extrudedfontstyle node
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

#ifndef _anmextrudedfontstyle_h
#define _anmextrudedfontstyle_h

#include "anmnode.h"
#include "anmfontstyle.h"

#define ANMEXTRUDEDFONTSTYLE_DEFAULT_FAMILY			"SERIF"
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_HORIZONTAL		TRUE
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_JUSTIFY		"BEGIN"
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_LANGUAGE		""
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_LEFTTORIGHT	TRUE
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_SIZE			1.0f
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_SPACING		1.0f
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_STYLE			"PLAIN"
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_TOPTOBOTTOM	TRUE
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_DEPTH			0.1f
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_BACK			TRUE
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_SIDE			TRUE
#define ANMEXTRUDEDFONTSTYLE_DEFAULT_FRONT			TRUE

class CAnmExtrudedFontStyle : public CAnmNode
{
protected :

	StringArray					*m_family;
	Boolean						 m_horizontal;
	StringArray					*m_justify;
	String						 m_language;
	Boolean						 m_leftToRight;
	Float						 m_size;
	Float						 m_spacing;
	String						 m_style;
	Boolean						 m_topToBottom;
	Float						 m_depth;

	Boolean						m_back;
	Boolean						m_side;
	Boolean						m_front;


public:

	// constructor/destructor
	CAnmExtrudedFontStyle();
	virtual ~CAnmExtrudedFontStyle();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level rendering structures

	// New methods
	virtual eFontStyleJustify GetJustifyType(int index);

	// Accessors
 	void SetFamily(StringArray *pFamily);
	StringArray *GetFamily() { return m_family; }
	void GetFamily(StringArray **pVal)
	{ 
		if (pVal)
			*pVal = m_family;
	}

	void SetHorizontal(Boolean horizontal);
	Boolean GetHorizontal() { return m_horizontal; }
	void GetHorizontal(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_horizontal;
	}

 	void SetJustify(StringArray *pJustify);
	StringArray *GetJustify() { return m_justify; }
	void GetJustify(StringArray **pVal)
	{ 
		if (pVal)
			*pVal = m_justify;
	}

 	void SetLanguage(String language);
	String GetLanguage() { return m_language; }
	void GetLanguage(String *pVal)
	{ 
		if (pVal)
			*pVal = m_language;
	}
	
	void SetLeftToRight(Boolean leftToRight);
	Boolean GetLeftToRight() { return m_leftToRight; }
	void GetLeftToRight(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_leftToRight;
	}

	void SetSize(Float size);
	Float GetSize() { return m_size; }
	void GetSize(Float *pVal)
	{
		assert(pVal);
		*pVal = m_size;
	}

	void SetSpacing(Float spacing);
	Float GetSpacing() { return m_spacing; }
	void GetSpacing(Float *pVal)
	{
		assert(pVal);
		*pVal = m_spacing;
	}

	void SetDepth(Float depth);
	Float GetDepth() { return m_depth; }
	void GetDepth(Float *pVal)
	{
		assert(pVal);
		*pVal = m_depth;
	}

	void SetStyle(String style);
	String GetStyle() { return m_style; }
	void GetStyle(String *pVal)
	{ 
		if (pVal)
			*pVal = m_style;
	}
	
	void SetTopToBottom(Boolean topToBottom);
	Boolean GetTopToBottom() { return m_topToBottom; }
	void GetTopToBottom(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_topToBottom;
	}

	void SetFront(Boolean front);
	Boolean GetFront() { return m_front; }
	void GetFront(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_front;
	}


	void SetBack(Boolean back);
	Boolean GetBack() { return m_back; }
	void GetBack(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_back;
	}

	void SetSide(Boolean side);
	Boolean GetSide() { return m_side; }
	void GetSide(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_side;
	}



	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmExtrudedFontStyle);

	DECLARE_VALUE(family);
	DECLARE_VALUE(horizontal);
	DECLARE_VALUE(justify);
	DECLARE_VALUE(language);
	DECLARE_VALUE(leftToRight);
	DECLARE_VALUE(size);
	DECLARE_VALUE(spacing);
	DECLARE_VALUE(depth);
	DECLARE_VALUE(style);
	DECLARE_VALUE(topToBottom);
	DECLARE_VALUE(back);
	DECLARE_VALUE(side);
	DECLARE_VALUE(front);
};

DECLARE_GETIMPLEMENTATION(CAnmExtrudedFontStyle);

#endif //_anmextrudedfontstyle_h

