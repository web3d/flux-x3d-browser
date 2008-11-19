/********************************************************************************
 * Flux
 *
 * File: anmnormal.h
 * Description: Normal node
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

#ifndef _anmnormal_h
#define _anmnormal_h

#include "anmnode.h"

class CAnmNormal : public CAnmNode
{
protected :

	Point3Array					*m_vector;


public:

	// constructor/destructor
	CAnmNormal();
	virtual ~CAnmNormal();

	// CAnmNode overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	void SetVector(Point3Array *pVector);
	Point3Array	*GetVector() { return m_vector; }
	void GetVector(Point3Array **pVal)
	{ 
		if (pVal)
			*pVal = m_vector;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmNormal);

	DECLARE_CALLBACK(vector_changed);

	DECLARE_VALUE(vector);

	DECLARE_METHOD(set_vector);

};

DECLARE_GETIMPLEMENTATION(CAnmNormal);

#endif _anmnormal_h
