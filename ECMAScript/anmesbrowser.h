/********************************************************************************
 * Flux
 *
 * File: anmesbrowser.h
 * Description: ECMAScript browser object
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


#ifndef _anmesbrowser_h
#define _anmesbrowser_h

#include "anmesobject.h"

class CAnmESBrowserObject : public CAnmESBuiltinObject
{
protected :

public :

	// Constructor/destructor
	CAnmESBrowserObject(class CAnmBrowser *pBrowser, class CAnmECMAScriptLanguageBinding *pOwner);
	virtual ~CAnmESBrowserObject();

	// New Methods
	AnmESVar createVrmlFromURL(int nArgs, AnmESVar *args);
	AnmESVar createVrmlFromString(int nArgs, AnmESVar *args);
	AnmESVar createX3DFromString(int nArgs, AnmESVar *args);
	AnmESVar name();
	AnmESVar version();
	AnmESVar currentFrameRate();
	AnmESVar currentSpeed();
	AnmESVar currentScene();
	AnmESVar setDescription(int nArgs, AnmESVar *args);
	AnmESVar print(int nArgs, AnmESVar *args);
	AnmESVar addRoute(int nArgs, AnmESVar *args);
	AnmESVar deleteRoute(int nArgs, AnmESVar *args);
	AnmESVar getCurrentFrameRate(int nArgs, AnmESVar *args);
	AnmESVar getCurrentSpeed(int nArgs, AnmESVar *args);
	AnmESVar getName(int nArgs, AnmESVar *args);
	AnmESVar getVersion(int nArgs, AnmESVar *args);

	AnmESVar moveView(int nArgs, AnmESVar *args);
	AnmESVar rotateView(int nArgs, AnmESVar *args);
	AnmESVar orbitView(int nArgs, AnmESVar *args);

	AnmESVar firstViewpoint(int nArgs, AnmESVar *args);
	AnmESVar lastViewpoint(int nArgs, AnmESVar *args);
	AnmESVar nextViewpoint(int nArgs, AnmESVar *args);
	AnmESVar previousViewpoint(int nArgs, AnmESVar *args);
	AnmESVar getViewpoints();
	
	AnmESVar showConsole(int nArgs, AnmESVar *args);

	// Linkage for callbacks, methods, properties. include in all classes derived from this one
	DECLARE_CLASS(CAnmESBrowserObject);

	DECLARE_ESMETHOD(createVrmlFromURL);
	DECLARE_ESMETHOD(createVrmlFromString);
	DECLARE_ESMETHOD(createX3DFromString);
	DECLARE_ESMETHOD(setDescription);
	DECLARE_ESMETHOD(print);
	DECLARE_ESMETHOD(addRoute);
	DECLARE_ESMETHOD(deleteRoute);
	DECLARE_ESMETHOD(getCurrentFrameRate);
	DECLARE_ESMETHOD(getCurrentSpeed);
	DECLARE_ESMETHOD(getName);
	DECLARE_ESMETHOD(getVersion);
	DECLARE_ESMETHOD(moveView);
	DECLARE_ESMETHOD(rotateView);
	DECLARE_ESMETHOD(orbitView);
	DECLARE_ESMETHOD(firstViewpoint);
	DECLARE_ESMETHOD(lastViewpoint);
	DECLARE_ESMETHOD(nextViewpoint);
	DECLARE_ESMETHOD(previousViewpoint);
	DECLARE_ESMETHOD(showConsole);
	DECLARE_ESPROPERTY(name);
	DECLARE_ESPROPERTY(version);
	DECLARE_ESPROPERTY(currentFrameRate);
	DECLARE_ESPROPERTY(currentSpeed);
	DECLARE_ESPROPERTY(currentScene);
	DECLARE_ESPROPERTY(viewpoints);
};

#endif // _anmesbrowser_h
