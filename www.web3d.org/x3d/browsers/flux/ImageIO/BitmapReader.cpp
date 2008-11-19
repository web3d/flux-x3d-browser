#include "stdafx.h"

#include "ImageIO.h"

unsigned char* read_bmp(const char *filename, int* pwidth, int* pheight )
{

	BOOL bpreview = TRUE;
/* Retrieve a handle identifying the file. */

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	HANDLE hfbm = CreateFile(filename, GENERIC_READ,
		  FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES) NULL,
		  OPEN_EXISTING, FILE_ATTRIBUTE_READONLY,
		  (HANDLE) NULL);

	if( !hfbm ) {
		return NULL;
	}


	DWORD dwRead;
	ReadFile(hfbm, &bmfh, sizeof(BITMAPFILEHEADER),
	  &dwRead, (LPOVERLAPPED)NULL);

	/* Retrieve the BITMAPFILEHEADER structure. */

	ReadFile(hfbm, &bmih, sizeof(BITMAPINFOHEADER),
	  &dwRead, (LPOVERLAPPED)NULL);

 /* Allocate memory for the BITMAPINFO structure. */


	if( bmih.biWidth < 1 ||
		 bmih.biWidth > 1000 ||
		 bmih.biHeight < 1 ||
		 bmih.biHeight > 1000 ) {
		return NULL;
	}


	int file_width = bmih.biWidth;
	int file_height = bmih.biHeight;

	int isize, ogl_width = 0, ogl_height = 0;
	int ibit;



	if( bpreview ) {
		 ogl_width = file_width;
		 ogl_height = file_height;
	}
	else {
		for( ibit=1; ibit<12; ibit++ ) {
			isize = 1<<ibit;
			if( file_width <= isize ) {
				 ogl_width = isize;
				 break;
			}
		}
		for( ibit=1; ibit<12; ibit++ ) {
			isize = 1<<ibit;
			if( file_height <= isize ) {
				 ogl_height = isize;
				 break;
			}
		}
	}

	if( !ogl_height || !ogl_width ) {
		CloseHandle(hfbm);
		return NULL;
	}

	*pwidth = ogl_width;
	*pheight = ogl_height;





	RGBQUAD *bmiColors = NULL;
	if( bmih.biBitCount < 9 ) {
		int size = bmfh.bfOffBits - sizeof(BITMAPINFOHEADER) -
											 sizeof(BITMAPFILEHEADER);
		bmiColors = new RGBQUAD[1<<bmih.biBitCount];
		ReadFile(hfbm, bmiColors, size,
//        ((1<<bmih.biBitCount) * sizeof(RGBQUAD)), bmfh.bfOffBits
			&dwRead, (LPOVERLAPPED) NULL);

	}

 /*
  * Load BITMAPINFOHEADER into the BITMAPINFO
  * structure.  RGBQUAD
	 BYTE    rgbBlue;
	 BYTE    rgbGreen;
	 BYTE    rgbRed;

  */


/*****
 lpbmi->bmiHeader.biPlanes = bmih.biPlanes;
 lpbmi->bmiHeader.biBitCount = bmih.biBitCount;
 lpbmi->bmiHeader.biCompression = bmih.biCompression;
 lpbmi->bmiHeader.biSizeImage = bmih.biSizeImage;
 lpbmi->bmiHeader.biXPelsPerMeter = bmih.biXPelsPerMeter;
 lpbmi->bmiHeader.biYPelsPerMeter = bmih.biYPelsPerMeter;
 lpbmi->bmiHeader.biClrUsed = bmih.biClrUsed;
 lpbmi->bmiHeader.biClrImportant = bmih.biClrImportant;
 ******/
 /*
  * Retrieve the color table.
  * 1 << bmih.biBitCount == 2 ^ bmih.biBitCount
  */
 /*
  * Allocate memory for the required number of
  * bytes.
  */

	unsigned char *pdata = NULL;


	unsigned char* lpvBits =
		new unsigned char[bmfh.bfSize - bmfh.bfOffBits];


 /* Retrieve the bitmap data. */


	if( bmfh.bfSize - bmfh.bfOffBits < 2 ||
		 bmfh.bfSize - bmfh.bfOffBits > 1000000 ) {
		return NULL;
	}


	ReadFile(hfbm, lpvBits,
	  (bmfh.bfSize - bmfh.bfOffBits),
	  &dwRead, (LPOVERLAPPED) NULL);




	int nnpixx = ogl_width*ogl_height;
	int other_datasize = nnpixx * 3;
	pdata = new unsigned char[ other_datasize ];
	unsigned char *pd = pdata;
	unsigned char *pv = lpvBits;
	unsigned char index, * pd_last;
	int ix, iy;
	int nx = file_width;
	int ny = file_height;
	int extra_width = ogl_width - file_width;
	int extra_height = ogl_height - file_height;
	double ratiox = (double)extra_width / (double)nx;
	double ratioy = (double)extra_height / (double)ny;
	int iextrax, iextray;
	double rextrax, rextray;
	int extra_data;
	int extra_data_per_line;


	if( bmih.biBitCount == 24 ) {
		extra_data = ( bmfh.bfSize - bmfh.bfOffBits ) - file_width*file_height*3;
		extra_data_per_line = extra_data / file_height;
		unsigned char rc, gc, bc;

		iextray = 0;
		rextray = 0.0;
		for( iy=0; iy<ny; iy++ ) {
			iextrax = 0;
			rextrax = 0.0;
			for( ix=0; ix<nx; ix++ ) {

				rc = *(pv++);
				gc = *(pv++);
				bc = *(pv++);
				*(pd++) = bc;
				*(pd++) = gc;
				*(pd++) = rc;

				rextrax+=ratiox;
				if( rextrax > 1.0 && iextrax<extra_width) {
					rextrax-=1.0;
					iextrax++;
					*(pd++) = bc;
					*(pd++) = gc;
					*(pd++) = rc;
				}
			}
			for( ix=0; ix<extra_width-iextrax; ix++ ) {
				*(pd++) = bc;
				*(pd++) = gc;
				*(pd++) = rc;
			}
			pv+=extra_data_per_line;

			rextray+=ratioy;
			if( rextray > 1.0 && iextray<extra_height) {
				rextray-=1.0;
				iextray++;
				pd_last = pd-ogl_width*3;
				for( ix=0; ix<ogl_width*3; ix++ ) {
					*(pd++) = *(pd_last++);
				}
			}
		}
		for( iy=0; iy<extra_height-iextray; iy++ ) {
			pd_last = pd-ogl_width*3;
			for( ix=0; ix<ogl_width*3; ix++ ) {
				*(pd++) = *(pd_last++);
			}
		}


	}
	else if( bmih.biBitCount < 9 ) {
		int j;
		int jbit = 8 / bmih.biBitCount;

		unsigned char tmp_dat[8];
		int idat;


		extra_data = ( bmfh.bfSize - bmfh.bfOffBits ) * jbit - file_width*file_height;
		extra_data_per_line = extra_data / ( file_height * jbit );


		iextray = 0;
		rextray = 0.0;
		for( iy=0; iy<ny; iy++ ) {
			iextrax = 0;
			rextrax = 0.0;
			idat = 100;
			for( ix=0; ix<nx; ix++ ) {
				if( ++idat >= jbit ) {
					index = *(pv++);
					idat = 0;

					if( bmih.biBitCount == 1 ) {
						for( j=7; j>=0; j-- ) {
							tmp_dat[7-j] = ( index&( 1<<j ) ) ? 1 : 0;
						}
					}
					else if( bmih.biBitCount == 4 ) {
						tmp_dat[1] = index&15;
						tmp_dat[0] = index/16;
					}
					else {
						tmp_dat[0] = index;
					}
				}
				index = tmp_dat[idat];

				*(pd++) = bmiColors[index].rgbRed;
				*(pd++) = bmiColors[index].rgbGreen;
				*(pd++) = bmiColors[index].rgbBlue;


				rextrax+=ratiox;
				if( rextrax > 1.0 && iextrax<extra_width) {
					rextrax-=1.0;
					iextrax++;
					*(pd++) = bmiColors[index].rgbRed;
					*(pd++) = bmiColors[index].rgbGreen;
					*(pd++) = bmiColors[index].rgbBlue;
				}
			}
			for( ix=0; ix<extra_width-iextrax; ix++ ) {
				*(pd++) = bmiColors[index].rgbRed;
				*(pd++) = bmiColors[index].rgbGreen;
				*(pd++) = bmiColors[index].rgbBlue;
			}
			pv+=extra_data_per_line;

			rextray+=ratioy;
			if( rextray > 1.0 && iextray<extra_height) {
				rextray-=1.0;
				iextray++;
				pd_last = pd-ogl_width*3;
				for( ix=0; ix<ogl_width*3; ix++ ) {
					*(pd++) = *(pd_last++);
				}
			}
		}
		for( iy=0; iy<extra_height-iextray; iy++ ) {
			pd_last = pd-ogl_width*3;
			for( ix=0; ix<ogl_width*3; ix++ ) {
				*(pd++) = *(pd_last++);
			}
		}


	}




	delete []bmiColors;
	delete []lpvBits;

	CloseHandle(hfbm);


	return pdata;
}


