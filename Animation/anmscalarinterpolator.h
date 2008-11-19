/********************************************************************************
 * Flux
 *
 * File: anmscalarinterpolator.h
 * Description: Scalar Interpolator class
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


#ifndef _anmscalarinterpolator_h
#define _anmscalarinterpolator_h

#include "anmnode.h"
#include "anminterpolator.h"

class CAnmScalarInterpolator : public CAnmNode
{
protected:

	CAnmInterpolator<Float>		m_interp;

public:

	// constructor/destructor
	CAnmScalarInterpolator();
	virtual ~CAnmScalarInterpolator();

	// CAnmNode overrides
	virtual void Realize();

	// New methods
	virtual void SetFraction(Float fraction);

	// Accessors
	virtual void SetKey(FloatArray *pKey);
	virtual FloatArray *GetKey() { return m_interp.GetKey(); }
	virtual void GetKey(FloatArray **pKey) { m_interp.GetKey(pKey); }

	virtual void SetKeyValue(FloatArray *pKeyValue);
	virtual FloatArray *GetKeyValue() { return m_interp.GetKeyValue(); }
	virtual void GetKeyValue(FloatArray **pKeyValue) { m_interp.GetKeyValue(pKeyValue); }

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmScalarInterpolator);

	DECLARE_VALUE(key);
	DECLARE_VALUE(keyValue);

	DECLARE_METHOD(set_fraction);
	DECLARE_METHOD(set_key);
	DECLARE_METHOD(set_keyValue);

	DECLARE_CALLBACK(key_changed);
	DECLARE_CALLBACK(keyValue_changed);
	DECLARE_CALLBACK(value_changed);

};

#endif // _anmscalarinterpolator_h

