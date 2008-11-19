/********************************************************************************
 * Flux
 *
 * File: anmcamera.h
 * Description: Camera class
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

#ifndef _anmcamera_h
#define _anmcamera_h

#define ANMCAMERA_DEFAULT_FIELDOFVIEW		0.785398f
#define ANMCAMERA_DEFAULT_POSITION			Point3D(0.0,0.0,0.0)
#define ANMCAMERA_DEFAULT_DIRECTION			Point3D(0.0,0.0,1.0)	
#define ANMCAMERA_DEFAULT_UP				Point3D(0.0,1.0,0.0)
#define ANMCAMERA_DEFAULTNEAR				.5f
#define ANMCAMERA_DEFAULTFAR				4000.0

#define ANMCAMERA_HEADLIGHTCOLOR			(color4::White)
#define ANMCAMERA_AMBIENTCOLOR				(color4::Black)

#include "anmfrustum.h"

class EXPORT CAnmCamera : public CAnmObject
{
protected:

	double					 m_fieldOfView;
	Point3D					 m_position;
	Point3D					 m_direction;
	Point3D					 m_up;
	CAnmGraphics::Light		*m_headlight;
	Boolean					 m_headlighton;
	int						 m_lightindex;
	matrix4D				 m_matrix;
	double					 m_near;
	double					 m_far;
	matrix4D			     m_projectionmatrix;
	CAnmFrustum				 m_frustum;

#ifndef		AN_NO_ORTHO
//	krv:  New Orthographic camera paramters.
//	
	Boolean					 m_bOrthographic;
	double					 m_fOrthoX0;
	double					 m_fOrthoX1;
	double					 m_fOrthoY0;
	double					 m_fOrthoY1;
	double					 m_fOrthoZ0;
	double					 m_fOrthoZ1;
#endif


	virtual void UpdateMatrix();
	virtual void SetProjectionMatrix(class CAnmDevice *pDevice);

public:

	CAnmCamera(cApplication *pApp, int lightindex, bool headlighton = true);
	~CAnmCamera();

	// New methods
	virtual void SetToDevice(class CAnmDevice *pDevice);

	// Help with picking
	virtual void GetPickRay(class CAnmDevice *pDevice, int x, int y, Point3 *pFrom, Point3 *pTo);

	// Accessors
	void SetFieldOfView(Float fov);
	Float GetFieldOfView() { return m_fieldOfView; }
	void GetFieldOfView(Float *pVal)
	{
		assert(pVal);
		*pVal = m_fieldOfView;
	}
	
	virtual void SetNearDistance(double neard);
	virtual double GetNearDistance()
	{
		return m_near;
	}

	virtual void SetFarDistance(double fard);
	virtual double GetFarDistance()
	{
		return m_far;
	}

	void SetPosition(Point3 p);
	Point3 GetPosition() { return m_position; }
	void GetPosition(Point3 *pp)
	{
		assert(pp);
		*pp = m_position;
	}

	void SetOrientation(Point3 direction, Point3 up);
	void GetOrientation(Point3 *pDirection, Point3 *pUp)
	{
		*pDirection = m_direction;
		*pUp = m_up;
	}

	virtual matrix4 GetMatrix()
	{
		return m_matrix;
	}

	void SetHeadlightOn(Boolean headlight);

	Boolean GetHeadlightOn()
	{
		return m_headlighton;
	}

	CAnmFrustum *GetFrustum()
	{
		return &m_frustum;
	}

//	krv:  New Orthographic camera paramters.
//	
	void SetOrthographic(Boolean b);
	Boolean GetOrthographic(){ return m_bOrthographic; }

	void SetOrthoParams( Float x0, Float x1,
						 Float y0, Float y1,
						 Float z0, Float z1 );

	Boolean GetOrthoParams( Float params[6] ) {
		params[0] = m_fOrthoX0;
		params[1] = m_fOrthoX1;
		params[2] = m_fOrthoY0;
		params[3] = m_fOrthoY1;
		params[4] = m_fOrthoZ0;
		params[5] = m_fOrthoZ1;
		return m_bOrthographic;
	}

	Float GetOrthoX0(){ return m_fOrthoX0; }
	Float GetOrthoX1(){ return m_fOrthoX1; }
	Float GetOrthoY0(){ return m_fOrthoY0; }
	Float GetOrthoY1(){ return m_fOrthoY1; }
	Float GetOrthoZ0(){ return m_fOrthoZ0; }
	Float GetOrthoZ1(){ return m_fOrthoZ1; }

};

#endif // _anmcamera_h
