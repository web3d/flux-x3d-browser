/********************************************************************************
 * Flux
 *
 * File: anmspheresensor.h
 * Description: SphereSensor class
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

#ifndef _anmspheresensor_h
#define _anmspheresensor_h

#include "anmpicksensor.h"

#define SPHERESENSOR_DEFAULT_ENABLED		TRUE
#define SPHERESENSOR_DEFAULT_AUTOOFFSET		TRUE
#define SPHERESENSOR_DEFAULT_OFFSET			Rotation(0, 1, 0, 0)

class CAnmSphereSensor : public CAnmPickSensor
{
protected:

	Boolean					m_autoOffset;
	Rotation				m_offset;

	Float					m_radius;
	Point3					m_hitpoint;
	Point3					m_trackPoint;
	Rotation				m_lastRotation;
	
	Point3					m_vBearing;

	int						m_InitMousex;
	int						m_InitMousey;

public:

	// constructor/destructor
	CAnmSphereSensor();
	virtual ~CAnmSphereSensor();

	// CAnmPickSensor overrides
	virtual void Realize();
	virtual void Update();
	virtual cCursor GetOverCursor();
	virtual cCursor GetActiveCursor();

	// overrides to handle input state based on mouse up/down and move
	virtual void HandleActive(Boolean active, Point3 from, Point3 to, Point3 hitpoint);
	virtual void HandleMove(Point3 from, Point3 to, Point3 hitpoint);

	// New methods
	
	// Accessors

	virtual void SetAutoOffset(Boolean autoOffset);
	virtual Boolean GetAutoOffset() { return m_autoOffset; }
	virtual void GetAutoOffset(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_autoOffset;
	}

	virtual void SetOffset(Rotation offset);
	virtual Rotation GetOffset() { return m_offset; }
	virtual void GetOffset(Rotation *pVal)
	{
		if (pVal)
			*pVal = m_offset;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmSphereSensor);

	DECLARE_VALUE(autoOffset);
	DECLARE_VALUE(enabled);
	DECLARE_VALUE(offset);

	DECLARE_METHOD(set_autoOffset);
	DECLARE_METHOD(set_enabled);
	DECLARE_METHOD(set_offset);

	DECLARE_CALLBACK(autoOffset_changed);
	DECLARE_CALLBACK(enabled_changed);
	DECLARE_CALLBACK(offset_changed);
	DECLARE_CALLBACK(rotation_changed);
	DECLARE_CALLBACK(trackPoint_changed);
	DECLARE_CALLBACK(isActive);

};

#endif // _anmspheresensor_h

