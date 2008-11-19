/********************************************************************************
 * Flux
 *
 * File: anmmultitexturetransform.cpp
 * Description: MultiTextureTransform node
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
#include "anmmultitexturetransform.h"
#include "anmdevice.h"
#include "anmworld.h"
				
CAnmMultiTextureTransform::CAnmMultiTextureTransform() : CAnmTextureTransform()
{		
	m_textureTransform = new NodeArray;
}

CAnmMultiTextureTransform::~CAnmMultiTextureTransform()
{
	SafeUnRef(m_textureTransform);
}

void CAnmMultiTextureTransform::Realize()
{
	// chain down for texture realize
	CAnmTextureTransform::Realize();
	
	// N.B.: for now - flag everything dirty
	SetStateDirty();
}

void CAnmMultiTextureTransform::Update()
{
	if (!StateDirty())
		return;

	int sz = m_textureTransform->size();

	for (int i = 0; i < sz; i++)
	{
		CAnmTextureTransform *pTextureTransform = GetTextureTransformChild(i);
		if (pTextureTransform)
			pTextureTransform->Update();
	}

	ClearStateDirty();
}


void CAnmMultiTextureTransform::SetToDevice(class CAnmDevice *pDevice, int startLevel, int nLevels)
{
	int sz = m_textureTransform->size();

	// do only one pass if the app has disabled multitexturing
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	if (pApp && !pApp->GetOptions(eAnmOptionsMultiTexturing))
		sz = 1;

	int endLevel = startLevel + nLevels;
	if (endLevel > sz)
		endLevel = sz;

	int i, l;
	for (i = 0, l = startLevel; l < endLevel; i++, l++)
	{
		CAnmTextureTransform *pTextureTransform = GetTextureTransformChild(l);
		if (pTextureTransform)
		{
			SetStageToDevice(pDevice, pTextureTransform, i);
		}
	}
}

void CAnmMultiTextureTransform::SetStageToDevice(CAnmDevice *pDevice, CAnmTextureTransform *pTextureTransform, 
												 int stage)
{
	assert(pTextureTransform);

	pDevice->SetTextureTransform(stage, pTextureTransform->GetMatrix());
}

int CAnmMultiTextureTransform::NumLevels()
{
	// N.B.: might want to bolster this for NULL or non-texture children at some point
	return m_textureTransform->size();
}

CAnmTextureTransform *CAnmMultiTextureTransform::GetTextureTransformChild(int index)
{
	assert(index >= 0 && index < m_textureTransform->size());

	CAnmNode *pChild = (*m_textureTransform)[index];
	if (pChild)
	{
		CAnmTextureTransform *pTextureTransform = GETIMPLEMENTATION(CAnmTextureTransform, pChild);
		return pTextureTransform;
	}
	else
		return NULL;
}

// Accessors
void CAnmMultiTextureTransform::SetTextureTransform(NodeArray *pTextureTransform)
{
	assert(pTextureTransform != NULL);

	SafeUnRef(m_textureTransform);
	m_textureTransform = pTextureTransform;
	m_textureTransform->Ref();

	CAnmGroup::SetChildren(pTextureTransform);

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMultiTextureTransform, textureTransform_changed), &pTextureTransform);
}

// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmMultiTextureTransform, textureTransform, eValueNodeArray, NodeArray*, GetTextureTransform, SetTextureTransform);

DEFINE_METHOD(CAnmMultiTextureTransform, set_textureTransform, eValueNodeArray, NodeArray*, SetTextureTransform);

DEFINE_CALLBACK(CAnmMultiTextureTransform, textureTransform_changed, eValueNodeArray);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmMultiTextureTransform)

VALUEID(CAnmMultiTextureTransform, textureTransform),

METHODID(CAnmMultiTextureTransform, set_textureTransform),

CALLBACKID(CAnmMultiTextureTransform, textureTransform_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmMultiTextureTransform, CAnmTextureTransform);

