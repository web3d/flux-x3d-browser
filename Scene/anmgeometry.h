/********************************************************************************
 * Flux
 *
 * File: anmgeometry.h
 * Description: Geometry node base class
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

#ifndef _anmgeometry_h
#define _anmgeometry_h

#define ANMGEOMETRY_DEFAULT_SOLID					TRUE

#include "anmgroup.h"

class CAnmGeometry : public CAnmGroup
{

protected:
	
	Boolean					 m_solid;				// solid shape hint
	class CAnmMesh			*m_mesh;
	Point3					 m_modelscale;				// for scaling objects
	long					 m_polycount;			// for fun

	virtual	void	GenBoundingVolumes();

public:

	// constructor/destructor
	CAnmGeometry();
	virtual ~CAnmGeometry();

	// CAnmGroup overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);

	virtual void GetUnscaledBoundingBox(CAnmBoundingBox *pBoundingBox)
	{
		*pBoundingBox = m_bbox;
	}

	virtual void GetBoundingBox(CAnmBoundingBox *pBoundingBox)
	{
		*pBoundingBox = m_bbox;
	}

	virtual float GetRadius()
	{
		return m_bbox.GetRadius();
	}

	// New methods
	virtual void Render(class CAnmDevice *pDevice, int texturestage, class CAnmCamera *pCamera,
						  const matrix4 &worldmat);

	virtual void SetModelScale(Point3 scale);
	virtual Point3 GetModelScale()
	{
		return m_modelscale;
	}

	virtual eAnmRenderOptions GetRenderOptions()
	{
		return eAnmRenderOptionsNone;
	}

	virtual class CAnmNode *GetTextureCoordinate()
	{
		return NULL;
	}

	// Accessors
	void SetSolid(Boolean solid);
	Boolean	GetSolid() { return m_solid; }
	void GetSolid(Boolean *pVal)
	{ 
		if (pVal)
			*pVal = m_solid;
	}
	
	
	virtual CAnmMesh *GetMesh()
	{
		return m_mesh;
	}

	virtual void GetObjectCenter(Point3 *pCenter)
	{
		Point3 size;
		m_bbox.GetCenterSize(pCenter, &size);
	}

	virtual long GetPolyCount()
	{
		return m_polycount;
	}

	virtual bool HasAlpha();

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmGeometry);

	DECLARE_VALUE(solid);
	
};

DECLARE_GETIMPLEMENTATION(CAnmGeometry);

#define ANMGEOMETRY_SOLIDDIRTY					ANMNODE_NDIRTYBITS
#define ANMGEOMETRY_NDIRTYBITS					(ANMGEOMETRY_SOLIDDIRTY + 1)

typedef enum eAnmGeometryDirtyFlags {
eAnmGeometrySolidDirty				= ANMDIRTYBIT(ANMGEOMETRY_SOLIDDIRTY),
} eAnmGeometryDirtyFlags;

inline BOOL Scaling(Point3 sc)
{
	return (sc.x != 1.f) || (sc.y != 1.f) || (sc.z != 1.f);
}


#endif // _anmgeometry_h
