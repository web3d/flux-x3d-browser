/********************************************************************************
 * Flux
 *
 * File: anmcylindersensor.h
 * Description: CylinderSensor class
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

#ifndef _anmcylindersensor_h
#define _anmcylindersensor_h

#include "anmpicksensor.h"

#define CYLINDERSENSOR_DEFAULT_AUTOOFFSET		TRUE
#define CYLINDERSENSOR_DEFAULT_DISKANGLE		0.262f
#define CYLINDERSENSOR_DEFAULT_ENABLED			TRUE
#define CYLINDERSENSOR_DEFAULT_MAXANGLE			-1.f
#define CYLINDERSENSOR_DEFAULT_MINANGLE			0.f
#define CYLINDERSENSOR_DEFAULT_OFFSET			0.f


#define	DA_CYLINDER									0
#define	DA_DISK										1

class CAnmCylinderSensor : public CAnmPickSensor
{
protected:

	Boolean					m_autoOffset;
	Float					m_diskAngle;
	Float					m_minAngle;
	Float					m_maxAngle;
	Float					m_offset;

	Float					m_radius;
	Point3					m_hitpoint;
	Point3					m_trackPoint;
	Rotation				m_lastRotation;
	
	float					m_yDisk;
	float					m_PrevAngle;
	float					m_PrevAngle2;
	float					m_InitAngle;
	int					m_DiskAngleMode;
	float					m_LastCylAngle;
	float					m_fsign;

	bool					m_bLastMode;

	void					CalcCylinderDrag(Point3 *pTrackPoint, float radius,
		Point3 from, Point3 to);
	void					CalcRotation(Point3 p1, Point3 p2, Rotation *pRot);
	bool CalcDragAngle( Point3 from, Point3 vBearing, Point3& trackPointOut, float& angleOut );


public:

	// constructor/destructor
	CAnmCylinderSensor();
	virtual ~CAnmCylinderSensor();

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

	virtual void SetDiskAngle(Float diskAngle);
	virtual Float GetDiskAngle() { return m_diskAngle; }
	virtual void GetDiskAngle(Float *pVal)
	{
		if (pVal)
			*pVal = m_diskAngle;
	}

	virtual void SetMinAngle(Float minAngle);
	virtual Float GetMinAngle() { return m_minAngle; }
	virtual void GetMinAngle(Float *pVal)
	{
		if (pVal)
			*pVal = m_minAngle;
	}

	virtual void SetMaxAngle(Float maxAngle);
	virtual Float GetMaxAngle() { return m_maxAngle; }
	virtual void GetMaxAngle(Float *pVal)
	{
		if (pVal)
			*pVal = m_maxAngle;
	}

	virtual void SetOffset(Float offset);
	virtual Float GetOffset() { return m_offset; }
	virtual void GetOffset(Float *pVal)
	{
		if (pVal)
			*pVal = m_offset;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmCylinderSensor);

	DECLARE_VALUE(autoOffset);
	DECLARE_VALUE(diskAngle);
	DECLARE_VALUE(enabled);
	DECLARE_VALUE(minAngle);
	DECLARE_VALUE(maxAngle);
	DECLARE_VALUE(offset);

	DECLARE_METHOD(set_autoOffset);
	DECLARE_METHOD(set_diskAngle);
	DECLARE_METHOD(set_enabled);
	DECLARE_METHOD(set_minAngle);
	DECLARE_METHOD(set_maxAngle);
	DECLARE_METHOD(set_offset);

	DECLARE_CALLBACK(autoOffset_changed);
	DECLARE_CALLBACK(diskAngle_changed);
	DECLARE_CALLBACK(enabled_changed);
	DECLARE_CALLBACK(minAngle_changed);
	DECLARE_CALLBACK(maxAngle_changed);
	DECLARE_CALLBACK(offset_changed);
	DECLARE_CALLBACK(rotation_changed);
	DECLARE_CALLBACK(trackPoint_changed);

};

#endif // _anmcylindersensor_h

