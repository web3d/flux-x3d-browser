/********************************************************************************
 * Flux
 *
 * File: anmlight.h
 * Description: Base light class
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

#ifndef _anmlight_h
#define _anmlight_h

#include "anmnode.h"

#define ANMLIGHT_DEFAULT_AMBIENTINTENSITY 0.f
#define ANMLIGHT_DEFAULT_COLOR Color(1, 1, 1)
#define ANMLIGHT_DEFAULT_DIRECTION Point3(0, 0, -1)		// RH coords
#define ANMLIGHT_DEFAULT_LOCATION Point3(0, 0, 0)
#define ANMLIGHT_DEFAULT_INTENSITY 1.f
#define ANMLIGHT_DEFAULT_ON TRUE

class CAnmLight : public CAnmNode
{

protected:

	eAnmLightType	m_lightType;

	Float			m_ambientIntensity;
	Color			m_color;
	Point3			m_direction;
	Point3			m_location;
	Float			m_intensity;
	Boolean			m_on;

	static bool m_showLights;

	class CAnmLightInstance *NextLightInstance(CAnmNodePath &p)
	{
		CAnmLightInstance *pLightInstance;
		return NextInstance(pLightInstance, p);
	}

	class CAnmLightInstance *GetLightInstance(int index)
	{
		return (class CAnmLightInstance *) GetInstance(index);
	}

	class CAnmLightInstance *FindLightInstance(CAnmNodePath &p)
	{
		return (class CAnmLightInstance *) FindInstance(p);
	}

	virtual void CopyToInstances();
	virtual void CopyValuesToInstance(class CAnmLightInstance *pLightInstance);
	virtual void TransformInstance(class CAnmLightInstance *pLightInstance, matrix4 &wmat);

public:


	// Constructor/Destructor
	CAnmLight(eAnmLightType lightType = eAnmLightBadType,
		Color color = ANMLIGHT_DEFAULT_COLOR ,
		Point3 direction = ANMLIGHT_DEFAULT_DIRECTION,
		Point3 location = ANMLIGHT_DEFAULT_LOCATION,
		Float intensity = ANMLIGHT_DEFAULT_INTENSITY,
		Boolean on = ANMLIGHT_DEFAULT_ON,
		Float ambientIntensity = ANMLIGHT_DEFAULT_AMBIENTINTENSITY);

	virtual ~CAnmLight();

	// Node class overrides
	virtual void Realize();		// build lower-level rendering structures
	virtual void Update();		// re-render/reset rendering structures
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);

	
	// New methods
	static void SetShowLights(bool showLights);
	static bool GetShowLights();

	// Accessors
	void SetAmbientIntensity(Float ambientIntensity);
	Float GetAmbientIntensity() { return m_ambientIntensity; }
	void GetAmbientIntensity(Float *pVal)
	{
		assert(pVal);
		*pVal = m_ambientIntensity;
	}

	void SetColor(Color color);
	Color GetColor() { return m_color; }
	void GetColor(Color *pVal)
	{
		assert(pVal);
		*pVal = m_color;
	}

	void SetDirection(Point3 direction);
	Point3 GetDirection() { return m_direction; }
	void GetDirection(Point3 *pp)
	{
		assert(pp);
		*pp = m_direction;
	}

	void SetLocation(Point3 location);
	Point3 GetLocation() { return m_location; }
	void GetLocation(Point3 *pp)
	{
		assert(pp);
		*pp = m_location;
	}

	void SetIntensity(Float intensity);
	Float GetIntensity() { return m_intensity; }
	void GetIntensity(Float *pVal)
	{
		assert(pVal);
		*pVal = m_intensity;
	}

	void SetOn(Boolean on);
	Boolean GetOn() { return m_on; }
	void GetOn(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_on;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmLight);

	DECLARE_VALUE(ambientIntensity);
	DECLARE_VALUE(color);
	DECLARE_VALUE(direction);
	DECLARE_VALUE(intensity);
	DECLARE_VALUE(location);
	DECLARE_VALUE(on);

	DECLARE_METHOD(set_ambientIntensity);
	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_direction);
	DECLARE_METHOD(set_intensity);
	DECLARE_METHOD(set_location);
	DECLARE_METHOD(set_on);

	DECLARE_CALLBACK(ambientIntensity_changed);
	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(direction_changed);
	DECLARE_CALLBACK(intensity_changed);
	DECLARE_CALLBACK(location_changed);
	DECLARE_CALLBACK(on_changed);

};


#endif // _anmlight_h
