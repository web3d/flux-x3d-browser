/********************************************************************************
 * Flux
 *
 * File: anmvisitor.cpp
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

#include "stdafx.h"
#include "anmvisitor.h"
#include "anmnode.h"

// constructor/destructor
CAnmVisitor::CAnmVisitor(class CAnmDevice *pDevice,
						 class CAnmCamera *pCamera,
							   BOOL bRender,
							   BOOL bPick,
							   BOOL bSavePath,
							   BOOL bCull,
							   BOOL bCollide
							   )
 : m_dorender(bRender),
   m_dopick(bPick),
   m_savepath(bSavePath),
   m_cullobjects(bCull),
   m_testcollisions(bCollide)
{
	m_device = pDevice;
	m_camera = pCamera;
	// N.B.: should ref/unref device, right?
}

void CAnmVisitor::Traverse(CAnmNode *pNode)
{
	// pNode->Traverse(this);
}

