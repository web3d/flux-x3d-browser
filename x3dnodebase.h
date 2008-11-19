/********************************************************************************
 * Flux
 *
 * File: x3dnodebase.h
 * Description: Base class for COM node interfaces
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


#ifndef _x3dnodebase_h
#define _x3dnodebase_h

class X3DNodeBase
{
protected :

	class CAnmSAINode *m_node;
	
public :

	X3DNodeBase()
	{
		m_node = NULL;
	}

	void setNativeNode(CAnmSAINode *pNode)
	{
		m_node = pNode;
	}

	CAnmSAINode *getNativeNode()
	{
		return m_node;
	}

};


#endif // _x3dnodebase_h
