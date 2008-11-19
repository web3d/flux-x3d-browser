/********************************************************************************
 * Flux
 *
 * File: anmsaimatrix.h
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

#ifndef _anmsaimatrix_h
#define _anmsaimatrix_h

#include "matrix4.h"
#include "anmtransform.h"

class EXPORT CAnmSAIMatrix4 : public CAnmObject
{
protected:

	matrix4										m_matrix;

	float *GetValues()
	{
		return (float *) &m_matrix;
	}

	matrix4 *GetMatrix()
	{
		return &m_matrix;
	}

public:

	// Constructor/destructor
	CAnmSAIMatrix4(DoubleArray *pValues);
	CAnmSAIMatrix4(matrix4 &mat);
	~CAnmSAIMatrix4()
	{
	}

	// New methods
	void setTransform(
		Point3 translation = ANMTRANSFORM_DEFAULT_TRANSLATION,
		Rotation rotation = ANMTRANSFORM_DEFAULT_ROTATION,
		Point3 scale = ANMTRANSFORM_DEFAULT_SCALE,
		Rotation scaleOrientation = ANMTRANSFORM_DEFAULT_SCALEORIENTATION,
		Point3 center = ANMTRANSFORM_DEFAULT_CENTER);

	void getTransform(
		Point3 &translation,
		Rotation &rotation,
		Point3 &scale);

	CAnmSAIMatrix4 *inverse();
	CAnmSAIMatrix4 *transpose();
	CAnmSAIMatrix4 *multLeft(CAnmSAIMatrix4 *matrix);
	CAnmSAIMatrix4 *multRight(CAnmSAIMatrix4 *matrix);
	Point3 multVecMatrix(const Point3 &vec);
	Point3 multMatrixVec(const Point3 &vec);
	String toString();

	float &operator[](unsigned int index)
	{
		float *fp = GetValues();

		assert (index >= 0 && index < 16);
		return fp[index];
	}

	// Accessors

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmSAIMatrix4);
};


#endif // _anmsaimatrix_h
