/********************************************************************************
 * Flux
 *
 * File: anmhanimjoint.h
 * Description: HAnim Joint node
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

#ifndef _anmhanimjoint_h
#define _anmhanimjoint_h

#include "anmtransform.h"

#define ANMHANIMJOINT_DEFAULT_NAME							""
#define ANMHANIMJOINT_DEFAULT_LIMITORIENTATION				Rotation(0, 0, 1, 0)

class  CAnmHAnimJoint : public CAnmTransform
{
protected:

	NodeArray							*m_displacers;
	Rotation							 m_limitOrientation;
	FloatArray							*m_llimit;
	String								 m_name;
	IntegerArray						*m_skinCoordIndex;
	FloatArray							*m_skinCoordWeight;
	IntegerArray						*m_skinNormalIndex;
	FloatArray							*m_skinNormalWeight;
	FloatArray							*m_stiffness;
	FloatArray							*m_ulimit;
	
	bool								 m_singleMesh;

	// krv: is this the correct type of array to use?
	// 
	std::vector<Point3>					m_localCoords;
	std::vector<Point3>					m_localNormals;

	
	void AddDisplacerCallbacks();
	void HandleDisplacersChanged();
	static void DisplacersChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, 
		void *callData, void *userData);


	// krv???:  whare are these virtual?  Is this defined on the Group class.
	virtual void UpdateSkin(class CAnmCoordinate *pCoord, class CAnmNormal *pNormal,
		const matrix4 &mat);
	virtual void InitializeSkin(class CAnmCoordinate *pCoord, 
		class CAnmNormal *pNormal, const matrix4 &mat);

	static void UpdateSkin(class CAnmGroup *pGroup, class CAnmCoordinate *pCoord, class CAnmNormal *pNormal,
		const matrix4 &mat);
	static void InitializeSkin(class CAnmGroup *pGroup, 
		class CAnmCoordinate *pCoord, class CAnmNormal *pNormal, const matrix4 &mat);


	friend class CAnmHAnimHumanoid;

public:

	// constructor/destructor
	CAnmHAnimJoint();
	virtual ~CAnmHAnimJoint();

	// CAnmTransform overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void ChildChanged(CAnmNode *pChild);

	// Transform matrix overrides for single mesh animation
	virtual void SetCenter(Point3 p);
	virtual void SetRotation(Rotation r);
	virtual void SetScale(Point3 p);
	virtual void SetScaleOrientation(Rotation r);
	virtual void SetTranslation(Point3 p);

	// New methods
	
	// Accessors
	void SetDisplacers(NodeArray *pDisplacers);
	NodeArray *GetDisplacers() { return m_displacers; }
	void GetDisplacers(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_displacers;
	}

	virtual void SetLimitOrientation(Rotation r);
	virtual Rotation GetLimitOrientation()
	{
		return m_limitOrientation;
	}
	virtual void GetLimitOrientation(Rotation *pr)
	{
		assert(pr);
		*pr = m_limitOrientation;
	}

	void SetLLimit(FloatArray *pLLimit);
	FloatArray *GetLLimit() { return m_llimit; }
	void GetLLimit(FloatArray **pVal)
	{
		assert(pVal);
		*pVal = m_llimit;
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

	


	void SetSkinCoordIndex(IntegerArray *pSkinCoordIndex);
	IntegerArray *GetSkinCoordIndex() { return m_skinCoordIndex; }
	void GetSkinCoordIndex(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_skinCoordIndex;
	}

	void SetSkinCoordWeight(FloatArray *pSkinCoordWeight);
	FloatArray *GetSkinCoordWeight() { return m_skinCoordWeight; }
	void GetSkinCoordWeight(FloatArray **pVal)
	{
		assert(pVal);
		*pVal = m_skinCoordWeight;
	}

	void SetSkinNormalIndex(IntegerArray *pSkinNormalIndex);
	IntegerArray *GetSkinNormalIndex() { return m_skinNormalIndex; }
	void GetSkinNormalIndex(IntegerArray **pVal)
	{ 
		if (pVal)
			*pVal = m_skinNormalIndex;
	}

	void SetSkinNormalWeight(FloatArray *pSkinNormalWeight);
	FloatArray *GetSkinNormalWeight() { return m_skinNormalWeight; }
	void GetSkinNormalWeight(FloatArray **pVal)
	{
		assert(pVal);
		*pVal = m_skinNormalWeight;
	}

	void SetStiffness(FloatArray *pStiffnes);
	FloatArray *GetStiffness() { return m_stiffness; }
	void GetStiffness(FloatArray **pVal)
	{
		assert(pVal);
		*pVal = m_stiffness;
	}
	
	void SetULimit(FloatArray *pULimit);
	FloatArray *GetULimit() { return m_ulimit; }
	void GetULimit(FloatArray **pVal)
	{
		assert(pVal);
		*pVal = m_ulimit;
	}

	
	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmHAnimJoint);

	DECLARE_VALUE(bboxCenter);
	DECLARE_VALUE(bboxSize);
	DECLARE_VALUE(center);
	DECLARE_VALUE(children);
	DECLARE_VALUE(displacers);
	DECLARE_VALUE(limitOrientation);
	DECLARE_VALUE(llimit);
	DECLARE_VALUE(name);
	DECLARE_VALUE(rotation);
	DECLARE_VALUE(scale);
	DECLARE_VALUE(scaleOrientation);
	DECLARE_VALUE(skinCoordIndex);
	DECLARE_VALUE(skinCoordWeight);
	DECLARE_VALUE(skinNormalIndex);
	DECLARE_VALUE(skinNormalWeight);
	DECLARE_VALUE(stiffness);
	DECLARE_VALUE(translation);
	DECLARE_VALUE(ulimit);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_center);
	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_displacers);
	DECLARE_METHOD(set_limitOrientation);
	DECLARE_METHOD(set_llimit);
	DECLARE_METHOD(set_name);
	DECLARE_METHOD(set_rotation);
	DECLARE_METHOD(set_scale);
	DECLARE_METHOD(set_scaleOrientation);
	DECLARE_METHOD(set_skinCoordIndex);
	DECLARE_METHOD(set_skinCoordWeight);
	DECLARE_METHOD(set_skinNormalIndex);
	DECLARE_METHOD(set_skinNormalWeight);
	DECLARE_METHOD(set_stiffness);
	DECLARE_METHOD(set_translation);
	DECLARE_METHOD(set_ulimit);

	DECLARE_CALLBACK(center_changed);
	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(displacers_changed);
	DECLARE_CALLBACK(limitOrientation_changed);
	DECLARE_CALLBACK(llimit_changed);
	DECLARE_CALLBACK(name_changed);
	DECLARE_CALLBACK(rotation_changed);
	DECLARE_CALLBACK(scale_changed);
	DECLARE_CALLBACK(scaleOrientation_changed);
	DECLARE_CALLBACK(skinCoordIndex_changed);
	DECLARE_CALLBACK(skinCoordWeight_changed);
	DECLARE_CALLBACK(skinNormalIndex_changed);
	DECLARE_CALLBACK(skinNormalWeight_changed);
	DECLARE_CALLBACK(stiffness_changed);
	DECLARE_CALLBACK(translation_changed);
	DECLARE_CALLBACK(ulimit_changed);
};


#endif // _anmhanimjoint_h
