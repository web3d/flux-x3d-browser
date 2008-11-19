/********************************************************************************
 * Flux
 *
 * File: anmesfields.h
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

#ifndef _anmesfields_h
#define _anmesfields_h

#include "anmesobject.h"

// field objects
#define MAXTOSTRBUFLEN 1024

class CAnmESFieldObject : public CAnmESBuiltinObject
{
public :

	// Constructor/destructor
	CAnmESFieldObject(class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESBuiltinObject(NULL, pOwner, eFieldObject) {}

	CAnmESFieldObject(class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner);
	CAnmESFieldObject(eValueType valueType, void *val, class CAnmECMAScriptLanguageBinding *pOwner);

	virtual ~CAnmESFieldObject();


	// Accessors
	class CAnmField *GetField()
	{
		return (CAnmField *) m_object;
	}

	void *GetValue();
	eValueType GetValueType();

	operator AnmESVar();

	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);
};

class CAnmESSFColor : public CAnmESFieldObject
{
public : 
	CAnmESSFColor (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESFieldObject(pFieldValue, pOwner)  {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// CAnmESFieldObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getR();
	virtual AnmESVar getG();
	virtual AnmESVar getB();
	virtual void setR(AnmESVar v);
	virtual void setG(AnmESVar v);
	virtual void setB(AnmESVar v);
	
	// methods
	// void setHSV(float h, float s, float v)
	virtual AnmESVar setHSV(int nArgs, AnmESVar *args);
	// numeric[3] getHSV( )
	virtual AnmESVar getHSV(int nArgs, AnmESVar *args);
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESSFColor);

	DECLARE_ESPROPERTY(r);
	DECLARE_ESPROPERTY(g);
	DECLARE_ESPROPERTY(b);

	DECLARE_ESMETHOD(setHSV);
	DECLARE_ESMETHOD(getHSV);
	DECLARE_ESMETHOD(toString);
};

class CAnmESSFImage : public CAnmESFieldObject
{
public : 
	CAnmESSFImage (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESFieldObject(pFieldValue, pOwner)  {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// properties
	virtual AnmESVar getX();
	virtual AnmESVar getY();
	virtual AnmESVar getComp();
	virtual AnmESVar getArray();
	virtual void setX(AnmESVar v);
	virtual void setY(AnmESVar v);
	virtual void setComp(AnmESVar v);
	virtual void setArray(AnmESVar v);
	
	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESSFColor);

	DECLARE_ESPROPERTY(x);
	DECLARE_ESPROPERTY(y);
	DECLARE_ESPROPERTY(comp);
	DECLARE_ESPROPERTY(array);

	DECLARE_ESMETHOD(toString);
};

class CAnmESSFNode : public CAnmESFieldObject
{
public : 
	CAnmESSFNode (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESFieldObject(pFieldValue, pOwner)  {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESSFNode);

	DECLARE_ESMETHOD(toString);
};

class CAnmESSFRotation : public CAnmESFieldObject
{
public : 
	CAnmESSFRotation (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESFieldObject(pFieldValue, pOwner)  {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// CAnmESFieldObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getX();
	virtual AnmESVar getY();
	virtual AnmESVar getZ();
	virtual AnmESVar getAngle();
	virtual void setX(AnmESVar v);
	virtual void setY(AnmESVar v);
	virtual void setZ(AnmESVar v);
	virtual void setAngle(AnmESVar v);
	
	// methods
	// SFVec3f getAxis( )
	virtual AnmESVar getAxis(int nArgs, AnmESVar *args);
	// SFRotation inverse( )
	virtual AnmESVar inverse(int nArgs, AnmESVar *args);
	// SFRotation multiply(SFRotation rot)
	virtual AnmESVar multiply(int nArgs, AnmESVar *args);
	// SFVec3f multVec(SFVec3f vec)
	virtual AnmESVar multVec(int nArgs, AnmESVar *args);
	// void setAxis(SFVec3f vec)
	virtual AnmESVar setAxis(int nArgs, AnmESVar *args);
	// SFRotation slerp(SFRotation dest, numeric t)
	virtual AnmESVar slerp(int nArgs, AnmESVar *args);
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESSFRotation);

	DECLARE_ESPROPERTY(x);
	DECLARE_ESPROPERTY(y);
	DECLARE_ESPROPERTY(z);
	DECLARE_ESPROPERTY(angle);

	DECLARE_ESMETHOD(getAxis);
	DECLARE_ESMETHOD(inverse);
	DECLARE_ESMETHOD(multiply);
	DECLARE_ESMETHOD(multVec);
	DECLARE_ESMETHOD(setAxis);
	DECLARE_ESMETHOD(slerp);
	DECLARE_ESMETHOD(toString);

};


class CAnmESSFTime : public CAnmESFieldObject
{
public : 
	CAnmESSFTime (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESFieldObject(pFieldValue, pOwner)  {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);
};

class CAnmESSFVec2f : public CAnmESFieldObject
{
public : 
	CAnmESSFVec2f (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESFieldObject(pFieldValue, pOwner)  {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// CAnmESFieldObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getX();
	virtual AnmESVar getY();
	virtual void setX(AnmESVar v);
	virtual void setY(AnmESVar v);
	
	// methods
	// SFVec2f add(SFVec2f vec)
	virtual AnmESVar add(int nArgs, AnmESVar *args);
	// SFVec2f divide(numeric n)
	virtual AnmESVar divide(int nArgs, AnmESVar *args);
	// numeric dot(SFVec2f vec)
	virtual AnmESVar dot(int nArgs, AnmESVar *args);
	// numeric length( )
	virtual AnmESVar length(int nArgs, AnmESVar *args);
	// SFVec2f multiply(numeric n)
	virtual AnmESVar multiply(int nArgs, AnmESVar *args);
	// SFVec2f normalize( )
	virtual AnmESVar normalize(int nArgs, AnmESVar *args);
	// SFVec2f subtract(SFVec2f vec)
	virtual AnmESVar subtract(int nArgs, AnmESVar *args);
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESSFVec2f);

	DECLARE_ESPROPERTY(x);
	DECLARE_ESPROPERTY(y);

	DECLARE_ESMETHOD(add);
	DECLARE_ESMETHOD(divide);
	DECLARE_ESMETHOD(dot);
	DECLARE_ESMETHOD(length);
	DECLARE_ESMETHOD(multiply);
	DECLARE_ESMETHOD(normalize);
	DECLARE_ESMETHOD(subtract);
	DECLARE_ESMETHOD(toString);


};

class CAnmESSFVec3f : public CAnmESFieldObject
{
public : 
	CAnmESSFVec3f (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESFieldObject(pFieldValue, pOwner) {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// CAnmESFieldObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getX();
	virtual AnmESVar getY();
	virtual AnmESVar getZ();
	virtual void setX(AnmESVar v);
	virtual void setY(AnmESVar v);
	virtual void setZ(AnmESVar v);

	// methods
	// SFVec3f add(SFVec3f vec)
	virtual AnmESVar add(int nArgs, AnmESVar *args);
	// SFVec3f cross(SFVec3f vec)
	virtual AnmESVar cross(int nArgs, AnmESVar *args);
	// SFVec2f divide(numeric n)
	virtual AnmESVar divide(int nArgs, AnmESVar *args);
	// numeric dot(SFVec3f vec)
	virtual AnmESVar dot(int nArgs, AnmESVar *args);
	// numeric length( )
	virtual AnmESVar length(int nArgs, AnmESVar *args);
	// SFVec3f multiply(numeric n)
	virtual AnmESVar multiply(int nArgs, AnmESVar *args);
	// SFVec3f negate( )
	virtual AnmESVar negate(int nArgs, AnmESVar *args);
	// SFVec3f normalize( )
	virtual AnmESVar normalize(int nArgs, AnmESVar *args);
	// SFVec3f subtract(SFVec3f vec)
	virtual AnmESVar subtract(int nArgs, AnmESVar *args);
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESSFVec3f);

	DECLARE_ESPROPERTY(x);
	DECLARE_ESPROPERTY(y);
	DECLARE_ESPROPERTY(z);

	DECLARE_ESMETHOD(add);
	DECLARE_ESMETHOD(cross);
	DECLARE_ESMETHOD(divide);
	DECLARE_ESMETHOD(dot);
	DECLARE_ESMETHOD(length);
	DECLARE_ESMETHOD(multiply);
	DECLARE_ESMETHOD(negate);
	DECLARE_ESMETHOD(normalize);
	DECLARE_ESMETHOD(subtract);
	DECLARE_ESMETHOD(toString);
};

class CAnmESMFieldObject : public CAnmESFieldObject
{
public :
	CAnmESMFieldObject (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESFieldObject(pFieldValue, pOwner)  {}

	// properties
	virtual AnmESVar getLength() PURE;
	virtual void setLength(AnmESVar v) PURE;

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args) PURE;

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFieldObject);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);


};

class CAnmESMFBool : public CAnmESMFieldObject
{
public : 
	CAnmESMFBool (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)  {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFBool);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};

class CAnmESMFColor : public CAnmESMFieldObject
{
public : 
	CAnmESMFColor (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)   {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);
	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFColor);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);


};

class CAnmESMFDouble : public CAnmESMFieldObject
{
public : 
	CAnmESMFDouble (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)   {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);
	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFDouble);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};

class CAnmESMFFloat : public CAnmESMFieldObject
{
public : 
	CAnmESMFFloat (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)   {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	virtual ~CAnmESMFFloat()
	{
	}

	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFFloat);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};

class CAnmESMFImage : public CAnmESMFieldObject
{
public : 
	CAnmESMFImage (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)   {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);
	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFImage);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};

class CAnmESMFInt32 : public CAnmESMFieldObject
{
public : 
	CAnmESMFInt32 (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)   {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);
	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFInt32);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};

class CAnmESMFNode : public CAnmESMFieldObject
{
public : 
	CAnmESMFNode (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)   {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);
	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFNode);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};

class CAnmESMFRotation : public CAnmESMFieldObject
{
public : 
	CAnmESMFRotation (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)   {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);
	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFRotation);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};

class CAnmESMFString : public CAnmESMFieldObject
{
public : 
	CAnmESMFString (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)   {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);
	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFString);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};

class CAnmESMFTime : public CAnmESMFieldObject
{
public : 
	CAnmESMFTime (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)   {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFTime);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};

class CAnmESMFVec2f : public CAnmESMFieldObject
{
public : 
	CAnmESMFVec2f (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)   {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFVec2f);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};

class CAnmESMFVec3f : public CAnmESMFieldObject
{
public : 
	CAnmESMFVec3f (class CAnmField *pFieldValue, class CAnmECMAScriptLanguageBinding *pOwner)
		: CAnmESMFieldObject(pFieldValue, pOwner)   {}
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// properties
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMFVec3f);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};


#endif // _anmesfields_h