/********************************************************************************
 * Flux
 *
 * File: anmlayer3D.cpp
 * Description: Layer3D node - grouping node for 3D layer
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

#include "anmlayer3D.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmnavigationinfo.h"
#include "anmupdatevisitor.h"
#include "anmviewpoint.h"
#include "anmviewport.h"
#include "anmworld.h"
#include "anmMatrixInvert.h"

CAnmLayer3D::CAnmLayer3D() : CAnmLayer(),
 m_autoOrient(ANMLAYER3D_DEFAULT_AUTOORIENT)
{
}

CAnmLayer3D::~CAnmLayer3D()
{
}

void CAnmLayer3D::Realize()
{
	CAnmLayer::Realize();
}

void CAnmLayer3D::RecalculateCameraParams(CAnmDevice *pDevice)
{
	if (m_autoOrient)
	{
		CAnmWorld *pWorld = GetWorld();
//		assert(pWorld);
		ABORT_IF_WORLD_IS_NULL

		CAnmCamera *pMainLayerCamera = pWorld->GetCamera();
		assert(pMainLayerCamera);

		Point3 dir, up;
		pMainLayerCamera->GetOrientation(&dir, &up);

		m_camera->SetOrientation(dir, up);
	}

	// Set up view clip
	double neard, fard;
	CalcClippingPlanes(&neard, &fard, pDevice->GetZBufferBytes());

	// Set up our camera
	m_camera->SetNearDistance(neard);
	m_camera->SetFarDistance(fard);

}

#define TURKOWSKI_REV_2


#define MINNEARMINSCALEFACTOR .0001f
#define SMALLDISTANCE .001f
#define MINNEARMAXSCALEFACTOR .01f
#define SMALLSIZE 1.f


#ifdef TURKOWSKI_REV_2
// Useful ratio of far to near clipping plane distances
#define MIN_Z_RELATIVE_RANGE	1.0e3f					// Even 16-bit Z-bufffers perform well with this range of Z
#define MAX_Z_RELATIVE_RANGE	1.0e6f					// Artifacts probably appear when far/near is greater than this
#endif /* TURKOWSKI_REV_2 */

// tweak on Ken's last tweak; his values were too restrictive -- TP 7/2/03
#define MAX_16_ZREL_RANGE		3.0e3f
#define MAX_24_ZREL_RANGE		7.5e5f					// slightly < MAX_16_ZREL_RANGE * 256
#define MAX_32_ZREL_RANGE		1.9e8f					// slightly < MAX_24_ZREL_RANGE * 256	

#define ADJUSTCLIPFACTOR 0.95f

void CAnmLayer3D::CalcClippingPlanes(double *pNear, double *pFar, int nzbufferbytes)
{
#ifndef TURKOWSKI_REV_2
	// get the bounding box of the scene
	CAnmBoundingBox b;
	GetBoundingBox(&b);

	matrix4 vmat = m_camera->GetMatrix();
	matrix4 ivmat;
	MatrixInvert(ivmat, vmat);

	// Transform the bounding box to view space
	CAnmBoundingBox vb = b.Transform(ivmat);

	// Calc min/max for front/back clipping
	Point3 min, max;
	vb.GetMinMax(&min, &max);
	double minz = min.z;
	double maxz = max.z;
	double zextent = maxz - minz;

	// find a minimum near clipping distance
	double minnear = zextent * MINNEARMINSCALEFACTOR;

	// don't let the near clip go smaller than our small amount
	// N.B.: this should be based on NavigationInfo information
	if (minnear < SMALLSIZE)
		minnear = SMALLSIZE;

	// however, make sure we're not nearclipping a huge chunk of a small world
	if (minnear > zextent * MINNEARMAXSCALEFACTOR)
		minnear = zextent * MINNEARMAXSCALEFACTOR;


	float avatarRadius, visibilityLimit;
	if (m_navigationInfo)
	{
		avatarRadius = m_navigationInfo->GetCollisionRadius();
		visibilityLimit = m_navigationInfo->GetVisibilityLimit();
	}
	else
	{
		avatarRadius = ANMNAVINFO_DEFAULT_AVATAR_COLLISION_DISTANCE;
		visibilityLimit = ANMNAVINFO_DEFAULT_VISIBILITYLIMIT;
	}

	if (minz < minnear)
		minz = minnear;

	if (minz < avatarRadius / 2)
		minz = avatarRadius / 2;

	if (visibilityLimit > FLT_EPSILON && visibilityLimit < maxz)
		maxz = visibilityLimit;

	// Set near and far clip to bounding box values
	*pNear = minz; //  - SMALLDISTANCE;
	*pFar = maxz; // + SMALLDISTANCE;

	// Adjust near if it's less than our minimum
	if (*pNear < minnear)
		*pNear = minnear;

	// Make sure we have *something* between the clipping planes
	if (*pFar < minnear * 2)
		*pFar = minnear * 2;
#else /* TURKOWSKI_REV_2 */
	CAnmBoundingBox	b, vb;
	matrix4			vmat, ivmat;					// View matrix and its inverse
	Point3			boxMin, boxMax;					// min point and max point of box
	float			minSceneZ, maxSceneZ, difSceneZ;	// The minimum and maximum Z of the whole scene in view space
	float			adjMinSceneZ, adjMaxSceneZ;
	float			hither, yon;					// The (local) clipping planes
	float			avatarRadius, visibilityLimit;	// grasp radius and limit of visibility
	float			t;								// temp
	float			maxZRange;						// maximum far/near ratio

	// Get the Z range of the scene in view space
	GetBoundingBox(&b);								// World space bounding box
	vmat = m_camera->GetMatrix();					// View matrix @@@ doesn't this normally map from world space to view space already??
	MatrixInvert(ivmat, vmat);						// @@@ Since upper-left 3x3 of affine view matrix is orthogonal, a simpler inverse could be used
	vb = b.Transform(ivmat);						// Scene box in view space
	vb.GetMinMax(&boxMin, &boxMax);					// We need to do this because they are the only accessors to the box
	minSceneZ = boxMin.z;							// This is probably negative, from within the scene
	maxSceneZ = boxMax.z;							// This is probably positive, unless facing away from the scene
	if (minSceneZ < 0)
		minSceneZ = 0;
	difSceneZ = maxSceneZ - minSceneZ;				// Obviously positive
	adjMinSceneZ = minSceneZ * ADJUSTCLIPFACTOR;	// We might use scene bounds for both front
	adjMaxSceneZ = maxSceneZ / ADJUSTCLIPFACTOR;	// and back, so have these ready
													

	// Get some hints from the system as to good values for hither and yon
	if (m_navigationInfo != NULL) {		// If we are given some idea of the scale, use that information

		avatarRadius    = m_navigationInfo->GetCollisionRadius();
		if (avatarRadius <= 0.0f)											// Make sure that ...
			avatarRadius = ANMNAVINFO_DEFAULT_AVATAR_COLLISION_DISTANCE;	// ... the avatar collision radius is sensible

		visibilityLimit = m_navigationInfo->GetVisibilityLimit();
	}
	else {								// If not, use something else
		avatarRadius    = ANMNAVINFO_DEFAULT_AVATAR_COLLISION_DISTANCE;
		visibilityLimit = ANMNAVINFO_DEFAULT_VISIBILITYLIMIT;
	//	visibilityLimit = sqrt(ANMNAVINFO_DEFAULT_VISIBILITYLIMIT * difSceneZ);	// @@@ Might want to take scene size into account
	}


	// Near plane. Use supplied avatar size and then adjust for optimimum Z resolution
	hither = avatarRadius * 0.5f;										// We cannot focus on things that are closer than half our reach
//	if (hither > (t = difSceneZ / MIN_Z_RELATIVE_RANGE))				// If the scene is small relative to our size, ...
//		hither = t;														// ... set hither closer enough to make good use of Z resolution

	// krv:  Do we need to test if we have overlays?
	if (hither < adjMinSceneZ)											// Push hither out if the scene is in front of us
		hither = adjMinSceneZ;

	// Far plane. Use supplied visibility limit and then adjust for optimum Z resolution
	if (visibilityLimit > 0.f)											// Use supplied vis limit if it's valid
		yon = visibilityLimit;
	else
		yon = maxSceneZ;												// ... are sensible

	if (yon > adjMaxSceneZ)												// Bring yon to the scene back if possible
		yon = adjMaxSceneZ;

	// Determine the resolvability of the Z-buffer
	switch (nzbufferbytes) {
		default:
		case 2:	maxZRange = MAX_16_ZREL_RANGE; break;
		case 3:	maxZRange = MAX_24_ZREL_RANGE; break;
		case 4:	maxZRange = MAX_32_ZREL_RANGE; break;
	}

	// Now compare hither and yon to reduce Z artifacts based on roundoff.
	// We do this by bringing in the far plane. We might clip geometry but it's
	// still better than nasty Z artifacts
	if (yon > ((t = hither * maxZRange)))								// Check to make sure that the clipping planes are not too wacky...
		yon = t;														// ... and make it sensible

	// Do something
	if (yon <= hither)
		yon = hither + SMALLDISTANCE;
	
	*pNear = hither;
	*pFar  = yon;
#endif /* TURKOWSKI_REV_2 */
}


// Accessors

void CAnmLayer3D::SetAutoOrient(Boolean autoOrient)
{
	m_autoOrient = autoOrient;
	SetStateDirty();
	CallCallbacks(CALLBACKID(CAnmLayer3D, autoOrient_changed), &autoOrient);
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmLayer3D, children, CAnmLayer);
DEFINE_INHERITED_VALUE(CAnmLayer3D, transparency, CAnmLayer);
DEFINE_INHERITED_VALUE(CAnmLayer3D, type, CAnmLayer);

DEFINE_INHERITED_METHOD(CAnmLayer3D, addChildren, CAnmLayer);
DEFINE_INHERITED_METHOD(CAnmLayer3D, removeChildren, CAnmLayer);
DEFINE_INHERITED_METHOD(CAnmLayer3D, set_children, CAnmLayer);
DEFINE_INHERITED_METHOD(CAnmLayer3D, set_transparency, CAnmLayer);

DEFINE_INHERITED_CALLBACK(CAnmLayer3D, children_changed, CAnmLayer);
DEFINE_INHERITED_CALLBACK(CAnmLayer3D, transparency_changed, CAnmLayer);

DEFINE_VALUE(CAnmLayer3D, autoOrient, eValueBoolean, Boolean, GetAutoOrient, SetAutoOrient);
DEFINE_METHOD(CAnmLayer3D, set_autoOrient, eValueBoolean, Boolean, SetAutoOrient);
DEFINE_CALLBACK(CAnmLayer3D, autoOrient_changed, eValueBoolean);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmLayer3D)
VALUEID(CAnmLayer3D, children),
VALUEID(CAnmLayer3D, autoOrient),
VALUEID(CAnmLayer3D, transparency),
VALUEID(CAnmLayer3D, type),
METHODID(CAnmLayer3D, addChildren),
METHODID(CAnmLayer3D, removeChildren),
METHODID(CAnmLayer3D, set_children),
METHODID(CAnmLayer3D, set_autoOrient),
METHODID(CAnmLayer3D, set_transparency),
CALLBACKID(CAnmLayer3D, children_changed),
CALLBACKID(CAnmLayer3D, autoOrient_changed),
CALLBACKID(CAnmLayer3D, transparency_changed),


END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmLayer3D, CAnmLayer);
