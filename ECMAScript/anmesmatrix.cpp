/********************************************************************************
 * Flux
 *
 * File: anmesmatrix.cpp
 * Description: ECMAScript matrix objects
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
#include "anmesmatrix.h"
#include "anmesfields.h"
#include "anmesmessage.h"
#include "anmesstring.h"
#include "anmfields.h"
#include "anmsaimatrix.h"

// Constructor/destructor
CAnmESMatrix4::CAnmESMatrix4(CAnmSAIMatrix4 *pMatrix, CAnmECMAScriptLanguageBinding *pOwner)
: CAnmESBuiltinObject(pMatrix, pOwner)
{
}

CAnmESMatrix4::~CAnmESMatrix4()
{
}

CAnmESObject *CAnmESMatrix4::Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	DoubleArray *values = new DoubleArray(16);

	for (int i = 0; i < nargs && i < 16; i++)
	{
		values->push_back((double) args[i]);
	}
	
	CAnmSAIMatrix4 *pObj = new CAnmSAIMatrix4(values);

	delete values;

	return new CAnmESMatrix4(pObj, pOwner);
}

AnmESVar CAnmESMatrix4::setTransform(int nArgs, AnmESVar *args)
{
	CAnmSAIMatrix4 *pMatrix = (CAnmSAIMatrix4 *) m_object;

	Point3 translation = ANMTRANSFORM_DEFAULT_TRANSLATION;
	Rotation rotation = ANMTRANSFORM_DEFAULT_ROTATION;
	Point3 scale = ANMTRANSFORM_DEFAULT_SCALE;
	Rotation scaleOrientation = ANMTRANSFORM_DEFAULT_SCALEORIENTATION;
	Point3 center = ANMTRANSFORM_DEFAULT_CENTER;

	CAnmSFVec3f *pVec = NULL;
	CAnmSFRotation *pRot = NULL;

	if (nArgs > 0)
	{
		pVec = args[0];
		if (pVec)
			translation = * pVec->GetValue();
	}
	if (nArgs > 1)
	{
		pRot = args[1];
		if (pRot)
			rotation = * pRot->GetValue();
	}
	if (nArgs > 2)
	{
		pVec = args[2];
		if (pVec)
			scale = * pVec->GetValue();
	}
	if (nArgs > 3)
	{
		pRot = args[3];
		if (pRot)
			scaleOrientation = * pRot->GetValue();
	}
	if (nArgs > 4)
	{
		pVec = args[4];
		if (pVec)
			center = * pVec->GetValue();
	}
	
	pMatrix->setTransform(translation, rotation, scale, scaleOrientation, center);

	return (double) 0.f;
}

AnmESVar CAnmESMatrix4::getTransform(int nArgs, AnmESVar *args)
{
	CAnmSAIMatrix4 *pMatrix = (CAnmSAIMatrix4 *) m_object;

	Point3 translation;
	Rotation rotation;
	Point3 scale;
	pMatrix->getTransform(translation, rotation, scale);

	if (nArgs > 0)
	{
		CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) args[0];
		if (pVec3f)
			pVec3f->SetValue(translation);
	}
	if (nArgs > 1)
	{
		CAnmSFRotation *pRot = (CAnmSFRotation *) args[1];
		if (pRot)
			pRot->SetValue(rotation);
	}
	if (nArgs > 2)
	{
		CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) args[2];
		if (pVec3f)
			pVec3f->SetValue(scale);
	}

	return (double) 0.f;
}

AnmESVar CAnmESMatrix4::inverse(int nArgs, AnmESVar *args)
{
	CAnmSAIMatrix4 *pMatrix = (CAnmSAIMatrix4 *) m_object;

	CAnmSAIMatrix4 *pInvMatrix = pMatrix->inverse();
	if (pInvMatrix)
		return new CAnmESMatrix4(pInvMatrix, m_owner);
	else
		return NULL;
}

AnmESVar CAnmESMatrix4::transpose(int nArgs, AnmESVar *args)
{
	CAnmSAIMatrix4 *pMatrix = (CAnmSAIMatrix4 *) m_object;

	CAnmSAIMatrix4 *pTransMatrix = pMatrix->transpose();
	if (pTransMatrix)
		return new CAnmESMatrix4(pTransMatrix, m_owner);
	else
		return NULL;
}

void CAnmESMatrix4::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmSAIMatrix4 *pMatrix = (CAnmSAIMatrix4 *) m_object;

	if (index >= 0 && index < 16)
	{
		(*pMatrix)[index] = (float) (double) propValue;
	}
	else
		// ANMMSG_ESMATRIXINDEXOOB : "Matrix4: index out of bounds"
		throw CAnmError(ANMMSG_ESMATRIXINDEXOOB);
}

AnmESVar CAnmESMatrix4::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;

	CAnmSAIMatrix4 *pMatrix = (CAnmSAIMatrix4 *) m_object;

	AnmESVar v((double) 0.f);

	if (index >= 0 && index < 16)
	{
		return (*pMatrix)[index];
	}
	else
		// ANMMSG_ESMATRIXINDEXOOB : "Matrix4: index out of bounds"
		throw CAnmError(ANMMSG_ESMATRIXINDEXOOB);

	return v;
}

AnmESVar CAnmESMatrix4::multLeft(int nArgs, AnmESVar *args)
{
	CAnmSAIMatrix4 *pMatrix = (CAnmSAIMatrix4 *) m_object;

	AnmESVar v((double) 0.f);

	if (nArgs > 0)
	{
		CAnmSAIMatrix4 *pLeftMatrix = (CAnmSAIMatrix4 *) args[0];
		if (pLeftMatrix)
		{
			CAnmSAIMatrix4 *pResult = pMatrix->multLeft(pLeftMatrix);

			return new CAnmESMatrix4(pResult, m_owner);
		}
	}
	else
		// ANMMSG_ESMATRIXARGSMULTLEFT : "Wrong number of arguments to Matrix4::multLeft"
		throw CAnmError(ANMMSG_ESMATRIXARGSMULTLEFT);

	return v;
}

AnmESVar CAnmESMatrix4::multRight(int nArgs, AnmESVar *args)
{
	CAnmSAIMatrix4 *pMatrix = (CAnmSAIMatrix4 *) m_object;

	AnmESVar v((double) 0.f);

	if (nArgs > 0)
	{
		CAnmSAIMatrix4 *pRightMatrix = (CAnmSAIMatrix4 *) args[0];
		if (pRightMatrix)
		{
			CAnmSAIMatrix4 *pResult = pMatrix->multRight(pRightMatrix);

			return new CAnmESMatrix4(pResult, m_owner);
		}
	}
	else
		// ANMMSG_ESMATRIXARGSMULTRIGHT : "Wrong number of arguments to Matrix4::multRight"
		throw CAnmError(ANMMSG_ESMATRIXARGSMULTRIGHT);

	return v;
}

AnmESVar CAnmESMatrix4::multVecMatrix(int nArgs, AnmESVar *args)
{
	CAnmSAIMatrix4 *pMatrix = (CAnmSAIMatrix4 *) m_object;

	AnmESVar v((double) 0.f);

	if (nArgs > 0)
	{
		Point3 vec = (Point3) args[0];

		Point3 result = pMatrix->multVecMatrix(vec);
		
		CAnmField *pField = CAnmField::CreateField(eValuePoint3, &result);
		return AnmESVarFromField(pField, m_owner);
	}
	else
		// ANMMSG_ESMATRIXARGSMULTVM : "Wrong number of arguments to Matrix4::multVecMatrix"
		throw CAnmError(ANMMSG_ESMATRIXARGSMULTVM);

	return v;
}

AnmESVar CAnmESMatrix4::multMatrixVec(int nArgs, AnmESVar *args)
{
	CAnmSAIMatrix4 *pMatrix = (CAnmSAIMatrix4 *) m_object;

	AnmESVar v((double) 0.f);

	if (nArgs > 0)
	{
		Point3 vec = (Point3) args[0];

		Point3 result = pMatrix->multMatrixVec(vec);
		
		CAnmField *pField = CAnmField::CreateField(eValuePoint3, &result);
		return AnmESVarFromField(pField, m_owner);
	}
	else
		// ANMMSG_ESMATRIXARGSMULTMV : "Wrong number of arguments to Matrix4::multMatrixVec"
		throw CAnmError(ANMMSG_ESMATRIXARGSMULTMV);

	return v;
}

AnmESVar CAnmESMatrix4::toString(int nArgs, AnmESVar *args)
{
	CAnmSAIMatrix4 *pMatrix = (CAnmSAIMatrix4 *) m_object;

	return (AnmESVar) new CAnmESString(pMatrix->toString(), m_owner);
}


// Tables for Callbacks, Methods and Put/GetProperties

// Matrix4 object
DEFINE_ESMETHOD(CAnmESMatrix4, setTransform);
DEFINE_ESMETHOD(CAnmESMatrix4, getTransform);
DEFINE_ESMETHOD(CAnmESMatrix4, inverse);
DEFINE_ESMETHOD(CAnmESMatrix4, transpose);
DEFINE_ESMETHOD(CAnmESMatrix4, multLeft);
DEFINE_ESMETHOD(CAnmESMatrix4, multRight);
DEFINE_ESMETHOD(CAnmESMatrix4, multVecMatrix);
DEFINE_ESMETHOD(CAnmESMatrix4, multMatrixVec);
DEFINE_ESMETHOD(CAnmESMatrix4, toString);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMatrix4)
ESMETHODID(CAnmESMatrix4, setTransform),
ESMETHODID(CAnmESMatrix4, getTransform),
ESMETHODID(CAnmESMatrix4, inverse),
ESMETHODID(CAnmESMatrix4, transpose),
ESMETHODID(CAnmESMatrix4, multLeft),
ESMETHODID(CAnmESMatrix4, multRight),
ESMETHODID(CAnmESMatrix4, multVecMatrix),
ESMETHODID(CAnmESMatrix4, multMatrixVec),
ESMETHODID(CAnmESMatrix4, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMatrix4, CAnmESBuiltinObject);
