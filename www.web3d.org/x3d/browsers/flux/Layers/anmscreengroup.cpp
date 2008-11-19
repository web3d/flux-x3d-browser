/********************************************************************************
 * Flux
 *
 * File: anmscreengroup.cpp
 * Description: ScreenGroup node class
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

#include "anmscreengroup.h"
#include "anmupdatevisitor.h"

	
CAnmScreenGroup::CAnmScreenGroup() : CAnmGroup ()
{
}

CAnmScreenGroup::~CAnmScreenGroup()
{
	// anything? just chain to group?
}

void CAnmScreenGroup::Traverse(CAnmUpdateVisitor *pVisitor)
{
	bool oldscreenmode = pVisitor->GetScreenMode();

	pVisitor->SetScreenMode(true);

	// call Group class to traverse children
	CAnmGroup::Traverse(pVisitor);

	pVisitor->SetScreenMode(oldscreenmode);

}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmScreenGroup, bboxCenter, CAnmGroup);
DEFINE_INHERITED_VALUE(CAnmScreenGroup, bboxSize, CAnmGroup);
DEFINE_INHERITED_VALUE(CAnmScreenGroup, children, CAnmGroup);

DEFINE_INHERITED_METHOD(CAnmScreenGroup, addChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmScreenGroup, removeChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmScreenGroup, set_bboxCenter, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmScreenGroup, set_bboxSize, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmScreenGroup, set_children, CAnmGroup);

DEFINE_INHERITED_CALLBACK(CAnmScreenGroup, bboxCenter_changed, CAnmGroup);
DEFINE_INHERITED_CALLBACK(CAnmScreenGroup, bboxSize_changed, CAnmGroup);
DEFINE_INHERITED_CALLBACK(CAnmScreenGroup, children_changed, CAnmGroup);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmScreenGroup)
VALUEID(CAnmScreenGroup, bboxCenter),
VALUEID(CAnmScreenGroup, bboxSize),
VALUEID(CAnmScreenGroup, children),

METHODID(CAnmScreenGroup, addChildren),
METHODID(CAnmScreenGroup, removeChildren),
METHODID(CAnmScreenGroup, set_bboxCenter),
METHODID(CAnmScreenGroup, set_bboxSize),
METHODID(CAnmScreenGroup, set_children),

CALLBACKID(CAnmScreenGroup, bboxCenter_changed),
CALLBACKID(CAnmScreenGroup, bboxSize_changed),
CALLBACKID(CAnmScreenGroup, children_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmScreenGroup, CAnmGroup);

