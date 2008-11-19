/********************************************************************************
 * Flux
 *
 * File: anmfog.cpp
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

#include "stdafx.h"
#include "anmfog.h"

#include "anmcamera.h"
#include "anmdevice.h"
#include "anmtransform.h"
#include "anmupdatevisitor.h"
#include "anmviewport.h"
#include "anmworld.h"

CAnmFog::CAnmFog() : 
	CAnmBindableNode(),
	m_color(ANMFOG_DEFAULT_COLOR),
	m_visibilityRange(ANMFOG_DEFAULT_VISIBILITYRANGE)
{
	m_fog = NULL;
	m_fogType = new CAnmString(ANMFOG_DEFAULT_FOGTYPE);
	m_fogMode = getFogMode(m_fogType);
	m_enabled = false;
	m_worldmatrix = matrix4::Identity;
}

CAnmFog::~CAnmFog()
{
	SafeDelete(m_fog);
	SafeUnRef(m_fogType);
}

void CAnmFog::Realize()
{
	CAnmBindableNode::Realize();

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmGraphics *pGraphics = pApp->GetGraphics();
	assert(pGraphics);

	m_fog = pGraphics->CreateFog();

}

void CAnmFog::Update()
{
	if (!m_isBound)
		return;

	if (StateDirty())
	{
		m_fogMode = getFogMode(m_fogType);

		if (m_visibilityRange == 0.f || m_fogMode == eFogModeBadMode)
		{
			// Turn off fog
			m_enabled = false;
		}
		else
		{
			// get our visibilityRange as a scaled value
			// by transforming a vector in world coordinates
			CAnmTransform *pParent = GetFirstTransformParent();
			
			float vislimit = m_visibilityRange;
			if (pParent)
			{
				Point3 pvis(m_visibilityRange, 0, 0);
				pvis = m_worldmatrix * pvis;

				// subtract out pesky origin
				Point3 porg = m_worldmatrix * point3::Zero;
				pvis = pvis - porg;

				vislimit = pvis.Mag();
			}

			if (!(vislimit > FLT_EPSILON))
				vislimit = FLT_EPSILON;
			
			m_fog->SetColor(m_color);
			m_fog->SetStart(0.f);
			m_fog->SetEnd(vislimit);
			// N.B.: This is an empirical value and needs to be thoroughly tested; ->
			// also it should be reviewed when we port to Open GL-- TP 1/20/04
			m_fog->SetDensity(2.f / vislimit);
			m_fog->SetMode(m_fogMode);

			// Good to go
			m_enabled = true;
		}

		ClearStateDirty();
	}
}


void CAnmFog::Traverse(CAnmUpdateVisitor *pVisitor)
{
	matrix4 mat;

	pVisitor->GetWorldMatrix(&mat);

	m_worldmatrix = mat;

	CAnmNode::Traverse(pVisitor);
}

eAnmFogMode CAnmFog::getFogMode(String fogType)
{
	const char *cp = fogType->GetBuf();

	eAnmFogMode result = eFogModeBadMode;

	if (!strcmp(cp, "LINEAR"))
		result = eFogModeLinear;
	else if (!strcmp(cp, "EXPONENTIAL"))
		result = eFogModeExponential;
	else if (!strcmp(cp, "EXPONENTIALSQUARED"))
		result = eFogModeExponentialSquared;

	return result;
}

// Accessors
void CAnmFog::SetColor(Color c)
{
	m_color = c;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmFog, color_changed), &c);
}

void CAnmFog::SetFogType(String s)
{
	SafeUnRef(m_fogType);
	m_fogType = s;		
	m_fogType->Ref();

	m_fogMode = getFogMode(m_fogType);

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmFog, fogType_changed), &s);
}

void CAnmFog::SetVisibilityRange(Float visibilityRange)
{
	m_visibilityRange = visibilityRange;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmFog, visibilityRange_changed), &visibilityRange);
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmFog, color, eValueColor, Color, GetColor, SetColor);
DEFINE_VALUE(CAnmFog, fogType, eValueString, String, GetFogType, SetFogType);
DEFINE_VALUE(CAnmFog, visibilityRange, eValueFloat, Float, GetVisibilityRange, SetVisibilityRange);

DEFINE_INHERITED_METHOD(CAnmFog, set_bind, CAnmBindableNode);
DEFINE_METHOD(CAnmFog, set_color, eValueColor, Color, SetColor);
DEFINE_METHOD(CAnmFog, set_fogType, eValueString, String, SetFogType);
DEFINE_METHOD(CAnmFog, set_visibilityRange, eValueFloat, Float, SetVisibilityRange);


DEFINE_INHERITED_CALLBACK(CAnmFog, isBound, CAnmBindableNode);
DEFINE_INHERITED_CALLBACK(CAnmFog, bindTime, CAnmBindableNode);
DEFINE_CALLBACK(CAnmFog, color_changed, eValueColor);
DEFINE_CALLBACK(CAnmFog, fogType_changed, eValueString);
DEFINE_CALLBACK(CAnmFog, visibilityRange_changed, eValueFloat);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmFog)
VALUEID(CAnmFog, color),
VALUEID(CAnmFog, fogType),
VALUEID(CAnmFog, visibilityRange),

METHODID(CAnmFog, set_bind),
METHODID(CAnmFog, set_color),
METHODID(CAnmFog, set_fogType),
METHODID(CAnmFog, set_visibilityRange),

CALLBACKID(CAnmFog, isBound),
CALLBACKID(CAnmFog, bindTime),
CALLBACKID(CAnmFog, color_changed),
CALLBACKID(CAnmFog, fogType_changed),
CALLBACKID(CAnmFog, visibilityRange_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmFog, CAnmBindableNode);
