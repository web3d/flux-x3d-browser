/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			anmmiscdraw.cpp
 *		Description:	Anima misc Windows drawing stuff (not in build)
 *
 * (C) 2001-2003 by Media Machines
 *
 *		Revision history:
 *			$Log: anmmiscdraw.cpp,v $
 *			Revision 1.1.1.1  2003/09/10 02:18:11  turk
 *			build466
 *			
 *
 *******************************************************************/

#if 0
//**----------------------------------------------------------
//** STEP 1: Load the bitmap into a CBitmap Object
//**----------------------------------------------------------
BOOL CMyDlg::LoadFileBitmap(CBitmap* pBmp, LPCTSTR szFilename)
{
   pBmp->DeleteObject();
   return pBmp->Attach(LoadImage(NULL, szFilename, IMAGE_BITMAP, 0, 0,
                      LR_LOADFROMFILE | LR_DEFAULTSIZE));
}


//**----------------------------------------------------------
//** STEP 2: Create the Mask and dump it into a CBitmap Object
//**----------------------------------------------------------
void CMyDlg::PrepareMask( CBitmap* pBmpSource,
                          CBitmap* pBmpMask,
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
void CMyDlg::DrawTransparentBitmap(CMemDC* pDC,
                                   int xStart,  int yStart,
                                   int wWidth,  int wHeight,
                                   CDC* pTmpDC,
                                   int xSource, // = 0
                                   int ySource  // = 0)
{

   // We are going to paint the two DDB's in sequence to the destination.
   // 1st the monochrome bitmap will be blitted using an AND operation to
   // cut a hole in the destination. The color image will then be ORed
   // with the destination, filling it into the hole, but leaving the
   // surrounding area untouched.

   CDC hdcMem;
   hdcMem.CreateCompatibleDC(NULL);

   CBitmap* hbmT = hdcMem.SelectObject(&m_bmpMask);

   pDC->BitBlt( xStart, yStart, wWidth, wHeight, &hdcMem,
                xSource, ySource, SRCAND);

   // Also note the use of SRCPAINT rather than SRCCOPY.

   pDC->BitBlt(xStart, yStart, wWidth, wHeight, pTmpDC,
               xSource, ySource,SRCPAINT);

   // Now, clean up.
   hdcMem.SelectObject(hbmT);
   hdcMem.DeleteDC();
}

It is that simple. MSDN examples are very confusing. Chris Becke's examples are a lot better but in Win32. :) 

So here is a example on how to put it together in a real life situation. 

//**------------------------------------
//** EXAMPLE: Drawing with Transparency
//**------------------------------------

//**------------------------------------------
//** EXP STEP 1: Declarations & variables
//**------------------------------------------
In YourDlg.h, add the following

CBitmap m_bmpPlmain;
CBitmap m_bmpMask;

BOOL LoadFileBitmap(CBitmap* pBmp, LPCTSTR szFilename);

void PrepareMask( CBitmap* pBmpSource,
                  CBitmap* pBmpMask,
                  COLORREF clrpTransColor,
                  int iTransPixelX = 0,
                  int iTransPixelY = 0 );

void DrawTransparentBitmap (CMemDC* pDC,
                            int xStart, int yStart,
                            int wWidth, int wHeight,
                            CDC* pTmpDC,
                            int xSource = 0,
                            int ySource = 0);


//**------------------------------------------
//** EXP STEP 2: Load and Prepare the bitmaps
//**------------------------------------------
CYourDlg::OnInitDialog()
{
  ...
  ...
  ...

  // I'm loading from a bitmap file but this can come from resource as well
  if (!LoadFileBitmap(&m_bmpPLMain, "BmpWithHoles.bmp"))
  {
    // Opps ... where did the picture go ??
  }

  // Third param is NULL because I don't know the transparent color
  // but I know the trans color is at pixel 200, 50
  // or is you know the trans color(RED) then do the following
  //  PrepareMask( &m_bmpPLMain, &m_bmpMask, RGB(255, 0, 0));

  PrepareMask( &m_bmpPLMain, &m_bmpMask, NULL, 200, 50);

}

//**---------------------------------
//** EXP STEP 3: Drawing the bitmaps
//**---------------------------------
CYourDlg::OnPaint()
{

  CPaintDC dc(this);              // device context for painting
  CDC dcMem;                  // memory device context
  dcMem.CreateCompatibleDC(&dc);

  // Select the bmp into the tmp memory DC
  CBitmap* pOldBmp = (CBitmap*) dcMem.SelectObject(&m_bmpPLMain);

  DrawTransparentBitmap( &dc,           // The destination DC.
                         POP_X,         // Where to draw
                         POP_Y,
                         POP_WIDTH,     // Width & Height
                         POP_HEIGHT,
                         &dcMem,        // the DC holding the bmp
                         POP_BMP_X_OFF, // x & y pos in master bmp
                         POP_BMP_Y_OFF);
  ....
  ....
  // Do whatever you want to do ...

  dcMem->SelectObject(pOldBmp);
}

//**-----------------------------
//** EXP STEP 4: Cleaning Up ..
//**-----------------------------
CYourDlg::OnDestroy()
{
   // Dont forget to delete the bmp's
   m_bmpPLMain.DeleteObject();
   m_bmpMask.DeleteObject();
}

#endif

// More cool stuff: gradient fills
#if 0
	TRIVERTEX        vert[2] ;
	GRADIENT_RECT    gRect;
	vert [0] .x      = 0;
	vert [0] .y      = 0;
	vert [0] .Red    = 0x1100;
	vert [0] .Green  = 0x2200;
	vert [0] .Blue   = 0x3300;
	vert [0] .Alpha  = 0x0000;

	vert [1] .x      = r.right;
	vert [1] .y      = r.bottom; 
	vert [1] .Red    = 0x3300;
	vert [1] .Green  = 0x6600;
	vert [1] .Blue   = 0x9900;
	vert [1] .Alpha  = 0x0000;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;
	::GradientFill(dc.m_hDC, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
#endif
