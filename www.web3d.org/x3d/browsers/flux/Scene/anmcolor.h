/********************************************************************************
 * Flux
 *
 * File: anmcolor.h
 * Description: Color node
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

#ifndef _anmcolor_h
#define _anmcolor_h

#include "anmnode.h"

class CAnmColor : public CAnmNode
{
protected :

	ColorArray					*m_color;

public:

	// constructor/destructor
	CAnmColor();
	virtual ~CAnmColor();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	// New methods

	// Accessors
 	void SetColor(ColorArray *pColors);
	ColorArray *GetColor() { return m_color; }
	void GetColor(ColorArray **pVal)
	{ 
		if (pVal)
			*pVal = m_color;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmColor);

	DECLARE_VALUE(color);

	DECLARE_METHOD(set_color);

	DECLARE_CALLBACK(color_changed);

};

DECLARE_GETIMPLEMENTATION(CAnmColor);

#endif //_anmcolor_h
