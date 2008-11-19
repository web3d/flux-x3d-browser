/********************************************************************************
 * Flux
 *
 * File: anmmetadatastring.cpp
 * Description: MetadataString class
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
#include "anmmetadatastring.h"

CAnmMetadataString::CAnmMetadataString() : 
	CAnmMetadataNode()
{
	m_value = new StringArray;
}

CAnmMetadataString::~CAnmMetadataString()
{
	SafeUnRef(m_value);
}

// Accessors

void CAnmMetadataString::SetValue(StringArray *pValue)
{
	SafeUnRef(m_value);
	m_value = pValue;
	m_value->Ref();

	CallCallbacks(CALLBACKID(CAnmMetadataString, value_changed), &pValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmMetadataString, name, CAnmMetadataNode);
DEFINE_INHERITED_VALUE(CAnmMetadataString, reference, CAnmMetadataNode);
DEFINE_VALUE(CAnmMetadataString, value, eValueStringArray, StringArray*, GetValue, SetValue);

DEFINE_INHERITED_METHOD(CAnmMetadataString, set_name, CAnmMetadataNode);
DEFINE_INHERITED_METHOD(CAnmMetadataString, set_reference, CAnmMetadataNode);
DEFINE_METHOD(CAnmMetadataString, set_value, eValueStringArray, StringArray*, SetValue);

DEFINE_INHERITED_CALLBACK(CAnmMetadataString, name_changed, CAnmMetadataNode);
DEFINE_INHERITED_CALLBACK(CAnmMetadataString, reference_changed, CAnmMetadataNode);
DEFINE_CALLBACK(CAnmMetadataString, value_changed, eValueStringArray);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmMetadataString)
VALUEID(CAnmMetadataString, name),
VALUEID(CAnmMetadataString, reference),
VALUEID(CAnmMetadataString, value),

METHODID(CAnmMetadataString, set_name),
METHODID(CAnmMetadataString, set_reference),
METHODID(CAnmMetadataString, set_value),

CALLBACKID(CAnmMetadataString, name_changed),
CALLBACKID(CAnmMetadataString, reference_changed),
CALLBACKID(CAnmMetadataString, value_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmMetadataString, CAnmMetadataNode);

