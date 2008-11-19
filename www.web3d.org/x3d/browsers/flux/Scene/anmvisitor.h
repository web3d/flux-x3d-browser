/********************************************************************************
 * Flux
 *
 * File: anmvisitor.h
 * Description: Visitor class
 *              Manages scene graph traversal
 *
 *              This is a modified version of the Visitor
 *              design pattern. Typically, the scene graph
 *              is a dumb dispatcher to visitors that have
 *              all the knowledge; in ours, the visitor class
 *              may provide information on rendering, picking,
 *              paths, culling and collision that the
 *              scene graph uses to save and restore state
 *              during traversal. Our visitor scheme can be
 *              used vanilla: if a derived class of Visitor
 *              does not supply any those values, save/restore
 *              operations are not performed; the node simply
 *              does the dispatch to the visitor.
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

#ifndef _anmvisitor_h
#define _anmvisitor_h

#include "anmobject.h"

class CAnmVisitor : public CAnmObject
{
protected:

	class CAnmDevice					*m_device;
	class CAnmCamera					*m_camera;
	BOOL								 m_dorender;		// are we rendering?
	BOOL								 m_dopick;			// are we picking?
	BOOL								 m_savepath;		// save path to node?
	BOOL								 m_cullobjects;		// are we doing visibility culling?
	BOOL								 m_testcollisions;	// are we doing collision testing?

public:

	// constructor/destructor
	CAnmVisitor(class CAnmDevice *pDevice,
				class CAnmCamera *pCamera,
							BOOL bRender = FALSE,
							BOOL bPick = FALSE,
							BOOL bSavePath = FALSE,
							BOOL bCull = FALSE,
							BOOL bCollide = FALSE);


	virtual ~CAnmVisitor()
	{
	}

	// New methods
	virtual void Visit(class CAnmNode *pNode) PURE;
	virtual void Traverse(class CAnmNode *pNode);

	// Accessors
	class CAnmDevice *GetDevice()
	{
		return m_device;
	}

	class CAnmCamera *GetCamera()
	{
		return m_camera;
	}

	void SetRendering(BOOL bRender)
	{
		m_dorender = bRender;
	}

	BOOL GetRendering()
	{
		return m_dorender;
	}
	
	void SetPicking(BOOL bPick)
	{
		m_dopick = bPick;
	}

	BOOL GetPicking()
	{
		return m_dopick;
	}

	void SetSavePath(BOOL bSavePath)
	{
		m_savepath = bSavePath;
	}

	BOOL GetSavePath()
	{
		return m_savepath;
	}

	
	void SetCulling(BOOL bCull)
	{
		m_cullobjects = bCull;
	}

	BOOL GetCulling()
	{
		return m_cullobjects;
	}

	void SetCollision(BOOL bCollide)
	{
		m_testcollisions = bCollide;
	}

	BOOL GetCollision()
	{
		return m_testcollisions;
	}


};

#endif // _anmvisitor_h

