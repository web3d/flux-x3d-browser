/********************************************************************************
 * Flux
 *
 * File: anmappearance.cpp
 * Description: Appearance node
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
#include "anmappearance.h"
#include "anmshader.h"
#include "anmshape.h"
#include "anmgeometry.h"
#include "anmmaterial.h"
#include "anmtexture.h"
#include "anmtexturetransform.h"


CAnmAppearance::CAnmAppearance() : CAnmGroup()
{
	m_material = NULL;
	m_shader = NULL;
	m_texture = NULL;
	m_textureTransform = NULL;
}

CAnmAppearance::~CAnmAppearance()
{
}

// Realize method - N.B.: this might be overridden and chained down to
void CAnmAppearance::Realize()
{
	CAnmGroup::Realize();	// realize the kids
}

void CAnmAppearance::Traverse(CAnmUpdateVisitor *pVisitor)
{
	CAnmGroup::Traverse(pVisitor);	// traverse the kids
}

// Update method - N.B.: this might be overridden and chained down to
void CAnmAppearance::Update()
{	
	if (m_material && m_material->StateDirty())
		GETIMPLEMENTATION(CAnmMaterial, m_material)->Update();

	if (m_shader && m_shader->StateDirty())
		GETIMPLEMENTATION(CAnmShader, m_shader)->Update();

	if (m_texture && m_texture->StateDirty())
		GETIMPLEMENTATION(CAnmTexture, m_texture)->Update();

	if (m_textureTransform && m_textureTransform->StateDirty())
		GETIMPLEMENTATION(CAnmTextureTransform, m_textureTransform)->Update();
}

eAnmReturnStatus CAnmAppearance::AddChild(CAnmNode *pChild)
{
	CAnmGroup::AddChild(pChild);

	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmMaterial)))
		m_material = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmShader)))
		m_shader = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmTexture)))
		m_texture = pChild;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmTextureTransform)))
		m_textureTransform = pChild;

	return eAnmReturnAllGood;
}

eAnmReturnStatus CAnmAppearance::RemoveChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmMaterial)))
		m_material = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmShader)))
		m_shader = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmTexture)))
		m_texture = NULL;
	else if (pImp && pImp->GetClass()->IsA(GETCLASS(CAnmTextureTransform)))
		m_textureTransform = NULL;

	CAnmGroup::RemoveChild(pChild);

	return eAnmReturnAllGood;
}


// Accessors

void CAnmAppearance::SetMaterial(CAnmNode *pMaterial)
{
	if (m_material == pMaterial)
		return;

	if (m_material)
		RemoveChild(m_material);

	if (pMaterial == NULL)
		return;

	AddChild(pMaterial);
	m_material = pMaterial;

	CallCallbacks(CALLBACKID(CAnmAppearance, material_changed), &m_material);
}

void CAnmAppearance::SetShader(CAnmNode *pShader)
{
	if (m_shader == pShader)
		return;

	if (m_shader)
		RemoveChild(m_shader);

	if (pShader == NULL)
		return;

	AddChild(pShader);
	m_shader = pShader;

	CallCallbacks(CALLBACKID(CAnmAppearance, shader_changed), &m_shader);
}

void CAnmAppearance::SetTexture(CAnmNode *pTexture)
{
	if (m_texture == pTexture)
		return;

	if (m_texture)
		RemoveChild(m_texture);

	if (pTexture == NULL)
		return;

	AddChild(pTexture);
	m_texture = pTexture;

	CallCallbacks(CALLBACKID(CAnmAppearance, texture_changed), &m_texture);

}

void CAnmAppearance::SetTextureTransform(CAnmNode *pTextureTransform)
{
	// quietly do nothing for now
	if (m_textureTransform == pTextureTransform)
		return;

	if (m_textureTransform)
		RemoveChild(m_textureTransform);

	if (pTextureTransform == NULL)
		return;

	AddChild(pTextureTransform);
	m_textureTransform = pTextureTransform;

	CallCallbacks(CALLBACKID(CAnmAppearance, textureTransform_changed), &m_textureTransform);

}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmAppearance, material, eValueNode, CAnmNode *, GetMaterial, SetMaterial);
DEFINE_VALUE(CAnmAppearance, shader, eValueNode, CAnmNode *, GetShader, SetShader);
DEFINE_VALUE(CAnmAppearance, texture, eValueNode, CAnmNode *, GetTexture, SetTexture);
DEFINE_VALUE(CAnmAppearance, textureTransform, eValueNode, CAnmNode *, GetTextureTransform, SetTextureTransform);

DEFINE_METHOD(CAnmAppearance, set_material, eValueNode, CAnmNode *, SetMaterial);
DEFINE_METHOD(CAnmAppearance, set_shader, eValueNode, CAnmNode *, SetShader);
DEFINE_METHOD(CAnmAppearance, set_texture, eValueNode, CAnmNode *, SetTexture);
DEFINE_METHOD(CAnmAppearance, set_textureTransform, eValueNode, CAnmNode *, SetTextureTransform);

DEFINE_CALLBACK(CAnmAppearance, material_changed, eValueNode);
DEFINE_CALLBACK(CAnmAppearance, shader_changed, eValueNode);
DEFINE_CALLBACK(CAnmAppearance, texture_changed, eValueNode);
DEFINE_CALLBACK(CAnmAppearance, textureTransform_changed, eValueNode);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmAppearance)
VALUEID(CAnmAppearance, material),
VALUEID(CAnmAppearance, shader),
VALUEID(CAnmAppearance, texture),
VALUEID(CAnmAppearance, textureTransform),
METHODID(CAnmAppearance, set_material),
METHODID(CAnmAppearance, set_shader),
METHODID(CAnmAppearance, set_texture),
METHODID(CAnmAppearance, set_textureTransform),
CALLBACKID(CAnmAppearance, material_changed),
CALLBACKID(CAnmAppearance, shader_changed),
CALLBACKID(CAnmAppearance, texture_changed),
CALLBACKID(CAnmAppearance, textureTransform_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmAppearance, CAnmGroup);
