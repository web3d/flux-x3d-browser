/********************************************************************************
 * Flux
 *
 * File: anmeaseineaseout.h
 * Description: Position Interpolator class
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

#ifndef _anmeaseineaseout_h
#define _anmeaseineaseout_h

#include "anmnode.h"
#include "anmeaseineaseout.h"

class CAnmEaseInEaseOut : public CAnmNode
{
protected:

	FloatArray					*m_key;
	Point2Array					*m_easeInEaseOut;

public:

	// constructor/destructor
	CAnmEaseInEaseOut();
	virtual ~CAnmEaseInEaseOut();

	// CAnmNode overrides
	virtual void Realize();

	// New methods
	virtual void SetFraction(Float fraction);

	// Accessors
	virtual void SetKey(FloatArray *pKey);
	virtual FloatArray *GetKey() { return m_key; }
	virtual void GetKey(FloatArray **pKey) { *pKey = m_key; }

	virtual void SetEaseInEaseOut(Point2Array *pEaseInEaseOut);
	virtual Point2Array *GetEaseInEaseOut() { return m_easeInEaseOut; }
	virtual void GetEaseInEaseOut(Point2Array **pEaseInEaseOut) { *pEaseInEaseOut = m_easeInEaseOut; }


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmEaseInEaseOut);

	DECLARE_VALUE(key);
	DECLARE_VALUE(easeInEaseOut);

	DECLARE_METHOD(set_key);
	DECLARE_METHOD(set_easeInEaseOut);
	DECLARE_METHOD(set_fraction);

	DECLARE_CALLBACK(key_changed);
	DECLARE_CALLBACK(easeInEaseOut_changed);
	DECLARE_CALLBACK(modifiedFraction_changed);
};

#endif // _anmEaseInEaseOut_h

