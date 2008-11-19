/********************************************************************************
 * Flux
 *
 * File: anmtexturedata.h
 * Description: TextureData class - loads image
 *							data for texture handling
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


#ifndef _anmtexturedata_h
#define _anmtexturedata_h

#include "anmobject.h"

enum eAnmTextureDataFormat
{
	eAnmTextureBMP,
	eAnmTextureJPG,
	eAnmTexturePNG,
	eAnmTextureGIF,
	eAnmTextureGenerated,
	eAnmTextureBadFormat,
};

class EXPORT CAnmTextureData : public CAnmObject
{
protected:

	String							 m_filename;
	eAnmTextureDataFormat			 m_format;
	int								 m_width;
	int								 m_height;

	// krv:  Changed class BitmapImage to simple Byte Array.
	//
	unsigned char*					 m_pPixelData;

	bool							 m_hasalpha;
	bool							 m_grayscale;
	// BOOL							 m_hastransparentcolor;
	// ULONG							 m_transparentcolor;
	bool							 m_imagevalid;
	bool							 m_mipmap;

	static CAnmTextureData *FindTextureData(cApplication *pApp, const char *filename);

public:

	// constructor/destructor
	CAnmTextureData(const char *filename, eAnmTextureDataFormat format);

	CAnmTextureData( int width, int height, 
					 bool bHasAlpha, bool bGrey = false , bool mipmap = true);


	~CAnmTextureData();

	// Methods
	static CAnmTextureData *LoadTextureData(cApplication *pApp, const char *filename, eAnmTextureDataFormat format);
	static void UnloadAllTextureData();

	// Accessors
	virtual String GetFilename()
	{
		return m_filename;
	}

	virtual eAnmTextureDataFormat GetFormat()
	{
		return m_format;
	}

	virtual int GetWidth()
	{
		return m_width;
	}

	virtual int GetHeight()
	{
		return m_height;
	}

	virtual unsigned char* GetPixelData()
	{
		return m_pPixelData;
	}

	virtual bool ImageValid()
	{
		return m_imagevalid;
	}

	virtual bool HasAlpha()
	{
		return m_hasalpha;
	}

	virtual bool IsGrayScale()
	{
		return m_grayscale;
	}

	virtual bool IsMipmapped()
	{
		return m_mipmap;
	}

};

class EXPORT CAnmBitmapTextureData : public CAnmTextureData
{
protected :

public :

	CAnmBitmapTextureData(const char *filename);
};

class EXPORT CAnmJPEGTextureData : public CAnmTextureData
{
protected :

public :

	CAnmJPEGTextureData(const char *filename);
};

class EXPORT CAnmPNGTextureData : public CAnmTextureData
{
protected :

public :

	CAnmPNGTextureData(const char *filename);
};

class EXPORT CAnmGIFTextureData : public CAnmTextureData
{
protected :

public :

	CAnmGIFTextureData(const char *filename);
};

#endif //_anmtexturedata_h
