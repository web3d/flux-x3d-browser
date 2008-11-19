/********************************************************************************
 * Flux
 *
 * File: anmlayer3D.h
 * Description: Layer3D node - grouping node for 3D layer
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


#ifndef _anmlayer3D_h
#define _anmlayer3D_h

#include "anmlayer.h"

#define ANMLAYER3D_DEFAULT_AUTOORIENT			false

class CAnmLayer3D : public CAnmLayer
{
protected : 

	Boolean				m_autoOrient;

	// helpers
	virtual void RecalculateCameraParams(class CAnmDevice *pDevice);
	virtual void CalcClippingPlanes(double *pNear, double *pFar, int nzbufferbytes);

public :

	// Constructor/destructor
	CAnmLayer3D();
	virtual ~CAnmLayer3D();

	// CAnmLayer overrides
	virtual void Realize();

	// New methods

	// Accessors
	void SetAutoOrient(Boolean autoOrient);
	Boolean GetAutoOrient() { return m_autoOrient; }
	void GetAutoOrient(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_autoOrient;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmLayer3D);

	DECLARE_VALUE(children);
	DECLARE_VALUE(autoOrient);
	DECLARE_VALUE(transparency);
	DECLARE_VALUE(type);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_autoOrient);
	DECLARE_METHOD(set_transparency);

	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(autoOrient_changed);
	DECLARE_CALLBACK(transparency_changed);
};

#endif // _anmlayer3D_h
