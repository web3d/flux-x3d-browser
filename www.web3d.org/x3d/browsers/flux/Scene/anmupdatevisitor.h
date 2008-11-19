/********************************************************************************
 * Flux
 *
 * File: anmupdatevisitor.h
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

#ifndef _anmupdatevisitor_h
#define _anmupdatevisitor_h

#include "anmnode.h"
#include "anmvisitor.h"

class EXPORT CAnmUpdateVisitor : public CAnmVisitor
{
protected:

	matrix4								 m_worldmatrix;
	Boolean								 m_collide;
	class CAnmRenderGroup				*m_currentgroup;
	class CAnmRenderGraph				*m_rendergraph;
	bool								 m_screenMode;
	matrix4								 m_screenmatrix;
	CAnmNodePath						 m_nodepath;

public:

	// constructor/destructor
	CAnmUpdateVisitor();
	virtual ~CAnmUpdateVisitor();

	// CAnmVisitor overrides
	virtual void Visit(class CAnmNode *pNode);

	// New methods
	virtual void Init(class CAnmRenderGraph *pRenderGraph, class CAnmCamera *pCamera);

	virtual void AddDrawable(class CAnmDrawable *pDrawable);

	virtual class CAnmLightGroup *GetCurrentLightGroup();
	virtual class CAnmLightGroup *GetGlobalLightGroup();

	virtual void PushPath(class CAnmGroup *pGroup);
	virtual void PopPath();
	virtual CAnmNodePath &GetNodePath();

	// Accessors
	virtual void SetWorldMatrix(matrix4 *pMat)
	{
		m_worldmatrix = *pMat;
	}

	virtual void GetWorldMatrix(matrix4 *pMat)
	{
		*pMat = m_worldmatrix;
	}

	virtual void SetCurrentGroup(class CAnmRenderGroup *pCurrentGroup)
	{
		m_currentgroup = pCurrentGroup;
	}

	virtual class CAnmRenderGroup *GetCurrentGroup()
	{
		return m_currentgroup;
	}

	virtual class CAnmRenderGraph *GetRenderGraph()
	{
		return m_rendergraph;
	}

	virtual void SetCollide(Boolean collide)
	{
		m_collide = collide;
	}

	virtual Boolean GetCollide()
	{
		return m_collide;
	}

	virtual void SetScreenMode(bool screenMode)
	{
		m_screenMode = screenMode;
	}

	virtual bool GetScreenMode()
	{
		return m_screenMode;
	}

	virtual void SetScreenMatrix(matrix4 *pMat)
	{
		m_screenmatrix = *pMat;
	}

	virtual void GetScreenMatrix(matrix4 *pMat)
	{
		*pMat = m_screenmatrix;
	}
};

#endif // _anmupdatevisitor_h

