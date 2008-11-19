
#include "stdafx.h"


// #include "..\mathlib.h"

//#include "..\vrmlDoc.h"

#include "jpeglib.h"


#include <setjmp.h>



struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

/*
 * Sample routine for JPEG decompression.  We assume that the source file name
 * is passed in.  We want to return 1 on success, 0 on error.
 */


static void rgbtobgr( unsigned char* image_buffer, int size )
{
	unsigned char* pdata;
	unsigned char tmp;
	int idata;

	for( idata = 0, pdata = image_buffer;
		  idata<size; idata++ ) {
		tmp = pdata[0];
		pdata[0] = pdata[2];
		pdata[2] = tmp;
		pdata+=3;
	}
}

unsigned char* read_jpg_IJG(const char * filename, int* pwidth, int* pheight )
{


	BOOL bpreview = TRUE;


   *pwidth = 
   *pheight = 0;
//   FILE* fp;
  /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;
  /* We use our private extension JPEG error handler.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct my_error_mgr jerr;
  /* More stuff */
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

  /* In this example we want to open the input file before doing anything else,
   * so that the setjmp() error recovery below can assume the file is open.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to read binary files.
   */

  if ((infile = fopen(filename, "rb")) == NULL) {
    return NULL;
  }

  /* Step 1: allocate and initialize JPEG decompression object */

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer)) {
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return NULL;
  }
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */

  jpeg_stdio_src(&cinfo, infile);

  /* Step 3: read file parameters with jpeg_read_header() */

  (void) jpeg_read_header(&cinfo, TRUE);
  /* We can ignore the return value from jpeg_read_header since
   *   (a) suspension is not possible with the stdio data source, and
   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   * See libjpeg.doc for more info.
   */

  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: Start decompressor */

  (void) jpeg_start_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* We may need to do some setup of our own at this point before reading
   * the data.  After jpeg_start_decompress() we have the correct scaled
   * output image dimensions available, as well as the output colormap
   * if we asked for color quantization.
   * In this example, we need to make an output work buffer of the right size.
   */ 
  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */

//   fp = fopen( "my.log", "w" );



   /* Retrieve the BITMAPFILEHEADER structure. */ 




   int file_width = cinfo.image_width;
   int file_height = cinfo.image_height;

   int isize, ogl_width = 0, ogl_height = 0;
   int ibit;




   int ix, iy;
   int nx = file_width;
   int ny = file_height;
   int extra_width = 0;
   int extra_height = 0;



/*
   if( file_width > 1024 )
   {
      ogl_width = 1024;
   }
   else */ if( bpreview )
   {
      ogl_width = file_width;
   }
   else
   {
      for( ibit=1; ibit<12; ibit++ )
      {
         isize = 1<<ibit;
         if( file_width <= isize )
         {
             ogl_width = isize;
             break;
         }
      }
      extra_width = ogl_width - file_width;
   }

/*
   if( file_height > 1024 )
   {
      ogl_height = 1024;
   }
   else */ if( bpreview )
   {
      ogl_height = file_height;
   }
   else
   {
      for( ibit=1; ibit<12; ibit++ )
      {
         isize = 1<<ibit;
         if( file_height <= isize )
         {
             ogl_height = isize;
             break;
         }
      }
      extra_height = ogl_height - file_height;
   }



   if( ogl_height == 0 || ogl_width == 0 )
   {
      (void) jpeg_finish_decompress(&cinfo);
      jpeg_destroy_decompress(&cinfo);
      fclose(infile);
      return NULL;
   }



   double ratiox = 1.0001 * (double)extra_width / (double)nx;
   double ratioy = 1.0001 * (double)extra_height / (double)ny;
   int iextrax, iextray;
   double rextrax, rextray;

   int nnpixx = ogl_width*ogl_height;
   int other_datasize = nnpixx * 3;
   unsigned char* pdata = new unsigned char[ other_datasize ];
   unsigned char* pdat = pdata;
   unsigned char* pfiledata;
   unsigned char* pdtmp, chin;


   if( row_stride != 3*file_width &&
       row_stride != file_width)
   {
      (void) jpeg_finish_decompress(&cinfo);
      jpeg_destroy_decompress(&cinfo);
      fclose(infile);
      ASSERT( 0 );
      return NULL;
   }
   BOOL momocolor = ( row_stride == file_width );
//  while (cinfo.output_scanline < cinfo.output_height) {
   iextray = 0;
   rextray = 0.0;
   for( iy=0; iy<ny && cinfo.output_scanline < cinfo.output_height /* && iy < 1024 */; iy++)
   {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    /* Assume put_scanline_someplace wants a pointer and sample count. */
//    put_scanline_someplace(buffer[0], row_stride);


      iextrax = 0;
      rextrax = 0.0;

      pfiledata = buffer[0];

      for( ix=0; ix<nx /* && ix < 1024 */ ; ix++ )
      {
         if( momocolor )
         {
            chin = *(pfiledata++);
            *(pdat++) = chin;
            *(pdat++) = chin;
            *(pdat++) = chin;
         }
         else
         {
            *(pdat++) = *(pfiledata++);
            *(pdat++) = *(pfiledata++);
            *(pdat++) = *(pfiledata++);
         }

         rextrax+=ratiox;
         if( rextrax > 1.0 && iextrax < extra_width )
         {
            pdtmp = pdat - 3;
            rextrax-=1.0;
            iextrax++;
            *(pdat++) = *(pdtmp++);
            *(pdat++) = *(pdtmp++);
            *(pdat++) = *(pdtmp++);
         }
      }
      ASSERT( extra_width == iextrax );


      rextray+=ratioy;
      if( rextray > 1.0 && iextray < extra_height )
      {
         rextray-=1.0;
         iextray++;
         pfiledata = pdat - 3*ogl_width;

         for( ix=0; ix<ogl_width; ix++ )
         {
           *(pdat++) = *(pfiledata++);
           *(pdat++) = *(pfiledata++);
           *(pdat++) = *(pfiledata++);
         }         
      }
   }
   ASSERT( extra_height == iextray );

//   fclose( fp );
  /* Step 7: Finish decompression */

  (void) jpeg_finish_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  /* After finish_decompress, we can close the input file.
   * Here we postpone it until after no more JPEG errors are possible,
   * so as to simplify the setjmp error logic above.  (Actually, I don't
   * think that jpeg_destroy can do an error exit, but why assume anything...)
   */
  fclose(infile);

  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */

  /* And we're done! */

	*pwidth  = ogl_width; 
	*pheight = ogl_height; 

//  now invert the image!
   
   int i;
   int itop = 0;
   int ibot = ogl_height - 1;

   unsigned char* ptop, *pbot;

   unsigned char tmp;

   while( itop < ibot )
   {
      ptop = &pdata[ 3*ogl_width*itop ];
      pbot = &pdata[ 3*ogl_width*ibot ];
      for( i=0; i<ogl_width*3; i++ )
      {
         tmp = *(ptop);
         *(ptop++) = *(pbot);
         *(pbot++) = tmp;
      }
      itop++;
      ibot--;
   }
 
   return pdata;
}


void WriteJpeg( char * filename,
                unsigned char * image_buffer,
                int image_width,
                int image_height,
                int datasize )
{
  /* This struct contains the JPEG compression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   * It is possible to have several such structures, representing multiple
   * compression/decompression processes, in existence at once.  We refer
   * to any one struct (and its associated working data) as a "JPEG object".
   */
  struct jpeg_compress_struct cinfo;
  /* This struct represents a JPEG error handler.  It is declared separately
   * because applications often want to supply a specialized error handler
   * (see the second half of this file for an example).  But here we just
   * take the easy way out and use the standard error handler, which will
   * print a message on stderr and call exit() if compression fails.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE * outfile;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */

   int quaility = 75;

  /* Step 1: allocate and initialize JPEG compression object */

  /* We have to set up the error handler first, in case the initialization
   * step fails.  (Unlikely, but it could happen if you are out of memory.)
   * This routine fills in the contents of struct jerr, and returns jerr's
   * address which we place into the link field in cinfo.
   */
  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  /* Step 2: specify data destination (eg, a file) */
  /* Note: steps 2 and 3 can be done in either order. */

  /* Here we use the library-supplied code to send compressed data to a
   * stdio stream.  You can also write your own code to do something else.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to write binary files.
   */
  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return;
  }
  jpeg_stdio_dest(&cinfo, outfile);

  /* Step 3: set parameters for compression */

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */
  cinfo.image_width = image_width; 	/* image width and height, in pixels */
  cinfo.image_height = image_height;
  cinfo.input_components = 3;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
  /* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */
  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */

   int quality = 85;
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  /* Step 4: Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5: while (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */

  /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */
  row_stride = image_width * 3;	/* JSAMPLEs per row in image_buffer */

   rgbtobgr( image_buffer, image_width * image_height );

  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    row_pointer[0] = & image_buffer[
            ( image_height - cinfo.next_scanline - 1 ) * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  /* Step 6: Finish compression */

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  fclose(outfile);

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  /* And we're done! */

}


#ifdef SUPPORT_GIF_IJG

void start_input_gif (void* sinfo, int* pwidth, int* pheight);
unsigned char*  get_pixel_rows_gif0(void* sinfo);
void finish_input_gif (void* sinfo);
void* jinit_read_gif0(FILE* input_file );



unsigned char* read_gif_IJG(char * filename, int* pwidth, int* pheight, 
							int* ptruewidth, int* ptrueheight, BOOL bpreview )
{


   *pwidth = 
   *pheight = 0;

  void* src_mgr;
  FILE * input_file;

   if ((input_file = fopen(filename, "rb")) == NULL)
   {
      return NULL;
   }





  /* Figure out the input file format, and set up to read it. */
  src_mgr = jinit_read_gif0( input_file );

  /* Read the input file header to obtain file size & colorspace. */
  start_input_gif(src_mgr, pwidth, pheight);



   int file_width = *pwidth;
   int file_height = *pheight;
	*ptruewidth =  file_width;
	*ptrueheight = file_height; 

   int isize, ogl_width = 0, ogl_height = 0;
   int ibit;


   if( bpreview )
   {
      ogl_width = file_width;
      ogl_height = file_height;
   }
   else
   {
      for( ibit=1; ibit<12; ibit++ )
      {
         isize = 1<<ibit;
         if( file_width <= isize )
         {
             ogl_width = isize;
             break;
         }
      }
      for( ibit=1; ibit<12; ibit++ )
      {
         isize = 1<<ibit;
         if( file_height <= isize )
         {
             ogl_height = isize;
             break;
         }
      }
   }

  unsigned char* pdata = new unsigned char[ ogl_height*ogl_width*3 ];

  if( pdata )
  {

     unsigned char* buffer, *pda = pdata;
      int icol;

     for( int irow=0; irow<*pheight; irow++ )
     {
        buffer = get_pixel_rows_gif0(src_mgr);
        for( icol=0; icol<*pwidth; icol++ )
        {
           *pda++ = *buffer++;
           *pda++ = *buffer++;
           *pda++ = *buffer++;
        } 
     }

     /* Finish compression and release memory */

  }
  /* Close files, if we opened them */
  finish_input_gif(src_mgr);
  fclose(input_file);

   return pdata;
}


#endif












































/*
 * djpeg.c
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains a command-line user interface for the JPEG decompressor.
 * It should work on any system with Unix- or MS-DOS-style command lines.
 *
 * Two different command line styles are permitted, depending on the
 * compile-time switch TWO_FILE_COMMANDLINE:
 *	djpeg [options]  inputfile outputfile
 *	djpeg [options]  [inputfile]
 * In the second style, output is always to standard output, which you'd
 * normally redirect to a file or pipe to some other program.  Input is
 * either from a named file or from standard input (typically redirected).
 * The second style is convenient on Unix but is unhelpful on systems that
 * don't support pipes.  Also, you MUST use the first style if your system
 * doesn't do binary I/O to stdin/stdout.
 * To simplify script writing, the "-outfile" switch is provided.  The syntax
 *	djpeg [options]  -outfile outputfile  inputfile
 * works regardless of which command line style is used.
 */

#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jversion.h"		/* for version message */

#include <ctype.h>		/* to declare isprint() */

/* Create the add-on message string table. */


static const char * const cdjpeg_message_table[] = {
#include "cderror.h"
  NULL
};


/*
 * This list defines the known output image formats
 * (not all of which need be supported by a given version).
 * You can change the default output format by defining DEFAULT_FMT;
 * indeed, you had better do so if you undefine PPM_SUPPORTED.
 */



/*
 * Argument-parsing code.
 * The switch parser is designed to be useful with DOS-style command line
 * syntax, ie, intermixed switches and file names, where only the switches
 * to the left of a given file name affect processing of that file.
 * The main program in this file doesn't actually use this capability...
 */



/*
 * Marker processor for COM markers.
 * This replaces the library's built-in processor, which just skips the marker.
 * We want to print out the marker as text, if possible.
 * Note this code relies on a non-suspending data source.
 */


/*
 * The main program.
 */





typedef struct tagImageIO
{
   int width;
   int height;
   unsigned char* pcolormap;
   unsigned char* pdata;
   int ncolors;
   int nsteps;
   double stepsize;
   int istep;
   int tranp;

} ImageIO;

void SetCinfo( djpeg_dest_ptr dest_mgr, ImageIO* cinfo );

void emit_image_header ( djpeg_dest_ptr dest_mgr );

void start_output_gif (ImageIO* cinfo, djpeg_dest_ptr dinfo);

void put_pixel_rows ( djpeg_dest_ptr dinfo, unsigned char* pdata );
void finish_output_gif ( djpeg_dest_ptr dinfo);
djpeg_dest_ptr myinit_write_gif (ImageIO* cinfo);


ImageIO* GetCinfo( djpeg_dest_ptr dest_mgr );

void finish_output_gif_frame ( djpeg_dest_ptr dinfo);
void finish_output_animated_gif ( djpeg_dest_ptr dinfo);




void BuildColormap( ImageIO* cinfo )
{
#ifdef NEEDTHIS

   int k=0;
   for( k=0; k<256*3; k++ )
   {
      cinfo->pcolormap[k] = 0;
   }

   int i1, i2, i3;
   unsigned char v1, v2, v3;
   int icolor = 0;
   unsigned char* pmap = cinfo->pcolormap;
   for( i1=0; i1<3; i1++ )
   {
      v1 = ( i1==0 ) ? 0 : ( i1 == 1 ) ? 127 : 255;
      for( i2=0; i2<3; i2++ )
      {
         v2 = ( i2==0 ) ? 0 : ( i2 == 1 ) ? 127 : 255;
         for( i3=0; i3<3; i3++ )
         {
            v3 = ( i3==0 ) ? 0 : ( i3 == 1 ) ? 127 : 255;

            *( pmap++ ) = v1;
            *( pmap++ ) = v2;
            *( pmap++ ) = v3;
            icolor++;
         }
      }
   }


   int isize = cinfo->width*cinfo->height;
   int ic, ixy, idiff;
   int diffthresh = 100;
   int itry = 0;
   unsigned char* pda, *pcolor;
   int icolor_last = icolor;

   while( icolor < 256 )
   {
      itry++;
      if( itry%1000 == 0 )
      {
         if( icolor - icolor_last < 20 )
         {
            diffthresh /= 2;
         }
         icolor_last = icolor;

      }
      ixy = ( int )( rnd()*isize );
      pda = cinfo->pdata+ixy*3;

 
      for( ic=0; ic<icolor; ic++ )
      {
         pcolor = &cinfo->pcolormap[3*ic];
         idiff = abs( pda[0]-pcolor[2] ) + abs( pda[1]-pcolor[1] ) + abs( pda[2]-pcolor[0] );
         if( idiff < diffthresh )
         {
            break;
         }
      }
      if( ic>=icolor )
      {
         *( pmap++ ) = pda[2];
         *( pmap++ ) = pda[1];
         *( pmap++ ) = pda[0];
         icolor++;
      }
   }
#endif
}


void RGBtoIndex( unsigned char* pcolormap, unsigned char* pda, 
            unsigned char* pindexes, int ncols, int ncolors )
{
   int ibest, diffbest, idiff;
   unsigned char v1, v2, v3;
   unsigned char *pcolor;

   for( int icol=0; icol<ncols; icol++ )
   {
      v1 = *(pda++);
      v2 = *(pda++);
      v3 = *(pda++);
      diffbest = 9999999;
      ibest = 0;
      for( int i=0; i<ncolors; i++ )
      {
         pcolor = &pcolormap[3*i];
         
         idiff = abs( v1-pcolor[2] ) + abs( v2-pcolor[1] ) + abs( v3-pcolor[0] );
         if( idiff < diffbest )
         {
            ibest = i;
            diffbest = idiff;
         }
      }
      *( pindexes++ ) = ibest;
   }
}


