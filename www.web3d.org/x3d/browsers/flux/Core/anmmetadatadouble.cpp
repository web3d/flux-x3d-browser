/********************************************************************************
 * Flux
 *
 * File: anmmetadatadouble.cpp
 * Description: MetadataDouble class
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
#include "anmmetadatadouble.h"

CAnmMetadataDouble::CAnmMetadataDouble() : 
	CAnmMetadataNode()
{
	m_value = new DoubleArray;
}

CAnmMetadataDouble::~CAnmMetadataDouble()
{
	SafeUnRef(m_value);
}

// Accessors

void CAnmMetadataDouble::SetValue(DoubleArray *pValue)
{
	SafeUnRef(m_value);
	m_value = pValue;
	m_value->Ref();

	CallCallbacks(CALLBACKID(CAnmMetadataDouble, value_changed), &pValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmMetadataDouble, name, CAnmMetadataNode);
DEFINE_INHERITED_VALUE(CAnmMetadataDouble, reference, CAnmMetadataNode);
DEFINE_VALUE(CAnmMetadataDouble, value, eValueDoubleArray, DoubleArray*, GetValue, SetValue);

DEFINE_INHERITED_METHOD(CAnmMetadataDouble, set_name, CAnmMetadataNode);
DEFINE_INHERITED_METHOD(CAnmMetadataDouble, set_reference, CAnmMetadataNode);
DEFINE_METHOD(CAnmMetadataDouble, set_value, eValueDoubleArray, DoubleArray*, SetValue);

DEFINE_INHERITED_CALLBACK(CAnmMetadataDouble, name_changed, CAnmMetadataNode);
DEFINE_INHERITED_CALLBACK(CAnmMetadataDouble, reference_changed, CAnmMetadataNode);
DEFINE_CALLBACK(CAnmMetadataDouble, value_changed, eValueDoubleArray);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmMetadataDouble)
VALUEID(CAnmMetadataDouble, name),
VALUEID(CAnmMetadataDouble, reference),
VALUEID(CAnmMetadataDouble, value),

METHODID(CAnmMetadataDouble, set_name),
METHODID(CAnmMetadataDouble, set_reference),
METHODID(CAnmMetadataDouble, set_value),

CALLBACKID(CAnmMetadataDouble, name_changed),
CALLBACKID(CAnmMetadataDouble, reference_changed),
CALLBACKID(CAnmMetadataDouble, value_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmMetadataDouble, CAnmMetadataNode);

