/********************************************************************************
 * Flux
 *
 * File: anmworld.h
 * Description: World object - top-level world
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

#ifndef _anmworld_h
#define _anmworld_h

#include "anmgroup.h"
#include "anmbindablenode.h"

class EXPORT CAnmWorld : public CAnmGroup
{
protected :

	class cApplication			*m_app;
	class CAnmScene				*m_scene;
	class CAnmMainLayer			*m_mainlayer;

	long						 m_polycount;

	ulong						 m_bgColor;

	std::vector<class CAnmRenderGraph *>
								 m_renderGraphs;

	int							 m_nextlightindex;			// helps managing global list of lights
	int							 m_nextRenderGraphIndex;	// for layers

	std::vector<class CAnmLayer *>	m_backgroundlayers;
	std::vector<class CAnmLayer *>	m_foregroundlayers;

	class CAnmLayer				*m_applayer;

	bool						 m_seekMode;

public :

	// Constructor/destructor
	CAnmWorld(class cApplication *pApp, class CAnmScene *pScene);
	virtual ~CAnmWorld();

	// CAnmTransform overrides
	virtual void Realize();
	virtual void Update();
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual void PreTraverse();
	
	virtual CAnmWorld *GetWorld()
	{
		return this;
	}

	virtual class CAnmScene *GetContainingScene()
	{
		return m_scene;
	}

	virtual CAnmLayer *GetLayer();
	
	virtual class CAnmProto *GetProtoContext();

	virtual void GetBoundingBox(CAnmBoundingBox *pBoundingBox);
	virtual float GetRadius();
	virtual void GetObjectCenter(Point3 *pCenter);

	// New methods
	virtual void Render(class CAnmDevice *pDevice);

	virtual class CAnmDevice *GetDevice();

	virtual void AddViewpoint(class CAnmViewpoint *pViewpoint);
	virtual std::list<class CAnmViewpoint *> *GetViewpoints();

	virtual void FirstViewpoint();
	virtual void LastViewpoint();
	virtual void NextViewpoint();
	virtual void PreviousViewpoint();
	virtual void BindToViewpoint(class CAnmViewpoint *pViewpoint);

	virtual int AllocLight();
	virtual void FreeLight(int index);

	virtual int AllocRenderGraph();

	virtual bool TestCollision(Point3 oldpos, Point3 newpos, float radius, Point3 &hitPoint, Point3 &hitNormal);
	virtual bool TestTerrain(Point3 oldpos, Point3 newpos, float radius, Point3 &hitPoint);
	virtual bool TestPick(int mousex, int mousey, Point3 &hitPoint, class CAnmDrawable **ppDrawable);

	// Accessors
	virtual class cApplication *GetApp()
	{
		return m_app;
	}

	virtual class CAnmScene *GetScene()
	{
		return m_scene;
	}

	virtual class CAnmCamera *GetCamera();
	virtual class CAnmBackground *GetBackground();
	virtual class CAnmFog *GetFog();
	virtual class CAnmNavigationInfo *GetNavigationInfo();
	virtual class CAnmViewpoint *GetViewpoint();

	virtual class CAnmRenderGraph *GetRenderGraph(int index = 0)
	{
		return m_renderGraphs[index];
	}

	virtual void AddLayer(class CAnmLayer *pLayer);

	virtual void SetSeekMode(bool seekMode)
	{
		m_seekMode = seekMode;
	}

	virtual bool GetSeekMode()
	{
		return m_seekMode;
	}

	virtual void SetBackgroundColor(ulong bgcolor);
	virtual ulong GetBackgroundColor()
	{
		return m_bgColor;
	}

	virtual matrix4 *GetMatrix();
	virtual void GetCenter(Point3 *pp);

	virtual class CAnmPickManager *GetPickManager();
};

#endif // _anmscene_h
