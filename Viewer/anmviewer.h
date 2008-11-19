/********************************************************************************
 * Flux
 *
 * File: anmviewer.h
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

#ifndef _anmviewer_h
#define _anmviewer_h

#include "anminterpolator.h"
#include "anmnavigationinfo.h"
#include "anmtime.h"

#define ANMVIEWER_DEFAULT_NAVMODE eNavigationModeFly

typedef enum eUserSpeed {
	eUserSpeedSlowest,
	eUserSpeedSlow,
	eUserSpeedMedium,
	eUserSpeedFast,
	eUserSpeedFaster,
	eUserSpeedFastest,
} eUserSpeed ;

typedef enum eAnmViewerNavAxis {
	eAnmViewerXAxis			= 0,
	eAnmViewerYAxis			= 1,
	eAnmViewerZAxis			= 2,
} eAnmViewerNavAxis ;

#define ANMVIEWER_SPEEDFACTOR_SLOWEST		.01f
#define ANMVIEWER_SPEEDFACTOR_SLOW			.1f
#define ANMVIEWER_SPEEDFACTOR_MEDIUM		1.f
#define ANMVIEWER_SPEEDFACTOR_FAST			10.f
#define ANMVIEWER_SPEEDFACTOR_FASTER		100.f
#define ANMVIEWER_SPEEDFACTOR_FASTEST		10000.f

#define ANMVIEWER_SEEKMODENONE				0x00
#define ANMVIEWER_SEEKMODESEEK				0x01
#define ANMVIEWER_SEEKMODECONTINUOUS		0x02

inline double UserSpeedFactor(eUserSpeed userspeed)
{
	switch (userspeed)
	{
		case eUserSpeedSlowest :
			return ANMVIEWER_SPEEDFACTOR_SLOWEST;
			break;

		case eUserSpeedSlow :
			return ANMVIEWER_SPEEDFACTOR_SLOW;
			break;

		case eUserSpeedFast :
			return ANMVIEWER_SPEEDFACTOR_FAST;
			break;

		case eUserSpeedFaster :
			return ANMVIEWER_SPEEDFACTOR_FASTER;
			break;

		case eUserSpeedFastest :
			return ANMVIEWER_SPEEDFACTOR_FASTEST;
			break;

		case eUserSpeedMedium :
		default :
			return ANMVIEWER_SPEEDFACTOR_MEDIUM;
			break;
	}
}

#define ANMVIEWER_GRAVITYSPEED 9.8f

class EXPORT CAnmViewer : public CAnmObject
{

protected:

	class cApplication		*m_app;
	class CAnmWorld			*m_world;
	class CAnmCamera		*m_camera;
	class cKeyboard			*m_keyboard;
	class CAnmViewpoint		*m_viewpoint;
	Point2					 m_mousepoint;
	Point2					 m_initialMousePoint;
	Point2					 m_winsize;
	double					 m_speed;
	eUserSpeed				 m_userspeed;
	double					 m_lastang;
	double					 m_lastxdisp;
	double					 m_lastydisp;
	double					 m_lastXangle;
	double					 m_lastYangle;
	bool					 m_navigating;
	eNavigationMode			 m_navigationMode;
	eNavigationTransitionMode m_transitionMode;
	bool					 m_autoRotate;
	bool					 m_panning;
	bool					 m_tilting;
	bool					 m_zooming;
	bool					 m_orbiting;
	class CAnmViewpoint		*m_savedviewpoint;
	Point3D					 m_savedpos;
	Point3D					 m_saveddir;
	Point3D					 m_savedup;
	Point3D					 m_viewscale;
	Point3D					 m_orbitcenter;
	bool					 m_collide;
	double					 m_collisionRadius;
	double					 m_avatarHeight;
	double					 m_maxStepHeight;
	bool					 m_gravity;
	bool					 m_applyGravity;
	Point3D					 m_centerOfRotation;
	Point3					 m_examineCenter;
	bool					 m_lookAt;
	bool					 m_useExamineCenter;
	int						 m_seekMode;
	bool					 m_mousemoved;
	ANMTIME					 m_prevnavtime;
	struct sAnmViewpointAnimation	*m_viewpointAnimation;
	Point3D					 m_walkdir;
	Point3D					 m_walkup;
	bool					 m_bVirginGravity;


	// Navigation modality
	virtual void BeginNavigate(int mousedown, int mousex, int mousey);
	virtual void EndNavigate();

	// Fly/Walk mode routines
	virtual void DoFly(double xdisp, double ydisp, double xangle, double yangle);
	virtual void DoCameraMove(Point3D &pos, Point3D &dir, Point3D &up, double zdisp, double yangle);
	virtual void DoCameraTilt(const Point3D &pos, Point3D &dir, Point3D &up, double xangle, double yangle);
	virtual void DoCameraPan(Point3D &pos, const Point3D &dir, const Point3D &up, double xdisp, double ydisp);
	virtual void DoCameraOrbit(Point3D &pos, Point3D &dir, Point3D &up, double xangle, double yangle);
	virtual void DoCollide(const Point3D &oldpos, Point3D &pos, Point3D &dir, Point3D &up);
	virtual void ApplyGravity();

	// Examine mode routines
	virtual void DoExamine(double xdisp, double ydisp, double xangle, double yangle);
	virtual void DoWorldRotate(Point3D &pos, Point3D &dir, Point3D &up, double xangle, double yangle);

	// Mouse handlers; translate from mouse movement to world values
	virtual void MouseFly(int mousex, int mousey);
	virtual void MouseExamine(int mousex, int mousey);

	// Keyboard handlers; translate from keystrokes to movement
	virtual void HandleKeyboard(int navKey);

	// Keyboard navigation
	virtual int PollNavKey();

	// Some helpers
	virtual Point3D CalcSpeed();

	virtual bool TestCollision(Point3D oldpos, Point3D newpos, double radius, Point3D &hitPoint, Point3D &hitNormal);
	virtual bool TestTerrain(Point3D oldpos, Point3D newpos, double radius, Point3D &hitPoint);

	virtual void GetFrustumCenter(Point3D &center);

	virtual ANMTIME GetCurrentTime();
	virtual ANMTIME GetElapsedTime();

	virtual void SeekTo(int mousex, int mousey);

	virtual void SaveCameraPos();

	virtual void SetViewpointUserOffsets(const Point3D &pos, const Point3D &dir, const Point3D &up);

	virtual void GetViewpointDefaultOrientation(Point3D &dir, Point3D &up);

	virtual void ComputeExamineCenter();

	virtual bool Animating()
	{
		return m_viewpointAnimation != NULL;
	}

	virtual void SetupAnimation();
	virtual void RunAnimation();
	virtual void EndAnimation();

	void GetCameraOrientation ( Point3D &dir, Point3D &up);

public:

	CAnmViewer(class cApplication *pApp, eNavigationMode navMode = ANMVIEWER_DEFAULT_NAVMODE);
	~CAnmViewer();

	// Polling
	virtual void Poll(bool pollMouse, bool pollKeyboard);

	// Navigation
	virtual void HandleMouseMove(int mousex, int mousey);
	virtual void HandleZScroll(double zdelta, bool shiftKey, bool ctrlKey);
	virtual void ChangeCamera(class CAnmViewpoint *pViewpoint, Point3D pos, Point3D dir, Point3D up, bool advise = false);
	virtual bool SetAnimationTarget(const Point3D &pos, const Point3D &dir, const Point3D &up);

	virtual void Move(eAnmViewerNavAxis axis, int direction);
	virtual void Rotate(eAnmViewerNavAxis axis, int direction);
	virtual void Orbit(eAnmViewerNavAxis axis, int direction);

	// Other methods
	virtual void AdviseViewpointBound(class CAnmViewpoint *pViewpoint);

	// Accessors

	virtual class cApplication *GetApplication()
	{
		return m_app;
	}

	virtual class CAnmWorld *GetWorld()
	{
		return m_world;
	}

	virtual void SetCamera(class CAnmCamera *pCamera)
	{
		assert(pCamera);
		m_camera = pCamera;
	}

	virtual class CAnmCamera *GetCamera()
	{
		return m_camera;
	}

	virtual void SetNavigationMode(eNavigationMode navMode);

	virtual eNavigationMode GetNavigationMode()
	{
		return m_navigationMode;
	}

	virtual void SetTransitionMode(eNavigationTransitionMode transitionMode);

	virtual eNavigationTransitionMode GetTransitionMode()
	{
		return m_transitionMode;
	}
	
	virtual bool AnimateTransitions()
	{
		return (m_transitionMode == eNavigationTransitionLinear) ||
			(m_transitionMode == eNavigationTransitionAnimate);
	}

	virtual void SetSpeed(double speed);
	virtual double GetSpeed()
	{
		return m_speed;
	}

	virtual void SetViewScale(Point3D scale);
	virtual Point3D GetViewScale()
	{
		return m_viewscale;
	}

	virtual void SetCollide(Boolean collide);
	virtual Boolean GetCollide()
	{
		return m_collide;
	}

	virtual void SetCollisionRadius(double collisionRadius);
	virtual double GetCollisionRadius()
	{
		return m_collisionRadius;
	}

	virtual void SetAvatarHeight(double avatarHeight);
	virtual double GetAvatarHeight()
	{
		return m_avatarHeight;
	}

	virtual void SetMaxStepHeight(double maxStepHeight);
	virtual double GetMaxStepHeight()
	{
		return m_maxStepHeight;
	}

	void SetCenterOfRotation(Point3D p);
	Point3D GetCenterOfRotation()
	{
		return m_centerOfRotation; 
	}

	void SetLookAt(bool lookAt);
	bool GetLookAt()
	{
		return m_lookAt; 
	}

	void SetSeekMode(int seekMode);
	int GetSeekMode()
	{
		return m_seekMode; 
	}

	void SetUserSpeed(eUserSpeed);
	eUserSpeed GetUserSpeed()
	{
		return m_userspeed;
	}

	void Straighten();
};


typedef struct sAnmCameraChange {
	class CAnmViewpoint		*viewpoint;
	Point3D frompos;
	Point3D fromdir;
	Point3D fromup;
	Point3D topos;
	Point3D todir;
	Point3D toup;

	sAnmCameraChange()
	{
		viewpoint = NULL;
		frompos = Point3D(0, 0, 0);
		fromdir = Point3D(0, 0, -1);
		fromup = Point3D(0, 1, 0);
		topos = Point3D(0, 0, 0);
		todir = Point3D(0, 0, -1);
		toup = Point3D(0, 1, 0);
	}

	sAnmCameraChange(class CAnmViewpoint *pViewpoint, Point3D fromcamerapos, Point3D fromcameradir, Point3D fromcameraup,
		Point3D tocamerapos, Point3D tocameradir, Point3D tocameraup)
	{
		viewpoint = pViewpoint;
		frompos = fromcamerapos;
		fromdir = fromcameradir;
		fromup = fromcameraup;
		topos = tocamerapos;
		todir = tocameradir;
		toup = tocameraup;
	}

} sAnmCameraChange ;

#define ANMVIEWER_ANIMATE_SLERP 1

typedef struct sAnmViewpointAnimation {
	class CAnmViewpoint				*pViewpoint;
	CAnmInterpolator<Point3>		*pPosinterp;
#if (!ANMVIEWER_ANIMATE_SLERP)
	CAnmInterpolator<Point3>		*pDirinterp;
	CAnmInterpolator<Point3>		*pUpinterp;
#else
	CAnmInterpolator<Rotation>		*pOrinterp;
#endif

	Time							 startTime;
	Time							 duration;
	Point3D							 startPos;
	Point3D							 startDir;
	Point3D							 startUp;
	Point3D							 endPos;
	Point3D							 endDir;
	Point3D							 endUp;

	sAnmViewpointAnimation(Time dur, Point3D startpos, Point3D startdir, Point3D startup)
	{
		pViewpoint = NULL;
		pPosinterp = NULL;
#if (!ANMVIEWER_ANIMATE_SLERP)
		pDirinterp = NULL;
		pUpinterp = NULL;
#else
		pOrinterp = NULL;
#endif
		duration = dur;
		startPos = startpos;
		startDir = startdir;
		startUp = startup;
	}

	~sAnmViewpointAnimation()
	{
		SafeDelete(pPosinterp);
#if (!ANMVIEWER_ANIMATE_SLERP)
		SafeDelete(pDirinterp);
		SafeDelete(pUpinterp);
#else
		SafeDelete(pOrinterp);
#endif

	}

	bool Started()
	{
		return pViewpoint != NULL;
	}

	double CalcFraction(Time curtime)
	{
		Time delta = curtime - startTime;
		return delta / duration;
	}

	void Start(class CAnmViewpoint *pVp, Time start, Point3D endpos, Point3D enddir, Point3D endup);
	bool Animate(Time curtime, Point3D &pos, Point3D &dir, Point3D &up);
	void UpdatePosition(Point3D endpos, Point3D enddir, Point3D endup);

} sAnmViewpointAnimation ;

#define ANMVIEWER_DEFAULTANIMATIONTIME		2.f


#endif // _anmviewer_h
