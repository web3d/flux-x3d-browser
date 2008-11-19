/********************************************************************************
 * Flux
 *
 * File: anmbindablenode.h
 * Description: Bindable node abstract class
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


#ifndef _anmbindablenode_h
#define _anmbindablenode_h

#include "anmnode.h"
#include <stack>


class CAnmBindableNode : public CAnmNode
{

protected:

	Boolean					 m_isBound;

	virtual void NotifyBound(Boolean bindFlag);

	friend class CAnmBindingStack;

public:

	// constructor/destructor
	CAnmBindableNode();
	virtual ~CAnmBindableNode();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level system structures

	// New methods
	virtual void SetBind(Boolean bindFlag);

	// Accessors

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmBindableNode);

	DECLARE_METHOD(set_bind);

	DECLARE_CALLBACK(bindTime);
	DECLARE_CALLBACK(isBound);
};

class CAnmBindingStack
{
protected :
	
	class CAnmClass *pClass;
	std::stack<class CAnmBindableNode *> stk;

public :

	CAnmBindingStack(class CAnmClass *pCls)
	{
		pClass = pCls;
	}

	class CAnmClass *GetClass()
	{
		return pClass;
	}

	void BindNode(CAnmBindableNode *pBindableNode)
	{
		if (!stk.empty())
		{
			CAnmBindableNode *pB = stk.top();
			pB->NotifyBound(false);
		}
		
		stk.push(pBindableNode);
	}

	void TryUnbindNode(CAnmBindableNode *pBindableNode)
	{
		if (!stk.empty())
		{
			CAnmBindableNode *pB = stk.top();
			if (pB == pBindableNode)
				stk.pop();

			if (!stk.empty())
			{
				pB = stk.top();
				pB->NotifyBound(true);
			}
		}
	}

	CAnmBindableNode *GetBoundNode()
	{
		if (!stk.empty())
			return stk.top();
		else
			return NULL;
	}

};


#endif // _anmbindablenode_h

