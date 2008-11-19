/********************************************************************************
 * Flux
 *
 * File: anmpointlight.h
 * Description: PointLight class
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

#ifndef _anmpointlight_h
#define _anmpointlight_h

#include "anmlight.h"

#define ANMPOINTLIGHT_DEFAULT_ATTENUATION			Point3(1, 0, 0)
#define ANMPOINTLIGHT_DEFAULT_LOCATION				Point3(0, 0, 0)
#define ANMPOINTLIGHT_DEFAULT_RADIUS				100.f

class CAnmPointLight : public CAnmLight
{

protected:

	Point3			m_attenuation;
	Float			m_radius;

	// CAnmLight overrides
	virtual void CopyValuesToInstance(CAnmLightInstance *pLightInstance);
	virtual void TransformInstance(CAnmLightInstance *pLightInstance, matrix4 &wmat);

public:


	// Constructor/Destructor
	CAnmPointLight();
	virtual ~CAnmPointLight();

	// Node class overrides
	virtual void Realize();				// build lower-level rendering structures

	// New methods

	// Accessors
	void SetAttenuation(Point3 attenuation);
	Point3 GetAttenuation() { return m_attenuation; }
	void GetAttenuation(Point3 *pp)
	{
		assert(pp);
		*pp = m_attenuation;
	}


	void SetRadius(Float radius);
	Float GetRadius() { return m_radius; }
	void GetRadius(Float *pVal)
	{
		assert(pVal);
		*pVal = m_radius;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmPointLight);

	DECLARE_VALUE(ambientIntensity);
	DECLARE_VALUE(color);
	DECLARE_VALUE(direction);
	DECLARE_VALUE(intensity);
	DECLARE_VALUE(on);
	DECLARE_VALUE(attenuation);
	DECLARE_VALUE(location);
	DECLARE_VALUE(radius);

	DECLARE_METHOD(set_ambientIntensity);
	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_direction);
	DECLARE_METHOD(set_intensity);
	DECLARE_METHOD(set_location);
	DECLARE_METHOD(set_on);
	DECLARE_METHOD(set_attenuation);
	DECLARE_METHOD(set_radius);

	DECLARE_CALLBACK(ambientIntensity_changed);
	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(direction_changed);
	DECLARE_CALLBACK(intensity_changed);
	DECLARE_CALLBACK(location_changed);
	DECLARE_CALLBACK(on_changed);
	DECLARE_CALLBACK(attenuation_changed);
	DECLARE_CALLBACK(radius_changed);
};


#endif // _anmdirectionallight_h
