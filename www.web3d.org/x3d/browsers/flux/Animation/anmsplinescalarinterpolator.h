/********************************************************************************
 * Flux
 *
 * File: anmsplinescalarinterpolator.h
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


#ifndef _anmsplinescalarinterpolator_h
#define _anmsplinescalarinterpolator_h

#include "anmnode.h"
#include "anmsplineinterpolator.h"

class CAnmSplineScalarInterpolator : public CAnmNode
{
protected:

	CAnmSplineInterpolator<Float>			m_interp;

public:

	// constructor/destructor
	CAnmSplineScalarInterpolator();
	virtual ~CAnmSplineScalarInterpolator();

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

	virtual void SetKeyVelocity(FloatArray *pKeyVelocity);
	virtual FloatArray *GetKeyVelocity() { return m_interp.GetKeyVelocity(); }
	virtual void GetKeyVelocity(FloatArray **pKeyVelocity) { m_interp.GetKeyVelocity(pKeyVelocity); }

	virtual void SetClosed(Boolean bClosed){ m_interp.SetClosed(bClosed); }
	virtual Boolean	GetClosed() { return m_interp.GetClosed(); }
	virtual void GetClosed(Boolean *pVal){ m_interp.GetClosed( pVal ); }

	virtual void SetNormalizeVelocity(Boolean bNormalizeVelocity){ m_interp.SetNormalizeVelocity(bNormalizeVelocity); }
	virtual Boolean	GetNormalizeVelocity() { return m_interp.GetNormalizeVelocity(); }
	virtual void GetNormalizeVelocity(Boolean *pVal){ m_interp.GetNormalizeVelocity( pVal ); }

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmSplineScalarInterpolator);

	DECLARE_VALUE(key);
	DECLARE_VALUE(keyValue);
	DECLARE_VALUE(keyVelocity);
	DECLARE_VALUE(closed);
	DECLARE_VALUE(normalizeVelocity);

	DECLARE_METHOD(set_fraction);
	DECLARE_METHOD(set_key);
	DECLARE_METHOD(set_keyValue);
	DECLARE_METHOD(set_keyVelocity);
	DECLARE_METHOD(set_closed);
	DECLARE_METHOD(set_normalizeVelocity);

	DECLARE_CALLBACK(key_changed);
	DECLARE_CALLBACK(keyValue_changed);
	DECLARE_CALLBACK(keyVelocity_changed);
	DECLARE_CALLBACK(value_changed);
	DECLARE_CALLBACK(closed_changed);
	DECLARE_CALLBACK(normalizeVelocity_changed);
};

#endif // _anmsplinescalarinterpolator_h

