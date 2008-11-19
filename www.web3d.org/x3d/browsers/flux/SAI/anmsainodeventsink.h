/********************************************************************************
 * Flux
 *
 * File: anmsainodeventsink.h
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

#ifndef _anmsainodeventsink_h
#define _anmsainodeventsink_h

class EXPORT CAnmSAINodeEventSink
{

protected:

	bool										 m_bufferevents;
	std::vector<class CAnmSAINode	*>			 m_changedNodes;

	// Accessors
	void SetEventBuffering(bool eventbuffering)
	{
		m_bufferevents = eventbuffering;
	}

	bool GetEventBuffering()
	{
		return m_bufferevents;
	}

public:

	// constructor/destructor
	CAnmSAINodeEventSink()
	{
		m_bufferevents = true;
	}

	virtual ~CAnmSAINodeEventSink()
	{
	}

	// New methods
	virtual void NotifyNodeChanged(class CAnmSAINode *pNode);
	virtual void CommitNodeChanges();
	virtual void ClearNodeChangedList();
	virtual void FlushBuffer();
	virtual class CAnmNode *GetSinkNode();
};

#endif // _anmsainodeventsink_h
