/********************************************************************************
 * Flux
 *
 * File: anmrenderobject.h
 * Description: Render graph classes
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

#ifndef _anmrenderobject_h
#define _anmrenderobject_h

#include "anmpickmanager.h"
#include "anmvolumes.h"

class CAnmRenderObject
{

protected:

	class CAnmRenderGroup			*m_parent;
	CAnmBoundingBox					 m_bbox;
	bool							 m_collide;
	void							*m_userdata;

	friend class CAnmRenderGroup;

	virtual void SetParent(class CAnmRenderGroup *pParent)
	{
		m_parent = pParent;
	}

	virtual void handleAdded(class CAnmRenderGroup *pParent)
	{
		SetParent(pParent);
	}

	virtual void handleRemoved()
	{
		SetParent(NULL);
	}

	virtual int NumParents();
	virtual void GetTopDownPath(CAnmRenderGroup **pParents, int nParents);

	virtual void InvalidateBBox();
	virtual void DrawBBox(class CAnmDevice *pDevice);

public:

	// Constructor/destructor
	CAnmRenderObject()
	{
		m_parent = NULL;
	}

	virtual ~CAnmRenderObject()
	{
	}

	// Bounding box support
	virtual void SetBoundingBox(CAnmBoundingBox *pBoundingBox);

	virtual void GetBoundingBox(CAnmBoundingBox *pBoundingBox);

	// Culling support
	virtual void Cull(class CAnmFrustum *pFrustum, 
		std::vector<class CAnmDrawable *>	*pRenderList);

	// Picking support
	virtual void TestPick(matrix4 &ivmat, Point3 from, Point3 to, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void SphereCollide(matrix4 &ivmat, Point3 center, float radius, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void CapsuleCollide(matrix4 &ivmat, const Point3 &capsulePt0, const Point3 &capsulePt1, 
		float capsuleRadius, eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	
	// Hierarchy stuff
	virtual class CAnmRenderGraph *GetRoot();

	// Accessors
	virtual void SetUserData(void *userdata)
	{
		m_userdata = userdata;
	}

	virtual void *GetUserData()
	{
		return m_userdata;
	}

	virtual void SetCollide(bool collide)
	{
		m_collide = collide;
	}

	virtual bool GetCollide()
	{
		return m_collide;
	}
};

// StateSet class: collects material, texture, light, shader, transform
// and render options into set of states
class CAnmStateSet
{

protected :
	matrix4								 m_worldmatrix;
	CAnmGraphics::Material				*m_material;
	class CAnmShader					*m_shader;
	class CAnmTexture					*m_texture;
	class CAnmMultiTextureCoordinate	*m_textureCoordinate;
	class CAnmTextureTransform			*m_textureTransform;
	class CAnmLightGroup				*m_lightGroup;
	eAnmRenderOptions					 m_renderOptions;
	bool								 m_alphablending;
	matrix4								*m_screenMatrix;
	matrix4								*m_combinedScreenMatrix;
	

	void UpdateScreenGroupMatrix(class CAnmDevice *pDevice, class CAnmCamera *pCamera);


public :

	CAnmStateSet();
	virtual ~CAnmStateSet();

	// Set our state into the device (unconditionally)
	virtual void SetState(class CAnmDevice *pDevice, class CAnmCamera *pCamera);

	// Transition from one graphics state to another
	virtual void ChangeState(class CAnmDevice *pDevice, class CAnmCamera *pCamera, CAnmStateSet *prevstate);

	// Accessors
	virtual void SetWorldMatrix(matrix4 *pMat);
	virtual void GetWorldMatrix(matrix4 *pMat)
	{
		*pMat = m_worldmatrix;
	}

	virtual void SetMaterial(CAnmGraphics::Material *pMaterial);
	virtual CAnmGraphics::Material *GetMaterial()
	{
		return m_material;
	}

	virtual void SetShader(class CAnmShader *pShader);
	virtual class CAnmShader *GetShader()
	{
		return m_shader;
	}

	virtual void SetTexture(class CAnmTexture *pTexture);
	virtual class CAnmTexture *GetTexture()
	{
		return m_texture;
	}

	virtual void SetTextureCoordinate(class CAnmMultiTextureCoordinate *pTextureCoordinate);
	virtual class CAnmMultiTextureCoordinate *GetTextureCoordinate()
	{
		return m_textureCoordinate;
	}

	virtual void SetTextureTransform(class CAnmTextureTransform *pTextureTransform);
	virtual class CAnmTextureTransform *GetTextureTransform()
	{
		return m_textureTransform;
	}

	virtual void SetLightGroup(class CAnmLightGroup *pLightGroup);
	virtual class CAnmLightGroup *GetLightGroup()
	{
		return m_lightGroup;
	}

	virtual void SetRenderOptions(eAnmRenderOptions renderOptions);
	virtual eAnmRenderOptions GetRenderOptions()
	{
		return m_renderOptions;
	}

	virtual void SetAlphaBlending(bool alphablending);
	virtual bool GetAlphaBlending()
	{
		return m_alphablending;
	}

	virtual void SetScreenMatrix(matrix4 *pMat);
	virtual matrix4 *GetScreenMatrix()
	{
		return m_screenMatrix;
	}

};

// Drawable : leaf in the render graph; contains geometry and state for one draw object
class CAnmDrawable : public CAnmRenderObject, public CAnmStateSet
{
protected :

	class CAnmGeometry				*m_geometry;

	// CAnmStateSet overrides
	void UpdateScreenGroupMatrix(class CAnmDevice *pDevice, class CAnmCamera *pCamera);

	friend class CAnmRenderGraph;

public :

	CAnmDrawable();
	virtual ~CAnmDrawable();

	// CAnmRenderObject overrides
	virtual void GetBoundingBox(CAnmBoundingBox *pBoundingBox);
	virtual void TestPick(matrix4 &ivmat, Point3 from, Point3 to, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void SphereCollide(matrix4 &ivmat, Point3 center, float radius, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void CapsuleCollide(matrix4 &ivmat, const Point3 &capsulePt0, const Point3 &capsulePt1, 
		float capsuleRadius, eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void Cull(class CAnmFrustum *pFrustum, 
		std::vector<CAnmDrawable *>	*pRenderList);
	virtual void handleAdded(class CAnmRenderGroup *pParent);
	virtual void handleRemoved();

	// CAnmStateSet overrides
	virtual void SetWorldMatrix(matrix4 *pMat);
	virtual bool GetAlphaBlending();

	// New methods
	virtual void Render(class CAnmDevice *pDevice, CAnmStateSet *&prevstate, class CAnmCamera *pCamera,
		class CAnmPrimitive *pPrimitive = NULL, int triangleIndex = 0);

	// Accessors
	virtual void SetGeometry(class CAnmGeometry *pGeometry);
	virtual class CAnmGeometry *GetGeometry()
	{
		return m_geometry;
	}

};

// Render group: collects leaves or other groups; home for bbox-based culling and such
class CAnmRenderGroup : public CAnmRenderObject
{
protected :
	
	std::list<CAnmRenderObject *>	m_children;

	virtual void DrawBBox(class CAnmDevice *pDevice);

public :

	CAnmRenderGroup();
	virtual ~CAnmRenderGroup();

	// CAnmRenderObject overrides
	virtual void GetBoundingBox(CAnmBoundingBox *pBoundingBox);
	virtual void TestPick(matrix4 &ivmat, Point3 from, Point3 to, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void SphereCollide(matrix4 &ivmat, Point3 center, float radius, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void CapsuleCollide(matrix4 &ivmat, const Point3 &capsulePt0, const Point3 &capsulePt1, 
		float capsuleRadius, eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void Cull(class CAnmFrustum *pFrustum, 
		std::vector<class CAnmDrawable *>	*pRenderList);

	// New methods
	virtual void AddChild(CAnmRenderObject *pChild);
	virtual void RemoveChild(CAnmRenderObject *pChild);
	virtual void ClearChildren(void);


	// Accessors
	std::list<CAnmRenderObject *> *GetChildren()
	{
		return &m_children;
	}

};

// The render graph: a group with global state, light allocation etc.

// allocation chunk for vector of drawables
#define ANMRENDERGRAPH_RENDERLISTALLOC 1024			

class CAnmRenderGraph : public CAnmRenderGroup, public CAnmStateSet
{
	std::vector<CAnmDrawable *>	 m_renderlist;
	class CAnmLightGroup		*m_globalLightGroup;
	int							 m_visibleobjects;
	bool						 m_zbufferenabled;
	CAnmGraphics::Fog			*m_fog;
	std::list<CAnmDrawable *>	 m_screenobjects;
	bool						 m_screenobjectsupdated;
	Point3						 m_lastcameraposforscreen;

	// CAnmRenderObject overrides
	virtual void TestPick(matrix4 &ivmat, Point3 from, Point3 to, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void SphereCollide(matrix4 &ivmat, Point3 center, float radius, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void CapsuleCollide(matrix4 &ivmat, const Point3 &capsulePt0, const Point3 &capsulePt1, 
		float capsuleRadius, eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);

	// Helpers
	void UpdateScreenObjects(class CAnmDevice *pDevice, class CAnmCamera *pCamera);


public :

	CAnmRenderGraph();
	virtual ~CAnmRenderGraph();

	// New methods

	// CAnmRenderObject overrides

	// I'm the root of this hierarchy
	virtual CAnmRenderGraph *GetRoot()
	{
		return this;
	}

	// New methods
	virtual void Render(class CAnmDevice *pDevice, class CAnmCamera *pCamera);
	virtual void TestPick(class CAnmDevice *pDevice, class CAnmCamera *pCamera, Point3 from, Point3 to, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void SphereCollide(class CAnmDevice *pDevice, class CAnmCamera *pCamera, Point3 center, float radius, 
		eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);
	virtual void CapsuleCollide(class CAnmDevice *pDevice, class CAnmCamera *pCamera, const Point3 &capsulePt0, const Point3 &capsulePt1, 
		float capsuleRadius, eAnmPickSort sortType, bool collideTest, struct sAnmRenderGraphPickList *pPickList);

	virtual class CAnmLightGroup *GetGlobalLightGroup()
	{
		return m_globalLightGroup;
	}

	virtual int NumVisibleObjects()
	{
		return m_visibleobjects;
	}

	virtual void AddScreenObject(CAnmDrawable *pDrawable);
	virtual void RemoveScreenObject(CAnmDrawable *pDrawable);

	// Accessors
	virtual void SetZBuffering(bool enabled)
	{
		m_zbufferenabled = enabled;
	}

	virtual bool GetZBuffering()
	{
		return m_zbufferenabled;
	}

	virtual void SetFog(CAnmGraphics::Fog *pFog)
	{
		m_fog = pFog;
	}

	virtual CAnmGraphics::Fog *GetFog()
	{
		return m_fog;
	}
};


// light instances and groups thereof
class CAnmLightInstance : public CAnmRenderObject
{
protected :

	CAnmGraphics::Light				*m_light;
	int								 m_lightIndex;
	bool							 m_on;

	friend class CAnmLight;

	virtual void SetLightIndex(int index)
	{
		m_lightIndex = index;
	}

public :

	CAnmLightInstance();
	CAnmLightInstance(cApplication *pApp, eAnmLightType lightType);
	virtual ~CAnmLightInstance();

	// New methods
	virtual void CreateLight(cApplication *pApp, eAnmLightType lightType);

	virtual int GetLightIndex()
	{
		return m_lightIndex;
	}

	virtual CAnmGraphics::Light *GetLight()
	{
		return m_light;
	}

	virtual void SetOn(bool on)
	{
		m_on = on;
	}

	virtual bool GetOn()
	{
		return m_on;
	}

	virtual void SetLightType(eAnmLightType lightType)
	{
		m_light->SetLightType(lightType);
	}

	virtual eAnmLightType GetLightType()
	{
		return m_light->GetLightType();
	}
};

class CAnmLightGroup
{
protected : 
	
	std::list<CAnmLightInstance *>	m_lights;
	CAnmLightGroup *				m_parentgroup;

public :

	CAnmLightGroup();
	virtual ~CAnmLightGroup();

	// New methods
	virtual void AddLight(CAnmLightInstance *pLight);
	virtual void RemoveLight(CAnmLightInstance *pLight);

	virtual void SetParentGroup(CAnmLightGroup *pLightGroup);

	virtual void ClearLights();

	static void ChangeLighting(class CAnmDevice *pDevice, 
		CAnmLightGroup *pOldLighting, CAnmLightGroup *pNewLighting);

	void ClearLighting(class CAnmDevice *pDevice);
	void SetLighting(class CAnmDevice *pDevice);
};


#endif // _anmrenderobject_h
