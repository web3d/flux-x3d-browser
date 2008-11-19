/********************************************************************************
 * Flux
 *
 * File: anmgroup.h
 * Description: Grouping node class
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

#ifndef _anmgroup_h
#define _anmgroup_h

#include "anmnodedefs.h"
#include "anmnode.h"

#define ANMGROUP_DEFAULT_BBOXCENTER		Point3(0, 0, 0)
#define ANMGROUP_DEFAULT_BBOXSIZE		Point3(-1, -1, -1)

// Switch support
#define ANMGROUP_DEFAULT_WHICHCHOICE -1

class CAnmGroup : public CAnmNode
{

protected:

	tAnmNodeList		m_children;
	Boolean				m_groupPicking;	// enables picksensor-less picking

	bool				m_notifyChildrenDirty;
	NodeArray			*m_getChildrenReturn;


	Point3				m_bboxCenter;
	Point3				m_bboxSize;

	CAnmBoundingBox		m_bbox;

	tAnmNodeList		m_scopedLights;
	class CAnmLightGroup		*m_lightgroup;

	// Switch support
	int					m_whichChoice;
	bool				m_isSwitch;

	class CAnmRenderGroup *NextRenderGroup(CAnmNodePath &p)
	{
		CAnmRenderGroup *pRenderGroup;
		return NextInstance(pRenderGroup, p);
	}

	class CAnmRenderGroup *CurrentRenderGroup()
	{
		return (class CAnmRenderGroup *) CurrentInstance();
	}

	class CAnmRenderGroup *FindRenderGroup(CAnmNodePath &p)
	{
		return (class CAnmRenderGroup *) FindInstance(p);
	}


	virtual void PropagateDirtyBitsDownward(ULONG dirtyBits);


public:

	// constructor/destructor
	CAnmGroup();
	virtual ~CAnmGroup();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures
	virtual float GetRadius();
	virtual void GetBoundingBox(CAnmBoundingBox *pBoundingBox);
	virtual void GetObjectCenter(Point3 *pCenter);
	virtual long GetPolyCount();
	virtual Boolean IsGroup() { return TRUE; }
	virtual Boolean IsAncestor(CAnmNode *pTestChild);
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual void PreTraverse();
	virtual void PostTraverse();
	virtual void SetMatrixDirty();
	void SetStateAndMatrixDirty();
	virtual void SetChildrenDirty();
	virtual void SetInstanceDirty();

	virtual void SetBaseUrl(String baseUrl);

	// New Methods
	virtual eAnmReturnStatus AddChild(CAnmNode *pChild);
	virtual eAnmReturnStatus RemoveChild(CAnmNode *pChild);
	virtual void AddChildren(NodeArray *pChildren);
	virtual void RemoveChildren(NodeArray *pChildren);
	void RemoveAllChildren();

	virtual void SetChildren(NodeArray *pChildren);
	virtual void GetChildren(NodeArray **ppChildren);

	virtual void ChildChanged(CAnmNode *pChild);
	virtual void CAnmGroup::NotifyChildrenOfChange();

	// Accessors
	tAnmNodeList *Children() { return &m_children; }
	virtual void SetGroupPicking(Boolean groupPicking);
	virtual Boolean GetGroupPicking() { return m_groupPicking; }

	class CAnmLightGroup *GetLightGroup()
	{
		return m_lightgroup;
	}

	virtual void SetBBoxCenter(Point3 p);
	virtual Point3 GetBBoxCenter() { return m_bboxCenter; }
	virtual void GetBBoxCenter(Point3 *pp)
	{
		assert(pp);
		*pp = m_bboxCenter;
	}

	virtual void SetBBoxSize(Point3 p);
	virtual Point3 GetBBoxSize() { return m_bboxSize; }
	virtual void GetBBoxSize(Point3 *pp)
	{
		assert(pp);
		*pp = m_bboxSize;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmGroup);

	DECLARE_VALUE(children);
	DECLARE_VALUE(bboxCenter);
	DECLARE_VALUE(bboxSize);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_bboxCenter);
	DECLARE_METHOD(set_bboxSize);

	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(bboxCenter_changed);
	DECLARE_CALLBACK(bboxSize_changed);
};

// Some definitions for traversal and dirty state
#define ANMGROUPDIRTY_CHECKTRAVERSECHILDREN \
(eAnmNodeMatrixDirty | eAnmNodeChildMatrixDirty | \
eAnmNodeChildrenDirty | eAnmNodeChildChildrenDirty | \
eAnmNodeInstanceDirty | eAnmNodeChildInstanceDirty )

#define ANMGROUPDIRTY_FORCETRAVERSECHILDREN \
(eAnmNodeMatrixDirty | eAnmNodeChildrenDirty | eAnmNodeInstanceDirty)

#define ANMGROUPDIRTY_CHECKPRETRAVERSECHILDREN \
(eAnmNodeStateDirty | eAnmNodeChildStateDirty)

#define ANMGROUP_NDIRTYBITS					(ANMNODE_NDIRTYBITS)

#endif // _anmgroup_h
