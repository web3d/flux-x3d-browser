/********************************************************************************
 * Flux
 *
 * File: anmbooleantoggle.h
 * Description: BooleanToggle class
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

#ifndef _anmbooleantoggle_h
#define _anmbooleantoggle_h

#include "anmnode.h"

#define ANMBOOLEANTOGGLE_DEFAULTTOGGLE		false

class CAnmBooleanToggle : public CAnmNode
{
protected:

	Boolean					m_toggle;

public:

	// constructor/destructor
	CAnmBooleanToggle();
	virtual ~CAnmBooleanToggle();

	// CAnmNode overrides
	virtual void Realize();

	// New methods
	virtual void SetBoolean(Boolean b);

	// Accessors
	virtual void SetToggle(Boolean toggle);
	virtual Boolean GetToggle() { return m_toggle; }
	virtual void GetToggle(Boolean *pToggle)
	{
		if (pToggle)
			*pToggle = m_toggle;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmBooleanToggle);

	DECLARE_VALUE(toggle);

	DECLARE_METHOD(set_boolean);
	DECLARE_METHOD(set_toggle);

	DECLARE_CALLBACK(toggle_changed);
};

#endif // _anmbooleantoggle_h
