/********************************************************************************
 * Flux
 *
 * File: anmsainode.cpp
 * Description: Node wrapper class for SAI
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
#include "anmsainode.h"
#include "anmfields.h"
#include "anmnode.h"
#include "anmscript.h"

CAnmSAINode::CAnmSAINode()
{
	m_node = NULL;
	m_nodeclass = NULL;
	m_eventsink = NULL;
}

CAnmSAINode::CAnmSAINode(CAnmNode *pNode)
{
	m_node = pNode;

	if (m_node)
	{
		m_node->Ref();

		m_nodeclass = NULL;
		m_eventsink = NULL;

		CAnmClass *pClass = pNode->GetClass();
		InitFromClass(pClass);
	}
}

CAnmSAINode::CAnmSAINode(CAnmClass *pClass)
{
	m_node = NULL;
	m_nodeclass = NULL;
	m_eventsink = NULL;

	if (pClass)
	{
		m_node = (CAnmNode *) (*pClass->GetCreateObject()) ();
		InitFromClass(pClass);
	}
}

CAnmSAINode::~CAnmSAINode()
{
	for (int i = 0; i < m_fields.size(); i++)
	{
		SafeUnRef(m_fields[i]);
	}

	m_fields.clear();

	SafeUnRef(m_node);
}

void CAnmSAINode::InitFromClass(class CAnmClass *pClass)
{
	assert(pClass != NULL);
	m_nodeclass = pClass;

	// initialize field list
	for (int i = 0; i < pClass->m_numMembers; i++)
	{
		CAnmField *pF = CAnmField::CreateField(m_node, pClass->m_memberTable[i]);
		pF->AddCallback(CALLBACKID(CAnmField, value_changed), FieldChangedCallback, this);
		m_fields.push_back(pF);
	}
}

// Retrieve a (cached) field object by name or member id
CAnmField *CAnmSAINode::GetField(const char *fieldname)
{
	CLASSMEMBERID mid = m_nodeclass->FindClassMember((char *) fieldname);

	if (mid)
		return GetField(mid);
	else
		return NULL;
}

CAnmField *CAnmSAINode::GetField(CLASSMEMBERID mid)
{
	assert(mid);

	// N.B.: linear search; there's gotta be a better way
	for (int i = 0; i < m_fields.size(); i++)
	{
		if (m_fields[i]->GetMemberID() == mid)
		{
			CAnmField *pF = m_fields[i];

			if (mid->memberType == eAnmValueMember)
			{
				// Good time to make sure the node is up to date
				CommitChanges(false);
#if 0

				void *pData = pF->GetData();
				m_node->GetValue(mid, (void **) pData);
#else
				pF->UpdateFromNode();
#endif
			}
			return pF;
		}
	}

	return NULL;
}

// Commit changes from any changed fields to real node
void CAnmSAINode::ClearChanges()
{
	m_changedFields.clear();
}

void CAnmSAINode::CommitChange(CAnmNode *pNode, CAnmField *pField)
{
	CLASSMEMBERID mid = pField->GetMemberID();

	if (mid->memberType == eAnmValueMember || mid->memberType == eAnmInitMember)
	{
		pNode->SetValue(mid, pField->GetData());
	}
	else if (mid->memberType == eAnmMethodMember)
	{
		pNode->CallMethod(mid, pField->GetData());
	}
	else
	{
		assert (mid->memberType == eAnmCallbackMember);
		pNode->CallCallbacks(mid, pField->GetData());
	}
}

void CAnmSAINode::CommitChanges(bool clear)
{
	Lock();
	for (int i = 0; i < m_changedFields.size(); i++)
	{
		CAnmField *pF = m_changedFields[i];
		CAnmNode *pNode = pF->GetNode();

		CommitChange(pNode, pF);
	}

	if (clear)
	{
		m_changedFields.clear();
	}

	Unlock();
}

void CAnmSAINode::FieldChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData)
{
	CAnmField *pField = (CAnmField *) sourceObject;
	CAnmSAINode *pNode = (CAnmSAINode *) userData;
	Boolean *pOver =  (Boolean *) callData;

	pNode->HandleFieldChanged(pField);
}

void CAnmSAINode::HandleFieldChanged(CAnmField *pField)
{
	Lock();

	CAnmNode *pContainingNode = NULL;
	if (m_eventsink)
		pContainingNode = m_eventsink->GetSinkNode();

	// N.B.: First, test to see if this is an "internal" node change i.e. ->
	// it's a field value, not a callback, on our own containing node. ->
	// if so, don't add this to our list
	CLASSMEMBERID mid = pField->GetMemberID();
	if ((pContainingNode == m_node))// &&
		// (mid->memberType == eAnmValueMember || mid->memberType == eAnmInitMember))
	{
		CommitChange(pContainingNode, pField);
	}
	else
	{
		// N.B.: this is really stupid; put it on a linked list and insert-if or hash or something instead
		BOOL found = FALSE;
		for (int i = 0; i < m_changedFields.size(); i++)
		{
			if (m_changedFields[i] == pField)
			{
				found = TRUE;
				break;
			}
		}

		if (!found)
		{
			m_changedFields.push_back(pField);
		}

		if (m_eventsink)
			m_eventsink->NotifyNodeChanged(this);
		else
			assert(FALSE);
	}

	Unlock();

}

void CAnmSAINode::UpdateValues()
{
	if (m_nodeclass && m_node)
	{
		int sz = m_fields.size();

		for (int i = 0; i < sz; i++)
		{
			CLASSMEMBERID mid = m_fields[i]->GetMemberID();

			// grab the value out of the node and save to temp field
			CAnmField *pF = CAnmField::CreateField(m_node, mid);

			// copy the value to my cached field
			m_fields[i]->Copy(pF->GetData());

			// dump the temp field
			pF->UnRef();
		}
	}
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_ABSTRACT_EMPTY_CLASS(CAnmSAINode, CAnmObject)
