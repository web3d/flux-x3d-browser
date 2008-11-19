/********************************************************************************
 * Flux
 *
 * File: anmanchor.h
 * Description: WWW Anchor node class
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

#ifndef _anmanchor_h
#define _anmanchor_h

#include "anmgroup.h"
#include "anmurlobject.h"

#define ANMANCHOR_DEFAULT_DESCRIPTION		""

class CAnmAnchor : public CAnmGroup, public CAnmUrlObject
{

protected:

	String				 m_description;
	StringArray			*m_parameter;

	class CAnmTouchSensor	*m_anchorSensor;

	// helpers
	void HandleClick(Time t);
	void HandleOver(Boolean isOver);
	void HandleActive(Boolean isActive);

	// static members
	static void AnchorClickCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData);
	static void AnchorOverCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData);
	static void AnchorActiveCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData);

	// helpers for async file loading
	virtual class CAnmAsset *handleUrlLoaded(String pRequestUrl, String pCacheFileName);
	virtual void handleUrlLoaded(String pRequestUrl, class CAnmAsset *pAsset);

	void BindToViewpoint(char *viewpointUrl);

public:


	// constructor/destructor
	CAnmAnchor();
	virtual ~CAnmAnchor();

	// CAnmGroup overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();								// re-render/reset rendering structures
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);
	virtual void PreTraverse();
	virtual void PostTraverse();

	virtual void SetBaseUrl(String baseUrl)
	{
		// we're a group *and* an urlobject; do both versions
		CAnmUrlObject::SetBaseUrl(baseUrl);
		CAnmGroup::SetBaseUrl(baseUrl);
	}

	virtual String GetBaseUrl()
	{
		return CAnmUrlObject::GetBaseUrl();
	}

	// basic QueryInterface-like capability
	virtual bool GetInterface(ANMINTERFACE ival, void *pvoid)
	{
		// actually we don't want the world to get our UrlObject so just
		// always return None
		return false;
	}


	// New methods

	// Accessors
	void SetUrl(StringArray *pUrl);
	// N.B.: GetUrl methods defined in CAnmUrlObject superclass

 	void SetDescription(String title);
	String GetDescription() { return m_description; }
	void GetDescription(String *pVal)
	{ 
		if (pVal)
			*pVal = m_description;
	}
	
	void SetParameter(StringArray *pInfo);
	StringArray *GetParameter() { return m_parameter; }
	void GetParameter(StringArray **pVal)
	{ 
		if (pVal)
			*pVal = m_parameter;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmAnchor);

	DECLARE_VALUE(bboxCenter);
	DECLARE_VALUE(bboxSize);
	DECLARE_VALUE(children);
	DECLARE_VALUE(description);
	DECLARE_VALUE(parameter);
	DECLARE_VALUE(url);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_bboxCenter);
	DECLARE_METHOD(set_bboxSize);
	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_description);
	DECLARE_METHOD(set_parameter);
	DECLARE_METHOD(set_url);

	DECLARE_CALLBACK(bboxCenter_changed);
	DECLARE_CALLBACK(bboxSize_changed);
	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(description_changed);
	DECLARE_CALLBACK(parameter_changed);
	DECLARE_CALLBACK(url_changed);

};


#endif // _anmanchor_h

