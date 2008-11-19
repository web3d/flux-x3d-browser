/********************************************************************************
 * Flux
 *
 * File: anmlayoutgroup.cpp
 * Description: LayoutGroup node
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
#include "anmlayoutgroup.h"
#include "anmlayout.h"
#include "anmlayer2d.h"
#include "anmupdatevisitor.h"
	
CAnmLayoutGroup::CAnmLayoutGroup() : CAnmGroup(),
	m_layout( ANMLAYOUTGROUP_DEFAULT_LAYOUT )
{
	m_WidthInRealUnits = 1.0f;
	m_HeightInRealUnits = 1.0f;
	m_WidthInPixels = 1.0f;
	m_HeightInPixels = 1.0f;
	m_scale[0] = 
	m_scale[1] = 1.0f;
	m_translation[0] = 
	m_translation[1] = 0.0f;
}

CAnmLayoutGroup::~CAnmLayoutGroup()
{
	SafeUnRef(m_layout);
}

// Methods




void CAnmLayoutGroup::Traverse(CAnmUpdateVisitor *pVisitor)
{


	// krv:
	// DO I do this every time????
	//
//	if (TestDirtyBits(ANMGROUPDIRTY_CHECKTRAVERSECHILDREN))
	if ( 1 )
	{

		// push transform state
		matrix4 saved, newmat, scaleMat;
		pVisitor->GetWorldMatrix(&saved);

		// calculate the new world matrix
		scaleMat.ToScale( Point3( m_scale[0], m_scale[1], ( m_scale[0] + m_scale[1] ) / 2.0f ) );
		scaleMat.Translate( Point3( m_translation[0], m_translation[1], 0.0f ) );

		newmat = scaleMat * saved;

		// set new transform state in visitor
		pVisitor->SetWorldMatrix(&newmat);

		// call Group class to traverse children
		CAnmGroup::Traverse(pVisitor);

		// pop transform state
		pVisitor->SetWorldMatrix(&saved);
	}
}


// Accessors
void CAnmLayoutGroup::SetLayout(CAnmNode* layout)
{
	SafeUnRef(m_layout);
	m_layout = layout;
	m_layout->Ref();

	SetStateAndMatrixDirty();

	CallCallbacks(CALLBACKID(CAnmLayoutGroup, layout_changed), layout);
}




void CAnmLayoutGroup::Update()
{
	UpdateLayoutTransform();
	CAnmGroup::Update();
}

void CAnmLayoutGroup::UpdateLayoutTransform()
{



	// Get Layout Node.
	//
	CAnmLayout* pLayout = NULL;
	if( m_layout ) {
		pLayout = GETIMPLEMENTATION(CAnmLayout, m_layout );
	}

	// Init the Data that we will get from Layout Node.
	//
	Float							width = 1.0f;
	Float							height = 1.0f;

	eUnitsMode						widthUnits = ANMLAYOUT_DEFAULT_SIZEUNITSMODE;
	eUnitsMode						heightUnits = ANMLAYOUT_DEFAULT_SIZEUNITSMODE;

	Float							horizontalOffset = 0.0f;
	Float							verticalOffset = 0.0f;

	eUnitsMode						horizontalOffsetUnits = ANMLAYOUT_DEFAULT_OFFSETUNITSMODE;;
	eUnitsMode						verticalOffsetUnits = ANMLAYOUT_DEFAULT_OFFSETUNITSMODE;

	euAlignmentMode					uAlignMode = ANMLAYOUT_DEFAULT_UALIGNMENTMODE;
	evAlignmentMode					vAlignMode = ANMLAYOUT_DEFAULT_VALIGNMENTMODE;

	eScaleMode						uScaleMode = ANMLAYOUT_DEFAULT_SCALE_MODEMODE;
	eScaleMode						vScaleMode = ANMLAYOUT_DEFAULT_SCALE_MODEMODE;

	// If we have a Layout, extract the info from it.
	//
	if( pLayout ) {


		// Get Size
		Point2 size( pLayout->GetSize() );
		width = size.x;
		height = size.y;

		// get Size Units enums
		StringArray* sizeUnits = pLayout->GetSizeUnits();
		if( sizeUnits && sizeUnits->size() > 0 ) {
			heightUnits =
			widthUnits = CAnmLayout::GetUnitsMode( (*sizeUnits)[0] );
			if( sizeUnits->size() > 1 ) {
				heightUnits = CAnmLayout::GetUnitsMode( (*sizeUnits)[1] );
			}
		}

		// Get Offset
		Point2 offset( pLayout->GetOffset() );
		horizontalOffset = offset.x;
		verticalOffset = offset.y;

		// get Offset Units enums
		StringArray* offsetUnits = pLayout->GetOffsetUnits();
		if( offsetUnits && offsetUnits->size() > 0 ) {
			verticalOffsetUnits =
			horizontalOffsetUnits = CAnmLayout::GetUnitsMode( (*offsetUnits)[0] );
			if( offsetUnits->size() > 1 ) {
				verticalOffsetUnits = CAnmLayout::GetUnitsMode( (*offsetUnits)[1] );
			}
		}

		// Get alignment Modes.
		//
		StringArray* align = pLayout->GetAlign();
		if( align && align->size() > 0 ) {
			uAlignMode = CAnmLayout::GetuAlignmentMode( (*align)[0] );
			if( align->size() > 1 ) {
				vAlignMode = CAnmLayout::GetvAlignmentMode( (*align)[1] );
			}
		}


		// get Offset Units enums
		StringArray* scaleMode = pLayout->GetScaleMode();
		if( scaleMode && scaleMode->size() > 0 ) {
			vScaleMode =
			uScaleMode = CAnmLayout::GetScaleMode( (*scaleMode)[0] );
			if( scaleMode->size() > 1 ) {
				vScaleMode = CAnmLayout::GetScaleMode( (*scaleMode)[1] );
			}
		}
	}


	



	// Look up parents for parent Layer2D or LayoutGroup Node.
	// What about Use DEF?????
	// Multiple Parents
	CAnmGroup *pParent = GetFirstParent();
	bool bNotFound = true;

	float ParentWidthInRealUnits = 1.0f;
	float ParentHeightInRealUnits = 1.0f;
	float ParentWidthInPixels = 1.0f;
	float ParentHeightInPixels = 1.0f;

	while( pParent && bNotFound ) {
		CAnmLayer2D *pLayer2D = GETIMPLEMENTATION(CAnmLayer2D, pParent); 
		if (pLayer2D)
		{
			ParentWidthInRealUnits = pLayer2D->GetWidthInRealUnits();
			ParentHeightInRealUnits = pLayer2D->GetHeightInRealUnits();
			ParentWidthInPixels = pLayer2D->GetWidthInPixels();
			ParentHeightInPixels = pLayer2D->GetHeightInPixels();

			bNotFound = false;
		}

		CAnmLayoutGroup *pLayoutGroup = GETIMPLEMENTATION(CAnmLayoutGroup, pParent); 
		if (pLayoutGroup)
		{
			ParentWidthInRealUnits = pLayoutGroup->GetWidthInRealUnits();
			ParentHeightInRealUnits = pLayoutGroup->GetHeightInRealUnits();
			ParentWidthInPixels = pLayoutGroup->GetWidthInPixels();
			ParentHeightInPixels = pLayoutGroup->GetHeightInPixels();

			bNotFound = false;
		}



	}


	// If we have good parent Data, then lets do it!
	//
	if( !bNotFound && 
		ParentWidthInPixels > 1.1 &&
		ParentHeightInPixels > 1.1 &&
		ParentWidthInRealUnits > 1.0e-10 &&
		ParentHeightInRealUnits > 1.0e-10 ) {
		

	//	float fMainPixelWidth  = ( float )iMainPixelWidth;
	//	float fMainPixelHeight = ( float )iMainPixelHeight;

		// Get Size of this LayoutGroup
		//
		float fThisWidthReal = width;
		float fThisHeightReal = height;

		// Convert Size to Real Units if needed.
		//
		if( widthUnits == eUnitsPixel ) {
			fThisWidthReal *= ( ParentWidthInRealUnits / ParentWidthInPixels );
		}
		else if( widthUnits == eUnitsFraction ) {
			fThisWidthReal *= ParentWidthInRealUnits;
		}

		if( heightUnits == eUnitsPixel ) {
			fThisHeightReal *= ( ParentHeightInRealUnits / ParentHeightInPixels );
		}
		else if( heightUnits == eUnitsFraction ) {
			fThisHeightReal *= ParentHeightInRealUnits;
		}


		// Get the Offset here, because it is just like the size.
		//
		float fOffsetUReal = horizontalOffset;
		float fOffsetVReal = verticalOffset;

		// Convert Offset to Real Units if needed.
		//
		if( horizontalOffsetUnits == eUnitsPixel ) {
			fOffsetUReal *= ( ParentWidthInRealUnits / ParentWidthInPixels );
		}
		else if( horizontalOffsetUnits == eUnitsFraction ) {
			fOffsetUReal *= ParentWidthInRealUnits;
		}

		if( verticalOffsetUnits == eUnitsPixel ) {
			fOffsetVReal *= ( ParentHeightInRealUnits / ParentHeightInPixels );
		}
		else if( verticalOffsetUnits == eUnitsFraction ) {
			fOffsetVReal *= ParentHeightInRealUnits;
		}


		float dx = 0.0f;
		float dy = 0.0f;

		// Now, locate the dx and dy offset form the current origin to the new center of the LayoutGroup
		//  First we use the Alignment, then we apply offsets.

		switch( uAlignMode ) {
			case euAlignmentLeft:
				dx = - 0.5f * ( ParentWidthInRealUnits - fThisWidthReal );
				break;

			case euAlignmentRight:
				dx =   0.5f * ( ParentWidthInRealUnits - fThisWidthReal );
				break;
		}

		switch( vAlignMode ) {
			case evAlignmentBottom:
				dy = - 0.5f * ( ParentHeightInRealUnits - fThisHeightReal );
				break;

			case evAlignmentTop:
				dy =   0.5f * ( ParentHeightInRealUnits - fThisHeightReal );
				break;
		}


		// Apply the Offsets.
		//
		dx += fOffsetUReal;
		dy += fOffsetVReal;

		// Now do Scaling.  Start with one.  One is good.
		float scalex = 1.0f;
		float scaley = 1.0f;



		// Apply simple scale.
		//

		switch( uScaleMode ) {
			case eScaleModeFraction:
				scalex = fThisWidthReal;
				break;

			case eScaleModePixel:
				scalex = ParentWidthInRealUnits / ParentWidthInPixels;
				break;
		}

		switch( vScaleMode ) {
			case eScaleModeFraction:
				scaley = fThisHeightReal;
				break;

			case eScaleModePixel:
				scaley = ParentHeightInRealUnits / ParentHeightInPixels;
				break;
		}


		// Now, do the streatch, if needed.
		// Enough aspect ratios should do it.
		float realAspectRatio = ( ParentHeightInRealUnits / ParentWidthInRealUnits );
		float pixelAspectRatio = ( ParentHeightInPixels / ParentWidthInPixels );

		float ratioOfRatios = realAspectRatio / pixelAspectRatio;

		if( uScaleMode == eScaleModeStretch ) {
			scalex = scaley / ratioOfRatios;
		}
		if( vScaleMode == eScaleModeStretch ) {
			scaley = scalex * ratioOfRatios;
		}

		// Save our current settings.
		float NewWidthInRealUnits = fThisWidthReal / scalex;
		float NewHeightInRealUnits = fThisHeightReal / scaley;
		float NewWidthInPixels = fThisWidthReal * ParentWidthInPixels / ParentWidthInRealUnits;
		float NewHeightInPixels = fThisHeightReal * ParentHeightInPixels / ParentHeightInRealUnits;



		// if any of our relevent data changed, set kids dirty.
		//
		if(
			fabs( m_WidthInRealUnits - NewWidthInRealUnits ) > 0.000001 ||
			fabs( m_HeightInRealUnits - NewHeightInRealUnits ) > 0.000001 ||
			fabs( m_WidthInPixels - NewWidthInPixels ) > 0.000001 ||
			fabs( m_HeightInPixels - NewHeightInPixels ) > 0.000001 ) {
			NotifyChildrenOfChange();
		}
		m_WidthInRealUnits = NewWidthInRealUnits;
		m_HeightInRealUnits = NewHeightInRealUnits;
		m_WidthInPixels = NewWidthInPixels;
		m_HeightInPixels = NewHeightInPixels;


		// Save off our values.
		//
		m_scale[0] = scalex;
		m_scale[1] = scaley;
		m_translation[0] = dx;
		m_translation[1] = dy;
	}
}

void CAnmLayoutGroup::Realize()
{
	CAnmGroup::Realize();
}


void CAnmLayoutGroup::PostTraverse()
{
	CAnmGroup::PostTraverse();
}


void CAnmLayoutGroup::ParentChanged(CAnmGroup *pParent)
{
	SetStateAndMatrixDirty();
}

void CAnmLayoutGroup::ChildChanged(CAnmNode *pChild)
{
	SetStateAndMatrixDirty();
}

// Tables for Callbacks, Methods and Set/GetValues

//DEFINE_INHERITED_VALUE(CAnmLayoutGroup, bboxCenter, CAnmGroup);
//DEFINE_INHERITED_VALUE(CAnmLayoutGroup, bboxSize, CAnmGroup);

DEFINE_INHERITED_VALUE(CAnmLayoutGroup, children, CAnmGroup);
DEFINE_VALUE(CAnmLayoutGroup, layout, eValueNode, CAnmNode*, GetLayout, SetLayout);

DEFINE_INHERITED_METHOD(CAnmLayoutGroup, addChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmLayoutGroup, removeChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmLayoutGroup, set_children, CAnmGroup);
DEFINE_METHOD(CAnmLayoutGroup, set_layout, eValueNode, CAnmNode*, SetLayout);

DEFINE_INHERITED_CALLBACK(CAnmLayoutGroup, children_changed, CAnmGroup);
DEFINE_CALLBACK(CAnmLayoutGroup, layout_changed, eValueNode);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmLayoutGroup)
//VALUEID(CAnmLayoutGroup, bboxCenter),
//VALUEID(CAnmLayoutGroup, bboxSize),
VALUEID(CAnmLayoutGroup, children),
VALUEID(CAnmLayoutGroup, layout),

METHODID(CAnmLayoutGroup, addChildren),
METHODID(CAnmLayoutGroup, removeChildren),
METHODID(CAnmLayoutGroup, set_children),
METHODID(CAnmLayoutGroup, set_layout),

CALLBACKID(CAnmLayoutGroup, children_changed),
CALLBACKID(CAnmLayoutGroup, layout_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmLayoutGroup, CAnmGroup);

