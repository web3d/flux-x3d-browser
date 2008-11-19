/********************************************************************************
 * Flux
 *
 * File: anmviewpoint.cpp
 * Description: Viewpoint class - user-defined camera
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
#include "anmviewpoint.h"
#include "anmcamera.h"
#include "anmlayer.h"
#include "anmtransform.h"
#include "anmupdatevisitor.h"
#include "anmviewer.h"
#include "anmworld.h"
#include "anmMatrixInvert.h"

CAnmViewpoint::CAnmViewpoint() : 
	CAnmBindableNode(),
	m_position (ANMVIEWPOINT_DEFAULT_POSITION),
	m_orientation (ANMVIEWPOINT_DEFAULT_ORIENTATION),
	m_fieldOfView (ANMVIEWPOINT_DEFAULT_FIELDOFVIEW),
	m_centerOfRotation(ANMVIEWPOINT_DEFAULT_CENTEROFROTATION),
	m_jump(ANMVIEWPOINT_DEFAULT_JUMP)
{
	m_userpos = point3D::Zero;		// no position offset
	m_userdir = point3D::k;			// default: Z is in
	m_userup = point3D::j;			// default: Y is up

	m_worldmatrix = matrix4::Identity;
	m_modelmatrix = matrix4::Identity;
	m_description = new CAnmString(ANMVIEWPOINT_DEFAULT_DESCRIPTION);
}

CAnmViewpoint::~CAnmViewpoint()
{
	SafeUnRef(m_description);
}

void CAnmViewpoint::Realize()
{
	CAnmLayer *pLayer = GetLayer();
	assert(pLayer);

	if( pLayer ) {
		pLayer->AddViewpoint(this);
	}

	CAnmBindableNode::Realize();
}


void CAnmViewpoint::Update()
{
	// if (!IsDirty())
	//	return;

	// Set the orientation. 
	// ANIMA viewpoint orientation is rotation about default up and direction vectors:
	// y is up, direction is toward the origin from 0 0 -1 (RHS)

	Point3D up(0, 1, 0);
	Point3D direction(0, 0, 1);

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	if (pApp->GetOptions(eAnmOptionsCoordinateSystem) == eAnmCoordinatesRightHanded)
		direction = Point3D(0, 0, -1);

	matrix4D mat;


	Point3D orientationVec( m_orientation.vec );

	// Rotate the direction and up vectors around the orientation vector
	mat.ToAxisAngle(orientationVec, ( double ) m_orientation.theta);

	up = mat * up;
	direction = mat * direction;
	up.Normalize();
	direction.Normalize();

	Point3D pos( m_position );

	// Now make the local matrix for the position, orientation values
	m_modelmatrix.ToObjectLookAt( pos, pos + direction, up);

	// Now combine model and world into a new world matrix
	matrix4D combined = m_modelmatrix * m_worldmatrix;

	// Translate user-offset position, direction and up into world values
	Point3D loc;
	loc = combined * m_userpos;


	direction = combined * m_userdir;
	up = combined * m_userup;

	// Pesky origin stuff
	Point3D org = combined * Point3D(0, 0, 0);
	direction = direction - org;
	up = up - org;

	direction.Normalize();
	up.Normalize();

	if (m_isBound)
	{
		CAnmLayer *pLayer = GetLayer();
		assert(pLayer);

		if( pLayer ) {

			CAnmCamera *pCamera = pLayer->GetCamera();
			assert(pCamera);

			pCamera->SetFieldOfView(m_fieldOfView);

			// If we're animating transitions, let the viewer class take care of camera position;
			// otherwise we do it directly here
			CAnmViewer *pViewer = pApp->GetViewer();
			assert(pViewer);


			Point3 loc_f( loc );
			Point3 direction_f( direction );
			Point3 up_f( up );

			if (pViewer->AnimateTransitions() && pViewer->SetAnimationTarget(loc_f, direction_f, up_f))
			{
			}
			else
			{


				pCamera->SetPosition(loc_f);
				pCamera->SetOrientation(direction_f, up_f);
			}
		}
	}
// fclose( fp );

	// clear the dirty
	ClearStateDirty();
}

void CAnmViewpoint::Traverse(CAnmUpdateVisitor *pVisitor)
{
	matrix4 mat;

	pVisitor->GetWorldMatrix(&mat);

	m_worldmatrix = mat;

	CAnmNode::Traverse(pVisitor);
}

void CAnmViewpoint::Restore()
{
	m_userpos = point3D::Zero;		// no position offset
	m_userdir = point3D::k;			// default: Z is in
	m_userup = point3D::j;			// default: Y is up

	CAnmWorld *pWorld = GetWorld();
//	assert(pWorld);
	ABORT_IF_WORLD_IS_NULL

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	// if (pApp->GetOptions(eAnmOptionsCoordinateSystem) == eAnmCoordinatesRightHanded)
	//	m_userdir = Point3(0, 0, -1);

}

void CAnmViewpoint::SaveUserOffsets(const Point3 &wcpos, const Point3 &wcdir, const Point3 &wcup)
{
	matrix4D modelworld = GetCombinedModelWorldMatrix();
	matrix4D invmw;

#ifdef DOMATRIXTESTS
	matrix4 matFlt( modelworld );
	matrix4 invmwFly;
	point3 wcposFlt( wcpos );
#endif



	MatrixInvert(invmw, modelworld);

	// Convert our relative position, direction and up vectors into viewpoint's space
	Point3D org = invmw * point3D::Zero;
	Point3D imwpos = invmw * Point3D( wcpos );
	Point3D imwdir = invmw * Point3D( wcdir );
	Point3D imwup = invmw * Point3D( wcup );


#ifdef DOMATRIXTESTS


	double dStartTime = (double)clock();
	for( int iii=0; iii<100000; iii++ ) {
		matrix4D mat( modelworld );
		matrix4D mat2;
		MatrixInvert(mat2, mat);
	}
	double dEndTimeTime = (double)clock();

	double delta = ( dEndTimeTime - dStartTime ) / ( double )CLOCKS_PER_SEC;

	double dStartTimeF = (double)clock();
	for(  iii=0; iii<100000; iii++ ) {
		matrix4 mat( matFlt );
		matrix4 mat2;
		MatrixInvert(mat2, mat);
	}
	double dEndTimeTimeF = (double)clock();

	double deltaF = ( dEndTimeTimeF - dStartTimeF ) / ( double )CLOCKS_PER_SEC;


	Point3D pointRounbdTrip = modelworld * imwpos;

	Point3D errvec = pointRounbdTrip - Point3D( wcpos );
	double errmag = errvec.Mag();
	if( errmag > 0.00001 ) {
		int jjj=0;
	}
 

	// do float test:


	MatrixInvert(invmwFly, matFlt);
	Point3 imwposFlt = invmwFly * Point3( wcposFlt );

	Point3 pointRounbdTripFlt = matFlt * imwposFlt;

	Point3 errvecFlt = pointRounbdTripFlt - Point3( wcposFlt );
	errmag = errvec.Mag();
	if( errmag > 0.00001 ) {
		int jjj=0;
	}


#endif


	// Pesky origin
	imwdir = imwdir - org;
	imwup = imwup - org;

	// Normalize too
	imwdir.Normalize();
	imwup.Normalize();

	SetUserOffsets(imwpos, imwdir, imwup);
}

void CAnmViewpoint::SaveUserOffsets()
{
	CAnmLayer *pLayer = GetLayer();
	assert(pLayer);

	if( pLayer ) {

		CAnmCamera *pCamera = pLayer->GetCamera();
		assert(pCamera);

		Point3 wcpos, wcdir, wcup;

		wcpos = pCamera->GetPosition();
		pCamera->GetOrientation(&wcdir, &wcup);

		SaveUserOffsets(wcpos, wcdir, wcup);
	}
}


void CAnmViewpoint::SetBind(Boolean bindFlag)
{
	// N.B.: this is to deal with weird jump == FALSE situation; we want the user offset
	// values to be set up so that the camera doesn't move even though we've switched to
	// a new viewpoint
	if (bindFlag)
	{
		if (m_jump)
			Restore();
		else
			SaveUserOffsets();
	}

	// Chain down for the basic binding behavior
	CAnmBindableNode::SetBind(bindFlag);
}


void CAnmViewpoint::SetFieldOfView(float fov)
{
	m_fieldOfView = fov;

	SetStateDirty();
}


void CAnmViewpoint::SetJump(Boolean jump)
{
	m_jump = jump;

	SetStateDirty();
}

void CAnmViewpoint::SetOrientation(Rotation r)
{
	m_orientation = r;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmViewpoint, orientation_changed), &r);
}


void CAnmViewpoint::SetPosition(Point3 p)
{
	m_position = p;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmViewpoint, position_changed), &p);
}

void CAnmViewpoint::SetDescription(String description)
{
	SafeUnRef(m_description);
	m_description = description;
	m_description->Ref();

	SetStateDirty();
}

void CAnmViewpoint::SetCenterOfRotation(Point3 p)
{
	m_centerOfRotation = p;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmViewpoint, centerOfRotation_changed), &p);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmViewpoint, fieldOfView, eValueFloat, Float, GetFieldOfView, SetFieldOfView);
DEFINE_VALUE(CAnmViewpoint, jump, eValueBoolean, Boolean, GetJump, SetJump);
DEFINE_VALUE(CAnmViewpoint, orientation, eValueRotation, Rotation, GetOrientation, SetOrientation);
DEFINE_VALUE(CAnmViewpoint, position, eValuePoint3, Point3, GetPosition, SetPosition);
DEFINE_VALUE(CAnmViewpoint, description, eValueString, String, GetDescription, SetDescription);
DEFINE_VALUE(CAnmViewpoint, centerOfRotation, eValuePoint3, Point3, GetCenterOfRotation, SetCenterOfRotation);

DEFINE_INHERITED_METHOD(CAnmViewpoint, set_bind, CAnmBindableNode);
DEFINE_METHOD(CAnmViewpoint, set_fieldOfView, eValueFloat, Float, SetFieldOfView);
DEFINE_METHOD(CAnmViewpoint, set_jump, eValueBoolean, Boolean, SetJump);
DEFINE_METHOD(CAnmViewpoint, set_orientation, eValueRotation, Rotation, SetOrientation);
DEFINE_METHOD(CAnmViewpoint, set_position, eValuePoint3, Point3, SetPosition);
DEFINE_METHOD(CAnmViewpoint, set_description, eValueString, String, SetDescription);
DEFINE_METHOD(CAnmViewpoint, set_centerOfRotation, eValuePoint3, Point3, SetCenterOfRotation);

DEFINE_INHERITED_CALLBACK(CAnmViewpoint, bindTime, CAnmBindableNode);
DEFINE_CALLBACK(CAnmViewpoint, fieldOfView_changed, eValueFloat);
DEFINE_INHERITED_CALLBACK(CAnmViewpoint, isBound, CAnmBindableNode);
DEFINE_CALLBACK(CAnmViewpoint, jump_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmViewpoint, orientation_changed, eValueRotation);
DEFINE_CALLBACK(CAnmViewpoint, position_changed, eValuePoint3);
DEFINE_CALLBACK(CAnmViewpoint, description_changed, eValueString);
DEFINE_CALLBACK(CAnmViewpoint, centerOfRotation_changed, eValuePoint3);


// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmViewpoint)
VALUEID(CAnmViewpoint, fieldOfView),
VALUEID(CAnmViewpoint, jump),
VALUEID(CAnmViewpoint, orientation),
VALUEID(CAnmViewpoint, position),
VALUEID(CAnmViewpoint, description),
VALUEID(CAnmViewpoint, centerOfRotation),

METHODID(CAnmViewpoint, set_bind),
METHODID(CAnmViewpoint, set_fieldOfView),
METHODID(CAnmViewpoint, set_jump),
METHODID(CAnmViewpoint, set_orientation),
METHODID(CAnmViewpoint, set_position),
METHODID(CAnmViewpoint, set_description),
METHODID(CAnmViewpoint, set_centerOfRotation),

CALLBACKID(CAnmViewpoint, bindTime),
CALLBACKID(CAnmViewpoint, fieldOfView_changed),
CALLBACKID(CAnmViewpoint, isBound),
CALLBACKID(CAnmViewpoint, jump_changed),
CALLBACKID(CAnmViewpoint, orientation_changed),
CALLBACKID(CAnmViewpoint, position_changed),
CALLBACKID(CAnmViewpoint, description_changed),
CALLBACKID(CAnmViewpoint, centerOfRotation_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmViewpoint, CAnmBindableNode);

