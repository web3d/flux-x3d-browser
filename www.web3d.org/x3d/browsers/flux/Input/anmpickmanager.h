/********************************************************************************
 * Flux
 *
 * File: anmpickmanager.h
 * Description: Pick Manager class
 *							manage list of mouse-based pick objects
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

#ifndef _anmpickmanager_h
#define _anmpickmanager_h

#include "anmobject.h"

typedef enum eAnmPickSort {
	eAnmPickSortZ,
	eAnmPickSortY,
} eAnmPickSort ;

template <class _LEAF_, class _GROUP_> struct sAnmPickDesc
{
	_LEAF_				*pLeaf;
	_GROUP_				**pParentArray;

#if 0
	class CAnmNode		*pNode;					// node picked
	class CAnmGroup		**pParentArray;		// path to this node
#endif

	int					 nParents;
	Point3				 hitPoint;				// intersection point in model coords
	Point3				 hitNormal;				// normal from intersected face
	Point2				 hitUV;					// texture coordinate
												// maybe someday we'll have mesh/face IDs etc
};


template <class _LEAF_, class _GROUP_> struct sAnmPickList
{
	std::vector< sAnmPickDesc<_LEAF_,_GROUP_> > pickList;

#if 0
	// insertion sort, front-to-back or top-to-bottom view order
	void Insert(matrix4 &ivmat, sAnmPickDesc<_LEAF_,_GROUP_> pickDesc, eAnmPickSort sortType)
	{
		// pickList.push_back(pickDesc);

		sAnmPickDesc<_LEAF_,_GROUP_> tmp;

		// this just gives us the proper size so the array doesn't go out of bounds
		pickList.push_back(tmp);

		int sz = pickList.size();

		// find the place to insert
		for (int i = 0; i < sz - 1; i++)
		{
			sAnmPickDesc<_LEAF_,_GROUP_> cur = pickList[i];

			Point3 hitp = pickDesc.hitPoint;
			Point3 curp = cur.hitPoint;

			// transform the two points into view space and compare
			hitp = ivmat * hitp;
			curp = ivmat * curp;

			if (sortType == eAnmPickSortZ)
			{
				if (hitp.z < curp.z)
					break;
			}
			else if (sortType == eAnmPickSortY)
			{
				if (hitp.y > curp.y)
					break;
			}
			else
				assert(FALSE);
		}

		// move each of these back one
		for (int j = sz - 1; j > i; --j)
		{
			pickList[j] = pickList[j - 1];
		}

		// now insert it
		pickList[i] = pickDesc;

	}
#else // 0

	// insertion sort, front-to-back or top-to-bottom view order
	void Insert(matrix4 &ivmat, sAnmPickDesc<_LEAF_,_GROUP_> pickDesc, eAnmPickSort sortType)
	{
		int sz = pickList.size();

		if (sz == 0)
		{
			pickList.push_back(pickDesc);
			return;
		}

		assert (sz == 1);

		sAnmPickDesc<_LEAF_,_GROUP_> cur = pickList[0];

		Point3 hitp = pickDesc.hitPoint;
		Point3 curp = cur.hitPoint;

		// transform the two points into view space and compare
		hitp = ivmat * hitp;
		curp = ivmat * curp;

		if (sortType == eAnmPickSortZ)
		{
			if (hitp.z < curp.z)
			{
				pickList[0] = pickDesc;
				Clear1(cur);
			}
		}
		else if (sortType == eAnmPickSortY)
		{
			if (hitp.y > curp.y)
			{
				pickList[0] = pickDesc;
				Clear1(cur);
			}
		}
	}

#endif // 0

	void Append(sAnmPickDesc<_LEAF_,_GROUP_> &pickDesc)
	{
		pickList.push_back(pickDesc);
	}

	int Size()
	{
		return pickList.size();
	}

	void GetItem(int index, sAnmPickDesc<_LEAF_,_GROUP_> &pickDesc)
	{
		int sz = pickList.size();

		if (index >= 0 && index < sz)
			pickDesc = pickList[index];
	}

	void Clear();
	void Clear1(sAnmPickDesc<_LEAF_,_GROUP_> &pickDesc);
};

typedef struct sAnmSceneGraphPickDesc : sAnmPickDesc<class CAnmNode, class CAnmGroup>
{
} sAnmSceneGraphPickDesc ;


typedef struct sAnmSceneGraphPickList : sAnmPickList<class CAnmNode, class CAnmGroup>
{
} sAnmSceneGraphPickList;

typedef struct sAnmRenderGraphPickDesc : sAnmPickDesc<class CAnmDrawable, class CAnmRenderGroup>
{
} sAnmRenderGraphPickDesc ;

typedef struct sAnmRenderGraphPickList : sAnmPickList<class CAnmDrawable, class CAnmRenderGroup>
{
} sAnmRenderGraphPickList;

class CAnmPickManager : public CAnmObject
{
protected:

	class cApplication			*m_app;
	sAnmSceneGraphPickList		 m_picklist;
	class CAnmGroup				*m_pickgroup;
	int							 m_pickindex;
	BOOL						 m_lookedforgroup;
	int							 m_mousex;
	int							 m_mousey;
	Point3						 m_pickfrom;
	Point3						 m_pickto;

	virtual void LookForGroup();

public:

	// constructor/destructor
	CAnmPickManager(class cApplication *pApp);

	virtual ~CAnmPickManager();

	// New methods
	virtual void Init(int mousex, int mousey, Point3 from, Point3 to);

	virtual BOOL TestPick(class CAnmGroup *pGroup, sAnmPickDesc<CAnmNode,CAnmGroup> *pDesc);

	virtual void AddToPickList(sAnmSceneGraphPickDesc pickDesc)
	{
		m_picklist.pickList.push_back(pickDesc);
	}

	// Accessors
	virtual sAnmSceneGraphPickList *GetPickList()
	{
		return &m_picklist;
	}

	virtual void ClearPickList()
	{
		m_picklist.Clear();
	}

	virtual int GetMouseX()
	{
		return m_mousex;
	}

	virtual int GetMouseY()
	{
		return m_mousey;
	}

	virtual void GetPickRay(Point3 *pFrom, Point3 *pTo)
	{
		*pFrom = m_pickfrom;
		*pTo = m_pickto;
	}

};

#endif // _anmpickmanager_h

