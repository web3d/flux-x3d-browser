/********************************************************************************
 * Flux
 *
 * File: anmswitch.h
 * Description: Switch node class
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

#ifndef _anmswitch_h
#define _anmswitch_h

#include "anmgroup.h"

#define ANMSWITCH_DEFAULT_WHICHCHOICE -1

class  CAnmSwitch : public CAnmGroup
{

protected:

//	int					m_whichChoice;

	virtual CAnmNode *getWhichChild();

public:


	// constructor/destructor
	CAnmSwitch();
	virtual ~CAnmSwitch();

	// CAnmGroup overrides
	virtual void Realize();
	virtual void Update();				// re-render/reset rendering structures
	virtual float GetRadius();
	virtual void GetBoundingBox(CAnmBoundingBox *pBoundingBox);
	virtual void GetObjectCenter(Point3 *pCenter);

	// Accessors
	virtual void SetWhichChoice(int whichChoice);
	virtual int GetWhichChoice()
	{
		return m_whichChoice;
	}
	virtual void GetWhichChoice(int *pVal)
	{
		assert(pVal);
		*pVal = m_whichChoice;
	}

	virtual void SetChoice(NodeArray *pVal);
	virtual void GetChoice(NodeArray **ppVal)
	{
		GetChildren(ppVal);
	}

	// N.B.: not sure whether we surface Choice or not

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmSwitch);

	DECLARE_VALUE(children);
	DECLARE_VALUE(choice);
	DECLARE_VALUE(whichChoice);

	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_choice);
	DECLARE_METHOD(set_whichChoice);

	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(choice_changed);
	DECLARE_CALLBACK(whichChoice_changed);
};


#endif // _anmswitch_h
