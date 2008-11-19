/********************************************************************************
 * Flux
 *
 * File: anmmultitexturecoordinate.cpp
 * Description: MultiTextureCoordinate node
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
#include "anmmultitexturecoordinate.h"
#include "anmtexturecoordinate.h"
#include "anmtexturecoordinategenerator.h"
#include "anmdevice.h"
#include "anmworld.h"
				
CAnmMultiTextureCoordinate::CAnmMultiTextureCoordinate() : CAnmGroup()
{		
	m_texCoord = new NodeArray;
}

CAnmMultiTextureCoordinate::~CAnmMultiTextureCoordinate()
{
	SafeUnRef(m_texCoord);
}

void CAnmMultiTextureCoordinate::Realize()
{
	// chain down
	CAnmGroup::Realize();	
}

void CAnmMultiTextureCoordinate::Update()
{
	if (!StateDirty())
		return;

	int sz = m_texCoord->size();

	for (int i = 0; i < sz; i++)
	{
		CAnmTextureCoordinate *pTextureCoordinate = GetTexCoordChild(i);
		if (pTextureCoordinate)
			pTextureCoordinate->Update();
		else
		{
			CAnmTextureCoordinateGenerator *pTexCoordGen = GetTexCoordGenChild(i);
			if (pTexCoordGen)
				pTexCoordGen->Update();
		}
	}

	ClearStateDirty();
}

bool CAnmMultiTextureCoordinate::SetToDevice(class CAnmDevice *pDevice, int startLevel, int nLevels)
{
	int sz = m_texCoord->size();

	// do only one pass if the app has disabled multitexturing
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL_RTN_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	if (pApp && !pApp->GetOptions(eAnmOptionsMultiTexturing))
		sz = 1;

	int endLevel = startLevel + nLevels;
	if (endLevel > sz)
		endLevel = sz;

	bool transformed = false;

	int i, l;
	for (i = 0, l = startLevel; l < endLevel; i++, l++)
	{
		CAnmTextureCoordinateGenerator *pTexCoordGen = GetTexCoordGenChild(l);
		if (pTexCoordGen)
		{
			if (pTexCoordGen->SetToDevice(pDevice, i))
				transformed = true;
		}
		else
		{
			pDevice->SetTextureOption(i, eAnmTextureTexCoordIndex, eAnmTextureTexCoordPassThru);
		}
	}

	return transformed;
}

int CAnmMultiTextureCoordinate::NumLevels()
{
	// N.B.: might want to bolster this for NULL or non-texture children at some point
	return m_texCoord->size();
}

CAnmTextureCoordinate *CAnmMultiTextureCoordinate::GetTexCoordChild(int index)
{
	assert(index >= 0 && index < m_texCoord->size());

	CAnmNode *pChild = (*m_texCoord)[index];
	if (pChild)
	{
		CAnmTextureCoordinate *pTexCoord = GETIMPLEMENTATION(CAnmTextureCoordinate, pChild);
		return pTexCoord;
	}
	else
		return NULL;
}

CAnmTextureCoordinateGenerator *CAnmMultiTextureCoordinate::GetTexCoordGenChild(int index)
{
	assert(index >= 0 && index < m_texCoord->size());

	CAnmNode *pChild = (*m_texCoord)[index];
	if (pChild)
	{
		CAnmTextureCoordinateGenerator *pTexCoordGen = GETIMPLEMENTATION(CAnmTextureCoordinateGenerator, pChild);
		return pTexCoordGen;
	}
	else
		return NULL;
}

// Accessors
void CAnmMultiTextureCoordinate::SetTexCoord(NodeArray *pTexCoord)
{
	assert(pTexCoord != NULL);

	SafeUnRef(m_texCoord);
	m_texCoord = pTexCoord;
	m_texCoord->Ref();

	CAnmGroup::SetChildren(pTexCoord);

	SetDirtyBits(eAnmMTCTexcoordDirty);
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmMultiTextureCoordinate, texCoord_changed), &pTexCoord);
}


// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmMultiTextureCoordinate, texCoord, eValueNodeArray, NodeArray*, GetTexCoord, SetTexCoord);

DEFINE_METHOD(CAnmMultiTextureCoordinate, set_texCoord, eValueNodeArray, NodeArray*, SetTexCoord);

DEFINE_CALLBACK(CAnmMultiTextureCoordinate, texCoord_changed, eValueNodeArray);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmMultiTextureCoordinate)
VALUEID(CAnmMultiTextureCoordinate, texCoord),

METHODID(CAnmMultiTextureCoordinate, set_texCoord),

CALLBACKID(CAnmMultiTextureCoordinate, texCoord_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmMultiTextureCoordinate, CAnmGroup);

