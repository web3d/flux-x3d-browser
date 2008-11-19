/********************************************************************************
 * Flux
 *
 * File: anmcylinder.h
 * Description: Cylinder node
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

#ifndef _anmcylinder_h
#define _anmcylinder_h

#include "anmgeometry.h"

#define ANMCYLINDER_NUMSIDES_GOOD 18
#define ANMCYLINDER_NUMSIDES_BETTER 27
#define ANMCYLINDER_NUMSIDES_BEST 36


#define ANMCYLINDER_DEFAULT_BOTTOM TRUE
#define ANMCYLINDER_DEFAULT_HEIGHT 2.f
#define ANMCYLINDER_DEFAULT_RADIUS 1.f
#define ANMCYLINDER_DEFAULT_SIDE TRUE
#define ANMCYLINDER_DEFAULT_TOP TRUE

class CAnmCylinder : public CAnmGeometry
{

protected:

	Float				m_height;				// cylinder radius & height
	Float				m_radius;
	Boolean				m_bottom;				// flags for bottom, sides, top
	Boolean				m_side;
	Boolean				m_top;


	void CreateCylinderMesh();
	virtual float GenRadius();
	virtual	void	GenBoundingVolumes();

public:

	// constructor/destructor
	CAnmCylinder();
	virtual ~CAnmCylinder();

	// CAnmGeometry overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures
	virtual void GetUnscaledBoundingBox(CAnmBoundingBox *pBoundingBox)
	{
		*pBoundingBox = CAnmBoundingBox( Point3( 0,0,0 ), 
				Point3( ANMCYLINDER_DEFAULT_HEIGHT, ANMCYLINDER_DEFAULT_HEIGHT, ANMCYLINDER_DEFAULT_HEIGHT ) );
	}

	// New methods

	// Accessors
	void SetBottom(Boolean bottom);
	Boolean GetBottom() { return m_bottom; }
	void GetBottom(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_bottom;
	}

	void SetHeight(Float height);
	Float GetHeight() { return m_height; }
	void GetHeight(Float *pVal)
	{
		if (pVal)
			*pVal = m_height;
	}

	void SetRadius(Float radius);
	Float GetRadius() { return m_radius; }
	void GetRadius(Float *pVal)
	{
		if (pVal)
			*pVal = m_radius;
	}

	void SetSide(Boolean side);
	Boolean GetSide() { return m_side; }
	void GetSide(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_side;
	}

	void SetTop(Boolean top);
	Boolean GetTop() { return m_top; }
	void GetTop(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_top;
	}
	
	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmCylinder);

	DECLARE_VALUE(bottom);
	DECLARE_VALUE(height);
	DECLARE_VALUE(radius);
	DECLARE_VALUE(side);
	DECLARE_VALUE(solid);
	DECLARE_VALUE(top);

};


#endif // _anmcylinder_h
