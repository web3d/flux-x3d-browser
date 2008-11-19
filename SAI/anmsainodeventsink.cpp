/********************************************************************************
 * Flux
 *
 * File: anmsainodeventsink.cpp
 * Description: SAI Node Event Sink class
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
#include "anmsainodeventsink.h"
#include "anmsainode.h"

void CAnmSAINodeEventSink::NotifyNodeChanged(CAnmSAINode *pNode)
{
#if 0
	if (!m_bufferevents)
	{
		pNode->CommitChanges();
		return;
	}
#endif

	// N.B.: this is really stupid; put it on a linked list and insert-if or hash or something instead
	BOOL found = FALSE;
	int sz = m_changedNodes.size();
	for (int i = 0; i < sz; i++)
	{
		if (m_changedNodes[i] == pNode)
		{
			found = TRUE;
			break;
		}
	}

	if (!found)
	{
		m_changedNodes.push_back(pNode);
		// Make sure it doesn't get garbage collected or anything -- TP 12/22/04
		pNode->Ref();
	}
}

void CAnmSAINodeEventSink::CommitNodeChanges()
{
	int sz = m_changedNodes.size();
	for (int i = 0; i < sz; i++)
	{
		CAnmSAINode *pSAINode = m_changedNodes[i];
		pSAINode->CommitChanges(true);
		pSAINode->UnRef();
	}
}

void CAnmSAINodeEventSink::ClearNodeChangedList()
{
	m_changedNodes.clear();
}

void CAnmSAINodeEventSink::FlushBuffer()
{
	CommitNodeChanges();
	ClearNodeChangedList();
}


class CAnmNode *CAnmSAINodeEventSink::GetSinkNode()
{
	return NULL;
}

