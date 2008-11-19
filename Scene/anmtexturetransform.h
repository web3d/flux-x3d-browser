/********************************************************************************
 * Flux
 *
 * File: anmtexturetransform.h
 * Description: TextureTransform node
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

#ifndef _anmtexturetransform_h
#define _anmtexturetransform_h

#include "anmgroup.h"

#define ANMTEXTURETRANSFORM_DEFAULT_CENTER		Point2(0, 0)
#define ANMTEXTURETRANSFORM_DEFAULT_ROTATION	0.f
#define ANMTEXTURETRANSFORM_DEFAULT_SCALE		Point2(1, 1)
#define ANMTEXTURETRANSFORM_DEFAULT_TRANSLATION		Point2(0, 0)

class CAnmTextureTransform : public CAnmGroup
{
protected :

	Point2							m_center;
	Float							m_rotation;
	Point2							m_scale;
	Point2							m_translation;

	matrix4							m_matrix;

	static CAnmTextureTransform		m_defaultTextureTransform;

public:

	// constructor/destructor
	CAnmTextureTransform();
	virtual ~CAnmTextureTransform();

	// CAnmNode overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();								// re-render/reset rendering structures

	// New methods
	virtual void SetToDevice(class CAnmDevice *pDevice);
	virtual void SetToDevice(class CAnmDevice *pDevice, int startLevel, int nLevels)
	{
		SetToDevice(pDevice);
	}
	virtual int NumLevels()
	{
		return 1;
	}

	static void SetToDevice(CAnmDevice *pDevice, matrix4* pMat );


	// Accessors
	virtual void SetCenter(Point2 center);
	virtual Point2 GetCenter() { return m_center; }
	virtual void GetCenter(Point2 *pVal)
	{
		if (pVal)
			*pVal = m_center;
	}

	virtual void SetRotation(Float rotation);
	virtual Float GetRotation() { return m_rotation; }
	virtual void GetRotation(Float *pVal)
	{
		if (pVal)
			*pVal = m_rotation;
	}

	virtual void SetScale(Point2 scale);
	virtual Point2 GetScale() { return m_scale; }
	virtual void GetScale(Point2 *pVal)
	{
		if (pVal)
			*pVal = m_scale;
	}

	virtual void SetTranslation(Point2 translation);
	virtual Point2 GetTranslation() { return m_translation; }
	virtual void GetTranslation(Point2 *pVal)
	{
		if (pVal)
			*pVal = m_translation;
	}

	virtual matrix4 *GetMatrix()
	{
		return &m_matrix;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmTextureTransform);

	DECLARE_VALUE(center);
	DECLARE_VALUE(rotation);
	DECLARE_VALUE(scale);
	DECLARE_VALUE(translation);

	DECLARE_METHOD(set_center);
	DECLARE_METHOD(set_rotation);
	DECLARE_METHOD(set_scale);
	DECLARE_METHOD(set_translation);

	DECLARE_CALLBACK(center_changed);
	DECLARE_CALLBACK(rotation_changed);
	DECLARE_CALLBACK(scale_changed);
	DECLARE_CALLBACK(translation_changed);

};

DECLARE_GETIMPLEMENTATION(CAnmTextureTransform);

#endif _anmtexturetransform_h
