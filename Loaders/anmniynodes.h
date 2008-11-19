/********************************************************************************
 * Flux
 *
 * File: anmniynodes.h
 * Description: Not-implemented node classes
 *						Used to support loading but not realizing
 *						unknown types
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


#include "anmnodedefs.h"
#include "anmsymbol.h"

struct sAnmNIYMember
{
	char *memberName;
	eValueType valueType;
	eAnmClassMemberType memberType;
};

struct sAnmNIYClass
{
	char *className;
	CAnmClassMember *members;
	int nMembers;
};

#define NIYMEMBER(_nm_,_vt_,_mt_) { #_nm_, _vt_, _mt_, NULL, NULL}

#define BEGIN_NIYCLASS(_nm_) \
static CAnmClassMember _nm_##table[] = {

#define END_NIYCLASS \
	};

#define NIYCLASS(_nm_) { #_nm_, _nm_##table, sizeof(_nm_##table) / sizeof(CAnmClassMember)}


class CAnmNIYNodes 
{
protected :

	static sAnmNIYClass m_NIYClasses[];

	static CAnmClass *createClass(sAnmNIYClass *pNIYClass);

public :

	static void InitNIYClassSymbols(CAnmSymbolTable *pClassSymbols);

};
