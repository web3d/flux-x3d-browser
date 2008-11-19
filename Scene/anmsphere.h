/********************************************************************************
 * Flux
 *
 * File: anmsphere.h
 * Description: Sphere node
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

#ifndef _anmsphere_h
#define _anmsphere_h

#include "anmgeometry.h"

#define ANMSPHERE_NCROSSSECTIONS_GOOD 12
#define ANMSPHERE_NSECTIONS_GOOD 24
#define ANMSPHERE_NCROSSSECTIONS_BETTER 16
#define ANMSPHERE_NSECTIONS_BETTER 32
#define ANMSPHERE_NCROSSSECTIONS_BEST 24
#define ANMSPHERE_NSECTIONS_BEST 48

#define ANMSPHERE_DEFAULT_RADIUS 1.f

class CAnmSphere : public CAnmGeometry
{

protected:

	Float				m_radius;				// size of the sphere

	static class CAnmMesh	*m_solidspheremesh;			// we make these only once
	static class CAnmMesh	*m_hollowspheremesh;
	
	static void CreateSphereMesh(class cApplication *pApp, bool solid);

	virtual float GenRadius()	{ return m_radius; }
	virtual	void	GenBoundingVolumes();

public:

	// constructor/destructor
	CAnmSphere();
	virtual ~CAnmSphere();

	// CAnmGeometry overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures
	virtual void GetUnscaledBoundingBox(CAnmBoundingBox *pBoundingBox)
	{
		*pBoundingBox = CAnmBoundingBox( Point3( 0,0,0 ), 
				Point3( 2.0f*ANMSPHERE_DEFAULT_RADIUS, 2.0f*ANMSPHERE_DEFAULT_RADIUS, 2.0f*ANMSPHERE_DEFAULT_RADIUS ) );
	}

	// New methods

	// Accessors
	void SetRadius(Float radius);
	Float GetRadius() { return m_radius; }
	void GetRadius(Float *pVal)
	{
		if (pVal)
			*pVal = m_radius;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmSphere);

	DECLARE_VALUE(radius);
	DECLARE_VALUE(solid);
	
};


#endif // _anmcube_h
