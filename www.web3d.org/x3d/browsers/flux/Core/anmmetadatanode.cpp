/********************************************************************************
 * Flux
 *
 * File: anmmetadatanode.cpp
 * Description: Metadata base class
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
#include "anmmetadatanode.h"

CAnmMetadataNode::CAnmMetadataNode() : 
	CAnmNode()
{
	m_metadataname = new CAnmString(ANMMETADATA_DEFAULT_NAME);
	m_reference = new CAnmString(ANMMETADATA_DEFAULT_REFERENCE);
}

CAnmMetadataNode::~CAnmMetadataNode()
{
	SafeUnRef(m_metadataname);
	SafeUnRef(m_reference);
}

void CAnmMetadataNode::Realize()
{
	CAnmNode::Realize();
}


// Accessors

void CAnmMetadataNode::SetMetadataName(String name)
{
	SafeUnRef(m_metadataname);
	m_metadataname = name;
	m_metadataname->Ref();

	CallCallbacks(CALLBACKID(CAnmMetadataNode, name_changed), &name);
}

void CAnmMetadataNode::SetReference(String reference)
{
	SafeUnRef(m_reference);
	m_reference = reference;
	m_reference->Ref();

	CallCallbacks(CALLBACKID(CAnmMetadataNode, reference_changed), &reference);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmMetadataNode, name, eValueString, String, GetMetadataName, SetMetadataName);
DEFINE_VALUE(CAnmMetadataNode, reference, eValueString, String, GetReference, SetReference);

DEFINE_METHOD(CAnmMetadataNode, set_name, eValueString, String, SetMetadataName);
DEFINE_METHOD(CAnmMetadataNode, set_reference, eValueString, String, SetReference);

DEFINE_CALLBACK(CAnmMetadataNode, name_changed, eValueString);
DEFINE_CALLBACK(CAnmMetadataNode, reference_changed, eValueString);

// member table
BEGIN_DEFINE_MEMBERS(CAnmMetadataNode)
VALUEID(CAnmMetadataNode, name),
VALUEID(CAnmMetadataNode, reference),

METHODID(CAnmMetadataNode, set_name),
METHODID(CAnmMetadataNode, set_reference),

CALLBACKID(CAnmMetadataNode, name_changed),
CALLBACKID(CAnmMetadataNode, reference_changed),

END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmMetadataNode, CAnmNode);

