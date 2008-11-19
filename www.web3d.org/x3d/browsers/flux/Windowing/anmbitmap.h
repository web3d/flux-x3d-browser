/********************************************************************************
 * Flux
 *
 * File: anmbitmap.h
 * Description: Bitmap class - abstracts Windows bitmaps
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

#ifndef _anmbitmap_h
#define _anmbitmap_h

class CAnmBitmap
{
protected:

	HDC							 m_hDC;
	HBITMAP						 m_hBitmap;
	bool						 m_hasalpha;
	void						*m_bits;
	int							 m_x;
	int							 m_y;
	int							 m_width;
	int							 m_height;

	void InitBitmap(class CAnmTextureData *pTextureData, int width, int height);
	void CopyDataToBitmap(class CAnmTextureData *pTextureData);

public:

	CAnmBitmap(class CAnmTextureData *pTextureData, int x, int y, int width = 0, int height = 0);
	virtual ~CAnmBitmap();

	virtual void MoveTo(int x, int y);
	virtual void Render(class CAnmDevice *pDevice);
};

#endif // _anmbitmap_h
