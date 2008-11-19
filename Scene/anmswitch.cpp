/********************************************************************************
 * Flux
 *
 * File: anmswitch.cpp
 * Description: Switch node class
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

#include "anmswitch.h"
#include "anmvisitor.h"
	
CAnmSwitch::CAnmSwitch() : CAnmGroup ()
{
	m_isSwitch = true;
	m_whichChoice = ANMSWITCH_DEFAULT_WHICHCHOICE;
}

CAnmSwitch::~CAnmSwitch()
{

}

void CAnmSwitch::Realize()
{
	// N.B.: anything? just chain to group? maybe we don't want to realize children
	// if they're never switched in
	CAnmGroup::Realize();

	// Make sure we do the activate/deactive bit
	SetStateDirty();
}

float CAnmSwitch::GetRadius()
{
	CAnmNode *pNode = getWhichChild();
	if (pNode)
		return pNode->GetRadius();
	else
		return 0.f;
}

void CAnmSwitch::GetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	CAnmNode *pNode = getWhichChild();
	if (pNode)
		pNode->GetBoundingBox(pBoundingBox);
}

void CAnmSwitch::GetObjectCenter(Point3 *pCenter)
{
	CAnmNode *pNode = getWhichChild();
	if (pNode)
		pNode->GetObjectCenter(pCenter);
}

// helper
CAnmNode *CAnmSwitch::getWhichChild()
{
	int nChildren = m_children.size();
	CAnmNode *pChild = NULL;

	int n = 0;
	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		if (n == m_whichChoice)
		{
			pChild = (*iter);
			break;
		}

		n++;
	}

	return pChild;
}

// Accessors
void CAnmSwitch::SetChoice(NodeArray *pChoice)
{
	SetChildren(pChoice);

	CallCallbacks(CALLBACKID(CAnmSwitch, choice_changed), &pChoice);
}

void CAnmSwitch::SetWhichChoice(int whichChoice)
{
	if (whichChoice != m_whichChoice)
	{
		m_whichChoice = whichChoice;

		SetChildrenDirty();

		CallCallbacks(CALLBACKID(CAnmSwitch, whichChoice_changed), &whichChoice);
	}
}

void CAnmSwitch::Update()
{
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmSwitch, children, CAnmGroup);
DEFINE_VALUE(CAnmSwitch, choice, eValueNodeArray, NodeArray *, GetChoice, SetChoice);
DEFINE_VALUE(CAnmSwitch, whichChoice, eValueInteger, Integer, GetWhichChoice, SetWhichChoice);

DEFINE_INHERITED_METHOD(CAnmSwitch, set_children, CAnmGroup);
DEFINE_METHOD(CAnmSwitch, set_choice, eValueNodeArray, NodeArray *, SetChoice);
DEFINE_METHOD(CAnmSwitch, set_whichChoice, eValueInteger, Integer, SetWhichChoice);

DEFINE_INHERITED_CALLBACK(CAnmSwitch, children_changed, CAnmGroup);
DEFINE_CALLBACK(CAnmSwitch, choice_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmSwitch, whichChoice_changed, eValueInteger);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmSwitch)
VALUEID(CAnmSwitch, children),
VALUEID(CAnmSwitch, choice),
VALUEID(CAnmSwitch, whichChoice),
METHODID(CAnmSwitch, set_children),
METHODID(CAnmSwitch, set_choice),
METHODID(CAnmSwitch, set_whichChoice),
CALLBACKID(CAnmSwitch, children_changed),
CALLBACKID(CAnmSwitch, choice_changed),
CALLBACKID(CAnmSwitch, whichChoice_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmSwitch, CAnmGroup);

