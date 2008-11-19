/********************************************************************************
 * Flux
 *
 * File: anmplanesensor.h
 * Description: PlaneSensor class
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

#ifndef _anmplanesensor_h
#define _anmplanesensor_h

#include "anmpicksensor.h"

#define PLANESENSOR_DEFAULT_ENABLED			TRUE
#define PLANESENSOR_DEFAULT_AUTOOFFSET		TRUE
#define PLANESENSOR_DEFAULT_MAXPOSITION		Point2(-1, -1)
#define PLANESENSOR_DEFAULT_MINPOSITION		Point2(0, 0)
#define PLANESENSOR_DEFAULT_OFFSET			Point3(0, 0, 0)

class CAnmPlaneSensor : public CAnmPickSensor
{
protected:

	Boolean					m_autoOffset;
	Point2					m_maxPosition;
	Point2					m_minPosition;
	Point3					m_offset;

	Point3					m_hitpoint;
	Point3					m_lastDragPoint;
	Point3					m_lastTranslation;
	eAnmReturnStatus		CalcPlaneDrag(Point3 *pDrag, Point3 from, Point3 to);

public:

	// constructor/destructor
	CAnmPlaneSensor();
	virtual ~CAnmPlaneSensor();

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

	virtual void SetMaxPosition(Point2 maxPosition);
	virtual Point2 GetMaxPosition() { return m_maxPosition; }
	virtual void GetMaxPosition(Point2 *pVal)
	{
		if (pVal)
			*pVal = m_maxPosition;
	}

	virtual void SetMinPosition(Point2 minPosition);
	virtual Point2 GetMinPosition() { return m_minPosition; }
	virtual void GetMinPosition(Point2 *pVal)
	{
		if (pVal)
			*pVal = m_minPosition;
	}

	virtual void SetOffset(Point3 offset);
	virtual Point3 GetOffset() { return m_offset; }
	virtual void GetOffset(Point3 *pVal)
	{
		if (pVal)
			*pVal = m_offset;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmPlaneSensor);

	DECLARE_VALUE(enabled);
	DECLARE_VALUE(autoOffset);
	DECLARE_VALUE(maxPosition);
	DECLARE_VALUE(minPosition);
	DECLARE_VALUE(offset);

	DECLARE_METHOD(set_enabled);
	DECLARE_METHOD(set_autoOffset);
	DECLARE_METHOD(set_maxPosition);
	DECLARE_METHOD(set_minPosition);
	DECLARE_METHOD(set_offset);

	DECLARE_CALLBACK(autoOffset_changed);
	DECLARE_CALLBACK(enabled_changed);
	DECLARE_CALLBACK(maxPosition_changed);
	DECLARE_CALLBACK(minPosition_changed);
	DECLARE_CALLBACK(offset_changed);
	DECLARE_CALLBACK(trackPoint_changed);
	DECLARE_CALLBACK(translation_changed);
	DECLARE_CALLBACK(isActive);

};

#endif // _anmplanesensor_h

