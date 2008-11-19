/********************************************************************************
 * Flux
 *
 * File: anmsimvisitor.h
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

#ifndef _anmsimvisitor_h
#define _anmsimvisitor_h

#include "anmvisitor.h"

class CAnmSimVisitor : public CAnmVisitor
{
protected:

	class cApplication				*m_application;
	class CAnmPickManager			*m_pickmanager;
	Point3							 m_pickfrom;
	Point3							 m_pickto;

public:

	// constructor/destructor
	CAnmSimVisitor(class cApplication *pApp,
							class CAnmDevice *pDevice,
							class CAnmCamera *pCamera,
							BOOL bRender = TRUE,
							BOOL bPick = TRUE,
							BOOL bSavePath = TRUE,
							BOOL bCull = FALSE,
							BOOL bCollide = FALSE);


	virtual ~CAnmSimVisitor();

	// CAnmVisitor overrides
	virtual void Visit(class CAnmNode *pNode);

	// New methods
	virtual void Init();
};

#endif // _anmsimvisitor_h

