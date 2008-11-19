/********************************************************************************
 * Flux
 *
 * File: anmesvar.h
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

#ifndef _anmesvar_h
#define _anmesvar_h

// Forward declarations
class CAnmESFieldObject;
class CAnmESNodeObject;
class CAnmNode;
class CAnmSFRotation;
class CAnmSFVec3f;
class CAnmSAIMatrix4;

// AnmESVar - the variant data type for an ECMAScript variable
typedef struct AnmESVar {
	eESValueType valueType;
	union {
		double dblVal;
		BOOL boolVal;
		class CAnmESString *strVal;
		class CAnmESObject *ptrVal;
	};

	AnmESVar()
	{
		valueType = eUndefined;
		dblVal = 0.0;
	}

	AnmESVar(double d)
	{
		valueType = eNumber;
		dblVal = d;
	}

	AnmESVar(int i)
	{
		valueType = eNumber;
		dblVal = (double) i;
	}

	AnmESVar(class CAnmESString *str)
	{
		valueType = eString;
		strVal = str;
	}

	AnmESVar(class CAnmESObject *pObj)
	{
		valueType = eObject;
		ptrVal = pObj;
	}

	// type converters
	operator int()
	{
		return (int) asDouble();
	}

	operator double()
	{
		return asDouble();
	}

	double asDouble()
	{
		if (valueType == eNumber)
			return dblVal;
		else if (valueType == eBoolean)
			return (double) boolVal;
		else if (valueType == eObject)	// ?
			return (double) (ulong) ptrVal;
		else if (valueType == eString)
		{
			return stringToDouble();
		}

		return 0.0;
	}

	// ECAMScript object conversions
	operator CAnmESObject * ();
	operator CAnmESString * ();
	operator CAnmESFieldObject * ();
	operator CAnmESNodeObject * ();

	// Node conversion
	operator CAnmNode * ();

	// Field type conversions
	operator Color ();
	operator Image ();
	operator Rotation ();
	operator Point2 ();
	operator Point3 ();
	operator BooleanArray * ();
	operator ColorArray * ();
	operator DoubleArray * ();
	operator FloatArray * ();
	operator ImageArray * ();
	operator IntegerArray * ();
	operator NodeArray * ();
	operator RotationArray * ();
	operator StringArray * ();
	operator Point2Array * ();
	operator Point3Array * ();
	operator CAnmSFVec3f * ();
	operator CAnmSFRotation * ();
	operator CAnmSAIMatrix4 * ();

	// unary operators
	AnmESVar operator - ();
	AnmESVar operator ~ ();
	AnmESVar operator ! ();

	// binary operators
	// numeric
	AnmESVar operator * (AnmESVar &v);
	AnmESVar operator / (AnmESVar &v);
	AnmESVar operator % (AnmESVar &v);
	AnmESVar operator + (AnmESVar &v);
	AnmESVar operator - (AnmESVar &v);

	// bitwise
	AnmESVar operator << (AnmESVar &v);
	AnmESVar operator >> (AnmESVar &v);
	AnmESVar operator & (AnmESVar &v);
	AnmESVar operator ^ (AnmESVar &v);
	AnmESVar operator | (AnmESVar &v);

	// logical
	AnmESVar operator < (AnmESVar &v);
	AnmESVar operator > (AnmESVar &v);
	AnmESVar operator <= (AnmESVar &v);
	AnmESVar operator >= (AnmESVar &v);
	AnmESVar operator == (AnmESVar &v);
	AnmESVar operator != (AnmESVar &v);
	AnmESVar operator && (AnmESVar &v);
	AnmESVar operator || (AnmESVar &v);	

	// methods

	// string operations
	AnmESVar concat(AnmESVar &v);
	bool stringEquals(AnmESVar &v);
	class CAnmESString *toString(class CAnmECMAScriptLanguageBinding *pOwner);
	double stringToDouble();

} AnmESVar;

// Lil' helper - this is 'cause BOOL is an int on most systems...
inline AnmESVar AnmESBool(BOOL boolval)
{
	AnmESVar v;
	v.boolVal = boolval;
	v.valueType = eBoolean;
	return v;
}

// Creation from field class
AnmESVar AnmESVarFromField(class CAnmField *pField, class CAnmECMAScriptLanguageBinding *pOwner);

// Convert to field; this should be an operator but there's a weird, stupid compiler bug
CAnmField *AnmESVarToField(AnmESVar v);

// inline operators
// unary operators
inline AnmESVar AnmESVar::operator - ()
{
	return (AnmESVar) - asDouble();
}

inline AnmESVar AnmESVar::operator ~ ()
{
	return (AnmESVar) ~ ((int) *this);
}

inline AnmESVar AnmESVar::operator ! ()
{
	return AnmESBool( ! ((BOOL) *this) );
}

// binary operators
// numeric binary operators
inline AnmESVar AnmESVar::operator * (AnmESVar &v)
{
	return (AnmESVar) (asDouble() * v.asDouble());
}

inline AnmESVar AnmESVar::operator / (AnmESVar &v)
{
	double nv;
	if ((nv = v.asDouble()) == 0.f)
	{
		// N.B. throw ECMAScript exception
		return 0;
	}
	else
		return (AnmESVar) (asDouble() / nv);
}

inline AnmESVar AnmESVar::operator % (AnmESVar &v)
{
	double nv;
	if ((nv = v.asDouble()) == 0.f)
	{
		// N.B. throw ECMAScript exception
		return 0;
	}
	else
		return (AnmESVar) fmod(asDouble(), nv);
}

inline AnmESVar AnmESVar::operator + (AnmESVar &v)
{
	if (valueType != eString && v.valueType != eString)
		return (AnmESVar) (asDouble() + v.asDouble());
	else
		return concat(v);
}

inline AnmESVar AnmESVar::operator - (AnmESVar &v)
{
	return (AnmESVar) (asDouble() - v.asDouble());
}

// bitwise binary operators
inline AnmESVar AnmESVar::operator << (AnmESVar &v)
{
	return (AnmESVar) ( ((int) *this) << ((int) v) );
}

inline AnmESVar AnmESVar::operator >> (AnmESVar &v)
{
	return (AnmESVar) ( ((int) *this) >> ((int) v) );
}

inline AnmESVar AnmESVar::operator & (AnmESVar &v)
{
	return (AnmESVar) ( ((int) *this) & ((int) v) );
}

inline AnmESVar AnmESVar::operator ^ (AnmESVar &v)
{
	return (AnmESVar) ( ((int) *this) ^ ((int) v) );
}

inline AnmESVar AnmESVar::operator | (AnmESVar &v)
{
	return (AnmESVar) ( ((int) *this) | ((int) v) );
}

// logical binary operators
inline AnmESVar AnmESVar::operator < (AnmESVar &v)
{
	return AnmESBool( asDouble() < v.asDouble() );
}

inline AnmESVar AnmESVar::operator > (AnmESVar &v)
{
	return AnmESBool( asDouble() > v.asDouble() );
}

inline AnmESVar AnmESVar::operator <= (AnmESVar &v)
{
	return AnmESBool( asDouble() <= v.asDouble() );
}

inline AnmESVar AnmESVar::operator >= (AnmESVar &v)
{
	return AnmESBool( asDouble() >= v.asDouble() );
}

inline AnmESVar AnmESVar::operator == (AnmESVar &v)
{
	BOOL boolval = FALSE;
	if (valueType == eBoolean)
		boolval = (this->boolVal == AnmESBool(v).boolVal);
	else if (valueType == eNumber)
		boolval = (this->dblVal == (double) v);
	else if (valueType == eObject)
		boolval = (this->ptrVal == (CAnmESObject *) v);
	else if (valueType == eString)
		boolval = stringEquals(v);

	return AnmESBool( boolval );
}

inline AnmESVar AnmESVar::operator != (AnmESVar &v)
{
	return AnmESBool (! ((*this) == v));
}

inline AnmESVar AnmESVar::operator && (AnmESVar &v)
{
	return AnmESBool ( ((int) (*this)) && ((int) v) );
}

inline AnmESVar AnmESVar::operator || (AnmESVar &v)
{
	return AnmESBool ( ((int) (*this)) || ((int) v) );
}

#endif // _anmesvar_h
