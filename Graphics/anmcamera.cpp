/********************************************************************************
 * Flux
 *
 * File: anmcamera.cpp
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

#include "stdafx.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmmatrixinvert.h"
#include "anmviewport.h"

#define TURKOWSKI_REV_2

CAnmCamera::CAnmCamera(cApplication *pApp, int lightindex, bool headlighton) :
	m_lightindex(lightindex),
	m_headlighton(headlighton),
	m_fieldOfView(ANMCAMERA_DEFAULT_FIELDOFVIEW), 
	m_position(ANMCAMERA_DEFAULT_POSITION),
	m_direction(ANMCAMERA_DEFAULT_DIRECTION),
	m_up(ANMCAMERA_DEFAULT_UP),
	m_near(ANMCAMERA_DEFAULTNEAR),
	m_far(ANMCAMERA_DEFAULTFAR)
{
//	krv:  New Orthographic camera paramters.
//	
	m_bOrthographic = false;
	m_fOrthoX0 =	-1.0;
	m_fOrthoX1 =	1.0;
	m_fOrthoY0 =	-1.0;
	m_fOrthoY1 =	1.0;
	m_fOrthoZ0 =	-1.0;
	m_fOrthoZ1 =	1.0;

	m_matrix = matrix4D::Identity;
	m_projectionmatrix = matrix4D::Identity;

	CAnmGraphics *pGraphics = pApp->GetGraphics();
	assert(pGraphics);

	m_headlight = pGraphics->CreateLight(eAnmLightDirectional);

	color4 diffuseColor = color4::Black;
	color4 ambientColor = color4::Black;
	if (m_headlighton)
	{
		diffuseColor = ANMCAMERA_HEADLIGHTCOLOR;
		ambientColor = ANMCAMERA_AMBIENTCOLOR; 
	}

	color4 specularColor = color4::White;

	Point3 dir( m_direction );
	Point3 pos( m_position );
	m_headlight->SetDirection(&dir);
	m_headlight->SetLocation(&pos);
	m_headlight->SetDiffuseColor(&diffuseColor);
	m_headlight->SetSpecularColor(&specularColor);
	m_headlight->SetAmbientColor(&ambientColor);
}

CAnmCamera::~CAnmCamera()
{
	SafeDelete(m_headlight);
}

void CAnmCamera::UpdateMatrix()
{
	// Create a LookAt matrix with these params
	if( m_bOrthographic ) {
		m_position = Point3D( 0.0, 0.0, 0.0 );
		m_direction = Point3D( 0.0, 0.0, 1.0 );
		m_up = Point3D( 0.0, 1.0, 0.0 );
		m_matrix.ToObjectLookAt(m_position, m_position + m_direction, m_up);
	}
	else
		m_matrix.ToObjectLookAt(m_position, m_position + m_direction, m_up);
}

void CAnmCamera::SetFieldOfView(float fov)
{
	m_fieldOfView = fov;
}

void CAnmCamera::SetNearDistance(double neard)
{
	m_near = neard;
}

void CAnmCamera::SetFarDistance(double fard)
{
	m_far = fard;
}

void CAnmCamera::SetPosition(Point3 p)
{
	m_position = p;

	UpdateMatrix();
}

void CAnmCamera::SetOrientation(Point3 direction, Point3 up)
{
	m_direction = direction;
	m_up = up;

	m_direction.Normalize();
	m_up.Normalize();

	UpdateMatrix();
}

void CAnmCamera::SetHeadlightOn(Boolean headlighton)
{
	m_headlighton = headlighton;

	UpdateMatrix();
}

void CAnmCamera::SetOrthographic(Boolean b)
{ 
	m_bOrthographic = b; 
	UpdateMatrix();
}

void CAnmCamera::SetOrthoParams( Float x0, Float x1,
					 Float y0, Float y1,
					 Float z0, Float z1 )
{
	m_fOrthoX0 = x0;
	m_fOrthoX1 = x1;
	m_fOrthoY0 = y0;
	m_fOrthoY1 = y1;
	m_fOrthoZ0 = z0;
	m_fOrthoZ1 = z1;
	UpdateMatrix();
}

void CAnmCamera::SetProjectionMatrix(CAnmDevice *pDevice)
{
	CAnmViewport *pViewport = pDevice->GetViewport();
	assert(pViewport);

	int height = pViewport->Height();
	int width = pViewport->Width();

    if( height <= 0 || width <= 0 )
        return;

	double fAspect = ((double)height) / ((double)width);

	if( m_bOrthographic ) {

		double dX = m_fOrthoX1 - m_fOrthoX0;
		double dY = m_fOrthoY1 - m_fOrthoY0;
		double dZ = m_fOrthoZ1 - m_fOrthoZ0;
		if( dX < 0.0001 ||
			dY < 0.0001 ||
			dZ < 0.0001 ) {
	        return ;
		}

		double locX0 = m_fOrthoX0;
		double locX1 = m_fOrthoX1;
		double locY0 = m_fOrthoY0;
		double locY1 = m_fOrthoY1;
		double locZ0 = m_fOrthoZ0;
		double locZ1 = m_fOrthoZ1;

		double AspectProvided = dY / dX;
		double AspectOfWindow = fAspect;
		double RatioOfAspects = AspectOfWindow / AspectProvided;

		// krv  extend either the width or the height so the 
		// Ortho paramters match the aspect ratio.
		// NOTE, we could shrink the other paramters, but that would clip
		//
		if ( RatioOfAspects < 1.0 )
		{
			double growth = dX * 0.5 * ( ( 1.0 / ( RatioOfAspects ) ) - 1.0 );
			locX0 -= growth;
			locX1 += growth;
		}
		else
		{
			double growth = dY * 0.5 * ( RatioOfAspects - 1.0 );
			locY0 -= growth;
			locY1 += growth;
		}

		// Paranoid double check to make sure I did this correctly:
#ifdef _DEBUG
		{
			double dX = locX1 - locX0;
			double dY = locY1 - locY0;
			double AspectProvided = dY / dX;
			double AspectOfWindow = fAspect;
			assert( fabs( AspectProvided - AspectOfWindow ) < 0.00001 );
		}
#endif

		// Set the values that we just calculated.
		m_frustum.SetOrtho(locX0, locX1, locY0, locY1,  locZ0,  locZ1 );

		// invert and flip flop the Z planes to setup the Ortho Camera using the Utility!
		pDevice->SetOrthoMatrix(locX0, locX1, locY0, locY1, -locZ1, -locZ0 );
	}
	else
	{

		if( fabs(m_far-m_near) < 0.01f )
			return;

		if( fabs(sin(m_fieldOfView/2)) < 0.01f )
			return;

#ifdef TURKOWSKI_REV_2

		// X3D specifies fov to be the minimum field of view in either the horizontal or vertical directions
		double tanminfov = tan(m_fieldOfView * 0.5);
		double nfl = 1.0 / tanminfov;	// focal length, normalized to the smaller dimension
		double w;
		double h;
		double tanX = tanminfov;
		double tanY = tanminfov;

		if (fAspect < 1.0 )
		{
			w = nfl * fAspect;
			h = nfl;
			tanX /= fAspect;
		}
		else
		{
			w = nfl;
			h = nfl / fAspect;
			tanY *= fAspect;
		}

		tanX = 1.0 / w;
		tanY = 1.0 / h;

#else
		if (m_width > m_height)
			fAspect = ((double)m_height) / m_width;
		else
			fAspect = ((double)m_width) / m_height;

		double w = fAspect * (double)( cos(m_fieldOfView/2)/sin(m_fieldOfView/2) );
		double h =   1.0  * (double)( cos(m_fieldOfView/2)/sin(m_fieldOfView/2) );

#endif /* TURKOWSKI_REV_2 */


		double Q = (double) (m_far / (m_far - m_near));

		ZeroMemory( &m_projectionmatrix, sizeof(matrix4D) );
		m_projectionmatrix._11 = w;
		m_projectionmatrix._22 = h;
		m_projectionmatrix._33 = Q;
		m_projectionmatrix._34 = 1.0;
		m_projectionmatrix._43 = -Q*m_near;

		m_frustum.Set(m_position, m_direction, m_up, tanX, tanY, m_near, m_far);

		pDevice->SetProjectionMatrix(m_projectionmatrix);	
	}
}

void CAnmCamera::SetToDevice(CAnmDevice *pDevice)
{
	// set the light values, and enable/disable the light
	Point3 dir( m_direction );
	Point3 pos( m_position );
	m_headlight->SetDirection(&dir);
	m_headlight->SetLocation(&pos);
	color4 diffuseColor = color4::Black;
	color4 ambientColor = color4::Black;
	if (m_headlighton)
	{
		diffuseColor = ANMCAMERA_HEADLIGHTCOLOR;
		ambientColor = ANMCAMERA_AMBIENTCOLOR; 
	}

	m_headlight->SetDiffuseColor(&diffuseColor);
	m_headlight->SetAmbientColor(&ambientColor);

	pDevice->SetLight(m_lightindex, m_headlight);
	pDevice->EnableLight(m_lightindex, m_headlighton);

	// set the device's view matrix (view space)
	matrix4D imat;
	MatrixInvert(imat, m_matrix);
	pDevice->SetViewMatrix(imat);

	// now set the projection matrix
	SetProjectionMatrix(pDevice);
}

#define FLT_BIG  1.0e+10F

void CAnmCamera::GetPickRay(CAnmDevice *pDevice, int x, int y, Point3 *pFrom, Point3 *pTo)
{
	CAnmViewport *pViewport = pDevice->GetViewport();
	assert(pViewport);

	int height = pViewport->Height();
	int width = pViewport->Width();

	// turn x,y (down and to the right) into floating range -1..1 (up and to the right)
	double aspect = ((double)height) / width;


	if( m_bOrthographic ) {

		double normx = ( (double) x / ( double )(width ) );
		double normy = 1.0 - ( (double) y / ( double )(height ) );

		double xx = m_fOrthoX0 + normx*( m_fOrthoX1 - m_fOrthoX0 );
		double yy = m_fOrthoY0 + normy*( m_fOrthoY1 - m_fOrthoY0 );

		double fromZ =	m_near;
		double toZ =		m_far	;

		fromZ =	-FLT_BIG;
		toZ =	FLT_BIG;

		// from vector 
		Point3D FromVector(xx, yy, fromZ);
		*pFrom = m_matrix * FromVector;

		// TO vector.
		Point3D ToVector(xx, yy, toZ);
		*pTo = m_matrix * ToVector;

	}
	else {

		double normx = ((double) x / (width * .5f) - 1.0) / aspect;
		double normy = 1.0 - ((double) y / (height * .5f));

		double tanfov = tanf(m_fieldOfView * 0.5f);

		double dx = tanfov * normx;
		double dy = tanfov * normy;

		// from vector is on front clipping plane
		Point3D p3(dx * m_near, dy * m_near, m_near);
		
		// Get the World coordinates of the from vector by doing an inverse transform of the
		// camera matrix
		// matrix4D mat, vmat;
		// m_device->GetViewMatrix(&vmat);
		// MatrixInvert(mat, vmat);
		
		*pFrom = m_matrix * p3;

		// Now the to vector; z = back clipping plane
		p3 = Point3D(dx * m_far, dy * m_far, m_far);

		*pTo = m_matrix * p3;
	}
}
