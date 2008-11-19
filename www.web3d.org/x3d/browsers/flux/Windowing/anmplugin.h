/********************************************************************************
 * Flux
 *
 * File: anmplugin.h
 * Description: Abstract Plugin Window class
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

#ifndef _anmplugin_h
#define _anmplugin_h

class CAnmPlugin 
{
protected :

public:


	virtual void SetStatusText(char *txt) PURE ;
	virtual void WriteToConsole(char *txt) PURE ;
	virtual void PostAppMsgCall( sAnmThreadedCallback *pTCB ) PURE ;

	virtual void AdviseViewpointBound(class CAnmViewpoint *pViewpoint) PURE ;
	virtual void AdviseNavigationModeChange(int navmode) PURE ;
	virtual void AdviseCameraChange(struct sAnmCameraChange *pCameraChange) PURE ;
	virtual void AdviseUndoMoveChange(bool canundo, bool canredo) PURE ;

	virtual void ShowConsole() PURE ;
	virtual void ShowUserGuide() PURE ;

	virtual void ComputeBaseUrl(char *pBaseUrl, int maxCharCount) PURE ;

	virtual void LinkToUrl(const char *url, const char *target) PURE;

};

#endif // _anmplugin_h
