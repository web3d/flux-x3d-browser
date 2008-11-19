/********************************************************************************
 * Flux
 *
 * File: anmspotlight.h
 * Description: SpotLight class
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

#ifndef _anmspotlight_h
#define _anmspotlight_h

#include "anmlight.h"

#define ANMSPOTLIGHT_DEFAULT_ATTENUATION			Point3(1.f, 0.f, 0.f)
#define ANMSPOTLIGHT_DEFAULT_BEAMWIDTH				(PI/2.f)
#define ANMSPOTLIGHT_DEFAULT_CUTOFFANGLE			(PI/4.f)
#define ANMSPOTLIGHT_DEFAULT_LOCATION				Point3(0.f, 0.f, 0.f)
#define ANMSPOTLIGHT_DEFAULT_RADIUS					100.f
#define ANMSPOTLIGHT_MAX_ANGLE						(PI/2.f)

class CAnmSpotLight : public CAnmLight
{
protected:

	Point3			m_attenuation;
	Float			m_beamWidth;
	Float			m_cutOffAngle;
	Point3			m_location;
	Float			m_radius;

	// CAnmLight overrides
	virtual void CopyValuesToInstance(CAnmLightInstance *pLightInstance);
	virtual void TransformInstance(CAnmLightInstance *pLightInstance, matrix4 &wmat);

public:


	// Constructor/Destructor
	CAnmSpotLight();
	virtual ~CAnmSpotLight();

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


	void SetBeamWidth(Float beamWidth);
	Float GetBeamWidth() { return m_beamWidth; }
	void GetBeamWidth(Float *pVal)
	{
		assert(pVal);
		*pVal = m_beamWidth;
	}

	void SetCutOffAngle(Float cutOffAngle);
	Float GetCutOffAngle() { return m_cutOffAngle; }
	void GetCutOffAngle(Float *pVal)
	{
		assert(pVal);
		*pVal = m_cutOffAngle;
	}

	void SetRadius(Float radius);
	Float GetRadius() { return m_radius; }
	void GetRadius(Float *pVal)
	{
		assert(pVal);
		*pVal = m_radius;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmSpotLight);

	DECLARE_VALUE(ambientIntensity);
	DECLARE_VALUE(beamWidth);
	DECLARE_VALUE(color);
	DECLARE_VALUE(cutOffAngle);
	DECLARE_VALUE(direction);
	DECLARE_VALUE(intensity);
	DECLARE_VALUE(on);
	DECLARE_VALUE(attenuation);
	DECLARE_VALUE(location);
	DECLARE_VALUE(radius);

	DECLARE_METHOD(set_ambientIntensity);
	DECLARE_METHOD(set_beamWidth);
	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_cutOffAngle);
	DECLARE_METHOD(set_direction);
	DECLARE_METHOD(set_intensity);
	DECLARE_METHOD(set_on);
	DECLARE_METHOD(set_attenuation);
	DECLARE_METHOD(set_location);
	DECLARE_METHOD(set_radius);

	DECLARE_CALLBACK(ambientIntensity_changed);
	DECLARE_CALLBACK(beamWidth_changed);
	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(cutOffAngle_changed);
	DECLARE_CALLBACK(direction_changed);
	DECLARE_CALLBACK(intensity_changed);
	DECLARE_CALLBACK(on_changed);
	DECLARE_CALLBACK(attenuation_changed);
	DECLARE_CALLBACK(location_changed);
	DECLARE_CALLBACK(radius_changed);
};


#endif // _anmspotlight_h

