/********************************************************************************
 * Flux
 *
 * File: anmmozplugin.cpp
 * Description: Mozilla Plugin Window class
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
#include "anmmozplugin.h"
#include "plugin.h"

CAnmMOZPlugin::CAnmMOZPlugin(CPlugin *pNsp)
{
	m_nsp = pNsp;
}

CAnmMOZPlugin::~CAnmMOZPlugin()
{
}

void CAnmMOZPlugin::PostAppMsgCall( sAnmThreadedCallback *pTCB )
{
	assert(m_nsp);

	m_nsp->PostAppMsgCall(pTCB);
}

void CAnmMOZPlugin::SetStatusText(char *txt)
{
	assert(m_nsp);

	m_nsp->SetStatusText(txt);
}

void CAnmMOZPlugin::WriteToConsole(char *txt)
{
	assert(m_nsp);

	m_nsp->WriteToConsole(txt);
}

void CAnmMOZPlugin::AdviseViewpointBound(class CAnmViewpoint *pViewpoint)
{
	assert(m_nsp);

	m_nsp->AdviseViewpointBound(pViewpoint);
}

void CAnmMOZPlugin::AdviseNavigationModeChange(int navmode)
{
	assert(m_nsp);

	m_nsp->AdviseNavigationModeChange(navmode);
}

void CAnmMOZPlugin::AdviseCameraChange(struct sAnmCameraChange *pCameraChange)
{
	assert(m_nsp);

	m_nsp->AdviseCameraChange(pCameraChange);
}

void CAnmMOZPlugin::AdviseUndoMoveChange(bool canundo, bool canredo)
{
	assert(m_nsp);

	m_nsp->AdviseUndoMoveChange(canundo, canredo);
}


void CAnmMOZPlugin::ShowConsole()
{
	assert(m_nsp);

	m_nsp->ShowConsole();
}

void CAnmMOZPlugin::ShowUserGuide()
{
	assert(m_nsp);

	m_nsp->ShowUserGuide();
}


void CAnmMOZPlugin::ComputeBaseUrl(char *pBaseUrl, int maxCharCount)
{
	assert(m_nsp);

	m_nsp->ComputeBaseUrl(pBaseUrl, maxCharCount);
}

void CAnmMOZPlugin::LinkToUrl(const char *url, const char *target)
{
	assert(m_nsp);

	m_nsp->LinkToUrl(url, target);
}
