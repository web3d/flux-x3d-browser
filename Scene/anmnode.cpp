/********************************************************************************
 * Flux
 *
 * File: anmnode.cpp
 * Description: Base node class
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

#include "anmnode.h"
#include "anmcallback.h"
#include "anmevent.h"
#include "anmmetadatanode.h"
#include "anmgroup.h"
#include "anmtransform.h"
#include "anmvisitor.h"

CAnmNode::CAnmNode()
{
	m_parents.clear();
	m_realized = FALSE;
	m_active = true;
	m_eventQueue.clear();

	m_instanceIndex = 0;
	m_dirtyBits = 0;
	m_name = new CAnmString("");
	m_metadata = NULL;
}

CAnmNode::~CAnmNode()
{
	int sz = m_instances.size();
	for (int i = 0; i < sz; i++)
		SafeDelete(m_instances[i]);

	SafeUnRef(m_name);
	SafeUnRef(m_metadata);
}

void CAnmNode::Realize()
{
	m_realized = TRUE;

	SetInstanceDirty();
}

void CAnmNode::AddParent(CAnmGroup *pParent)
{
	m_parents.push_back(pParent);
	SetInstanceDirty();
}

void CAnmNode::RemoveParent(CAnmGroup *pParent)
{
	m_parents.remove(pParent);
	SetInstanceDirty();
}

void CAnmNode::NotifyParentsOfChange()
{

	std::list< CAnmGroup* >::iterator iter;
	for( 
		iter = m_parents.begin(); 
		iter != m_parents.end();
		iter++ )
	{
		(*iter)->ChildChanged(this);
	}
}


void CAnmNode::ParentChanged(CAnmGroup *pParent)
{
}

CAnmGroup *CAnmNode::GetFirstParent()
{
	std::list< CAnmGroup* >::iterator iter;
	for( 
		iter = m_parents.begin(); 
		iter != m_parents.end();
		iter++ )
	{
		// just grab the first one
		return *iter;
	}

	return NULL;
}

CAnmTransform *CAnmNode::GetFirstTransformParent()
{
	CAnmGroup *pGroup = GetFirstParent();

	if (pGroup)
	{
		if (pGroup->GetClass()->IsA(GETCLASS(CAnmTransform)))
			return (CAnmTransform *) pGroup;
		else
			return pGroup->GetFirstTransformParent();
	}
	else
		return NULL;
}

Boolean CAnmNode::IsDescendant(class CAnmGroup *pTestParent)
{
	Boolean found = FALSE;

	std::list< CAnmGroup* >::iterator iter;
	for( 
		iter = m_parents.begin(); 
		iter != m_parents.end();
		iter++ )
	{
		if ((*iter) == pTestParent)
		{
			found = TRUE;
			break;
		}
	}

	if (!found)
	{
		std::list< CAnmGroup* >::iterator iter;
		for( 
			iter = m_parents.begin(); 
			iter != m_parents.end();
			iter++ )
		{
			if ((*iter)->IsDescendant(pTestParent))
			{
				found = TRUE;
				break;
			}
		}
	}

	return found;
}

void CAnmNode::Traverse(CAnmUpdateVisitor *pVisitor)
{
}

void CAnmNode::PreTraverse()
{
	m_instanceIndex = 0;
	if (StateDirty())
		Update();
}

void CAnmNode::PostTraverse()
{
	ClearAllDirtyBits();
}

void CAnmNode::Update()
{
	ClearStateDirty();
}

CAnmWorld *CAnmNode::GetWorld()
{
#if 1
	// any way up the tree is fine, as long as we ultimately get to the root
	CAnmGroup *pParent = GetFirstParent();

	if (pParent)
		return pParent->GetWorld();
	else
		return NULL;
#else
	// N.B.: trying something new TP 9/23/03
	std::list< CAnmGroup* >::iterator iter;
	for( 
		iter = m_parents.begin(); 
		iter != m_parents.end();
		iter++ )
	{
		CAnmGroup *pParent = (*iter);
		if (pParent)
		{
			CAnmWorld *pWorld = pParent->GetWorld();
			if (pWorld)
				return pWorld;
		}
	}

	return NULL;
#endif
}

CAnmProto *CAnmNode::GetProtoContext()
{
	// any way up the tree is fine, as long as we ultimately get to the root
	CAnmGroup *pParent = GetFirstParent();

	if (pParent)
		return pParent->GetProtoContext();
	else
		return NULL;
}

CAnmScene *CAnmNode::GetContainingScene()
{
	// any way up the tree is fine, as long as we ultimately get to the root
	CAnmGroup *pParent = GetFirstParent();

	if (pParent)
		return pParent->GetContainingScene();
	else
		return NULL;
}

CAnmLayer *CAnmNode::GetLayer()
{
	// any way up the tree is fine, as long as we ultimately get to the root
	CAnmGroup *pParent = GetFirstParent();

	if (pParent)
		return pParent->GetLayer();
	else
		return NULL;
}

// event support

void CAnmNode::PostEvent(CAnmEvent *pEvent)
{
	m_eventQueue.push_back(pEvent);
}

BOOL CAnmNode::HandleEvents()
{
	CAnmEvent *pEvent;

	Boolean hadEvents = FALSE;
	int nEvents = 0;

	while (!m_eventQueue.empty())
	{
		pEvent = m_eventQueue.front();
		m_eventQueue.pop_front();

		HandleOneEvent(pEvent);

		hadEvents = TRUE;
		nEvents++;
	}

	return hadEvents;
}

eAnmReturnStatus CAnmNode::HandleOneEvent(CAnmEvent *pEvent)
{
	// N.B.: we only know how to handle events for ourselves; non-this source objects
	// must be dealt with in subclass who know what they're looking for
	if (pEvent->GetSourceObject() == this)
	{
		CallMethod(pEvent->GetReason(), pEvent->GetData());
	}

	return eAnmReturnAllGood;
}

void CAnmNode::PropagateDirtyBitsUpward(ULONG dirtyBits)
{
	m_dirtyBits |= dirtyBits;
	
	std::list< CAnmGroup* >::iterator iter;
	for( 
		iter = m_parents.begin(); 
		iter != m_parents.end();
		iter++ )
	{
		(*iter)->PropagateDirtyBitsUpward(dirtyBits);
	}
}

void CAnmNode::PropagateDirtyBitsDownward(ULONG dirtyBits)
{
	m_dirtyBits |= dirtyBits;
}

void CAnmNode::SetMatrixDirty()
{
	SetDirtyBits(eAnmNodeMatrixDirty);
	PropagateDirtyBitsUpward(eAnmNodeChildMatrixDirty);
}

void CAnmNode::SetStateDirty()
{
	SetDirtyBits(eAnmNodeStateDirty);
	PropagateDirtyBitsUpward(eAnmNodeChildStateDirty);
}

void CAnmNode::SetStateAndMatrixDirty()
{
	SetDirtyBits(eAnmNodeStateDirty | eAnmNodeMatrixDirty);
	PropagateDirtyBitsUpward(eAnmNodeChildStateDirty | eAnmNodeChildMatrixDirty);
}

void CAnmNode::SetInstanceDirty()
{
	SetDirtyBits(eAnmNodeInstanceDirty);
	PropagateDirtyBitsUpward(eAnmNodeChildInstanceDirty);
}

void CAnmNode::SetActive(bool active)
{
	m_active = active;
	SetInstanceDirty();
}

// Accessors
void CAnmNode::SetMetadata(CAnmNode *pMetadata)
{
	CAnmNode *pImp = NULL;
	if (pMetadata)
		pImp = pMetadata->GetImplementationNode();

	if (!pImp || pImp->GetClass()->IsA(GETCLASS(CAnmMetadataNode)))
	{
		SafeUnRef(m_metadata);

		m_metadata = pMetadata;
		if (pMetadata)
			pMetadata->Ref();

		CallCallbacks(CALLBACKID(CAnmNode, metadata_changed), &m_metadata);
	}
	else
	{
		// N.B.: should probably warn about this...
	}
	
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmNode, metadata, eValueNode, CAnmNode*, GetMetadata, SetMetadata);
DEFINE_METHOD(CAnmNode, set_metadata, eValueNode, CAnmNode*, SetMetadata);
DEFINE_CALLBACK(CAnmNode, metadata_changed, eValueNode);

// member table
BEGIN_DEFINE_MEMBERS(CAnmNode)
VALUEID(CAnmNode, metadata),
METHODID(CAnmNode, set_metadata),
CALLBACKID(CAnmNode, metadata_changed),

END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmNode, CAnmObject)


