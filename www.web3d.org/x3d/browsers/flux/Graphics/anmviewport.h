/********************************************************************************
 * Flux
 *
 * File: anmviewport.h
 * Description: Viewport class
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

#ifndef _anmviewport_h
#define _anmviewport_h

class EXPORT CAnmViewport : public CAnmObject
{

protected:

	class CAnmDevice		*m_device;
	int						 m_width;
	int						 m_height;
	class CAnmCamera		*m_camera;

	void MakeD3DViewport();

public:

	CAnmViewport(class CAnmDevice *pDevice, int width, int height);
	~CAnmViewport();

	// Resize/redisplay etc.
	virtual void HandleSize(int newwidth, int newheight);

	// Accessors
	virtual int Width() const
	{
		return m_width;
	}

	virtual int Height() const
	{
		return m_height;
	}
};

#endif // _anmviewport_h
