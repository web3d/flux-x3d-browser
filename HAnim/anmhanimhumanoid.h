/********************************************************************************
 * Flux
 *
 * File: anmhanimhumanoid.h
 * Description: HAnim Humanoid node
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

#ifndef _anmanmhanimhumanoid_h
#define _anmanmhanimhumanoid_h

#include "anmtransform.h"

#define ANMHANIMHUMANOID_DEFAULT_NAME							""
#define ANMHANIMHUMANOID_DEFAULT_VERSION						""

class  CAnmHAnimHumanoid : public CAnmTransform
{
 
protected:

	StringArray							*m_info;
	NodeArray							*m_joints;
	String								 m_name;
	NodeArray							*m_segments;
	NodeArray							*m_sites;
	NodeArray							*m_skeleton;
	NodeArray							*m_skin;
	CAnmNode							*m_skinCoord;
	CAnmNode							*m_skinNormal;
	String								 m_version;
	NodeArray							*m_viewpoints;

	virtual void InitializeSkin();
	virtual void InitializeSkin(class CAnmCoordinate *pCoord, class CAnmNormal *pNormal);

public:

	// constructor/destructor
	CAnmHAnimHumanoid();
	virtual ~CAnmHAnimHumanoid();

	// CAnmTransform overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures
	virtual void ChildChanged(CAnmNode *pChild);

	// New methods
	// krv, why virtual?
	virtual void UpdateSkin(class CAnmCoordinate *pCoord, class CAnmNormal *pNormal);
	

	// Accessors
	void SetInfo(StringArray *info);
	StringArray *GetInfo() { return m_info; }
	void GetInfo(StringArray **pVal)
	{
		if (pVal)
			*pVal = m_info;
	}

	void SetJoints(NodeArray *pJoints);
	NodeArray *GetJoints() { return m_joints; }
	void GetJoints(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_joints;
	}

	void SetName(String name);
	String GetName()
	{
		return m_name;
	}
	void GetName(String *pStr)
	{
		assert(pStr);
		*pStr = m_name;
	}

	void SetSegments(NodeArray *pSegments);
	NodeArray *GetSegments() { return m_segments; }
	void GetSegments(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_segments;
	}
	

	void SetSites(NodeArray *pSites);
	NodeArray *GetSites() { return m_sites; }
	void GetSites(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_sites;
	}
	
	// These two are special; they pass thru to Transform::SetChildren
	void SetSkeleton(NodeArray *pSkeleton);
	NodeArray *GetSkeleton() { return m_skeleton; }
	void GetSkeleton(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_skeleton;
	}

	void SetSkin(NodeArray *pSkin);
	NodeArray *GetSkin() { return m_skin; }
	void GetSkin(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_skin;
	}
	

	void SetSkinCoord(CAnmNode *pSkinCoord);
	class CAnmNode *GetSkinCoord() { return m_skinCoord; }
	void GetSkinCoord(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_skinCoord;
	}
	
	void SetSkinNormal(CAnmNode *pSkinNormal);
	class CAnmNode *GetSkinNormal() { return m_skinNormal; }
	void GetSkinNormal(CAnmNode **pVal)
	{ 
		if (pVal)
			*pVal = m_skinNormal;
	}
	
	void SetVersion(String version);
	String GetVersion()
	{
		return m_version;
	}
	void GetVersion(String *pStr)
	{
		assert(pStr);
		*pStr = m_version;
	}

	void SetViewpoints(NodeArray *pViewpoints);
	NodeArray *GetViewpoints() { return m_viewpoints; }
	void GetViewpoints(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_viewpoints;
	}
	
	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmHAnimHumanoid);

	DECLARE_VALUE(bboxCenter);
	DECLARE_VALUE(bboxSize);
	DECLARE_VALUE(center);
	DECLARE_VALUE(info);
	DECLARE_VALUE(joints);
	DECLARE_VALUE(name);
	DECLARE_VALUE(rotation);
	DECLARE_VALUE(scale);
	DECLARE_VALUE(scaleOrientation);
	DECLARE_VALUE(segments);
	DECLARE_VALUE(sites);
	DECLARE_VALUE(skeleton);
	DECLARE_VALUE(skin);
	DECLARE_VALUE(skinCoord);
	DECLARE_VALUE(skinNormal);
	DECLARE_VALUE(translation);
	DECLARE_VALUE(version);
	DECLARE_VALUE(viewpoints);

	DECLARE_METHOD(set_bboxCenter);
	DECLARE_METHOD(set_bboxSize);
	DECLARE_METHOD(set_center);
	DECLARE_METHOD(set_info);
	DECLARE_METHOD(set_joints);
	DECLARE_METHOD(set_name);
	DECLARE_METHOD(set_rotation);
	DECLARE_METHOD(set_scale);
	DECLARE_METHOD(set_scaleOrientation);
	DECLARE_METHOD(set_segments);
	DECLARE_METHOD(set_sites);
	DECLARE_METHOD(set_skeleton);
	DECLARE_METHOD(set_skin);
	DECLARE_METHOD(set_skinCoord);
	DECLARE_METHOD(set_skinNormal);
	DECLARE_METHOD(set_translation);
	DECLARE_METHOD(set_version);
	DECLARE_METHOD(set_viewpoints);

	DECLARE_CALLBACK(bboxCenter_changed);
	DECLARE_CALLBACK(bboxSize_changed);
	DECLARE_CALLBACK(center_changed);
	DECLARE_CALLBACK(info_changed);
	DECLARE_CALLBACK(joints_changed);
	DECLARE_CALLBACK(name_changed);
	DECLARE_CALLBACK(rotation_changed);
	DECLARE_CALLBACK(scale_changed);
	DECLARE_CALLBACK(scaleOrientation_changed);
	DECLARE_CALLBACK(segments_changed);
	DECLARE_CALLBACK(sites_changed);
	DECLARE_CALLBACK(skeleton_changed);
	DECLARE_CALLBACK(skin_changed);
	DECLARE_CALLBACK(skinCoord_changed);
	DECLARE_CALLBACK(skinNormal_changed);
	DECLARE_CALLBACK(translation_changed);
	DECLARE_CALLBACK(version_changed);
	DECLARE_CALLBACK(viewpoints_changed);

};

#define ANMHUMANOID_SKINCOORDDIRTY			(ANMTRANSFORM_NDIRTYBITS)
#define ANMHUMANOID_SKINNORMALDIRTY			(ANMHUMANOID_SKINCOORDDIRTY + 1)
#define ANMHUMANOID_NDIRTYBITS				(ANMHUMANOID_SKINNORMALDIRTY + 1)

typedef enum eAnmHumanoidDirtyFlags {
eAnmHumanoidSkinCoordDirty				= ANMDIRTYBIT(ANMHUMANOID_SKINCOORDDIRTY),
eAnmHumanoidSkinNormalDirty				= ANMDIRTYBIT(ANMHUMANOID_SKINNORMALDIRTY),
} eAnmHumanoidDirtyFlags;


#endif // _anmtransform_h
