/********************************************************************************
 * Flux
 *
 * File: anmesmath.cpp
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
 
#include "stdafx.h"
#include "anmesmath.h"
#include "anmmath.h"

// Math object

double CAnmESMathObject::m_E = ANM_E;
double CAnmESMathObject::m_LN10 = ANM_LN10;
double CAnmESMathObject::m_LN2 = ANM_LN2;
double CAnmESMathObject::m_PI = ANM_PI;
double CAnmESMathObject::m_SQRT1_2 = ANM_SQRT1_2;
double CAnmESMathObject::m_SQRT2 = ANM_SQRT2;

CAnmESMathObject::CAnmESMathObject(CAnmECMAScriptLanguageBinding *pOwner)
	: 	CAnmESBuiltinObject(NULL, pOwner)

{
	// seed the random number generator
	srand( (unsigned)time( NULL ) );
}

// workaround for 'PI'
AnmESVar CAnmESMathObject::GetProperty(char *propName)
{
	if (!strcmp(propName, "PI"))
		return getPI();
	else
		return CAnmESBuiltinObject::GetProperty(propName);
}

AnmESVar CAnmESMathObject::CallMethod(char *methodName, int nArgs, AnmESVar *args)
{
	if (!strcmp(methodName, "max"))
		return maxfn(nArgs, args);
	else if (!strcmp(methodName, "min"))
		return minfn(nArgs, args);
	else
		return CAnmESBuiltinObject::CallMethod(methodName, nArgs, args);
}


// getprops
AnmESVar CAnmESMathObject::getE()
{
	return (AnmESVar) m_E;
}

AnmESVar CAnmESMathObject::getLN10()
{
	return (AnmESVar) m_LN10;
}

AnmESVar CAnmESMathObject::getLN2()
{
	return (AnmESVar) m_LN2;
}

AnmESVar CAnmESMathObject::getPI()
{
	return (AnmESVar) m_PI;
}

AnmESVar CAnmESMathObject::getSQRT1_2()
{
	return (AnmESVar) m_SQRT1_2;
}

AnmESVar CAnmESMathObject::getSQRT2()
{
	return (AnmESVar) m_SQRT2;
}


// methods
AnmESVar CAnmESMathObject::abs(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::fabs(in);
	return v;
}

AnmESVar CAnmESMathObject::acos(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::acos(in);
	return v;
}

AnmESVar CAnmESMathObject::asin(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::asin(in);
	return v;
}

AnmESVar CAnmESMathObject::atan(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::atan(in);
	return v;
}

AnmESVar CAnmESMathObject::ceil(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::ceil(in);
	return v;
}

AnmESVar CAnmESMathObject::cos(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::cos(in);
	return v;
}

AnmESVar CAnmESMathObject::exp(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::exp(in);
	return v;
}

AnmESVar CAnmESMathObject::floor(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::floor(in);
	return v;
}

AnmESVar CAnmESMathObject::log(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::log(in);
	return v;
}

AnmESVar CAnmESMathObject::maxfn(int nArgs, AnmESVar *args)
{
	double result = -DBL_MAX;

	for (int i = 0; i < nArgs; i++)
		if ((double) args[i] > result)
			result = (double) args[i];

	AnmESVar v = (AnmESVar) result;
	return v;
}

AnmESVar CAnmESMathObject::minfn(int nArgs, AnmESVar *args)
{
	double result = DBL_MAX;

	for (int i = 0; i < nArgs; i++)
		if ((double) args[i] < result)
			result = (double) args[i];

	AnmESVar v = (AnmESVar) result;
	return v;
}

AnmESVar CAnmESMathObject::pow(int nArgs, AnmESVar *args)
{
	double in1 = 0.f, in2 = 0.f;

	if (nArgs > 0)
		in1 = args[0];
	else
		;
		// N.B.: runtime warning or error?

	if (nArgs > 1)
		in2 = args[1];
	else
		// N.B.: runtime warning or error?
		;

	AnmESVar v = (AnmESVar) ::pow(in1, in2);
	return v;
}

AnmESVar CAnmESMathObject::random(int nArgs, AnmESVar *args)
{
	double drand = ((double) rand()) / ((double) RAND_MAX);

	AnmESVar v = (AnmESVar) drand;
	return v;
}

AnmESVar CAnmESMathObject::round(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::floor(in + .5f);
	return v;
}
 
AnmESVar CAnmESMathObject::sin(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::sin(in);
	return v;
}

AnmESVar CAnmESMathObject::sqrt(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::sqrt(in);
	return v;
}

AnmESVar CAnmESMathObject::tan(int nArgs, AnmESVar *args)
{
	double in = 0.f;

	if (nArgs > 0)
		in = args[0];
	else
		// N.B.: runtime warning or error?
	;

	AnmESVar v = (AnmESVar) ::tan(in);
	return v;
}


// Tables for Callbacks, Methods and Put/GetProperties
// Math object
DEFINE_ESREADONLYPROPERTY(CAnmESMathObject, E, getE);
DEFINE_ESREADONLYPROPERTY(CAnmESMathObject, LN10, getLN10);
DEFINE_ESREADONLYPROPERTY(CAnmESMathObject, LN2, getLN2);
DEFINE_ESREADONLYPROPERTY(CAnmESMathObject, SQRT1_2, getSQRT1_2);
DEFINE_ESREADONLYPROPERTY(CAnmESMathObject, SQRT2, getSQRT2);

DEFINE_ESMETHOD(CAnmESMathObject, abs);
DEFINE_ESMETHOD(CAnmESMathObject, acos);
DEFINE_ESMETHOD(CAnmESMathObject, asin);
DEFINE_ESMETHOD(CAnmESMathObject, atan);
DEFINE_ESMETHOD(CAnmESMathObject, ceil);
DEFINE_ESMETHOD(CAnmESMathObject, cos);
DEFINE_ESMETHOD(CAnmESMathObject, exp);
DEFINE_ESMETHOD(CAnmESMathObject, floor);
DEFINE_ESMETHOD(CAnmESMathObject, log);
DEFINE_ESMETHOD(CAnmESMathObject, pow);
DEFINE_ESMETHOD(CAnmESMathObject, random);
DEFINE_ESMETHOD(CAnmESMathObject, round);
DEFINE_ESMETHOD(CAnmESMathObject, sin);
DEFINE_ESMETHOD(CAnmESMathObject, sqrt);
DEFINE_ESMETHOD(CAnmESMathObject, tan);


// member table
BEGIN_DEFINE_MEMBERS(CAnmESMathObject)
ESPROPID(CAnmESMathObject, E),
ESPROPID(CAnmESMathObject, LN10),
ESPROPID(CAnmESMathObject, LN2),
ESPROPID(CAnmESMathObject, SQRT1_2),
ESPROPID(CAnmESMathObject, SQRT2),
ESMETHODID(CAnmESMathObject, abs),
ESMETHODID(CAnmESMathObject, acos),
ESMETHODID(CAnmESMathObject, asin),
ESMETHODID(CAnmESMathObject, atan),
ESMETHODID(CAnmESMathObject, ceil),
ESMETHODID(CAnmESMathObject, cos),
ESMETHODID(CAnmESMathObject, exp),
ESMETHODID(CAnmESMathObject, floor),
ESMETHODID(CAnmESMathObject, log),
ESMETHODID(CAnmESMathObject, pow),
ESMETHODID(CAnmESMathObject, random),
ESMETHODID(CAnmESMathObject, round),
ESMETHODID(CAnmESMathObject, sin),
ESMETHODID(CAnmESMathObject, sqrt),
ESMETHODID(CAnmESMathObject, tan),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESMathObject, CAnmESBuiltinObject);

