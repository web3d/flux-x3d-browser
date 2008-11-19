/********************************************************************************
 * Flux
 *
 * File: anmscreengroup.h
 * Description: ScreenGroup node class
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

#ifndef _anmscreengroup_h
#define _anmscreengroup_h

#include "anmgroup.h"

class  CAnmScreenGroup : public CAnmGroup
{
protected:

public:

	// N.B.: nutty, but for now...
	static BOOL m_drawBoxes;

	// constructor/destructor
	CAnmScreenGroup();
	virtual ~CAnmScreenGroup();

	// CAnmGroup overrides
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmScreenGroup);

	DECLARE_VALUE(bboxCenter);
	DECLARE_VALUE(bboxSize);
	DECLARE_VALUE(children);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_bboxCenter);
	DECLARE_METHOD(set_bboxSize);
	DECLARE_METHOD(set_children);

	DECLARE_CALLBACK(bboxCenter_changed);
	DECLARE_CALLBACK(bboxSize_changed);
	DECLARE_CALLBACK(children_changed);
};


#endif // _anmscreengroup_h
