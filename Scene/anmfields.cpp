/********************************************************************************
 * Flux
 *
 * File: anmfields.cpp
 * Description: Field classes -
 *							polymorphic data for object fields
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
#include "anmfields.h"
#include "anmnode.h"

CAnmField *CAnmField::CreateField(eValueType valueType, void *pData)
{
	CAnmField *pF = NULL;

	switch (valueType)
	{
		case eValueBoolean :
			pF = new CAnmSFBool();
			break;

		case eValueBooleanArray :
			pF = new CAnmMFBool();
			break;

		case eValueColor :
			pF = new CAnmSFColor();
			break;

		case eValueColorArray :
			pF = new CAnmMFColor();
			break;

		case eValueDouble :
			pF = new CAnmSFDouble();
			break;

		case eValueDoubleArray :
			pF = new CAnmMFDouble();
			break;

		case eValueFloat :
			pF = new CAnmSFFloat();
			break;

		case eValueFloatArray :
			pF = new CAnmMFFloat();
			break;

		case eValueImage :
			pF = new CAnmSFImage();
			break;

		case eValueImageArray :
			pF = new CAnmMFImage();
			break;

		case eValueInteger :
			pF = new CAnmSFInt32();
			break;

		case eValueIntegerArray :
			pF = new CAnmMFInt32();
			break;

		case eValueNode :
			pF = new CAnmSFNode();
			break;

		case eValueNodeArray :
			pF = new CAnmMFNode();
			break;

		case eValueRotation :
			pF = new CAnmSFRotation();
			break;

		case eValueRotationArray :
			pF = new CAnmMFRotation();
			break;

		case eValueString :
			pF = new CAnmSFString();
			break;

		case eValueStringArray :
			pF = new CAnmMFString();
			break;

		case eValueTime :
			pF = new CAnmSFTime();
			break;

		case eValueTimeArray :
			pF = new CAnmMFTime();
			break;

		case eValuePoint2 :
			pF = new CAnmSFVec2f();
			break;

		case eValuePoint2Array :
			pF = new CAnmMFVec2f();
			break;

		case eValuePoint3 :
			pF = new CAnmSFVec3f();
			break;

		case eValuePoint3Array :
			pF = new CAnmMFVec3f();
			break;

		default :
			assert(FALSE);
			break;
	}

	if (pData)
		pF->SetData(pData);

	return pF;
}

CAnmField *CAnmField::CreateField(CAnmNode *pNode, CLASSMEMBERID mid)
{
	assert(pNode);

	Boolean b;
	Color c;
	Float f;
	// Image img;
	Integer i;
	Rotation r;
	String s, defaultstr;
	Time t;
	Point2 p2;
	Point3 p3;
	void *ptr = NULL;

	void *val = NULL;

	// N.B.: eventually do eventOut last value thing
	if (mid->memberType == eAnmValueMember || mid->memberType == eAnmInitMember)
	{
		switch (mid->valueType)
		{
			case eValueBoolean :
				pNode->GetValue(mid, (void **) &b);
				val = &b;
				break;

			case eValueColor :
				pNode->GetValue(mid, (void **) &c);
				val = &c;
				break;

			case eValueFloat :
				pNode->GetValue(mid, (void **) &f);
				val = &f;
				break;

			case eValueImage :
				assert(FALSE);
				//pNode->GetValue(mid, (void **) &img);
				//val = &img;
				break;

			case eValueInteger :
				pNode->GetValue(mid, (void **) &i);
				val = &i;
				break;

			case eValueRotation :
				pNode->GetValue(mid, (void **) &r);
				val = &r;
				break;

			case eValueString :
				defaultstr = new CAnmString("");
				s = defaultstr;
				pNode->GetValue(mid, (void **) &s);
				val = &s;
				if (s != defaultstr)
					defaultstr->UnRef();
				break;

			case eValueTime :
				pNode->GetValue(mid, (void **) &t);
				val = &t;
				break;

			case eValuePoint2 :
				pNode->GetValue(mid, (void **) &p2);
				val = &p2;
				break;

			case eValuePoint3 :
				pNode->GetValue(mid, (void **) &p3);
				val = &p3;
				break;

			case eValueBooleanArray :
			case eValueColorArray :
			case eValueFloatArray :
			case eValueIntegerArray :
			case eValueNode :
			case eValueNodeArray :
			case eValueRotationArray :
			case eValueStringArray :
			case eValueTimeArray :
			case eValuePoint2Array :
			case eValuePoint3Array :
				pNode->GetValue(mid, &ptr);
				val = &ptr;
				break;

			default :
				assert(FALSE);
				break;
		}
	}

	CAnmField *pF = CreateField(mid->valueType, val);
	pF->SetNode(pNode);
	pF->SetMemberID(mid);
	return pF;
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_CALLBACK(CAnmField, value_changed, eValueBoolean);

BEGIN_DEFINE_MEMBERS(CAnmField)
CALLBACKID(CAnmField, value_changed),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmField, CAnmObject)

// N.B.: empty classes for now but we'll add methods once we have general method descriptors
DEFINE_ABSTRACT_EMPTY_CLASS(CAnmMField, CAnmField)

DEFINE_EMPTY_CLASS(CAnmSFBool, CAnmField)
DEFINE_EMPTY_CLASS(CAnmSFColor, CAnmField)
DEFINE_EMPTY_CLASS(CAnmSFDouble, CAnmField)
DEFINE_EMPTY_CLASS(CAnmSFFloat, CAnmField)
DEFINE_EMPTY_CLASS(CAnmSFImage, CAnmField)
DEFINE_EMPTY_CLASS(CAnmSFInt32, CAnmField)
DEFINE_EMPTY_CLASS(CAnmSFNode, CAnmField)
DEFINE_EMPTY_CLASS(CAnmSFRotation, CAnmField)
DEFINE_EMPTY_CLASS(CAnmSFString, CAnmField)
DEFINE_EMPTY_CLASS(CAnmSFTime, CAnmField)
DEFINE_EMPTY_CLASS(CAnmSFVec2f, CAnmField)
DEFINE_EMPTY_CLASS(CAnmSFVec3f, CAnmField)

DEFINE_EMPTY_CLASS(CAnmMFBool, CAnmMField)
DEFINE_EMPTY_CLASS(CAnmMFColor, CAnmMField)
DEFINE_EMPTY_CLASS(CAnmMFDouble, CAnmMField)
DEFINE_EMPTY_CLASS(CAnmMFFloat, CAnmMField)
DEFINE_EMPTY_CLASS(CAnmMFImage, CAnmMField)
DEFINE_EMPTY_CLASS(CAnmMFInt32, CAnmMField)
DEFINE_EMPTY_CLASS(CAnmMFNode, CAnmMField)
DEFINE_EMPTY_CLASS(CAnmMFRotation, CAnmMField)
DEFINE_EMPTY_CLASS(CAnmMFString, CAnmMField)
DEFINE_EMPTY_CLASS(CAnmMFTime, CAnmMField)
DEFINE_EMPTY_CLASS(CAnmMFVec2f, CAnmMField)
DEFINE_EMPTY_CLASS(CAnmMFVec3f, CAnmMField)
