/********************************************************************************
 * Flux
 *
 * File: anmcollision.h
 * Description: Collision node class
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

#ifndef _anmcollision_h
#define _anmcollision_h

#include "anmgroup.h"

#define ANMCOLLISION_DEFAULT_COLLIDE		TRUE

class  CAnmCollision : public CAnmGroup
{

protected:

	Boolean				 m_collide;
	CAnmNode			*m_proxy;

public:

	// constructor/destructor
	CAnmCollision();
	virtual ~CAnmCollision();

	// CAnmGroup overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();								// re-render/reset rendering structures
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);

	// New methods

	
	// Accessors

	virtual void SetCollide(Boolean collide);
	virtual void GetCollide(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_collide;
	}

	virtual void SetProxy(class CAnmNode *pProxy);
	virtual void GetProxy(CAnmNode **pVal)
	{
		assert(pVal);
		*pVal = m_proxy;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmCollision);

	DECLARE_VALUE(bboxCenter);
	DECLARE_VALUE(bboxSize);
	DECLARE_VALUE(children);
	DECLARE_VALUE(collide);
	DECLARE_VALUE(proxy);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_bboxCenter);
	DECLARE_METHOD(set_bboxSize);
	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_collide);

	DECLARE_CALLBACK(bboxCenter_changed);
	DECLARE_CALLBACK(bboxSize_changed);
	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(collide_changed);
	DECLARE_CALLBACK(collideTime);
};


#endif // _anmcollision_h
