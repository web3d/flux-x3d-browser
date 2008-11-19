/********************************************************************************
 * Flux
 *
 * File: anmfontstyle.cpp
 * Description: FontStyle node
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

CAnmFontStyle::CAnmFontStyle() : CAnmNode(), // parent Text will add me later
	m_horizontal(ANMFONTSTYLE_DEFAULT_HORIZONTAL),
	m_leftToRight(ANMFONTSTYLE_DEFAULT_LEFTTORIGHT),
	m_size(ANMFONTSTYLE_DEFAULT_SIZE),
	m_spacing(ANMFONTSTYLE_DEFAULT_SPACING),
	m_topToBottom(ANMFONTSTYLE_DEFAULT_TOPTOBOTTOM)
{
	m_language = new CAnmString(ANMFONTSTYLE_DEFAULT_LANGUAGE);
	m_style = new CAnmString(ANMFONTSTYLE_DEFAULT_STYLE);

	m_family = new StringArray;
	m_family->push_back(new CAnmString(ANMFONTSTYLE_DEFAULT_FAMILY));

	m_justify = new StringArray;
	m_justify->push_back(new CAnmString(ANMFONTSTYLE_DEFAULT_JUSTIFY));
}

CAnmFontStyle::~CAnmFontStyle()
{
	SafeUnRef(m_language);
	SafeUnRef(m_style);
	SafeUnRef(m_family);
	SafeUnRef(m_justify);
}

void CAnmFontStyle::Realize()
{
	CAnmNode::Realize();
}

void CAnmFontStyle::SetFamily(StringArray *pFamily)
{
	assert(pFamily != NULL);

	SafeUnRef(m_family);
	m_family = pFamily;
	m_family->Ref();

	// N.B.: no need for dirty bits, parent notify or callbacks; this is not a mutable field
}

void CAnmFontStyle::SetHorizontal(Boolean horizontal)
{
	m_horizontal = horizontal;
}

void CAnmFontStyle::SetJustify(StringArray *pJustify)
{
	assert(pJustify != NULL);

	SafeUnRef(m_justify);
	m_justify = pJustify;
	m_justify->Ref();

	// N.B.: no need for dirty bits, parent notify or callbacks; this is not a mutable field
}

eFontStyleJustify CAnmFontStyle::GetJustifyType(int index)
{
	if (m_justify)
	{
		int sz = m_justify->size();
		if (index < sz)
			return String2JustifyType((*m_justify)[index]);
	}

	if( index > 0 ) {
		return eFontStyleJustifyFirst;
	}
	else {
		return eFontStyleJustifyLeft;
	}
}

eFontStyleJustify CAnmFontStyle::String2JustifyType(String s)
{
	const char *cp = s->GetBuf();

	if (!strcmp(cp, "BEGIN"))
		return eFontStyleJustifyLeft;
	else if (!strcmp(cp, "MIDDLE"))
		return eFontStyleJustifyMiddle;
	else if (!strcmp(cp, "END"))
		return eFontStyleJustifyRight;
	else if (!strcmp(cp, "FIRST"))
		return eFontStyleJustifyFirst;
	else
		return eFontStyleJustifyLeft;
}

void CAnmFontStyle::SetLanguage(String language)
{
	SafeUnRef(m_language);
	m_language = language;
	m_language->Ref();
}

void CAnmFontStyle::SetLeftToRight(Boolean leftToRight)
{
	m_leftToRight = leftToRight;
}

void CAnmFontStyle::SetSize(Float size)
{
	m_size = size;
}

void CAnmFontStyle::SetSpacing(Float spacing)
{
	m_spacing = spacing;
}

void CAnmFontStyle::SetStyle(String style)
{
	SafeUnRef(m_style);
	m_style = style;
	m_style->Ref();
}

void CAnmFontStyle::SetTopToBottom(Boolean topToBottom)
{
	m_topToBottom = topToBottom;
}

DEFINE_VALUE(CAnmFontStyle, family, eValueStringArray, StringArray*, GetFamily, SetFamily);
DEFINE_VALUE(CAnmFontStyle, horizontal, eValueBoolean, Boolean, GetHorizontal, SetHorizontal);
DEFINE_VALUE(CAnmFontStyle, justify, eValueStringArray, StringArray*, GetJustify, SetJustify);
DEFINE_VALUE(CAnmFontStyle, language, eValueString, String, GetLanguage, SetLanguage);
DEFINE_VALUE(CAnmFontStyle, leftToRight, eValueBoolean, Boolean, GetLeftToRight, SetLeftToRight);
DEFINE_VALUE(CAnmFontStyle, size, eValueFloat, Float, GetSize, SetSize);
DEFINE_VALUE(CAnmFontStyle, spacing, eValueFloat, Float, GetSpacing, SetSpacing);
DEFINE_VALUE(CAnmFontStyle, style, eValueString, String, GetStyle, SetStyle);
DEFINE_VALUE(CAnmFontStyle, topToBottom, eValueBoolean, Boolean, GetTopToBottom, SetTopToBottom);

BEGIN_DEFINE_NODE_MEMBERS(CAnmFontStyle)

VALUEID(CAnmFontStyle, family),
VALUEID(CAnmFontStyle, horizontal),
VALUEID(CAnmFontStyle, justify),
VALUEID(CAnmFontStyle, language),
VALUEID(CAnmFontStyle, leftToRight),
VALUEID(CAnmFontStyle, size),
VALUEID(CAnmFontStyle, spacing),
VALUEID(CAnmFontStyle, style),
VALUEID(CAnmFontStyle, topToBottom),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmFontStyle, CAnmNode);


