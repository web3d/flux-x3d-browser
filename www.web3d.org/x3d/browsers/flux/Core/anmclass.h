/********************************************************************************
 * Flux
 *
 * File: anmclass.cpp
 * Description: Class metadata for object system
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

#ifndef _anmclass_h
#define _anmclass_h

typedef void (*CALLMETHODPOINTER) (class CAnmObject *pNode, void *pValue);
typedef void (*SETVALUEPOINTER) (class CAnmObject *pNode, void *pValue);
typedef void (*GETVALUEPOINTER) (class CAnmObject *pNode, void *ppValue);

typedef int MEMBEROFFSET;
#define CALCMEMBEROFFSET(_cls_, _mmb_) (MEMBEROFFSET) &((_cls_* ) NULL)->_mmb_

enum eAnmClassMemberType {
	eAnmInitMember			=		0x01,
	eAnmValueMember			=		0x02,
	eAnmMethodMember		=		0x04,
	eAnmCallbackMember		=		0x08,
	eAnmInheritedMember		=		0x10,
};

struct CAnmClassMember
{
public :
	char *name;								// external name (for automation etc.)
	eValueType valueType;					// used by methods, callbacks and values
	eAnmClassMemberType memberType;			// flag-- Callback, Method or Value
	union {
	MEMBEROFFSET	memberOffset;			// for Callbacks
	CALLMETHODPOINTER pCallMethod;			// for CallMethod
	GETVALUEPOINTER pGetValue;				// for GetValue; used only for Value members
	CAnmClassMember *pInherited;			// used to jam in copied values for superclass
	};
	SETVALUEPOINTER pSetValue;				// for SetValue; used only for Value members
};

typedef class CAnmObject * ((*pFnCreateObject)(void));

class EXPORT CAnmClass
{
public :

	char					 *m_className;
	pFnCreateObject			  m_createObject;		// class Factory
	CAnmClassMember			**m_memberTable;		// list of Callbacks, Methods and Values
	int						  m_numMembers;			// size of member list
	CAnmClass				 *m_superclass;			// pointer to superclass type
	class CAnmClassSymbol	 *m_classSymbol;
	bool					  m_fixupinherited;		// flags that we need to fix some values at init time

	// methods

	class CAnmObject *Create();
	CAnmClassMember *FindClassMember(char *name);
	CAnmClassMember *FindClassMember(char *name, eAnmClassMemberType memberType);
	CAnmClassMember *FindCallback(char *name);
	CAnmClassMember *FindMethod(char *name);
	CAnmClassMember *FindValue(char *name);
	CAnmClassMember *FindInit(char *name);
	BOOL IsA(CAnmClass *pTargetSuperclass)
	{
		if (this == pTargetSuperclass)
			return TRUE;
		else if (m_createObject && (m_createObject == pTargetSuperclass->m_createObject))
			return TRUE;
		else if (m_superclass)
			return m_superclass->IsA(pTargetSuperclass);
		else
			return FALSE;
	}
	void FixupInherited();

	// accessors
	CAnmClassMember **GetMemberTable()
	{
		if (m_fixupinherited)
			FixupInherited();

		return m_memberTable;
	}
	int GetNumMembers()
	{
		return m_numMembers;
	}
	CAnmClass *GetSuperClass()
	{
		return m_superclass;
	}
	pFnCreateObject	GetCreateObject()
	{
		return m_createObject;
	}
};

// Class identifier is a pointer to the object's static ClassType member
#define GETCLASS(_cls_) (_cls_::GetClassData())

// ClassMember scheme: static variables in object class point to ClassMember objects
// that describe each exposed member. A prefix is prepended to distinguish callbacks, methods
// and values (properties); this keeps the namespaces separate.
// The ClassMember identifier is a pointer to this static variable. These macros help
// declare the static class members, and access them appropriately
#define CALLBACKNAME(_nm_) m_callback##_nm_
#define CALLBACKID(_cls_, _nm_) (&_cls_::CALLBACKNAME(_nm_))
#define METHODNAME(_nm_) m_method##_nm_
#define METHODID(_cls_, _nm_) (&_cls_::METHODNAME(_nm_))
#define VALUENAME(_nm_) m_value##_nm_
#define VALUEID(_cls_, _nm_) (&_cls_::VALUENAME(_nm_))

// this helps objects manage their list of callbacks
#define CALLBACKLISTNAME(_nm_) m_callbacklist_##_nm_
#define CALLBACKLISTPTR(_nm_) (&m_callbacklist##_nm_)

typedef CAnmClassMember *CLASSMEMBERID;

// Helpers to use in object's class declaration (header)

#define DECLARE_CLASS(_cls_) \
static class CAnmClass *m_classdata; \
static class CAnmClass *GetClassData(); \
virtual class CAnmClass *GetClass();

#define DECLARE_CALLBACK(_cb_) \
CAnmCallbackList CALLBACKLISTNAME(_cb_); \
static CAnmClassMember CALLBACKNAME(_cb_);

#define DECLARE_METHOD(_mth_) \
static CAnmClassMember METHODNAME(_mth_);

#define DECLARE_VALUE(_val_) \
static CAnmClassMember VALUENAME(_val_);

#define DECLARE_INITVALUE(_val_) \
static CAnmClassMember VALUENAME(_val_);

// Helpers used in class implementation (source)

// N.B.: crap to help get around not being able to cast method pointers;
// each class's source file invokes these macros to define and reference 
// statically scoped helper functions that turn around and call object methods
// -- ick. $5 to somebody who shows me a better way. T

#define DEFGETVALUEHELPER(_cls, _mth, _ty) \
static void gv##_mth(_cls *obj, _ty *pValue) { obj->_mth(pValue); }
#define DEFSETVALUEHELPER(_cls, _mth, _ty) \
static void sv##_mth(_cls *obj, _ty *pValue) { obj->_mth(*pValue); }
#define DEFCALLMETHODHELPER(_cls, _mth, _ty) \
static void m##_mth(_cls *obj, _ty *pValue) { obj->_mth(*pValue); }

#define GETVALUEHELPER(_mth) (GETVALUEPOINTER)&gv##_mth
#define SETVALUEHELPER(_mth) (SETVALUEPOINTER)&sv##_mth
#define CALLMETHODHELPER(_mth) (CALLMETHODPOINTER)&m##_mth


#define DEFINE_CALLBACK(_cls_, _cb_, _ty_) \
CAnmClassMember _cls_::CALLBACKNAME(_cb_) = \
{ \
	#_cb_, \
	_ty_, \
	eAnmCallbackMember, \
	CALCMEMBEROFFSET(_cls_, CALLBACKLISTNAME(_cb_)), \
	NULL, \
 };

#define DEFINE_METHOD(_cls_, _mth_, _ty_, _objtype_, _mthname_) \
DEFCALLMETHODHELPER(_cls_, _mthname_, _objtype_); \
CAnmClassMember _cls_::METHODNAME(_mth_) = \
{ \
	#_mth_, \
	_ty_, \
	eAnmMethodMember, \
	(MEMBEROFFSET) CALLMETHODHELPER(_mthname_), \
	NULL, \
};

#define DEFINE_VALUE(_cls_, _val_, _ty_, _objtype_, _getmth_, _setmth_) \
DEFGETVALUEHELPER(_cls_, _getmth_, _objtype_); \
DEFSETVALUEHELPER(_cls_, _setmth_, _objtype_); \
CAnmClassMember _cls_::VALUENAME(_val_) = \
{ \
	#_val_, \
	_ty_, \
	eAnmValueMember, \
	(MEMBEROFFSET) GETVALUEHELPER(_getmth_), \
	SETVALUEHELPER(_setmth_), \
};

#define DEFINE_INITVALUE(_cls_, _val_, _ty_, _objtype_, _setmth_) \
DEFSETVALUEHELPER(_cls_, _setmth_, _objtype_); \
CAnmClassMember _cls_::VALUENAME(_val_) = \
{ \
	#_val_, \
	_ty_, \
	eAnmInitMember, \
	NULL, \
	SETVALUEHELPER(_setmth_), \
};

#define DEFINE_INHERITED_VALUE(_cls_, _val_, _super_) \
CAnmClassMember _cls_::VALUENAME(_val_) = \
{ \
	#_val_, \
	eValueBadType, \
	eAnmInheritedMember, \
	(MEMBEROFFSET) &_super_::VALUENAME(_val_), \
	NULL, \
};

#define DEFINE_INHERITED_METHOD(_cls_, _val_, _super_) \
CAnmClassMember _cls_::METHODNAME(_val_) = \
{ \
	#_val_, \
	eValueBadType, \
	eAnmInheritedMember, \
	(MEMBEROFFSET) &_super_::METHODNAME(_val_), \
	NULL, \
};

#define DEFINE_INHERITED_CALLBACK(_cls_, _val_, _super_) \
CAnmClassMember _cls_::CALLBACKNAME(_val_) = \
{ \
	#_val_, \
	eValueBadType, \
	eAnmInheritedMember, \
	(MEMBEROFFSET) &_super_::CALLBACKNAME(_val_), \
	NULL, \
};

#define BEGIN_DEFINE_MEMBERS(_cls_) \
static CAnmClassMember *memberTable##_cls_[] = \
{

#define END_DEFINE_MEMBERS \
};


// class definition macros-- a slew of them to handle different permutations. there's got
// to be a better way

#define DEFINE_CLASS(_cls_, _super_) \
static CAnmObject *create##_cls_() \
{ \
	return new _cls_(); \
} \
\
static CAnmClass classdata##_cls_ = \
{ \
	#_cls_, \
	create##_cls_, \
	memberTable##_cls_, \
	sizeof(memberTable##_cls_) / sizeof(CAnmClassMember *), \
	_super_::m_classdata, \
	NULL, \
	true, \
}; \
CAnmClass *_cls_::m_classdata = &classdata##_cls_; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; } \
CAnmClass *_cls_::GetClass() { return _cls_::m_classdata; }

// abstract class
#define DEFINE_ABSTRACT_CLASS(_cls_, _super_) \
static CAnmClass classdata##_cls_ = \
{ \
	#_cls_, \
	NULL, \
	memberTable##_cls_, \
	sizeof(memberTable##_cls_) / sizeof(CAnmClassMember *), \
	_super_::m_classdata, \
	NULL, \
	true, \
}; \
CAnmClass *_cls_::m_classdata = &classdata##_cls_; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; } \
CAnmClass *_cls_::GetClass() { return _cls_::m_classdata; }

// class with no members
#define DEFINE_EMPTY_CLASS(_cls_, _super_) \
static CAnmObject *create##_cls_() \
{ \
	return new _cls_(); \
} \
\
static CAnmClass classdata##_cls_ = \
{ \
	#_cls_, \
	create##_cls_, \
	NULL, \
	0, \
	_super_::m_classdata, \
	NULL, \
	true, \
}; \
CAnmClass *_cls_::m_classdata = &classdata##_cls_; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; } \
CAnmClass *_cls_::GetClass() { return _cls_::m_classdata; }

// abstract class with no members
#define DEFINE_ABSTRACT_EMPTY_CLASS(_cls_, _super_) \
static CAnmClass classdata##_cls_ = \
{ \
	#_cls_, \
	NULL, \
	NULL, \
	0, \
	_super_::m_classdata, \
	NULL, \
	true, \
}; \
CAnmClass *_cls_::m_classdata = &classdata##_cls_; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; } \
CAnmClass *_cls_::GetClass() { return _cls_::m_classdata; }

// class with no superclass
#define DEFINE_ROOT_CLASS(_cls_) \
static CAnmObject *create##_cls_() \
{ \
	return new _cls_(); \
} \
\
static CAnmClass classdata##_cls_ = \
{ \
	#_cls_, \
	create##_cls_, \
	memberTable##_cls_, \
	sizeof(memberTable##_cls_) / sizeof(CAnmClassMember), \
	NULL, \
	NULL, \
	true, \
}; \
CAnmClass *_cls_::m_classdata = &classdata##_cls_; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; } \
CAnmClass *_cls_::GetClass() { return _cls_::m_classdata; }

// abstract class with no superclass
#define DEFINE_ABSTRACT_ROOT_CLASS(_cls_) \
static CAnmClass classdata##_cls_ = \
{ \
	#_cls_, \
	NULL, \
	memberTable##_cls_, \
	sizeof(memberTable##_cls_) / sizeof(CAnmClassMember), \
	NULL, \
	NULL, \
	true, \
}; \
CAnmClass *_cls_::m_classdata = &classdata##_cls_; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; } \
CAnmClass *_cls_::GetClass() { return _cls_::m_classdata; }

// class with no superclass *and* no members
#define DEFINE_EMPTY_ROOT_CLASS(_cls_) \
static CAnmObject *create##_cls_() \
{ \
	return new _cls_(); \
} \
\
static CAnmClass classdata##_cls_ = \
{ \
	#_cls_, \
	create##_cls_, \
	NULL, \
	0, \
	NULL, \
	NULL, \
	true, \
}; \
CAnmClass *_cls_::m_classdata = &classdata##_cls_; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; } \
CAnmClass *_cls_::GetClass() { return _cls_::m_classdata; }

// and finally (urpf), an abstract class with no superclass *and* no members
#define DEFINE_ABSTRACT_EMPTY_ROOT_CLASS(_cls_) \
static CAnmClass classdata##_cls_ = \
{ \
	#_cls_, \
	NULL, \
	NULL, \
	0, \
	NULL, \
	NULL, \
	true, \
}; \
CAnmClass *_cls_::m_classdata = &classdata##_cls_; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; } \
CAnmClass *_cls_::GetClass() { return _cls_::m_classdata; }

typedef int ANMINTERFACE_VALUE;
typedef ANMINTERFACE_VALUE *ANMINTERFACE;

#define ANMDECLARE_INTERFACE(_cls_) static ANMINTERFACE_VALUE m_iface##_cls_
#define ANMDEFINE_INTERFACE(_cls_) ANMINTERFACE_VALUE _cls_::m_iface##_cls_ = 0L;

#define ANMINTERFACEID(_cls_) (&_cls_::m_iface##_cls_)

#endif // _anmclasstype_h

