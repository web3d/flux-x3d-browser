/********************************************************************************
 * Flux
 *
 * File: anmbooleanfilter.h
 * Description: BooleanFilter class
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

#ifndef _anmbooleanfilter_h
#define _anmbooleanfilter_h

#include "anmnode.h"

class CAnmBooleanFilter : public CAnmNode
{
protected:

public:

	// constructor/destructor
	CAnmBooleanFilter();
	virtual ~CAnmBooleanFilter();

	// CAnmNode overrides
	virtual void Realize();

	// New methods
	virtual void SetBoolean(Boolean b);

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmBooleanFilter);

	DECLARE_METHOD(set_boolean);

	DECLARE_CALLBACK(inputFalse);
	DECLARE_CALLBACK(inputNegate);
	DECLARE_CALLBACK(inputTrue);
};

#endif // _anmbooleanfilter_h
