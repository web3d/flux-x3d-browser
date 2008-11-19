/********************************************************************************
 * Flux
 *
 * File: anmmetadatadouble.h
 * Description: MetadataDouble class
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


#ifndef _anmmetadatadouble_h
#define _anmmetadatadouble_h

#include "anmmetadatanode.h"

class CAnmMetadataDouble : public CAnmMetadataNode
{
protected:

	DoubleArray				*m_value;

public:

	// constructor/destructor
	CAnmMetadataDouble();

	virtual ~CAnmMetadataDouble();

	// Accessors
	void SetValue(DoubleArray *pValue);
	DoubleArray *GetValue()
	{
		return m_value;
	}
	void GetValue(DoubleArray **pValue)
	{
		assert(pValue);
		*pValue = m_value;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmMetadataDouble);

	DECLARE_VALUE(name);
	DECLARE_VALUE(reference);
	DECLARE_VALUE(value);

	DECLARE_METHOD(set_name);
	DECLARE_METHOD(set_reference);
	DECLARE_METHOD(set_value);

	DECLARE_CALLBACK(name_changed);
	DECLARE_CALLBACK(reference_changed);
	DECLARE_CALLBACK(value_changed);

};

#endif // _anmmetadatadouble_h

