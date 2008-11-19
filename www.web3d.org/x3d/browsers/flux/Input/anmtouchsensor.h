/********************************************************************************
 * Flux
 *
 * File: anmtouchsensor.h
 * Description: TouchSensor class
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

#ifndef _anmtouchsensor_h
#define _anmtouchsensor_h

#include "anmpicksensor.h"

#define TOUCHSENSOR_DEFAULT_ENABLED			TRUE

class CAnmTouchSensor : public CAnmPickSensor
{
protected:

	Point3				m_hitpoint;

public:

	// constructor/destructor
	CAnmTouchSensor();
	virtual ~CAnmTouchSensor();

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
	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmTouchSensor);

	DECLARE_VALUE(enabled);

	DECLARE_METHOD(set_enabled);

	DECLARE_CALLBACK(enabled_changed);
	DECLARE_CALLBACK(hitNormal_changed);
	DECLARE_CALLBACK(hitPoint_changed);
	DECLARE_CALLBACK(hitTexCoord_changed);
	DECLARE_CALLBACK(isActive);
	DECLARE_CALLBACK(isOver);
	DECLARE_CALLBACK(touchTime);

};

#endif // _anmtouchsensor_h

