/********************************************************************************
 * Flux
 *
 * File: anmpicksensor.h
 * Description: PickSensor class
 *							base class for mouse-based picking
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

#ifndef _anmpicksensor_h
#define _anmpicksensor_h

#include "anmsensor.h"

#define ANMPICKSENSOR_EPSILON 0.00001f
#define ANMPICKSENSOR_BADPOINTVALUE Point3(ANMPICKSENSOR_EPSILON, ANMPICKSENSOR_EPSILON, ANMPICKSENSOR_EPSILON)
#define ANMPICKSENSOR_BADMOUSEVALUE 10000

class CAnmPickSensor : public CAnmSensor
{
protected:

	Boolean				 m_active;
	Boolean				 m_over;
	int					 m_mousex;
	int					 m_mousey;
	Time				 m_mousedowntime;
	matrix4				 m_localmatrix;


	virtual class CAnmGroup *GetPickParent(class CAnmPickManager *pPickManager, 
		struct sAnmSceneGraphPickDesc *pDesc);


public:

	// constructor/destructor
	CAnmPickSensor(Boolean enabled);
	virtual ~CAnmPickSensor();

	// CAnmSensor overrides
	virtual void Realize();
	virtual void Poll();

	// New methods
	virtual void Update() PURE;

	// Mouse handlers
	// These methods may be overridden by subclasses
	// By default they generate isOver and isActive callbacks at the right times
	virtual void HandleOver(Boolean over);
	virtual void HandleActive(Boolean active, Point3 from, Point3 to, Point3 hitpoint);

	// this is an ABSTRACT base class; sublcass *must* override this one method
	virtual void HandleMove(Point3 from, Point3 to, Point3 hitpoint) PURE;

	// N.B.: eventually need to deal with double-click, I think

	// Cursor management for visual feedback
	// These methods provide defaults, but subclasses should override for custom
	// cursor shapes
	virtual cCursor GetOverCursor();
	virtual cCursor GetActiveCursor();

	// Accessors
	virtual Boolean IsActive()
	{
		return m_active;
	}

	virtual Boolean IsOver()
	{
		return m_over;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmPickSensor);

	DECLARE_CALLBACK(isActive);
	DECLARE_CALLBACK(isOver);
};

#endif // _anmpicksensor_h

