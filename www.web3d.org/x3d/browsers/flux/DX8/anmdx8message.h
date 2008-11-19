/********************************************************************************
 * Flux
 *
 * File: anmdx8message.h
 * Description: DirectX 8 Message definitions
 *
 *		We *could* use Window string resources, but they're not
 *			portable and they're hardly better than #defines in 
 *			practice-- TP
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

#ifndef _anmdx8message_h
#define _anmdx8message_h

#define ANMMSG_VBCREATEFAILED			"Failed to create vertex buffer"
#define ANMMSG_VBLOCKFAILED				"Failed to lock vertex buffer"
#define ANMMSG_IBCREATEFAILED			"Failed to create index buffer"
#define ANMMSG_VIEWPORTSETFAILED		"Viewport setting failed"
#define ANMMSG_D3DCREATE8FAILED			"Direct3DCreate8 failed"
#define ANMMSG_GETADAPTERDISPLAYMODEFAILED	"GetAdapterDisplayMode failed"
#define ANMMSG_NOAPPROPRIATEDEVICE		"An appropriate device cannot be created"
#define ANMMSG_COULDNTCONFIRMDEVICE		"Could not confirm device"
#define ANMMSG_ERRORCREATINGDEVICE		"Error creating device: %s"
#define ANMMSG_ERRORRESETDEVICE			"Error on Reset Device: %s"
#define ANMMSG_GETSURFACELEVELFAILED	"GetSurfaceLevel failed"
#define ANMMSG_GETDESCFAILED			"GetDesc failed"
#define ANMMSG_CREATEIMAGESURFACEFAILED	"CreateImageSurface failed"
#define ANMMSG_LOCKRECTFAILED			"LockRect failed"
#define ANMMSG_UNKNOWNRGBBITCOUNTCD2S	"Unknown dwRGBBitCount [CAnmJPEGTextureData::CopyDataToSurface]"
#define ANMMSG_UNLOCKRECTFAILED			"UnlockRect failed"
#define ANMMSG_GETSURFACELEVELFAILEDMIP	"GetSurfaceLevel failed [CAnmD3DTextureSurface8::GenMipMap]"
#define ANMMSG_UNKNUMBITSPARENTMIP		"Unknown numBitsInParentFormat [CAnmD3DTextureSurface8::GenMipMapLevel]"
#define ANMMSG_UNKNUMBITSCHILDMIP		"Unknown numBitsInChildFormat [CAnmD3DTextureSurface8::GenMipMapLevel]"
#define ANMMSG_COULDNTCREATEKEYBOARD	"Keyboard could not be created"
#define ANMMSG_COULDNTSETKBCOOPLEVEL	"Keyboard coop level could not be changed"
#define ANMMSG_COULDNTCREATEMOUSE		"[cMouse::Init]: Could not create the device"
#define ANMMSG_SETDATAFORMATMOUSEFAILED	"[cMouse::Init]: SetDataFormat failed"
#define ANMMSG_SETCOOPLEVELMOUSEFAILED	"[cMouse::Init]: SetCooperativeLevel failed"
#define ANMMSG_COULDNTCREATEDI8			"DirectInput8 object could not be created"
#define ANMMSG_UNKNOWNRGBBITCOUNTCBMTB	"Unknown dwRGBBitCount [CAnmDShowMovieTextureBlitter8::CopyBits]"


#endif //_anmdx8message_h