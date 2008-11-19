/********************************************************************************
 * Flux
 *
 * File: anmcylindersensor.cpp
 * Description: CylinderSensor class
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
#include "anmcylindersensor.h"
#include "anmquaternion.h"

#define CYLINDERSENSOR_DEFAULT_AUTOOFFSET		TRUE
#define CYLINDERSENSOR_DEFAULT_DISKANGLE		0.262f
#define CYLINDERSENSOR_DEFAULT_ENABLED			TRUE
#define CYLINDERSENSOR_DEFAULT_MAXANGLE			-1.f
#define CYLINDERSENSOR_DEFAULT_MINANGLE			0.f
#define CYLINDERSENSOR_DEFAULT_OFFSET			0.f

CAnmCylinderSensor::CAnmCylinderSensor()
: CAnmPickSensor(CYLINDERSENSOR_DEFAULT_ENABLED),
m_offset(CYLINDERSENSOR_DEFAULT_OFFSET),
m_autoOffset(CYLINDERSENSOR_DEFAULT_AUTOOFFSET),
m_radius(0),
m_diskAngle(CYLINDERSENSOR_DEFAULT_DISKANGLE),
m_minAngle(CYLINDERSENSOR_DEFAULT_MINANGLE),
m_maxAngle(CYLINDERSENSOR_DEFAULT_MAXANGLE),
m_hitpoint(ANMPICKSENSOR_BADPOINTVALUE),
m_trackPoint(ANMPICKSENSOR_BADPOINTVALUE),
m_lastRotation(Rotation(0, 1, 0, CYLINDERSENSOR_DEFAULT_OFFSET)),
m_yDisk( 0.0f ),
m_PrevAngle( 0.0f ),
m_PrevAngle2( 0.0f ),
m_InitAngle( 0.0f ),
m_LastCylAngle( 0.0f ),
m_DiskAngleMode( 0 ),
m_fsign( 1.0 ),
m_bLastMode( false )
{
}

CAnmCylinderSensor::~CAnmCylinderSensor()
{
}

void CAnmCylinderSensor::Realize()
{
	CAnmPickSensor::Realize();
}

void CAnmCylinderSensor::Update()
{
	ClearStateDirty();
}

#define CYLINDERSENSOR_EPSILON 0.000001

void CAnmCylinderSensor::HandleActive(Boolean active, Point3 from, Point3 to, Point3 hitpoint)
{
	if (active)
	{
		// get initial track point, and compute radius (magnitude of vector)
		m_hitpoint = hitpoint;
		// Determine DiskAngle Mode
		//

		Point3 vBearing = ( to - from );
		vBearing.Normalize();
		// Get the Bearing Vector doted by the Y Axis.
		float diskDot = fabs( vBearing.y );
		float diskAngle;
		if( diskDot > 0.999 ) {
			diskAngle = 0.0f;
		}
		else if( diskDot < -0.999 ) {
			diskAngle = 3.1415;
		}
		else {
			diskAngle = acos( diskDot );
		}

		// Compare to disk angle, and get the DiskAngleMode.
		m_DiskAngleMode = ( ( diskAngle < m_diskAngle ) ? DA_DISK : DA_CYLINDER );

		if( m_DiskAngleMode == DA_DISK ) {
			// Calculate the Y value of the Disk.  The Disk is in the X-Z Plane, at the
			// coincedent with the intersection point.
			m_yDisk = hitpoint.y;
		}
		else {
			// In Cylinder Mode, get the Radius.
			m_radius = sqrt( hitpoint.x*hitpoint.x + hitpoint.z*hitpoint.z );
		}

		// Get the angle and the TrackPoint
		//
		m_trackPoint = hitpoint;
		Point3 newTrackPoint;
		CalcDragAngle( from, vBearing, newTrackPoint, m_InitAngle );
		m_PrevAngle = m_InitAngle;

		m_lastRotation = Rotation(0, 1, 0, m_offset);
	}
	else
	{
		if (m_autoOffset)
			SetOffset(m_lastRotation.theta);
	}

	// chain down to get basic activate/deactive behavior for all picking sensors
	CAnmPickSensor::HandleActive(active, from, to, hitpoint);
}

void CAnmCylinderSensor::HandleMove(Point3 from, Point3 to, Point3 hitpoint)
{

	Point3 newTrackPoint;
	Point3 vBearing = ( to - from );
	vBearing.Normalize();
	float newAngle = 0;
	if( CalcDragAngle( from, vBearing, newTrackPoint, newAngle ) ) {
		if( fabs( newAngle - m_PrevAngle ) > 0.001 ) {
			m_PrevAngle2 = m_PrevAngle;
			m_PrevAngle = newAngle;
			m_lastRotation.theta = newAngle - m_InitAngle + m_offset;
			m_trackPoint = newTrackPoint;

			CallCallbacks(CALLBACKID(CAnmCylinderSensor, trackPoint_changed), &m_trackPoint);
			CallCallbacks(CALLBACKID(CAnmCylinderSensor, rotation_changed), &m_lastRotation);
		}
	}


}



float static dist2D( float vec1[], float vec2[] )
{
	float dx = vec1[0] - vec2[0];
	float dy = vec1[1] - vec2[1];
	float mag = dx*dx + dy*dy;
	return sqrt( mag );
}



void static Normalize2D( float vec[] )
{
	float mag = vec[0]*vec[0] + vec[1]*vec[1];
	mag = sqrt( mag );
	vec[0] /= mag;
	vec[1] /= mag;
}



int static slv2x2( float* matx, float* cnst, float* vout )
{
	float det = matx[0]*matx[3] - matx[1]*matx[2];
	if( fabs( det ) < 0.00001 ) {
		vout[0] = vout[1] = 0.0f;
		return FALSE;
	}

	vout[0] = ( cnst[0]*matx[3] - cnst[1]*matx[2] ) / det;
	vout[1] = ( cnst[1]*matx[0] - cnst[0]*matx[1] ) / det;

	return TRUE;
}


bool CAnmCylinderSensor::CalcDragAngle( Point3 from, Point3 vBearing, Point3& trackPointOut, float& angleOut )
{
	bool bStatus = false;
	float angle = 0.0f;
	if( m_DiskAngleMode == DA_DISK ) {
		// Find the intersection between the disk at m_yDisk, X-Z plane, and the Bearing Vector.
		//
		if( fabs( vBearing.y ) < 0.00001 ) {
			return bStatus;
		}
		float tlamda = ( m_yDisk - from.y ) / vBearing.y;
		trackPointOut = from + tlamda*vBearing;

		if( fabs( trackPointOut.x ) < 0.00001 && fabs( trackPointOut.z ) < 0.00001 ) {
			return bStatus;
		}
		angle = atan2( -trackPointOut.z, trackPointOut.x );
		bStatus = true;
	}
	else {
		// Find the intersection of the invisible cylinder of radius m_Radus and the vector.
		//
		float V2Da[2];
		float V2Db[2];
		float Vconst[2];
		float matx[4];
		float lamda[2];
		float PInt[2];
		float PInt1[2];
		float PInt2[2];
		// find the solutin to:
		// from = V2Da * lamdaA + V2Db * lamdaB;
		// Squash to 2D, ignore Y.

		// Get the 2D Bearing Angle.
		//
		V2Db[0] = vBearing.x;
		V2Db[1] = -vBearing.z;
		// Get a vector that is orthogonal
		//
		V2Da[0] = vBearing.z;
		V2Da[1] = vBearing.x;

		Normalize2D( V2Da );
		Normalize2D( V2Db );

		matx[0] = V2Da[0];
		matx[1] = V2Da[1];
		matx[2] = V2Db[0];
		matx[3] = V2Db[1];

		Vconst[0] = from.x;
		Vconst[1] = -from.z;

		// solve 
		if( slv2x2( matx, Vconst, lamda ) ) {

			float lenBSqr = m_radius*m_radius - lamda[0]*lamda[0];
			if( lenBSqr > 0.00001 ) {
				float lenB = sqrt( lenBSqr );

				// Get the two points that intersect the circle.
				//
				PInt1[0] = V2Da[0]*lamda[0] + V2Db[0]*lenB;
				PInt1[1] = V2Da[1]*lamda[0] + V2Db[1]*lenB;

				PInt2[0] = V2Da[0]*lamda[0] - V2Db[0]*lenB;
				PInt2[1] = V2Da[1]*lamda[0] - V2Db[1]*lenB;

				// Take the one that is closest to the From point.
				//
				if( dist2D( Vconst, PInt1 ) < dist2D( Vconst, PInt2 ) ) {
					PInt[0] = PInt1[0];
					PInt[1] = PInt1[1];
				}
				else {
					PInt[0] = PInt2[0];
					PInt[1] = PInt2[1];
				}

				// Calc the angle with the closest interection point.
				//
				float lamdaTrack = 0.0f;
				angle = atan2( PInt[1], PInt[0] );
				m_LastCylAngle = angle;
				if( fabs( V2Db[0] ) > fabs( V2Db[1] ) ) {
					lamdaTrack = ( PInt[0] - Vconst[0] ) / V2Db[0];
				}
				else {
					lamdaTrack = ( PInt[1] - Vconst[1] ) / V2Db[1];
				}
				trackPointOut = from + lamdaTrack*vBearing;
		
				m_bLastMode = true;
				bStatus = true;
			}
			else {
				// The pick missed the invisible cyinder
				// Get an angle beyond the cylinder
				//
				float lenB = sqrt( -lenBSqr );

				if( m_bLastMode ) {
					if( m_PrevAngle2 < m_PrevAngle ) {
						m_fsign = 1.0;
					}
					else {
						m_fsign = -1.0;
					}
				}

				angle = m_LastCylAngle + m_fsign*lenB /m_radius;
				bStatus = true;
				m_bLastMode = false;

			}
		}
	}
	angleOut = angle;

	// try clamping the angle
	if( m_minAngle < m_maxAngle ) {
		if( angleOut < m_minAngle ) {
			angleOut = m_minAngle;
		}
		else if( angleOut > m_maxAngle ) {
			angleOut = m_maxAngle;
		}
	}

	return bStatus;
}

cCursor CAnmCylinderSensor::GetOverCursor()
{
	return cApplication::cylinderSensorCursor;
}

cCursor CAnmCylinderSensor::GetActiveCursor()
{
	return cApplication::cylinderSensorCursor;
}

// Accessors
void CAnmCylinderSensor::SetAutoOffset(Boolean autoOffset)
{
	m_autoOffset = autoOffset;
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmCylinderSensor, autoOffset_changed), &autoOffset);
}

void CAnmCylinderSensor::SetDiskAngle(Float diskAngle)
{
	m_diskAngle = diskAngle;
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmCylinderSensor, diskAngle_changed), &diskAngle);
}

void CAnmCylinderSensor::SetMinAngle(Float minAngle)
{
	m_minAngle = minAngle;
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmCylinderSensor, minAngle_changed), &minAngle);
}

void CAnmCylinderSensor::SetMaxAngle(Float maxAngle)
{
	m_maxAngle = maxAngle;
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmCylinderSensor, maxAngle_changed), &maxAngle);
}

void CAnmCylinderSensor::SetOffset(Float offset)
{
	m_offset = offset;
	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmCylinderSensor, offset_changed), &offset);
}


// Tables for Callbacks, Methods and Set/GetValues
DEFINE_VALUE(CAnmCylinderSensor, autoOffset, eValueBoolean, Boolean, GetAutoOffset, SetAutoOffset);
DEFINE_VALUE(CAnmCylinderSensor, diskAngle, eValueFloat, Float, GetDiskAngle, SetDiskAngle);
DEFINE_INHERITED_VALUE(CAnmCylinderSensor, enabled, CAnmPickSensor);
DEFINE_VALUE(CAnmCylinderSensor, minAngle, eValueFloat, Float, GetMinAngle, SetMinAngle);
DEFINE_VALUE(CAnmCylinderSensor, maxAngle, eValueFloat, Float, GetMaxAngle, SetMaxAngle);
DEFINE_VALUE(CAnmCylinderSensor, offset, eValueFloat, Float, GetOffset, SetOffset);

DEFINE_METHOD(CAnmCylinderSensor, set_autoOffset, eValueBoolean, Boolean, SetAutoOffset);
DEFINE_METHOD(CAnmCylinderSensor, set_diskAngle, eValueFloat, Float, SetDiskAngle);
DEFINE_INHERITED_METHOD(CAnmCylinderSensor, set_enabled, CAnmPickSensor);
DEFINE_METHOD(CAnmCylinderSensor, set_minAngle, eValueFloat, Float, SetMinAngle);
DEFINE_METHOD(CAnmCylinderSensor, set_maxAngle, eValueFloat, Float, SetMaxAngle);
DEFINE_METHOD(CAnmCylinderSensor, set_offset, eValueFloat, Float, SetOffset);

DEFINE_CALLBACK(CAnmCylinderSensor, autoOffset_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmCylinderSensor, diskAngle_changed, eValueFloat);
DEFINE_INHERITED_CALLBACK(CAnmCylinderSensor, enabled_changed, CAnmPickSensor);
DEFINE_CALLBACK(CAnmCylinderSensor, minAngle_changed, eValueFloat);
DEFINE_CALLBACK(CAnmCylinderSensor, maxAngle_changed, eValueFloat);
DEFINE_CALLBACK(CAnmCylinderSensor, offset_changed, eValueFloat);
DEFINE_CALLBACK(CAnmCylinderSensor, rotation_changed, eValueRotation);
DEFINE_CALLBACK(CAnmCylinderSensor, trackPoint_changed, eValuePoint3);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmCylinderSensor)
VALUEID(CAnmCylinderSensor, autoOffset),
VALUEID(CAnmCylinderSensor, diskAngle),
VALUEID(CAnmCylinderSensor, enabled),
VALUEID(CAnmCylinderSensor, minAngle),
VALUEID(CAnmCylinderSensor, maxAngle),
VALUEID(CAnmCylinderSensor, offset),

METHODID(CAnmCylinderSensor, set_autoOffset),
METHODID(CAnmCylinderSensor, set_diskAngle),
METHODID(CAnmCylinderSensor, set_enabled),
METHODID(CAnmCylinderSensor, set_minAngle),
METHODID(CAnmCylinderSensor, set_maxAngle),
METHODID(CAnmCylinderSensor, set_offset),

CALLBACKID(CAnmCylinderSensor, autoOffset_changed),
CALLBACKID(CAnmCylinderSensor, diskAngle_changed),
CALLBACKID(CAnmCylinderSensor, enabled_changed),
CALLBACKID(CAnmCylinderSensor, minAngle_changed),
CALLBACKID(CAnmCylinderSensor, maxAngle_changed),
CALLBACKID(CAnmCylinderSensor, offset_changed),
CALLBACKID(CAnmCylinderSensor, rotation_changed),
CALLBACKID(CAnmCylinderSensor, trackPoint_changed),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmCylinderSensor, CAnmPickSensor);
