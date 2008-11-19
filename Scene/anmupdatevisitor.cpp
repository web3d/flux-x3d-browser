/********************************************************************************
 * Flux
 *
 * File: anmupdatevisitor.cpp
 * Description: Update Visitor class
 *							Traverses scene graph to update render graph
 *
 *						See anmvisitor.h for details on Visitor
 *						paradigm.
 *						
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
#include "anmupdatevisitor.h"
#include "anmnode.h"
#include "anmgroup.h"

// constructor/destructor
CAnmUpdateVisitor::CAnmUpdateVisitor()
 : CAnmVisitor(NULL, NULL)
{
	m_currentgroup = NULL;
	m_rendergraph  = NULL;
	m_collide = false;
	m_screenMode = false;
}

CAnmUpdateVisitor::~CAnmUpdateVisitor()
{
}

void CAnmUpdateVisitor::Init(CAnmRenderGraph *pRenderGraph, CAnmCamera *pCamera)
{
	m_worldmatrix = matrix4::Identity;
	m_currentgroup = pRenderGraph;
	m_rendergraph  = pRenderGraph;
	m_camera = pCamera;
	m_collide = false;
	m_screenMode = false;
	m_screenmatrix = matrix4::Identity;
}

void CAnmUpdateVisitor::Visit(CAnmNode *pNode)
{
}

void CAnmUpdateVisitor::AddDrawable(CAnmDrawable *pDrawable)
{
	if (m_currentgroup)
	{
		// add the thing
		m_currentgroup->AddChild(pDrawable);
	}
}

CAnmLightGroup *CAnmUpdateVisitor::GetCurrentLightGroup()
{
	CAnmGroup *pGroup = (CAnmGroup *) m_currentgroup->GetUserData();
	assert(pGroup);

	return pGroup->GetLightGroup();
}

CAnmLightGroup *CAnmUpdateVisitor::GetGlobalLightGroup()
{
	return m_rendergraph->GetGlobalLightGroup();
}

void CAnmUpdateVisitor::PushPath(class CAnmGroup *pGroup)
{
	m_nodepath.Push(pGroup);
}

void CAnmUpdateVisitor::PopPath()
{
	m_nodepath.Pop();
}

CAnmNodePath &CAnmUpdateVisitor::GetNodePath()
{
	return m_nodepath;
}
