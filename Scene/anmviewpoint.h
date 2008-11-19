/********************************************************************************
 * Flux
 *
 * File: anmviewpoint.h
 * Description: Viewpoint class - user-defined camera
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

#ifndef _anmviewpoint_h
#define _anmviewpoint_h

#include "anmbindablenode.h"

#define ANMVIEWPOINT_DEFAULT_CENTEROFROTATION	Point3(0.f,0.f,0.f)
#define ANMVIEWPOINT_DEFAULT_DESCRIPTION		""
#define ANMVIEWPOINT_DEFAULT_FIELDOFVIEW		0.785398f
#define ANMVIEWPOINT_DEFAULT_JUMP				TRUE
#define ANMVIEWPOINT_DEFAULT_ORIENTATION		Rotation(0.f,0.f,1.f,0.f)
#define ANMVIEWPOINT_DEFAULT_POSITION			Point3(0.f,0.f,10.f)

class EXPORT CAnmViewpoint : public CAnmBindableNode
{

protected:

	Float					m_fieldOfView;
	Boolean					m_jump;
	Rotation				m_orientation;
	Point3					m_position;
	String					m_description;
	Point3					m_centerOfRotation;

	Point3D					m_userpos;
	Point3D					m_userdir;
	Point3D					m_userup;

	matrix4D					m_worldmatrix;
	matrix4D					m_modelmatrix;

	// helpers
	
	void SetUserOffsets(const Point3D &userpos, const Point3D &userdir, const Point3D &userup)
	{
		m_userpos = userpos;
		m_userdir = userdir;
		m_userup = userup;
	}

	void GetUserOffsets(Point3D &userpos, Point3D &userdir, Point3D &userup)
	{
		userpos = m_userpos;
		userdir = m_userdir;
		userup = m_userup;
	}

	void SaveUserOffsets();

public:

	// Constructor/Destructor
	CAnmViewpoint();
	virtual ~CAnmViewpoint();

	// Node class overrides
	virtual void Realize();		// build lower-level rendering structures
	virtual void Update();		// re-render/reset rendering structures
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);

	// CAnmBindableNode overrides
	virtual void SetBind(Boolean bindFlag);

	// New methods
	virtual void Restore();
	virtual void SaveUserOffsets(const Point3 &wcpos, const Point3 &wcdir, const Point3 &wcup);

	// Accessors
	void SetFieldOfView(Float fov);
	Float GetFieldOfView() { return m_fieldOfView; }
	void GetFieldOfView(Float *pVal)
	{
		assert(pVal);
		*pVal = m_fieldOfView;
	}

	void SetJump(Boolean jump);
	Boolean GetJump() { return m_jump; }
	void GetJump(Boolean *pVal)
	{
		assert(pVal);
		*pVal = m_jump;
	}

	void SetOrientation(Rotation r);
	Rotation GetOrientation() { return m_orientation; }
	void GetOrientation(Rotation *pr)
	{
		assert(pr);
		*pr = m_orientation;
	}

	void SetPosition(Point3 p);
	Point3 GetPosition() { return m_position; }
	void GetPosition(Point3 *pp)
	{
		assert(pp);
		*pp = m_position;
	}

	void SetDescription(String description);
	String GetDescription()
	{
		return m_description;
	}
	void GetDescription(String *pStr)
	{
		assert(pStr);
		*pStr = m_description;
	}

	void SetCenterOfRotation(Point3 p);
	Point3 GetCenterOfRotation() { return m_centerOfRotation; }
	void GetCenterOfRotation(Point3 *pp)
	{
		assert(pp);
		*pp = m_centerOfRotation;
	}

	matrix4 GetWorldMatrix()
	{
		return matrix4( m_worldmatrix );
	}

	matrix4D GetModelMatrix()
	{
		return m_modelmatrix;
	}

	matrix4D GetCombinedModelWorldMatrix()
	{
		return m_modelmatrix * m_worldmatrix;
	}



	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmViewpoint);

	DECLARE_VALUE(fieldOfView);
	DECLARE_VALUE(jump);
	DECLARE_VALUE(orientation);
	DECLARE_VALUE(position);
	DECLARE_VALUE(description);
	DECLARE_VALUE(centerOfRotation);

	DECLARE_METHOD(set_bind);
	DECLARE_METHOD(set_fieldOfView);
	DECLARE_METHOD(set_jump);
	DECLARE_METHOD(set_orientation);
	DECLARE_METHOD(set_position);
	DECLARE_METHOD(set_description);
	DECLARE_METHOD(set_centerOfRotation);

	DECLARE_CALLBACK(bindTime);
	DECLARE_CALLBACK(fieldOfView_changed);
	DECLARE_CALLBACK(isBound);
	DECLARE_CALLBACK(jump_changed);
	DECLARE_CALLBACK(orientation_changed);
	DECLARE_CALLBACK(position_changed);
	DECLARE_CALLBACK(description_changed);
	DECLARE_CALLBACK(centerOfRotation_changed);

};

DECLARE_GETIMPLEMENTATION(CAnmViewpoint);

#endif // _anmviewpoint_h

