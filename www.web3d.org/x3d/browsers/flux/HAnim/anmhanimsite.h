/********************************************************************************
 * Flux
 *
 * File: anmhanimsite.h
 * Description: HAnim Site node
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

#ifndef _anmhanimsite_h
#define _anmhanimsite_h

#include "anmtransform.h"

#define ANMHANIMSITE_DEFAULT_NAME							""

class  CAnmHAnimSite : public CAnmTransform
{

protected:

	String								 m_name;

public:

	// constructor/destructor
	CAnmHAnimSite();
	virtual ~CAnmHAnimSite();

	// CAnmTransform overrides
	// $$$ None, this is just a wrapper

	// New methods
	
	// Accessors
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

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmHAnimSite);

	DECLARE_VALUE(bboxCenter);
	DECLARE_VALUE(bboxSize);
	DECLARE_VALUE(center);
	DECLARE_VALUE(children);
	DECLARE_VALUE(name);
	DECLARE_VALUE(rotation);
	DECLARE_VALUE(scale);
	DECLARE_VALUE(scaleOrientation);
	DECLARE_VALUE(translation);

	DECLARE_METHOD(addChildren);
	DECLARE_METHOD(removeChildren);
	DECLARE_METHOD(set_center);
	DECLARE_METHOD(set_children);
	DECLARE_METHOD(set_name);
	DECLARE_METHOD(set_rotation);
	DECLARE_METHOD(set_scale);
	DECLARE_METHOD(set_scaleOrientation);
	DECLARE_METHOD(set_translation);

	DECLARE_CALLBACK(center_changed);
	DECLARE_CALLBACK(children_changed);
	DECLARE_CALLBACK(name_changed);
	DECLARE_CALLBACK(rotation_changed);
	DECLARE_CALLBACK(scale_changed);
	DECLARE_CALLBACK(scaleOrientation_changed);
	DECLARE_CALLBACK(translation_changed);
};


#endif // _anmhanimsite_h
