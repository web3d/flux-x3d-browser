/********************************************************************************
 * Flux
 *
 * File: anmgroup.cpp
 * Description: Grouping node class
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

#include "anmgroup.h"
#include "anmdirectionallight.h"
#include "anmpicksensor.h"
#include "anmrenderobject.h"
#include "anmvisitor.h"
#include "anmupdatevisitor.h"
#include "anmworld.h"



CAnmGroup::CAnmGroup() : CAnmNode(),
m_bboxCenter(ANMGROUP_DEFAULT_BBOXCENTER),
m_bboxSize(ANMGROUP_DEFAULT_BBOXSIZE),
m_whichChoice(ANMGROUP_DEFAULT_WHICHCHOICE)
{
	m_groupPicking = FALSE;
	m_notifyChildrenDirty = true;

	m_getChildrenReturn = new NodeArray;

	m_lightgroup = new CAnmLightGroup;

	m_isSwitch = false;
}

CAnmGroup::~CAnmGroup()
{
	// do we need to destroy anything?
	// this is where we get into the age-old problem of:
	// who is in charge of destroying when there are two object trees reflecting each other?

	RemoveAllChildren();

	SafeUnRef(m_getChildrenReturn);

	SafeDelete(m_lightgroup);
}


eAnmReturnStatus CAnmGroup::AddChild(CAnmNode *pChild)
{
	if (pChild == NULL)
	{
		TRACE(_T("Tried to AddChild with NULL node; might be ok\n"));
		return eAnmReturnAllGood;
	}
	if (pChild == this)
	{
		TRACE(_T("Tried to AddChild with Self.\n"));
		return eAnmReturnAllGood;
	}

	pChild->AddParent(this);
	pChild->Ref();

	m_children.push_back(pChild);

	if (m_realized && !pChild->Realized())
		pChild->Realize();

	// save any lights on our list
	CAnmDirectionalLight *pImp = GETIMPLEMENTATION(CAnmDirectionalLight, pChild);
	if (pImp)
	{
		pImp->Ref();
		m_scopedLights.push_back(pImp);
	}

	if (m_notifyChildrenDirty)
		SetChildrenDirty();

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmGroup::RemoveChild(CAnmNode *pChild)
{
	if (pChild == NULL)
		return eAnmReturnAllGood;

	bool bFound = false;
	// first make sure that child is in there.
	//
	std::list< CAnmNode* >::iterator iter;

	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		if ( (*iter) == pChild ) {
			bFound = true;
			break;
		}
	}
	if( bFound ) {

		// save any lights on our list
		CAnmDirectionalLight *pImp = GETIMPLEMENTATION(CAnmDirectionalLight, pChild);
		if (pImp)
		{
			m_scopedLights.remove(pImp);
			pImp->UnRef();
		}

		m_children.remove(pChild);
		pChild->RemoveParent(this);

		pChild->UnRef();

		if (m_notifyChildrenDirty) {
			SetChildrenDirty();
		}

	}
	return eAnmReturnAllGood;
}

void CAnmGroup::AddChildren(NodeArray *pChildren)
{
	int nChildren = pChildren->size();

	// N.B.: turn dirty notification of children off; we'll do it once at the end
	// otherwise it's an N^^2 thing
	m_notifyChildrenDirty = false;
	for (int i = 0; i < nChildren; i++)
	{


#ifdef DEBUG
		
		// krv:
		// Make sure this is not here already
		// Debug Only!!!
		// first make sure that child is in there.
		//
		std::list< CAnmNode* >::iterator iter;

		int iiii=0;
		for( 
			iter = m_children.begin(); 
			iter != m_children.end();
			iter++ , iiii++ )
		{
			if ( (*iter) == (*pChildren)[i] ) {
				assert( (*iter) != (*pChildren)[i] );
			}
		}

#endif

		AddChild((*pChildren)[i]);
	}
	m_notifyChildrenDirty = true;

	SetChildrenDirty();
}

void CAnmGroup::RemoveChildren(NodeArray *pChildren)
{

	int nChildren = pChildren->size();


	// N.B.: turn dirty notification of children off; we'll do it once at the end
	// otherwise it's an N^^2 thing
	m_notifyChildrenDirty = false;
	for (int i = 0; i < nChildren; i++)
	{
		RemoveChild((*pChildren)[i]);
	}
	m_notifyChildrenDirty = true;

	SetChildrenDirty();
}

void CAnmGroup::SetChildren(NodeArray *pChildren)
{
	RemoveAllChildren();

	int nChildren = pChildren->size();

	// N.B.: turn dirty notification of children off; we'll do it once at the end
	// otherwise it's an N^^2 thing
	m_notifyChildrenDirty = false;
	for (int i = 0; i < nChildren; i++)
	{
		AddChild((*pChildren)[i]);
	}
	m_notifyChildrenDirty = true;

	SetChildrenDirty();

	// CallCallbacks(CALLBACKID(CAnmGroup, children_changed), &pChildren);

}

void CAnmGroup::GetChildren(NodeArray **ppChildren)
{
	// N.B.: memory?

	SafeUnRef(m_getChildrenReturn);
	m_getChildrenReturn = new NodeArray;

	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		m_getChildrenReturn->push_back((*iter));
	}

	m_getChildrenReturn->Ref();

	*ppChildren = m_getChildrenReturn;
}

void CAnmGroup::RemoveAllChildren()
{

	int nChildren = m_children.size();

	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		(*iter)->RemoveParent(this);
		(*iter)->UnRef();
	}

	m_children.clear();

	SetChildrenDirty();

}

void CAnmGroup::ChildChanged(CAnmNode *pChild)
{
}

void CAnmGroup::SetGroupPicking(Boolean groupPicking)
{
	m_groupPicking = groupPicking;
}

Boolean CAnmGroup::IsAncestor(CAnmNode *pTestChild)
{
	Boolean found = FALSE;

	// breadth-first search
	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		if ((*iter) == pTestChild)
		{
			found = TRUE;
			break;
		}
	}

	if (!found)
	{
		for( 
			iter = m_children.begin(); 
			iter != m_children.end();
			iter++ )
		{
			if ((*iter)->IsAncestor(pTestChild))
			{
				found = TRUE;
				break;
			}
		}
	}

	return found;
}

void CAnmGroup::Realize()
{
	CAnmNode::Realize();

	// make sure the children are realized

	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		if (!(*iter)->Realized())
		{
			(*iter)->Realize();
		}
	}
}

void CAnmGroup::Update()
{
	CAnmNode::Update();
}

float CAnmGroup::GetRadius()
{
	CAnmBoundingBox b;
	if (!m_bbox.IsValid())
		GetBoundingBox(&b);

	return m_bbox.GetRadius();
}

void CAnmGroup::GetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	if (m_bboxSize.x != -1 && m_bboxSize.y != -1 && m_bboxSize.z != -1)
	{
		m_bbox.SetCenterSize(m_bboxCenter, m_bboxSize);
	}
	else
	{
		CAnmBoundingBox b;
		BOOL oncethru = FALSE;
		std::list< CAnmNode* >::iterator iter;

		for( 
			iter = m_children.begin(); 
			iter != m_children.end();
			iter++ )
		{

			// Magic here; only do groups or shape nodes (which are groups)
			// shape node will do its geometry child, the only thing
			// that actually has a bounding box
			if ((*iter)->GetClass()->IsA(GETCLASS(CAnmGroup)))
			{
				(*iter)->GetBoundingBox(&b);

				CAnmNode *pNode = (*iter);
				if (b.IsValid())
				{
					if (!oncethru)
					{
						m_bbox = b;
						oncethru = TRUE;
					}
					else
					{
						m_bbox.Merge(b);
					}
				}
			}
		}
	}

	*pBoundingBox = m_bbox;
}


void CAnmGroup::GetObjectCenter(Point3 *pCenter)
{
	CAnmBoundingBox b;
	if (!m_bbox.IsValid())
		GetBoundingBox(&b);

	Point3 size;
	m_bbox.GetCenterSize(pCenter, &size);
}

long CAnmGroup::GetPolyCount()
{
	long pcount = 0;

	std::list< CAnmNode* >::iterator iter;

	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		pcount += (*iter)->GetPolyCount();
	}

	return pcount;
}

void CAnmGroup::Traverse(CAnmUpdateVisitor *pVisitor)
{
	if (!m_realized)
		return;

	CAnmRenderGroup * pOldGroup = pVisitor->GetCurrentGroup();
	CAnmRenderGroup * pCurGroup = NULL;
	
	if (TestDirtyBits(eAnmNodeInstanceDirty))
		pCurGroup = NextRenderGroup(pVisitor->GetNodePath());
	else
		pCurGroup = FindRenderGroup(pVisitor->GetNodePath());

	// N.B.: WATCH THIS CAREFULLY!
	if (pCurGroup == NULL)
		return;

	assert(pCurGroup);

	// Set collision state
	pCurGroup->SetCollide(pVisitor->GetCollide());

	// Add to the parent render group's child list if I'm dirty
	if (TestDirtyBits(eAnmNodeInstanceDirty))
	{
		pOldGroup->AddChild(pCurGroup);
	}


	// N.B.: traverse my children if either we have to recalc matrices and instances for
	// everyone, or because some child has requested updated matrices
	if (TestDirtyBits(ANMGROUPDIRTY_CHECKTRAVERSECHILDREN))
	{
		// Push group state
		pVisitor->SetCurrentGroup(pCurGroup);
		pVisitor->PushPath(this);

		// Rebuild the hierarchy if requested
		if (TestDirtyBits(eAnmNodeChildrenDirty))
		{
			pCurGroup->ClearChildren();

			// Make sure my lighting group is correct
			CAnmGroup *pOldGroupNode = (CAnmGroup *) pOldGroup->GetUserData();
			assert(pOldGroupNode);

			// N.B.: this prevents a nasty recursion because the old group node is ->
			// the same as the world at top level, and we'd be adding a light group to itself...
			if (pOldGroupNode != this)
			{
				m_lightgroup->ClearLights();
				m_lightgroup->SetParentGroup(pOldGroupNode->GetLightGroup());
			}

		}

		// If we have a valid bounding box, use it
		if (TestDirtyBits(eAnmNodeMatrixDirty) && m_bbox.IsValid())
		{
			matrix4 mat;
			pVisitor->GetWorldMatrix(&mat);
			CAnmBoundingBox b = m_bbox.Transform(mat);
			pCurGroup->SetBoundingBox(&b);
		}

		std::list< CAnmNode* >::iterator iter;

		// Traverse the children
		// if we have a dirty matrix or child list, traverse all children;
		// otherwise traverse only them whut asked fer it
		int childIndex = 0;
		for( 
			iter = m_children.begin(); 
			iter != m_children.end();
			iter++, childIndex++ )
		{
			// Skip deactivated nodes, e.g. in a Switch or LOD
#if 0
				if (!(*iter)->GetActive())
				continue;
#else
				if (m_isSwitch && (childIndex != m_whichChoice))
					continue;
#endif

			if (TestDirtyBits(ANMGROUPDIRTY_FORCETRAVERSECHILDREN))
				(*iter)->Traverse(pVisitor);
			else if ((*iter)->TestDirtyBits(ANMGROUPDIRTY_CHECKTRAVERSECHILDREN))
				(*iter)->Traverse(pVisitor);

			
		}

			// krv: dopnt clear here, clear in PostTraverse
			//
//		ClearDirtyBits(eAnmNodeChildrenDirty);

		// pop group state
		pVisitor->SetCurrentGroup(pOldGroup);
		pVisitor->PopPath();

	}
}

void CAnmGroup::PreTraverse()
{
	if (!m_realized)
		return;

	CAnmNode::PreTraverse();

	if (TestDirtyBits(ANMGROUPDIRTY_CHECKPRETRAVERSECHILDREN | ANMGROUPDIRTY_CHECKTRAVERSECHILDREN))
	{
		// Clear out all my light instances if we're rebuilding the
		// hierarchy; the lights will re-add as we traverse
		if (TestDirtyBits(eAnmNodeChildrenDirty))
			m_lightgroup->ClearLights();

		std::list< CAnmNode* >::iterator iter;

		for( 
			iter = m_children.begin(); 
			iter != m_children.end();
			iter++ )
		{
			if (TestDirtyBits(ANMGROUPDIRTY_FORCETRAVERSECHILDREN))
				(*iter)->PreTraverse();
			else if ((*iter)->TestDirtyBits((ANMGROUPDIRTY_CHECKPRETRAVERSECHILDREN | ANMGROUPDIRTY_CHECKTRAVERSECHILDREN)))
				(*iter)->PreTraverse();
		}

		ClearDirtyBits(ANMGROUPDIRTY_CHECKPRETRAVERSECHILDREN);
	}

}

void CAnmGroup::PostTraverse()
{
	if (!m_realized)
		return;

	// Clear out all traversal flags
	if (TestDirtyBits( ANMGROUPDIRTY_CHECKTRAVERSECHILDREN | eAnmNodeChildrenDirty ) )
	{
		std::list< CAnmNode* >::iterator iter;

		for( 
			iter = m_children.begin(); 
			iter != m_children.end();
			iter++ )
		{
			if (TestDirtyBits(ANMGROUPDIRTY_FORCETRAVERSECHILDREN))
				(*iter)->PostTraverse();
			else if ((*iter)->TestDirtyBits(ANMGROUPDIRTY_CHECKTRAVERSECHILDREN | eAnmNodeChildrenDirty ))
				(*iter)->PostTraverse();
		}

		ClearDirtyBits(ANMGROUPDIRTY_CHECKTRAVERSECHILDREN);
		ClearDirtyBits(eAnmNodeChildrenDirty);
	}

	CAnmNode::PostTraverse();

}

void CAnmGroup::PropagateDirtyBitsDownward(ULONG dirtyBits)
{
	m_dirtyBits |= dirtyBits;
	
	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		(*iter)->PropagateDirtyBitsDownward(dirtyBits);
	}
}

void CAnmGroup::SetMatrixDirty()
{
	// Signal my matrix is dirty
	SetDirtyBits(eAnmNodeMatrixDirty);

	// Tell all my parents
	PropagateDirtyBitsUpward(eAnmNodeChildMatrixDirty);

	// Tell all my children to recalc their matrices, too
	PropagateDirtyBitsDownward(eAnmNodeMatrixDirty);
}

void CAnmGroup::SetStateAndMatrixDirty()
{
	// Signal my state and matrix are dirty
	SetDirtyBits(eAnmNodeStateDirty | eAnmNodeMatrixDirty);

	// Tell all my parents
	PropagateDirtyBitsUpward(eAnmNodeChildStateDirty | eAnmNodeChildMatrixDirty);

	// Tell all my children to recalc their matrices, too (state not needed)
	PropagateDirtyBitsDownward(eAnmNodeMatrixDirty);
}


void CAnmGroup::SetChildrenDirty()
{
	// Signal my child list is dirty
	SetDirtyBits(eAnmNodeChildrenDirty);

	// Tell all my parents to come find me
	PropagateDirtyBitsUpward(eAnmNodeChildChildrenDirty);

	// Tell all my children to re-init their instances, too
	PropagateDirtyBitsDownward(eAnmNodeChildrenDirty);
	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		(*iter)->SetInstanceDirty();
	}
}


void CAnmGroup::SetInstanceDirty()
{
	// Signal my child list is dirty
	SetDirtyBits(eAnmNodeInstanceDirty);

	// Tell all my parents to come find me
	PropagateDirtyBitsUpward(eAnmNodeChildInstanceDirty);

	// Tell all my children to re-init their instances, too
	PropagateDirtyBitsDownward(eAnmNodeInstanceDirty);
}

void CAnmGroup::SetBaseUrl(String baseUrl)
{
	CAnmNode::SetBaseUrl(baseUrl);

	std::list< CAnmNode* >::iterator iter;

	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		(*iter)->SetBaseUrl(baseUrl);
	}
}


void CAnmGroup::SetBBoxCenter(point3 bboxCenter)
{
	m_bboxCenter = bboxCenter;

	SetMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmGroup, bboxCenter_changed), &bboxCenter);
}

void CAnmGroup::SetBBoxSize(point3 bboxSize)
{
	m_bboxSize = bboxSize;

	SetMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmGroup, bboxSize_changed), &bboxSize);
}


void CAnmGroup::NotifyChildrenOfChange()
{

	std::list< CAnmNode* >::iterator iter;
	for( 
		iter = m_children.begin(); 
		iter != m_children.end();
		iter++ )
	{
		(*iter)->ParentChanged(this);
	}
}




// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmGroup, children, eValueNodeArray, NodeArray*, GetChildren, SetChildren);
DEFINE_VALUE(CAnmGroup, bboxCenter, eValuePoint3, Point3, GetBBoxCenter, SetBBoxCenter);
DEFINE_VALUE(CAnmGroup, bboxSize, eValuePoint3, Point3, GetBBoxSize, SetBBoxSize);

DEFINE_METHOD(CAnmGroup, addChildren, eValueNodeArray, NodeArray*, AddChildren);
DEFINE_METHOD(CAnmGroup, removeChildren, eValueNodeArray, NodeArray*, RemoveChildren);
DEFINE_METHOD(CAnmGroup, set_children, eValueNodeArray, NodeArray*, SetChildren);
DEFINE_METHOD(CAnmGroup, set_bboxCenter, eValuePoint3, Point3, SetBBoxCenter);
DEFINE_METHOD(CAnmGroup, set_bboxSize, eValuePoint3, Point3, SetBBoxSize);

DEFINE_CALLBACK(CAnmGroup, children_changed, eValueNodeArray);
DEFINE_CALLBACK(CAnmGroup, bboxCenter_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmGroup, bboxSize_changed, eValuePoint3);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmGroup)
VALUEID(CAnmGroup, children),
VALUEID(CAnmGroup, bboxCenter),
VALUEID(CAnmGroup, bboxSize),
METHODID(CAnmGroup, addChildren),
METHODID(CAnmGroup, removeChildren),
METHODID(CAnmGroup, set_children),
METHODID(CAnmGroup, set_bboxCenter),
METHODID(CAnmGroup, set_bboxSize),
CALLBACKID(CAnmGroup, children_changed),
CALLBACKID(CAnmGroup, bboxCenter_changed),
CALLBACKID(CAnmGroup, bboxSize_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmGroup, CAnmNode)


