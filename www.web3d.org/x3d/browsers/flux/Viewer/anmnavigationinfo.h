/********************************************************************************
 * Flux
 *
 * File: anmnavigationinfo.h
 * Description: NavigationInfo node
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

#ifndef _anmnavigationinfo_h
#define _anmnavigationinfo_h

#include "anmbindablenode.h"

enum eNavigationMode {
	eNavigationModeWalk,
	eNavigationModeFly,
	eNavigationModeExamine,
	eNavigationModeLookAt,
	eNavigationModeNone,
};

enum eNavigationTransitionMode {
	eNavigationTransitionAnimate,
	eNavigationTransitionLinear,
	eNavigationTransitionTeleport,
};

#define ANMNAVINFO_DEFAULT_AVATAR_COLLISION_DISTANCE	0.25f
#define ANMNAVINFO_DEFAULT_AVATAR_HEIGHT_ABOVE_TERRAIN	1.6f
#define ANMNAVINFO_DEFAULT_AVATAR_TALLEST_OBJECT_HEIGHT	0.75f
#define ANMNAVINFO_DEFAULT_HEADLIGHT					true
#define ANMNAVINFO_DEFAULT_SPEED						1.0f
#define ANMNAVINFO_DEFAULT_VISIBILITYLIMIT				0.0f
#define ANMNAVINFO_DEFAULT_LOOKAT						false

class CAnmNavigationInfo : public CAnmBindableNode
{

protected:

	FloatArray					*m_avatarSize;
	Boolean						 m_headlight;
	Float						 m_speed;
	StringArray					*m_transitionType;
	StringArray					*m_type;
	Float						 m_visibilityLimit;
	Boolean						 m_isBound;

	eNavigationMode				 m_navigationMode;
	eNavigationTransitionMode	 m_transitionMode;
	bool						 m_lookAt;

public:

	// Constructor/Destructor
	CAnmNavigationInfo();
	virtual ~CAnmNavigationInfo();

	// Node class overrides
	virtual void Realize();		// build lower-level rendering structures
	virtual void Update();		// re-render/reset rendering structures

	// New methods
	virtual float GetCollisionRadius();
	virtual float GetAvatarHeight();
	virtual float GetMaxStepHeight();

	// Accessors
	void SetAvatarSize(FloatArray *pAvatarSize);
	FloatArray *GetAvatarSize() { return m_avatarSize; }
	void GetAvatarSize(FloatArray **pVal)
	{
		assert(pVal);
		*pVal = m_avatarSize;
	}

	void SetHeadlight(Boolean headlight);
	Boolean GetHeadlight() { return m_headlight; }
	void GetHeadlight(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_headlight;
	}

	void SetSpeed(Float speed);
	Float GetSpeed() { return m_speed; }
	void GetSpeed(Float *pVal)
	{
		assert(pVal);
		*pVal = m_speed;
	}

	void SetTransitionType(StringArray *pTransitionType);
	StringArray *GetTransitionType() { return m_transitionType; }
	void GetTransitionType(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_transitionType;
	}
	
	void SetType(StringArray *pType);
	StringArray *GetType() { return m_type; }
	void GetType(StringArray **pVal)
	{
		assert(pVal);
		*pVal = m_type;
	}

	void SetVisibilityLimit(Float visibilityLimit);
	Float GetVisibilityLimit() { return m_visibilityLimit; }
	void GetVisibilityLimit(Float *pVal)
	{
		assert(pVal);
		*pVal = m_visibilityLimit;
	}

	virtual void SetNavigationMode(eNavigationMode navMode)
	{
		m_navigationMode = navMode;
	}

	virtual eNavigationMode GetNavigationMode()
	{
		return m_navigationMode;
	}

	virtual eNavigationTransitionMode GetTransitionMode()
	{
		return m_transitionMode;
	}

	virtual bool GetLookAt()
	{
		return m_lookAt; 
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmNavigationInfo);

	DECLARE_VALUE(avatarSize);
	DECLARE_VALUE(headlight);
	DECLARE_VALUE(speed);
	DECLARE_VALUE(transitionType);
	DECLARE_VALUE(type);
	DECLARE_VALUE(visibilityLimit);

	DECLARE_METHOD(set_bind);
	DECLARE_METHOD(set_avatarSize);
	DECLARE_METHOD(set_headlight);
	DECLARE_METHOD(set_speed);
	DECLARE_METHOD(set_transitionType);
	DECLARE_METHOD(set_type);
	DECLARE_METHOD(set_visibilityLimit);

	DECLARE_CALLBACK(isBound);
	DECLARE_CALLBACK(avatarSize_changed);
	DECLARE_CALLBACK(headlight_changed);
	DECLARE_CALLBACK(speed_changed);
	DECLARE_CALLBACK(transitionType_changed);
	DECLARE_CALLBACK(type_changed);
	DECLARE_CALLBACK(visibilityLimit_changed);
};

#endif // _anmnavigationinfo_h
