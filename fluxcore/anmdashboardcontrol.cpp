/********************************************************************************
 * Flux
 *
 * File: anmdashboardcontrol.cpp
 * Description: Dashboard Control class
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


#include <stdafx.h>
#include "anmdashboardcontrol.h"
#include "anmdashboard.h"

CAnmDashboardControl::CAnmDashboardControl(CDashboard1 *pDashboard, UINT nRsrc, int commandID,
										   int x, int y, bool visible,
										   eAnmDashboardControlState state, 
										   char *tooltip)
{
	m_dashboard = pDashboard;
	m_commandID = commandID;
	m_boundingRect.left = x;
	m_boundingRect.top = y;
	m_state = state;
	m_visible = true;
	m_pressed = false;
	m_highlighted = false;
	m_tooltip = tooltip;
	m_justify = eDCJLeft;

	BOOL loaded = m_bitmap.LoadBitmap(nRsrc);
	if (loaded)
	{
		BITMAP bmp;
		m_bitmap.GetObject(sizeof(BITMAP), &bmp);

		// N.B.: each button has 3 bitmaps of equal size for each state
		m_bitmapwidth = bmp.bmWidth / ANMDASHBOARDCONTROL_NMAPS ;
		m_bitmapheight = bmp.bmHeight;

		m_boundingRect.right = x + m_bitmapwidth;
		m_boundingRect.bottom = y + m_bitmapheight;

#if 0
		// create 0-based region (to get mask bits right)
		m_boundingRegion.CreateRectRgn(0, 0, m_bitmapwidth, m_bitmapheight);

		// create bit mask and fill region
		PrepareMask( &m_bitmap, &m_bmpMask, &m_boundingRegion, NULL);

		// move the region to our origin
		m_boundingRegion.OffsetRgn(x, y);
#endif
	}
	else
	{
		m_boundingRect.right = x;
		m_boundingRect.top = y;
	}
}

CAnmDashboardControl::~CAnmDashboardControl()
{
}

void CAnmDashboardControl::Paint(CDC *pDC, CDC *pMemDC)
{
	if (!m_visible)
		return;

	if (m_bitmap.GetSafeHandle() == NULL)
		return;

	// calc offset into source bitmap
	int offset = 0;
										// dimmed state in 1st bitmap
	if (m_state == eDCSNormal)			// normal state in 2rd bitmap
		offset = m_bitmapwidth;

	if (m_highlighted)
		offset = m_bitmapwidth * 2;		// highlighted state in 3rd bitmap
	else if (m_pressed)						// pressed state in 4th bitmap
		offset = m_bitmapwidth * 3;

	// get our rect's width and height
	int rectwidth = m_boundingRect.right - m_boundingRect.left;
	int rectheight = m_boundingRect.bottom - m_boundingRect.top;

	CBitmap* pOldBmp = (CBitmap*) pMemDC->SelectObject(&m_bitmap);
	pDC->StretchBlt(m_boundingRect.left, m_boundingRect.top,
		rectwidth, rectheight, pMemDC, offset, 0, m_bitmapwidth, m_bitmapheight, SRCCOPY);
	pMemDC->SelectObject(pOldBmp);

#if 0
	// paint the button
	CBitmap* pOldBmp = (CBitmap*) pMemDC->SelectObject(&m_bitmap);

	DrawTransparentBitmap( pDC,           // destination DC.
						 m_boundingRect.left,         // destination dimensions
						 m_boundingRect.top,
						 rectwidth,			
						 rectheight,
						 pMemDC,		// source DC
						 offset,	// source dimensions
						 0,
						 m_bitmapwidth,
						 m_bitmapheight);

	// restore old DC state
	pMemDC->SelectObject(pOldBmp);
#endif

	if (!m_label.IsEmpty())
	{
		int oldmode = pDC->SetBkMode(TRANSPARENT);
		COLORREF oldcolor = pDC->SetTextColor(RGB(0, 0, 0));

		//pDC->TextOut(m_boundingRect.left + ANMDASHBOARDCONTROL_TEXTHMARGIN,
		//	m_boundingRect.top + ANMDASHBOARDCONTROL_TEXTVMARGIN, m_label);

		UINT flags = 0;
		if (m_justify == eDCJCenter)
			flags |= DT_CENTER;

		RECT r = m_boundingRect;
		InflateRect(&r, -ANMDASHBOARDCONTROL_TEXTHMARGIN, -ANMDASHBOARDCONTROL_TEXTVMARGIN);

		pDC->DrawText(m_label, &r, flags);

		pDC->SetBkMode(oldmode);
		pDC->SetTextColor(oldcolor);
	}
}

bool CAnmDashboardControl::Poll(POINT pt)
{
	if (!m_visible)
		return false;

	// Try trivial reject on bounding rect first
	bool over = PtInRect(&m_boundingRect, pt);

	// Let's just use rect now
	return over;

	if (over)
		over = PtInRegion(m_boundingRegion, pt.x, pt.y);

	return over;
}

void CAnmDashboardControl::SetState(eAnmDashboardControlState ds)
{
	if (ds != m_state)
	{
		m_state = ds;

		if (m_visible && m_dashboard->GetSafeHwnd())
			m_dashboard->InvalidateRect(&m_boundingRect, FALSE);
	}
}

void CAnmDashboardControl::MoveTo(int x, int y)
{

	// invalidate our old rect to get repaint
	m_dashboard->InvalidateRect(&m_boundingRect, FALSE);

	int dx = x - m_boundingRect.left;
	int dy = y - m_boundingRect.top;

	OffsetRect(&m_boundingRect, dx, dy);
	if (m_boundingRegion.GetSafeHandle())
		m_boundingRegion.OffsetRgn(dx, dy);

	// now invalidate our new rect to get repaint too
	if (m_visible && m_dashboard->GetSafeHwnd())
		m_dashboard->InvalidateRect(&m_boundingRect, FALSE);
}

void CAnmDashboardControl::SetSize(int width, int height)
{
	// invalidate our old rect to get repaint
	m_dashboard->InvalidateRect(&m_boundingRect, FALSE);

	m_boundingRect.right = m_boundingRect.left + width;
	m_boundingRect.bottom = m_boundingRect.top + height;

/*
	if (m_boundingRegion.GetSafeHandle())
		m_boundingRegion.OffsetRgn(dx, dy);
*/

	// now invalidate our new rect to get repaint too
	if (m_visible && m_dashboard->GetSafeHwnd())
		m_dashboard->InvalidateRect(&m_boundingRect, FALSE);
}

void CAnmDashboardControl::SetLabel(CString label)
{
	m_label = label;

	if (m_visible && m_dashboard->GetSafeHwnd())
		m_dashboard->InvalidateRect(&m_boundingRect, FALSE);
}

void CAnmDashboardControl::SetPressed(bool pressed)
{
	if (pressed != m_pressed)
	{
		m_pressed = pressed;

		RECT r = m_boundingRect;

		if (m_visible && m_dashboard->GetSafeHwnd())
			m_dashboard->InvalidateRect(&r, FALSE);
	}
}

void CAnmDashboardControl::SetHighlighted(bool highlighted)
{
	if (highlighted != m_highlighted)
	{
		m_highlighted = highlighted;

		RECT r = m_boundingRect;

		if (m_visible && m_dashboard->GetSafeHwnd())
			m_dashboard->InvalidateRect(&r, FALSE);
	}
}

void CAnmDashboardControl::Show()
{
	if (m_visible)
		return;

	m_visible = true;
	if (m_dashboard->GetSafeHwnd())
		m_dashboard->InvalidateRect(&m_boundingRect, FALSE);
}

void CAnmDashboardControl::Hide()
{
	if (!m_visible)
		return;

	m_visible = false;
	if (m_dashboard->GetSafeHwnd())
		m_dashboard->InvalidateRect(&m_boundingRect, FALSE);
}

void CAnmDashboardControl::SetJustify(eAnmDashboardControlJustify justify)
{
	if (m_justify != justify)
	{
		m_justify = justify;

		if (m_visible && m_dashboard->GetSafeHwnd())
			m_dashboard->InvalidateRect(&m_boundingRect, FALSE);
	}
}

//**----------------------------------------------------------
//** STEP 2: Create the Mask and dump it into a CBitmap Object
//**----------------------------------------------------------
void CAnmDashboardControl::PrepareMask( CBitmap* pBmpSource,
                          CBitmap* pBmpMask,
						  CRgn *pRgnOut,
                          COLORREF clrpTransColor, // Pass null if unknown
                          int iTransPixelX,      // = 0
                          int iTransPixelY       // = 0
                        )
{
   BITMAP bm;

   // Get the dimensions of the source bitmap
   pBmpSource->GetObject(sizeof(BITMAP), &bm);

   // Create the mask bitmap
   pBmpMask->DeleteObject();
   pBmpMask->CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL);

   // We will need two DCs to work with. One to hold the Image
   // (the source), and one to hold the mask (destination).
   // When blitting onto a monochrome bitmap from a color, pixels
   // in the source color bitmap that are equal to the background
   // color are blitted as white. All the remaining pixels are
   // blitted as black.

   CDC hdcSrc, hdcDst;

   hdcSrc.CreateCompatibleDC(NULL);
   hdcDst.CreateCompatibleDC(NULL);

   // Load the bitmaps into memory DC
   CBitmap* hbmSrcT = (CBitmap*) hdcSrc.SelectObject(pBmpSource);
   CBitmap* hbmDstT = (CBitmap*) hdcDst.SelectObject(pBmpMask);

   // Dynamically get the transparent color
   COLORREF clrTrans;
   if (clrpTransColor == NULL)
   {
      // User did not specify trans color so get it from bmp
      clrTrans = hdcSrc.GetPixel(iTransPixelX, iTransPixelY);
   }
   else
   {
      clrTrans = clrpTransColor;
   }


   // Change the background to trans color
   COLORREF clrSaveBk  = hdcSrc.SetBkColor(clrTrans);

   // This call sets up the mask bitmap.
   hdcDst.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcSrc,0,0,SRCCOPY);

   // Now use the mask bitmap to create a region
   COLORREF dstColor;
	for (int x = 0; x < bm.bmWidth; x++)
	{
		for (int y = 0; y < bm.bmHeight; y++)
		{
			dstColor = hdcDst.GetPixel(x, y);
			if (dstColor == RGB(255,255,255))
			{
				CRgn tmpRgn;
				tmpRgn.CreateRectRgn(x, y, x+1, y+2);
				if (pRgnOut->CombineRgn(pRgnOut, &tmpRgn, RGN_DIFF) == ERROR)
				{
					int foo = 1;
				}
			}
		}
	}

   // Now, we need to paint onto the original image, making
   // sure that the "transparent" area is set to black. What
   // we do is AND the monochrome image onto the color Image
   // first. When blitting from mono to color, the monochrome
   // pixel is first transformed as follows:
   // if  1 (black) it is mapped to the color set by SetTextColor().
   // if  0 (white) is is mapped to the color set by SetBkColor().
   // Only then is the raster operation performed.

   COLORREF clrSaveDstText = hdcSrc.SetTextColor(RGB(255,255,255));
   hdcSrc.SetBkColor(RGB(0,0,0));

   hdcSrc.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcDst,0,0,SRCAND);

   // Clean up by deselecting any objects, and delete the
   // DC's.
   hdcDst.SetTextColor(clrSaveDstText);

   hdcSrc.SetBkColor(clrSaveBk);
   hdcSrc.SelectObject(hbmSrcT);
   hdcDst.SelectObject(hbmDstT);

   hdcSrc.DeleteDC();
   hdcDst.DeleteDC();
}


//**----------------------------------------------------------
//** STEP 3: Drawing with Transparency. Call from OnPaint
//**----------------------------------------------------------
void CAnmDashboardControl::DrawTransparentBitmap(CDC* pDC,
                                   int xStart,  int yStart,
                                   int wWidth,  int wHeight,
                                   CDC* pTmpDC,
                                   int xSource, // = 0
                                   int ySource,
								   int wSource,
								   int hSource)  // = 0)
{

   // We are going to paint the two DDB's in sequence to the destination.
   // 1st the monochrome bitmap will be blitted using an AND operation to
   // cut a hole in the destination. The color image will then be ORed
   // with the destination, filling it into the hole, but leaving the
   // surrounding area untouched.

   CDC hdcMem;
   hdcMem.CreateCompatibleDC(NULL);

   CBitmap* hbmT = hdcMem.SelectObject(&m_bmpMask);

   if (wWidth != wSource || wHeight != hSource)
   {
		pDC->StretchBlt(xStart, yStart, wWidth, wHeight, &hdcMem, xSource, ySource,
		   wSource, hSource, SRCAND);

		// Also note the use of SRCPAINT rather than SRCCOPY.

		pDC->StretchBlt(xStart, yStart, wWidth, wHeight, pTmpDC, xSource, ySource,
		   wSource, hSource, SRCPAINT);
   }
   else
   {
		pDC->BitBlt( xStart, yStart, wWidth, wHeight, &hdcMem,
					xSource, ySource, SRCAND);

		// Also note the use of SRCPAINT rather than SRCCOPY.

		pDC->BitBlt(xStart, yStart, wWidth, wHeight, pTmpDC,
				   xSource, ySource,SRCPAINT);
   }

   // Now, clean up.
   hdcMem.SelectObject(hbmT);
   hdcMem.DeleteDC();
}
