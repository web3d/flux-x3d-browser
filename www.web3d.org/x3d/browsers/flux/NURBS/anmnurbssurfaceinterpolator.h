/********************************************************************************
 * Flux
 *
 * File: anmnurbssurfaceinterpolator.h
 * Description: NurbsSurfaceInterpolator node
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

#ifndef _anmnurbssurfaceinterpolator_h
#define _anmnurbssurfaceinterpolator_h

#include "anmgroup.h"
#include "anmnurbssurfaceevaluator.h"

#define ANMNURBSSURFACEINTERPOLATOR_DEFAULT_UDIMENSION			0
#define ANMNURBSSURFACEINTERPOLATOR_DEFAULT_UORDER				3
#define ANMNURBSSURFACEINTERPOLATOR_DEFAULT_VDIMENSION			0
#define ANMNURBSSURFACEINTERPOLATOR_DEFAULT_VORDER				3

class CAnmNurbsSurfaceInterpolator : public CAnmGroup
{

protected:

	CAnmNode					*m_controlPoint;
	DoubleArray					*m_weight;
	Integer						 m_uDimension;
	DoubleArray					*m_uKnot;
	Integer						 m_uOrder;
	Integer						 m_vDimension;
	DoubleArray					*m_vKnot;
	Integer						 m_vOrder;

	CAnmNurbsSurfaceEvaluator	 m_evaluator;

public:

	// constructor/destructor
	CAnmNurbsSurfaceInterpolator();
	virtual ~CAnmNurbsSurfaceInterpolator();

	// CAnmGroup overrides
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

	// input-only
	void SetFraction(Point2 fraction);

	void SetWeight(DoubleArray *pWeight);
	DoubleArray *GetWeight() { return m_weight; }
	void GetWeight(DoubleArray **pVal)
	{
		assert(pVal);
		*pVal = m_weight;
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
	DECLARE_NODECLASS(CAnmNurbsSurfaceInterpolator);

	DECLARE_VALUE(controlPoint);
	DECLARE_VALUE(weight);
	DECLARE_VALUE(uDimension);
	DECLARE_VALUE(uKnot);
	DECLARE_VALUE(uOrder);
	DECLARE_VALUE(vDimension);
	DECLARE_VALUE(vKnot);
	DECLARE_VALUE(vOrder);
	
	DECLARE_METHOD(set_controlPoint);
	DECLARE_METHOD(set_fraction);
	DECLARE_METHOD(set_weight);

	DECLARE_CALLBACK(controlPoint_changed);
	DECLARE_CALLBACK(position_changed);
	DECLARE_CALLBACK(normal_changed);
	DECLARE_CALLBACK(weight_changed);
};


#endif // _anmnurbssurfaceinterpolator_h
