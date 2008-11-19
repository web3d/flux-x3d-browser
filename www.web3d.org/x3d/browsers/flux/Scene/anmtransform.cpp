/********************************************************************************
 * Flux
 *
 * File: anmtransform.cpp
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

#include "stdafx.h"

#include "anmnodedefs.h"
#include "anmtransform.h"
#include "anmdevice.h"
#include "anmlayer.h"
#include "anmquaternion.h"
#include "anmvisitor.h"
#include "anmupdatevisitor.h"
#include "anmworld.h"
#include "anmMatrixInvert.h"

CAnmTransform::CAnmTransform() : CAnmGroup (),
	m_center(ANMTRANSFORM_DEFAULT_CENTER),
	m_translation(ANMTRANSFORM_DEFAULT_TRANSLATION),
	m_scale (ANMTRANSFORM_DEFAULT_SCALE),
	m_rotation (ANMTRANSFORM_DEFAULT_ROTATION),
	m_scaleOrientation(ANMTRANSFORM_DEFAULT_SCALEORIENTATION)
{
	m_matrix = matrix4::Identity;
}

CAnmTransform::~CAnmTransform()
{
	// anything? just chain to group?
}

void CAnmTransform::Realize()
{
	// create the frame, and chain the realize down to the children
	CAnmGroup::Realize();

	SetStateAndMatrixDirty();
}

void CAnmTransform::Traverse(CAnmUpdateVisitor *pVisitor)
{
	if (!m_realized)
		return;

	if (StateDirty())
		Update();

	// N.B.: traverse my children if either we have to recalc matrices and instances for
	// everyone, or because some child has requested updated matrices or state
	if (TestDirtyBits(ANMGROUPDIRTY_CHECKTRAVERSECHILDREN))
	{
		// push transform state
		matrix4 saved, current;

		// set new transform state in visitor
		if (!pVisitor->GetScreenMode())
		{
			pVisitor->GetWorldMatrix(&saved);
			current = m_matrix * saved;
			pVisitor->SetWorldMatrix(&current);
		}
		else
		{
			pVisitor->GetScreenMatrix(&saved);
			current = m_matrix * saved;
			pVisitor->SetScreenMatrix(&current);
		}

		// call Group class to traverse children
		CAnmGroup::Traverse(pVisitor);

		// pop transform state
		if (!pVisitor->GetScreenMode())
		{
			pVisitor->SetWorldMatrix(&saved);
		}
		else
		{
			pVisitor->SetScreenMatrix(&saved);
		}

	}
	else
	{
		int debug = 1;
	}
}


#if METHODSTILL_HERE


#if 1
HRESULT MatrixInvert( matrix4& q, matrix4& a )
{

are we compliing this?	

    if( ( fabs(a._44 - 1.0f) > .001f) ||
        (  fabs(a._14) > .001f || fabs(a._24) > .001f || fabs(a._34) > .001f ) ) {

		long retval = MMInvertMatrix(a.m[0], q.m[0], 4);
		return retval ? S_OK : E_INVALIDARG;

	}
	else {
		
#define DOMATRIX_INVERSION_IN_DOUBLE
#ifdef DOMATRIX_INVERSION_IN_DOUBLE



		double a_11 = ( double ) a._11;
		double a_12 = ( double ) a._12;
		double a_13 = ( double ) a._13;

		double a_21 = ( double ) a._21;
		double a_22 = ( double ) a._22;
		double a_23 = ( double ) a._23;

		double a_31 = ( double ) a._31;
		double a_32 = ( double ) a._32;
		double a_33 = ( double ) a._33;

		double a_41 = ( double ) a._41;
		double a_42 = ( double ) a._42;
		double a_43 = ( double ) a._43;


		double q_11;
		double q_12;
		double q_13;

		double q_21;
		double q_22;
		double q_23;

		double q_31;
		double q_32;
		double q_33;

		double fDet =			( a_11 * ( a_22 * a_33 - a_23 * a_32 ) -
								  a_12 * ( a_21 * a_33 - a_23 * a_31 ) +
								  a_13 * ( a_21 * a_32 - a_22 * a_31 ) );

		if( fabs( fDet ) < 1.0e-50 ) {
			q = matrix4::Identity;
			return E_INVALIDARG;
		}
		else {

			double fDetInv = 1.0 / fDet;

			q_11 =  fDetInv * ( a_22 * a_33 - a_23 * a_32 );
			q_12 = -fDetInv * ( a_12 * a_33 - a_13 * a_32 );
			q_13 =  fDetInv * ( a_12 * a_23 - a_13 * a_22 );
			q._14 = 0.0f;

			q_21 = -fDetInv * ( a_21 * a_33 - a_23 * a_31 );
			q_22 =  fDetInv * ( a_11 * a_33 - a_13 * a_31 );
			q_23 = -fDetInv * ( a_11 * a_23 - a_13 * a_21 );
			q._24 = 0.0f;

			q_31 =  fDetInv * ( a_21 * a_32 - a_22 * a_31 );
			q_32 = -fDetInv * ( a_11 * a_32 - a_12 * a_31 );
			q_33 =  fDetInv * ( a_11 * a_22 - a_12 * a_21 );
			q._34 = 0.0f;

			q._41 = ( float ) -( a_41 * q_11 + a_42 * q_21 + a_43 * q_31 );
			q._42 = ( float ) -( a_41 * q_12 + a_42 * q_22 + a_43 * q_32 );
			q._43 = ( float ) -( a_41 * q_13 + a_42 * q_23 + a_43 * q_33 );
			q._44 = 1.0f;


			q._11 = ( float )q_11;
			q._12 = ( float )q_12;
			q._13 = ( float )q_13;

			q._21 = ( float )q_21;
			q._22 = ( float )q_22;
			q._23 = ( float )q_23;

			q._31 = ( float )q_31;
			q._32 = ( float )q_32;
			q._33 = ( float )q_33;


			return S_OK;
		}		
	
#else
		


		FLOAT fDet =		   ( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
								 a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
								 a._13 * ( a._21 * a._32 - a._22 * a._31 ) );

		if( fabs( fDet ) < 1.0e-30 ) {
			q = matrix4::Identity;
			return E_INVALIDARG;
		}
		else {

			FLOAT fDetInv = 1.0f / fDet;

			q._11 =  fDetInv * ( a._22 * a._33 - a._23 * a._32 );
			q._12 = -fDetInv * ( a._12 * a._33 - a._13 * a._32 );
			q._13 =  fDetInv * ( a._12 * a._23 - a._13 * a._22 );
			q._14 = 0.0f;

			q._21 = -fDetInv * ( a._21 * a._33 - a._23 * a._31 );
			q._22 =  fDetInv * ( a._11 * a._33 - a._13 * a._31 );
			q._23 = -fDetInv * ( a._11 * a._23 - a._13 * a._21 );
			q._24 = 0.0f;

			q._31 =  fDetInv * ( a._21 * a._32 - a._22 * a._31 );
			q._32 = -fDetInv * ( a._11 * a._32 - a._12 * a._31 );
			q._33 =  fDetInv * ( a._11 * a._22 - a._12 * a._21 );
			q._34 = 0.0f;

			q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
			q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
			q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
			q._44 = 1.0f;
		}
		return S_OK;
#endif
	
	}	
	
	
}

#elif 0

// N.B. move to our own math lib soon
HRESULT MatrixInvert( matrix4& q, matrix4& a )
{

    if( fabs(a._44 - 1.0f) > .001f)
        return E_INVALIDARG;
    if( fabs(a._14) > .001f || fabs(a._24) > .001f || fabs(a._34) > .001f )
        return E_INVALIDARG;

    FLOAT fDetInv = 1.0f / ( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
                             a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
                             a._13 * ( a._21 * a._32 - a._22 * a._31 ) );

    q._11 =  fDetInv * ( a._22 * a._33 - a._23 * a._32 );
    q._12 = -fDetInv * ( a._12 * a._33 - a._13 * a._32 );
    q._13 =  fDetInv * ( a._12 * a._23 - a._13 * a._22 );
    q._14 = 0.0f;

    q._21 = -fDetInv * ( a._21 * a._33 - a._23 * a._31 );
    q._22 =  fDetInv * ( a._11 * a._33 - a._13 * a._31 );
    q._23 = -fDetInv * ( a._11 * a._23 - a._13 * a._21 );
    q._24 = 0.0f;

    q._31 =  fDetInv * ( a._21 * a._32 - a._22 * a._31 );
    q._32 = -fDetInv * ( a._11 * a._32 - a._12 * a._31 );
    q._33 =  fDetInv * ( a._11 * a._22 - a._12 * a._21 );
    q._34 = 0.0f;

    q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
    q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
    q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
    q._44 = 1.0f;

    return S_OK;
}

#elif 0
HRESULT MatrixInvert( matrix4& q, matrix4& a )
{
	float *mat = (float *) &a;
	float *dst = (float *) &q;

double tmp[12]; /* temp array for pairs */
double src[16]; /* array of transpose source matrix */
double det; /* determinant */
/* transpose matrix */
for ( int i = 0; i < 4; i++) {
src[i] = mat[i*4];
src[i + 4] = mat[i*4 + 1];
src[i + 8] = mat[i*4 + 2];
src[i + 12] = mat[i*4 + 3];
}
/* calculate pairs for first 8 elements (cofactors) */
tmp[0] = src[10] * src[15];
tmp[1] = src[11] * src[14];
tmp[2] = src[9] * src[15];
tmp[3] = src[11] * src[13];
tmp[4] = src[9] * src[14];
tmp[5] = src[10] * src[13];
tmp[6] = src[8] * src[15];
tmp[7] = src[11] * src[12];
tmp[8] = src[8] * src[14];
tmp[9] = src[10] * src[12];
tmp[10] = src[8] * src[13];
tmp[11] = src[9] * src[12];
/* calculate first 8 elements (cofactors) */
dst[0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
dst[1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
dst[2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
dst[3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
dst[4] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
dst[5] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
dst[6] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
dst[7] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
/* calculate pairs for second 8 elements (cofactors) */
tmp[0] = src[2]*src[7];
tmp[1] = src[3]*src[6];
tmp[2] = src[1]*src[7];
tmp[3] = src[3]*src[5];
tmp[4] = src[1]*src[6];
tmp[5] = src[2]*src[5];
tmp[6] = src[0]*src[7];
tmp[7] = src[3]*src[4];
tmp[8] = src[0]*src[6];
tmp[9] = src[2]*src[4];
tmp[10] = src[0]*src[5];
tmp[11] = src[1]*src[4];
/* calculate second 8 elements (cofactors) */
dst[8] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
dst[9] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
/* calculate determinant */
det=src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];
/* calculate matrix inverse */
det = 1/det;
for ( int j = 0; j < 16; j++)
dst[j] *= det;

	return S_OK;
}
#else
/*  

  From "Matrices and Excuses"
  http://www-scf.usc.edu/~akotaobi/gptut12.html

	Let matrix M = [ a1 a2 a3 0 ]
                 [ b1 b2 b3 0 ]
                 [ c1 c2 c3 0 ]
                 [ d1 d2 d3 1 ]

  Set vector i = (a1, a2, a3)
  Set vector j = (b1, b2, b3)
  Set vector k = (c1, c2, c3)
  Set vector p = (d1, d2, d3)

  Inverse matrix M_inv = [  a1    b1    c1   0 ]
                         [  a2    b2    c2   0 ]
                         [  a3    b3    c3   0 ]
                         [ -p•i  -p•j  -p•k  1 ]
*/
HRESULT MatrixInvert( matrix4& q, matrix4& a )
{
	// Make sure last column is OK
    if( fabs(a._44 - 1.0f) > .001f)
        return E_INVALIDARG;
    if( fabs(a._14) > .001f || fabs(a._24) > .001f || fabs(a._34) > .001f )
        return E_INVALIDARG;

	Point3 i(a._11, a._12, a._13);
	Point3 j(a._21, a._22, a._23);
	Point3 k(a._31, a._32, a._33);
	Point3 p(a._41, a._42, a._43);

	// Transpose 3x3
	q._11 = a._11;
	q._12 = a._21;
	q._13 = a._31;
	q._21 = a._12;
	q._22 = a._22;
	q._23 = a._32;
	q._31 = a._13;
	q._32 = a._23;
	q._33 = a._33;

	// Calc translation row
	q._41 = -p * i;
	q._42 = -p * j;
	q._43 = -p * k;

	// Fill in last column
	q._14 = 0.f;
	q._24 = 0.f;
	q._34 = 0.f;
	q._44 = 1.f;

	return S_OK;
}
#endif


#endif  // METHODSTILL_HERE


HRESULT MatrixTranspose( matrix4& q, matrix4& a )
{
	// copy the diagonal
	q._11 = a._11;
	q._22 = a._22;
	q._33 = a._33;
	q._44 = a._44;

	// reflect everything else
	q._12 = a._21;
	q._13 = a._31;
	q._14 = a._41;
	q._21 = a._12;
	q._23 = a._32;
	q._24 = a._42;
	q._31 = a._13;
	q._32 = a._23;
	q._34 = a._43;
	q._41 = a._14;
	q._42 = a._24;
	q._43 = a._34;

	return S_OK;
}

void CAnmTransform::GetLocalMatrix(CAnmGroup *pTargetGroup,
										  CAnmGroup **pParents, int nParents, matrix4 *pMatReturn)
{
	// Make a world matrix from this top-down list of groups
	// Invert it and you've got the local matrix

	*pMatReturn = matrix4::Identity;

	for (int i = 0; i < nParents; i++)
	{
		CAnmGroup *pGroup = pParents[i];

		if (pGroup->GetClass()->IsA(GETCLASS(CAnmTransform)))
		{
			matrix4 *pMat = ((CAnmTransform *) pGroup)->GetMatrix();
			*pMatReturn = (*pMat) * (*pMatReturn);
		}

		if (pGroup == pTargetGroup)
			break;
	}

}

inline BOOL IntersectRay(Point3 from, Point3 dir, float radius)
{
	// N.B.: I'm not sure this works completely. Test it thoroughly!
	// Taken from http://cas00.bu.edu/course/cs580/vtek/p2/sphere.cpp

	double t, descriminant, b, c, r2;
	
	r2 = radius * radius;

	// a = d.x^2 + d.y^2 +d.z^2 = 1 since d is a unit vector
	b = dir * from * 2;
	c = from * from - (from * Point3(0, 0, 0)) - r2;

	descriminant = b * b - 4 * c;

	// check for intersection with sphere
	if (descriminant < 0)
		return FALSE;

	descriminant = sqrt(descriminant);

	// calculate the closer intersection point at t
	t = (-b - descriminant) * .5;

	// if closer intersection point is behind o,d, calculate other point
	if (t < 0){

		t += descriminant; // -b/2 + desc/2 = -b/2 - desc/2 (+ desc) 
		
		// if sphere is totally behind ray
		if (t < 0)
			return FALSE;
	}

	return TRUE;
}


float CAnmTransform::GetRadius()
{
	// get radius of group of children
	float rad = CAnmGroup::GetRadius();

	// transform it with our current matrix
	Point3 p3(rad, 0, 0);

	p3 = m_matrix * p3;

	return p3.Mag();
}

void CAnmTransform::GetBoundingBox(CAnmBoundingBox *pBoundingBox)
{
	// N.B.: this really needs to be optimized with a dirty flag;
	// we're calcing this every frame right now

	// get bbox of group (untransformed)
	CAnmBoundingBox b;
	CAnmGroup::GetBoundingBox(&b);

	if (b.IsValid())
	{
		m_bbox = b.Transform(m_matrix);
	}

	*pBoundingBox = m_bbox;
}

// Accessors
void CAnmTransform::SetCenter(point3 p)
{
	m_center = p;

	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmTransform, center_changed), &p);
}

void CAnmTransform::SetCenter(float x, float y, float z)
{
	point3 p(x, y, z);
	SetCenter(p);
}

void CAnmTransform::SetTranslation(point3 p)
{
	m_translation = p;

	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmTransform, translation_changed), &p);
}

void CAnmTransform::SetTranslation(float x, float y, float z)
{
	point3 p(x, y, z);
	SetTranslation(p);
}


void CAnmTransform::SetScale(point3 p)
{
	m_scale = p;

	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmTransform, scale_changed), &p);
}

void CAnmTransform::SetScale(float x, float y, float z)
{
	point3 p(x, y, z);
	SetScale(p);
}



void CAnmTransform::SetRotation(sAnmRotation r)
{
	m_rotation = r;

	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmTransform, rotation_changed), &r);

}

void CAnmTransform::SetRotation(float x, float y, float z, float theta)
{
	sAnmRotation r(x, y, z, theta);
	SetRotation(r);
}

void CAnmTransform::SetScaleOrientation(sAnmRotation r)
{
	m_scaleOrientation = r;

	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmTransform, scaleOrientation_changed), &r);

}

void CAnmTransform::SetScaleOrientation(float x, float y, float z, float theta)
{
	sAnmRotation r(x, y, z, theta);
	SetScaleOrientation(r);
}

void CAnmTransform::UpdateMatrix(matrix4 &matrix,
		const Point3 &translation,
		const Rotation &rotation,
		const Point3 &scale,
		const Rotation &scaleOrientation,
		const Point3 &center)
{
	matrix4 mat;
	matrix = matrix4::Identity;

	// try the easy case first; no extra rotations
	if (center.x == 0.f && center.y == 0.f && center.z == 0.f && scaleOrientation.theta == 0.f)
	{
		// apply the components of the transform, in the proper order
		// Scale
		matrix.ToScale(scale);

		// Rotation
		mat.ToAxisAngle(rotation.vec, rotation.theta);
		matrix = matrix * mat;

		// Translation
		mat.ToTranslation(translation);
		matrix = matrix * mat;


	}
	else
	{
		// apply the components of the transform, in the proper order

		// translate from center
		matrix.ToTranslation(-(Point3) center);
	
		// rotation about the scaleOrientation vector
		mat.ToAxisAngle(scaleOrientation.vec, -scaleOrientation.theta);
		matrix = matrix * mat;

		// now scale it
		mat.ToScale(scale);
		matrix = matrix * mat;

		// rotate back from scaleOrientation
		mat.ToAxisAngle(scaleOrientation.vec, scaleOrientation.theta);
		matrix = matrix * mat;

		// now rotate by rotation value
		mat.ToAxisAngle(rotation.vec, rotation.theta);
		matrix = matrix * mat;

		// translate back to center
		mat.ToTranslation(center);
		matrix = matrix * mat;

		// finally (whew) add in translation
		mat.ToTranslation(translation);
		matrix = matrix * mat;
	}
}

void CAnmTransform::Update()
{
	UpdateMatrix(m_matrix, m_translation, m_rotation, m_scale, m_scaleOrientation, m_center);

	// Ok, our general state bit is clear, but we'll still have a dirty matrix bit
	// to signal that we need to re-traverse
	ClearStateDirty();

}

bool CAnmTransform::m_drawBoxes = false;
void CAnmTransform::SetDrawBoxes(bool drawBoxes)
{
	m_drawBoxes = drawBoxes;
}

bool CAnmTransform::GetDrawBoxes()
{
	return m_drawBoxes;
}

	

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmTransform, bboxCenter, CAnmGroup);
DEFINE_INHERITED_VALUE(CAnmTransform, bboxSize, CAnmGroup);
DEFINE_INHERITED_VALUE(CAnmTransform, children, CAnmGroup);
DEFINE_VALUE(CAnmTransform, center, eValuePoint3, Point3, GetCenter, SetCenter);
DEFINE_VALUE(CAnmTransform, translation, eValuePoint3, Point3, GetTranslation, SetTranslation);
DEFINE_VALUE(CAnmTransform, scale, eValuePoint3, Point3, GetScale, SetScale);
DEFINE_VALUE(CAnmTransform, rotation, eValueRotation, Rotation, GetRotation, SetRotation);
DEFINE_VALUE(CAnmTransform, scaleOrientation, eValueRotation, Rotation, GetScaleOrientation, SetScaleOrientation);

DEFINE_INHERITED_METHOD(CAnmTransform, addChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmTransform, removeChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmTransform, set_children, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmTransform, set_bboxCenter, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmTransform, set_bboxSize, CAnmGroup);
DEFINE_METHOD(CAnmTransform, set_center, eValuePoint3, Point3, SetCenter);
DEFINE_METHOD(CAnmTransform, set_translation, eValuePoint3, Point3, SetTranslation);
DEFINE_METHOD(CAnmTransform, set_scale, eValuePoint3, Point3, SetScale);
DEFINE_METHOD(CAnmTransform, set_rotation, eValueRotation, Rotation, SetRotation);
DEFINE_METHOD(CAnmTransform, set_scaleOrientation, eValueRotation, Rotation, SetScaleOrientation);

DEFINE_INHERITED_CALLBACK(CAnmTransform, bboxCenter_changed, CAnmGroup);
DEFINE_INHERITED_CALLBACK(CAnmTransform, bboxSize_changed, CAnmGroup);
DEFINE_INHERITED_CALLBACK(CAnmTransform, children_changed, CAnmGroup);
DEFINE_CALLBACK(CAnmTransform, center_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmTransform, translation_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmTransform, scale_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmTransform, rotation_changed, eValueRotation);
DEFINE_CALLBACK(CAnmTransform, scaleOrientation_changed, eValueRotation);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmTransform)
VALUEID(CAnmTransform, bboxCenter),
VALUEID(CAnmTransform, bboxSize),
VALUEID(CAnmTransform, children),
VALUEID(CAnmTransform, center),
VALUEID(CAnmTransform, translation),
VALUEID(CAnmTransform, scale),
VALUEID(CAnmTransform, rotation),
VALUEID(CAnmTransform, scaleOrientation),

METHODID(CAnmTransform, addChildren),
METHODID(CAnmTransform, removeChildren),
METHODID(CAnmTransform, set_bboxCenter),
METHODID(CAnmTransform, set_bboxSize),
METHODID(CAnmTransform, set_children),
METHODID(CAnmTransform, set_center),
METHODID(CAnmTransform, set_translation),
METHODID(CAnmTransform, set_scale),
METHODID(CAnmTransform, set_rotation),
METHODID(CAnmTransform, set_scaleOrientation),

CALLBACKID(CAnmTransform, bboxCenter_changed),
CALLBACKID(CAnmTransform, bboxSize_changed),
CALLBACKID(CAnmTransform, children_changed),
CALLBACKID(CAnmTransform, center_changed),
CALLBACKID(CAnmTransform, translation_changed),
CALLBACKID(CAnmTransform, scale_changed),
CALLBACKID(CAnmTransform, rotation_changed),
CALLBACKID(CAnmTransform, scaleOrientation_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmTransform, CAnmGroup);

