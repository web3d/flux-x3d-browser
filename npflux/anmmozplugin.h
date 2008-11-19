/********************************************************************************
 * Flux
 *
 * File: anmmozplugin.h
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

#ifndef _anmmozplugin_h
#define _anmmozplugin_h

#include "anmplugin.h"

class CAnmMOZPlugin : public CAnmPlugin
{
protected :

	class CPlugin			*m_nsp;

public:


	CAnmMOZPlugin(class CPlugin *pNsp);
	virtual ~CAnmMOZPlugin();

	virtual void SetStatusText(char *txt);
	virtual void WriteToConsole(char *txt);
	virtual void PostAppMsgCall( sAnmThreadedCallback *pTCB );

	virtual void AdviseViewpointBound(class CAnmViewpoint *pViewpoint);
	virtual void AdviseNavigationModeChange(int navmode);
	virtual void AdviseCameraChange(struct sAnmCameraChange *pCameraChange);
	virtual void AdviseUndoMoveChange(bool canundo, bool canredo);

	virtual void ShowConsole();
	virtual void ShowUserGuide();

	virtual void ComputeBaseUrl(char *pBaseUrl, int maxCharCount);

	virtual void LinkToUrl(const char *url, const char *target);

};

#endif // _anmmozplugin_h
