/********************************************************************************
 * Flux
 *
 * File: anmsaimatrix.cpp
 * Description: Matrix wrapper classes for SAI
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
#include "anmsaimatrix.h"
#include "anmquaternion.h"
#include "anmtransform.h"
#include "anmMatrixInvert.h"

// Constructor/destructor
CAnmSAIMatrix4::CAnmSAIMatrix4(DoubleArray *pValues)
{
	m_matrix = matrix4::Identity;

	float *fp = GetValues();
	int sz = pValues->size();
	for (int i = 0; i < sz; i++)
	{
		fp[i] = (*pValues)[i];
	}
}

CAnmSAIMatrix4::CAnmSAIMatrix4(matrix4 &mat)
{
	m_matrix = mat;
}

// New methods
void CAnmSAIMatrix4::setTransform(
		Point3 translation,
		Rotation rotation,
		Point3 scale,
		Rotation scaleOrientation,
		Point3 center)
{
	CAnmTransform::UpdateMatrix(m_matrix, translation, rotation, scale, scaleOrientation, center);
}


void CAnmSAIMatrix4::getTransform(
		Point3 &translation,
		Rotation &rotation,
		Point3 &scale)
{
	translation = Point3(m_matrix._41, m_matrix._42, m_matrix._43);
	scale = Point3(m_matrix._11, m_matrix._22, m_matrix._33);

	CAnmQuaternion q(m_matrix);
	rotation = q.AsRotation();
}

CAnmSAIMatrix4 *CAnmSAIMatrix4::inverse()
{
	matrix4 imat;
	MatrixInvert(imat, m_matrix);
	return new CAnmSAIMatrix4(imat);
}

extern HRESULT MatrixTranspose( matrix4& q, matrix4& a );
CAnmSAIMatrix4 *CAnmSAIMatrix4::transpose()
{
	matrix4 tmat;
	MatrixTranspose(tmat, m_matrix);
	return new CAnmSAIMatrix4(tmat);
}

CAnmSAIMatrix4 *CAnmSAIMatrix4::multLeft(CAnmSAIMatrix4 *matrix)
{
	matrix4 result = (*(matrix->GetMatrix())) * m_matrix;
	return new CAnmSAIMatrix4(result);
}

CAnmSAIMatrix4 *CAnmSAIMatrix4::multRight(CAnmSAIMatrix4 *matrix)
{
	matrix4 result = m_matrix * (*(matrix->GetMatrix()));
	return new CAnmSAIMatrix4(result);
}

Point3 CAnmSAIMatrix4::multVecMatrix(const Point3 &vec)
{
	return m_matrix * vec;
}

Point3 CAnmSAIMatrix4::multMatrixVec(const Point3 &vec)
{
	return vec * m_matrix;
}

#define MAXSTRBUF 1024	// should be more than enough?

String CAnmSAIMatrix4::toString()
{
	char buf[MAXSTRBUF];

	float *fp = GetValues();

	sprintf(buf, "%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g",
		(double) fp[0],
		(double) fp[1],
		(double) fp[2],
		(double) fp[3],
		(double) fp[4],
		(double) fp[5],
		(double) fp[6],
		(double) fp[7],
		(double) fp[8],
		(double) fp[9],
		(double) fp[10],
		(double) fp[11],
		(double) fp[12],
		(double) fp[13],
		(double) fp[14],
		(double) fp[15]);

	return new CAnmString(buf);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_ABSTRACT_EMPTY_CLASS(CAnmSAIMatrix4, CAnmObject)
