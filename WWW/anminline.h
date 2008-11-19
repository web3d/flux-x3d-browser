/********************************************************************************
 * Flux
 *
 * File: anminline.h
 * Description: WWW Inline node class
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

#ifndef _anminline_h
#define _anminline_h

#include "anmgroup.h"
#include "anmurlobject.h"

class CAnmInline : public CAnmGroup, public CAnmUrlObject
{

protected:

	class CAnmScene			*m_inlineScene;
	String					 m_baseUrlForChildren;

	bool					 m_loading;

	// helpers for async file loading
	virtual class CAnmAsset *handleUrlLoaded(String pRequestUrl, String pCacheFileName);
	virtual void handleUrlLoaded(String pRequestUrl, class CAnmAsset *pAsset);
	virtual void handleSceneLoaded(class CAnmSceneSymbol *pSceneSymbol, cApplication *pApp, 
		class CAnmWorld *pWorld);

public:


	// constructor/destructor
	CAnmInline();
	virtual ~CAnmInline();

	// CAnmGroup overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();								// re-render/reset rendering structures
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual class CAnmScene *GetContainingScene()
	{
		return m_inlineScene;
	}

	virtual class CAnmProto *GetProtoContext();


	virtual void SetBaseUrl(String baseUrl)
	{
		// our children are opaque, so don't do the group node's version
		// the base Url is actually set on each child when the inline file is loaded
		CAnmUrlObject::SetBaseUrl(baseUrl);
	}

	virtual String GetBaseUrl()
	{
		return CAnmUrlObject::GetBaseUrl();
	}


	// basic QueryInterface-like capability
	virtual bool GetInterface(ANMINTERFACE ival, void *pvoid)
	{
		if (ival == ANMINTERFACEID(CAnmUrlObject))
		{
			CAnmUrlObject **ppUrlObj = (CAnmUrlObject **) pvoid;
			*ppUrlObj = this;
			return true;
		}

		return false;
	}

	// New methods

	// Accessors
	void SetUrl(StringArray *pUrl);
	// N.B.: GetUrl methods defined in CAnmUrlObject superclass

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmInline);

	DECLARE_VALUE(bboxCenter);
	DECLARE_VALUE(bboxSize);
	DECLARE_VALUE(url);

	DECLARE_METHOD(set_bboxCenter);
	DECLARE_METHOD(set_bboxSize);
	DECLARE_METHOD(set_url);

	DECLARE_CALLBACK(bboxCenter_changed);
	DECLARE_CALLBACK(bboxSize_changed);
	DECLARE_CALLBACK(url_changed);

};


#endif // _anminline_h
