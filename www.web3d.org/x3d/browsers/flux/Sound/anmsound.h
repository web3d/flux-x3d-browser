/********************************************************************************
 * Flux
 *
 * File: anmsound.h
 * Description: Spatialized sound node
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

#ifndef _anmsound_h
#define _anmsound_h

#include "anmgroup.h"

#define ANMSOUND_DEFAULT_DIRECTION		Point3(0, 0, 1)		// RH coords
#define ANMSOUND_DEFAULT_INTENSITY		1.f
#define ANMSOUND_DEFAULT_LOCATION		Point3(0, 0, 0)
#define ANMSOUND_DEFAULT_MAXBACK		10.f
#define ANMSOUND_DEFAULT_MAXFRONT		10.f
#define ANMSOUND_DEFAULT_MINBACK		1.f
#define ANMSOUND_DEFAULT_MINFRONT		1.f
#define ANMSOUND_DEFAULT_PRIORITY		0.f
#define ANMSOUND_DEFAULT_SPATIALIZE		TRUE

class CAnmSound : public CAnmGroup
{

protected:

	Point3			m_direction;
	Float			m_intensity;
	Point3			m_location;
	Float			m_maxBack;
	Float			m_maxFront;
	Float			m_minBack;
	Float			m_minFront;
	Float			m_priority;
	Boolean			m_spatialize;

	CAnmNode		*m_source;


public:


	// Constructor/Destructor
	CAnmSound();
	virtual ~CAnmSound();

	// Group class overrides
	virtual void Realize();		// build lower-level rendering structures
	virtual void Update();		// re-render/reset rendering structures
	virtual eAnmReturnStatus AddChild(CAnmNode *pChild);
	virtual eAnmReturnStatus RemoveChild(CAnmNode *pChild);

	// New methods

	// Accessors
	void SetDirection(Point3 direction);
	Point3 GetDirection() { return m_direction; }
	void GetDirection(Point3 *pp)
	{
		assert(pp);
		*pp = m_direction;
	}

	void SetIntensity(Float intensity);
	Float GetIntensity() { return m_intensity; }
	void GetIntensity(Float *pVal)
	{
		assert(pVal);
		*pVal = m_intensity;
	}

	void SetLocation(Point3 location);
	Point3 GetLocation() { return m_location; }
	void GetLocation(Point3 *pp)
	{
		assert(pp);
		*pp = m_location;
	}

	void SetMaxBack(Float maxBack);
	Float GetMaxBack() { return m_maxBack; }
	void GetMaxBack(Float *pVal)
	{
		assert(pVal);
		*pVal = m_maxBack;
	}

	void SetMaxFront(Float maxFront);
	Float GetMaxFront() { return m_maxFront; }
	void GetMaxFront(Float *pVal)
	{
		assert(pVal);
		*pVal = m_maxFront;
	}

	void SetMinBack(Float minBack);
	Float GetMinBack() { return m_minBack; }
	void GetMinBack(Float *pVal)
	{
		assert(pVal);
		*pVal = m_minBack;
	}

	void SetMinFront(Float minFront);
	Float GetMinFront() { return m_minFront; }
	void GetMinFront(Float *pVal)
	{
		assert(pVal);
		*pVal = m_minFront;
	}

	void SetPriority(Float priority);
	Float GetPriority() { return m_priority; }
	void GetPriority(Float *pVal)
	{
		assert(pVal);
		*pVal = m_priority;
	}

	void SetSource(CAnmNode *pSource);
	class CAnmNode *GetSource() { return m_source; }
	void GetSource(CAnmNode **pVal)
	{
		assert(pVal);
		*pVal = m_source;
	}

	void SetSpatialize(Boolean spatialize);
	Boolean GetSpatialize() { return m_spatialize; }
	void GetSpatialize(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_spatialize;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmSound);

	DECLARE_VALUE(direction);
	DECLARE_VALUE(intensity);
	DECLARE_VALUE(location);
	DECLARE_VALUE(maxBack);
	DECLARE_VALUE(maxFront);
	DECLARE_VALUE(minBack);
	DECLARE_VALUE(minFront);
	DECLARE_VALUE(priority);
	DECLARE_VALUE(source);
	DECLARE_VALUE(spatialize);

	DECLARE_METHOD(set_direction);
	DECLARE_METHOD(set_intensity);
	DECLARE_METHOD(set_location);
	DECLARE_METHOD(set_maxBack);
	DECLARE_METHOD(set_maxFront);
	DECLARE_METHOD(set_minBack);
	DECLARE_METHOD(set_minFront);
	DECLARE_METHOD(set_priority);
	DECLARE_METHOD(set_source);
	DECLARE_METHOD(set_spatialize);

	DECLARE_CALLBACK(direction_changed);
	DECLARE_CALLBACK(intensity_changed);
	DECLARE_CALLBACK(location_changed);
	DECLARE_CALLBACK(maxBack_changed);
	DECLARE_CALLBACK(maxFront_changed);
	DECLARE_CALLBACK(minBack_changed);
	DECLARE_CALLBACK(minFront_changed);
	DECLARE_CALLBACK(priority_changed);
	DECLARE_CALLBACK(source_changed);
	DECLARE_CALLBACK(spatialize_changed);
};

DECLARE_GETIMPLEMENTATION(CAnmSound);

#endif // _anmsound_h
