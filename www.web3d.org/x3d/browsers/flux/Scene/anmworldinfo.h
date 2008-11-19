/********************************************************************************
 * Flux
 *
 * File: anmworldinfo.h
 * Description: WorldInfo node
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
 
#ifndef _anmworldinfo_h
#define _anmworldinfo_h

#include "anmnode.h"

#define ANMWORLDINFO_DEFAULT_TITLE				""

class CAnmWorldInfo : public CAnmNode
{
protected :

	StringArray					*m_info;
	String						 m_title;

public:

	// constructor/destructor
	CAnmWorldInfo();
	virtual ~CAnmWorldInfo();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	// New methods

	// Accessors
 	void SetInfo(StringArray *pInfo);
	StringArray *GetInfo() { return m_info; }
	void GetInfo(StringArray **pVal)
	{ 
		if (pVal)
			*pVal = m_info;
	}

 	void SetTitle(String title);
	String GetTitle() { return m_title; }
	void GetTitle(String *pVal)
	{ 
		if (pVal)
			*pVal = m_title;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmWorldInfo);

	DECLARE_VALUE(info);
	DECLARE_VALUE(title);

};

#endif //_anmworldinfo_h
