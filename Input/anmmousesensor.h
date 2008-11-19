/********************************************************************************
 * Flux
 *
 * File: anmmousesensor.h
 * Description: MouseSensor class
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

#ifndef _anmmousesensor_h
#define _anmmousesensor_h

#include "anmsensor.h"

#define ANMMOUSESENSOR_DEFAULT_ENABLED TRUE
#define ANMMOUSESENSOR_BAD_POINTVAL (Point2(-1,-1))

class CAnmMouseSensor : public CAnmSensor
{
protected:

	Point2				m_mousepoint;
	bool				m_leftButtonDown;
	bool				m_middleButtonDown;
	bool				m_rightButtonDown;

public:

	// constructor/destructor
	CAnmMouseSensor();
	virtual ~CAnmMouseSensor ();

	// CAnmKeyboardSensor overrides
	virtual void Realize();
	virtual void Update();
	virtual void Poll();
	virtual void SetEnabled(Boolean enabled);

	// New methods
	
	// Accessors
	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmMouseSensor);

	DECLARE_VALUE(enabled);

	DECLARE_METHOD(set_enabled);

	DECLARE_CALLBACK(enabled_changed);

	DECLARE_CALLBACK(position_changed);
	DECLARE_CALLBACK(leftButtonDown);
	DECLARE_CALLBACK(middleButtonDown);
	DECLARE_CALLBACK(rightButtonDown);
};

#endif // _anmmousesensor_h

