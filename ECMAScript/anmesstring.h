/********************************************************************************
 * Flux
 *
 * File: anmesstring.h
 * Description: ECMAScript String object
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

#ifndef _anmesstring_h
#define _anmesstring_h

#include "anmesobject.h"

class CAnmESString : public CAnmESBuiltinObject
{
protected :

public :

	// Constructor/destructor
	CAnmESString(String s, class CAnmECMAScriptLanguageBinding *pOwner);
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);
	virtual ~CAnmESString();

	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// Accessors
	String GetString()
	{
		return (CAnmString *) m_object;
	}

	// properties
	virtual AnmESVar getLength();

	// methods
	// String toString( )
	virtual AnmESVar toString(int nArgs, AnmESVar *args);
	virtual CAnmESString *Clone();

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESString);

	DECLARE_ESPROPERTY(length);

	DECLARE_ESMETHOD(toString);

};

#endif // _anmesstring_h
