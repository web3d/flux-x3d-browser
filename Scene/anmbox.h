/********************************************************************************
 * Flux
 *
 * File: anmbox.h
 * Description: Box node
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

#ifndef _anmbox_h
#define _anmbox_h

#include "anmgeometry.h"

#define ANMBOX_DEFAULT_SIZE Point3(2, 2, 2)

class CAnmBox : public CAnmGeometry
{

protected:

	Point3					 m_size;				// size of the box

	static class CAnmMesh	*m_solidcubemesh;			// we make these only once
	static class CAnmMesh	*m_hollowcubemesh;
	
	void CreateCubeMesh(bool solid);
	virtual float GenRadius();
	virtual	void	GenBoundingVolumes();

public:


	// constructor/destructor
	CAnmBox();
	virtual ~CAnmBox();

	// CAnmGeometry overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual void GetUnscaledBoundingBox(CAnmBoundingBox *pBoundingBox)
	{
		*pBoundingBox = CAnmBoundingBox( Point3( 0,0,0 ), ANMBOX_DEFAULT_SIZE );
	}


	// New methods

	// Accessors
	void SetSize(Point3 size);
	Point3 GetSize() { return m_size; }
	void GetSize(Point3 *pVal)
	{
		if (pVal)
			*pVal = m_size;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmBox);

	DECLARE_VALUE(size);
	DECLARE_VALUE(solid);	
};


#endif // _anmcube_h
