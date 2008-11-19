/********************************************************************************
 * Flux
 *
 * File: anmhanimdisplacer.h
 * Description: HAnim Displacer node
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

#ifndef _anmhanimdisplacer_h
#define _anmhanimdisplacer_h

#include "anmnode.h"

#define ANMHANIMDISPLACER_DEFAULT_NAME							""
#define ANMHANIMDISPLACER_DEFAULT_WEIGHT							0.0f

class CAnmHAnimDisplacer : public CAnmNode
{
protected :

	IntegerArray				*m_coordIndex;
	Point3Array					*m_displacements;
	String						 m_name;
	Float						 m_weight;

	class CAnmCoordinate		*m_coordinate;

	virtual void SetCoordinate(class CAnmCoordinate *pCoordinate);
	virtual class CAnmCoordinate *GetCoordinate()
	{
		return m_coordinate;
	}
 
	// krv???:  why virtual????????
	// krv   why protected and fiend?????  why not just make it public???
	virtual void DisplaceSingleMesh(Point3Array* pRawCoords, const matrix4 &mat);
	virtual void DisplaceSegment( Point3Array* pRawCoords );


	friend class CAnmHAnimSegment;
	friend class CAnmHAnimJoint;

public:

	// constructor/destructor
	CAnmHAnimDisplacer();
	virtual ~CAnmHAnimDisplacer();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	// Accessors
	void SetCoordIndex(IntegerArray *pCoordIndex);
	IntegerArray *GetCoordIndex() { return m_coordIndex; }
	void GetCoordIndex(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_coordIndex;
	}




	void SetDisplacements(Point3Array *pDisplacements);
	Point3Array	*GetDisplacements() { return m_displacements; }
	void GetDisplacements(Point3Array **pVal)
	{ 
		if (pVal)
			*pVal = m_displacements;
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


	void SetWeight(Float weight);
	Float GetWeight()
	{
		return m_weight;
	}
	void GetWeight(Float *pVal)
	{
		assert(pVal);
		*pVal = m_weight;
	}
	
	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmHAnimDisplacer);

	DECLARE_VALUE(coordIndex);
	DECLARE_VALUE(displacements);
	DECLARE_VALUE(name);
	DECLARE_VALUE(weight);

	DECLARE_METHOD(set_coordIndex);
	DECLARE_METHOD(set_displacements);
	DECLARE_METHOD(set_name);
	DECLARE_METHOD(set_weight);

	DECLARE_CALLBACK(coordIndex_changed);
	DECLARE_CALLBACK(displacements_changed);
	DECLARE_CALLBACK(name_changed);
	DECLARE_CALLBACK(weight_changed);

};

DECLARE_GETIMPLEMENTATION(CAnmHAnimDisplacer);

#define ANMDISPLACER_COORDINDEXDIRTY	(ANMNODE_NDIRTYBITS)
#define ANMDISPLACER_DISPLACEMENTSDIRTY	(ANMDISPLACER_COORDINDEXDIRTY + 1)
#define ANMDISPLACER_WEIGHTDIRTY		(ANMDISPLACER_COORDINDEXDIRTY + 2)
#define ANMDISPLACER_NDIRTYBITS			(ANMDISPLACER_WEIGHTDIRTY + 1)

typedef enum eAnmDisplacerDirtyFlags {
eAnmDisplacerCoordIndexDirty	= ANMDIRTYBIT(ANMDISPLACER_COORDINDEXDIRTY),
eAnmDisplacerDisplacementsDirty	= ANMDIRTYBIT(ANMDISPLACER_DISPLACEMENTSDIRTY),
eAnmDisplacerWeightDirty		= ANMDIRTYBIT(ANMDISPLACER_WEIGHTDIRTY),
} eAnmDisplacerDirtyFlags;


#endif _anmhanimdisplacer_h
