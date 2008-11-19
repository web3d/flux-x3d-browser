/********************************************************************************
 * Flux
 *
 * File: anmesobject.h
 * Description: ECMAScript base object
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

#ifndef _anmesobject_h
#define _anmesobject_h

#include "anmecmascript.h"
#include "anmesclass.h"
#include <map>

typedef enum eESObjectType
{
	eBuiltinObject,			// built-in object (node, field, browser, scene, route etc)
	eUserObject,			// user-defined object
} eESObjectType ;

class CAnmESObject
{
protected :

	eESObjectType							 m_objType;
	class CAnmECMAScriptLanguageBinding		*m_owner;

	BOOL									 m_marked;

	void SetObjectType(eESObjectType objType)
	{
		m_objType = objType;
	}

	friend class CAnmECMAScriptLanguageBinding;

	// Garbage collection help
	void Mark()
	{
		m_marked = TRUE;
	}

	void Unmark()
	{
		m_marked = FALSE;
	}

	BOOL IsMarked()
	{
		return m_marked;
	}

public :

	CAnmESObject(class CAnmECMAScriptLanguageBinding *pOwner);
	CAnmESObject(eESObjectType objType, class CAnmECMAScriptLanguageBinding *pOwner);

	virtual ~CAnmESObject()
	{
	}

	// Accessors

	eESObjectType GetObjectType()
	{
		return m_objType;
	}

	class CAnmECMAScriptLanguageBinding *GetOwner()
	{
		return m_owner;
	}

	// Methods
	virtual void PutProperty(char *propName, AnmESVar propValue) PURE ;
	virtual AnmESVar GetProperty(char *propName) PURE ;
	virtual void PutArrayProperty(AnmESVar vindex, AnmESVar propValue) PURE ;
	virtual AnmESVar GetArrayProperty(AnmESVar vindex) PURE ;
	virtual AnmESVar CallMethod(char *methodName, int nArgs, AnmESVar *args) PURE;
};

typedef enum eESBuiltinObjectType
{
	eNodeObject,			// points to node
	eFieldObject,			// points to field
	eOtherObject,			// points to something else
} eESBuiltinObjectType ;

class CAnmESBuiltinObject : public CAnmESObject
{
protected :

	class CAnmObject		*m_object;
	eESBuiltinObjectType	 m_builtinType;

public :

	// Constructor/destructor
	CAnmESBuiltinObject(class CAnmObject *pObj, 
		class CAnmECMAScriptLanguageBinding *pOwner,
		eESBuiltinObjectType builtinType = eOtherObject);
	virtual ~CAnmESBuiltinObject();

	// Accessors

	class CAnmObject *GetObject()
	{
		return m_object;
	}

	eESBuiltinObjectType GetBuiltinType()
	{
		return m_builtinType;
	}
	
	// CAnmESObject overrides
	virtual void PutProperty(char *propName, AnmESVar propValue);
	virtual AnmESVar GetProperty(char *propName);
	virtual void PutArrayProperty(AnmESVar vindex, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);
	virtual AnmESVar CallMethod(char *methodName, int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESBuiltinObject);

};

struct sAnmESAssoc
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

typedef std::map<const char*, AnmESVar, sAnmESAssoc> tAnmESAssocMap;

class CAnmESUserObject : public CAnmESObject
{
protected :

	tAnmESAssocMap				m_map;
	
public :

	// Constructor/destructor
	CAnmESUserObject(class CAnmECMAScriptLanguageBinding *pOwner);
	CAnmESUserObject(class CAnmECMAScriptLanguageBinding *pOwner, int nArrayElements, 
		CAnmESObject **ppArrayElements);
	virtual ~CAnmESUserObject();

	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// CAnmESObject overrides
	virtual void PutProperty(char *propName, AnmESVar propValue);
	virtual AnmESVar GetProperty(char *propName);
	virtual void PutArrayProperty(AnmESVar vindex, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);
	virtual AnmESVar CallMethod(char *methodName, int nArgs, AnmESVar *args);

	// New methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);
	// numeric length( )
	virtual AnmESVar getLength();
	virtual void setLength(AnmESVar v);

	// Accessors


	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESUserObject);

	DECLARE_ESPROPERTY(length);
	DECLARE_ESMETHOD(toString);
};

#endif // _anmesobject_h
