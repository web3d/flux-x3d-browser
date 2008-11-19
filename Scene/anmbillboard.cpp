/********************************************************************************
 * Flux
 *
 * File: anmbillboard.cpp
 * Description: Billboard node class
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

#include "anmbillboard.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmlayer.h"
#include "anmproximitysensor.h"
#include "anmupdatevisitor.h"
#include "anmviewport.h"
#include "anmvisitor.h"
#include "anmworld.h"
#include "anmMatrixInvert.h"


CAnmBillboard::CAnmBillboard() : CAnmGroup(),
	m_axisOfRotation(ANMBILLBOARD_DEFAULT_AXISOFROTATION),
	m_bboxCenter(ANMBILLBOARD_DEFAULT_BBOXCENTER),
	m_bboxSize(ANMBILLBOARD_DEFAULT_BBOXSIZE)
{
	m_proxSensor = new CAnmProximitySensor;
	m_proxSensor->SetOwner(this);
	m_proxSensor->AddCallback(CALLBACKID(CAnmProximitySensor, position_changed), CameraPositionChangedCallback, this);
	m_proxSensor->AddCallback(CALLBACKID(CAnmProximitySensor, orientation_changed), CameraOrientationChangedCallback, this);
	m_proxSensor->SetSize(Point3(FLT_MAX, FLT_MAX, FLT_MAX));
}

CAnmBillboard::~CAnmBillboard()
{
	SafeUnRef(m_proxSensor);
}

void CAnmBillboard::Realize()
{
	CAnmGroup::Realize();
	m_proxSensor->Realize();
}


inline void RotateTo(matrix4 &mat, Point3 from, Point3 to)
{
	float dot = from * to;
	float theta = acos(dot);

	if (fabs(theta) < EPSILON)
	{
		mat = matrix4::Identity;
	}
	else
	{
		Point3 axis = from ^ to;
		mat.ToAxisAngle(axis, theta);
	}
}

// N.B.: this one assumes right-handed
#if 0 // $$$ TP try this
inline void ViewerAlign(matrix4 &mat, const matrix4 &worldmat, const matrix4 &cameramat)
{
	// Make axes and origin
	Point3 xAxis(1, 0, 0);
	Point3 yAxis(0, 1, 0);
	Point3 zAxis(0, 0, 1);
	Point3 origin(0, 0, 0);		// get all those pesky translations out

	// Get origin, axes in world coords
	xAxis = worldmat * xAxis;
	yAxis = worldmat * yAxis;
	zAxis = worldmat * zAxis;
	origin = worldmat * origin;

	// Transform them to camera space
	matrix4 icam;
	MatrixInvert(icam, (matrix4 &) cameramat);
	xAxis = icam * xAxis;
	yAxis = icam * yAxis;
	zAxis = icam * zAxis;
	origin = icam * origin;

	// Pull out the translations
	xAxis = xAxis - origin;
	yAxis = yAxis - origin;
	zAxis = zAxis - origin;

	// Normalize everything
	xAxis.Normalize();
	yAxis.Normalize();
	zAxis.Normalize();

	// Combine the 3 axes into a rotation matrix
	matrix4 mat2;
	mat2._11 = xAxis.x;
	mat2._12 = xAxis.y;
	mat2._13 = xAxis.z;
	mat2._14 = 0.f;
	mat2._21 = yAxis.x;
	mat2._22 = yAxis.y;
	mat2._23 = yAxis.z;
	mat2._24 = 0.f;
	mat2._31 = zAxis.x;
	mat2._32 = zAxis.y;
	mat2._33 = zAxis.z;
	mat2._34 = 0.f;
	mat2._41 = 0.f;
	mat2._42 = 0.f;
	mat2._43 = 0.f;
	mat2._44 = 1.f;

	// Invert camera-space matrix to get new world matrix
	MatrixInvert(mat, mat2);
}
#else // $$$ TP try this
inline void ViewerAlign(matrix4 &mat, const matrix4 &worldmat, CAnmCamera *pCamera)
{
	// Get up vector from camera (world space)
	Point3 cameradir, cameraup;
	pCamera->GetOrientation(&cameradir, &cameraup);

	// Get camera position (world space)
	Point3 camerapos;
	pCamera->GetPosition(&camerapos);

	// Translate camera pos, up into model space
	matrix4 w2m;
	MatrixInvert(w2m, (matrix4 &) worldmat);

	Point3 mcameradir = w2m * cameradir;
	Point3 mcameraup = w2m * cameraup;

	// Substract out pesky origin
	mcameradir = mcameradir - (w2m * Point3::Zero);
	mcameraup = mcameraup - (w2m * Point3::Zero);

	// Now we have camera dir and up in model space
	// BB2V is opposite camera direction
	Point3 bb2v = -mcameradir;
	bb2v.Normalize();

	float theta = acos(bb2v * mcameraup);

	// Point at camera but with same up vector as camera (unless the two vectors parallel)
	if (fabs(theta) > EPSILON)
	{
		mat.ToObjectLookAt(Point3::Zero, bb2v, mcameraup);
	}
	else
	{
		mat = matrix4::Identity;
	}
}
#endif // $$$ TP try this

void CAnmBillboard::CalcMatrix(const matrix4 &worldmat, matrix4 &newmat)
{
	// get the camera
	CAnmLayer *pLayer = GetLayer();
	assert(pLayer);

	CAnmCamera *pCamera = pLayer->GetCamera();
	assert(pCamera);

	// If axisOfRotation is small, or is our sentinel value for viewer-aligned,
	// then do a viewer-aligned billboard
	if (m_axisOfRotation == Point3::Zero)
	{
#if 0
		matrix4 cmat = pCamera->GetMatrix();
		ViewerAlign(newmat, worldmat, cmat);
#else
		ViewerAlign(newmat, worldmat, pCamera);
#endif
	}
	else
	{
		// Get camera pos in world coordinates
		Point3 cameraPos = pCamera->GetPosition();

		// Compute inverse matrix for local coords
		matrix4 imat;
		MatrixInvert(imat, (matrix4 &) worldmat);

		// Billboard-to-viewer line (= cameraPos - 0 in local coord system)
		Point3 bb2v =  imat * cameraPos;
		bb2v.Normalize();

		// normalize the axis of rotation, just to be sure
		Point3 rotAxis = m_axisOfRotation;
		rotAxis.Normalize();

		// Compute normal to plane made between our axis of rotation and bb2v
		Point3 norm =  rotAxis ^ bb2v;

		// Only do rotation if we actually have a plane
		if ((norm * norm) >= EPSILON)
		{
			norm.Normalize();

			// Now we compute the projection of our out-looking z axis onto that plane
			float theta;

			// make our out-looking z axis
			CAnmWorld *pWorld = GetWorld();
//			assert(pWorld);
			ABORT_IF_WORLD_IS_NULL

			Point3 zaxis(0, 0, -1);
			cApplication *pApp = pWorld->GetApp();
			if (pApp->GetOptions(eAnmOptionsCoordinateSystem) == eAnmCoordinatesRightHanded)
				zaxis = Point3(0, 0, 1);

			// Rotate the local Z-axis into the computed plane
			// Don't mess with small rotations
			Point3 temp = zaxis - rotAxis;
			if ((temp * temp) < EPSILON)
			{
				theta = 0.f;
			}
			else
			{
				// Get the projection of our rotation axis into the plane
				Point3 inplane, outplane;

				outplane = norm * (zaxis * norm);
				inplane = zaxis - outplane;

				inplane.Normalize();

				// make sure the projection vector is positive
				if ((inplane * bb2v) < 0.f)
					inplane = inplane * -1.f;

				// Calculate angle between the projection and our z axis

				float dot = zaxis * inplane;
				dot = ClampValue(-1.f, dot, 1.f);

				theta = acos(dot);

				Point3 dir = zaxis ^ inplane;

				// if our direction is pointing away from the viewer, flip the angle
				if ((rotAxis * dir) < 0.f)
					theta = -theta;
			}

			// Do the rotation
			newmat.ToAxisAngle(rotAxis, theta);

		}
		else
			newmat = matrix4::Identity;
	}
}

void CAnmBillboard::CameraPositionChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData)
{
	CAnmProximitySensor *pPS = (CAnmProximitySensor *) sourceObject;
	CAnmBillboard *pBB = (CAnmBillboard *) userData;
	Point3 *pp =  (Point3 *) callData;

	pBB->HandleCameraPositionChanged(pp);
}

void CAnmBillboard::CameraOrientationChangedCallback(CAnmObject *sourceObject, CLASSMEMBERID reason, void *callData, void *userData)
{
	CAnmProximitySensor *pPS = (CAnmProximitySensor *) sourceObject;
	CAnmBillboard *pBB = (CAnmBillboard *) userData;
	Rotation *rp =  (Rotation *) callData;

	pBB->HandleCameraOrientationChanged(rp);
}


void CAnmBillboard::HandleCameraPositionChanged(Point3 *pCameraPosition)
{
	SetMatrixDirty();
}

void CAnmBillboard::HandleCameraOrientationChanged(Rotation *pCameraOrientation)
{
	// Camera orientation only matters if we're viewer-aligned
	if (m_axisOfRotation == Point3::Zero)
		SetMatrixDirty();
}

void CAnmBillboard::Traverse(CAnmUpdateVisitor *pVisitor)
{
	// N.B.: traverse my children if either we have to recalc matrices and instances for
	// everyone, or because some child has requested updated matrices or state
	if (TestDirtyBits(ANMGROUPDIRTY_CHECKTRAVERSECHILDREN))
	{
		// Make sure to update the proximity sensor if we're re-traversing
		m_proxSensor->Traverse(pVisitor);

		// push transform state
		matrix4 saved, newmat, current;
		pVisitor->GetWorldMatrix(&saved);

		// calculate the new world matrix
		CalcMatrix(saved, newmat);
		current = newmat * saved;

		// set new transform state in visitor
		pVisitor->SetWorldMatrix(&current);

		// call Group class to traverse children
		CAnmGroup::Traverse(pVisitor);

		// pop transform state
		pVisitor->SetWorldMatrix(&saved);
	}
	else
	{
		int debug = 1;
	}
}

void CAnmBillboard::PreTraverse()
{
	CAnmGroup::PreTraverse();

	m_proxSensor->PreTraverse();
}

void CAnmBillboard::PostTraverse()
{
	CAnmGroup::PostTraverse();

	m_proxSensor->PostTraverse();
}

// Accessors
void CAnmBillboard::SetAxisOfRotation(point3 p)
{
	m_axisOfRotation = p;

	CallCallbacks(CALLBACKID(CAnmBillboard, axisOfRotation_changed), &p);
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmBillboard, axisOfRotation, eValuePoint3, Point3, GetAxisOfRotation, SetAxisOfRotation);
DEFINE_INHERITED_VALUE(CAnmBillboard, bboxCenter, CAnmGroup);
DEFINE_INHERITED_VALUE(CAnmBillboard, bboxSize, CAnmGroup);
DEFINE_INHERITED_VALUE(CAnmBillboard, children, CAnmGroup);

DEFINE_INHERITED_METHOD(CAnmBillboard, addChildren, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmBillboard, removeChildren, CAnmGroup);
DEFINE_METHOD(CAnmBillboard, set_axisOfRotation, eValuePoint3, Point3, SetAxisOfRotation);
DEFINE_INHERITED_METHOD(CAnmBillboard, set_children, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmBillboard, set_bboxCenter, CAnmGroup);
DEFINE_INHERITED_METHOD(CAnmBillboard, set_bboxSize, CAnmGroup);

DEFINE_CALLBACK(CAnmBillboard, axisOfRotation_changed, eValuePoint3);
DEFINE_INHERITED_CALLBACK(CAnmBillboard, bboxCenter_changed, CAnmGroup);
DEFINE_INHERITED_CALLBACK(CAnmBillboard, bboxSize_changed, CAnmGroup);
DEFINE_INHERITED_CALLBACK(CAnmBillboard, children_changed, CAnmGroup);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmBillboard)
VALUEID(CAnmBillboard, axisOfRotation),
VALUEID(CAnmBillboard, bboxCenter),
VALUEID(CAnmBillboard, bboxSize),
VALUEID(CAnmBillboard, children),
METHODID(CAnmBillboard, addChildren),
METHODID(CAnmBillboard, removeChildren),
METHODID(CAnmBillboard, set_axisOfRotation),
METHODID(CAnmBillboard, set_bboxCenter),
METHODID(CAnmBillboard, set_bboxSize),
METHODID(CAnmBillboard, set_children),
CALLBACKID(CAnmBillboard, axisOfRotation_changed),
CALLBACKID(CAnmBillboard, bboxCenter_changed),
CALLBACKID(CAnmBillboard, bboxSize_changed),
CALLBACKID(CAnmBillboard, children_changed),

END_DEFINE_MEMBERS


DEFINE_CLASS(CAnmBillboard, CAnmGroup);

