/********************************************************************************
 * Flux
 *
 * File: anmviewer.cpp
 * Description: Viewer class - viewing and navigation
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
#include "anmviewer.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmfrustum.h"
#include "anmviewport.h"
#include "anmkeyboard.h"
#include "anmnavigationinfo.h"
#include "anmquaternion.h"
#include "anmpickmanager.h"
#include "anmviewpoint.h"
#include "anmworld.h"

#include "inputlayer.h"
#include "keyboard.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

CAnmViewer::CAnmViewer(cApplication *pApp, eNavigationMode navMode)
{
	assert(pApp);
	m_app = pApp;
	
	m_world = pApp->GetWorld();
	assert(m_world);

	assert(pApp->GetInput());
	m_keyboard = pApp->GetInput()->GetKeyboard();
	assert(m_keyboard);

	m_viewpoint = NULL;

	m_mousepoint = Point2(-1, -1);
	m_winsize = Point2(0, 0);

	m_speed = ANMNAVINFO_DEFAULT_SPEED;
	m_userspeed = eUserSpeedMedium;

	m_lastang = 0.0;
	m_lastxdisp = 0.0;
	m_lastydisp = 0.0;
	m_lastXangle = 0.0;
	m_lastYangle = 0.0;

	m_navigationMode = navMode;
	m_transitionMode = eNavigationTransitionLinear;
	m_autoRotate = false;
	m_panning = false;
	m_tilting = false;
	m_zooming = false;
	m_orbiting = false;

	m_viewscale.Assign(1, 1, 1);

	m_collide = true;
	m_collisionRadius = ANMNAVINFO_DEFAULT_AVATAR_COLLISION_DISTANCE;
	m_avatarHeight = ANMNAVINFO_DEFAULT_AVATAR_HEIGHT_ABOVE_TERRAIN;
	m_maxStepHeight = ANMNAVINFO_DEFAULT_AVATAR_TALLEST_OBJECT_HEIGHT;

	m_gravity = (navMode == eNavigationModeWalk);
	m_applyGravity = false;

	m_prevnavtime = ANMTIME_BADTIMEVALUE;

	m_lookAt = false;
	m_useExamineCenter = false;
	m_centerOfRotation.Assign(0, 0, 0);
	m_examineCenter.Assign(0, 0, 0);

	m_seekMode = ANMVIEWER_SEEKMODENONE;

	m_viewpointAnimation = false;
	m_mousemoved = false;

	Point3D zaxis = Point3D::k;
	m_walkdir = -zaxis;						// N.B.: RH
	m_walkup = Point3D::j;

	m_bVirginGravity = true;
}

CAnmViewer::~CAnmViewer()
{
	SafeUnRef(m_viewpoint);
	m_app = NULL;
}

void CAnmViewer::SetNavigationMode(eNavigationMode navMode)
{
	m_navigationMode = navMode;

	m_gravity = (navMode == eNavigationModeWalk);

	m_app->AdviseNavigationModeChange((int) navMode);
}

void CAnmViewer::SetTransitionMode(eNavigationTransitionMode transitionMode)
{
	m_transitionMode = transitionMode;
}

void CAnmViewer::SetSpeed(double speed)
{
	m_speed = speed;
}

void CAnmViewer::SetViewScale(Point3D scale)
{
	m_viewscale = scale;
}

void CAnmViewer::SetCollide(Boolean collide)
{
	m_collide = collide;
}

void CAnmViewer::SetCollisionRadius(double collisionRadius)
{
	m_collisionRadius = collisionRadius;
}

void CAnmViewer::SetAvatarHeight (double avatarHeight)
{
	m_avatarHeight = avatarHeight;
}

void CAnmViewer::SetMaxStepHeight (double maxStepHeight)
{
	m_maxStepHeight = maxStepHeight;
}

void CAnmViewer::SetCenterOfRotation(Point3D p)
{
	if (!(m_centerOfRotation == p))
	{
		m_centerOfRotation = p;
		if (m_lookAt)
			m_examineCenter = m_centerOfRotation;
	}
}

void CAnmViewer::SetLookAt(bool lookAt)
{
	m_lookAt = lookAt;
	m_useExamineCenter = false;
}

int CAnmViewer::PollNavKey()
{
	if (m_keyboard->Poll(DIK_LEFT))
		return DIK_LEFT;
	else if (m_keyboard->Poll(DIK_RIGHT))
		return DIK_RIGHT;
	else if (m_keyboard->Poll(DIK_UP))
		return DIK_UP;
	else if (m_keyboard->Poll(DIK_DOWN))
		return DIK_DOWN;
	else
		return -1;
}

void CAnmViewer::Poll(bool pollMouse, bool pollKeyboard)
{
	if (Animating())
		RunAnimation();

	if (m_navigationMode == eNavigationModeNone)
		return;

	BOOL grabbed;

	// if there's already a grab going on, bail out
	CAnmObject *pGrabbed = m_app->GetExclusiveGrab();
	if (!pGrabbed || pGrabbed == this)
	{
		grabbed = (pGrabbed == this);

		// poll key state from the device
		int navKey = -1;
		if (pollKeyboard && m_app->HasFocus())
			navKey = PollNavKey();

		// get mouse state from the app
		int mousex, mousey, mousedown;
		m_app->GetMouseInfo(&mousex, &mousey, &mousedown);

		// check for escapes
		if (m_seekMode && m_keyboard->Poll(DIK_ESCAPE))
		{
			SetSeekMode(ANMVIEWER_SEEKMODENONE);
			m_app->SetDefaultCursor();
		}

		if (m_seekMode)
			m_app->SetCursor(cApplication::seekCursor);

		if (grabbed)
		{
			if (pollMouse && mousedown)
			{
				HandleMouseMove(mousex, mousey);
			}
			else if (navKey == -1)
			{
				m_app->ReleaseExclusiveGrab();
				EndNavigate();
			}
		}
		else
		{
			if (mousedown)
			{
				if (m_seekMode)
				{
					// N.B.: seek mode overrides pollMouse flag; we always do it
					SeekTo(mousex, mousey);
					SetSeekMode(m_seekMode &~ ANMVIEWER_SEEKMODESEEK);
					if (!m_seekMode)
						m_app->SetDefaultCursor();
				}
				else if (pollMouse)
				{
					m_app->SetExclusiveGrab(this);
					BeginNavigate(mousedown, mousex, mousey);
				}
			}
			else if (pollKeyboard && navKey != -1)
			{
				m_app->SetExclusiveGrab(this);
			}
		}

		if (navKey != -1)
			HandleKeyboard(navKey);
	}

	if (m_gravity && m_applyGravity)
		ApplyGravity();

	// Save current time
	m_prevnavtime = GetCurrentTime();
}

void CAnmViewer::BeginNavigate(int mousedown, int mousex, int mousey)
{
	m_lastang = 0.0;
	m_lastxdisp = 0.0;
	m_lastydisp = 0.0;

	if (mousedown & ANM_RBUTTON)
		m_panning = true;
	else if (mousedown & ANM_MBUTTON)
	{
		if (m_navigationMode == eNavigationModeExamine)
			m_zooming = true;
		else
			m_tilting = true;
	}
	else if (m_navigationMode != eNavigationModeExamine && 
		m_keyboard->Poll(DIK_LCONTROL) || m_keyboard->Poll(DIK_RCONTROL))
	{
#if 0
		// N.B.: this doesn't really work yet.
		GetFrustumCenter(m_orbitcenter);
		m_orbiting = true;
#endif
	}


	m_mousepoint.x = mousex;
	m_mousepoint.y = mousey;
	m_initialMousePoint = m_mousepoint;

	m_savedviewpoint = m_viewpoint;
	m_savedpos = m_camera->GetPosition();

	GetCameraOrientation( m_saveddir, m_savedup);

	cWindow *pWin = m_app->MainWindow();
	assert(pWin);

	m_winsize.x = pWin->GetWidth();
	m_winsize.y = pWin->GetHeight();

	if (m_navigationMode == eNavigationModeExamine)
		ComputeExamineCenter();

}

void CAnmViewer::EndNavigate()
{
	m_lastang = 0.0;
	m_lastxdisp = 0.0;
	m_lastydisp = 0.0;
	m_lastXangle = 0.0;
	m_lastYangle = 0.0;
	m_mousepoint.x = -1;
	m_mousepoint.y = -1;
	m_panning = false;
	m_tilting = false;
	m_zooming = false;
	m_orbiting = false;

	if (m_mousemoved)
		SaveCameraPos();

	m_mousemoved = false;
}

void CAnmViewer::SaveCameraPos()
{
	Point3D newpos = m_camera->GetPosition();

	Point3D newdir;
	Point3D newup;
	GetCameraOrientation( newdir, newup );

	sAnmCameraChange camchange(m_savedviewpoint, m_savedpos, m_saveddir, m_savedup,
		newpos, newdir, newup);

	m_app->AdviseCameraChange(&camchange);

	m_savedviewpoint = m_viewpoint;
	m_savedpos = newpos;
	m_saveddir = newdir;
	m_savedup = newup;
}

void CAnmViewer::SetViewpointUserOffsets(const Point3D &pos, const Point3D &dir, const Point3D &up)
{
	if (m_viewpoint)
		m_viewpoint->SaveUserOffsets(pos, dir, up);
}

void CAnmViewer::AdviseViewpointBound(CAnmViewpoint *pViewpoint)
{
//	assert(pViewpoint);

	if( pViewpoint ) {
		CAnmViewpoint *pOldViewpoint = m_viewpoint;

		SafeUnRef(m_viewpoint);

		m_viewpoint = pViewpoint;
		m_viewpoint->Ref();

		// Try animation thing
		if (pOldViewpoint && (pOldViewpoint != pViewpoint) && AnimateTransitions() && pViewpoint->GetJump())
			SetupAnimation();

		m_applyGravity = false;
		m_useExamineCenter = false;

		GetViewpointDefaultOrientation(m_walkdir, m_walkup);
	}
}

void CAnmViewer::HandleMouseMove(int mousex, int mousey)
{
	switch ( m_navigationMode )
	{
		case eNavigationModeWalk :
			MouseFly(mousex, mousey);
			break;

		case eNavigationModeFly :
			MouseFly(mousex, mousey);
			break;

		case eNavigationModeExamine :
			MouseExamine(mousex, mousey);
			break;
		
		default :
			break;
	}


	// If the mouse really moved, start the gravity processing
	if (AnmMouseMoved(m_mousepoint.x, mousex) || AnmMouseMoved(m_mousepoint.y, mousey))
	{
		m_applyGravity = true;
		m_mousemoved = true;

		// Save mouse position
		m_mousepoint.x = mousex;
		m_mousepoint.y = mousey;
	}

}

const double bigexangle = PI * .25;

void CAnmViewer::HandleKeyboard(int navKey)
{
	assert(navKey != -1);

	ANMTIME deltat = GetElapsedTime();

	Point3D speed = CalcSpeed();
	double angle = bigexangle;

	if (m_keyboard->Poll(DIK_RSHIFT) || m_keyboard->Poll(DIK_LSHIFT))
	{
		speed = speed * 2.0;
		angle *= 2.0;
	}

	if (navKey == DIK_LEFT || navKey == DIK_RIGHT || navKey == DIK_UP || navKey == DIK_DOWN)
	{
		if (navKey == DIK_LEFT)
		{
			if (m_navigationMode == eNavigationModeExamine)
				DoExamine(0, 0, -deltat * angle, 0);
			else
				DoFly(0, 0, -deltat * angle, 0);
		}
		else if (navKey == DIK_RIGHT)
		{
			if (m_navigationMode == eNavigationModeExamine)
				DoExamine(0, 0, deltat * angle, 0);
			else
				DoFly(0, 0, deltat * angle, 0);
		}
		else if (navKey == DIK_UP)
		{
			if (m_navigationMode == eNavigationModeExamine)
				DoExamine(0, 0, 0, -deltat * angle);
			else
				DoFly(0, deltat * speed.z, 0, 0);
		}
		else if (navKey == DIK_DOWN)
		{
			if (m_navigationMode == eNavigationModeExamine)
				DoExamine(0, 0, 0, deltat * angle);
			else
				DoFly(0, -deltat * speed.z, 0, 0);
		}

		m_applyGravity = true;
	}

	SaveCameraPos();
}

void CAnmViewer::HandleZScroll(double zdelta, bool shiftKey, bool ctrlKey)
{
	if (m_navigationMode == eNavigationModeNone)
		return;

	// N.B. these values need to be made more robust, dependent on world size, etc.
	double ydisp = zdelta / 80 * m_world->GetRadius();

	Point3D speed = CalcSpeed();
	if (shiftKey)
		speed = speed * 2.0;

	DoFly(0.0, ydisp * speed.z, 0.0, 0.0);

	SaveCameraPos();
}


#define VIEWER_EPSILON 0.000001

void CAnmViewer::MouseFly(int mousex, int mousey)
{
	assert(m_camera);

	double xang, yang, xdisp, ydisp;

	int deltax = mousex - m_initialMousePoint.x;
	int deltay = -(mousey - m_initialMousePoint.y);		// flip Y up


	ANMTIME deltat = GetElapsedTime();

	xdisp = deltat * (double) deltax * .05f;
	ydisp = deltat * (double) deltay * .05f;

	xang = deltat * (double) deltax / (2.0 * PI) * .0025f;
	yang = -deltat * (double) deltay / (2.0 * PI) * .0025f;

	if (abs(deltax) < ANM_MOUSEDEADZONE)
	{
		deltax = 0;
		xdisp = 0.0;
	}

	if (abs(deltay) < ANM_MOUSEDEADZONE)
	{
		deltay = 0;
		ydisp = 0.0;
	}

	Point3D speed = CalcSpeed();
	double rotspeed = 1.0; //  / ( 2.0 * PI ) ;
	if (m_keyboard->Poll(DIK_LSHIFT) || m_keyboard->Poll(DIK_RSHIFT))
	{
		speed = speed * 2.0;
		rotspeed = rotspeed * 2.0;
	}

	DoFly(xdisp * speed.x, ydisp * speed.z, xang * rotspeed, yang * rotspeed);

	m_lastXangle = xang;
	m_lastYangle = yang;
	m_lastxdisp = xdisp;
	m_lastydisp = ydisp;
}

void CAnmViewer::MouseExamine(int mousex, int mousey)
{
	assert(m_camera);

	double xangle, yangle, xdisp, ydisp;

	int deltax = mousex - m_mousepoint.x;
	int deltay = mousey - m_mousepoint.y;


	if (abs(deltax) < ANM_MOUSEDEADZONE)
		deltax = 0;

	if (abs(deltay) < ANM_MOUSEDEADZONE)
		deltay = 0;


	// N.B. these values need to be made more robust, dependent on world size, etc.
	xangle = (double) deltax / 1000 * PI;
	yangle = (double) deltay / 1000 * PI;

	if (!m_autoRotate && !m_zooming && !m_panning && (xangle == 0.0 && yangle == 0.0))
		return;

	Point3D speed = CalcSpeed();
	xdisp = (double) deltax * speed.x / 1000 * m_world->GetRadius();
	ydisp = - (double) deltay * speed.y / 1000 * m_world->GetRadius();

	if (xangle == 0.0)
		xangle = m_lastXangle;
	if (yangle == 0.0)
		yangle = m_lastYangle;
	if (xdisp == 0.0)
		xdisp = m_lastxdisp;
	if (ydisp == 0.0)
		ydisp = m_lastydisp;


	DoExamine(xdisp, ydisp, xangle, yangle);

	m_lastXangle = xangle;
	m_lastYangle = yangle;
}

// Worker bees - navigation abstracted from input devices
void CAnmViewer::DoFly(double xdisp, double ydisp, double xangle, double yangle)
{
	m_world->Lock();

	Point3D pos = m_camera->GetPosition();
	Point3D oldpos = pos;
	Point3D dir, up;
	GetCameraOrientation( dir, up);


	if (m_panning)
		DoCameraPan(pos, dir, up, xdisp, ydisp);
	else if (m_tilting)
		DoCameraTilt(pos, dir, up, yangle, -xangle);		// x->y; y->x
	else if (m_orbiting)
		DoCameraOrbit(pos, dir, up, yangle, xangle);		// x->y
	else
		DoCameraMove(pos, dir, up, ydisp, xangle);		// x->y

	if (m_collide && !(pos == oldpos))
		DoCollide(oldpos, pos, dir, up);

	m_camera->SetPosition(pos);
	m_camera->SetOrientation(dir, up);

	SetViewpointUserOffsets(pos, dir, up);

	m_world->Unlock();

}

void CAnmViewer::DoCollide(const Point3D &oldpos, Point3D &pos, Point3D &dir, Point3D &up)
{
	Point3D hitPoint, hitNormal;
	if (TestCollision(oldpos, pos, m_collisionRadius, hitPoint, hitNormal))
	{
#if 0
		
		// attempt at collision response; this blows because it can flip us around badly
		pos = hitPoint + (hitNormal * m_collisionRadius);
		Point3D newdir = pos - oldpos;
		newdir.Normalize();
		double dot = dir * newdir;
		double theta = acos(dot);
		if (fabs(theta) < (PI / 2))
			dir = newdir;

#else

#define KRV_SLIDE_ALONG_WALL
#ifdef KRV_SLIDE_ALONG_WALL
		hitNormal.Normalize();
		// Get the Bounce Back step.
		//
		Point3D stepBouncedBack( pos - hitPoint );

		// Subtract out the component that is along the HitNormal to get the vector along the wall
		//
		double dot = stepBouncedBack * hitNormal;
		Point3D stepSlideAlongWall = stepBouncedBack - dot*hitNormal;

		//  if that step along wall is non-zero...
		//
		if( stepSlideAlongWall.Mag() > 0.00001 ) {

			pos = hitPoint;
			Point3D posNew = hitPoint + stepSlideAlongWall;

			// See if we can step in that direction along the wall.
			// We might hit another wall, like at the corner of a room.
			//
			Point3D hitPointSlide, hitNormalSlide;
			if (TestCollision(pos, posNew, m_collisionRadius, hitPointSlide, hitNormalSlide))
			{
				// We hit another obsticle
				//
				pos = hitPointSlide;
			}
			else {
				pos = posNew;
			}
		}
		else {
			pos = hitPoint;
		}

#else

		pos = hitPoint;


#endif

#endif

	}
}

void CAnmViewer::GetViewpointDefaultOrientation(Point3D &dir, Point3D &up)
{
	if (m_viewpoint)
	{
		matrix4D vmat = m_viewpoint->GetWorldMatrix();
		Point3D org = vmat * Point3D::Zero;
		// Pesky origin
		Point3D zaxis = Point3D::k;
		dir = vmat * - zaxis - org;		// N.B.: RH
		up = vmat * Point3D::j - org;
	}
	else
	{
		dir = Point3D::k;
		up = Point3D::j;
	}
}

// krv:  For now, use this BIG number.
#define FLT_BIG  1.0e+10F


void CAnmViewer::ApplyGravity()
{
	// return;

	m_world->Lock();

	Point3D pos = m_camera->GetPosition();
	Point3D oldpos = pos;

	// use world up vector
	Point3D dir, up;
	GetViewpointDefaultOrientation(dir, up);

	ANMTIME deltat = GetElapsedTime();

	// Translate straight down
	Point3D speed = CalcSpeed();

	// For now, just use nav speed Y component as constant velocity, no acceleration
	double ydisp = deltat * ANMVIEWER_GRAVITYSPEED;

	// krv:  do NOT use FLT_MAX.  If you scale that by any number bigger than 1.0, we get
	//       a BAD number.
	DoCameraPan(pos, dir, up, 0.0, -FLT_BIG); // DoCameraPan(pos, dir, up, 0.0, -FLT_MAX);

	// Get downward direction vector
	Point3D downdir = pos - oldpos;
	downdir.Normalize();

	// Anything below us? If so, drop but maintain height above terrain
	Point3D hitPoint;
	if (TestTerrain(oldpos, pos, m_collisionRadius, hitPoint))
	{
		// THere is Ground Below us, so check if we need to fall.
		//
		Point3D old2hit = hitPoint - oldpos;
		double heightfromground = downdir * old2hit;

		if (heightfromground < m_avatarHeight)
		{
			// Our feet are at or below ground.
			//
			// krv:  If this is the first gravity test since VP Bound ( m_bVirginGravity )
			// then we should pop up so that are feet are on the ground as long as our eyes 
			// are above the grond.
			//
			double stepsize = m_avatarHeight - heightfromground;
			if (stepsize <= m_maxStepHeight ||
				( m_bVirginGravity && heightfromground > 0.0 ) ) { 
				// If our feet are below ground just a little bit, then pop up to the surface.
				//
				pos = hitPoint - (downdir * (m_avatarHeight));
			}
			else {
				// If our feet are below ground be more than the step size, then stay put.
				//
				pos = oldpos;
			}
		}
		else
		{
			// Our feet are above the ground, so we are falling!
			//
			pos = oldpos + (downdir * ydisp);

			old2hit = hitPoint - pos;

			// krv:
			// Need to get the signed value of this!
			// if we are below ground, this needs to be negative!
//			heightfromground = old2hit.Mag();
			heightfromground = downdir * old2hit;
			

			// If we fell past the ground, then pop back up above ground.
			//
			if (heightfromground < m_avatarHeight)
			{
				pos = hitPoint - (downdir * (m_avatarHeight));
			}
		}
	}
	else
	{
		// N.B.: Otherwise, just free fall?
		// for now, just stay where we are
		pos = oldpos;
	}
	m_bVirginGravity = false;

	m_camera->SetPosition(pos);

	GetCameraOrientation( dir, up);

	SetViewpointUserOffsets(pos, dir, up);
	
	m_world->Unlock();
}

void CAnmViewer::DoCameraMove(Point3D &pos, Point3D &dir, Point3D &up, double zdisp, double yangle)
{
	matrix4D mat, xmat;

//	if (m_navigationMode == eNavigationModeWalk)
//		mat.ToObjectLookAt(pos, pos + m_walkdir, m_walkup);
//	else
		mat.ToObjectLookAt(pos, pos + dir, up);

	xmat.ToTranslation(Point3D(0, 0, zdisp));
	mat = xmat * mat;
	pos = mat * Point3D(0, 0, 0);

	xmat.ToAxisAngle(Point3D(0, 1, 0), yangle);
	dir = xmat * dir;
	up = xmat * up;
}

void CAnmViewer::DoCameraTilt(const Point3D &pos, Point3D &dir, Point3D &up, double xangle, double yangle)
{
	matrix4D mat, xmat;

	// Find current x axis and rotate xangle around that
	Point3D xAxis = up ^ dir;
	mat.ToAxisAngle(xAxis, xangle);

	// Rotate yangle around current dir vector
	xmat.ToAxisAngle(dir, yangle);

	// Combine them
	mat = xmat * mat;

	// Transform
	dir = mat * dir;
	up = mat * up;

}

void CAnmViewer::DoCameraPan(Point3D &pos, const Point3D &dir, const Point3D &up, double xdisp, double ydisp)
{
	matrix4D mat, xmat;

	mat.ToObjectLookAt(pos, pos + dir, up);
	xmat.ToTranslation(Point3D(xdisp, ydisp, 0));
	mat = xmat * mat;
	pos = mat * Point3D(0, 0, 0);
}

void CAnmViewer::DoCameraOrbit(Point3D &pos, Point3D &dir, Point3D &up, double xangle, double yangle)
{
	dir.Normalize();
	up.Normalize();

	// get x axis from up, dir
	Point3D xAxis = up ^ dir;
	matrix4D mat, mat2;

	xAxis.Normalize();

	// get combined rotation matrix
	mat.ToAxisAngle(up, yangle);
	mat2.ToAxisAngle(xAxis, xangle);
	mat = mat * mat2;

	// Rotate direction vector
	dir = mat * dir;
	dir.Normalize();

	up = mat * up;
	up.Normalize();


	Point3D offset = pos - m_orbitcenter;	// vector from frustum center to camera

	pos = mat * offset + m_orbitcenter;
}

void CAnmViewer::GetFrustumCenter(Point3D &center)
{ 
	CAnmFrustum *pFrustum = m_camera->GetFrustum();
	assert(pFrustum);

	Point3 center_f;
	pFrustum->GetCenter(center_f);
	center = center_f;
}

void CAnmViewer::ComputeExamineCenter()
{
	CAnmFrustum *pFrustum = m_camera->GetFrustum();
	assert(pFrustum);

	// First, try the previously set examine center (via seek operation)
	if (m_useExamineCenter)
	{
		eFrustumLoc frustumLoc = pFrustum->TestPoint(m_examineCenter);

		// If our previously set (with seek) examine center is still visible, use that
		// Otherwise, unflag this because all bets are off
		if (frustumLoc == eInsideFrustum)
			return;
		else
			m_useExamineCenter = false;
	}

	// No go? Try examine center set via LOOKAT, or world center if no LOOKAT
	if (m_lookAt)
		m_examineCenter = m_centerOfRotation;
	else
		m_world->GetObjectCenter(&m_examineCenter);

	eFrustumLoc frustumLoc = pFrustum->TestPoint(m_examineCenter);

	// If our examine center is visible, use that.
	// Otherwise, try to calculate something reasonable
	if (frustumLoc == eInsideFrustum)
		return;
	else
	{
		Point3 hitPoint;

		// Test intersection of eye-based ray and world. If it hits
		// some geometry, use that as the center of rotatation

		CAnmDrawable *pDrawable = NULL;
		if (m_world->TestPick(m_winsize.x / 2, m_winsize.y / 2, hitPoint, &pDrawable))
		{
			CAnmBoundingBox bbox;

			m_world->GetBoundingBox(&bbox);

			eFrustumLoc frustumLoc = pFrustum->TestBox(bbox);

			// Is the scene completely contained in the frustum?
			// If so, use its center
			if (frustumLoc == eInsideFrustum)
			{
				m_world->GetCenter(&m_examineCenter);
			}
			else
			{
				pDrawable->GetBoundingBox(&bbox);

				frustumLoc = pFrustum->TestBox(bbox);

				// Is the hit object completely contained in the frustum?
				// If so, use its center
				if (frustumLoc == eInsideFrustum)
				{
					Point3 dummySize;

					bbox.GetCenterSize(&m_examineCenter, &dummySize);
				}
				else
				{
					// Otherwise use the intersection point
					m_examineCenter = hitPoint;
				}
			}
		}
		else
		{
			// Didn't hit anything with eye-based ray.

			CAnmBoundingBox bbox;

			m_world->GetBoundingBox(&bbox);

			eFrustumLoc frustumLoc = pFrustum->TestBox(bbox);

			// Is the scene completely contained in the frustum?
			// If so, use its center and orbit around the model
			if (frustumLoc == eInsideFrustum)
			{
				m_world->GetCenter(&m_examineCenter);
			}
			else
			{
				// Otherwise use the camera's location and spin around inside the world
				m_examineCenter = m_camera->GetPosition();
			}

			
		}
	}
}

void CAnmViewer::DoExamine(double xdisp, double ydisp, double xangle, double yangle)
{
	m_world->Lock();

	// setup: get current pos, orientation in
	// a matrix
	Point3D camerapos = m_camera->GetPosition();

	Point3D dir, up;
	GetCameraOrientation( dir, up );


	if (m_panning)
		DoCameraPan(camerapos, dir, up, -xdisp, -ydisp);
	else if (m_zooming)
		DoCameraMove(camerapos, dir, up, ydisp, 0);
	else
		DoWorldRotate(camerapos, dir, up, yangle, xangle);

	// set the values
	m_camera->SetPosition(camerapos);
	m_camera->SetOrientation(dir, up);
	
	SetViewpointUserOffsets(camerapos, dir, up);
	
	m_world->Unlock();

}

#if 0
////////////////////////////////////////////////////////////////////////////////
// Transform vectors
////////////////////////////////////////////////////////////////////////////////

static void TransformVectors
#endif

void CAnmViewer::DoWorldRotate(Point3D &pos, Point3D &dir, Point3D &up, double xangle, double yangle)
{
	dir.Normalize();
	up.Normalize();

	// get x axis from up, dir
	Point3D xAxis = up ^ dir;
	matrix4D mat, mat2;

	xAxis.Normalize();

	
	// get combined rotation matrix
	mat.ToAxisAngle(up, yangle);
	mat2.ToAxisAngle(xAxis, xangle);
	mat = mat * mat2;


	// Rotate direction vector
	dir = mat * dir;
	dir.Normalize();

	up = mat * up;
	up.Normalize();

	Point3D center(0,0,0);
#if 0
	if (m_lookAt)
		center = m_centerOfRotation;
	else
		m_world->GetObjectCenter(&center);
#else
	center = m_examineCenter;
#endif

	if (fabs(center.x) < VIEWER_EPSILON)
		center.x = 0.0;
	if (fabs(center.y) < VIEWER_EPSILON)
		center.y = 0.0;
	if (fabs(center.z) < VIEWER_EPSILON)
		center.z = 0.0;


	Point3D offset = pos - center;	// vector from scen center to camerae

	pos = mat * offset + center;
}


void CAnmViewer::ChangeCamera(CAnmViewpoint *pViewpoint, Point3D pos, Point3D dir, Point3D up, bool advise)
{
	m_world->Lock();

	// set the values
	m_camera->SetPosition(pos);
	m_camera->SetOrientation(dir, up);
	
	// Tell world to bind new viewpoint
	if (pViewpoint) {
		m_bVirginGravity = true;
		m_world->BindToViewpoint(pViewpoint);
	}

	// Make sure saved offsets are used in viewpoint
	SetViewpointUserOffsets(pos, dir, up);
	
	m_world->Unlock();

	if (advise)
	{
		sAnmCameraChange camchange(m_savedviewpoint, m_savedpos, m_saveddir, m_savedup,
			pos, dir, up);

		m_app->AdviseCameraChange(&camchange);
	}

	m_savedviewpoint = pViewpoint;
	m_savedpos = pos;
	m_saveddir = dir;
	m_savedup = up;
}

bool CAnmViewer::TestCollision(Point3D oldpos, Point3D newpos, double radius, Point3D &hitPoint, Point3D &hitNormal)
{
	Point3 oldpos_f( oldpos );
	Point3 newpos_f( newpos );
	Point3 hitPoint_f( hitPoint );
	Point3 hitNormal_f( hitNormal );
	bool bRet = m_world->TestCollision(oldpos_f, newpos_f, radius, hitPoint_f, hitNormal_f );
	oldpos = oldpos_f;
	newpos = newpos_f;
	hitPoint = hitPoint_f;
	hitNormal = hitNormal_f;
	return bRet;
}

bool CAnmViewer::TestTerrain(Point3D oldpos, Point3D newpos, double radius, Point3D &hitPoint)
{
	Point3 oldpos_f( oldpos );
	Point3 newpos_f( newpos );
	Point3 hitPoint_f( hitPoint );
	bool bRet = m_world->TestTerrain(oldpos_f, newpos_f, radius, hitPoint_f);
	oldpos = oldpos_f;
	newpos = newpos_f;
	hitPoint = hitPoint_f;
	return bRet;
}

Point3D CAnmViewer::CalcSpeed()
{
	return m_viewscale * m_speed * UserSpeedFactor(m_userspeed);
}

ANMTIME CAnmViewer::GetCurrentTime()
{
	CAnmTime *pClock = m_app->GetClock();
	return pClock->GetCurrentTime();
}

ANMTIME CAnmViewer::GetElapsedTime()
{
	return GetCurrentTime() - m_prevnavtime;
}

void CAnmViewer::SetSeekMode(int seekMode)
{
	m_seekMode = seekMode;

	m_world = m_app->GetWorld();
	assert(m_world);

	m_world->Lock();
	m_world->SetSeekMode(seekMode ? true : false);
	m_world->Unlock();
}

void CAnmViewer::SetUserSpeed(eUserSpeed userspeed)
{
	m_userspeed = userspeed;
}

#define LOOKATFACTOR .66667

void CAnmViewer::SeekTo(int mousex, int mousey)
{
	Point3D pickfrom, pickto;
	CAnmPickManager *pPickManager = m_world->GetPickManager();
	sAnmSceneGraphPickList *pPickList = 
		pPickManager->GetPickList();

	int sz = pPickList->pickList.size();
	if (sz > 0)
	{
		sAnmPickDesc<class CAnmNode, class CAnmGroup> scenePickDesc =
			pPickList->pickList[0];

		Point3D hitpoint = scenePickDesc.hitPoint;

		m_world->Lock();

		m_savedviewpoint = m_viewpoint;
		m_savedpos = m_camera->GetPosition();

		GetCameraOrientation( m_saveddir, m_savedup );


		Point3D pos = m_savedpos;
		Point3D dir = hitpoint - pos;
		double zdisp = dir.Mag();

		matrix4D mat;

		bool shiftKey = m_keyboard->Poll(DIK_LSHIFT) || m_keyboard->Poll(DIK_RSHIFT) ;

		if (!shiftKey)
		{
			mat.ToTranslation(dir * LOOKATFACTOR);
			pos = mat * pos;
			m_camera->SetPosition(pos);
		}

		SetViewpointUserOffsets(pos, m_saveddir, m_savedup);
	
		m_examineCenter = hitpoint;
		m_useExamineCenter = true;

		m_world->Unlock();
	
		SaveCameraPos();
	}

}

void CAnmViewer::Straighten()
{
	m_world->Lock();

	m_savedviewpoint = m_viewpoint;
	m_savedpos = m_camera->GetPosition();
	GetCameraOrientation( m_saveddir, m_savedup );

	Point3D pos = m_camera->GetPosition();
	Point3D dir, up;
	GetCameraOrientation( dir, up );


	Point3D xAxis = up ^ m_walkup;
	double dot = m_walkup * up;
	double angle = acos(dot);

	matrix4D mat;
	mat.ToAxisAngle(xAxis, angle);

	dir = mat * dir;
	up = mat * up;

	m_camera->SetOrientation(dir, up);

	SetViewpointUserOffsets(pos, dir, up);

	m_world->Unlock();

	SaveCameraPos();

}

// Camera animation stuff
void CAnmViewer::SetupAnimation()
{
	// Don't do this if we're already animating
	if (m_viewpointAnimation != NULL)
		return;

	Point3D pos, dir, up;

	m_world->Lock();

	pos = m_camera->GetPosition();
	GetCameraOrientation( dir, up );

	// SetViewpointUserOffsets(pos, dir, up);
	
	m_world->Unlock();

	m_viewpointAnimation = new sAnmViewpointAnimation(ANMVIEWER_DEFAULTANIMATIONTIME,
		pos, dir, up);
}

bool CAnmViewer::SetAnimationTarget(const Point3D &pos, const Point3D &dir, const Point3D &up)
{
	if (m_viewpointAnimation == NULL)
		return false;

	if (m_viewpointAnimation->Started())
	{
		m_viewpointAnimation->UpdatePosition(pos, dir, up);
		return true;
	}

	Time curtime = GetCurrentTime();

	m_viewpointAnimation->Start(m_viewpoint, curtime, pos, dir, up);

	return true;
}

void CAnmViewer::RunAnimation()
{
	if (m_viewpointAnimation == NULL)
		return;

	Point3D pos, dir, up;

	Time curtime = GetCurrentTime();

	if (m_viewpointAnimation->Started())
	{
		bool stillgoing = m_viewpointAnimation->Animate(curtime, pos, dir, up);

		m_world->Lock();

		m_camera->SetPosition(pos);
		m_camera->SetOrientation(dir, up);

//		SetViewpointUserOffsets(pos, dir, up);

		m_world->Unlock();

		if (!stillgoing)
		{
			EndAnimation();
			m_viewpoint->Restore();
		}
	}
}

void CAnmViewer::EndAnimation()
{
	SafeDelete(m_viewpointAnimation);
}

void sAnmViewpointAnimation::Start(class CAnmViewpoint *pVp, Time start, Point3D endpos, Point3D enddir, Point3D endup)
{
	assert(pVp);

	pViewpoint = pVp;
	startTime = start;
	endPos = endpos;
	endDir = enddir;
	endUp = endup;

	FloatArray *pKey;
	Point3Array *pKeyValues;

	pPosinterp = new CAnmInterpolator<Point3>;
	
	pKey = pPosinterp->GetKey();
	pKey->push_back(0.0);
	pKey->push_back(1.0);
	
	pKeyValues = pPosinterp->GetKeyValue();
	pKeyValues->push_back(startPos);
	pKeyValues->push_back(endPos);

#if (!ANMVIEWER_ANIMATE_SLERP)
	pDirinterp = new CAnmInterpolator<Point3>;
	
	pKey = pDirinterp->GetKey();
	pKey->push_back(0.0);
	pKey->push_back(1.0);
	
	pKeyValues = pDirinterp->GetKeyValue();
	pKeyValues->push_back(startDir);
	pKeyValues->push_back(endDir);

	pUpinterp = new CAnmInterpolator<Point3>;
	
	pKey = pUpinterp->GetKey();
	pKey->push_back(0.0);
	pKey->push_back(1.0);
	
	pKeyValues = pUpinterp->GetKeyValue();
	pKeyValues->push_back(startUp);
	pKeyValues->push_back(endUp);
#else

	RotationArray *pOrKeyValues;

	pOrinterp = new CAnmInterpolator<Rotation>;
	pKey = pOrinterp->GetKey();
	pKey->push_back(0.0);
	pKey->push_back(1.0);

	matrix4D mat;
	mat.ToObjectLookAt(Point3D::Zero, startDir, startUp);
	CAnmQuaternion q(mat);
	Rotation startRot = q.AsRotation();

	mat.ToObjectLookAt(Point3D::Zero, endDir, endUp);
	q = matrix4( mat );
	Rotation endRot = q.AsRotation();

	pOrKeyValues = pOrinterp->GetKeyValue();
	pOrKeyValues->push_back(startRot);
	pOrKeyValues->push_back(endRot);

#endif

}

void sAnmViewpointAnimation::UpdatePosition(Point3D endpos, Point3D enddir, Point3D endup)
{
	endPos = endpos;
	endDir = enddir;
	endUp = endup;

	Point3Array *pKeyValues = pPosinterp->GetKeyValue();
	assert(pKeyValues);
	(*pKeyValues)[1] = endPos;

#if (!ANMVIEWER_ANIMATE_SLERP)

	pKeyValues = pDirinterp->GetKeyValue();
	assert(pKeyValues);
	(*pKeyValues)[1] = endDir;

	pKeyValues = pUpinterp->GetKeyValue();
	assert(pKeyValues);
	(*pKeyValues)[1] = endUp;

#else

	matrix4D mat;
	mat.ToObjectLookAt(Point3D::Zero, endDir, endUp);
	CAnmQuaternion q(mat);
	q = matrix4( mat );
	Rotation endRot = q.AsRotation();

	RotationArray *pOrKeyValues = pOrinterp->GetKeyValue();
	assert(pOrKeyValues);
	(*pOrKeyValues)[1] = endRot;


#endif

}

#define EASE 1

#if EASE

#define ZERO FLT_EPSILON

inline double Ease( double u, double a, double b )
{
	// a is Ease out from Prev
	// b is Ease in from Next

	double s = a + b;
	if( s < ZERO ) {
		return u;
	}
	if( s > 1.0 ) {
		a /= s;
		b /= s;
	}

	double k = 1.0 / ( 2.0 - a - b );
	if( u < a ) {
		return ( ( k/a ) *u*u );
	}
	else if( u < 1.0 - b ) {
		return ( k*( 2.0*u - a ));
	}
	else {
		u = 1.0 - u;
		return  ( 1.0 - ( k/b )*u*u );
	}
}

#endif

bool sAnmViewpointAnimation::Animate(Time curtime, Point3D &pos, Point3D &dir, Point3D &up)
{
	double fract = CalcFraction(curtime);

#if 0
	// Ease in to .5, ease out from .5
#if 1
	double incr = fmod(fract, .5);
	if (fract >= .5)
		incr = .5 - incr;

	double delta = incr * incr * 2;

	if (fract < .5)
		fract = delta;
	else if (fract < 1)
		fract = 1.f - delta;
#else
	if (fract >= 0.0 && fract < 1.0)
		fract = fract * fract; 
#endif
#else

	if (fract < 1)
		fract = Ease(fract, .5, .5);

#endif

	if (fract < 0.0 || fract >= 1.0)
	{
		pos = endPos;
		dir = endDir;
		up = endUp;
		return false;
	}
	else
	{
		Point3 pos_f;
		pPosinterp->Interp(fract, &pos_f);
		pos = pos_f;

#if (!ANMVIEWER_ANIMATE_SLERP)
		pDirinterp->Interp(fract, &dir);
		pUpinterp->Interp(fract, &up);
#else

		Rotation rot;
		pOrinterp->Interp(fract, &rot);

		matrix4D mat;
		mat.ToAxisAngle(rot.vec, rot.theta);
		dir = mat * Point3D::k;
		up = mat * Point3D::j;
#endif

		return true;
	}
}



void CAnmViewer::GetCameraOrientation ( Point3D &dir, Point3D &up)
{
	Point3 dir_f;
	Point3 up_f;
	m_camera->GetOrientation(&dir_f, &up_f);
	dir = dir_f;
	up = up_f;
}

// Author-controlled navigation
// DoFly(double xdisp, double ydisp, double xangle, double yangle)

void CAnmViewer::Move(eAnmViewerNavAxis axis, int direction)
{
	ANMTIME deltat = GetElapsedTime();

	Point3D speed = CalcSpeed();

	switch (axis)
	{
		case eAnmViewerXAxis :
			m_panning = true;
			DoFly(direction * deltat * speed.z, 0, 0, 0);
			m_panning = false;
			break;

		case eAnmViewerYAxis :
			m_panning = true;
			DoFly(0, direction * deltat * speed.z, 0, 0);
			m_panning = false;
			break;

		case eAnmViewerZAxis :
			DoFly(0, -direction * deltat * speed.z, 0, 0);
			break;

		default :
			break;
	}

}

void CAnmViewer::Rotate(eAnmViewerNavAxis axis, int direction)
{
	ANMTIME deltat = GetElapsedTime();

	Point3D speed = CalcSpeed();

	switch (axis)
	{
		case eAnmViewerXAxis :
			m_tilting = true;
			DoFly(0, 0, 0, direction * deltat);
			m_tilting = false;
			break;

		case eAnmViewerYAxis :
			DoFly(0, 0, direction * deltat * bigexangle, 0);
			break;

		case eAnmViewerZAxis :
			m_tilting = true;
			DoFly(0, 0, direction * deltat, 0);
			m_tilting = false;
			break;

		default :
			break;
	}
}

void CAnmViewer::Orbit(eAnmViewerNavAxis axis, int direction)
{
	ANMTIME deltat = GetElapsedTime();

	Point3D speed = CalcSpeed();

	switch (axis)
	{
		case eAnmViewerXAxis :
			DoExamine(0, 0, 0, direction * deltat * bigexangle);
			break;

		case eAnmViewerYAxis :
			DoExamine(0, 0, direction * deltat * bigexangle, 0);
			break;

		default :
			break;
	}
}

