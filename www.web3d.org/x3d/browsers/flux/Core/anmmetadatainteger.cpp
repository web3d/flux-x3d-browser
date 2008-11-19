/********************************************************************************
 * Flux
 *
 * File: anmmetadatainteger.cpp
 * Description: MetadataInteger class
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
#include "anmmetadatainteger.h"

CAnmMetadataInteger::CAnmMetadataInteger() : 
	CAnmMetadataNode()
{
	m_value = new IntegerArray;
}

CAnmMetadataInteger::~CAnmMetadataInteger()
{
	SafeUnRef(m_value);
}

// Accessors

void CAnmMetadataInteger::SetValue(IntegerArray *pValue)
{
	SafeUnRef(m_value);
	m_value = pValue;
	m_value->Ref();

	CallCallbacks(CALLBACKID(CAnmMetadataInteger, value_changed), &pValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmMetadataInteger, name, CAnmMetadataNode);
DEFINE_INHERITED_VALUE(CAnmMetadataInteger, reference, CAnmMetadataNode);
DEFINE_VALUE(CAnmMetadataInteger, value, eValueIntegerArray, IntegerArray*, GetValue, SetValue);

DEFINE_INHERITED_METHOD(CAnmMetadataInteger, set_name, CAnmMetadataNode);
DEFINE_INHERITED_METHOD(CAnmMetadataInteger, set_reference, CAnmMetadataNode);
DEFINE_METHOD(CAnmMetadataInteger, set_value, eValueIntegerArray, IntegerArray*, SetValue);

DEFINE_INHERITED_CALLBACK(CAnmMetadataInteger, name_changed, CAnmMetadataNode);
DEFINE_INHERITED_CALLBACK(CAnmMetadataInteger, reference_changed, CAnmMetadataNode);
DEFINE_CALLBACK(CAnmMetadataInteger, value_changed, eValueIntegerArray);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmMetadataInteger)
VALUEID(CAnmMetadataInteger, name),
VALUEID(CAnmMetadataInteger, reference),
VALUEID(CAnmMetadataInteger, value),

METHODID(CAnmMetadataInteger, set_name),
METHODID(CAnmMetadataInteger, set_reference),
METHODID(CAnmMetadataInteger, set_value),

CALLBACKID(CAnmMetadataInteger, name_changed),
CALLBACKID(CAnmMetadataInteger, reference_changed),
CALLBACKID(CAnmMetadataInteger, value_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmMetadataInteger, CAnmMetadataNode);

