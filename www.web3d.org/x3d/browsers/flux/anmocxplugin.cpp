/********************************************************************************
 * Flux
 *
 * File: anmocxplugin.cpp
 * Description: ActiveX Control Plugin Window class
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
#include "anmocxplugin.h"
#include "animactl.h"
#include "anmurlobject.h"

#include "hlink.h"

CAnmOCXPlugin::CAnmOCXPlugin(CAnimaCtrl *pCtrl)
{
	m_ctl = pCtrl;
}

CAnmOCXPlugin::~CAnmOCXPlugin()
{
}


void CAnmOCXPlugin::PostAppMsgCall( sAnmThreadedCallback *pTCB )
{
	m_ctl->PostAppMsgCall(pTCB);
}

void CAnmOCXPlugin::SetStatusText(char *txt)
{
	assert(m_ctl);

	m_ctl->SetStatusText(txt);
}

void CAnmOCXPlugin::WriteToConsole(char *txt)
{
	assert(m_ctl);

	m_ctl->WriteToConsole(txt);
}

void CAnmOCXPlugin::AdviseViewpointBound(class CAnmViewpoint *pViewpoint)
{
	assert(m_ctl);

	m_ctl->AdviseViewpointBound(pViewpoint);
}

void CAnmOCXPlugin::AdviseNavigationModeChange(int navmode)
{
	assert(m_ctl);

	m_ctl->AdviseNavigationModeChange(navmode);
}

void CAnmOCXPlugin::AdviseCameraChange(struct sAnmCameraChange *pCameraChange)
{
	assert(m_ctl);

	m_ctl->AdviseCameraChange(pCameraChange);
}

void CAnmOCXPlugin::AdviseUndoMoveChange(bool canundo, bool canredo)
{
	assert(m_ctl);

	m_ctl->AdviseUndoMoveChange(canundo, canredo);
}


void CAnmOCXPlugin::ShowConsole()
{
	assert(m_ctl);

	m_ctl->ShowConsole();
}

void CAnmOCXPlugin::ShowUserGuide()
{
	assert(m_ctl);

	m_ctl->ShowUserGuide();
}


void CAnmOCXPlugin::ComputeBaseUrl(char *pBaseUrl, int maxCharCount)
{
	assert(m_ctl);

	LPOLECLIENTSITE pClientSite = m_ctl->GetClientSite();
	if (pClientSite)
	{
		HRESULT hr;
		IMoniker *pMoniker = NULL;
		IBindCtx *pBC = NULL;
		LPOLESTR wRootPath = NULL;

		hr = pClientSite->GetMoniker(OLEGETMONIKER_FORCEASSIGN, OLEWHICHMK_CONTAINER, &pMoniker);
		if (SUCCEEDED(hr))
		{
			hr = CreateBindCtx(0, &pBC);
			if (SUCCEEDED(hr))
			{
				hr = pMoniker->GetDisplayName(pBC, NULL, &wRootPath);
				if (SUCCEEDED(hr))
				{
					assert(wRootPath);

					wcstombs(pBaseUrl, wRootPath, wcslen(wRootPath) + 1);

					CAnmUrlObject::ComputeBaseUrl(pBaseUrl, pBaseUrl, maxCharCount);

					LPMALLOC pMalloc;
					hr = CoGetMalloc(1, &pMalloc);
					if (SUCCEEDED(hr))
					{
						pMalloc->Free(wRootPath);
					}
					else
					{
						// N.B.: panic!
					}
				}
			}
		}
	}
}

void CAnmOCXPlugin::LinkToUrl(const char *url, const char *target)
{

	BSTR wurl;
	BSTR frame = NULL;

	wurl = CString(url).AllocSysString();
	if (target)
		frame = CString(target).AllocSysString();

	IBindCtx *pbc; 

	CreateBindCtx( 0, &pbc ); 

	HRESULT hr = HlinkSimpleNavigateToString(wurl, NULL, frame, m_ctl->GetControllingUnknown(),
		pbc, NULL, HLNF_NAVIGATINGFORWARD, 0);

	SysFreeString(wurl);
	if (frame)
		SysFreeString(frame);

	pbc->Release();

}