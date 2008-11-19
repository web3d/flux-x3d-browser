/********************************************************************************
 * Flux
 *
 * File: mouse.h
 * Description: Mouse manager
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

#ifndef _MOUSE_H
#define _MOUSE_H

/**
 * Any object that implements this interface can receive input
 * from the mouse.
 */
struct iMouseReceiver
{
	virtual void MouseMoved( int dx, int dy ){};
	virtual void MouseButtonUp( int button ){};
	virtual void MouseButtonDown( int button ){};
};


class EXPORT cMouse  
{
protected :
	
	iMouseReceiver*			m_pTarget;

	class cApplication		*m_app;

	cMouse( class cApplication *pApp, HWND hWnd, bool bExclusive );

public:

	~cMouse();

	/**
	 * Use this to establish a MouseReceiver as the current 
	 * input focus
	 */
	void SetReceiver( iMouseReceiver* pTarget );

	// Overridables - subclass MUST supply
	virtual eResult Update() PURE;
	virtual eResult Acquire() PURE;
	virtual void UnAcquire() PURE;
};

#endif // _MOUSE_H
