/********************************************************************************
 * Flux
 *
 * File: anmfog.h
 * Description: Fog node
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

#ifndef _anmfog_h
#define _anmfog_h

#include "anmbindablenode.h"

#define ANMFOG_DEFAULT_COLOR				Color(1.f, 1.f, 1.f)
#define ANMFOG_DEFAULT_FOGTYPE				"LINEAR"
#define ANMFOG_DEFAULT_VISIBILITYRANGE		0.f
#define ANMFOG_DENSITY						1.f

class CAnmFog : public CAnmBindableNode
{

protected:

	CAnmGraphics::Fog		*m_fog;
	Color					 m_color;
	String					 m_fogType;
	Float					 m_visibilityRange;

	eAnmFogMode				 m_fogMode;
	bool					 m_enabled;

	matrix4					 m_worldmatrix;

	eAnmFogMode				 getFogMode(String fogType);

public:

	// Constructor/Destructor
	CAnmFog();
	virtual ~CAnmFog();

	// Node class overrides
	virtual void Realize();		// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures
	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);

	// New methods

	// Accessors
	void SetColor(Color color);
	Color GetColor()
	{
		return m_color;
	}
	void GetColor(Color *pVal)
	{
		assert(pVal);
		*pVal = m_color;
	}

	void SetFogType(String fogType);
	String GetFogType()
	{
		return m_fogType;
	}
	void GetFogType(String *pStr)
	{
		assert(pStr);
		*pStr = m_fogType;
	}

	void SetVisibilityRange(Float visibilityRange);
	Float GetVisibilityRange() { return m_visibilityRange; }
	void GetVisibilityRange(Float *pVal)
	{
		assert(pVal);
		*pVal = m_visibilityRange;
	}

	virtual bool FogEnabled()
	{
		return m_enabled;
	}

	virtual CAnmGraphics::Fog *GetRawFog()
	{
		return m_fog;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmFog);

	DECLARE_VALUE(color);
	DECLARE_VALUE(fogType);
	DECLARE_VALUE(visibilityRange);

	DECLARE_METHOD(set_bind);
	DECLARE_METHOD(set_color);
	DECLARE_METHOD(set_fogType);
	DECLARE_METHOD(set_visibilityRange);

	DECLARE_CALLBACK(isBound);
	DECLARE_CALLBACK(bindTime);
	DECLARE_CALLBACK(color_changed);
	DECLARE_CALLBACK(fogType_changed);
	DECLARE_CALLBACK(visibilityRange_changed);
};

#endif // _anmfog_h
