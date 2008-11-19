/********************************************************************************
 * Flux
 *
 * File: anmroute.h
 * Description: Route class
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

#ifndef _anmroute_h
#define _anmroute_h

#include "anmnode.h"
#include "anmcallback.h"

class CAnmRoute : public CAnmObject
{

protected:

	cApplication			*m_app;
	CAnmNode				*m_fromNode;
	CLASSMEMBERID			 m_fromCallback;
	CAnmNode				*m_toNode;
	CLASSMEMBERID			 m_toMethod;
	Boolean					 m_eventFlag;
	CAnmCallback			*m_callbackPtr;
	class CAnmEvent			*m_event;

public:

	// constructor/destructor
	CAnmRoute(cApplication *pApp,
				CAnmNode				*pFromNode,
				CLASSMEMBERID		fromCallback,
				CAnmNode			*pToNode,
				CLASSMEMBERID		toMethod,
				Boolean				eventFlag = FALSE);

	virtual ~CAnmRoute();

	// Methods
	void Fire(void *pData);

	// Accessors
	void SetFromNode(CAnmNode *pFromNode);
	CAnmNode *GetFromNode() { return m_fromNode; }

	void SetFromCallback(CLASSMEMBERID fromCallback);
	CLASSMEMBERID GetFromCallback() { return m_fromCallback; }

	void SetToNode(CAnmNode *pToNode);
	CAnmNode *GetToNode() { return m_toNode; }

	void SetToMethod(CLASSMEMBERID toMethod);
	CLASSMEMBERID GetToMethod() { return m_toMethod; }
};

#endif // _anmroute_h
