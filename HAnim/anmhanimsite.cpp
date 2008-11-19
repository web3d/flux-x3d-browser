/********************************************************************************
 * Flux
 *
 * File: anmhanimsite.cpp
 * Description: HAnim Site node
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
#include "anmhanimsite.h"
	
CAnmHAnimSite::CAnmHAnimSite() : CAnmTransform()
{
	m_name = new CAnmString(ANMHANIMSITE_DEFAULT_NAME);
}

CAnmHAnimSite::~CAnmHAnimSite()
{
	SafeUnRef(m_name);
}

// Methods

// Accessors
void CAnmHAnimSite::SetName(String name)
{
	SafeUnRef(m_name);
	m_name = name;
	m_name->Ref();

	// I don't think we need this
	// SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmHAnimSite, name_changed), &name);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmHAnimSite, bboxCenter, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimSite, bboxSize, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimSite, center, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimSite, children, CAnmTransform);
DEFINE_VALUE(CAnmHAnimSite, name, eValueString, String, GetName, SetName);
DEFINE_INHERITED_VALUE(CAnmHAnimSite, rotation, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimSite, scale, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimSite, scaleOrientation, CAnmTransform);
DEFINE_INHERITED_VALUE(CAnmHAnimSite, translation, CAnmTransform);

DEFINE_INHERITED_METHOD(CAnmHAnimSite, addChildren, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimSite, removeChildren, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimSite, set_center, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimSite, set_children, CAnmTransform);
DEFINE_METHOD(CAnmHAnimSite, set_name, eValueString, String, SetName);
DEFINE_INHERITED_METHOD(CAnmHAnimSite, set_rotation, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimSite, set_scale, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimSite, set_scaleOrientation, CAnmTransform);
DEFINE_INHERITED_METHOD(CAnmHAnimSite, set_translation, CAnmTransform);

DEFINE_INHERITED_CALLBACK(CAnmHAnimSite, center_changed, CAnmTransform);
DEFINE_INHERITED_CALLBACK(CAnmHAnimSite, children_changed, CAnmTransform);
DEFINE_CALLBACK(CAnmHAnimSite, name_changed, eValueString);
DEFINE_INHERITED_CALLBACK(CAnmHAnimSite, rotation_changed, CAnmTransform);
DEFINE_INHERITED_CALLBACK(CAnmHAnimSite, scale_changed, CAnmTransform);
DEFINE_INHERITED_CALLBACK(CAnmHAnimSite, scaleOrientation_changed, CAnmTransform);
DEFINE_INHERITED_CALLBACK(CAnmHAnimSite, translation_changed, CAnmTransform);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmHAnimSite)
VALUEID(CAnmHAnimSite, bboxCenter),
VALUEID(CAnmHAnimSite, bboxSize),
VALUEID(CAnmHAnimSite, center),
VALUEID(CAnmHAnimSite, children),
VALUEID(CAnmHAnimSite, name),
VALUEID(CAnmHAnimSite, rotation),
VALUEID(CAnmHAnimSite, scale),
VALUEID(CAnmHAnimSite, scaleOrientation),
VALUEID(CAnmHAnimSite, translation),

METHODID(CAnmHAnimSite, addChildren),
METHODID(CAnmHAnimSite, removeChildren),
METHODID(CAnmHAnimSite, set_center),
METHODID(CAnmHAnimSite, set_children),
METHODID(CAnmHAnimSite, set_name),
METHODID(CAnmHAnimSite, set_rotation),
METHODID(CAnmHAnimSite, set_scale),
METHODID(CAnmHAnimSite, set_scaleOrientation),
METHODID(CAnmHAnimSite, set_translation),

CALLBACKID(CAnmHAnimSite, center_changed),
CALLBACKID(CAnmHAnimSite, children_changed),
CALLBACKID(CAnmHAnimSite, name_changed),
CALLBACKID(CAnmHAnimSite, rotation_changed),
CALLBACKID(CAnmHAnimSite, scale_changed),
CALLBACKID(CAnmHAnimSite, scaleOrientation_changed),
CALLBACKID(CAnmHAnimSite, translation_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmHAnimSite, CAnmTransform);

