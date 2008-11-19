/********************************************************************************
 * Flux
 *
 * File: anmshader.h
 * Description: Container class for Flux Programmable Shaders
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

#ifndef _anmshader_h
#define _anmshader_h

#include "anmgroup.h"

class CAnmShader : public CAnmGroup
{

protected:


public:

	// constructor/destructor
	CAnmShader() 
	{
	}

	virtual ~CAnmShader()
	{
	}

	virtual void SetToDevice(class CAnmDevice *pDevice) PURE;
};


DECLARE_GETIMPLEMENTATION(CAnmShader);

#endif // _anmshader_h
