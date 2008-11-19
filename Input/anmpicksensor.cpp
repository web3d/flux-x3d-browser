/********************************************************************************
 * Flux
 *
 * File: anmpicksensor.cpp
 * Description: PickSensor class
 *							base class for mouse-based picking
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
#include "anmpicksensor.h"
#include "anmpickmanager.h"
#include "anmlayer.h"
#include "anmproto.h"
#include "anmtransform.h"
#include "anmworld.h"
#include "anmMatrixInvert.h"

CAnmPickSensor::CAnmPickSensor(Boolean enabled)
: CAnmSensor(enabled)
{
	m_active = FALSE;
	m_over = FALSE;
	m_localmatrix = matrix4::Identity;
	m_mousex = ANMPICKSENSOR_BADMOUSEVALUE;
	m_mousey = ANMPICKSENSOR_BADMOUSEVALUE;
	m_mousedowntime = ANMTIME_BADTIMEVALUE;
}

CAnmPickSensor::~CAnmPickSensor()
{
}

void CAnmPickSensor::Realize()
{
	CAnmSensor::Realize();
}


void CAnmPickSensor::Poll()
{
	// shouldn't be here otherwise
	assert(m_enabled);

	CAnmWorld *pWorld = GetWorld();

	if (pWorld == NULL)
		return;

	cApplication *pApp = pWorld->GetApp();
	assert(pApp);

	// If there's an exclusive grab going on, bail out
	if (pApp->GetExclusiveGrab())
		return;

	// Make sure all the data is up to date
	if (StateDirty())
		Update();

	// Get mouse position and pick ray from the pick manager
	CAnmLayer *pLayer = GetLayer();
	assert(pLayer);

	CAnmPickManager *pPickManager = pLayer->GetPickManager();
	assert(pPickManager);

	int mousex = pPickManager->GetMouseX();
	int mousey = pPickManager->GetMouseY();
	int mousedown = 0;
	Point3 from, to;
	pPickManager->GetPickRay(&from, &to);

	// See if we're over the mouse
	BOOL over;

	sAnmSceneGraphPickDesc pickDesc;
	CAnmGroup *pPickParent = GetPickParent(pPickManager, &pickDesc);

	over = (pPickParent != NULL);

	// Get the new intersection point
	Point3 badpointval = ANMPICKSENSOR_BADPOINTVALUE;
	Point3 newhitpoint = badpointval;
	matrix4 imat;

	// Transform WC points into LC's
	if (m_active)
	{
		// m_localmatrix is set up when we are activated
		MatrixInvert(imat, m_localmatrix);

		newhitpoint = imat * pickDesc.hitPoint;
		from = imat * from;
		to = imat * to;
	}


	// Handle over/not over state change
	if (over && !m_over)
	{
		pApp->SetCursor(GetOverCursor());
		HandleOver(TRUE);
	}
	else if (!over && m_over)
	{
		// we went from being over our objects to not being over;
		// set the cursor back
		pApp->SetDefaultCursor();
		HandleOver(FALSE);
	}

	// Get current mouse down state from the app
	mousedown = pApp->GetMouseDown();

	// Let's squirrel away when the mouse goes down
	CAnmTime *pClock = pApp->GetClock();
	assert(pClock);

	Time now = pClock->GetCurrentTime();

	if (mousedown)
	{
		if (m_mousedowntime == ANMTIME_BADTIMEVALUE)
			m_mousedowntime = now;
	}
	else
		m_mousedowntime = ANMTIME_BADTIMEVALUE;

	// See if we've already grabbed the mouse
	BOOL grabbed = pApp->FindGrabObject(this);
	if (grabbed)
	{
		// If it's down and we're already grabbed, this is just a move.
		// Only do it if the mouse coords are different
		if (mousedown & ANM_PRIMARYBUTTON)
		{
			if (mousex != m_mousex || mousey != m_mousey)
			{
				HandleMove(from, to, newhitpoint);
				m_mousex = mousex;
				m_mousey = mousey;
			}

		}
		else
		{
			// If the mouse is not down and we're grabbed, let go of it and deactivate
			pApp->RemoveGrabObject(this);
			pApp->SetDefaultCursor();
			HandleActive(FALSE, badpointval, badpointval, badpointval);
			m_mousex = ANMPICKSENSOR_BADMOUSEVALUE;
			m_mousey = ANMPICKSENSOR_BADMOUSEVALUE;
		}
	}
	else
	{
		if (over)
		{
			// We weren't grabbed, and now we're clicking.
			// Grab the mouse and activate the sensor
			if ((mousedown & ANM_PRIMARYBUTTON) && m_mousedowntime >= now)
			{
				pApp->AddGrabObject(this);

				pApp->SetCursor(GetActiveCursor());

				// Save the local matrix; we'll need it while we're active
				CAnmTransform::GetLocalMatrix(pPickParent, pickDesc.pParentArray,
					pickDesc.nParents, &m_localmatrix);
				
				// Transform all WC's into LC's
				MatrixInvert(imat, m_localmatrix);

				newhitpoint = imat * pickDesc.hitPoint;
				from = imat * from;
				to = imat * to;
		
				HandleActive(TRUE, from, to, newhitpoint);
				HandleMove(from, to, newhitpoint);

				m_mousex = mousex;
				m_mousey = mousey;
			}
		}
	}

}

void CAnmPickSensor::HandleOver(Boolean over)
{
	if (over != m_over)
	{
		m_over = over;
		CallCallbacks(CALLBACKID(CAnmPickSensor, isOver), &m_over);
	}
}

void CAnmPickSensor::HandleActive(Boolean active, Point3 from, Point3 to, Point3 hitpoint)
{
	if (active != m_active)
	{
		m_active = active;
		CallCallbacks(CALLBACKID(CAnmPickSensor, isActive), &m_active);
	}
}

cCursor CAnmPickSensor::GetOverCursor()
{
	return cApplication::helpCursor;
}

cCursor CAnmPickSensor::GetActiveCursor()
{
	return cApplication::helpCursor;
}

// Some helpers
// Find my parent that contains the picked geometry
static inline CAnmGroup *GetFirstRealGroup(CAnmGroup *pGroup)
{
	while (pGroup->GetClass()->IsA(GETCLASS(CAnmProto)))
	{
		pGroup = pGroup->GetFirstParent();
		if (pGroup == NULL)
			break;
	}

	return pGroup;
}

CAnmGroup *CAnmPickSensor::GetPickParent(CAnmPickManager *pPickManager, sAnmSceneGraphPickDesc *pDesc)
{
	std::list< CAnmGroup* >::iterator iter;

	for( 
		iter = m_parents.begin(); 
		iter != m_parents.end();
		iter++ )
	{
		CAnmGroup *pGroup = GetFirstRealGroup(*iter);
		if (pGroup && pPickManager->TestPick(pGroup, pDesc))
			return (pGroup);
	}

	if (m_owner && m_owner->GetClass()->IsA(GETCLASS(CAnmGroup)))
	{
		if (pPickManager->TestPick((CAnmGroup *) m_owner, pDesc))
			return (CAnmGroup *) m_owner;
	}

	return NULL;
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_CALLBACK(CAnmPickSensor, isOver, eValueBoolean);
DEFINE_INHERITED_CALLBACK(CAnmPickSensor, isActive, CAnmSensor);

// member table
BEGIN_DEFINE_MEMBERS(CAnmPickSensor)
CALLBACKID(CAnmPickSensor, isOver),
CALLBACKID(CAnmPickSensor, isActive),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmPickSensor, CAnmSensor);
