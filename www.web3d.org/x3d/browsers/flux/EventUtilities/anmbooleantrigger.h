/********************************************************************************
 * Flux
 *
 * File: anmbooleantrigger.h
 * Description: BooleanTrigger class
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

#ifndef _anmbooleantrigger_h
#define _anmbooleantrigger_h

#include "anmnode.h"

class CAnmBooleanTrigger : public CAnmNode
{
protected:

public:

	// constructor/destructor
	CAnmBooleanTrigger();
	virtual ~CAnmBooleanTrigger();

	// CAnmNode overrides
	virtual void Realize();

	// New methods
	virtual void SetTriggerTime(Time triggerTime);

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmBooleanTrigger);

	DECLARE_METHOD(set_triggerTime);

	DECLARE_CALLBACK(triggerTrue);
};

#endif // _anmbooleantrigger_h
