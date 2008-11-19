/********************************************************************************
 * Flux
 *
 * File: anmesfields.cpp
 * Description: ECMAScript field objects
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
#include "anmesfields.h"
#include "anmesnode.h"
#include "anmesstring.h"
#include "anmfields.h"
#include "anmhsvcolor.h"
#include "anmquaternion.h"
#include "anmsainode.h"
#include "anmsymbol.h"

// Field objects
CAnmESFieldObject::CAnmESFieldObject(CAnmField *pFieldValue, CAnmECMAScriptLanguageBinding *pOwner)
: CAnmESBuiltinObject(pFieldValue, pOwner, eFieldObject)
{
	assert(pFieldValue);

	// N.B.: Remove a ref 'cause I think this is one too many; experimenting... TP 1/5/06
	// m_object->UnRef();
}

CAnmESFieldObject::CAnmESFieldObject(eValueType valueType, void *val, CAnmECMAScriptLanguageBinding *pOwner)
: CAnmESBuiltinObject(NULL, pOwner, eFieldObject)
{
	m_object = CAnmField::CreateField(valueType, val);

	// N.B.: I think this is one too many; experimenting... TP 1/5/06
	//m_object->Ref();
}

CAnmESFieldObject::~CAnmESFieldObject()
{
}

void *CAnmESFieldObject::GetValue()
{
	CAnmField *pField = GetField();
	return pField->GetData();
}

eValueType CAnmESFieldObject::GetValueType()
{
	CAnmField *pField = GetField();
	return pField->GetType();
}

void CAnmESFieldObject::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
}

AnmESVar CAnmESFieldObject::GetArrayProperty(AnmESVar vindex)
{
	AnmESVar v;
	return v;
}

// SFColor
CAnmESObject *CAnmESSFColor::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	Color c(0, 0, 0);

	for (int i = 0; i < nargs && i < 3; i++)
	{
		if (i == 0)
			c.r = (float) (double) args[i];
		else if (i == 1)
			c.g = (float) (double) args[i];
		else if (i == 2)
			c.b = (float) (double) args[i];
	}

	CAnmSFColor *pObj = new CAnmSFColor();
	pObj->SetValue(c);

	CAnmESObject *pESObject = new CAnmESSFColor(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}


// SFColor properties
void CAnmESSFColor::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;
	switch(index)
	{
		case 0 :
			setR(propValue);
			break;
		case 1 :
			setG(propValue);
			break;
		case 2 :
			setB(propValue);
			break;
	}
}

AnmESVar CAnmESSFColor::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	AnmESVar v((double) 0.f);
	switch(index)
	{
		case 0 :
			v = getR();
			break;
		case 1 :
			v = getG();
			break;
		case 2 :
			v = getB();
			break;
	}
	return v;
}

AnmESVar CAnmESSFColor::getR()
{
	CAnmSFColor *pColor = (CAnmSFColor *) m_object;

	Color c = * pColor->GetValue();

	return (AnmESVar) (double) c.r;
}

AnmESVar CAnmESSFColor::getG()
{
	CAnmSFColor *pColor = (CAnmSFColor *) m_object;

	Color c = * pColor->GetValue();

	return (AnmESVar) (double) c.g;
}

AnmESVar CAnmESSFColor::getB()
{
	CAnmSFColor *pColor = (CAnmSFColor *) m_object;

	Color c = * pColor->GetValue();

	return (AnmESVar) (double) c.b;
}

void CAnmESSFColor::setR(AnmESVar v)
{
	CAnmSFColor *pColor = (CAnmSFColor *) m_object;

	Color c = * pColor->GetValue();
	float f = (float) (double) v;
	c.r = f;
	pColor->SetValue(c);
}

void CAnmESSFColor::setG(AnmESVar v)
{
	CAnmSFColor *pColor = (CAnmSFColor *) m_object;

	Color c = * pColor->GetValue();
	float f = (float) (double) v;
	c.g = f;
	pColor->SetValue(c);
}

void CAnmESSFColor::setB(AnmESVar v)
{
	CAnmSFColor *pColor = (CAnmSFColor *) m_object;

	Color c = * pColor->GetValue();
	float f = (float) (double) v;
	c.b = f;
	pColor->SetValue(c);
}


// SFColor methods
// void setHSV(float h, float s, float v)
AnmESVar CAnmESSFColor::setHSV(int nArgs, AnmESVar *args)
{
	CAnmSFColor *pColor = (CAnmSFColor *) m_object;

	float h = 0.f, s = 0.f, v = 0.f;

	if (nArgs > 0)
		h = (float) (double) args[0];
	if (nArgs > 1)
		s = (float) (double) args[1];
	if (nArgs > 2)
		v = (float) (double) args[2];

	CAnmHSVColor hsv;
	hsv.h = h;
	hsv.s = s;
	hsv.v = v;

	Color c = hsv.AsColor();
	pColor->SetValue(c);

	AnmESVar ret;
	return ret;
}

// numeric[3] getHSV( )
AnmESVar CAnmESSFColor::getHSV(int nArgs, AnmESVar *args)
{
	// N.B. need to figure this out (returning user-define array object)
	AnmESVar v;
	return v;
}

// String toString( )
AnmESVar CAnmESSFColor::toString(int nArgs, AnmESVar *args)
{
	CAnmSFColor *pColor = (CAnmSFColor *) m_object;

	Color c = * pColor->GetValue();

	char buf[MAXTOSTRBUFLEN];
	sprintf(buf, "%f,%f,%f", (double) c.r, (double) c.g, (double) c.b);
	return (AnmESVar) new CAnmESString(new CAnmString(buf), m_owner);
}

// SFImage
CAnmESObject *CAnmESSFImage::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	Color c(0, 0, 0);

	for (int i = 0; i < nargs && i < 3; i++)
	{
		if (i == 0)
			c.r = (float) (double) args[i];
		else if (i == 1)
			c.g = (float) (double) args[i];
		else if (i == 2)
			c.b = (float) (double) args[i];
	}

	CAnmSFColor *pObj = new CAnmSFColor();
	pObj->SetValue(c);

	CAnmESObject *pESObject = new CAnmESSFColor(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}


// SFImage properties
AnmESVar CAnmESSFImage::getX()
{
	CAnmSFImage *pImage = (CAnmSFImage *) m_object;

	Image img = * pImage->GetValue();

	return (AnmESVar) (double) img.width;
}

AnmESVar CAnmESSFImage::getY()
{
	CAnmSFImage *pImage = (CAnmSFImage *) m_object;

	Image img = * pImage->GetValue();

	return (AnmESVar) (double) img.height;
}

AnmESVar CAnmESSFImage::getComp()
{
	CAnmSFImage *pImage = (CAnmSFImage *) m_object;

	Image img = * pImage->GetValue();

	return (AnmESVar) (double) img.numComponents;
}

AnmESVar CAnmESSFImage::getArray()
{
	CAnmSFImage *pImage = (CAnmSFImage *) m_object;

	Image img = * pImage->GetValue();

	IntegerArray *pIA = img.pixels;

	CAnmMFInt32 *pObj = new CAnmMFInt32();
	pObj->SetValue(pIA);
	
	// setvalue adds a ref
	pIA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFInt32(pObj, m_owner);

	pObj->UnRef();

	return pESObject;
}

void CAnmESSFImage::setX(AnmESVar v)
{
	CAnmSFImage *pImage = (CAnmSFImage *) m_object;

	Image img = * pImage->GetValue();
	img.width = (int) (double) v;
	pImage->SetValue(img);
}

void CAnmESSFImage::setY(AnmESVar v)
{
	CAnmSFImage *pImage = (CAnmSFImage *) m_object;

	Image img = * pImage->GetValue();
	img.height = (int) (double) v;
	pImage->SetValue(img);
}

void CAnmESSFImage::setComp(AnmESVar v)
{
	CAnmSFImage *pImage = (CAnmSFImage *) m_object;

	Image img = * pImage->GetValue();
	img.numComponents = (int) (double) v;
	pImage->SetValue(img);
}

void CAnmESSFImage::setArray(AnmESVar v)
{
	CAnmSFImage *pImage = (CAnmSFImage *) m_object;

	Image img = * pImage->GetValue();

	IntegerArray *pIA = (IntegerArray *) v;

	img.SetPixels(pIA);

	pImage->SetValue(img);
}

// SFImage methods
// String toString( )
AnmESVar CAnmESSFImage::toString(int nArgs, AnmESVar *args)
{
	CAnmSFColor *pColor = (CAnmSFColor *) m_object;

	Color c = * pColor->GetValue();

	char buf[MAXTOSTRBUFLEN];
	sprintf(buf, "%f,%f,%f", (double) c.r, (double) c.g, (double) c.b);
	return (AnmESVar) new CAnmESString(new CAnmString(buf), m_owner);
}

// SFNode
CAnmESObject *CAnmESSFNode::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	CAnmNode *pNode = NULL;


	if (nargs >= 1)
	{
		CAnmESNodeObject *pNodeObj = (CAnmESNodeObject *) args[0];
		if (pNodeObj)
		{
			CAnmSAINode *pSAINode = pNodeObj->GetNode();
			if (pSAINode)
			{
				pNode = pSAINode->GetNode();
			}
		}
	}

	CAnmSFNode *pObj = new CAnmSFNode;
	pObj->SetValue(pNode);

	CAnmESObject *pESObject = new CAnmESSFNode(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// String toString( )
AnmESVar CAnmESSFNode::toString(int nArgs, AnmESVar *args)
{
	CAnmSFNode *pSFNode = (CAnmSFNode *) m_object;

	CAnmNode *pNode = * pSFNode->GetValue();

	char buf[MAXTOSTRBUFLEN];
	if (pNode)
	{
		strcpy(buf, "(unknown node type)");
		CAnmClass *pClass = pNode->GetClass();
		if (pClass)
		{
			CAnmClassSymbol *pClassSym = pClass->m_classSymbol;
			if (pClassSym)
			{
				String s = pClassSym->GetName();
				const char *cp = s->GetBuf();
				if (strlen(cp))
					sprintf(buf, "%s {\n}", cp);
			}
		}
	}
	else
		strcpy(buf, "(null)");

	return (AnmESVar) new CAnmESString(new CAnmString(buf), m_owner);
}

// SFRotation
CAnmESObject *CAnmESSFRotation::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	Rotation r(0, 0, 1, 0);

	if (nargs > 0)
	{
		if (args[0].valueType == eObject)
		{
			CAnmSFVec3f *pSFVec3f = args[0];

			if (pSFVec3f && nargs > 1)
			{
				CAnmSFVec3f *pSFVec3f2 = args[1];

				if (pSFVec3f2)
				{
					// calculate rotation between two vectors
					Point3 v1 = * pSFVec3f->GetValue();
					Point3 v2 = * pSFVec3f2->GetValue();

					v1.Normalize();
					v2.Normalize();

					r.vec =  v1 ^ v2;
					r.theta = acos(v1 * v2);
				}
				else
				{
					// axis-angle
					if (args[1].valueType == eNumber)
					{
						r.vec = * pSFVec3f->GetValue();
						r.theta = (float) (double) args[1];
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < nargs && i < 4; i++)
			{
				if (i == 0)
					r.vec.x = (float) (double) args[i];
				else if (i == 1)
					r.vec.y = (float) (double) args[i];
				else if (i == 2)
					r.vec.z = (float) (double) args[i];
				else if (i == 3)
					r.theta = (float) (double) args[i];
			}
		}
	}


	CAnmSFRotation *pObj = new CAnmSFRotation();
	pObj->SetValue(r);

	CAnmESObject *pESObject = new CAnmESSFRotation(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// SFRotation properties
void CAnmESSFRotation::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;
	switch(index)
	{
		case 0 :
			setX(propValue);
			break;
		case 1 :
			setY(propValue);
			break;
		case 2 :
			setZ(propValue);
			break;
		case 3 :
			setAngle(propValue);
			break;
	}
}

AnmESVar CAnmESSFRotation::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	AnmESVar v((double) 0.f);
	switch(index)
	{
		case 0 :
			v = getX();
			break;
		case 1 :
			v = getY();
			break;
		case 2 :
			v = getZ();
			break;
		case 3 :
			v = getAngle();
			break;
	}
	return v;
}

AnmESVar CAnmESSFRotation::getX()
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();

	return (AnmESVar) (double) r.vec.x;
}

AnmESVar CAnmESSFRotation::getY()
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();

	return (AnmESVar) (double) r.vec.y;
}

AnmESVar CAnmESSFRotation::getZ()
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();

	return (AnmESVar) (double) r.vec.z;
}

AnmESVar CAnmESSFRotation::getAngle()
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();

	return (AnmESVar) (double) r.theta;
}

void CAnmESSFRotation::setX(AnmESVar v)
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();
	float f = (float) (double) v;
	r.vec.x = f;
	pRot->SetValue(r);
}

void CAnmESSFRotation::setY(AnmESVar v)
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();
	float f = (float) (double) v;
	r.vec.y = f;
	pRot->SetValue(r);
}

void CAnmESSFRotation::setZ(AnmESVar v)
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();
	float f = (float) (double) v;
	r.vec.z = f;
	pRot->SetValue(r);
}

void CAnmESSFRotation::setAngle(AnmESVar v)
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();
	float f = (float) (double) v;
	r.theta = f;
	pRot->SetValue(r);
}

// SFRotation methods
// SFVec3f getAxis( )
AnmESVar CAnmESSFRotation::getAxis(int nArgs, AnmESVar *args)
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();

	CAnmField *pField = CAnmField::CreateField(eValuePoint3, &r.vec);
	return AnmESVarFromField(pField, m_owner);
}

// SFRotation inverse( )
AnmESVar CAnmESSFRotation::inverse(int nArgs, AnmESVar *args)
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();

	// N.B.: still need to invert it
	CAnmField *pField = CAnmField::CreateField(eValueRotation, &r);
	return AnmESVarFromField(pField, m_owner);
}

// SFRotation multiply(SFRotation rot)
AnmESVar CAnmESSFRotation::multiply(int nArgs, AnmESVar *args)
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();

	Rotation r2(0, 0, 1, 0);
	if (nArgs > 0)
		r2 = (Rotation) args[0];

	// N.B.: still need to multiply it
	CAnmField *pField = CAnmField::CreateField(eValueRotation, &r);
	return AnmESVarFromField(pField, m_owner);
}

// SFVec3f multVec(SFVec3f vec)
AnmESVar CAnmESSFRotation::multVec(int nArgs, AnmESVar *args)
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();

	Point3 vec(0, 0, 0);
	if (nArgs > 0)
		vec = (Point3) args[0];

	matrix4 mat;
	mat.ToAxisAngle(r.vec, r.theta);
	vec = mat * vec;

	CAnmField *pField = CAnmField::CreateField(eValuePoint3, &vec);
	return AnmESVarFromField(pField, m_owner);
}

// void setAxis(SFVec3f vec)
AnmESVar CAnmESSFRotation::setAxis(int nArgs, AnmESVar *args)
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();

	Point3 vec(0, 0, 0);
	if (nArgs > 0)
		vec = (Point3) args[0];
	else
		// N.B.: runtime error?
		;

	r.vec = vec;
	pRot->SetValue(r);

	AnmESVar v;
	return v;
}

// SFRotation slerp(SFRotation dest, numeric t)
AnmESVar CAnmESSFRotation::slerp(int nArgs, AnmESVar *args)
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();

	Rotation dest(0, 0, 1, 0);
	if (nArgs > 0)
		dest = (Rotation) args[0];
	else
		// N.B.: runtime error?
		;

	float t = 0.f;
	if (nArgs > 1)
		t = (float) (double) args[1];

	CAnmQuaternion q1(r);
	CAnmQuaternion q2(dest);
	CAnmQuaternion q3;
	CAnmQuaternion::SphericalLinearInterp(t, &q1, &q2, &q3);
	Rotation result = q3.AsRotation();

	CAnmField *pField = CAnmField::CreateField(eValueRotation, &result);
	return AnmESVarFromField(pField, m_owner);
}

// String toString( )
AnmESVar CAnmESSFRotation::toString(int nArgs, AnmESVar *args)
{
	CAnmSFRotation *pRot = (CAnmSFRotation *) m_object;

	Rotation r = * pRot->GetValue();

	char buf[MAXTOSTRBUFLEN];
	sprintf(buf, "%f,%f,%f,%f", (double) r.vec.x, (double) r.vec.y, (double) r.vec.z, (double) r.theta);
	return (AnmESVar) new CAnmESString(new CAnmString(buf), m_owner);
}

// SFTime
CAnmESObject *CAnmESSFTime::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	double d = 0.f;

	if (nargs >= 1)
		d = (double) args[0];

	CAnmSFTime *pObj = new CAnmSFTime;
	pObj->SetValue(d);

	CAnmESObject *pESObject = new CAnmESSFTime(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// SFVec2f
CAnmESObject *CAnmESSFVec2f::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	Point2 p2(0, 0);
	for (int i = 0; i < nargs && i < 2; i++)
	{
		if (i == 0)
			p2.x = (float) (double) args[i];
		else if (i == 1)
			p2.y = (float) (double) args[i];
	}

	CAnmSFVec2f *pObj = new CAnmSFVec2f();
	pObj->SetValue(p2);

	CAnmESObject *pESObject = new CAnmESSFVec2f(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// SFVec2f properties
void CAnmESSFVec2f::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;
	switch(index)
	{
		case 0 :
			setX(propValue);
			break;
		case 1 :
			setY(propValue);
			break;
	}
}

AnmESVar CAnmESSFVec2f::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	AnmESVar v((double) 0.f);
	switch(index)
	{
		case 0 :
			v = getX();
			break;
		case 1 :
			v = getY();
			break;
	}
	return v;
}

AnmESVar CAnmESSFVec2f::getX()
{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	return (AnmESVar) (double) p2.x;
}

AnmESVar CAnmESSFVec2f::getY()
{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	return (AnmESVar) (double) p2.y;
}

void CAnmESSFVec2f::setX(AnmESVar v)
{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	float f = (float) (double) v;
	p2.x = f;
	pVec2f->SetValue(p2);
}

void CAnmESSFVec2f::setY(AnmESVar v)
{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	float f = (float) (double) v;
	p2.y = f;
	pVec2f->SetValue(p2);
}

// SFVec2f methods
// SFVec2f add(SFVec2f vec)
AnmESVar CAnmESSFVec2f::add(int nArgs, AnmESVar *args)
{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	Point2 p22(0, 0);
	if (nArgs > 0)
		p22 = (Point2) args[0];

	p2.x = p2.x + p22.x;
	p2.y = p2.y + p22.y;

	CAnmField *pField = CAnmField::CreateField(eValuePoint2, &p2);
	return AnmESVarFromField(pField, m_owner);
}

// SFVec2f divide(numeric n)
AnmESVar CAnmESSFVec2f::divide(int nArgs, AnmESVar *args)
{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	float f = 1.f;
	if (nArgs > 0)
		f = (float) (double) args[0];

	p2.x = p2.x / f;
	p2.y = p2.y / f;

	CAnmField *pField = CAnmField::CreateField(eValuePoint2, &p2);
	return AnmESVarFromField(pField, m_owner);
}

// numeric dot(SFVec2f vec)
AnmESVar CAnmESSFVec2f::dot(int nArgs, AnmESVar *args)

{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	Point2 p22(0, 0);
	if (nArgs > 0)
		p22 = (Point2) args[0];

	// N.B.: still need a 2D dot product
	double dot = 0.f; // p2 * p22;

	return (AnmESVar) dot;
}

// numeric length( )
AnmESVar CAnmESSFVec2f::length(int nArgs, AnmESVar *args)
{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	// N.B.: still need a 2D Mag function
	double mag = 1.f; // p2.Mag();

	return (AnmESVar) mag;
}

// SFVec2f multiply(numeric n)
AnmESVar CAnmESSFVec2f::multiply(int nArgs, AnmESVar *args)
{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	float f = 1.f;
	if (nArgs > 0)
		f = (float) (double) args[0];

	p2.x = p2.x * f;
	p2.y = p2.y * f;

	CAnmField *pField = CAnmField::CreateField(eValuePoint2, &p2);
	return AnmESVarFromField(pField, m_owner);
}

// SFVec2f normalize( )
AnmESVar CAnmESSFVec2f::normalize(int nArgs, AnmESVar *args)
{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	// N.B.: still need a normalize function
	p2 = Point2(0, 1);

	CAnmField *pField = CAnmField::CreateField(eValuePoint2, &p2);
	return AnmESVarFromField(pField, m_owner);
}

// SFVec2f subtract(SFVec2f vec)
AnmESVar CAnmESSFVec2f::subtract(int nArgs, AnmESVar *args)
{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	Point2 p22(0, 0);
	if (nArgs > 0)
		p22 = (Point2) args[0];

	p2.x = p2.x - p22.x;
	p2.y = p2.y - p22.y;

	CAnmField *pField = CAnmField::CreateField(eValuePoint2, &p2);
	return AnmESVarFromField(pField, m_owner);
}

// String toString( )
AnmESVar CAnmESSFVec2f::toString(int nArgs, AnmESVar *args)
{
	CAnmSFVec2f *pVec2f = (CAnmSFVec2f *) m_object;

	Point2 p2 = * pVec2f->GetValue();

	char buf[MAXTOSTRBUFLEN];
	sprintf(buf, "%f,%f", (double) p2.x, (double) p2.y);
	return (AnmESVar) new CAnmESString(new CAnmString(buf), m_owner);
}

// SFVec3f
CAnmESObject *CAnmESSFVec3f::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	Point3 p3(0, 0, 0);
	for (int i = 0; i < nargs && i < 3; i++)
	{
		p3.v[i] = (float) (double) args[i];
	}

	CAnmSFVec3f *pObj = new CAnmSFVec3f();
	pObj->SetValue(p3);

	CAnmESObject *pESObject = new CAnmESSFVec3f(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// SFVec3f properties
void CAnmESSFVec3f::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;
	switch(index)
	{
		case 0 :
			setX(propValue);
			break;
		case 1 :
			setY(propValue);
			break;
		case 2 :
			setZ(propValue);
			break;
	}
}

AnmESVar CAnmESSFVec3f::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	AnmESVar v((double) 0.f);
	switch(index)
	{
		case 0 :
			v = getX();
			break;
		case 1 :
			v = getY();
			break;
		case 2 :
			v = getZ();
			break;
	}
	return v;
}

AnmESVar CAnmESSFVec3f::getX()
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	return (AnmESVar) (double) p3.x;
}

AnmESVar CAnmESSFVec3f::getY()
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	return (AnmESVar) (double) p3.y;
}

AnmESVar CAnmESSFVec3f::getZ()
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	return (AnmESVar) (double) p3.z;
}

void CAnmESSFVec3f::setX(AnmESVar v)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	float f = (float) (double) v;
	p3.x = f;
	pVec3f->SetValue(p3);
}

void CAnmESSFVec3f::setY(AnmESVar v)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	float f = (float) (double) v;
	p3.y = f;
	pVec3f->SetValue(p3);
}

void CAnmESSFVec3f::setZ(AnmESVar v)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	float f = (float) (double) v;
	p3.z = f;
	pVec3f->SetValue(p3);
}


// SFVec3f methods
// SFVec3f add(SFVec3f vec)
AnmESVar CAnmESSFVec3f::add(int nArgs, AnmESVar *args)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	Point3 p32(0, 0, 0);
	if (nArgs > 0)
		p32 = (Point3) args[0];

	p3 = p3 + p32;

	CAnmField *pField = CAnmField::CreateField(eValuePoint3, &p3);
	return AnmESVarFromField(pField, m_owner);
}

// SFVec3f cross(SFVec3f vec)
AnmESVar CAnmESSFVec3f::cross(int nArgs, AnmESVar *args)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	Point3 p32(0, 0, 0);
	if (nArgs > 0)
		p32 = (Point3) args[0];

	p3 = p3 ^ p32;

	CAnmField *pField = CAnmField::CreateField(eValuePoint3, &p3);
	return AnmESVarFromField(pField, m_owner);
}


// SFVec3f divide(numeric n)
AnmESVar CAnmESSFVec3f::divide(int nArgs, AnmESVar *args)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	float f = 1.f;
	if (nArgs > 0)
		f = (float) (double) args[0];

	p3 = p3 / f;

	CAnmField *pField = CAnmField::CreateField(eValuePoint3, &p3);
	return AnmESVarFromField(pField, m_owner);
}

// numeric dot(SFVec3f vec)
AnmESVar CAnmESSFVec3f::dot(int nArgs, AnmESVar *args)

{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	Point3 p32(0, 0, 0);
	if (nArgs > 0)
		p32 = (Point3) args[0];

	double dot = p3 * p32;

	return (AnmESVar) dot;
}

// numeric length( )
AnmESVar CAnmESSFVec3f::length(int nArgs, AnmESVar *args)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	// N.B.: still need a 2D Mag function
	double mag = p3.Mag();

	return (AnmESVar) mag;
}

// SFVec3f multiply(numeric n)
AnmESVar CAnmESSFVec3f::multiply(int nArgs, AnmESVar *args)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	float f = 1.f;
	if (nArgs > 0)
		f = (float) (double) args[0];

	p3 = p3 * f;

	CAnmField *pField = CAnmField::CreateField(eValuePoint3, &p3);
	return AnmESVarFromField(pField, m_owner);
}

// SFVec3f normalize( )
AnmESVar CAnmESSFVec3f::normalize(int nArgs, AnmESVar *args)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	p3.Normalize();

	CAnmField *pField = CAnmField::CreateField(eValuePoint3, &p3);
	return AnmESVarFromField(pField, m_owner);
}

// SFVec3f negate( )
AnmESVar CAnmESSFVec3f::negate(int nArgs, AnmESVar *args)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	p3 = - p3;

	CAnmField *pField = CAnmField::CreateField(eValuePoint3, &p3);
	return AnmESVarFromField(pField, m_owner);
}

// SFVec3f subtract(SFVec2f vec)
AnmESVar CAnmESSFVec3f::subtract(int nArgs, AnmESVar *args)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();

	Point3 p32(0, 0, 0);
	if (nArgs > 0)
		p32 = (Point3) args[0];

	p3 = p3 - p32;

	CAnmField *pField = CAnmField::CreateField(eValuePoint3, &p3);
	return AnmESVarFromField(pField, m_owner);
}

// String toString( )
AnmESVar CAnmESSFVec3f::toString(int nArgs, AnmESVar *args)
{
	CAnmSFVec3f *pVec3f = (CAnmSFVec3f *) m_object;

	Point3 p3 = * pVec3f->GetValue();


	char buf[MAXTOSTRBUFLEN];
	sprintf(buf, "%f,%f,%f", (double) p3.x, (double) p3.y, (double) p3.z);
	return (AnmESVar) new CAnmESString(new CAnmString(buf), m_owner);
}

// MFBool
CAnmESObject *CAnmESMFBool::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	BooleanArray *pA = new BooleanArray;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		Boolean b = (Boolean) (int) args[i];
		pA->push_back(b);
	}

	CAnmMFBool *pObj = new CAnmMFBool();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFBool(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFBool properties
void CAnmESMFBool::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFBool *pBool = (CAnmMFBool*) m_object;

	BooleanArray **ppA = pBool->GetValue();
	assert(ppA);
	BooleanArray *pA = *ppA;

	if (pA)
		pA->Ref();
	else
		pA = new BooleanArray;
	
	int sz = pA->size();
	while (sz <= index)
	{
		pA->push_back(false);
		sz++;
	}

	Boolean b = (Boolean) (BOOL) propValue;

	(*pA)[index] = b;
	pBool->SetValue(pA);

	pA->UnRef();
}

AnmESVar CAnmESMFBool::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFBool *pBool = (CAnmMFBool*) m_object;

	BooleanArray **ppA = pBool->GetValue();
	assert(ppA);
	BooleanArray *pA = *ppA;
	int sz = pA->size();

	Boolean boolval = false;
	if (index < sz)
		boolval = (*pA)[index];

	return AnmESBool((BOOL) boolval);
}

AnmESVar CAnmESMFBool::getLength()
{
	CAnmMFBool *pBool = (CAnmMFBool*) m_object;

	BooleanArray **ppA = pBool->GetValue();
	assert(ppA);
	BooleanArray *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFBool::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFBool methods
// String toString( )
AnmESVar CAnmESMFBool::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}

// MFColor
CAnmESObject *CAnmESMFColor::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	ColorArray *pA = new ColorArray;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		Color c = (Color) args[i];
		pA->push_back(c);
	}

	CAnmMFColor *pObj = new CAnmMFColor();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFColor(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFColor properties
void CAnmESMFColor::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFColor *pColor = (CAnmMFColor *) m_object;

	ColorArray **ppA = pColor->GetValue();
	assert(ppA);
	ColorArray *pA = *ppA;

	if (pA)
		pA->Ref();
	else
		pA = new ColorArray;
	
	int sz = pA->size();
	while (sz <= index)
	{
		pA->push_back(Color(0, 0, 0));
		sz++;
	}

	Color c = (Color) propValue;

	(*pA)[index] = c;

	pColor->SetValue(pA);

	pA->UnRef();
}

AnmESVar CAnmESMFColor::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFColor *pColor = (CAnmMFColor *) m_object;

	ColorArray **ppA = pColor->GetValue();
	assert(ppA);
	ColorArray *pA = *ppA;
	int sz = pA->size();

	Color c(0, 0, 0);
	if (index < sz)
		c = (*pA)[index];

	CAnmField *pField = CAnmField::CreateField(eValueColor, &c);
	return AnmESVarFromField(pField, m_owner);
}

AnmESVar CAnmESMFColor::getLength()
{
	CAnmMFColor *pColor = (CAnmMFColor *) m_object;

	ColorArray **ppA = pColor->GetValue();
	assert(ppA);
	ColorArray *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFColor::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFColor methods
// String toString( )
AnmESVar CAnmESMFColor::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}

// MFDouble
CAnmESObject *CAnmESMFDouble::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	DoubleArray *pA = new DoubleArray;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		Double d = (Double) args[i];
		pA->push_back(d);
	}

	CAnmMFDouble *pObj = new CAnmMFDouble();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFDouble(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFDouble properties
void CAnmESMFDouble::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFDouble *pDouble = (CAnmMFDouble *) m_object;

	DoubleArray **ppA = pDouble->GetValue();
	assert(ppA);
	DoubleArray *pA = *ppA;

	if (pA)
		pA->Ref();
	else
		pA = new DoubleArray;
	
	int sz = pA->size();
	while (sz <= index)
	{
		pA->push_back(0.f);
		sz++;
	}

	Double d = (Double) propValue;

	(*pA)[index] = d;

	pDouble->SetValue(pA);

	pA->UnRef();
}

AnmESVar CAnmESMFDouble::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFDouble *pDouble = (CAnmMFDouble *) m_object;

	DoubleArray **ppA = pDouble->GetValue();
	assert(ppA);
	DoubleArray *pA = *ppA;
	int sz = pA->size();

	Double d = 0.f;
	if (index < sz)
		d = (*pA)[index];

	return (AnmESVar) d;
}

AnmESVar CAnmESMFDouble::getLength()
{
	CAnmMFDouble *pDouble = (CAnmMFDouble *) m_object;

	DoubleArray **ppA = pDouble->GetValue();
	assert(ppA);
	DoubleArray *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFDouble::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFDouble methods
// String toString( )
AnmESVar CAnmESMFDouble::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}


// MFFloat
CAnmESObject *CAnmESMFFloat::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	FloatArray *pA = new FloatArray;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		Float f = (Float) (double) args[i];
		pA->push_back(f);
	}

	CAnmMFFloat *pObj = new CAnmMFFloat();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFFloat(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFFloat properties
void CAnmESMFFloat::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFFloat *pFloat = (CAnmMFFloat *) m_object;

	FloatArray **ppA = pFloat->GetValue();
	assert(ppA);
	FloatArray *pA = *ppA;

	if (pA)
		pA->Ref();
	else
		pA = new FloatArray;
	
	int sz = pA->size();
	while (sz <= index)
	{
		pA->push_back(0.f);
		sz++;
	}

	Float f = (Float) (double) propValue;

	(*pA)[index] = f;

	pFloat->SetValue(pA);

	pA->UnRef();
}

AnmESVar CAnmESMFFloat::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFFloat *pFloat = (CAnmMFFloat *) m_object;

	FloatArray **ppA = pFloat->GetValue();
	assert(ppA);
	FloatArray *pA = *ppA;
	int sz = pA->size();

	Float f = 0.f;
	if (index < sz)
		f = (*pA)[index];

	return (AnmESVar) (double) f;
}

AnmESVar CAnmESMFFloat::getLength()
{
	CAnmMFFloat *pFloat = (CAnmMFFloat *) m_object;

	FloatArray **ppA = pFloat->GetValue();
	assert(ppA);
	FloatArray *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFFloat::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFFloat methods
// String toString( )
AnmESVar CAnmESMFFloat::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}

// MFImage
CAnmESObject *CAnmESMFImage::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	ImageArray *pA = new ImageArray;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		Image img = (Image) args[i];
		pA->push_back(img);
	}

	CAnmMFImage *pObj = new CAnmMFImage();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFImage(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFImage properties
void CAnmESMFImage::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFImage *pImg = (CAnmMFImage *) m_object;

	ImageArray **ppA = pImg->GetValue();
	assert(ppA);
	ImageArray *pA = *ppA;

	if (pA)
		pA->Ref();
	else
		pA = new ImageArray;
	
	int sz = pA->size();
	while (sz <= index)
	{
		Image img;
		pA->push_back(img);
		sz++;
	}

	Image img = (Image) propValue;

	(*pA)[index] = img;

	pImg->SetValue(pA);

	pA->UnRef();

}

AnmESVar CAnmESMFImage::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFImage *pImg = (CAnmMFImage *) m_object;

	ImageArray **ppA = pImg->GetValue();
	assert(ppA);
	ImageArray *pA = *ppA;
	int sz = pA->size();

	Image img;
	if (index < sz)
		img = (*pA)[index];

	CAnmField *pField = CAnmField::CreateField(eValueImage, &img);
	return AnmESVarFromField(pField, m_owner);
}

AnmESVar CAnmESMFImage::getLength()
{
	CAnmMFImage *pImg = (CAnmMFImage *) m_object;

	ImageArray **ppA = pImg->GetValue();
	assert(ppA);
	ImageArray *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFImage::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFImage methods
// String toString( )
AnmESVar CAnmESMFImage::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}

// MFInt32
CAnmESObject *CAnmESMFInt32::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	IntegerArray *pA = new IntegerArray;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		int ival = (int) args[i];
		pA->push_back(ival);
	}

	CAnmMFInt32 *pObj = new CAnmMFInt32();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFInt32(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFInt32 properties
void CAnmESMFInt32::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFInt32 *pInt = (CAnmMFInt32 *) m_object;

	IntegerArray **ppA = pInt->GetValue();
	assert(ppA);
	IntegerArray *pA = *ppA;

	if (pA)
		pA->Ref();
	else
		pA = new IntegerArray;
	
	int sz = pA->size();
	while (sz <= index)
	{
		pA->push_back(0L);
		sz++;
	}

	Integer i = (Integer) propValue;

	(*pA)[index] = i;

	pInt->SetValue(pA);

	pA->UnRef();
}

AnmESVar CAnmESMFInt32::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFInt32 *pInt = (CAnmMFInt32 *) m_object;

	IntegerArray **ppA = pInt->GetValue();
	assert(ppA);
	IntegerArray *pA = *ppA;
	int sz = pA->size();

	Integer i = 0L;
	if (index < sz)
		i = (*pA)[index];

	return (AnmESVar) (double) i;
}

AnmESVar CAnmESMFInt32::getLength()
{
	CAnmMFInt32 *pInt = (CAnmMFInt32 *) m_object;

	IntegerArray **ppA = pInt->GetValue();
	assert(ppA);
	IntegerArray *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFInt32::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFInt32 methods
// String toString( )
AnmESVar CAnmESMFInt32::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}

// MFNode
CAnmESObject *CAnmESMFNode::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	NodeArray *pA = new NodeArray;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		CAnmNode *pNode = (CAnmNode *) args[i];
		pA->push_back(pNode);
	}

	CAnmMFNode *pObj = new CAnmMFNode();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFNode(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFNode properties
void CAnmESMFNode::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFNode *pNodes = (CAnmMFNode *) m_object;

	NodeArray **ppA = pNodes->GetValue();
	assert(ppA);
	NodeArray *pA = *ppA;

	if (pA)
		pA->Ref();
	else
		pA = new NodeArray;
		
	int sz = pA->size();
	while (sz <= index)
	{
		pA->push_back(NULL);
		sz++;
	}

	CAnmNode *pNode = (CAnmNode *) propValue;

	(*pA)[index] = pNode;

	pNodes->SetValue(pA);

	pA->UnRef();
}

AnmESVar CAnmESMFNode::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFNode *pNodes = (CAnmMFNode *) m_object;

	NodeArray **ppA = pNodes->GetValue();
	assert(ppA);
	NodeArray *pA = *ppA;
	int sz = pA->size();

	CAnmNode *pNode = NULL;
	if (index < sz)
		pNode = (*pA)[index];

	CAnmField *pField = CAnmField::CreateField(eValueNode, &pNode);
	return AnmESVarFromField(pField, m_owner);
}


AnmESVar CAnmESMFNode::getLength()
{
	CAnmMFNode *pNodes = (CAnmMFNode *) m_object;

	NodeArray **ppA = pNodes->GetValue();
	assert(ppA);
	NodeArray *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFNode::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFNode methods
// String toString( )
AnmESVar CAnmESMFNode::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}

// MFRotation
CAnmESObject *CAnmESMFRotation::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	RotationArray *pA = new RotationArray;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		Rotation r = (Rotation) args[i];
		pA->push_back(r);
	}

	CAnmMFRotation *pObj = new CAnmMFRotation();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFRotation(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFRotation properties
void CAnmESMFRotation::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFRotation *pRot = (CAnmMFRotation *) m_object;

	RotationArray **ppA = pRot->GetValue();
	assert(ppA);
	RotationArray *pA = *ppA;

	if (pA)
		pA->Ref();
	else
		pA = new RotationArray;

	int sz = pA->size();
	while (sz <= index)
	{
		pA->push_back(Rotation(0, 1, 0, 0));
		sz++;
	}

	Rotation r = (Rotation) propValue;

	(*pA)[index] = r;

	pRot->SetValue(pA);

	pA->UnRef();
}

AnmESVar CAnmESMFRotation::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFRotation *pRot = (CAnmMFRotation *) m_object;

	RotationArray **ppA = pRot->GetValue();
	assert(ppA);
	RotationArray *pA = *ppA;
	int sz = pA->size();

	Rotation r(0, 1, 0, 0);
	if (index < sz)
		r = (*pA)[index];

	CAnmField *pField = CAnmField::CreateField(eValueRotation, &r);
	return AnmESVarFromField(pField, m_owner);
}

AnmESVar CAnmESMFRotation::getLength()
{
	CAnmMFRotation *pRot = (CAnmMFRotation *) m_object;

	RotationArray **ppA = pRot->GetValue();
	assert(ppA);
	RotationArray *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFRotation::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFRotation methods
// String toString( )
AnmESVar CAnmESMFRotation::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}

// MFString
CAnmESObject *CAnmESMFString::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	StringArray *pA = new StringArray;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		CAnmESString *pStr = args[i];
		if (pStr == NULL)
			pStr = args[i].toString(pOwner);

		String s = pStr->GetString();
		pA->push_back(s);
	}

	CAnmMFString *pObj = new CAnmMFString();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFString(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFString properties
void CAnmESMFString::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFString *pStr = (CAnmMFString *) m_object;

	StringArray **ppA = pStr->GetValue();
	assert(ppA);
	StringArray *pA = *ppA;
 
	if (pA)
		pA->Ref();
	else
		pA = new StringArray;

	int sz = pA->size();
	while (sz <= index)
	{
		pA->push_back(new CAnmString(""));
		sz++;
	}

	(*pA)[index]->UnRef();

	CAnmESString *pESStr = (CAnmESString *) propValue;
	if (pESStr == NULL)
		pESStr = propValue.toString(m_owner);

	(*pA)[index] = pESStr->GetString();

	(*pA)[index]->Ref();

	pStr->SetValue(pA);

	pA->UnRef();
}

AnmESVar CAnmESMFString::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFString *pStr = (CAnmMFString *) m_object;

	StringArray **ppA = pStr->GetValue();
	assert(ppA);
	StringArray *pA = *ppA;
	int sz = pA->size();

	String s;
	if (index < sz)
		s = (*pA)[index]->Clone();
	else
		s = new CAnmString("");

	CAnmESString *pESStr = new CAnmESString(s, m_owner);
	AnmESVar v = pESStr;
	return v;
}

AnmESVar CAnmESMFString::getLength()
{
	CAnmMFString *pStr = (CAnmMFString *) m_object;

	StringArray **ppA = pStr->GetValue();
	assert(ppA);
	StringArray *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFString::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFString methods
// String toString( )
AnmESVar CAnmESMFString::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}

// MFTime
CAnmESObject *CAnmESMFTime::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	TimeArray *pA = new TimeArray;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		Time t = (double) args[i];
		pA->push_back(t);
	}

	CAnmMFTime *pObj = new CAnmMFTime();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFTime(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFTime properties
void CAnmESMFTime::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFTime *pTime = (CAnmMFTime *) m_object;

	TimeArray **ppA = pTime->GetValue();
	assert(ppA);
	TimeArray *pA = *ppA;

	if (pA)
		pA->Ref();
	else
		pA = new TimeArray;

	int sz = pA->size();
	while (sz <= index)
	{
		pA->push_back(0.f);
		sz++;
	}

	Time t = (Time) propValue;

	(*pA)[index] = t;

	pTime->SetValue(pA);

	pA->UnRef();
}

AnmESVar CAnmESMFTime::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFTime *pTime = (CAnmMFTime *) m_object;

	TimeArray **ppA = pTime->GetValue();
	assert(ppA);
	TimeArray *pA = *ppA;
	int sz = pA->size();

	Time t = 0.f;
	if (index < sz)
		t = (*pA)[index];

	return (AnmESVar) t;
}

AnmESVar CAnmESMFTime::getLength()
{
	CAnmMFTime *pTime = (CAnmMFTime *) m_object;

	TimeArray **ppA = pTime->GetValue();
	assert(ppA);
	TimeArray *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFTime::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFTime methods
// String toString( )
AnmESVar CAnmESMFTime::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}

// MFVec2f
CAnmESObject *CAnmESMFVec2f::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	Point2Array *pA = new Point2Array;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		Point2 p2 = (Point2) args[i];
		pA->push_back(p2);
	}

	CAnmMFVec2f *pObj = new CAnmMFVec2f();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFVec2f(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFVec2f properties
void CAnmESMFVec2f::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFVec2f *pVec2f = (CAnmMFVec2f *) m_object;

	Point2Array **ppA = pVec2f->GetValue();
	assert(ppA);
	Point2Array *pA = *ppA;

	if (pA)
		pA->Ref();
	else
		pA = new Point2Array;

	int sz = pA->size();
	while (sz <= index)
	{
		pA->push_back(Point2(0, 0));
		sz++;
	}

	Point2 p2 = (Point2) propValue;

	(*pA)[index] = p2;

	pVec2f->SetValue(pA);

	pA->UnRef();
}

AnmESVar CAnmESMFVec2f::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFVec2f *pVec2f = (CAnmMFVec2f *) m_object;

	Point2Array **ppA = pVec2f->GetValue();
	assert(ppA);
	Point2Array *pA = *ppA;
	int sz = pA->size();

	Point2 p2(0,0);
	if (index < sz)
		p2 = (*pA)[index];

	CAnmField *pField = CAnmField::CreateField(eValuePoint2, &p2);
	return AnmESVarFromField(pField, m_owner);
}

AnmESVar CAnmESMFVec2f::getLength()
{
	CAnmMFVec2f *pVec2f = (CAnmMFVec2f *) m_object;

	Point2Array **ppA = pVec2f->GetValue();
	assert(ppA);
	Point2Array *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFVec2f::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFVec2f methods
// String toString( )
AnmESVar CAnmESMFVec2f::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}

// MFVec3f
CAnmESObject *CAnmESMFVec3f::Create(CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args)
{
	Point3Array *pA = new Point3Array;
	
	if (nargs > 0)
		pA->reserve(nargs);

	for (int i = 0; i < nargs; i++)
	{
		Point3 p3 = (Point3) args[i];
		pA->push_back(p3);
	}

	CAnmMFVec3f *pObj = new CAnmMFVec3f();
	pObj->SetValue(pA);
	
	// setvalue adds a ref
	pA->UnRef();

	CAnmESObject *pESObject = new CAnmESMFVec3f(pObj, pOwner);

	pObj->UnRef();

	return pESObject;
}

// MFVec3f properties
void CAnmESMFVec3f::PutArrayProperty(AnmESVar vindex, AnmESVar propValue)
{
	int index = vindex;

	CAnmMFVec3f *pVec3f = (CAnmMFVec3f *) m_object;

	Point3Array **ppA = pVec3f->GetValue();
	assert(ppA);
	Point3Array *pA = *ppA;

	if (pA)
		pA->Ref();
	else
		pA = new Point3Array;

	int sz = pA->size();
	while (sz <= index)
	{
		pA->push_back(Point3(0, 0, 0));
		sz++;
	}

	Point3 p3 = (Point3) propValue;

	(*pA)[index] = p3;
	pVec3f->SetValue(pA);

	pA->UnRef();
}

AnmESVar CAnmESMFVec3f::GetArrayProperty(AnmESVar vindex)
{
	int index = vindex;
	CAnmMFVec3f *pVec3f = (CAnmMFVec3f *) m_object;

	Point3Array **ppA = pVec3f->GetValue();
	assert(ppA);
	Point3Array *pA = *ppA;
	int sz = pA->size();

	Point3 p3(0,0,0);
	if (index < sz)
		p3 = (*pA)[index];

	CAnmField *pField = CAnmField::CreateField(eValuePoint3, &p3);
	return AnmESVarFromField(pField, m_owner);
}

AnmESVar CAnmESMFVec3f::getLength()
{
	CAnmMFVec3f *pVec3f = (CAnmMFVec3f *) m_object;

	Point3Array **ppA = pVec3f->GetValue();
	assert(ppA);
	Point3Array *pA = *ppA;
	int sz = pA->size();

	return (AnmESVar) (double) sz;
}

void CAnmESMFVec3f::setLength(AnmESVar v)
{
	// N.B.: do this later
}

// MFVec3f methods
// String toString( )
AnmESVar CAnmESMFVec3f::toString(int nArgs, AnmESVar *args)
{
	// N.B.: do this later
	return (AnmESVar) new CAnmESString(new CAnmString(""), m_owner);
}



// Tables for Callbacks, Methods and Put/GetProperties
// Field objects

// SFColor object
DEFINE_ESPROPERTY(CAnmESSFColor, r, getR, setR);
DEFINE_ESPROPERTY(CAnmESSFColor, g, getG, setG);
DEFINE_ESPROPERTY(CAnmESSFColor, b, getB, setB);

DEFINE_ESMETHOD(CAnmESSFColor, setHSV);
DEFINE_ESMETHOD(CAnmESSFColor, getHSV);
DEFINE_ESMETHOD(CAnmESSFColor, toString);


// member table
BEGIN_DEFINE_MEMBERS(CAnmESSFColor)
ESPROPID(CAnmESSFColor, r),
ESPROPID(CAnmESSFColor, g),
ESPROPID(CAnmESSFColor, b),
ESMETHODID(CAnmESSFColor, setHSV),
ESMETHODID(CAnmESSFColor, getHSV),
ESMETHODID(CAnmESSFColor, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESSFColor, CAnmESBuiltinObject);

// SFNode object
DEFINE_ESMETHOD(CAnmESSFNode, toString);


// member table
BEGIN_DEFINE_MEMBERS(CAnmESSFNode)
ESMETHODID(CAnmESSFNode, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESSFNode, CAnmESBuiltinObject);

// SFRotation object
DEFINE_ESPROPERTY(CAnmESSFRotation, x, getX, setX);
DEFINE_ESPROPERTY(CAnmESSFRotation, y, getY, setY);
DEFINE_ESPROPERTY(CAnmESSFRotation, z, getZ, setZ);
DEFINE_ESPROPERTY(CAnmESSFRotation, angle, getAngle, setAngle);

DEFINE_ESMETHOD(CAnmESSFRotation, getAxis);
DEFINE_ESMETHOD(CAnmESSFRotation, inverse);
DEFINE_ESMETHOD(CAnmESSFRotation, multiply);
DEFINE_ESMETHOD(CAnmESSFRotation, multVec);
DEFINE_ESMETHOD(CAnmESSFRotation, setAxis);
DEFINE_ESMETHOD(CAnmESSFRotation, slerp);
DEFINE_ESMETHOD(CAnmESSFRotation, toString);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESSFRotation)
ESPROPID(CAnmESSFRotation, x),
ESPROPID(CAnmESSFRotation, y),
ESPROPID(CAnmESSFRotation, z),
ESPROPID(CAnmESSFRotation, angle),
ESMETHODID(CAnmESSFRotation, getAxis),
ESMETHODID(CAnmESSFRotation, inverse),
ESMETHODID(CAnmESSFRotation, multiply),
ESMETHODID(CAnmESSFRotation, multVec),
ESMETHODID(CAnmESSFRotation, setAxis),
ESMETHODID(CAnmESSFRotation, slerp),
ESMETHODID(CAnmESSFRotation, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESSFRotation, CAnmESBuiltinObject);

// SFVec2f object
DEFINE_ESPROPERTY(CAnmESSFVec2f, x, getX, setX);
DEFINE_ESPROPERTY(CAnmESSFVec2f, y, getY, setY);

DEFINE_ESMETHOD(CAnmESSFVec2f, add);
DEFINE_ESMETHOD(CAnmESSFVec2f, divide);
DEFINE_ESMETHOD(CAnmESSFVec2f, dot);
DEFINE_ESMETHOD(CAnmESSFVec2f, length);
DEFINE_ESMETHOD(CAnmESSFVec2f, multiply);
DEFINE_ESMETHOD(CAnmESSFVec2f, normalize);
DEFINE_ESMETHOD(CAnmESSFVec2f, subtract);
DEFINE_ESMETHOD(CAnmESSFVec2f, toString);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESSFVec2f)
ESPROPID(CAnmESSFVec2f, x),
ESPROPID(CAnmESSFVec2f, y),
ESMETHODID(CAnmESSFVec2f, add),
ESMETHODID(CAnmESSFVec2f, divide),
ESMETHODID(CAnmESSFVec2f, dot),
ESMETHODID(CAnmESSFVec2f, length),
ESMETHODID(CAnmESSFVec2f, multiply),
ESMETHODID(CAnmESSFVec2f, normalize),
ESMETHODID(CAnmESSFVec2f, subtract),
ESMETHODID(CAnmESSFVec2f, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESSFVec2f, CAnmESBuiltinObject);

// SFVec3f object
DEFINE_ESPROPERTY(CAnmESSFVec3f, x, getX, setX);
DEFINE_ESPROPERTY(CAnmESSFVec3f, y, getY, setY);
DEFINE_ESPROPERTY(CAnmESSFVec3f, z, getZ, setZ);

DEFINE_ESMETHOD(CAnmESSFVec3f, add);
DEFINE_ESMETHOD(CAnmESSFVec3f, cross);
DEFINE_ESMETHOD(CAnmESSFVec3f, divide);
DEFINE_ESMETHOD(CAnmESSFVec3f, dot);
DEFINE_ESMETHOD(CAnmESSFVec3f, length);
DEFINE_ESMETHOD(CAnmESSFVec3f, multiply);
DEFINE_ESMETHOD(CAnmESSFVec3f, negate);
DEFINE_ESMETHOD(CAnmESSFVec3f, normalize);
DEFINE_ESMETHOD(CAnmESSFVec3f, subtract);
DEFINE_ESMETHOD(CAnmESSFVec3f, toString);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESSFVec3f)
ESPROPID(CAnmESSFVec3f, x),
ESPROPID(CAnmESSFVec3f, y),
ESPROPID(CAnmESSFVec3f, z),
ESMETHODID(CAnmESSFVec3f, add),
ESMETHODID(CAnmESSFVec3f, cross),
ESMETHODID(CAnmESSFVec3f, divide),
ESMETHODID(CAnmESSFVec3f, dot),
ESMETHODID(CAnmESSFVec3f, length),
ESMETHODID(CAnmESSFVec3f, multiply),
ESMETHODID(CAnmESSFVec3f, negate),
ESMETHODID(CAnmESSFVec3f, normalize),
ESMETHODID(CAnmESSFVec3f, subtract),
ESMETHODID(CAnmESSFVec3f, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESSFVec3f, CAnmESBuiltinObject);

// MField object
DEFINE_ESPROPERTY(CAnmESMFieldObject, length, getLength, setLength);
DEFINE_ESMETHOD(CAnmESMFieldObject, toString);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFieldObject)
ESPROPID(CAnmESMFieldObject, length),
ESMETHODID(CAnmESMFieldObject, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFieldObject, CAnmESBuiltinObject);

// MFBool object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFBool, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFBool, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFBool)
ESPROPID(CAnmESMFBool, length),
ESMETHODID(CAnmESMFBool, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFBool, CAnmESMFieldObject);


// MFColor object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFColor, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFColor, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFColor)
ESPROPID(CAnmESMFColor, length),
ESMETHODID(CAnmESMFColor, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFColor, CAnmESMFieldObject);

// MFDouble object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFDouble, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFDouble, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFDouble)
ESPROPID(CAnmESMFDouble, length),
ESMETHODID(CAnmESMFDouble, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFDouble, CAnmESMFieldObject);


// MFFloat object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFFloat, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFFloat, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFFloat)
ESPROPID(CAnmESMFFloat, length),
ESMETHODID(CAnmESMFFloat, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFFloat, CAnmESMFieldObject);

// MFImage object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFImage, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFImage, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFImage)
ESPROPID(CAnmESMFImage, length),
ESMETHODID(CAnmESMFImage, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFImage, CAnmESMFieldObject);

// MFInt32 object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFInt32, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFInt32, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFInt32)
ESPROPID(CAnmESMFInt32, length),
ESMETHODID(CAnmESMFInt32, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFInt32, CAnmESMFieldObject);

// MFNode object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFNode, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFNode, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFNode)
ESPROPID(CAnmESMFNode, length),
ESMETHODID(CAnmESMFNode, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFNode, CAnmESMFieldObject);

// MFRotation object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFRotation, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFRotation, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFRotation)
ESPROPID(CAnmESMFRotation, length),
ESMETHODID(CAnmESMFRotation, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFRotation, CAnmESMFieldObject);

// MFString object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFString, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFString, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFString)
ESPROPID(CAnmESMFString, length),
ESMETHODID(CAnmESMFString, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFString, CAnmESMFieldObject);


// MFTime object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFTime, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFTime, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFTime)
ESPROPID(CAnmESMFTime, length),
ESMETHODID(CAnmESMFTime, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFTime, CAnmESMFieldObject);


// MFVec2f object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFVec2f, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFVec2f, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFVec2f)
ESPROPID(CAnmESMFVec2f, length),
ESMETHODID(CAnmESMFVec2f, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFVec2f, CAnmESMFieldObject);


// MFVec3f object
DEFINE_ESINHERITED_PROPERTY(CAnmESMFVec3f, length, CAnmESMFieldObject);
DEFINE_ESINHERITED_METHOD(CAnmESMFVec3f, toString, CAnmESMFieldObject);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESMFVec3f)
ESPROPID(CAnmESMFVec3f, length),
ESMETHODID(CAnmESMFVec3f, toString),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMFVec3f, CAnmESMFieldObject);
