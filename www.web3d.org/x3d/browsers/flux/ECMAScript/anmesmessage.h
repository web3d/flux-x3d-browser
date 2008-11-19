/********************************************************************************
 * Flux
 *
 * File: anmesmessage.h
 * Description: ECMAScript Message definitions
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

#ifndef _anmesmessage_h
#define _anmesmessage_h


// ECMAScript
#define ANMMSG_ESCOMPILEERROR			"ECMAScript compile error: [%s] at line %d"
#define ANMMSG_ESCOMPILEWARNING			"ECMAScript compile warning: [%s] at line %d"
#define ANMMSG_ESERRORSTATUS			"Flux: ECMAScript error"
#define ANMMSG_ESRESERVEDWORD			"ECMAScript warning: reserved word '%s' used at line '%d"
#define ANMMSG_ESINVALIDOBJECTPUTPROP	"ECMAScript intepreter: invalid object in PutPropV call (property '%s') "
#define ANMMSG_ESINVALIDOBJECTGETPROP	"ECMAScript intepreter: invalid object in GetPropV call (property '%s') "
#define ANMMSG_ESATTEMPTSETUNKNOWNPROP	"Attempt to set value of unknown property '%s'"
#define ANMMSG_ESATTEMPTGETUNKNOWNPROP	"Attempt to get value of unknown property '%s'"
#define ANMMSG_ESATTEMPTSETPROPNULLNODE	"Attempt to set property of NULL node"
#define ANMMSG_ESATTEMPTGETPROPNULLNODE	"Attempt to get property of NULL node"
#define ANMMSG_ESINCOMPATIBLEFIELDTYPES	"Incompatible field types"
#define ANMMSG_ESEXPECTEDFIELDOBJECT	"Expected field object"
#define ANMMSG_ESOBJDOESNTSUPPORTARRAY	"Object type does not support array properties"
#define ANMMSG_ESINVALIDOBJECTPUTPROPA	"ECMAScript intepreter: invalid object in PutArrayPropV call"
#define ANMMSG_ESINVALIDOBJECTGETPROPA	"ECMAScript intepreter: invalid object in GetArrayPropV call"
#define ANMMSG_ESINVALIDOBJECTCALLMETHOD	"ECMAScript intepreter: invalid object in CallMethodNV call"
#define ANMMSG_ESINVALIDOBJECTNEWOP		"ECMAScript intepreter: invalid type code in NewOpNV call"
#define ANMMSG_ESBADOPCODE				"[CAnmESCode::Execute] bad opcode"
#define ANMMSG_ESBADOPERATOR			"[CAnmESCode::OperatorCall] bad operator"
#define ANMMSG_ESUNKNOWNPROPERTY		"Unknown property: %s"
#define ANMMSG_ESUNKNOWNMETHOD			"Unknown method call: %s"
#define ANMMSG_ESCREATEVRMLNOTSTRING	"Browser::createVrmlFromUrl: Can't convert parameter 1 to string array value"
#define ANMMSG_ESCREATEVRMLNOTNODE		"Browser::createVrmlFromUrl: Expected node object in parameter 2"
#define ANMMSG_ESCREATEVRMLNOTEVENT		"Browser::createVrmlFromUrl: Expected event name in parameter 3"
#define ANMMSG_ESMATRIXINDEXOOB			"Matrix4: index out of bounds"
#define ANMMSG_ESMATRIXARGSMULTLEFT		"Wrong number of arguments to Matrix4::multLeft"
#define ANMMSG_ESMATRIXARGSMULTRIGHT	"Wrong number of arguments to Matrix4::multRight"
#define ANMMSG_ESMATRIXARGSMULTVM		"Wrong number of arguments to Matrix4::multVecMatrix"
#define ANMMSG_ESMATRIXARGSMULTMV		"Wrong number of arguments to Matrix4::multMatrixVec"
#define ANMMSG_ESCANTCONVERTTOSFCOLOR	"Can't convert variable to SFColor value"
#define ANMMSG_ESCANTCONVERTTOSFIMAGE	"Can't convert variable to SFImage value"
#define ANMMSG_ESCANTCONVERTTOSFROTATION	"Can't convert variable to SFRotation value"
#define ANMMSG_ESCANTCONVERTTOSFVEC2F	"Can't convert variable to SFVec2f value"
#define ANMMSG_ESCANTCONVERTTOSFVEC3F	"Can't convert variable to SFVec3f value"
#define ANMMSG_ESCANTCONVERTTOMATRIX4	"Can't convert variable to Matrix4 value"
#define ANMMSG_ESCANTCONVERTTOMFBOOL	"Can't convert variable to MFBool value"
#define ANMMSG_ESCANTCONVERTTOMFCOLOR	"Can't convert variable to MFColor value"
#define ANMMSG_ESCANTCONVERTTOMFDOUBLE	"Can't convert variable to MFDouble value"
#define ANMMSG_ESCANTCONVERTTOMFFLOAT	"Can't convert variable to MFFloat value"
#define ANMMSG_ESCANTCONVERTTOMFIMAGE	"Can't convert variable to MFImage value"
#define ANMMSG_ESCANTCONVERTTOMFINT32	"Can't convert variable to MFInt32 value"
#define ANMMSG_ESCANTCONVERTTOMFNODE	"Can't convert variable to MFNode value"
#define ANMMSG_ESCANTCONVERTTOMFROTATION	"Can't convert variable to MFRotation value"
#define ANMMSG_ESCANTCONVERTTOMFSTRING	"Can't convert variable to MFString value"
#define ANMMSG_ESCANTCONVERTTOMFVEC2F	"Can't convert variable to MFVec2f value"
#define ANMMSG_ESCANTCONVERTTOMFVEC3F	"Can't convert variable to MFVec3f value"
#define ANMMSG_ESNOOWNERFORSTRINGALLOC	"Internal error: no owning script context for string allocation"

#endif //_anmesmessage_h