/********************************************************************************
 * Flux
 *
 * File: anmesmatrix.h
 * Description: ECMAScript matrix objects
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

#ifndef _anmesmatrix_h
#define _anmesmatrix_h

#include "anmesobject.h"

class CAnmESMatrix4 : public CAnmESBuiltinObject
{
protected :

public :

	// Constructor/destructor
	CAnmESMatrix4(class CAnmSAIMatrix4 *pMatrix, class CAnmECMAScriptLanguageBinding *pOwner);
	virtual ~CAnmESMatrix4();
	static CAnmESObject *Create(class CAnmECMAScriptLanguageBinding *pOwner, int nargs, AnmESVar *args);

	// New Methods
	AnmESVar setTransform(int nArgs, AnmESVar *args);
	AnmESVar getTransform(int nArgs, AnmESVar *args);
	AnmESVar inverse(int nArgs, AnmESVar *args);
	AnmESVar transpose(int nArgs, AnmESVar *args);
	AnmESVar multLeft(int nArgs, AnmESVar *args);
	AnmESVar multRight(int nArgs, AnmESVar *args);
	AnmESVar multVecMatrix(int nArgs, AnmESVar *args);
	AnmESVar multMatrixVec(int nArgs, AnmESVar *args);
	AnmESVar toString(int nArgs, AnmESVar *args);

	// CAnmESBuiltinObject overrides
	virtual void PutArrayProperty(AnmESVar index, AnmESVar propValue);
	virtual AnmESVar GetArrayProperty(AnmESVar vindex);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESMatrix4);

	DECLARE_ESMETHOD(setTransform);
	DECLARE_ESMETHOD(getTransform);
	DECLARE_ESMETHOD(inverse);
	DECLARE_ESMETHOD(transpose);
	DECLARE_ESMETHOD(multLeft);
	DECLARE_ESMETHOD(multRight);
	DECLARE_ESMETHOD(multVecMatrix);
	DECLARE_ESMETHOD(multMatrixVec);
	DECLARE_ESMETHOD(toString);
};

#endif // _anmesmatrix_h
