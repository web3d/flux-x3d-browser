/********************************************************************************
 * Flux
 *
 * File: anmesvar.cpp
 * Description: ECMAScript variable data type
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
#include "anmecmascript.h"
#include "anmesnode.h"
#include "anmesfields.h"
#include "anmesmessage.h"
#include "anmesstring.h"
#include "anmfields.h"
#include "anmnode.h"
#include "anmsaimatrix.h"
#include "anmsainode.h"
#include "anmeslanguagebinding.h"
#include "anmscript.h"

// ECMAScript object conversions
AnmESVar::operator class CAnmESObject * ()
{
	if (valueType == eObject || valueType == eString)
		return ptrVal;
	else
		return NULL;
}

AnmESVar::operator class CAnmESString * ()
{
	if (valueType == eString)
		return strVal;
	else
		return NULL;
}

AnmESVar::operator class CAnmESFieldObject * ()
{
	if (valueType == eObject)
	{
		CAnmESObject *pObj = (CAnmESObject *) ptrVal;

		if (pObj == NULL)
			return NULL;

		if (pObj->GetObjectType() != eBuiltinObject)
			return NULL;

		CAnmESBuiltinObject *pBuiltinObj = (CAnmESBuiltinObject *) pObj;
		if (pBuiltinObj->GetBuiltinType() != eFieldObject)
			return NULL;

		CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) pBuiltinObj;
		return pFieldObj;
	}
	else
		return NULL;
}
// Convert a field to AnmESVar; use owning script to initialize context of any new's
AnmESVar AnmESVarFromField(CAnmField *pField, CAnmECMAScriptLanguageBinding *pOwner)
{
	AnmESVar v;
	eValueType valueType = pField->GetType();
	void *pValue = pField->GetData();

	if (valueType == eValueNode)
	{
		if (pValue && (* (CAnmNode **) pValue))
		{
// krv: reverted this back to original code to fix a Cisro bug that this change caused.
// Mantis 261
#if 1
			CAnmSAINode *pSAINode = new CAnmSAINode(* (CAnmNode **) pValue);
			pSAINode->SetSink(pOwner->GetScriptNode());

			CAnmESNodeObject *pNodeObj = new CAnmESNodeObject(pSAINode, pOwner);

			// drop one ref otherwise this thing will live forever
			pSAINode->UnRef();
#else
			CAnmSAINode *pSAINode = pOwner->LookupNode(* (CAnmNode **) pValue);
			assert(pSAINode);

			pSAINode->SetSink(pOwner->GetScriptNode());

			CAnmESNodeObject *pNodeObj = new CAnmESNodeObject(pSAINode, pOwner);

			// drop one ref otherwise this thing will live forever
//			pSAINode->UnRef();
#endif
			v = pNodeObj;
		}
		else
			v = (CAnmESNodeObject *) NULL;
	}
	else if (valueType == eValueBoolean)
	{
		v = AnmESBool(* (Boolean *)pValue);
	}
	else if (valueType == eValueDouble)
	{
		v = (double) * (Double *) pValue;
	}
	else if (valueType == eValueInteger)
	{
		v = (double) * (Integer *)pValue;
	}
	else if (valueType == eValueFloat)
	{
		v = (double) * (Float *) pValue;
	}
	else if (valueType == eValueString)
	{
		String s = * ((String *) pValue);
		String s2 = s->Clone();

		v = new CAnmESString(s2, pOwner);

		// Drop one or this never goes away
		s2->UnRef();
	}
	else if (valueType == eValueTime)
	{
		v = (double) * (Time *) pValue;
	}
	else if (valueType == eValueBadType)
	{
		v = (CAnmESObject *) NULL;
	}
	else 
	{
		CAnmESFieldObject *pFieldObj = NULL;
		switch(valueType)
		{
			case eValueColor :
				pFieldObj = new CAnmESSFColor(pField, pOwner);
				break;
			case eValueRotation :
				pFieldObj = new CAnmESSFRotation(pField, pOwner);
				break;
			case eValuePoint2 :
				pFieldObj = new CAnmESSFVec2f(pField, pOwner);
				break;
			case eValuePoint3 :
				pFieldObj = new CAnmESSFVec3f(pField, pOwner);
				break;
			case eValueBooleanArray :
				pFieldObj = new CAnmESMFBool(pField, pOwner);
				break;
			case eValueDoubleArray :
				pFieldObj = new CAnmESMFDouble(pField, pOwner);
				break;
			case eValueColorArray :
				pFieldObj = new CAnmESMFColor(pField, pOwner);
				break;
			case eValueFloatArray :
				pFieldObj = new CAnmESMFFloat(pField, pOwner);
				break;
			case eValueImageArray :
				pFieldObj = new CAnmESMFImage(pField, pOwner);
				break;
			case eValueIntegerArray :
				pFieldObj = new CAnmESMFInt32(pField, pOwner);
				break;
			case eValueNodeArray :
				pFieldObj = new CAnmESMFNode(pField, pOwner);
				break;
			case eValueRotationArray :
				pFieldObj = new CAnmESMFRotation(pField, pOwner);
				break;
			case eValueStringArray :
				pFieldObj = new CAnmESMFString(pField, pOwner);
				break;
			case eValueTimeArray :
				pFieldObj = new CAnmESMFTime(pField, pOwner);
				break;
			case eValuePoint2Array :
				pFieldObj = new CAnmESMFVec2f(pField, pOwner);
				break;
			case eValuePoint3Array :
				pFieldObj = new CAnmESMFVec3f(pField, pOwner);
				break;

			default :
				assert(FALSE);
				break;
		}

		assert(pFieldObj);
		v = pFieldObj;
	}

	return v;
}

CAnmField *AnmESVarToField(AnmESVar v)
{
	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) v;
	if (pFieldObj)
		return pFieldObj->GetField();
	else
		return NULL;
}


AnmESVar::operator class CAnmESNodeObject * ()
{
	if (valueType == eObject)
	{
		CAnmESObject *pObj = (CAnmESObject *) ptrVal;
		if (pObj == NULL)
			return NULL;

		if (pObj->GetObjectType() != eBuiltinObject)
			return NULL;

		CAnmESBuiltinObject *pBuiltinObj = (CAnmESBuiltinObject *) pObj;
		if (pBuiltinObj->GetBuiltinType() != eNodeObject)
			return NULL;

		CAnmESNodeObject *pNodeObj = (CAnmESNodeObject *) pBuiltinObj;
		return pNodeObj;
	}
	else
		return NULL;
}

// Node conversion

AnmESVar::operator CAnmNode * ()
{
	CAnmNode *pNode = NULL;
	CAnmSAINode *pSAINode = NULL;
	CAnmESNodeObject *pNodeObj = (CAnmESNodeObject *) *this;
	if (pNodeObj)
	{
		pSAINode = pNodeObj->GetNode();
		if (pSAINode)
			pNode = pSAINode->GetNode();
	}
	
	return pNode;
}

// Field type conversions

AnmESVar::operator Color ()
{
	Color c(0, 0, 0);

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueColor)
		{
			c = *(Color *) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOSFCOLOR : "Can't convert variable to SFColor value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOSFCOLOR);
	}

	return c;
}

AnmESVar::operator Image ()
{
	Image img;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueImage)
		{
			img = *(Image *) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOSFIMAGE : "Can't convert variable to SFImage value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOSFIMAGE);
	}

	return img;
}


AnmESVar::operator Rotation ()
{
	Rotation r(0, 0, 1, 0);

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueRotation)
		{
			r = *(Rotation *) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOSFROTATION : "Can't convert variable to SFRotation value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOSFROTATION);
	}

	return r;
}

AnmESVar::operator Point2 ()
{
	Point2 p2(0, 0);

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValuePoint2)
		{
			p2 = *(Point2 *) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOSFVEC2F : "Can't convert variable to SFVec2f value"
			throw CAnmError("Can't convert variable to SFVec2f value");
	}

	return p2;
}

AnmESVar::operator Point3 ()
{
	Point3 p3(0, 0, 0);

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValuePoint3)
		{
			p3 = *(Point3 *) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOSFVEC3F : "Can't convert variable to SFVec3f value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOSFVEC3F);
	}

	return p3;
}

AnmESVar::operator CAnmSFRotation * ()
{
	CAnmSFRotation *pRot = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueRotation)
		{
			pRot = (CAnmSFRotation *) pFieldObj->GetObject();
		}
		else
			// ANMMSG_ESCANTCONVERTTOSFROTATION : "Can't convert variable to SFRotation value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOSFROTATION);
	}

	return pRot;
}

AnmESVar::operator CAnmSFVec3f * ()
{
	CAnmSFVec3f *pVec3f = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValuePoint3)
		{
			pVec3f = (CAnmSFVec3f *) pFieldObj->GetObject();
		}
		else
			// ANMMSG_ESCANTCONVERTTOSFVEC3F : "Can't convert variable to SFVec3f value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOSFVEC3F);
	}

	return pVec3f;
}


AnmESVar::operator CAnmSAIMatrix4 * ()
{
	CAnmSAIMatrix4 *pMatrix = NULL;

	CAnmESObject *pESObj = (CAnmESObject *) *this;
	if (pESObj)
	{
		if (pESObj->GetObjectType() == eBuiltinObject)
		{
			CAnmESBuiltinObject *pBuiltinObj = (CAnmESBuiltinObject *) pESObj;

			CAnmObject *pObj = pBuiltinObj->GetObject();

			if (pObj && pObj->GetClass()->IsA(GETCLASS(CAnmSAIMatrix4)))
				pMatrix = (CAnmSAIMatrix4 *) pObj;
		}
	}

	if (pMatrix == NULL)
		// ANMMSG_ESCANTCONVERTTOMATRIX4 : "Can't convert variable to Matrix4 value"
		throw CAnmError(ANMMSG_ESCANTCONVERTTOMATRIX4);

	return pMatrix;
}

AnmESVar::operator BooleanArray * ()
{
	BooleanArray *pBoolArray = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueBooleanArray)
		{
			pBoolArray = *(BooleanArray **) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOMFBOOL : "Can't convert variable to MFBool value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOMFBOOL);
	}

	return pBoolArray;
}

AnmESVar::operator ColorArray * ()
{
	ColorArray *pClrArray = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueColorArray)
		{
			pClrArray = *(ColorArray **) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOMFCOLOR : "Can't convert variable to MFColor value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOMFCOLOR);
	}

	return pClrArray;
}

AnmESVar::operator DoubleArray * ()
{
	DoubleArray *pDoubleArray = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueDoubleArray ||
			pFieldObj->GetValueType() == eValueTimeArray)
		{
			pDoubleArray = *(DoubleArray **) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOMFDOUBLE : "Can't convert variable to MFDouble value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOMFDOUBLE);
	}

	return pDoubleArray;
}

AnmESVar::operator FloatArray * ()
{
	FloatArray *pFloatArray = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueFloatArray)
		{
			pFloatArray = *(FloatArray **) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOMFFLOAT : "Can't convert variable to MFFloat value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOMFFLOAT);
	}

	return pFloatArray;
}

AnmESVar::operator ImageArray * ()
{
	ImageArray *pImgArray = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueImageArray)
		{
			pImgArray = *(ImageArray **) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOMFIMAGE : "Can't convert variable to MFImage value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOMFIMAGE);
	}

	return pImgArray;
}


AnmESVar::operator IntegerArray * ()
{
	IntegerArray *pIntArray = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueIntegerArray)
		{
			pIntArray = *(IntegerArray **) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOMFINT32 : "Can't convert variable to MFInt32 value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOMFINT32);
	}

	return pIntArray;
}

AnmESVar::operator NodeArray * ()
{
	NodeArray *pNodeArray = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueNodeArray)
		{
			pNodeArray = *(NodeArray **) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOMFNODE : "Can't convert variable to MFNode value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOMFNODE);
	}

	return pNodeArray;
}

AnmESVar::operator RotationArray * ()
{
	RotationArray *pRotArray = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueRotationArray)
		{
			pRotArray = *(RotationArray **) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOMFROTATION : "Can't convert variable to MFRotation value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOMFROTATION);
	}

	return pRotArray;
}

AnmESVar::operator StringArray * ()
{
	StringArray *pStrArray = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValueStringArray)
		{
			pStrArray = *(StringArray **) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOMFSTRING : "Can't convert variable to MFString value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOMFSTRING);
	}

	return pStrArray;
}


AnmESVar::operator Point2Array * ()
{
	Point2Array *p2Array = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValuePoint2Array)
		{
			p2Array = *(Point2Array **) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOMFVEC2F : "Can't convert variable to MFVec2f value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOMFVEC2F);
	}

	return p2Array;
}

AnmESVar::operator Point3Array * ()
{
	Point3Array *p3Array = NULL;

	CAnmESFieldObject *pFieldObj = (CAnmESFieldObject *) *this;
	if (pFieldObj)
	{
		if (pFieldObj->GetValueType() == eValuePoint3Array)
		{
			p3Array = *(Point3Array **) pFieldObj->GetValue();
		}
		else
			// ANMMSG_ESCANTCONVERTTOMFVEC3F : "Can't convert variable to MFVec3f value"
			throw CAnmError(ANMMSG_ESCANTCONVERTTOMFVEC3F);
	}

	return p3Array;
}

// methods
// string concatenation
AnmESVar AnmESVar::concat(AnmESVar &v)
{
	CAnmESString *esstr1 = NULL, *esstr2 = NULL;
	CAnmECMAScriptLanguageBinding *pOwner = NULL;
	String str1, str2;

	if (valueType == eString)
	{
		esstr1 = strVal;
		pOwner = strVal->GetOwner();
	}
	else
	{
		if (v.valueType != eString)
			// ANMMSG_ESNOOWNERFORSTRINGALLOC : "Internal error: no owning script context for string allocation"
			throw CAnmError(ANMMSG_ESNOOWNERFORSTRINGALLOC);

		pOwner = v.strVal->GetOwner();
		esstr1 = toString(pOwner);
	}

	str1 = esstr1->GetString();

	if (v.valueType == eString)
		esstr2 = v.strVal;
	else
	{
		if (valueType != eString)
			// ANMMSG_ESNOOWNERFORSTRINGALLOC : "Internal error: no owning script context for string allocation"
			throw CAnmError(ANMMSG_ESNOOWNERFORSTRINGALLOC);

		esstr2 = v.toString(strVal->GetOwner());
	}

	str2 = esstr2->GetString();

	assert(str1 && str2);
	int len = strlen(str1->GetBuf()) + strlen(str2->GetBuf());

	String newstr;

	if (len == 0)
		newstr = new CAnmString("");
	else
	{
		newstr = new CAnmString;
		newstr->alloc(len + 1);
		char *cp = newstr->GetBuf();		
		strcpy(cp, str1->GetBuf());
		strcat(cp, str2->GetBuf());
	}

	assert(pOwner);

	CAnmESString *newesstr = new CAnmESString(newstr, pOwner);

	// Drop one or this never goes away
	newstr->UnRef();

	return newesstr;
}

// string equality
bool AnmESVar::stringEquals(AnmESVar &v)
{
	CAnmESString *esstr = NULL;
	String str;

	assert (valueType == eString);

	if (v.valueType == eString)
		esstr = v.strVal;
	else
		esstr = v.toString(strVal->GetOwner());

	str = esstr->GetString();

	char *cp1 = str->GetBuf();
	char *cp2 = strVal->GetString()->GetBuf();

	bool retval = !strcmp(cp1, cp2);

	return retval;
}

// oughta be enuff
#define MAXNUMBUF 128

CAnmESString *AnmESVar::toString(class CAnmECMAScriptLanguageBinding *pOwner)
{
	String s = NULL;
	if (valueType == eString)
		return strVal->Clone();
	else if (valueType == eBoolean)
		s = boolVal ? new CAnmString("true") : new CAnmString("false");
	else if (valueType == eObject)
	{
		if (ptrVal == NULL)
			s = new CAnmString("(null)");
		else
			s = new CAnmString("(Object)");
	}
	else if (valueType == eNumber)
	{
		char buf[MAXNUMBUF];
	
		sprintf(buf, "%g", dblVal);
		
		s = new CAnmString(buf);
	}
	else
		s = new CAnmString("(unknown)");

	assert(s != NULL);

	CAnmESString *newesstr = new CAnmESString(s, pOwner);

	// Drop one or this never goes away
	s->UnRef();

	return newesstr;

}

double AnmESVar::stringToDouble()
{
	String s = strVal->GetString();
	double d;
	sscanf(s->GetBuf(), "%lg", &d);
	return d;
}