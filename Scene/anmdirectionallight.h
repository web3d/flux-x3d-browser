/********************************************************************************
 * Flux
 *
 * File: anmdirectionallight.h
 * Description: Directional light class
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

#ifndef _anmdirectionallight_h
#define _anmdirectionallight_h

#include "anmlight.h"


class CAnmDirectionalLight : public CAnmLight
{

protected:

public:


	// Constructor/Destructor
	CAnmDirectionalLight();
	virtual ~CAnmDirectionalLight();

	// Node class overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	// New methods

	// Accessors

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmDirectionalLight);

	DECLARE_VALUE(ambientIntensity);
	DECLARE_VALUE(color);
	DECLARE_VALUE(direction);
	DECLARE_VALUE(intensity);
	DECLARE_VALUE(on);

	DECLARE_METHOD(set_ambientIntensity);
	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_direction);
	DECLARE_METHOD(set_intensity);
	DECLARE_METHOD(set_on);

	DECLARE_CALLBACK(ambientIntensity_changed);
	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(direction_changed);
	DECLARE_CALLBACK(intensity_changed);
	DECLARE_CALLBACK(on_changed);

};

DECLARE_GETIMPLEMENTATION(CAnmDirectionalLight);

#endif // _anmdirectionallight_h
