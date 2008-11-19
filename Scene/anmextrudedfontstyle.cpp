/********************************************************************************
 * Flux
 *
 * File: anmextrudedfontstyle.cpp
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
 
#include "stdafx.h"
#include "anmfontstyle.h"
#include "anmextrudedfontstyle.h"

CAnmExtrudedFontStyle::CAnmExtrudedFontStyle() : CAnmNode(), // parent Text will add me later
	m_horizontal(ANMEXTRUDEDFONTSTYLE_DEFAULT_HORIZONTAL),
	m_leftToRight(ANMEXTRUDEDFONTSTYLE_DEFAULT_LEFTTORIGHT),
	m_size(ANMEXTRUDEDFONTSTYLE_DEFAULT_SIZE),
	m_spacing(ANMEXTRUDEDFONTSTYLE_DEFAULT_SPACING),
	m_depth(ANMEXTRUDEDFONTSTYLE_DEFAULT_DEPTH),
	m_topToBottom(ANMEXTRUDEDFONTSTYLE_DEFAULT_TOPTOBOTTOM),
	m_back(ANMEXTRUDEDFONTSTYLE_DEFAULT_BACK),
	m_side(ANMEXTRUDEDFONTSTYLE_DEFAULT_SIDE),
	m_front(ANMEXTRUDEDFONTSTYLE_DEFAULT_FRONT)
{
	m_language = new CAnmString(ANMEXTRUDEDFONTSTYLE_DEFAULT_LANGUAGE);
	m_style = new CAnmString(ANMEXTRUDEDFONTSTYLE_DEFAULT_STYLE);

	m_family = new StringArray;
	m_family->push_back(new CAnmString(ANMEXTRUDEDFONTSTYLE_DEFAULT_FAMILY));

	m_justify = new StringArray;
	m_justify->push_back(new CAnmString(ANMEXTRUDEDFONTSTYLE_DEFAULT_JUSTIFY));
}

CAnmExtrudedFontStyle::~CAnmExtrudedFontStyle()
{
	SafeUnRef(m_language);
	SafeUnRef(m_style);
	SafeUnRef(m_family);
	SafeUnRef(m_justify);
}

void CAnmExtrudedFontStyle::Realize()
{
	CAnmNode::Realize();
}

void CAnmExtrudedFontStyle::SetFamily(StringArray *pFamily)
{
	assert(pFamily != NULL);

	SafeUnRef(m_family);
	m_family = pFamily;
	m_family->Ref();

	// N.B.: no need for dirty bits, parent notify or callbacks; this is not a mutable field
}

void CAnmExtrudedFontStyle::SetHorizontal(Boolean horizontal)
{
	m_horizontal = horizontal;
}

void CAnmExtrudedFontStyle::SetJustify(StringArray *pJustify)
{
	assert(pJustify != NULL);

	SafeUnRef(m_justify);
	m_justify = pJustify;
	m_justify->Ref();

	// N.B.: no need for dirty bits, parent notify or callbacks; this is not a mutable field
}

eFontStyleJustify CAnmExtrudedFontStyle::GetJustifyType(int index)
{
	if (m_justify)
	{
		int sz = m_justify->size();
		if (index < sz)
			return CAnmFontStyle::String2JustifyType((*m_justify)[index]);
	}

	return eFontStyleJustifyLeft;
}


void CAnmExtrudedFontStyle::SetLanguage(String language)
{
	SafeUnRef(m_language);
	m_language = language;
	m_language->Ref();
}

void CAnmExtrudedFontStyle::SetLeftToRight(Boolean leftToRight)
{
	m_leftToRight = leftToRight;
}

void CAnmExtrudedFontStyle::SetSize(Float size)
{
	m_size = size;
}

void CAnmExtrudedFontStyle::SetSpacing(Float spacing)
{
	m_spacing = spacing;
}

void CAnmExtrudedFontStyle::SetDepth(Float depth)
{
	m_depth = depth;
}

void CAnmExtrudedFontStyle::SetStyle(String style)
{
	SafeUnRef(m_style);
	m_style = style;
	m_style->Ref();
}

void CAnmExtrudedFontStyle::SetTopToBottom(Boolean topToBottom)
{
	m_topToBottom = topToBottom;
}

void CAnmExtrudedFontStyle::SetBack(Boolean back)
{
	m_back = back;
	SetStateDirty();
}

void CAnmExtrudedFontStyle::SetSide(Boolean side)
{
	m_side = side;
	SetStateDirty();
}

void CAnmExtrudedFontStyle::SetFront(Boolean front)
{
	m_front = front;
	SetStateDirty();
}


DEFINE_VALUE(CAnmExtrudedFontStyle, family, eValueStringArray, StringArray*, GetFamily, SetFamily);
DEFINE_VALUE(CAnmExtrudedFontStyle, horizontal, eValueBoolean, Boolean, GetHorizontal, SetHorizontal);
DEFINE_VALUE(CAnmExtrudedFontStyle, justify, eValueStringArray, StringArray*, GetJustify, SetJustify);
DEFINE_VALUE(CAnmExtrudedFontStyle, language, eValueString, String, GetLanguage, SetLanguage);
DEFINE_VALUE(CAnmExtrudedFontStyle, leftToRight, eValueBoolean, Boolean, GetLeftToRight, SetLeftToRight);
DEFINE_VALUE(CAnmExtrudedFontStyle, size, eValueFloat, Float, GetSize, SetSize);
DEFINE_VALUE(CAnmExtrudedFontStyle, spacing, eValueFloat, Float, GetSpacing, SetSpacing);
DEFINE_VALUE(CAnmExtrudedFontStyle, depth, eValueFloat, Float, GetDepth, SetDepth);
DEFINE_VALUE(CAnmExtrudedFontStyle, style, eValueString, String, GetStyle, SetStyle);
DEFINE_VALUE(CAnmExtrudedFontStyle, topToBottom, eValueBoolean, Boolean, GetTopToBottom, SetTopToBottom);
DEFINE_VALUE(CAnmExtrudedFontStyle, back, eValueBoolean, Boolean, GetBack, SetBack);
DEFINE_VALUE(CAnmExtrudedFontStyle, side, eValueBoolean, Boolean, GetSide, SetSide);
DEFINE_VALUE(CAnmExtrudedFontStyle, front, eValueBoolean, Boolean, GetFront, SetFront);

BEGIN_DEFINE_NODE_MEMBERS(CAnmExtrudedFontStyle)

VALUEID(CAnmExtrudedFontStyle, family),
VALUEID(CAnmExtrudedFontStyle, horizontal),
VALUEID(CAnmExtrudedFontStyle, justify),
VALUEID(CAnmExtrudedFontStyle, language),
VALUEID(CAnmExtrudedFontStyle, leftToRight),
VALUEID(CAnmExtrudedFontStyle, size),
VALUEID(CAnmExtrudedFontStyle, spacing),
VALUEID(CAnmExtrudedFontStyle, depth),
VALUEID(CAnmExtrudedFontStyle, style),
VALUEID(CAnmExtrudedFontStyle, topToBottom),
VALUEID(CAnmExtrudedFontStyle, back),
VALUEID(CAnmExtrudedFontStyle, side),
VALUEID(CAnmExtrudedFontStyle, front),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmExtrudedFontStyle, CAnmNode);


