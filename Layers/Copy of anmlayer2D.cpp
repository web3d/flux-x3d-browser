/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			anmlayer2D.cpp
 *		Description:	Anima Layer2D node - grouping node for 2D layer
 *
 * (C) 2001-2005 by Media Machines
 *
 *		Revision history:
 *			$Log: anmlayer2D.cpp,v $
 *
 *******************************************************************/

#include "stdafx.h"

#include "anmlayer2D.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmupdatevisitor.h"
#include "anmviewport.h"
#include "anmworld.h"
#include "anmlayout.h"

CAnmLayer2D::CAnmLayer2D() : CAnmLayer() ,
	m_scaleToPixelFactor( 1.0f ),
	m_layout( ANMLAYER2D_DEFAULT_LAYOUT ),
	m_transparency( ANMLAYER2D_DEFAULT_TRANSPARENCY )
{
	m_WidthInRealUnits = 1.0f;
	m_HeightInRealUnits = 1.0f;
	m_WidthInPixels = 1.0f;
	m_HeightInPixels = 1.0f;
}

CAnmLayer2D::~CAnmLayer2D()
{
	SafeUnRef(m_layout);
}

void CAnmLayer2D::Realize()
{
	CAnmLayer::Realize();

	// Set the camera to Orthogonal, and init the params
	//
	m_camera->SetOrthographic( true );
	m_camera->SetOrthoParams( -2.0, 2.0, -2.0, 2.0, -2.0, 2.0 );

}


void CAnmLayer2D::RecalculateCameraParams(CAnmDevice *pDevice)
{

	float x0 = -2.0;
	float x1 =  2.0;
	float y0 = -2.0;
	float y1 =  2.0;
	float z0 = -2.0;
	float z1 =  2.0;


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


	// krv  How do we know this BB is up to date?????
	//
	CAnmBoundingBox bbox;
	m_renderGraph->GetBoundingBox(&bbox);

	// First, get the Z extents using the Bounding Box.
	Point3 bbmin, bbmax;
	bbox.GetMinMax(&bbmin, &bbmax);
	z0 = bbmin.z;
	z1 = bbmax.z;

	// Add a tiny bit of epsilon, because some pixels were bleeding through.
	//
	float epsilon = z1 - z0 * 0.001f;
	if( epsilon < 0.0001f ) {
		epsilon = 0.0001f;
	}
	z0 -= epsilon;
	z1 += epsilon;

	CAnmViewport *pViewport = pDevice->GetViewport();
	assert(pViewport);

	int iMainPixelWidth = pViewport->Width();
	int iMainPixelHeight = pViewport->Height();

	if( iMainPixelWidth <= 0 || iMainPixelHeight <= 0 ) {
		return;
	}

	// Get the size of the window in Pixels
	//
	float fMainPixelWidth  = ( float )iMainPixelWidth;
	float fMainPixelHeight = ( float )iMainPixelHeight;

	float fOverlayPixelWidth = width;
	float fOverlayPixelHeight = height;

	if( widthUnits != eUnitsPixel ) {
		fOverlayPixelWidth *= fMainPixelWidth;
	}
	if( heightUnits != eUnitsPixel ) {
		fOverlayPixelHeight *= fMainPixelHeight;
	}

	if( fOverlayPixelHeight < 0.001f || fOverlayPixelWidth < 0.001f ) {
		return;
	}


	// Get the size of the Viewport in Ortho Units
	// It must have the same AR as the Viewport ( obviously )
	// A square VP will have go from -1,-1 to 1,1, so the width and height would be 2.0
	// The center is 0,0.
	float fVPOrthoWidth  = 2.0f;
	float fVPOrthoHeight = 2.0f;

	float fAspectRatioOfViewport = fOverlayPixelHeight / fOverlayPixelWidth;

	if( fAspectRatioOfViewport > 1.0f ) {
		fVPOrthoHeight *= fAspectRatioOfViewport;
	}
	else {
		fVPOrthoWidth  /= fAspectRatioOfViewport;
	}
	

	// calculate viewport location
	//

	float posVPOrthX0 = 0.0f;
	float posVPOrthX1 = 1.0f;
	float posVPOrthY0 = 0.0f;
	float posVPOrthY1 = 1.0f;

	float posXCenter = 0.5f;
	float posYCenter = 0.5f;


	float fScreenOrthoWidth  = fMainPixelWidth  * fVPOrthoWidth  / fOverlayPixelWidth;
	float fScreenOrthoHeight = fMainPixelHeight * fVPOrthoHeight / fOverlayPixelHeight;

	switch( uAlignMode ) {
		case euAlignmentLeft:
			posVPOrthX0 = 0.0f;
			posVPOrthX1 = fVPOrthoWidth;
			break;

		case euAlignmentCenter:
			posVPOrthX0 = 0.5f * fScreenOrthoWidth - 0.5f * fVPOrthoWidth;
			posVPOrthX1 = 0.5f * fScreenOrthoWidth + 0.5f * fVPOrthoWidth;
			break;

		case euAlignmentRight:
			posVPOrthX0 = fScreenOrthoWidth - fVPOrthoWidth;
			posVPOrthX1 = fScreenOrthoWidth;
			break;
	}

	switch( vAlignMode ) {
		case evAlignmentBottom:
			posVPOrthY0 = 0.0f;
			posVPOrthY1 = fVPOrthoHeight;
			break;

		case evAlignmentMiddle:
			posVPOrthY0 = 0.5f * fScreenOrthoHeight - 0.5f * fVPOrthoHeight;
			posVPOrthY1 = 0.5f * fScreenOrthoHeight + 0.5f * fVPOrthoHeight;
			break;

		case evAlignmentTop:
			posVPOrthY0 = fScreenOrthoHeight - fVPOrthoHeight;
			posVPOrthY1 = fScreenOrthoHeight;
			break;
	}

	// Calcualate Offsets
	//
	float fXOffsetPixels = horizontalOffset;
	float fYOffsetPixels = verticalOffset;

	if( horizontalOffsetUnits != eUnitsPixel ) {
		fXOffsetPixels *= fMainPixelWidth;
	}
	if( verticalOffsetUnits != eUnitsPixel ) {
		fYOffsetPixels *= fMainPixelHeight;
	}

	float fXOffsetOrtho = fXOffsetPixels * fVPOrthoWidth  / fOverlayPixelWidth;
	float fYOffsetOrtho = fYOffsetPixels * fVPOrthoHeight / fOverlayPixelHeight;

	// Apply Offsets
	//

	
	posVPOrthX0 += fXOffsetOrtho;
	posVPOrthX1 += fXOffsetOrtho;
	posVPOrthY0 += fYOffsetOrtho;
	posVPOrthY1 += fYOffsetOrtho;


	// Get the Center of the VP in ortho units
	//
	float posVPCenterX = 0.5f * ( posVPOrthX0 + posVPOrthX1 );
	float posVPCenterY = 0.5f * ( posVPOrthY0 + posVPOrthY1 );

	// Offset a Zero to Width ortho values so that ZERO will be in the center of the Viewport.
	//
	x0 = -posVPCenterX;
	x1 = fScreenOrthoWidth -  posVPCenterX;
	y0 = -posVPCenterY;
	y1 = fScreenOrthoHeight - posVPCenterY;


	m_scaleToPixelFactor = fVPOrthoWidth  / fOverlayPixelWidth;

	// Scale the X points
	//
	float factor = 1.0f;
	
	factor = 1.0f / m_scaleToPixelFactor;
	

	float NewWidthInRealUnits = fOverlayPixelWidth;
	float NewHeightInRealUnits = fOverlayPixelHeight;

	float NewWidthInPixels = fOverlayPixelWidth;
	float NewHeightInPixels = fOverlayPixelHeight;


	x0 *= factor;
	x1 *= factor;
	y0 *= factor;
	y1 *= factor;

	z0 *= factor;
	z1 *= factor;

	m_camera->SetOrthoParams( x0 - 0.5f, x1 - 0.5f, y0 + 0.5f, y1 + 0.5f, z0, z1 );



	float scaleX = 1.0f;
	float scaleY = 1.0f;
	float scaleZ = 1.0f;

	scaleZ = factor;


	if( uScaleMode != eScaleModePixel ) {
		scaleX = ( fOverlayPixelWidth );
	}
	if( vScaleMode != eScaleModePixel ) {
		scaleY = ( fOverlayPixelHeight );
	}

	// Fix Aspect Ratio if Needed.
	//
	if( uScaleMode == eScaleModeStretch ) {
		scaleX = scaleY;
	}

	if( vScaleMode == eScaleModeStretch ) {
		scaleY = scaleX;
	}


	NewWidthInRealUnits /= scaleX;
	NewHeightInRealUnits /= scaleY;

	m_scale = Point3(scaleX, scaleY, scaleZ );
	// SetScale( Point3(scaleX, scaleY, scaleZ ));


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

}



// void CAnmLayer2D::UpdateAlignmentModes()

// Accessors




void CAnmLayer2D::SetLayout(CAnmNode* layout)
{
	SafeUnRef(m_layout);
	m_layout = layout;
	m_layout->Ref();
	SetStateDirty();
	CallCallbacks(CALLBACKID(CAnmLayer2D, layout_changed), &layout);
}


void CAnmLayer2D::ParentChanged(CAnmGroup *pParent)
{
	SetStateAndMatrixDirty();
}

void CAnmLayer2D::ChildChanged(CAnmNode *pChild)
{
	SetStateAndMatrixDirty();
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmLayer2D, children, CAnmLayer);

DEFINE_INHERITED_METHOD(CAnmLayer2D, addChildren, CAnmLayer);
DEFINE_INHERITED_METHOD(CAnmLayer2D, removeChildren, CAnmLayer);
DEFINE_INHERITED_METHOD(CAnmLayer2D, set_children, CAnmLayer);

DEFINE_INHERITED_CALLBACK(CAnmLayer2D, children_changed, CAnmLayer);






DEFINE_VALUE(CAnmLayer2D, layout, eValueNode, CAnmNode*, GetLayout, SetLayout);
DEFINE_METHOD(CAnmLayer2D, set_layout, eValueNode, CAnmNode*, SetLayout);
DEFINE_CALLBACK(CAnmLayer2D, layout_changed, eValueNode);

DEFINE_INHERITED_VALUE(CAnmLayer2D, transparency, CAnmLayer);
DEFINE_INHERITED_METHOD(CAnmLayer2D, set_transparency, CAnmLayer);
DEFINE_INHERITED_CALLBACK(CAnmLayer2D, transparency_changed, CAnmLayer);

DEFINE_INHERITED_VALUE(CAnmLayer2D, type, CAnmLayer);


// member table
BEGIN_DEFINE_MEMBERS(CAnmLayer2D)
VALUEID(CAnmLayer2D, children),
METHODID(CAnmLayer2D, addChildren),
METHODID(CAnmLayer2D, removeChildren),
METHODID(CAnmLayer2D, set_children),
CALLBACKID(CAnmLayer2D, children_changed),


VALUEID(CAnmLayer2D, layout),
METHODID(CAnmLayer2D, set_layout),
CALLBACKID(CAnmLayer2D, layout_changed),


VALUEID(CAnmLayer2D, transparency),
METHODID(CAnmLayer2D, set_transparency),
CALLBACKID(CAnmLayer2D, transparency_changed),

VALUEID(CAnmLayer2D, type),





END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmLayer2D, CAnmLayer);
