/********************************************************************************
 * Flux
 *
 * File: anmscreenfontstyle.cpp
 * Description: ScreenFontStyle node
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
#include "anmfontstyle.h"
#include "anmscreenfontstyle.h"

CAnmScreenFontStyle::CAnmScreenFontStyle() : CAnmNode(), // parent Text will add me later
	m_horizontal(ANMSCREENFONTSTYLE_DEFAULT_HORIZONTAL),
	m_leftToRight(ANMSCREENFONTSTYLE_DEFAULT_LEFTTORIGHT),
	m_pointSize(ANMSCREENFONTSTYLE_DEFAULT_POINTSIZE),
	m_spacing(ANMSCREENFONTSTYLE_DEFAULT_SPACING),
	m_topToBottom(ANMSCREENFONTSTYLE_DEFAULT_TOPTOBOTTOM)
{
	m_language = new CAnmString(ANMSCREENFONTSTYLE_DEFAULT_LANGUAGE);
	m_style = new CAnmString(ANMSCREENFONTSTYLE_DEFAULT_STYLE);

	m_family = new StringArray;
	m_family->push_back(new CAnmString(ANMSCREENFONTSTYLE_DEFAULT_FAMILY));

	m_justify = new StringArray;
	m_justify->push_back(new CAnmString(ANMSCREENFONTSTYLE_DEFAULT_JUSTIFY));
}

CAnmScreenFontStyle::~CAnmScreenFontStyle()
{
	SafeUnRef(m_language);
	SafeUnRef(m_style);
	SafeUnRef(m_family);
	SafeUnRef(m_justify);
}

void CAnmScreenFontStyle::Realize()
{
	CAnmNode::Realize();
}

void CAnmScreenFontStyle::SetFamily(StringArray *pFamily)
{
	assert(pFamily != NULL);

	SafeUnRef(m_family);
	m_family = pFamily;
	m_family->Ref();

	// N.B.: no need for dirty bits, parent notify or callbacks; this is not a mutable field
}

void CAnmScreenFontStyle::SetHorizontal(Boolean horizontal)
{
	m_horizontal = horizontal;
}

void CAnmScreenFontStyle::SetJustify(StringArray *pJustify)
{
	assert(pJustify != NULL);

	SafeUnRef(m_justify);
	m_justify = pJustify;
	m_justify->Ref();

	// N.B.: no need for dirty bits, parent notify or callbacks; this is not a mutable field
}

eFontStyleJustify CAnmScreenFontStyle::GetJustifyType(int index)
{
	if (m_justify)
	{
		int sz = m_justify->size();
		if (index < sz)
			return CAnmFontStyle::String2JustifyType((*m_justify)[index]);
	}

	return eFontStyleJustifyLeft;
}

void CAnmScreenFontStyle::SetLanguage(String language)
{
	SafeUnRef(m_language);
	m_language = language;
	m_language->Ref();
}

void CAnmScreenFontStyle::SetLeftToRight(Boolean leftToRight)
{
	m_leftToRight = leftToRight;
}

void CAnmScreenFontStyle::SetPointSize(Float pointSize)
{
	m_pointSize = pointSize;
}

void CAnmScreenFontStyle::SetSpacing(Float spacing)
{
	m_spacing = spacing;
}

void CAnmScreenFontStyle::SetStyle(String style)
{
	SafeUnRef(m_style);
	m_style = style;
	m_style->Ref();
}

void CAnmScreenFontStyle::SetTopToBottom(Boolean topToBottom)
{
	m_topToBottom = topToBottom;
}


DEFINE_VALUE(CAnmScreenFontStyle, family, eValueStringArray, StringArray*, GetFamily, SetFamily);
DEFINE_VALUE(CAnmScreenFontStyle, horizontal, eValueBoolean, Boolean, GetHorizontal, SetHorizontal);
DEFINE_VALUE(CAnmScreenFontStyle, justify, eValueStringArray, StringArray*, GetJustify, SetJustify);
DEFINE_VALUE(CAnmScreenFontStyle, language, eValueString, String, GetLanguage, SetLanguage);
DEFINE_VALUE(CAnmScreenFontStyle, leftToRight, eValueBoolean, Boolean, GetLeftToRight, SetLeftToRight);
DEFINE_VALUE(CAnmScreenFontStyle, pointSize, eValueFloat, Float, GetPointSize, SetPointSize);
DEFINE_VALUE(CAnmScreenFontStyle, spacing, eValueFloat, Float, GetSpacing, SetSpacing);
DEFINE_VALUE(CAnmScreenFontStyle, style, eValueString, String, GetStyle, SetStyle);
DEFINE_VALUE(CAnmScreenFontStyle, topToBottom, eValueBoolean, Boolean, GetTopToBottom, SetTopToBottom);

BEGIN_DEFINE_NODE_MEMBERS(CAnmScreenFontStyle)

VALUEID(CAnmScreenFontStyle, family),
VALUEID(CAnmScreenFontStyle, horizontal),
VALUEID(CAnmScreenFontStyle, justify),
VALUEID(CAnmScreenFontStyle, language),
VALUEID(CAnmScreenFontStyle, leftToRight),
VALUEID(CAnmScreenFontStyle, pointSize),
VALUEID(CAnmScreenFontStyle, spacing),
VALUEID(CAnmScreenFontStyle, style),
VALUEID(CAnmScreenFontStyle, topToBottom),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmScreenFontStyle, CAnmNode);


