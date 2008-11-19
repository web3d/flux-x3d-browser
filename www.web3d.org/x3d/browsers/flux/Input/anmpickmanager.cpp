/********************************************************************************
 * Flux
 *
 * File: anmpickmanager.cpp
 * Description: Pick Manager class
 *							manage list of mouse-based pick objects
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
#include "anmpickmanager.h"
#include "anmdevice.h"
#include "anmviewport.h"
#include "anmcamera.h"
#include "../animaapp.h"
#include "anmpicksensor.h"
#include "anmgroup.h"
#include "anmanchor.h"
#include "anmworld.h"

CAnmPickManager::CAnmPickManager(class cApplication *pApp) : CAnmObject(),
m_app(pApp)
{
	Init(-1, -1, ANMPICKSENSOR_BADPOINTVALUE, ANMPICKSENSOR_BADPOINTVALUE);
}

void CAnmPickManager::Init(int mousex, int mousey, Point3 from, Point3 to)
{
	// clear out old picklist
	m_picklist.Clear();

	m_lookedforgroup = FALSE;
	m_pickgroup = NULL;
	m_mousex = mousex;
	m_mousey = mousey;
	m_pickfrom = from;
	m_pickto = to;
}

CAnmPickManager::~CAnmPickManager()
{
	m_picklist.Clear();
}


// LookForGroup - find innermost enclosing Group node for the picked geometry that
//		has picking device sensors
// Each simulation tick our list is cleared by the owning application;
// when a node's TestPick method succeeds, it adds to our list. We only do this group
// search once because it's potentially costly
// N.B.: this is purely for X3D-like picking device sensor semantics, where
// the sensors to activate are siblings or aunts of the frontmost picked geometry

void CAnmPickManager::LookForGroup()
{
	// If we've already been here, get out
	if (m_lookedforgroup)
		return;

	// If nothing was picked, get out
	if (m_picklist.pickList.size() == 0)
		return;

	// Get the frontmost picked geometry
	sAnmPickDesc<CAnmNode, CAnmGroup> pickDesc = m_picklist.pickList[0];

	// Find its innermost enclosing Group node that has a picking sensor child,
	// or picking switched on for itself
	m_pickgroup = NULL;
	for (int j = pickDesc.nParents - 1; j >= 0 ; j--)
	{
		CAnmGroup *pGroup = pickDesc.pParentArray[j];
		std::list<CAnmNode *> *pChildren = pGroup->Children();

		std::list< CAnmNode* >::iterator iter;
		for( 
			iter = pChildren->begin(); 
			iter != pChildren->end();
			iter++ )
		{
			CAnmNode *pChild = (*iter);
			CAnmNode *pImp = pChild->GetImplementationNode();
			if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmPickSensor)))
			{
				CAnmPickSensor *pPickSensor = (CAnmPickSensor *) pImp;
				if (pPickSensor->GetEnabled())
					m_pickgroup = pGroup;
			}

			if (m_pickgroup)
				break;
		}

		// No child sensors? How about the group itself?
		if (!m_pickgroup && pGroup->GetGroupPicking())
		{
			m_pickgroup = pGroup;
		}

		// outer loop break;
		if (m_pickgroup)
			break;

	}

	m_lookedforgroup = TRUE;
}


// TestPick() - see if a Group hierarchy has any objects under the mouse

BOOL CAnmPickManager::TestPick(CAnmGroup *pGroup, sAnmPickDesc<CAnmNode,CAnmGroup> *pDesc)
{
	// N.B.: we only do this once per simulation tick
	if (!m_lookedforgroup)
		LookForGroup();

	if (m_pickgroup == pGroup)
	{
		*pDesc = m_picklist.pickList[0];
		return TRUE;
	}
	else
		return FALSE;

}

void sAnmPickList<CAnmNode, CAnmGroup>::Clear()
{
	for (int i = 0; i < pickList.size(); i++)
	{
		sAnmPickDesc<CAnmNode, CAnmGroup> pickDesc = pickList[i];

		if (pickDesc.pLeaf)
			pickDesc.pLeaf->UnRef();

		for (int j = 0; j < pickDesc.nParents; j++)
		{
			pickDesc.pParentArray[j]->UnRef();
		}

		if (pickDesc.pParentArray)
			delete pickDesc.pParentArray;
	}
	pickList.clear();
}

void sAnmPickList<CAnmNode, CAnmGroup>::Clear1(sAnmPickDesc<CAnmNode, CAnmGroup> &pickDesc)
{
	if (pickDesc.pLeaf)
		pickDesc.pLeaf->UnRef();

	for (int j = 0; j < pickDesc.nParents; j++)
	{
		pickDesc.pParentArray[j]->UnRef();
	}

	if (pickDesc.pParentArray)
		delete pickDesc.pParentArray;
}
