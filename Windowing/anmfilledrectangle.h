/********************************************************************************
 * Flux
 *
 * File: anmfilledrectangle.h
 * Description: Filled Rectangle class - abstracts Windows
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

#ifndef _anmfilledrectangle_h
#define _anmfilledrectangle_h

class CAnmFilledRectangle
{
protected:

	HDC							 m_hDC;
	HBRUSH						 m_hBrush;
	int							 m_x;
	int							 m_y;
	int							 m_width;
	int							 m_height;

public:

	CAnmFilledRectangle(int x, int y, int width = 0, int height = 0);
	virtual ~CAnmFilledRectangle();

	virtual void MoveTo(int x, int y);
	virtual void Resize(int width, int height);
	virtual void Render(class CAnmDevice *pDevice);
};

#endif // _anmfilledrectangle_h
