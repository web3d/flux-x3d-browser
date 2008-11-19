/********************************************************************************
 * Flux
 *
 * File: anmesnode.h
 * Description: ECMAScript node object
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

#ifndef _anmesnode_h
#define _anmesnode_h

#include "anmesobject.h"

class CAnmESNodeObject : public CAnmESBuiltinObject
{
public :

	// Constructor/destructor
	CAnmESNodeObject(class CAnmSAINode *pNode, class CAnmECMAScriptLanguageBinding *pOwner);
	virtual ~CAnmESNodeObject();

	// Accessors
	class CAnmSAINode *GetNode();

	// CAnmESBuiltinObject overrides
	virtual void PutProperty(char *propName, AnmESVar propValue);
	virtual AnmESVar GetProperty(char *propName);
	virtual AnmESVar CallMethod(char *methodName, int nArgs, AnmESVar *args);
};


#endif // _anmesnode_h
