/********************************************************************************
 * Flux
 *
 * File: anmlayoutgroup.h
 * Description: LayoutGroup node
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

#ifndef _anmlayoutgroup_h
#define _anmlayoutgroup_h

#include "anmgroup.h"


#define ANMLAYOUTGROUP_DEFAULT_LAYOUT							( new CAnmLayout )



class  CAnmLayoutGroup : public CAnmGroup
{

protected:

	CAnmNode*							m_layout;


	float m_WidthInRealUnits;
	float m_HeightInRealUnits;
	float m_WidthInPixels;
	float m_HeightInPixels;

	float m_scale[2];
	float m_translation[2];

public:

	// constructor/destructor
	CAnmLayoutGroup();
	virtual ~CAnmLayoutGroup();

	// CAnmGroup overrides
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual void Update();								// re-render/reset rendering structures

	virtual void Realize();								// build lower-level rendering structures
	virtual void PostTraverse();

	virtual void ChildChanged(CAnmNode *pChild);
	virtual void ParentChanged(class CAnmGroup *pParent);

	// New methods
	void CAnmLayoutGroup::UpdateLayoutTransform();

	static void CAnmLayoutGroup::LayoutChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, 
											  void *callData, void *userData);

	
	// Accessors
	float GetWidthInRealUnits(){ return m_WidthInRealUnits; }
	float GetHeightInRealUnits(){ return m_HeightInRealUnits; }
	float GetWidthInPixels(){ return m_WidthInPixels; }
	float GetHeightInPixels(){ return m_HeightInPixels; }



	void SetLayout(CAnmNode* pLayout);
	CAnmNode* GetLayout() { return m_layout; }
	void GetLayout(CAnmNode**pVal)
	{
		if (pVal)
			*pVal = m_layout;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmLayoutGroup);

//	DECLARE_VALUE(bboxCenter);
//	DECLARE_VALUE(bboxSize);

	DECLARE_VALUE(layout);
	DECLARE_VALUE(children);


	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);

	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_layout);

	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(layout_changed);
};

DECLARE_GETIMPLEMENTATION(CAnmLayoutGroup);

#endif // _anmlayoutgroup_h
