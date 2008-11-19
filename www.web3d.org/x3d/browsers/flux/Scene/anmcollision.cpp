/********************************************************************************
 * Flux
 *
 * File: anmcollision.cpp
 * Description: Collision node class
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

#include "anmcollision.h"
#include "anmupdatevisitor.h"

CAnmCollision::CAnmCollision() : CAnmGroup (),
	m_collide(ANMCOLLISION_DEFAULT_COLLIDE),
	m_proxy(NULL)
{
}

CAnmCollision::~CAnmCollision()
{
	// anything? just chain to group?
}

void CAnmCollision::Realize()
{
	// create the frame, and chain the realize down to the children
	CAnmGroup::Realize();

	SetStateDirty();
}

void CAnmCollision::Update()
{
	if (StateDirty())
	{
	}
}

void CAnmCollision::Traverse(CAnmUpdateVisitor *pVisitor)
{
	// Make sure to update the proxy if we're re-traversing
	if (m_proxy && TestDirtyBits(ANMGROUPDIRTY_CHECKTRAVERSECHILDREN))
		m_proxy->Traverse(pVisitor);

	// Save old collision setting
	Boolean oldCollide = pVisitor->GetCollide();

	// Set new collision value, but only if it wasn't already turned off
	if (oldCollide && !m_collide)
		pVisitor->SetCollide(false);

	CAnmGroup::Traverse(pVisitor);

	// Restore old collision setting if necessary
	if (oldCollide && !m_collide)
		pVisitor->SetCollide(true);
}

// Accessors
void CAnmCollision::SetCollide(Boolean collide)
{
	m_collide = collide;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmCollision, collide_changed), &collide);
}

void CAnmCollision::SetProxy(CAnmNode *pProxy)
{
	m_proxy = pProxy;
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmCollision, bboxCenter, CAnmGroup);
DEFINE_INHERITED_VALUE(CAnmCollision, bboxSize, CAnmGroup);
DEFINE_INHERITED_VALUE(CAnmCollision, children, CAnmGroup);
DEFINE_VALUE(CAnmCollision, collide, eValueBoolean, Boolean, GetCollide, SetCollide);
DEFINE_VALUE(CAnmCollision, proxy, eValueNode, CAnmNode *, GetProxy, SetProxy);

DEFINE_INHERITED_METHOD(CAnmCollision, addChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmCollision, removeChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmCollision, set_children, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmCollision, set_bboxCenter, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmCollision, set_bboxSize, CAnmGroup);
DEFINE_METHOD(CAnmCollision, set_collide, eValueBoolean, Boolean, SetCollide);

DEFINE_INHERITED_CALLBACK(CAnmCollision, children_changed, CAnmGroup);
DEFINE_INHERITED_CALLBACK(CAnmCollision, bboxCenter_changed, CAnmGroup);
DEFINE_INHERITED_CALLBACK(CAnmCollision, bboxSize_changed, CAnmGroup);
DEFINE_CALLBACK(CAnmCollision, collide_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmCollision, collideTime, eValueTime);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmCollision)
VALUEID(CAnmCollision, bboxCenter),
VALUEID(CAnmCollision, bboxSize),
VALUEID(CAnmCollision, children),
VALUEID(CAnmCollision, collide),
VALUEID(CAnmCollision, proxy),

METHODID(CAnmCollision, addChildren),
METHODID(CAnmCollision, removeChildren),
METHODID(CAnmCollision, set_bboxCenter),
METHODID(CAnmCollision, set_bboxSize),
METHODID(CAnmCollision, set_children),
METHODID(CAnmCollision, set_collide),

CALLBACKID(CAnmCollision, bboxCenter_changed),
CALLBACKID(CAnmCollision, bboxSize_changed),
CALLBACKID(CAnmCollision, children_changed),
CALLBACKID(CAnmCollision, collide_changed),
CALLBACKID(CAnmCollision, collideTime),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmCollision, CAnmGroup);

