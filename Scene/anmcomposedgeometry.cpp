/********************************************************************************
 * Flux
 *
 * File: anmcomposedgeometry.cpp
 * Description: ComposedGeometry node; base for
 *							geometry types composed from separate
 *							Color, Normal, TextureCoordinate nodes
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
#include "anmcomposedgeometry.h"

#include "anmcolor.h"
#include "anmcoordinate.h"
#include "anmnormal.h"
#include "anmtexturecoordinate.h"
#include "anmmesh.h"
#include "anmdevice.h"
#include "anmworld.h"

CAnmComposedGeometry::CAnmComposedGeometry() : CAnmGeometry(),
	m_ccw(ANMCG_DEFAULT_CCW),
	m_colorPerVertex(ANMCG_DEFAULT_COLORPERVERTEX),
	m_normalPerVertex(ANMCG_DEFAULT_NORMALPERVERTEX)
{
	m_color = NULL;
	m_coord = NULL;
	m_normal = NULL;
	m_texCoord = NULL;
}

CAnmComposedGeometry::~CAnmComposedGeometry()
{
}

eAnmReturnStatus CAnmComposedGeometry::AddChild(CAnmNode *pChild)
{
	CAnmGroup::AddChild(pChild);

	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmColor)))
		m_color = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_coord = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmNormal)))
		m_normal = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmTextureCoordinate)))
		m_texCoord = pChild;

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmComposedGeometry::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmColor)))
		m_color = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmCoordinate)))
		m_coord = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmNormal)))
		m_normal = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmTextureCoordinate)))
		m_texCoord = NULL;

	CAnmGroup::RemoveChild(pChild);

	return eAnmReturnAllGood;
}

void CAnmComposedGeometry::ChildChanged(CAnmNode *pChild)
{
	if (pChild == m_color)
	{
		SetDirtyBits(eAnmCGColorDirty);
	}
	else if (pChild == m_coord)
	{
		SetDirtyBits(eAnmCGCoordDirty);
		SetMatrixDirty();
	}
	else if (pChild == m_normal)
	{
		SetDirtyBits(eAnmCGNormalDirty);
	}
	else if (pChild == m_texCoord)
	{
		SetDirtyBits(eAnmCGTexCoordDirty);
	}

	SetStateDirty();
}

eAnmRenderOptions CAnmComposedGeometry::GetRenderOptions()
{
	if (m_color)
		return (eAnmRenderOptions) (eAnmRenderOptionsUseVertexForDiffuse | eAnmRenderOptionsUseVertexForAmbient);
	else
		return eAnmRenderOptionsNone;
}

// Accessors
void CAnmComposedGeometry::SetCCW(Boolean ccw)
{
	m_ccw = ccw;
	SetDirtyBits(eAnmCGCCWDirty);
}

void CAnmComposedGeometry::SetColor(CAnmNode *pColor)
{
	if (m_color == pColor)
		return;

	if (m_color)
		RemoveChild(m_color);

	if (pColor == NULL)
		return;

	AddChild(pColor);
	m_color = pColor;

	SetDirtyBits(eAnmCGColorDirty);

	CallCallbacks(CALLBACKID(CAnmComposedGeometry, color_changed), &m_color);
}

void CAnmComposedGeometry::SetColorPerVertex(Boolean colorPerVertex)
{
	m_colorPerVertex = colorPerVertex;
	SetDirtyBits(eAnmCGColorPerVertexDirty);
}

void CAnmComposedGeometry::SetCoord(CAnmNode *pCoord)
{
	if (m_coord == pCoord)
		return;

	if (m_coord)
		RemoveChild(m_coord);

	if (pCoord == NULL)
		return;

	AddChild(pCoord);
	m_coord = pCoord;

	SetDirtyBits(eAnmCGCoordDirty);

	CallCallbacks(CALLBACKID(CAnmComposedGeometry, coord_changed), &m_coord);
}

void CAnmComposedGeometry::SetNormal(CAnmNode *pNormal)
{
	if (m_normal == pNormal)
		return;

	if (m_normal)
		RemoveChild(m_normal);

	if (pNormal == NULL)
		return;

	AddChild(pNormal);
	m_normal = pNormal;

	SetDirtyBits(eAnmCGNormalDirty);

	CallCallbacks(CALLBACKID(CAnmComposedGeometry, normal_changed), &m_normal);
}

void CAnmComposedGeometry::SetNormalPerVertex(Boolean normalPerVertex)
{
	m_normalPerVertex = normalPerVertex;
	SetDirtyBits(eAnmCGNormalPerVertexDirty);
}

void CAnmComposedGeometry::SetTexCoord(CAnmNode *pTexCoord)
{
	if (m_texCoord == pTexCoord)
		return;

	if (m_texCoord)
		RemoveChild(m_texCoord);

	if (pTexCoord == NULL)
		return;

	AddChild(pTexCoord);
	m_texCoord = pTexCoord;

	SetDirtyBits(eAnmCGTexCoordDirty);

	CallCallbacks(CALLBACKID(CAnmComposedGeometry, texCoord_changed), &m_texCoord);
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_INHERITED_VALUE(CAnmComposedGeometry, solid, CAnmGeometry);

DEFINE_VALUE(CAnmComposedGeometry, ccw, eValueBoolean, Boolean, 
			 GetCCW, SetCCW);
DEFINE_VALUE(CAnmComposedGeometry, color, eValueNode, CAnmNode*, 
			 GetColor, SetColor);
DEFINE_VALUE(CAnmComposedGeometry, colorPerVertex, eValueBoolean, Boolean, 
			 GetColorPerVertex, SetColorPerVertex);
DEFINE_VALUE(CAnmComposedGeometry, coord, eValueNode, CAnmNode*, 
			 GetCoord, SetCoord);
DEFINE_VALUE(CAnmComposedGeometry, normal, eValueNode, CAnmNode*, 
			 GetNormal, SetNormal);
DEFINE_VALUE(CAnmComposedGeometry, normalPerVertex, eValueBoolean, Boolean,
			 GetNormalPerVertex, SetNormalPerVertex);
DEFINE_VALUE(CAnmComposedGeometry, texCoord, eValueNode, CAnmNode *,
			 GetTexCoord, SetTexCoord);

DEFINE_METHOD(CAnmComposedGeometry, set_color, eValueNode, CAnmNode*, SetColor);
DEFINE_METHOD(CAnmComposedGeometry, set_coord, eValueNode, CAnmNode*, SetCoord);
DEFINE_METHOD(CAnmComposedGeometry, set_normal, eValueNode, CAnmNode *, SetNormal);
DEFINE_METHOD(CAnmComposedGeometry, set_texCoord, eValueNode, CAnmNode*, SetTexCoord);

DEFINE_CALLBACK(CAnmComposedGeometry, color_changed, eValueNode);
DEFINE_CALLBACK(CAnmComposedGeometry, coord_changed, eValueNode);
DEFINE_CALLBACK(CAnmComposedGeometry, normal_changed, eValueNode);
DEFINE_CALLBACK(CAnmComposedGeometry, texCoord_changed, eValueNode);


// member table
BEGIN_DEFINE_MEMBERS(CAnmComposedGeometry)
VALUEID(CAnmComposedGeometry, ccw), 
VALUEID(CAnmComposedGeometry, color), 
VALUEID(CAnmComposedGeometry, colorPerVertex), 
VALUEID(CAnmComposedGeometry, coord), 
VALUEID(CAnmComposedGeometry, normal), 
VALUEID(CAnmComposedGeometry, normalPerVertex),
VALUEID(CAnmComposedGeometry, solid),
VALUEID(CAnmComposedGeometry, texCoord),

METHODID(CAnmComposedGeometry, set_color), 
METHODID(CAnmComposedGeometry, set_coord), 
METHODID(CAnmComposedGeometry, set_normal), 
METHODID(CAnmComposedGeometry, set_texCoord),

CALLBACKID(CAnmComposedGeometry, color_changed),
CALLBACKID(CAnmComposedGeometry, coord_changed),
CALLBACKID(CAnmComposedGeometry, normal_changed),
CALLBACKID(CAnmComposedGeometry, texCoord_changed),

END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmComposedGeometry, CAnmGeometry);


