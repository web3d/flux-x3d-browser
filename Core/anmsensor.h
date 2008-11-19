/********************************************************************************
 * Flux
 *
 * File: anmsensor.h
 * Description: Sensor base class
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

#ifndef _anmsensor_h
#define _anmsensor_h

#include "anmnode.h"

class CAnmSensor : public CAnmNode
{

protected:

	Boolean			 m_enabled;

	CAnmNode		*m_owner;

	cApplication	*m_app;

public:

	// constructor/destructor
	CAnmSensor(Boolean enabled = TRUE);
	virtual ~CAnmSensor();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level system structures
	virtual class CAnmWorld *GetWorld();
	virtual class CAnmLayer *GetLayer();
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);

	// New methods
	virtual void Poll() PURE;						// subclass *must* implement
	virtual void Enable() { SetEnabled(TRUE); }
	virtual void Disable() { SetEnabled(FALSE); }
	
	// Accessors
	virtual void SetEnabled(Boolean enabled);
	virtual Boolean GetEnabled() { return m_enabled; }
	virtual void GetEnabled(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_enabled;
	}

	virtual void SetOwner(CAnmNode *owner)
	{
		m_owner = owner;
	}

	virtual CAnmNode *GetOwner()
	{
		return m_owner;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmSensor);

	DECLARE_VALUE(enabled);

	DECLARE_METHOD(set_enabled);

	DECLARE_CALLBACK(enabled_changed);
	DECLARE_CALLBACK(isActive);
};

#endif // _anmsensor_h

