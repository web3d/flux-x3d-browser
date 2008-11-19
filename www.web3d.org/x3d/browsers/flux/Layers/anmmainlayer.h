/********************************************************************************
 * Flux
 *
 * File: anmmainlayer.h
 * Description: Main Layer node for the main scene
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

#ifndef _anmmainlayer_h
#define _anmmainlayer_h

#include "anmlayer3D.h"

class CAnmMainLayer : public CAnmLayer3D
{
protected : 

	class cApplication			*m_app;

	std::list<class CAnmViewpoint *>::iterator
								 m_viewpointIter;


	virtual void SetNavigationInfo(class CAnmNavigationInfo *pNavigationInfo);
	virtual void SetViewpoint(class CAnmViewpoint *pViewpoint);
	virtual void UpdateBindables();

public :

	// Constructor/destructor
	CAnmMainLayer(class cApplication *pApp);
	virtual ~CAnmMainLayer();

	// CAnmLayer3D overrides
	virtual void Realize();
	virtual bool IsMainLayer()
	{
		return true;
	}

	virtual void Ref()
	{
		m_refcount++;
	}

	virtual void UnRef()
	{
		--m_refcount;

		if (m_refcount == 0)
			delete this;
		else if (m_refcount < 0)
			assert(FALSE);
	}

	// New methods
	virtual void FirstViewpoint();
	virtual void LastViewpoint();
	virtual void NextViewpoint();
	virtual void PreviousViewpoint();

	// Accessors
};

#endif // _anmmainlayer_h
