/********************************************************************************
 * Flux
 *
 * File: anmmetadataset.cpp
 * Description: MetadataSet class
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
#include "anmmetadataset.h"

CAnmMetadataSet::CAnmMetadataSet() : 
	CAnmMetadataNode()
{
	m_value = new NodeArray;
}

CAnmMetadataSet::~CAnmMetadataSet()
{
	SafeUnRef(m_value);
}

// Accessors

void CAnmMetadataSet::SetValue(NodeArray *pValue)
{
	SafeUnRef(m_value);
	m_value = pValue;
	m_value->Ref();

	CallCallbacks(CALLBACKID(CAnmMetadataSet, value_changed), &pValue);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmMetadataSet, name, CAnmMetadataNode);
DEFINE_INHERITED_VALUE(CAnmMetadataSet, reference, CAnmMetadataNode);
DEFINE_VALUE(CAnmMetadataSet, value, eValueNodeArray, NodeArray*, GetValue, SetValue);

DEFINE_INHERITED_METHOD(CAnmMetadataSet, set_name, CAnmMetadataNode);
DEFINE_INHERITED_METHOD(CAnmMetadataSet, set_reference, CAnmMetadataNode);
DEFINE_METHOD(CAnmMetadataSet, set_value, eValueNodeArray, NodeArray*, SetValue);

DEFINE_INHERITED_CALLBACK(CAnmMetadataSet, name_changed, CAnmMetadataNode);
DEFINE_INHERITED_CALLBACK(CAnmMetadataSet, reference_changed, CAnmMetadataNode);
DEFINE_CALLBACK(CAnmMetadataSet, value_changed, eValueNodeArray);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmMetadataSet)
VALUEID(CAnmMetadataSet, name),
VALUEID(CAnmMetadataSet, reference),
VALUEID(CAnmMetadataSet, value),

METHODID(CAnmMetadataSet, set_name),
METHODID(CAnmMetadataSet, set_reference),
METHODID(CAnmMetadataSet, set_value),

CALLBACKID(CAnmMetadataSet, name_changed),
CALLBACKID(CAnmMetadataSet, reference_changed),
CALLBACKID(CAnmMetadataSet, value_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmMetadataSet, CAnmMetadataNode);

