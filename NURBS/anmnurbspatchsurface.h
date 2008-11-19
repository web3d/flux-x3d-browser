/********************************************************************************
 * Flux
 *
 * File: anmnurbspatchsurface.h
 * Description: NurbsPatchSurface node
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

#ifndef _anmnurbspatchsurface_h
#define _anmnurbspatchsurface_h

#include "anmgeometry.h"

#include "anmnurbssurfaceevaluator.h"


#define ANMNURBSPATCHSURFACE_DEFAULT_UTESSELLATION		0
#define ANMNURBSPATCHSURFACE_DEFAULT_VTESSELLATION		0
#define ANMNURBSPATCHSURFACE_DEFAULT_SOLID				TRUE
#define ANMNURBSPATCHSURFACE_DEFAULT_UCLOSED			FALSE
#define ANMNURBSPATCHSURFACE_DEFAULT_UDIMENSION			0
#define ANMNURBSPATCHSURFACE_DEFAULT_UORDER				3
#define ANMNURBSPATCHSURFACE_DEFAULT_VCLOSED			FALSE
#define ANMNURBSPATCHSURFACE_DEFAULT_VDIMENSION			0
#define ANMNURBSPATCHSURFACE_DEFAULT_VORDER				3

class CAnmNurbsPatchSurface : public CAnmGeometry
{

protected:

	CAnmNode					*m_controlPoint;
	CAnmNode					*m_texCoord;
	Integer						 m_uTessellation;
	Integer						 m_vTessellation;
	DoubleArray					*m_weight;
	Boolean						 m_uClosed;
	Integer						 m_uDimension;
	DoubleArray					*m_uKnot;
	Integer						 m_uOrder;
	Boolean						 m_vClosed;
	Integer						 m_vDimension;
	DoubleArray					*m_vKnot;
	Integer						 m_vOrder;


	
// Data for Closed Nurbs Surface
//	
	Point3Array					*m_ClosedControlPointArray;
	Point2Array					*m_ClosedTexCoordArray;
	DoubleArray					*m_ClosedWeight;
	Boolean						 m_uUseClosed;
	Boolean						 m_vUseClosed;
	Integer						 m_uClosedDimension;
	DoubleArray					*m_uClosedKnot;
	Integer						 m_vClosedDimension;
	DoubleArray					*m_vClosedKnot;
	
	
	double 						m_uKnotStart;
	double 						m_uKnotEnd;
	double 						m_vKnotStart;
	double 						m_vKnotEnd;


	CAnmNurbsSurfaceEvaluator	 m_evaluator;

	virtual void CreateMesh();

	bool IsMeshClosed( bool& buClosed, bool& bvClosed );
	bool CreateClosedMesh( bool buClosed, bool bvClosed );
	bool CreateOpenMesh();
	bool GenerateClosedArrays( bool buClosed, bool bvClosed );

public:

	// constructor/destructor
	CAnmNurbsPatchSurface();
	virtual ~CAnmNurbsPatchSurface();

	// CAnmGeometry overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures
	virtual eAnmReturnStatus AddChild(CAnmNode *pChild);
	virtual eAnmReturnStatus RemoveChild(CAnmNode *pChild);
	virtual void ChildChanged(CAnmNode *pChild);

	// New methods

	// Accessors
	void SetControlPoint(CAnmNode *pControlPoint);
	CAnmNode *GetControlPoint()
	{
		return m_controlPoint;
	}
	void GetControlPoint(CAnmNode **ppCpt)
	{
		assert(ppCpt);
		*ppCpt = m_controlPoint;
	}

	void SetTexCoord(CAnmNode *pTexCoord);
	class CAnmNode *GetTexCoord() { return m_texCoord; }
	void GetTexCoord(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_texCoord;
	}

	void SetUTessellation(Integer uTessellation);
	Integer GetUTessellation() { return m_uTessellation; }
	void GetUTessellation(Integer *pVal)
	{ 
		if (pVal)
			*pVal = m_uTessellation;
	}

	void SetVTessellation(Integer vTessellation);
	Integer GetVTessellation() { return m_vTessellation; }
	void GetVTessellation(Integer *pVal)
	{ 
		if (pVal)
			*pVal = m_vTessellation;
	}

	void SetWeight(DoubleArray *pWeight);
	DoubleArray *GetWeight() { return m_weight; }
	void GetWeight(DoubleArray **pVal)
	{
		assert(pVal);
		*pVal = m_weight;
	}

	void SetUClosed(Boolean uClosed);
	Boolean	GetUClosed() { return m_uClosed; }
	void GetUClosed(Boolean *pVal)
	{ 
		if (pVal)
			*pVal = m_uClosed;
	}
	
	void SetUDimension(Integer uDimension);
	Integer GetUDimension() { return m_uDimension; }
	void GetUDimension(Integer *pVal)
	{ 
		if (pVal)
			*pVal = m_uDimension;
	}

	void SetUKnot(DoubleArray *pUknot);
	DoubleArray *GetUKnot() { return m_uKnot; }
	void GetUKnot(DoubleArray **pVal)
	{
		assert(pVal);
		*pVal = m_uKnot;
	}
	
	void SetUOrder(Integer uOrder);
	Integer GetUOrder() { return m_uOrder; }
	void GetUOrder(Integer *pVal)
	{ 
		if (pVal)
			*pVal = m_uOrder;
	}

	void SetVClosed(Boolean vClosed);
	Boolean	GetVClosed() { return m_vClosed; }
	void GetVClosed(Boolean *pVal)
	{ 
		if (pVal)
			*pVal = m_vClosed;
	}
	
	void SetVDimension(Integer vDimension);
	Integer GetVDimension() { return m_vDimension; }
	void GetVDimension(Integer *pVal)
	{ 
		if (pVal)
			*pVal = m_vDimension;
	}

	void SetVKnot(DoubleArray *pVknot);
	DoubleArray *GetVKnot() { return m_vKnot; }
	void GetVKnot(DoubleArray **pVal)
	{
		assert(pVal);
		*pVal = m_vKnot;
	}
	
	void SetVOrder(Integer vOrder);
	Integer GetVOrder() { return m_vOrder; }
	void GetVOrder(Integer *pVal)
	{ 
		if (pVal)
			*pVal = m_vOrder;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmNurbsPatchSurface);

	DECLARE_VALUE(controlPoint);
	DECLARE_VALUE(texCoord);	
	DECLARE_VALUE(uTessellation);
	DECLARE_VALUE(vTessellation);
	DECLARE_VALUE(weight);
	DECLARE_VALUE(solid);
	DECLARE_VALUE(uClosed);
	DECLARE_VALUE(uDimension);
	DECLARE_VALUE(uKnot);
	DECLARE_VALUE(uOrder);
	DECLARE_VALUE(vClosed);
	DECLARE_VALUE(vDimension);
	DECLARE_VALUE(vKnot);
	DECLARE_VALUE(vOrder);
	
	DECLARE_METHOD(set_controlPoint);
	DECLARE_METHOD(set_texCoord);
	DECLARE_METHOD(set_uTessellation);
	DECLARE_METHOD(set_vTessellation);
	DECLARE_METHOD(set_weight);

	DECLARE_CALLBACK(controlPoint_changed);
	DECLARE_CALLBACK(texCoord_changed);
	DECLARE_CALLBACK(uTessellation_changed);
	DECLARE_CALLBACK(vTessellation_changed);
	DECLARE_CALLBACK(weight_changed);
};


#endif // _anmnurbspatchsurface_h
