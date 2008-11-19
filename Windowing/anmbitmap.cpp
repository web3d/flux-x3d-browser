/********************************************************************************
 * Flux
 *
 * File: anmbitmap.cpp
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

#include "stdafx.h"

#include "anmbitmap.h"
#include "anmdevice.h"
#include "anmtexturedata.h"

// #include "anmd3ddevice7.h"

CAnmBitmap::CAnmBitmap(CAnmTextureData *pTextureData,
					   int x, int y, int width, int height)
{
	m_hDC = NULL;
	m_hBitmap = NULL;
	m_hasalpha = false;
	m_x = x;
	m_y = y;

	if (width == 0 || height == 0)
		InitBitmap(pTextureData, pTextureData->GetWidth(), pTextureData->GetHeight());
	else
		InitBitmap(pTextureData, width, height);

}

CAnmBitmap::~CAnmBitmap()
{
    if (m_hBitmap)
		DeleteObject(m_hBitmap);

	if (m_hDC)
		DeleteDC(m_hDC);
}

void CAnmBitmap::InitBitmap(CAnmTextureData *pTextureData, int width, int height)
{
	m_width = width;
	m_height = height;

	m_hasalpha = pTextureData->HasAlpha();

	BITMAPINFO bmi;        // bitmap header

    // create a DC for our bitmap -- the source DC for AlphaBlend 
    m_hDC = CreateCompatibleDC(NULL);
    
    // zero the memory for the bitmap info
    ZeroMemory(&bmi, sizeof(BITMAPINFO));

	// Always do 4-bytes, even if the bitmap doesn't have alpha. It's just easier
	int bytecount = 4;
	int bitcount = bytecount * 8;

    // setup bitmap info 
    // set the bitmap width and height to 60% of the width and height of each of the three horizontal areas. Later on, the blending will occur in the center of each of the three areas.
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = bitcount;         // four 8-bit components
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = width * height * bytecount;

    // create our DIB section and select the bitmap into the dc
    m_hBitmap = CreateDIBSection(m_hDC, &bmi, DIB_RGB_COLORS, &m_bits, NULL, 0x0);

	// Now get the bits into my bitmap
	CopyDataToBitmap(pTextureData);
}

#define iR	2	
#define iG	1	
#define iB	0	

void CAnmBitmap::CopyDataToBitmap(CAnmTextureData *pTextureData)
{
#if 0

	BitmapImage *pImage = pTextureData->GetBitmapImage();

	// Possibly scale the bitmap
	float XRatio, YRatio, X, Y;
	int Xc, Yc;
	int *XPoints = NULL;
	int *YPoints = NULL;

	BOOL scaling = FALSE;
	int datawidth = pTextureData->GetWidth();
	int dataheight = pTextureData->GetHeight();
	if (datawidth != m_width || dataheight != m_height)
	{
		scaling = TRUE;
		XRatio = (float) ( (float) datawidth /
						   (float) m_width );

		YRatio = (float) ( (float) dataheight /
						  (float) m_height  );

		//alloc the look up table
		XPoints = new int[m_width];

		for( Xc=0, X = 0.f; Xc < m_width ; X+=XRatio, Xc++ )
		   XPoints[Xc] = X;

		//same for the y axis
		YPoints = new int[ m_height];

		for( Yc=0, Y = 0.f ; Yc <  m_height ; Y+=YRatio, Yc++ )
		   YPoints[Yc] = Y;

	}

    for( DWORD y=0; y<m_height; y++ )
    {
        for( DWORD x=0; x<m_width; x++ )
        {
			if (m_hasalpha)
			{
				if (scaling)
					((UINT32 *)m_bits)[x + (m_height - y - 1) * m_width] = 
							(*pImage)[YPoints[y]][XPoints[x]*4] |
							((*pImage)[YPoints[y]][XPoints[x]*4+1] << 8) |
							((*pImage)[YPoints[y]][XPoints[x]*4+2] << 16) | 
							((*pImage)[YPoints[y]][XPoints[x]*4+3] << 24);
				else
					((UINT32 *)m_bits)[x + (m_height - y - 1)  * m_width] = 
							(*pImage)[y][x*4] |
							((*pImage)[y][x*4+1] << 8) |
							((*pImage)[y][x*4+2] << 16) | 
							((*pImage)[y][x*4+3] << 24);
			}
			else
			{
				if (scaling)
					((UINT32 *)m_bits)[x + (m_height - y - 1)  * m_width] = 
							(*pImage)[YPoints[y]][XPoints[x]*3] |
							((*pImage)[YPoints[y]][XPoints[x]*3+1] << 8) |
							((*pImage)[YPoints[y]][XPoints[x]*3+2] << 16) | 
							0xFF000000;
				else
					((UINT32 *)m_bits)[x + (m_height - y - 1)  * m_width] = 
							(*pImage)[y][x*3] |
							((*pImage)[y][x*3+1] << 8) |
							((*pImage)[y][x*3+2] << 16) | 
							0xFF000000;
			}
        }
    }
#else

	unsigned char *pPixelData = pTextureData->GetPixelData();

	bool grayscale = pTextureData->IsGrayScale();
	int nBytesPerPixel = ( grayscale ) ? 1 : 3;
	if( pTextureData->HasAlpha() ) {
		nBytesPerPixel++;
	}

	// Possibly scale the bitmap
	float XRatio, YRatio;
	int datawidth = pTextureData->GetWidth();
	int dataheight = pTextureData->GetHeight();

	BOOL scalingX = (m_width != datawidth );
	BOOL scalingY = (m_height !=  dataheight);

	XRatio = (float) ( (float) datawidth /
					   (float) m_width );

	YRatio = (float) ( (float) dataheight /
					  (float) m_height  );

	int iA = ( grayscale ) ? 1 : 3;
	BYTE  alpahByte = 0xFF;
	BYTE* pPixSrc;
	int iYOffset;
	int iXOffset;
	int iOffset;

    for( DWORD y=0; y<m_height; y++ )
    {
		if( scalingY ) {
			iYOffset = ( nBytesPerPixel * datawidth ) * ( ( ( int ) ( ( ( float ) ( m_height - 1 - y ) ) * YRatio ) ) );
		}
		else {
			iYOffset = ( m_height - 1 - y ) * nBytesPerPixel * datawidth;
		}
		iOffset = iYOffset;

        for( DWORD x=0; x<m_width; x++ )
        {
			if (scalingX) {
				iOffset = iYOffset + nBytesPerPixel * ( ( int ) ( ( ( float ) x )*XRatio ) );
			}

			pPixSrc = &pPixelData[iOffset];

			if (m_hasalpha)
			{
				alpahByte = pPixSrc[iA];
			}

			if (grayscale)
			{
				((UINT32 *)m_bits)[x + (m_height - y - 1)  * m_width] = 
					pPixSrc[0] | pPixSrc[0] << 8 | pPixSrc[0] << 16 | alpahByte;
			}
			else
			{
				((UINT32 *)m_bits)[x + (m_height - y - 1)  * m_width] = 
					pPixSrc[iR] | pPixSrc[iG] << 8 | pPixSrc[iB] << 16 | alpahByte;
			}

			if (!scalingX) {
				iOffset+=nBytesPerPixel;
			}
        }
    }

#endif
}

void CAnmBitmap::MoveTo(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CAnmBitmap::Render(CAnmDevice *pDevice)
{
#if 0

	// N.B.: Commented out until we un-funk 2D drawing; also we did the rendering rework-- TP 8/2/04
	assert(m_hBitmap && m_hDC);

	SelectObject(m_hDC, m_hBitmap);

	HDC hDC;
	HRESULT hr = ((CAnmD3DDevice7 *)pDevice)->GetBackBuffer()->GetD3DSurface()->GetDC(&hDC);

	if (SUCCEEDED(hr))
	{
		if (m_hasalpha)
		{
			BLENDFUNCTION bf;      // structure for alpha blending
			bf.BlendOp = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.AlphaFormat = AC_SRC_NO_PREMULT_ALPHA; // AC_SRC_ALPHA;   // use source alpha 
			bf.SourceConstantAlpha = 0xff;   // use constant alpha, with 
											 // 75% opaqueness

			// TextOut(hDC, x, y, "Hello world", 11); 
			AlphaBlend(hDC, m_x, m_y, m_width, m_height, m_hDC, 0, 0, m_width, m_height, bf);
		}
		else
		{
			BitBlt(hDC, m_x, m_y, m_width, m_height, m_hDC, 0, 0, SRCCOPY);
		}

		((CAnmD3DDevice7 *)pDevice)->GetBackBuffer()->GetD3DSurface()->ReleaseDC(hDC);
	}
#endif

}

#if 0
void DrawAlphaBlend (HWND hWnd, HDC hdcwnd)
{
    HDC hdc;               // handle of the DC we will create 
    BLENDFUNCTION bf;      // structure for alpha blending
    HBITMAP hbitmap;       // bitmap handle
    BITMAPINFO bmi;        // bitmap header
    VOID *m_bits;          // pointer to DIB section
    ULONG   ulWindowWidth, ulWindowHeight;      // window width/height
    ULONG   ulBitmapWidth, ulBitmapHeight;      // bitmap width/height
    RECT    rt;            // used for getting window dimensions
    UINT32   x,y;          // stepping variables
    UCHAR ubAlpha;         // used for doing transparent gradient
    UCHAR ubRed;        
    UCHAR ubGreen;
    UCHAR ubBlue;
    float fAlphaFactor;    // used to do premultiply
            
    // get window dimensions
    GetClientRect(hWnd, &rt);
    
    // calculate window width/height
    ulWindowWidth = rt.right - rt.left;  
    ulWindowHeight = rt.bottom - rt.top;  

    // make sure we have at least some window size
    if ((!ulWindowWidth) || (!ulWindowHeight))
        return;

    // divide the window into 3 horizontal areas
    ulWindowHeight = ulWindowHeight / 3;

    // create a DC for our bitmap -- the source DC for AlphaBlend 
    hdc = CreateCompatibleDC(hdcwnd);
    
    // zero the memory for the bitmap info
    ZeroMemory(&bmi, sizeof(BITMAPINFO));

    // setup bitmap info 
    // set the bitmap width and height to 60% of the width and height of each of the three horizontal areas. Later on, the blending will occur in the center of each of the three areas.
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = ulBitmapWidth = ulWindowWidth - (ulWindowWidth/5)*2;
    bmi.bmiHeader.biHeight = ulBitmapHeight = ulWindowHeight - (ulWindowHeight/5)*2;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = ulBitmapWidth * ulBitmapHeight * 4;

    // create our DIB section and select the bitmap into the dc
    hbitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &m_bits, NULL, 0x0);
    SelectObject(hdc, hbitmap);

    // in top window area, constant alpha = 50%, but no source alpha
    // the color format for each pixel is 0xaarrggbb 
    // set all pixels to blue and set source alpha to zero
    for (y = 0; y < ulBitmapHeight; y++)
        for (x = 0; x < ulBitmapWidth; x++)
            ((UINT32 *)m_bits)[x + y * ulBitmapWidth] = 0x000000ff; 

    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 0x7f;  // half of 0xff = 50% transparency
    bf.AlphaFormat = 0;             // ignore source alpha channel

    if (!AlphaBlend(hdcwnd, ulWindowWidth/5, ulWindowHeight/5, 
                    ulBitmapWidth, ulBitmapHeight, 
                    hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf))
        return;                     // alpha blend failed
    
    // in middle window area, constant alpha = 100% (disabled), source 
    // alpha is 0 in middle of bitmap and opaque in rest of bitmap 
    for (y = 0; y < ulBitmapHeight; y++)
        for (x = 0; x < ulBitmapWidth; x++)
            if ((x > (int)(ulBitmapWidth/5)) && (x < (ulBitmapWidth-ulBitmapWidth/5)) &&
                (y > (int)(ulBitmapHeight/5)) && (y < (ulBitmapHeight-ulBitmapHeight/5)))
                //in middle of bitmap: source alpha = 0 (transparent).
                // This means multiply each color component by 0x00.
                // Thus, after AlphaBlend, we have a, 0x00 * r, 
                // 0x00 * g,and 0x00 * b (which is 0x00000000)
                // for now, set all pixels to red
                ((UINT32 *)m_bits)[x + y * ulBitmapWidth] = 0x00ff0000; 
            else
                // in the rest of bitmap, source alpha = 0xff (opaque) 
                // and set all pixels to blue 
                ((UINT32 *)m_bits)[x + y * ulBitmapWidth] = 0xff0000ff; 
            endif;
    
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha 
    bf.SourceConstantAlpha = 0xff;  // opaque (disable constant alpha)
   
    if (!AlphaBlend(hdcwnd, ulWindowWidth/5, ulWindowHeight/5+ulWindowHeight, ulBitmapWidth, ulBitmapHeight, hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf))
        return;

    // bottom window area, use constant alpha = 75% and a changing
    // source alpha. Create a gradient effect using source alpha, and 
    // then fade it even more with constant alpha
    ubRed = 0x00;
    ubGreen = 0x00;
    ubBlue = 0xff;
    
    for (y = 0; y < ulBitmapHeight; y++)
        for (x = 0; x < ulBitmapWidth; x++)
        {
            // for a simple gradient, base the alpha value on the x 
            // value of the pixel 
            ubAlpha = (UCHAR)((float)x / (float)ulBitmapWidth * 255);
            //calculate the factor by which we multiply each component
            fAlphaFactor = (float)ubAlpha / (float)0xff; 
            // multiply each pixel by fAlphaFactor, so each component 
            // is less than or equal to the alpha value.
            ((UINT32 *)m_bits)[x + y * ulBitmapWidth] 
                = (ubAlpha << 24) |                       //0xaa000000
                 ((UCHAR)(ubRed * fAlphaFactor) << 16) |  //0x00rr0000
                 ((UCHAR)(ubGreen * fAlphaFactor) << 8) | //0x0000gg00
                 ((UCHAR)(ubBlue   * fAlphaFactor));      //0x000000bb
        }

    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = AC_SRC_ALPHA;   // use source alpha 
    bf.SourceConstantAlpha = 0xbf;   // use constant alpha, with 
                                     // 75% opaqueness

    AlphaBlend(hdcwnd, ulWindowWidth/5, 
               ulWindowHeight/5+2*ulWindowHeight, ulBitmapWidth, 
               ulBitmapHeight, hdc, 0, 0, ulBitmapWidth, 
               ulBitmapHeight, bf);

    // do cleanup
    DeleteObject(hbitmap);
    DeleteDC(hdc);
    
}

#endif // 0

