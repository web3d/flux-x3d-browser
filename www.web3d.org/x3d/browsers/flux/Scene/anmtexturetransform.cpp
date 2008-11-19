/********************************************************************************
 * Flux
 *
 * File: anmtexturetransform.cpp
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

#include "stdafx.h"

#include "anmtexturetransform.h"
#include "anmdevice.h"
#include "anmworld.h"

CAnmTextureTransform::CAnmTextureTransform() : 

	CAnmGroup(),						// Parent Appearance will add me
	m_translation(ANMTEXTURETRANSFORM_DEFAULT_TRANSLATION),
	m_rotation(ANMTEXTURETRANSFORM_DEFAULT_ROTATION),
	m_scale(ANMTEXTURETRANSFORM_DEFAULT_SCALE),
	m_center(ANMTEXTURETRANSFORM_DEFAULT_CENTER)
{
	m_matrix = matrix4::Identity;
	SetStateDirty();
}

CAnmTextureTransform::~CAnmTextureTransform()
{
}

void CAnmTextureTransform::SetToDevice(CAnmDevice *pDevice, matrix4* pMat )
{
	pDevice->SetTextureTransform(0, pMat );
}

void CAnmTextureTransform::SetToDevice(CAnmDevice *pDevice)
{
	pDevice->SetTextureTransform(0, &m_matrix);
}

// CAnmNode overrides
void CAnmTextureTransform::Realize()
{
	CAnmGroup::Realize();
}


////////////////////////////////////////////////////////////////////////////////
// CAnmTextureTransform::Update
//
// The VRML97 spec stipulates that texture coordinates are transformed thusly:
//
//		Tc' = -C × S × R × C × T × Tc
//
// where Tc is the original and Tc' is the transformed texture.
//
//			KRV:  THis odes not work!
//
// It should be obvious that the texture coordinates in this equation
// are given as column vectors. Our matrix package deals with row vectors,
// so we implement it as follows:
//
//		Tc' = Tc × T × -C × R × S × C
//
// In order to assure correct implementation, we compute the transformation
// in the coordinate space specified by VRML:
//
//   t
//   ^
//   |
// 1 +****
//   |****
//   |****
// 0 +---+--> s
//   0   1
//
// This is then converted to the coordinate system of the 3D graphics system
// at some later point. (This seems to be the same space used by Direct3D).
//
// For expediency, we use 4x4 matrices; in the future we may use 3x2.
////////////////////////////////////////////////////////////////////////////////

#define D3D_TEXTURE_TRANSFORM 1
// #define ORIGINAL

void CAnmTextureTransform::Update()
{
	// N.B.: the parent Appearance checks me for dirty before calling this;
	// don't test again


	Point3	v;		// temporary vector
	matrix4	mat;	// temporary matrix

/***********
	// apply translation T and translate center C to origin
	v.Assign(m_translation.x - m_center.x, m_translation.y - m_center.y, 0);	// translation vector
	m_matrix.ToTranslation(v);				// T × -C

	// rotation about Z
	v.Assign(0, 0, 1);						// axis of rotation
	mat.ToAxisAngle(v, m_rotation);			// R
	m_matrix = m_matrix * mat;				// T × -C × R

	// scale
	v.Assign(m_scale.x, m_scale.y, 1);		// scale vector
	mat.ToScale(v);							// S
	m_matrix = m_matrix * mat;				// T × -C × R × S

	// translation back from center
	v.Assign(m_center.x, m_center.y, 0);	// translation vector
	mat.ToTranslation(v);					// C
	m_matrix = m_matrix * mat;				// T × -C × R × S × C
***************/










//		krv
//		Spec says this, so we do this.

//		Tc' = -C × S × R × C × T × Tc
//			m_matrix = -C × S × R × C × T
//
	matrix4	matScale;	// temporary matrix
	matrix4	matRot;	// temporary matrix

	// First Get S x R
	// rotation about Z
	v.Assign(0, 0, 1);						// axis of rotation
	matRot.ToAxisAngle(v, m_rotation);			// R

	// scale
	v.Assign(m_scale.x, m_scale.y, 1);		// scale vector
	matScale.ToScale(v);							// S

	// Get S x R
	m_matrix = matRot * matScale;			// matrix = S x R

	//			Premultiple by negative Center, or just set the translation
	m_matrix.Place( Point3( -m_center.x, -m_center.y, 0.0 ) );   // matrix = (-C) x S x R


	// Now post multiply by ( C x T )
	v.Assign(m_translation.x + m_center.x, m_translation.y + m_center.y, 0);	// translation vector
	mat.ToTranslation(v);				// C x T
	m_matrix = mat * m_matrix;			// matrix = (-C) x S x R x C x C x T	
										


# ifdef D3D_TEXTURE_TRANSFORM
	// m00 m01 0 0           m00 m01 0 0
	// m10 m11 0 0    -->    m10 m11 0 0
	//  0   0  1 0           m20 m21 1 0
	// m20 m21 0 1            0   0  0 1
	m_matrix.m[0][1] = -m_matrix.m[0][1];
	m_matrix.m[1][1] = -m_matrix.m[1][1];
	m_matrix.m[2][0] =  m_matrix.m[3][0];
	m_matrix.m[2][1] = 1.0f - m_matrix.m[3][1];
	m_matrix.m[3][0] = 0.0f;
	m_matrix.m[3][1] = 0.0f;
#endif // D3D_TEXTURE_TRANSFORM

	ClearStateDirty();
}


// Accessors
void CAnmTextureTransform::SetCenter(Point2 center)
{
	m_center = center;

	SetStateDirty();
}

void CAnmTextureTransform::SetRotation(Float rotation)
{
	m_rotation = rotation;

	SetStateDirty();
}

void CAnmTextureTransform::SetScale(Point2 scale)
{
	m_scale = scale;

	SetStateDirty();
}

void CAnmTextureTransform::SetTranslation(Point2 translation)
{
	m_translation = translation;

	SetStateDirty();
}



// Tables for Callbacks, Methods and Set/GetValues


DEFINE_VALUE(CAnmTextureTransform, center, eValuePoint2, Point2, GetCenter, SetCenter);
DEFINE_VALUE(CAnmTextureTransform, rotation, eValueFloat, Float, GetRotation, SetRotation);
DEFINE_VALUE(CAnmTextureTransform, scale, eValuePoint2, Point2, GetScale, SetScale);
DEFINE_VALUE(CAnmTextureTransform, translation, eValuePoint2, Point2, GetTranslation, SetTranslation);

DEFINE_METHOD(CAnmTextureTransform, set_center, eValuePoint2, Point2, SetCenter);
DEFINE_METHOD(CAnmTextureTransform, set_rotation, eValueFloat, Float, SetRotation);
DEFINE_METHOD(CAnmTextureTransform, set_scale, eValuePoint2, Point2, SetScale);
DEFINE_METHOD(CAnmTextureTransform, set_translation, eValuePoint2, Point2, SetTranslation);

DEFINE_CALLBACK(CAnmTextureTransform, center_changed, eValuePoint2);
DEFINE_CALLBACK(CAnmTextureTransform, rotation_changed, eValueFloat);
DEFINE_CALLBACK(CAnmTextureTransform, scale_changed, eValuePoint2);
DEFINE_CALLBACK(CAnmTextureTransform, translation_changed, eValuePoint2);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmTextureTransform)
VALUEID(CAnmTextureTransform, center),
VALUEID(CAnmTextureTransform, rotation),
VALUEID(CAnmTextureTransform, scale),
VALUEID(CAnmTextureTransform, translation),

METHODID(CAnmTextureTransform, set_center),
METHODID(CAnmTextureTransform, set_rotation),
METHODID(CAnmTextureTransform, set_scale),
METHODID(CAnmTextureTransform, set_translation),

CALLBACKID(CAnmTextureTransform, center_changed),
CALLBACKID(CAnmTextureTransform, rotation_changed),
CALLBACKID(CAnmTextureTransform, scale_changed),
CALLBACKID(CAnmTextureTransform, translation_changed),


END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmTextureTransform, CAnmNode);

