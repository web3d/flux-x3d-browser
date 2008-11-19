/********************************************************************************
 * Flux
 *
 * File: anmintegertrigger.h
 * Description: IntegerTrigger class
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

#ifndef _anmintegertrigger_h
#define _anmintegertrigger_h

#include "anmnode.h"

#define ANMINTEGERTRIGGER_DEFAULTINTEGERKEY -1

class CAnmIntegerTrigger : public CAnmNode
{
protected:

	Integer					m_integerKey;

public:

	// constructor/destructor
	CAnmIntegerTrigger();
	virtual ~CAnmIntegerTrigger();

	// CAnmNode overrides
	virtual void Realize();

	// New methods
	virtual void SetBoolean(Boolean b);

	// Accessors
	virtual void SetIntegerKey(Integer keyval);
	virtual Integer GetIntegerKey() { return m_integerKey; }
	virtual void GetIntegerKey(Integer *pKey)
	{
		if (pKey)
			*pKey = m_integerKey;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmIntegerTrigger);

	DECLARE_VALUE(integerKey);

	DECLARE_METHOD(set_boolean);
	DECLARE_METHOD(set_integerKey);

	DECLARE_CALLBACK(integerKey_changed);
	DECLARE_CALLBACK(triggerValue);
};

#endif // _anmintegertrigger_h
