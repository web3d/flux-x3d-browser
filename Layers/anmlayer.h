/********************************************************************************
 * Flux
 *
 * File: anmlayer.h
 * Description: Layer node - base class for layers
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


#ifndef _anmlayer_h
#define _anmlayer_h

#define ANMLAYER_DEFAULT_TRANSPARENCY						(0.0f)

#include "anmtransform.h"
#include "anmbindablenode.h"

typedef enum eAnmLayerType {
	eAnmLayerForeground,
	eAnmLayerBackground,
	eAnmLayerMain,
} eAnmLayerType ;

#define ANMLAYER_DEFAULT_TYPE		"FOREGROUND"
#define ANMLAYER_DEFAULT_LAYERTYPE	eAnmLayerForeground

class CAnmLayer : public CAnmTransform
{
protected : 

	class CAnmRenderGraph			*m_renderGraph;
	class CAnmCamera				*m_camera;

	int								 m_renderGraphIndex;
	class CAnmPickManager			*m_pickmanager;

	float							 m_transparency;
	String							 m_type;
	eAnmLayerType					 m_layerType;

	class CAnmViewpoint			*m_viewpoint;
	class CAnmBackground		*m_background;
	class CAnmFog				*m_fog;
	class CAnmNavigationInfo	*m_navigationInfo;

	std::list<class CAnmViewpoint *>			
								 m_viewpoints;
	std::vector<CAnmBindingStack>
								 m_bindingstacks;

	// Binding stack handling
	friend class CAnmBackground;
	friend class CAnmFog;
	friend class CAnmNavigationInfo;
	friend class CAnmViewpoint;
	friend class CAnmBindableNode;

	virtual void SetBackground(class CAnmBackground *pBackground);
	virtual void SetFog(class CAnmFog *pFog);
	virtual void SetNavigationInfo(class CAnmNavigationInfo *pNavigationInfo);
	virtual void SetViewpoint(class CAnmViewpoint *pViewpoint);

	virtual void BindNode(class CAnmBindableNode *pBindableNode, class CAnmClass *pClass);
	virtual void TryUnbindNode(class CAnmBindableNode *pBindableNode, class CAnmClass *pClass);
	virtual class CAnmBindableNode *GetBoundNode(class CAnmClass *pClass);
	virtual CAnmBindingStack *AddBindingStack(class CAnmClass *pClass);
	virtual CAnmBindingStack *FindBindingStack(class CAnmClass *pClass);

	virtual class CAnmBackground *GetBoundBackground();
	virtual class CAnmFog *GetBoundFog();
	virtual class CAnmNavigationInfo *GetBoundNavigationInfo();
	virtual class CAnmViewpoint *GetBoundViewpoint();

	virtual void UpdateBoundNode(class CAnmClass *pClass);
	virtual void CAnmLayer::RemoveViewpointOrphans();

	// Helpers
	virtual void RecalculateCameraParams(class CAnmDevice *pDevice) PURE;
	eAnmLayerType CalcLayerType();
	virtual void UpdateBindables();
	void InitPick();

public :

	// Constructor/destructor
	CAnmLayer();
	virtual ~CAnmLayer();

	// CAnmTransform overrides
	virtual void Realize();
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual void PreTraverse();
	virtual void Render(class CAnmDevice *pDevice);
	virtual CAnmLayer *GetLayer()
	{
		return this;
	}

	// New Methods
	virtual void AddViewpoint(class CAnmViewpoint *pViewpoint);
	virtual std::list<class CAnmViewpoint *> *GetViewpoints();

	virtual void BindToViewpoint(class CAnmViewpoint *pViewpoint);

	virtual bool TestCollision(class CAnmDevice *pDevice, Point3 oldpos, Point3 newpos, float radius, Point3 &hitPoint, Point3 &hitNormal);
	virtual bool TestTerrain(class CAnmDevice *pDevice, Point3 oldpos, Point3 newpos, float radius, Point3 &hitPoint);
	virtual bool TestPick(class CAnmDevice *pDevice, int mousex, int mousey, Point3 &hitPoint, class CAnmDrawable **ppDrawable);

	virtual bool TryPick();

	virtual void FreeResources();

	// Accessors
	void SetTransparency(Float transparency);
	Float GetTransparency() { return m_transparency; }
	void GetTransparency(Float *pVal)
	{
		if (pVal)
			*pVal = m_transparency;
	}

	void SetType(String type);
	String GetType() { return m_type; }
	void GetType(String *pVal)
	{
		if (pVal)
			*pVal = m_type;
	}

	eAnmLayerType GetLayerType()
	{
		return m_layerType;
	}

	virtual class CAnmCamera *GetCamera()
	{
		return m_camera;
	}
 
	virtual class CAnmPickManager *GetPickManager()
	{
		return m_pickmanager;
	}

	virtual bool IsMainLayer()
	{
		return false;
	}

	virtual class CAnmBackground *GetBackground()
	{
		return m_background;
	}

	virtual class CAnmFog *GetFog()
	{
		return m_fog;
	}

	virtual class CAnmNavigationInfo *GetNavigationInfo()
	{
		return m_navigationInfo;
	}

	virtual class CAnmViewpoint *GetViewpoint()
	{
		return m_viewpoint;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmLayer);

	DECLARE_VALUE(children);
	DECLARE_VALUE(transparency);
	DECLARE_VALUE(type);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_transparency);

	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(transparency_changed);
};

#endif // _anmlayer_h

/**

SFFloat   transparency    [0.0]

***/
