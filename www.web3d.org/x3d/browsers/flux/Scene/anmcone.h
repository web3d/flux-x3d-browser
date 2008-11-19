/********************************************************************************
 * Flux
 *
 * File: anmcone.h
 * Description: Cone node
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

#ifndef _anmcone_h
#define _anmcone_h

#include "anmgeometry.h"

#define ANMCONE_NSIDES_GOOD 18
#define ANMCONE_NSIDES_BETTER 27
#define ANMCONE_NSIDES_BEST 36


#define ANMCONE_DEFAULT_BOTTOMRADIUS 1.f
#define ANMCONE_DEFAULT_HEIGHT 2.f
#define ANMCONE_DEFAULT_SIDE TRUE
#define ANMCONE_DEFAULT_BOTTOM TRUE

class CAnmCone : public CAnmGeometry
{

protected:

	Float					 m_bottomradius;		// cone radius & height
	Float					 m_height;				

	Boolean					 m_bottom;				// flags for bottom, sides, top
	Boolean					 m_side;

	void CreateConeMesh();
	virtual float GenRadius();
	virtual	void	GenBoundingVolumes();

public:

	// constructor/destructor
	CAnmCone();
	virtual ~CAnmCone();

	// CAnmGeometry overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures
	virtual void GetUnscaledBoundingBox(CAnmBoundingBox *pBoundingBox)
	{
		*pBoundingBox = CAnmBoundingBox( Point3( 0,0,0 ), 
				Point3( ANMCONE_DEFAULT_HEIGHT, ANMCONE_DEFAULT_HEIGHT, ANMCONE_DEFAULT_HEIGHT ) );
	}

	// New methods

	// Accessors
	void SetBottomRadius(Float bottomRadius);
	Float GetBottomRadius() { return m_bottomradius; }
	void GetBottomRadius(Float *pVal)
	{
		if (pVal)
			*pVal = m_bottomradius;
	}

	void SetHeight(Float height);
	Float GetHeight() { return m_height; }
	void GetHeight(Float *pVal)
	{
		if (pVal)
			*pVal = m_height;
	}

	void SetBottom(Boolean bottom);
	Boolean GetBottom() { return m_bottom; }
	void GetBottom(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_bottom;
	}

	void SetSide(Boolean side);
	Boolean GetSide() { return m_side; }
	void GetSide(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_side;
	}
	
	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmCone);

	DECLARE_VALUE(height);
	DECLARE_VALUE(bottomRadius);
	DECLARE_VALUE(bottom);
	DECLARE_VALUE(side);
	DECLARE_VALUE(solid);

};


#endif // _anmcone_h
