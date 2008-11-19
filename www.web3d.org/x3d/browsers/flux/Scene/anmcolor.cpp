/********************************************************************************
 * Flux
 *
 * File: anmcolor.cpp
 * Description: Color node
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
#include "anmcolor.h"
#include "anmindexedfaceset.h"

CAnmColor::CAnmColor() : CAnmNode() // parent IFS will add me later
{
	m_color = new ColorArray;
}

CAnmColor::~CAnmColor()
{
	SafeUnRef(m_color);
}

void CAnmColor::Realize()
{
	CAnmNode::Realize();
}

void CAnmColor::Update()
{
	ClearStateDirty();
}


void CAnmColor::SetColor(ColorArray *pColors)
{
	assert(pColors != NULL);

	SafeUnRef(m_color);
	m_color = pColors;
	m_color->Ref();

	SetStateDirty();
	NotifyParentsOfChange();

	CallCallbacks(CALLBACKID(CAnmColor, color_changed), &pColors);
}

DEFINE_VALUE(CAnmColor, color, eValueColorArray, ColorArray*, 
			 GetColor, SetColor);
DEFINE_METHOD(CAnmColor, set_color, eValueColorArray, ColorArray*, SetColor);
DEFINE_CALLBACK(CAnmColor, color_changed, eValueColorArray);

BEGIN_DEFINE_NODE_MEMBERS(CAnmColor)
VALUEID(CAnmColor, color), 
METHODID(CAnmColor, set_color), 
CALLBACKID(CAnmColor, color_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmColor, CAnmNode);
