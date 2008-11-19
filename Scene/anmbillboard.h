/********************************************************************************
 * Flux
 *
 * File: anmbillboard.h
 * Description: Billboard node class
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

#ifndef _anmbillboard_h
#define _anmbillboard_h

#include "anmtransform.h"

#define ANMBILLBOARD_DEFAULT_AXISOFROTATION Point3(0, 1, 0)
#define ANMBILLBOARD_DEFAULT_BBOXCENTER Point3(0, 0, 0)
#define ANMBILLBOARD_DEFAULT_BBOXSIZE Point3(-1, -1, -1)

class  CAnmBillboard : public CAnmGroup
{

protected:

	Point3			m_axisOfRotation;
	Point3			m_bboxCenter;
	Point3			m_bboxSize;
	
	class CAnmProximitySensor	*m_proxSensor;

	// helpers
	void HandleCameraPositionChanged(Point3 *pCameraPosition);
	void HandleCameraOrientationChanged(Rotation *pCameraOrientation);
	void CalcMatrix(const matrix4 &worldmat, matrix4 &newmat);

	// static members
	static void CameraPositionChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData);
	static void CameraOrientationChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData);

public:


	// constructor/destructor
	CAnmBillboard();
	virtual ~CAnmBillboard();

	// CAnmGroup overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual void PreTraverse();
	virtual void PostTraverse();

	// New methods

	// Accessors

	virtual void SetAxisOfRotation(Point3 p);
	virtual Point3 GetAxisOfRotation()
	{
		return m_axisOfRotation;
	}
	virtual void GetAxisOfRotation(Point3 *pp)
	{
		assert(pp);
		*pp = m_axisOfRotation;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmBillboard);

	DECLARE_VALUE(axisOfRotation);
	DECLARE_VALUE(bboxCenter);
	DECLARE_VALUE(bboxSize);
	DECLARE_VALUE(children);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_axisOfRotation);
	DECLARE_METHOD(set_bboxCenter);
	DECLARE_METHOD(set_bboxSize);
	DECLARE_METHOD(set_children);

	DECLARE_CALLBACK(axisOfRotation_changed);
	DECLARE_CALLBACK(bboxCenter_changed);
	DECLARE_CALLBACK(bboxSize_changed);
	DECLARE_CALLBACK(children_changed);
};


#endif // _anmbillboard_h
