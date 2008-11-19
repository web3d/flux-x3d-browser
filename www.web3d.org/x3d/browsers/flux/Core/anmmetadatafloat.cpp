/********************************************************************************
 * Flux
 *
 * File: anmmetadatafloat.cpp
 * Description: MetadataFloat class
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
#include "anmmetadatafloat.h"

CAnmMetadataFloat::CAnmMetadataFloat() : 
	CAnmMetadataNode()
{
	m_value = new FloatArray;
}

CAnmMetadataFloat::~CAnmMetadataFloat()
{
	SafeUnRef(m_value);
}

// Accessors

void CAnmMetadataFloat::SetValue(FloatArray *pValue)
{
	SafeUnRef(m_value);
	m_value = pValue;
	m_value->Ref();

	CallCallbacks(CALLBACKID(CAnmMetadataFloat, value_changed), &pValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmMetadataFloat, name, CAnmMetadataNode);
DEFINE_INHERITED_VALUE(CAnmMetadataFloat, reference, CAnmMetadataNode);
DEFINE_VALUE(CAnmMetadataFloat, value, eValueFloatArray, FloatArray*, GetValue, SetValue);

DEFINE_INHERITED_METHOD(CAnmMetadataFloat, set_name, CAnmMetadataNode);
DEFINE_INHERITED_METHOD(CAnmMetadataFloat, set_reference, CAnmMetadataNode);
DEFINE_METHOD(CAnmMetadataFloat, set_value, eValueFloatArray, FloatArray*, SetValue);

DEFINE_INHERITED_CALLBACK(CAnmMetadataFloat, name_changed, CAnmMetadataNode);
DEFINE_INHERITED_CALLBACK(CAnmMetadataFloat, reference_changed, CAnmMetadataNode);
DEFINE_CALLBACK(CAnmMetadataFloat, value_changed, eValueFloatArray);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmMetadataFloat)
VALUEID(CAnmMetadataFloat, name),
VALUEID(CAnmMetadataFloat, reference),
VALUEID(CAnmMetadataFloat, value),

METHODID(CAnmMetadataFloat, set_name),
METHODID(CAnmMetadataFloat, set_reference),
METHODID(CAnmMetadataFloat, set_value),

CALLBACKID(CAnmMetadataFloat, name_changed),
CALLBACKID(CAnmMetadataFloat, reference_changed),
CALLBACKID(CAnmMetadataFloat, value_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmMetadataFloat, CAnmMetadataNode);

