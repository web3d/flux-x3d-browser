/********************************************************************************
 * Flux
 *
 * File: anmtransform.h
 * Description: Transform node class
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

#ifndef _anmtransform_h
#define _anmtransform_h

#include "anmnodedefs.h"
#include "anmgroup.h"

#define ANMTRANSFORM_DEFAULT_CENTER				Point3(0, 0, 0)
#define ANMTRANSFORM_DEFAULT_TRANSLATION		Point3(0, 0, 0)
#define ANMTRANSFORM_DEFAULT_SCALE				Point3(1, 1, 1)
#define ANMTRANSFORM_DEFAULT_ROTATION			Rotation(0, 0, 1, 0)
#define ANMTRANSFORM_DEFAULT_SCALEORIENTATION	Rotation(0, 0, 1, 0)

class  CAnmTransform : public CAnmGroup
{

protected:

	matrix4			m_matrix;

	Point3			m_center;
	Point3			m_translation;
	Point3			m_scale;
	Rotation		m_rotation;
	Rotation		m_scaleOrientation;

	static bool m_drawBoxes;

public:

	// constructor/destructor
	CAnmTransform();
	virtual ~CAnmTransform();

	// CAnmGroup overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();								// re-render/reset rendering structures
	virtual float GetRadius();
	virtual void GetBoundingBox(CAnmBoundingBox *pBoundingBox);
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);

	// New methods
	static void GetLocalMatrix(CAnmGroup *pTargetGroup,
										  CAnmGroup **pParents, int nParents, matrix4 *pMatReturn);

	static void UpdateMatrix(matrix4 &matrix,
							const Point3 &translation,
							const Rotation &rotation,
							const Point3 &scale,
							const Rotation &scaleOrientation,
							const Point3 &center);
	
	static void SetDrawBoxes(bool drawBoxes);
	static bool GetDrawBoxes();

	// Accessors

	virtual matrix4 *GetMatrix()
	{
		return &m_matrix;
	}

	virtual void SetCenter(Point3 p);
	virtual void SetCenter(float x, float y, float z);
	virtual void GetCenter(Point3 *pp)
	{
		assert(pp);
		*pp = m_center;
	}

	virtual void SetTranslation(Point3 p);
	virtual void SetTranslation(float x, float y, float z);
	virtual Point3 GetTranslation()
	{
		return m_translation;
	}
	virtual void GetTranslation(Point3 *pp)
	{
		assert(pp);
		*pp = m_translation;
	}

	virtual void SetScale(Point3 p);
	virtual void SetScale(float x, float y, float z);
	virtual Point3 GetScale()
	{
		return m_scale;
	}
	virtual void GetScale(Point3 *pp)
	{
		assert(pp);
		*pp = m_scale;
	}

	virtual void SetRotation(Rotation r);
	virtual void SetRotation(float x, float y, float z, float theta);
	virtual Rotation GetRotation()
	{
		return m_rotation;
	}
	virtual void GetRotation(Rotation *pr)
	{
		assert(pr);
		*pr = m_rotation;
	}

	virtual void SetScaleOrientation(Rotation r);
	virtual void SetScaleOrientation(float x, float y, float z, float theta);
	virtual Rotation GetScaleOrientation()
	{
		return m_scaleOrientation;
	}
	virtual void GetScaleOrientation(Rotation *pr)
	{
		assert(pr);
		*pr = m_scaleOrientation;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmTransform);

	DECLARE_VALUE(bboxCenter);
	DECLARE_VALUE(bboxSize);
	DECLARE_VALUE(children);
	DECLARE_VALUE(center);
	DECLARE_VALUE(translation);
	DECLARE_VALUE(scale);
	DECLARE_VALUE(rotation);
	DECLARE_VALUE(scaleOrientation);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_bboxCenter);
	DECLARE_METHOD(set_bboxSize);
	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_center);
	DECLARE_METHOD(set_translation);
	DECLARE_METHOD(set_scale);
	DECLARE_METHOD(set_rotation);
	DECLARE_METHOD(set_scaleOrientation);

	DECLARE_CALLBACK(bboxCenter_changed);
	DECLARE_CALLBACK(bboxSize_changed);
	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(center_changed);
	DECLARE_CALLBACK(translation_changed);
	DECLARE_CALLBACK(scale_changed);
	DECLARE_CALLBACK(rotation_changed);
	DECLARE_CALLBACK(scaleOrientation_changed);

};

#define ANMTRANSFORM_NDIRTYBITS					(ANMGROUP_NDIRTYBITS)

#endif // _anmtransform_h
