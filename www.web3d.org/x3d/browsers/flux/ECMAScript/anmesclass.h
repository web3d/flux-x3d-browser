/********************************************************************************
 * Flux
 *
 * File: anmesclass.h
 * Description: Class data for ECMAScript builtin objects
 *						Layers on class scheme for real builtins
 *							but uses ESVars and varargs signatures
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

#ifndef _anmesclass_h
#define _anmesclass_h

#include "anmecmascript.h"

typedef AnmESVar (*ESMETHODPOINTER) (class CAnmESObject *pObject, int nArgs, AnmESVar *args);
typedef void (*ESPUTPROPPOINTER) (class CAnmESObject *pObject, AnmESVar propValue);
typedef AnmESVar (*ESGETPROPPOINTER) (class CAnmESObject *pObject);

// Helpers to use in object's class declaration (header)
#define ESMETHODNAME(_nm_) m_esmethod##_nm_
#define ESMETHODID(_cls_, _nm_) (&_cls_::ESMETHODNAME(_nm_))
#define ESPROPNAME(_nm_) m_esprop##_nm_
#define ESPROPID(_cls_, _nm_) (&_cls_::ESPROPNAME(_nm_))

#define DECLARE_ESMETHOD(_mth_) \
static CAnmClassMember ESMETHODNAME(_mth_);

#define DECLARE_ESPROPERTY(_val_) \
static CAnmClassMember ESPROPNAME(_val_);

// Helpers used in class implementation (source)

// N.B.: crap to help get around not being able to cast method pointers;
// each class's source file invokes these macros to define and reference 
// statically scoped helper functions that turn around and call object methods
// -- ick. $5 to somebody who shows me a better way. T

#define DEFESGETPROPHELPER(_cls, _mth) \
static AnmESVar gp##_mth##_cls(_cls *obj) { return obj->_mth(); }
#define DEFESPUTPROPHELPER(_cls, _mth) \
static void pp##_mth##_cls(_cls *obj, AnmESVar propVal) { obj->_mth(propVal); }
#define DEFESCALLMETHODHELPER(_cls, _mth) \
static AnmESVar m##_mth##_cls(_cls *obj, int nArgs, AnmESVar *args) { return obj->_mth(nArgs, args); }

#define ESGETPROPHELPER(_mth,_cls) (GETVALUEPOINTER)&gp##_mth##_cls
#define ESPUTPROPHELPER(_mth,_cls) (SETVALUEPOINTER)&pp##_mth##_cls
#define ESCALLMETHODHELPER(_mth,_cls) (CALLMETHODPOINTER)&m##_mth##_cls


#define DEFINE_ESMETHOD(_cls_, _mth_) \
DEFESCALLMETHODHELPER(_cls_, _mth_); \
CAnmClassMember _cls_::ESMETHODNAME(_mth_) = \
{ \
	#_mth_, \
	eValueBadType, \
	eAnmMethodMember, \
	(MEMBEROFFSET) ESCALLMETHODHELPER(_mth_,_cls_), \
	NULL, \
};

#define DEFINE_ESPROPERTY(_cls_, _val_, _getmth_, _setmth_) \
DEFESGETPROPHELPER(_cls_, _getmth_); \
DEFESPUTPROPHELPER(_cls_, _setmth_); \
CAnmClassMember _cls_::ESPROPNAME(_val_) = \
{ \
	#_val_, \
	eValueBadType, \
	eAnmValueMember, \
	(MEMBEROFFSET) ESGETPROPHELPER(_getmth_,_cls_), \
	ESPUTPROPHELPER(_setmth_,_cls_), \
};

#define DEFINE_ESREADONLYPROPERTY(_cls_, _val_, _getmth_) \
DEFESGETPROPHELPER(_cls_, _getmth_); \
CAnmClassMember _cls_::ESPROPNAME(_val_) = \
{ \
	#_val_, \
	eValueBadType, \
	eAnmValueMember, \
	(MEMBEROFFSET) ESGETPROPHELPER(_getmth_,_cls_), \
	NULL, \
};

#define DEFINE_ESWRITEONLYPROPERTY(_cls_, _val_, _setmth_) \
DEFESPUTPROPHELPER(_cls_, _setmth_); \
CAnmClassMember _cls_::ESPROPNAME(_val_) = \
{ \
	#_val_, \
	eValueBadType, \
	eAnmValueMember, \
	NULL, \
	ESPUTPROPHELPER(_setmth_), \
};

#define DEFINE_ESINHERITED_PROPERTY(_cls_, _val_, _super_) \
CAnmClassMember _cls_::ESPROPNAME(_val_) = _super_::ESPROPNAME(_val_);

#define DEFINE_ESINHERITED_METHOD(_cls_, _val_, _super_) \
CAnmClassMember _cls_::ESMETHODNAME(_val_) = _super_::ESMETHODNAME(_val_);


#endif // _anmesclass_h

