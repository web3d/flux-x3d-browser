/********************************************************************************
 * Flux
 *
 * File: anmlod.h
 * Description: LOD node class
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

#ifndef _anmlod_h
#define _anmlod_h

#include "anmgroup.h"

#define ANMLOD_DEFAULT_CENTER Point3(0, 0, 0)

class  CAnmLOD : public CAnmGroup
{

protected:

	Point3						 m_center;
	FloatArray					*m_range;

	class CAnmProximitySensor	*m_proxSensor;
	CAnmNode					*m_levelToDisplay;

	// helpers
	void HandleCameraChanged(Point3 p);
	virtual CAnmNode *getLevelToDisplay(Point3 camerapos);

	// static members
	static void CameraChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData);

public:


	// constructor/destructor
	CAnmLOD();
	virtual ~CAnmLOD();

	// CAnmGroup overrides
	virtual void Realize();
	virtual float GetRadius();
	virtual void GetBoundingBox(CAnmBoundingBox *pBoundingBox);
	virtual void GetObjectCenter(Point3 *pCenter);
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual void PreTraverse();
	virtual void PostTraverse();
	virtual void Update();		// re-render/reset rendering structures

	// Accessors
	virtual void SetCenter(Point3 center);
	virtual Point3 GetCenter()
	{
		return m_center;
	}
	virtual void GetCenter(Point3 *pVal)
	{
		assert(pVal);
		*pVal = m_center;
	}

	virtual void SetLevel(NodeArray *pVal);
	virtual void GetLevel(NodeArray **ppVal)
	{
		GetChildren(ppVal);
	}

	virtual void SetRange(FloatArray *pVal);
	virtual void GetRange(FloatArray **ppVal)
	{
		assert(ppVal);
		*ppVal = m_range;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmLOD);

	DECLARE_VALUE(center);
	DECLARE_VALUE(children);
	DECLARE_VALUE(level);
	DECLARE_VALUE(range);

	DECLARE_METHOD(set_center);
	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_level);
	DECLARE_METHOD(set_range);

	DECLARE_CALLBACK(center_changed);
	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(level_changed);
	DECLARE_CALLBACK(range_changed);
};


#endif // _anmlod_h
