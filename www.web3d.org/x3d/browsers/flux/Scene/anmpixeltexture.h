/********************************************************************************
 * Flux
 *
 * File: anmpixeltexture.h
 * Description: PixelTexture node
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

#ifndef _anmpixeltexture_h
#define _anmpixeltexture_h

#include "anmtexture.h"

class CAnmPixelTexture : public CAnmTexture
{

protected:
	
	Image				*m_image;

public:

	// constructor/destructor
	CAnmPixelTexture();
	virtual ~CAnmPixelTexture();

	// CAnmTexture overrides
	virtual void Realize();				// build lower-level rendering structures

	// Accessors
	virtual void SetImage(Image *pImage);
	virtual Image *GetImage()
	{
		return m_image;
	}
	virtual void GetImage (Image **ppImage)
	{
		assert(ppImage);
		*ppImage = m_image;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmPixelTexture);

	DECLARE_VALUE(image);
	DECLARE_VALUE(repeatS);
	DECLARE_VALUE(repeatT);
};


#endif // _anmpixeltexture_h
