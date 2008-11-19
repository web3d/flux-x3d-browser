/********************************************************************************
 * Flux
 *
 * File: anmsainode.h
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

#ifndef _anmsainode_h
#define _anmsainode_h

class EXPORT CAnmSAINode : public CAnmObject
{
protected:

	// pointer to the real node
	class CAnmNode				*m_node;
	class CAnmClass				*m_nodeclass;
	class CAnmSAINodeEventSink	*m_eventsink;

	// list of fields
	std::vector<class CAnmField *>	m_fields;
	std::vector<class CAnmField *>	m_changedFields;

	// Creation helper
	void InitFromClass(class CAnmClass *pClass);
	void HandleFieldChanged(class CAnmField *pField);
	static void FieldChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData);
	void CommitChange(class CAnmNode *pNode, class CAnmField *pField);

public:

	// Constructor/destructor
	CAnmSAINode();
	CAnmSAINode(class CAnmNode *pNode);
	CAnmSAINode(class CAnmClass *pClass);
	virtual ~CAnmSAINode();

	// New methods

	// Retrieve a (cached) field object by name or member id
	class CAnmField *GetField(const char *fieldname);
	class CAnmField *GetField(CLASSMEMBERID mid);

	// Grab latest values from real node
	void UpdateValues();

	// Commit changes from any changed fields to real node
	void ClearChanges();
	void CommitChanges(bool clear);

	// Attached to owning script node

	// Accessors
	class CAnmNode *GetNode()
	{
		return m_node;
	}

	void SetSink(class CAnmSAINodeEventSink *pEventSink)
	{
		m_eventsink = pEventSink;
	}

	class CAnmSAINodeEventSink *GetSink()
	{
		return m_eventsink;
	}

	DECLARE_CLASS(CAnmSAINode);
};


#endif // _anmsainode_h
