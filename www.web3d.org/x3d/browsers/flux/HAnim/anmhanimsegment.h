/********************************************************************************
 * Flux
 *
 * File: anmhanimsegment.cpp
 * Description: HAnim Segment node
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

#ifndef _anmhanimsegment_h
#define _anmhanimsegment_h

#include "anmgroup.h"

#define ANMHANIMSEGMENT_DEFAULT_CENTEROFMASS				Point3(0, 0, 0)
#define ANMHANIMSEGMENT_DEFAULT_MASS						0.f
#define ANMHANIMSEGMENT_DEFAULT_NAME						""

class  CAnmHAnimSegment : public CAnmGroup
{
 
protected:

	Point3								 m_centerOfMass;
	CAnmNode							*m_coord;
	NodeArray							*m_displacers;
	Float								 m_mass;
	FloatArray							*m_momentsOfInertia;
	String								 m_name;

	// Storage for BaseMesh of displaced verticies
	//
	std::vector<Point3>					m_baseMesh;

	void InitializeDisplacedSegment();
	void AddDisplacerCallbacks();
	void HandleDisplacersChanged();

	static void DisplacersChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, 
		void *callData, void *userData);

	friend class CAnmHAnimJoint;

public:

	// constructor/destructor
	CAnmHAnimSegment();
	virtual ~CAnmHAnimSegment();

	// CAnmGroup overrides
	virtual void Update();				// re-render/reset rendering structures
	virtual void Realize();				// build lower-level rendering structures

	// New methods
	
	// Accessors
	virtual void SetCenterOfMass(Point3 p);
	virtual Point3 GetCenterOfMass()
	{
		return m_centerOfMass;
	}
	virtual void GetCenterOfMass(Point3 *pp)
	{
		assert(pp);
		*pp = m_centerOfMass;
	}
	

	void SetCoord(CAnmNode *pCoord);
	class CAnmNode *GetCoord() { return m_coord; }
	void GetCoord(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_coord;
	}
	
	void SetDisplacers(NodeArray *pDisplacers);
	NodeArray *GetDisplacers() { return m_displacers; }
	void GetDisplacers(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_displacers;
	}

	void SetMass(Float mass);
	Float GetMass()
	{
		return m_mass;
	}
	void GetMass(Float *pVal)
	{
		assert(pVal);
		*pVal = m_mass;
	}
	
	void SetMomentsOfInertia(FloatArray *pMomentsOfInertia);
	FloatArray *GetMomentsOfInertia() { return m_momentsOfInertia; }
	void GetMomentsOfInertia(FloatArray **pVal)
	{
		assert(pVal);
		*pVal = m_momentsOfInertia;
	}

	void SetName(String name);
	String GetName()
	{
		return m_name;
	}
	void GetName(String *pStr)
	{
		assert(pStr);
		*pStr = m_name;
	}


	
	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmHAnimSegment);

	DECLARE_VALUE(bboxCenter);
	DECLARE_VALUE(bboxSize);
	DECLARE_VALUE(centerOfMass);
	DECLARE_VALUE(children);
	DECLARE_VALUE(coord);
	DECLARE_VALUE(displacers);
	DECLARE_VALUE(mass);
	DECLARE_VALUE(momentsOfInertia);
	DECLARE_VALUE(name);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_centerOfMass);
	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_coord);
	DECLARE_METHOD(set_displacers);
	DECLARE_METHOD(set_mass);
	DECLARE_METHOD(set_momentsOfInertia);
	DECLARE_METHOD(set_name);

	DECLARE_CALLBACK(centerOfMass_changed);
	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(coord_changed);
	DECLARE_CALLBACK(displacers_changed);
	DECLARE_CALLBACK(mass_changed);
	DECLARE_CALLBACK(momentsOfInertia_changed);
	DECLARE_CALLBACK(name_changed);
};

#define ANMSEGMENT_COORDDIRTY		(ANMGROUP_NDIRTYBITS)
#define ANMSEGMENT_DISPLACERSDIRTY	(ANMSEGMENT_COORDDIRTY + 1)
#define ANMSEGMENT_NDIRTYBITS		(ANMSEGMENT_DISPLACEMENTSDIRTY + 1)

typedef enum eAnmSegmentDirtyFlags {
eAnmSegmentCoordDirty			= ANMDIRTYBIT(ANMSEGMENT_COORDDIRTY),
eAnmSegmentDisplacersDirty		= ANMDIRTYBIT(ANMSEGMENT_DISPLACERSDIRTY),
} eAnmSegmentDirtyFlags;


#endif // _anmhanimsegment_h
