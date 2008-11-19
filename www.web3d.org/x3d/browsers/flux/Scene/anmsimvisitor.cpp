/********************************************************************************
 * Flux
 *
 * File: anmsimvisitor.cpp
 * Description: Simulation Visitor class
 *							Traverses scene graph for runtime simulation
 *
 *						See anmvisitor.h for details on Visitor
 *						paradigm.
 *						
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
#include "anmsimvisitor.h"
#include "application.h"
#include "anmcamera.h"
#include "anmdevice.h"
#include "anmnode.h"
#include "anmsensor.h"
#include "anmpicksensor.h"
#include "anmpickmanager.h"
#include "anmviewport.h"

// constructor/destructor
CAnmSimVisitor::CAnmSimVisitor(cApplication *pApp,
							   CAnmDevice *pDevice,
							   CAnmCamera *pCamera,
							   BOOL bRender,
							   BOOL bPick,
							   BOOL bSavePath,
							   BOOL bCull,
							   BOOL bCollide
							   )
 : CAnmVisitor(pDevice, pCamera, bRender, bPick, bSavePath, bCull, bCollide),
   m_application(pApp)
{
	m_pickmanager = m_application->GetPickManager();
}

CAnmSimVisitor::~CAnmSimVisitor()
{
}

void CAnmSimVisitor::Init()
{
	assert(m_application);

	// if the app has picking sensors, initialize and get the ray

	m_dopick = FALSE;

	std::list< CAnmSensor * > *pSensors =
		m_application->GetSensors();

	std::list< CAnmSensor * >::iterator iter;
	for( 
		iter = pSensors->begin(); 
		iter != pSensors->end();
		iter++ )
	{
		// N.B.: this is hack city; maybe we can find a cleaner design soon...
		if ((*iter)->GetClass()->IsA(GETCLASS(CAnmPickSensor)) &&
			(*iter)->GetEnabled())
		{
			m_dopick = TRUE;
		}
	}

	if (m_dopick)
	{
		int mousex = m_application->MouseX();
		int mousey = m_application->MouseY();

		m_camera->GetPickRay(m_device, mousex, mousey, &m_pickfrom, &m_pickto);

		m_pickmanager->Init(mousex, mousey, m_pickfrom, m_pickto);
	}

}

void CAnmSimVisitor::Visit(CAnmNode *pNode)
{
	assert(pNode != NULL);

	BOOL culled = FALSE;
/*
	N.B.: eventually do this

	if (m_cullobjects)
		culled = pNode->TestCull(m_device);
*/

	if (m_dorender && !culled)
	{
		// pNode->Render(m_device);
	}

	if (m_dopick && !culled)
	{
		// N.B.: obsolete way of picking directly against the scene graph instead of the render graph->
		// we may even want to remove this vistor class
		// TP 9/18/03
		// pNode->TestPick(m_device, m_pickfrom, m_pickto, m_pickmanager->GetPickList());
	}

	if (m_testcollisions)	// N.B.: should we test against culled objects?
	{
		// need to add TestCollision() method
		// pNode->TestCollision(m_device, radius, collidelist);
	}


}



