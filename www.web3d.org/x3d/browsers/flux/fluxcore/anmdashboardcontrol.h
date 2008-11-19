/********************************************************************************
 * Flux
 *
 * File: anmdashboardcontrol.h
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

#ifndef _anmdashboardcontrol_h
#define _anmdashboardcontrol_h

typedef enum eAnmDashboardControlState
{
	eDCSNormal,							// normal state
	eDCSDimmed,							// greyed state
} eAnmDashboardControlState;

typedef enum eAnmDashboardControlJustify
{
	eDCJLeft,
	eDCJCenter,
	eDCJRight,
} eAnmDashboardControlJustify ;

#define ANMDASHBOARDCONTROL_NMAPS 4
#define ANMDASHBOARDCONTROL_TEXTHMARGIN 10
#define ANMDASHBOARDCONTROL_TEXTVMARGIN 6

class EXPORT CAnmDashboardControl
{
protected :

	class CDashboard1				*m_dashboard;
	CBitmap							 m_bitmap;
	CBitmap							 m_bmpMask;
	CRgn							 m_boundingRegion;
	RECT							 m_boundingRect;
	int								 m_bitmapwidth;
	int								 m_bitmapheight;
	eAnmDashboardControlState		 m_state;
	int								 m_commandID;
	CString							 m_label;
	bool							 m_visible;
	bool							 m_pressed;
	bool							 m_highlighted;
	char							*m_tooltip;
	eAnmDashboardControlJustify		 m_justify;

	void PrepareMask( CBitmap* pBmpSource,
                          CBitmap* pBmpMask,
						  CRgn *pRgnOut,
                          COLORREF clrpTransColor, // Pass null if unknown
                          int iTransPixelX = 0,
                          int iTransPixelY = 0
                        );

	void DrawTransparentBitmap(CDC* pDC,
                                   int xStart,  int yStart,
                                   int wWidth,  int wHeight,
                                   CDC* pTmpDC,
                                   int xSource,
                                   int ySource,
								   int wSource,
								   int hSource);
public :

	CAnmDashboardControl(class CDashboard1 *pDashboard, UINT nRsrc, int commandID,
		int x, int y, bool visible = false, eAnmDashboardControlState state = eDCSNormal, char *tooltip = NULL);

	~CAnmDashboardControl();

	void Paint(CDC *pDC, CDC *pMemDC);

	bool Poll(POINT pt);

	void MoveTo(int x, int y);
	void SetSize(int width, int height);

	LPCRECT GetBoundingRect()
	{
		return &m_boundingRect;
	}

	void GetBitmapDimensions(int *pwidth, int *pheight)
	{
		*pwidth = m_bitmapwidth;
		*pheight = m_bitmapheight;
	}

	CRgn *GetBoundingRegion()
	{
		return &m_boundingRegion;
	}

	eAnmDashboardControlState GetState()
	{
		return m_state;
	}

	void SetState(eAnmDashboardControlState ds);

	int GetCommandID()
	{
		return m_commandID;
	}

	void SetLabel(CString label);

	void SetPressed(bool pressed);
	bool GetPressed()
	{
		return m_pressed;
	}
	void SetHighlighted(bool highlighted);
	bool GetHighlighted()
	{
		return m_highlighted;
	}

	void Show();
	void Hide();
	bool IsVisible()
	{
		return m_visible;
	}

	char *GetTooltip()
	{
		return m_tooltip;
	}

	void SetJustify(eAnmDashboardControlJustify justify);
	eAnmDashboardControlJustify GetJustify()
	{
		return m_justify;
	}
};

#endif // _anmdashboardcontrol_h
