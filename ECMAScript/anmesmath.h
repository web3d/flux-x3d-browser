/********************************************************************************
 * Flux
 *
 * File: anmesmath.h
 * Description: ECMAScript math object
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

#ifndef _anmesmath_h
#define _anmesmath_h

#include "anmesobject.h"

class CAnmESMathObject : public CAnmESBuiltinObject
{
protected :

	static double m_E;
	static double m_LN10;
	static double m_LN2;
	static double m_PI;
	static double m_SQRT1_2;
	static double m_SQRT2;

public :

	// Constructor/destructor
	CAnmESMathObject(class CAnmECMAScriptLanguageBinding *pOwner);
	virtual ~CAnmESMathObject()
	{
	}

	// New Methods

	// getprops
	AnmESVar getE();
	AnmESVar getLN10();
	AnmESVar getLN2();
	AnmESVar getPI();
	AnmESVar getSQRT1_2();
	AnmESVar getSQRT2();
	
	// methods
	AnmESVar abs(int nArgs, AnmESVar *args);
	AnmESVar acos(int nArgs, AnmESVar *args);
	AnmESVar asin(int nArgs, AnmESVar *args);
	AnmESVar atan(int nArgs, AnmESVar *args);
	AnmESVar ceil(int nArgs, AnmESVar *args);
	AnmESVar cos(int nArgs, AnmESVar *args);
	AnmESVar exp(int nArgs, AnmESVar *args);
	AnmESVar floor(int nArgs, AnmESVar *args);
	AnmESVar log(int nArgs, AnmESVar *args);
	AnmESVar maxfn(int nArgs, AnmESVar *args);
	AnmESVar minfn(int nArgs, AnmESVar *args);
	AnmESVar pow(int nArgs, AnmESVar *args);
	AnmESVar random(int nArgs, AnmESVar *args);
	AnmESVar round(int nArgs, AnmESVar *args);
	AnmESVar sin(int nArgs, AnmESVar *args);
	AnmESVar sqrt(int nArgs, AnmESVar *args);
	AnmESVar tan(int nArgs, AnmESVar *args); 
	
	// CAnmESBuiltinObject overrides - needed because of a few name clases
	virtual AnmESVar GetProperty(char *propName);
	virtual AnmESVar CallMethod(char *methodName, int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMathObject);

	DECLARE_ESPROPERTY(E);
	DECLARE_ESPROPERTY(LN10);
	DECLARE_ESPROPERTY(LN2);
	// N.B.: can't declare PI, it blows up because of macros
	// DECLARE_ESPROPERTY(PI);
	DECLARE_ESPROPERTY(SQRT1_2);
	DECLARE_ESPROPERTY(SQRT2);

	DECLARE_ESMETHOD(abs);
	DECLARE_ESMETHOD(acos);
	DECLARE_ESMETHOD(asin);
	DECLARE_ESMETHOD(atan);
	DECLARE_ESMETHOD(ceil);
	DECLARE_ESMETHOD(cos);
	DECLARE_ESMETHOD(exp);
	DECLARE_ESMETHOD(floor);
	DECLARE_ESMETHOD(log);
	// N.B.: can't declare min/max, they blow up because of macros
	// DECLARE_ESMETHOD(max);
	// DECLARE_ESMETHOD(min);
	DECLARE_ESMETHOD(pow);
	DECLARE_ESMETHOD(random);
	DECLARE_ESMETHOD(round);
	DECLARE_ESMETHOD(sin);
	DECLARE_ESMETHOD(sqrt);
	DECLARE_ESMETHOD(tan);

};

#endif // _anmesmath_h
