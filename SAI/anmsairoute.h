/********************************************************************************
 * Flux
 *
 * File: anmsairoute.h
 * Description: Route wrapper class for SAI
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

#ifndef _anmsairoute_h
#define _anmsairoute_h

class EXPORT CAnmSAIRoute : public CAnmObject
{
protected:

	// pointer to the real node
	class CAnmRoute				*m_route;
	class CAnmSAINode			*m_fromNode;
	class CAnmSAINode			*m_toNode;


public:

	// Constructor/destructor
	CAnmSAIRoute(class CAnmRoute *pRoute, class CAnmSAINode *pFromSAINode,
		class CAnmSAINode *pToSAINode);
	virtual ~CAnmSAIRoute();

	// Accessors
	class CAnmRoute *GetRoute()
	{
		return m_route;
	}

	class CAnmSAINode *GetFromNode()
	{
		return m_fromNode;
	}

	class CAnmSAINode *GetToNode()
	{
		return m_toNode;
	}

	DECLARE_CLASS(CAnmSAIRoute);
};


#endif // _anmsairoute_h
