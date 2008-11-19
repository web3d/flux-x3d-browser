/********************************************************************************
 * Flux
 *
 * File: anmmainlayer.cpp
 * Description: Main Layer node for the main scene
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

#include "stdafx.h"

#include "anmmainlayer.h"
#include "anmnavigationinfo.h"
#include "anmviewpoint.h"
#include "anmviewer.h"

CAnmMainLayer::CAnmMainLayer(cApplication *pApp) : CAnmLayer3D(),
	m_app(pApp)
{
	m_viewpointIter = m_viewpoints.begin();
}

CAnmMainLayer::~CAnmMainLayer()
{
}


void CAnmMainLayer::Realize()
{
	CAnmLayer3D::Realize();

	RemoveViewpointOrphans();
	if (m_viewpoints.empty())
	{
		// throw in a default camera if none was loaded
		Point3 vpt;
		if (m_app->GetOptions(eAnmOptionsCoordinateSystem) == eAnmCoordinatesLeftHanded)
			vpt = Point3(0, 0, -10.f);
		else
			vpt = Point3(0, 0, 10.f);

		CAnmViewpoint *pViewpoint = new CAnmViewpoint();
		m_app->AddGlobalObject(pViewpoint);
		pViewpoint->SetPosition(vpt);
		String vpdesc = new CAnmString("[Default Viewpoint]");
		pViewpoint->SetDescription(vpdesc);
		vpdesc->UnRef();

		AddChild(pViewpoint);

		BindToViewpoint(pViewpoint);
	}
	else
	{
		// if we have a viewpoint, go to it
		std::list<CAnmViewpoint *>::iterator iter;
		
		iter = m_viewpoints.begin();
		if (iter != m_viewpoints.end())
		{
			CAnmViewpoint *pViewpoint = *iter;
			if (pViewpoint)
			{
				BindToViewpoint(pViewpoint);
			}
		}
	}
}

void CAnmMainLayer::UpdateBindables()
{
	// Get base class functionality - updates the bindables and sets values into camera
	CAnmLayer::UpdateBindables();

	// Now while we're at it, make sure viewer knows about any viewpoint-based scaling
	// and other things for navigation
	CAnmViewer *pViewer = m_app->GetViewer();

	if (pViewer)
	{
		// make sure the camera is in the right place
		if (m_viewpoint)
		{
			matrix4 vwmat = m_viewpoint->GetWorldMatrix();
			// pesky origin
			point3 org = vwmat * point3::Zero;
			point3 x = vwmat * point3::i - org;
			point3 y = vwmat * point3::j - org;
			point3 z = vwmat * point3::k - org;
			float xmag = x.Mag();
			float ymag = y.Mag();
			float zmag = z.Mag();
			point3 scale(xmag, ymag, zmag);
			pViewer->SetViewScale(scale);

			Point3 center = m_viewpoint->GetCenterOfRotation();
			center = vwmat * center;
			pViewer->SetCenterOfRotation(center);
		}	

		// Make sure nav info is up to date in the camera and viewer
		if (m_navigationInfo)
		{
			pViewer->SetSpeed(m_navigationInfo->GetSpeed());
		}
		else
		{
			pViewer->SetSpeed(ANMNAVINFO_DEFAULT_SPEED);
		}
	}
}

void CAnmMainLayer::SetNavigationInfo(class CAnmNavigationInfo *pNavigationInfo)
{
	// make sure all the values are up to date
	if (pNavigationInfo)
		pNavigationInfo->Update();

	m_navigationInfo = pNavigationInfo;

	CAnmViewer *pViewer = m_app->GetViewer();
	if (pViewer)
	{
		if (pNavigationInfo)
		{
			pViewer->SetNavigationMode(pNavigationInfo->GetNavigationMode());
			pViewer->SetSpeed(pNavigationInfo->GetSpeed());
			pViewer->SetCollisionRadius(pNavigationInfo->GetCollisionRadius());
			pViewer->SetAvatarHeight(pNavigationInfo->GetAvatarHeight());
			pViewer->SetMaxStepHeight(pNavigationInfo->GetMaxStepHeight());
			pViewer->SetLookAt(pNavigationInfo->GetLookAt());
			pViewer->SetTransitionMode(pNavigationInfo->GetTransitionMode());
		}
		else
		{
			pViewer->SetNavigationMode(eNavigationModeWalk);
			pViewer->SetSpeed(ANMNAVINFO_DEFAULT_SPEED);
			pViewer->SetCollisionRadius(ANMNAVINFO_DEFAULT_AVATAR_COLLISION_DISTANCE);
			pViewer->SetAvatarHeight(ANMNAVINFO_DEFAULT_AVATAR_HEIGHT_ABOVE_TERRAIN);
			pViewer->SetMaxStepHeight(ANMNAVINFO_DEFAULT_AVATAR_TALLEST_OBJECT_HEIGHT);
			pViewer->SetLookAt(ANMNAVINFO_DEFAULT_LOOKAT);
			pViewer->SetTransitionMode(eNavigationTransitionLinear);
		}
	}
}

void CAnmMainLayer::SetViewpoint(class CAnmViewpoint *pViewpoint)
{
	m_viewpoint = pViewpoint;

	RemoveViewpointOrphans();
	std::list<CAnmViewpoint *>::iterator iter;
	for (iter = m_viewpoints.begin(); iter != m_viewpoints.end(); iter++)
		if ((*iter) == pViewpoint)
			break;

	m_viewpointIter = iter;

	m_app->AdviseViewpointBound(pViewpoint);
}

void CAnmMainLayer::FirstViewpoint()
{
	RemoveViewpointOrphans();
	if (m_viewpoints.size() == 0)
		return;

	m_viewpointIter = m_viewpoints.begin();

	if (*m_viewpointIter)
	{
		if (*m_viewpointIter != m_viewpoint)
			BindToViewpoint(*m_viewpointIter);
		else
			m_viewpoint->Restore();
	}

}

void CAnmMainLayer::LastViewpoint()
{
	RemoveViewpointOrphans();
	int sz = m_viewpoints.size();

	if (sz == 0)
		return;

	m_viewpointIter = m_viewpoints.begin();

	for (int i = 0; i < sz - 1; i++)
		m_viewpointIter++;

	if (*m_viewpointIter)
	{
		if (*m_viewpointIter != m_viewpoint)
			BindToViewpoint(*m_viewpointIter);
		else
			m_viewpoint->Restore();
	}

}

void CAnmMainLayer::NextViewpoint()
{
	RemoveViewpointOrphans();
	if (m_viewpoints.size() == 0)
		return;

	m_viewpointIter++;
	if (m_viewpointIter == m_viewpoints.end())
		m_viewpointIter = m_viewpoints.begin();

	if (*m_viewpointIter)
	{
		if (*m_viewpointIter != m_viewpoint)
			BindToViewpoint(*m_viewpointIter);
		else
			m_viewpoint->Restore();
	}
}

void CAnmMainLayer::PreviousViewpoint()
{
	RemoveViewpointOrphans();
	if (m_viewpoints.size() == 0)
		return;

	if (m_viewpointIter == m_viewpoints.begin())
		m_viewpointIter = m_viewpoints.end();
	m_viewpointIter--;

	if (*m_viewpointIter)
	{
		if (*m_viewpointIter != m_viewpoint)
			BindToViewpoint(*m_viewpointIter);
		else
			m_viewpoint->Restore();
	}
}

