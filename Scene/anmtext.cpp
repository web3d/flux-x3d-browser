/********************************************************************************
 * Flux
 *
 * File: anmtext.cpp
 * Description: Text node
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
#include "anmtext.h"
#include "anmfont.h"
#include "anmfontstyle.h"
#include "anmscreenfontstyle.h"
#include "anmextrudedfontstyle.h"
#include "anmmesh.h"
#include "anmtextmesh.h"
#include "anmworld.h"

CAnmText::CAnmText() : CAnmGeometry(),
m_maxextent(ANMTEXT_DEFAULT_MAXEXTENT)
{
	m_solid = ANMTEXT_DEFAULT_SOLID;		// overrides default geometry class value
	m_fontstyle = NULL;
	m_strings = new StringArray;
	m_lengths = new FloatArray;

	m_font = NULL;
}

CAnmText::~CAnmText()
{
	SafeUnRef(m_font);

	SafeUnRef(m_strings);
	SafeUnRef(m_lengths);
}

eAnmReturnStatus CAnmText::AddChild(CAnmNode *pChild)
{
	CAnmNode *pImp = pChild->GetImplementationNode();

	if (pImp && 
		(	pImp->GetClass()->IsA(GETCLASS(CAnmFontStyle) ) ||
			pImp->GetClass()->IsA(GETCLASS(CAnmExtrudedFontStyle) ) ||
			pImp->GetClass()->IsA(GETCLASS(CAnmScreenFontStyle) ) ) ) {
		CAnmGroup::AddChild(pChild);
		m_fontstyle = pChild;
		return eAnmReturnAllGood;
	}

	return eAnmReturnAllBad;
}

float CAnmText::GenRadius()
{
	return 1.f;		// N.B.: for now
}

void CAnmText::SetStrings(StringArray *strings)
{
	assert(strings != NULL);

	SafeUnRef(m_strings);
	m_strings = strings;
	m_strings->Ref();

	SetStateDirty();
	SetDirtyBits(eAnmTextStringsDirty);

	CallCallbacks(CALLBACKID(CAnmText, string_changed), &strings);
}

void CAnmText::SetLengths(FloatArray *lengths)
{
	assert(lengths != NULL);

	SafeUnRef(m_lengths);
	m_lengths = lengths;
	m_lengths->Ref();

	SetStateDirty();
	SetDirtyBits(eAnmTextLengthsDirty);

	CallCallbacks(CALLBACKID(CAnmText, length_changed), &lengths);
}

void CAnmText::SetMaxExtent(Float maxExtent)
{
	m_maxextent = maxExtent;

	SetStateDirty();
	SetDirtyBits(eAnmTextMaxExtentDirty);

	CallCallbacks(CALLBACKID(CAnmText, maxExtent_changed), &maxExtent);
}

void CAnmText::SetFontStyle(CAnmNode *pFontStyle)
{
	if (m_fontstyle == pFontStyle)
		return;

	if (m_fontstyle)
		RemoveChild(m_fontstyle);

	if (pFontStyle == NULL)
		return;

	AddChild(pFontStyle);

	SetStateDirty();
	SetDirtyBits(eAnmTextFontStyleDirty);

	CallCallbacks(CALLBACKID(CAnmText, fontStyle_changed), &m_fontstyle);
}

void CAnmText::CheckFontStyleParams(CAnmNode *pFontStyleNode)
{
	if (pFontStyleNode == NULL)
		return;

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmFontStyle* pFontStyle = NULL;
	CAnmScreenFontStyle* pFontStyleExt = NULL;

	// N.B.: strings
	if (pFontStyle = GETIMPLEMENTATION(CAnmFontStyle, pFontStyleNode))
	{
		if (pFontStyle->GetSize() <= 0)
		{
			pApp->Warn("Invalid size in FontStyle; using default");
		}
	}
	else if (pFontStyleExt = GETIMPLEMENTATION(CAnmScreenFontStyle, pFontStyleNode))
	{
		if (pFontStyleExt->GetPointSize() <= 0)
		{
			pApp->Warn("Invalid size in FontStyleExt; using default");
		}
	}

}

void CAnmText::CreateMesh()
{
	CAnmNode *pFontStyle = NULL;

	if (m_fontstyle)
		pFontStyle = m_fontstyle->GetImplementationNode();

	CheckFontStyleParams(pFontStyle);

	// N.B.: NULL pFontStyle param is OK here
	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	CAnmDevice *pDevice = pApp->GetDevice();
	assert(pDevice);

	m_mesh = new CAnmTextMesh(pDevice, this, m_font, pFontStyle);
	m_mesh->SetDiffuseColor(color4::White);
}

static eFontFamily FontFamily(const char *familyname)
{
	if (!strlen(familyname))
		return eFontFamilySerif;

	if (!strcmp(familyname, "SERIF"))
		return eFontFamilySerif;
	if (!strcmp(familyname, "SANS"))
		return eFontFamilySansSerif;
	if (!strcmp(familyname, "TYPEWRITER"))
		return eFontFamilyTypewriter;
	
	return eFontFamilyUseName;
}

static eFontStyle FontStyle(const char *stylename)
{
	if (!strlen(stylename))
		return eFontStylePlain;

	if (!strcmp(stylename, "PLAIN"))
		return eFontStylePlain;
	if (!strcmp(stylename, "BOLD"))
		return eFontStyleBold;
	if (!strcmp(stylename, "ITALIC"))
		return eFontStyleItalic;
	if (!strcmp(stylename, "BOLDITALIC"))
		return eFontStyleBoldItalic;

	return eFontStylePlain;
}

void CAnmText::FindFont()
{
	if (m_fontstyle) {
		CAnmExtrudedFontStyle* pExtrudedFontStyle = NULL;
		CAnmFontStyle* pFontStyle = NULL;
		CAnmScreenFontStyle* pFontStyleExt = NULL;
		bool bNeedDefaultFont = true;

		StringArray *pStrs = NULL;
		const char *stylename = NULL;

		// Only specify point size of Bitmap fonts
		//
		int iPointSize = 0;
		if( pFontStyle = GETIMPLEMENTATION(CAnmFontStyle, m_fontstyle) ) {
			pStrs = pFontStyle->GetFamily();
			stylename = pFontStyle->GetStyle()->GetBuf();
			bNeedDefaultFont = false;
		}
		else if( pExtrudedFontStyle = GETIMPLEMENTATION(CAnmExtrudedFontStyle, m_fontstyle) ) {
			pStrs = pExtrudedFontStyle->GetFamily();
			stylename = pExtrudedFontStyle->GetStyle()->GetBuf();
			bNeedDefaultFont = false;
		}
		else if( pFontStyleExt = GETIMPLEMENTATION(CAnmScreenFontStyle, m_fontstyle) ) {
			pStrs = pFontStyleExt->GetFamily();
			stylename = pFontStyleExt->GetStyle()->GetBuf();
			iPointSize = ( int )pFontStyleExt->GetPointSize();
			bNeedDefaultFont = false;
		}
		if( !bNeedDefaultFont ) {

			CAnmWorld *pWorld = GetWorld();
//			assert(pWorld);
			ABORT_IF_WORLD_IS_NULL

			cApplication *pApp = pWorld->GetApp();


			// for now, just try the first
			const char *familyname = "TYPEWRITER";
			if (pStrs) {
				familyname = (*pStrs)[0]->GetBuf();
			}

			eFontFamily family = FontFamily(familyname);

			eFontStyle style = FontStyle(stylename);

			m_font = CAnmFont::FindFont(familyname, family, style, iPointSize, pApp );
		}
		else {
			m_font = CAnmFont::DefaultFont();
		}
	}
	else
		m_font = CAnmFont::DefaultFont();

	if( m_font ) {
		m_font->Ref();
	}
}

void CAnmText::Realize()
{
	FindFont();
	CreateMesh();
			
	CAnmGeometry::Realize();		// chain down

	ClearStateDirty();				// we're built, so we're clean
}

void CAnmText::Update()
{
	// N.B.: why we need this I have no idea-- TP 11/24/03
	if (!m_realized)
		return;

	if (StateDirty())
	{
		if (TestDirtyBits(eAnmTextFontStyleDirty))
		{
			SafeUnRef(m_font);
			FindFont();
		}

		CAnmTextMesh *pTextMesh = (CAnmTextMesh *) m_mesh;

		CAnmNode * pFontStyle = NULL;
		if (m_fontstyle) 
			pFontStyle = m_fontstyle->GetImplementationNode();
	
		CheckFontStyleParams(pFontStyle);

		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL

		cApplication *pApp = pWorld->GetApp();

		CAnmDevice *pDevice = pApp->GetDevice();


		// N.B.: NULL pFontStyle param is OK here
		pTextMesh->Rebuild(pDevice, this, m_font, pFontStyle );
		m_mesh->SetDiffuseColor(color4::White);

		// Re-generate boxes
		GenBoundingVolumes();

		// N.B.: this is just to update the polycount, duh
		m_polycount = m_mesh->GetPolyCount();

		// N.B.: don't do this; very BAD things happen - extra
		// shape children added to the parent group. cf
		// CAnmShape::Traverse() which adds a child if the instance
		// is dirty. Question is, why aren't the old ones ever
		// cleaned up? need to review Traversal algorithm w.r.t this...
		// TP 06/13/05
		// SetInstanceDirty();
		ClearStateDirty();

		pWorld->SetStateDirty();
	}
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmText, solid, CAnmGeometry);

DEFINE_VALUE(CAnmText, string, eValueStringArray, StringArray *, GetStrings, SetStrings);
DEFINE_VALUE(CAnmText, length, eValueFloatArray, FloatArray *, GetLengths, SetLengths);
DEFINE_VALUE(CAnmText, maxExtent, eValueFloat, Float, GetMaxExtent, SetMaxExtent);
DEFINE_VALUE(CAnmText, fontStyle, eValueNode, CAnmNode *, GetFontStyle, SetFontStyle);

DEFINE_METHOD(CAnmText, set_string, eValueStringArray, StringArray *, SetStrings);
DEFINE_METHOD(CAnmText, set_length, eValueFloatArray, FloatArray *, SetLengths);
DEFINE_METHOD(CAnmText, set_maxExtent, eValueFloat, Float, SetMaxExtent);
DEFINE_METHOD(CAnmText, set_fontStyle, eValueNode, CAnmNode *, SetFontStyle);

DEFINE_CALLBACK(CAnmText, string_changed, eValueStringArray);
DEFINE_CALLBACK(CAnmText, length_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmText, maxExtent_changed, eValueFloat);
DEFINE_CALLBACK(CAnmText, fontStyle_changed, eValueNode);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmText)
VALUEID(CAnmText, solid),
VALUEID(CAnmText, string),
VALUEID(CAnmText, length),
VALUEID(CAnmText, maxExtent),
VALUEID(CAnmText, fontStyle),

METHODID(CAnmText, set_string),
METHODID(CAnmText, set_length),
METHODID(CAnmText, set_maxExtent),
METHODID(CAnmText, set_fontStyle),

CALLBACKID(CAnmText, string_changed),
CALLBACKID(CAnmText, length_changed),
CALLBACKID(CAnmText, maxExtent_changed),
CALLBACKID(CAnmText, fontStyle_changed)

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmText, CAnmGeometry);

