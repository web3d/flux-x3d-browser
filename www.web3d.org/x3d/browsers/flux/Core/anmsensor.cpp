/********************************************************************************
 * Flux
 *
 * File: anmsensor.cpp
 * Description: Sensor base class
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
#include "anmsensor.h"
#include "anmupdatevisitor.h"
#include "anmworld.h"


CAnmSensor::CAnmSensor(Boolean enabled) : CAnmNode()
{
	m_enabled = enabled;
	m_app = NULL;
	m_owner = NULL;
}

CAnmSensor::~CAnmSensor()
{
	if (m_enabled && m_realized && m_app)
		m_app->RemoveSensor(this);
}

void CAnmSensor::Realize()
{
	CAnmWorld *pWorld = GetWorld();

	if (pWorld)
		m_app = pWorld->GetApp();

	if (m_enabled)
	{
		if (m_app)
			m_app->AddSensor(this);
	}

	CAnmNode::Realize();
}


void CAnmSensor::SetEnabled(Boolean enabled)
{
	if (m_enabled == enabled)
		return;

	if (m_realized)
	{
		if (m_app)
		{
			if (enabled)
				m_app->AddSensor(this);
			else
				m_app->RemoveSensor(this);
		}
	}

	m_enabled = enabled;
	
	CallCallbacks(CALLBACKID(CAnmSensor, enabled_changed), &enabled);
}


CAnmWorld *CAnmSensor::GetWorld()
{
	CAnmWorld *pWorld = CAnmNode::GetWorld();
	if (pWorld)
		return pWorld;
	else if (m_owner)
		return m_owner->GetWorld();
	else
		return NULL;
}

CAnmLayer *CAnmSensor::GetLayer()
{
	CAnmLayer *pLayer = CAnmNode::GetLayer();
	if (pLayer)
		return pLayer;
	else if (m_owner)
		return m_owner->GetLayer();
	else
		return NULL;
}

void CAnmSensor::Traverse(CAnmUpdateVisitor *pVisitor)
{
	CAnmRenderObject *pRo;
	// if (TestDirtyBits(eAnmNodeInstanceDirty))
		NextInstance(pRo, pVisitor->GetNodePath());
}


// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmSensor, enabled, eValueBoolean, Boolean, GetEnabled, SetEnabled);

DEFINE_METHOD(CAnmSensor, set_enabled, eValueBoolean, Boolean, SetEnabled);

DEFINE_CALLBACK(CAnmSensor, enabled_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmSensor, isActive, eValueBoolean);

// member table
BEGIN_DEFINE_MEMBERS(CAnmSensor)
VALUEID(CAnmSensor, enabled),

METHODID(CAnmSensor, set_enabled),

CALLBACKID(CAnmSensor, enabled_changed),
CALLBACKID(CAnmSensor, isActive),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmSensor, CAnmNode);
