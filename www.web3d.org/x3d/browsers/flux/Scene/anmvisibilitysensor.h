/********************************************************************************
 * Flux
 *
 * File: anmvisibilitysensor.h
 * Description: VisibilitySensor class
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

#ifndef _anmvisibilitysensor_h
#define _anmvisibilitysensor_h

#define ANMVISSENSOR_DEFAULT_CENTER	Point3(0, 0, 0)
#define ANMVISSENSOR_DEFAULT_SIZE		Point3(0, 0, 0)
#define ANMVISSENSOR_DEFAULT_ENABLED	TRUE

#include "anmsensor.h"

class CAnmVisibilitySensor : public CAnmSensor
{

protected:

	Point3				 m_center;
	Point3				 m_size;

	Boolean				 m_wasActive;
	Boolean				 m_nowActive;
	matrix4				 m_localMatrix;

	void HandleActive(Boolean isActive);

	class CAnmFrustum *GetFrustum();

public:

	// constructor/destructor
	CAnmVisibilitySensor();
	virtual ~CAnmVisibilitySensor();

	// CAnmSensor overrides
	virtual void Realize();
	virtual void Update();
	virtual void Poll();
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);

	// New methods
	
	// Accessors
	virtual void SetCenter(Point3 center);
	virtual Point3 GetCenter() { return m_center; }
	virtual void GetCenter(Point3 *pVal)
	{
		if (pVal)
			*pVal = m_center;
	}

	virtual void SetSize(Point3 size);
	virtual Point3 GetSize() { return m_size; }
	virtual void GetSize(Point3 *pVal)
	{
		if (pVal)
			*pVal = m_size;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmVisibilitySensor);

	DECLARE_VALUE(center);
	DECLARE_VALUE(enabled);
	DECLARE_VALUE(size);

	DECLARE_METHOD(set_center);
	DECLARE_METHOD(set_enabled);
	DECLARE_METHOD(set_size);

	DECLARE_CALLBACK(center_changed);
	DECLARE_CALLBACK(enabled_changed);
	DECLARE_CALLBACK(size_changed);
	DECLARE_CALLBACK(isActive);
	DECLARE_CALLBACK(enterTime);
	DECLARE_CALLBACK(exitTime);

};

#endif // _anmvisibilitysensor_h

