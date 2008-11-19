/********************************************************************************
 * Flux
 *
 * File: anmshape.h
 * Description: Shape node
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

#ifndef _anmshape_h
#define _anmshape_h

#include "anmgroup.h"
#include "anmappearance.h"
#include "anmgeometry.h"

class CAnmShape : public CAnmGroup
{

protected:

	CAnmNode						*m_geometry;
	CAnmNode						*m_appearance;

	class CAnmDrawable *NextDrawable(CAnmNodePath &p)
	{
		CAnmDrawable *pDrawable;
		return NextInstance(pDrawable, p);
	}

	class CAnmDrawable *GetDrawable(int index)
	{
		return (class CAnmDrawable *) GetInstance(index);
	}

	class CAnmDrawable *CurrentDrawable()
	{
		return (class CAnmDrawable *) CurrentInstance();
	}

	class CAnmDrawable *FindDrawable(CAnmNodePath &p)
	{
		return (class CAnmDrawable *) FindInstance(p);
	}

	

	virtual void CopyToInstances();
	virtual void CopyValuesToInstance(class CAnmDrawable *pDrawable);

public:

	// constructor/destructor
	CAnmShape();
	virtual ~CAnmShape();

	// CAnmGroup overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void GetBoundingBox(CAnmBoundingBox *pBoundingBox);
	virtual void GetObjectCenter(Point3 *pCenter);

	virtual eAnmReturnStatus AddChild(CAnmNode *pChild);
	virtual eAnmReturnStatus RemoveChild(CAnmNode *pChild);

	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual void PreTraverse();

	// new methods

	// Accessors

	void SetGeometry(CAnmNode *pGeometry);
	class CAnmNode *GetGeometry()
	{
		return m_geometry;
	}
	void GetGeometry(CAnmNode **pG)
	{
		assert(pG);
		*pG = m_geometry;
	}

	void SetAppearance(CAnmNode *pAppearance);
	class CAnmNode *GetAppearance()
	{
		return m_appearance;
	}
	void GetAppearance(CAnmNode **pA)
	{
		assert(pA);
		*pA = m_appearance;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmShape);

	DECLARE_VALUE(geometry);
	DECLARE_VALUE(appearance);

	DECLARE_METHOD(set_geometry);
	DECLARE_METHOD(set_appearance);

	DECLARE_CALLBACK(geometry_changed);
	DECLARE_CALLBACK(appearance_changed);
};


#endif // _anmshape_h
