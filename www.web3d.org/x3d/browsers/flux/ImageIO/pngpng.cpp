#include "stdafx.h"

/* png.c - location for general purpose libpng functions
 *
 * libpng version 1.0.5 - October 15, 1999
 * Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.
 * Copyright (c) 1996, 1997 Andreas Dilger
 * Copyright (c) 1998, 1999 Glenn Randers-Pehrson
 * 
 */

#define PNG_INTERNAL
#define PNG_NO_EXTERN
#include "png.h"

PNG_GET_HEADER

/* Version information for C files.  This had better match the version
 * string defined in png.h.
 */

char png_libpng_ver[12] = "1.0.5";

/* Place to hold the signature string for a PNG file. */
png_byte FARDATA png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};

/* Constant strings for known chunk types.  If you need to add a chunk,
 * add a string holding the name here.  If you want to make the code
 * portable to EBCDIC machines, use ASCII numbers, not characters.
 */
png_byte FARDATA png_IHDR[5] = { 73,  72,  68,  82, '\0'};
png_byte FARDATA png_IDAT[5] = { 73,  68,  65,  84, '\0'};
png_byte FARDATA png_IEND[5] = { 73,  69,  78,  68, '\0'};
png_byte FARDATA png_PLTE[5] = { 80,  76,  84,  69, '\0'};
png_byte FARDATA png_bKGD[5] = { 98,  75,  71,  68, '\0'};
png_byte FARDATA png_cHRM[5] = { 99,  72,  82,  77, '\0'};
png_byte FARDATA png_gAMA[5] = {103,  65,  77,  65, '\0'};
png_byte FARDATA png_hIST[5] = {104,  73,  83,  84, '\0'};
png_byte FARDATA png_oFFs[5] = {111,  70,  70, 115, '\0'};
png_byte FARDATA png_pCAL[5] = {112,  67,  65,  76, '\0'};
png_byte FARDATA png_pHYs[5] = {112,  72,  89, 115, '\0'};
png_byte FARDATA png_sBIT[5] = {115,  66,  73,  84, '\0'};
png_byte FARDATA png_sRGB[5] = {115,  82,  71,  66, '\0'};
png_byte FARDATA png_tEXt[5] = {116,  69,  88, 116, '\0'};
png_byte FARDATA png_tIME[5] = {116,  73,  77,  69, '\0'};
png_byte FARDATA png_tRNS[5] = {116,  82,  78,  83, '\0'};
png_byte FARDATA png_zTXt[5] = {122,  84,  88, 116, '\0'};


/* arrays to facilitate easy interlacing - use pass (0 - 6) as index */

/* start of interlace block */
int FARDATA png_pass_start[] = {0, 4, 0, 2, 0, 1, 0};

/* offset to next interlace block */
int FARDATA png_pass_inc[] = {8, 8, 4, 4, 2, 2, 1};

/* start of interlace block in the y direction */
int FARDATA png_pass_ystart[] = {0, 0, 4, 0, 2, 0, 1};

/* offset to next interlace block in the y direction */
int FARDATA png_pass_yinc[] = {8, 8, 8, 4, 4, 2, 2};

/* Width of interlace block.  This is not currently used - if you need
 * it, uncomment it here and in png.h
int FARDATA png_pass_width[] = {8, 4, 4, 2, 2, 1, 1};
*/

/* Height of interlace block.  This is not currently used - if you need
 * it, uncomment it here and in png.h
int FARDATA png_pass_height[] = {8, 8, 4, 4, 2, 2, 1};
*/

/* Mask to determine which pixels are valid in a pass */
int FARDATA png_pass_mask[] = {0x80, 0x08, 0x88, 0x22, 0xaa, 0x55, 0xff};

/* Mask to determine which pixels to overwrite while displaying */
int FARDATA png_pass_dsp_mask[] = {0xff, 0x0f, 0xff, 0x33, 0xff, 0x55, 0xff};

/* Tells libpng that we have already handled the first "num_bytes" bytes
 * of the PNG file signature.  If the PNG data is embedded into another
 * stream we can set num_bytes = 8 so that libpng will not attempt to read
 * or write any of the magic bytes before it starts on the IHDR.
 */

void
png_set_sig_bytes(png_structp png_ptr, int num_bytes)
{
   png_debug(1, "in png_set_sig_bytes\n");
   if (num_bytes > 8)
      png_error(png_ptr, "Too many bytes for PNG signature.");

   png_ptr->sig_bytes = (png_byte)(num_bytes < 0 ? 0 : num_bytes);
}

/* Checks whether the supplied bytes match the PNG signature.  We allow
 * checking less than the full 8-byte signature so that those apps that
 * already read the first few bytes of a file to determine the file type
 * can simply check the remaining bytes for extra assurance.  Returns
 * an integer less than, equal to, or greater than zero if sig is found,
 * respectively, to be less than, to match, or be greater than the correct
 * PNG signature (this is the same behaviour as strcmp, memcmp, etc).
 */
int
png_sig_cmp(png_bytep sig, png_size_t start, png_size_t num_to_check)
{
   if (num_to_check > 8)
      num_to_check = 8;
   else if (num_to_check < 1)
      return (0);

   if (start > 7)
      return (0);

   if (start + num_to_check > 8)
      num_to_check = 8 - start;

   return ((int)(png_memcmp(&sig[start], &png_sig[start], num_to_check)));
}

/* (Obsolete) function to check signature bytes.  It does not allow one
 * to check a partial signature.  This function might be removed in the
 * future - use png_sig_cmp().  Returns true (nonzero) if the file is a PNG.
 */
int
png_check_sig(png_bytep sig, int num)
{
  return ((int)!png_sig_cmp(sig, (png_size_t)0, (png_size_t)num));
}

/* Function to allocate memory for zlib. */
voidpf
png_zalloc(voidpf png_ptr, uInt items, uInt size)
{
   png_uint_32 num_bytes = (png_uint_32)items * size;
   png_voidp ptr = (png_voidp)png_malloc((png_structp)png_ptr, num_bytes);

   if (num_bytes > (png_uint_32)0x8000L)
   {
      png_memset(ptr, 0, (png_size_t)0x8000L);
      png_memset((png_bytep)ptr + (png_size_t)0x8000L, 0,
         (png_size_t)(num_bytes - (png_uint_32)0x8000L));
   }
   else
   {
      png_memset(ptr, 0, (png_size_t)num_bytes);
   }
   return ((voidpf)ptr);
}

/* function to free memory for zlib */
void
png_zfree(voidpf png_ptr, voidpf ptr)
{
   png_free((png_structp)png_ptr, (png_voidp)ptr);
}

/* Reset the CRC variable to 32 bits of 1's.  Care must be taken
 * in case CRC is > 32 bits to leave the top bits 0.
 */
void
png_reset_crc(png_structp png_ptr)
{
   png_ptr->crc = crc32(0, Z_NULL, 0);
}

/* Calculate the CRC over a section of data.  We can only pass as
 * much data to this routine as the largest single buffer size.  We
 * also check that this data will actually be used before going to the
 * trouble of calculating it.
 */
void
png_calculate_crc(png_structp png_ptr, png_bytep ptr, png_size_t length)
{
   int need_crc = 1;

   if (png_ptr->chunk_name[0] & 0x20)                     /* ancillary */
   {
      if ((png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_MASK) ==
          (PNG_FLAG_CRC_ANCILLARY_USE | PNG_FLAG_CRC_ANCILLARY_NOWARN))
         need_crc = 0;
   }
   else                                                    /* critical */
   {
      if (png_ptr->flags & PNG_FLAG_CRC_CRITICAL_IGNORE)
         need_crc = 0;
   }

   if (need_crc)
      png_ptr->crc = crc32(png_ptr->crc, ptr, (uInt)length);
}

/* Allocate the memory for an info_struct for the application.  We don't
 * really need the png_ptr, but it could potentially be useful in the
 * future.  This should be used in favour of malloc(sizeof(png_info))
 * and png_info_init() so that applications that want to use a shared
 * libpng don't have to be recompiled if png_info changes size.
 */
png_infop
png_create_info_struct(png_structp png_ptr)
{
   png_infop info_ptr;

   png_debug(1, "in png_create_info_struct\n");
   if(png_ptr == NULL) return (NULL);
#ifdef PNG_USER_MEM_SUPPORTED
   if ((info_ptr = (png_infop)png_create_struct_2(PNG_STRUCT_INFO,
      png_ptr->malloc_fn)) != NULL)
#else
   if ((info_ptr = (png_infop)png_create_struct(PNG_STRUCT_INFO)) != NULL)
#endif
   {
      png_info_init(info_ptr);
   }

   return (info_ptr);
}

/* This function frees the memory associated with a single info struct.
 * Normally, one would use either png_destroy_read_struct() or
 * png_destroy_write_struct() to free an info struct, but this may be
 * useful for some applications.
 */
void
png_destroy_info_struct(png_structp png_ptr, png_infopp info_ptr_ptr)
{
   png_infop info_ptr = NULL;

   png_debug(1, "in png_destroy_info_struct\n");
   if (info_ptr_ptr != NULL)
      info_ptr = *info_ptr_ptr;

   if (info_ptr != NULL)
   {
      png_info_destroy(png_ptr, info_ptr);

#ifdef PNG_USER_MEM_SUPPORTED
      png_destroy_struct_2((png_voidp)info_ptr, png_ptr->free_fn);
#else
      png_destroy_struct((png_voidp)info_ptr);
#endif
      *info_ptr_ptr = (png_infop)NULL;
   }
}

/* Initialize the info structure.  This is now an internal function (0.89)
 * and applications using it are urged to use png_create_info_struct()
 * instead.
 */
void
png_info_init(png_infop info_ptr)
{
   png_debug(1, "in png_info_init\n");
   /* set everything to 0 */
   png_memset(info_ptr, 0, sizeof (png_info));
}

/* This is an internal routine to free any memory that the info struct is
 * pointing to before re-using it or freeing the struct itself.  Recall
 * that png_free() checks for NULL pointers for us.
 */
void
png_info_destroy(png_structp png_ptr, png_infop info_ptr)
{
#if defined(PNG_READ_tEXt_SUPPORTED) || defined(PNG_READ_zTXt_SUPPORTED)
   png_debug(1, "in png_info_destroy\n");
   if (info_ptr->text != NULL)
   {
      int i;
      for (i = 0; i < info_ptr->num_text; i++)
      {
         png_free(png_ptr, info_ptr->text[i].key);
      }
      png_free(png_ptr, info_ptr->text);
   }
#endif
#if defined(PNG_READ_pCAL_SUPPORTED)
   png_free(png_ptr, info_ptr->pcal_purpose);
   png_free(png_ptr, info_ptr->pcal_units);
   if (info_ptr->pcal_params != NULL)
   {
      int i;
      for (i = 0; i < (int)info_ptr->pcal_nparams; i++)
      {
         png_free(png_ptr, info_ptr->pcal_params[i]);
      }
      png_free(png_ptr, info_ptr->pcal_params);
   }
#endif

   png_info_init(info_ptr);
}

/* This function returns a pointer to the io_ptr associated with the user
 * functions.  The application should free any memory associated with this
 * pointer before png_write_destroy() or png_read_destroy() are called.
 */
png_voidp
png_get_io_ptr(png_structp png_ptr)
{
   return (png_ptr->io_ptr);
}

#if !defined(PNG_NO_STDIO)
/* Initialize the default input/output functions for the PNG file.  If you
 * use your own read or write routines, you can call either png_set_read_fn()
 * or png_set_write_fn() instead of png_init_io().
 */
void
png_init_io(png_structp png_ptr, FILE *fp)
{
   png_debug(1, "in png_init_io\n");
   png_ptr->io_ptr = (png_voidp)fp;
}
#endif

#if defined(PNG_TIME_RFC1123_SUPPORTED)
/* Convert the supplied time into an RFC 1123 string suitable for use in
 * a "Creation Time" or other text-based time string.
 */
png_charp
png_convert_to_rfc1123(png_structp png_ptr, png_timep ptime)
{
   static PNG_CONST char short_months[12][4] =
        {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

   if (png_ptr->time_buffer == NULL)
   {
      png_ptr->time_buffer = (png_charp)png_malloc(png_ptr, (png_uint_32)(29*
         sizeof(char)));
   }

#ifdef USE_FAR_KEYWORD
   {
      char near_time_buf[29];
      sprintf(near_time_buf, "%d %s %d %02d:%02d:%02d +0000",
               ptime->day % 32, short_months[(ptime->month - 1) % 12],
               ptime->year, ptime->hour % 24, ptime->minute % 60,
               ptime->second % 61);
      png_memcpy(png_ptr->time_buffer, near_time_buf,
      29*sizeof(char));
   }
#else
   sprintf(png_ptr->time_buffer, "%d %s %d %02d:%02d:%02d +0000",
               ptime->day % 32, short_months[(ptime->month - 1) % 12],
               ptime->year, ptime->hour % 24, ptime->minute % 60,
               ptime->second % 61);
#endif
   return ((png_charp)png_ptr->time_buffer);
}
#endif /* PNG_TIME_RFC1123_SUPPORTED */

png_charp
png_get_copyright(png_structp png_ptr)
{
   if (png_ptr != NULL || png_ptr == NULL)  /* silence compiler warning */
   return ("\n libpng version 1.0.5 - October 15, 1999\n\
   Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.\n\
   Copyright (c) 1996, 1997 Andreas Dilger\n\
   Copyright (c) 1998, 1999 Glenn Randers-Pehrson\n");
   return ("");
}

/* Generate a compiler error if there is an old png.h in the search path. */
void
png_check_version
   (version_1_0_5 png_h_is_not_version_1_0_5)
{
   if(png_h_is_not_version_1_0_5 == NULL)
     return;
}
















#ifdef asdfdasdf

#define PNG_BYTES_TO_CHECK 4
int check_if_png(char *file_name, FILE **fp)
{
   char buf[PNG_BYTES_TO_CHECK];

   /* Open the prospective PNG file. */
   if ((*fp = fopen(file_name, "rb")) != NULL)
      return 0;

   /* Read in some of the signature bytes */
   if (fread(buf, 1, PNG_BYTES_TO_CHECK, *fp) != PNG_BYTES_TO_CHECK)
      return 0;

   /* Compare the first PNG_BYTES_TO_CHECK bytes of the signature.
      Return nonzero (true) if they match */

   return(!png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK));
}


#endif

/* Read a PNG file.  You may want to return an error code if the read
 * fails (depending upon the failure).  There are two "prototypes" given
 * here - one where we are given the filename, and we need to open the
 * file, and the other where we are given an open file (possibly with
 * some or all of the magic bytes read - see comments above).
 */



unsigned char* read_png_png(const char *file_name, int* pwidth, int* pheight, int* pnBytes )  
{


	BOOL bpreview = TRUE;


   png_structp png_ptr;
   png_infop info_ptr;
   unsigned int sig_read = 0;
   png_uint_32 width, height;
   int bit_depth, color_type, interlace_type;
   FILE *fp;

   if ((fp = fopen(file_name, "rb")) == NULL)
      return NULL;

   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also supply the
    * the compiler header file version, so that we know if the application
    * was compiled with a compatible version of the library.  REQUIRED
    */
   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
         NULL, NULL, NULL );
//      png_voidp user_error_ptr, user_error_fn, user_warning_fn);

   if (png_ptr == NULL)
   {
      fclose(fp);
      return NULL;
   }

   /* Allocate/initialize the memory for image information.  REQUIRED. */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fclose(fp);
      png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
      return NULL;
   }

   /* Set error handling if you are using the setjmp/longjmp method (this is
    * the normal method of doing things with libpng).  REQUIRED unless you
    * set up your own error handlers in the png_create_read_struct() earlier.
    */
   if (setjmp(png_ptr->jmpbuf))
   {
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
      fclose(fp);
      /* If we get here, we had a problem reading the file */
      return NULL;
   }

   /* One of the following I/O initialization methods is REQUIRED */
   /* Set up the input control if you are using standard C streams */
   png_init_io(png_ptr, fp);

   /* If we have already read some of the signature */
   png_set_sig_bytes(png_ptr, sig_read);

   /* The call to png_read_info() gives us all of the information from the
    * PNG file before the first IDAT (image data chunk).  REQUIRED
    */
   png_read_info(png_ptr, info_ptr);

   png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
       &interlace_type, NULL, NULL);




{
   unsigned int nBytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
   unsigned int nBytesPerPixel = nBytesPerRow / width;
int iii=0;
}



/**** Set up the data transformations you want.  Note that these are all
 **** optional.  Only call them if you want/need them.  Many of the
 **** transformations only work on specific types of images, and many
 **** are mutually exclusive.
 ****/

   /* tell libpng to strip 16 bit/color files down to 8 bits/color */
//   png_set_strip_16(png_ptr);

   /* Strip alpha bytes from the input data without combining with the
    * background (not recommended).
    */
//   png_set_strip_alpha(png_ptr);

   /* Extract multiple pixels with bit depths of 1, 2, and 4 from a single
    * byte into separate bytes (useful for paletted and grayscale images).
    */
//   png_set_packing(png_ptr);

   /* Change the order of packed pixels to least significant bit first
    * (not useful if you are using png_set_packing). */
//   png_set_packswap(png_ptr);

   /* Expand paletted colors into true RGB triplets */
   if (color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_expand(png_ptr);

   /* Expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel */
   if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
      png_set_expand(png_ptr);

   /* Expand paletted or RGB images with transparency to full alpha channels
    * so the data will be available as RGBA quartets.
    */
   if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
      png_set_expand(png_ptr);

   /* Set the background color to draw transparent and alpha images over.
    * It is possible to set the red, green, and blue components directly
    * for paletted images instead of supplying a palette index.  Note that
    * even if the PNG file supplies a background, you are not required to
    * use it - you should use the (solid) application background if it has one.
    */
/**
   png_color_16 my_background, *image_background;

   if (png_get_bKGD(png_ptr, info_ptr, &image_background))
      png_set_background(png_ptr, image_background,
                         PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
   else
      png_set_background(png_ptr, &my_background,
                         PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
****/
   /* Some suggestions as to how to get a screen gamma value */

   /* Note that screen gamma is the display_exponent, which includes
    * the CRT_exponent and any correction for viewing conditions */
   char* gamma_str;

   double screen_gamma;

   if ( 0 ) // /* We have a user-defined screen gamma value */)
   {
  //    screen_gamma = user-defined screen_gamma;
   }
   /* This is one way that applications share the same screen gamma value */
   else if ((gamma_str = getenv("SCREEN_GAMMA")) != NULL)
   {
      screen_gamma = atof(gamma_str);
   }
   /* If we don't have another value */
   else
   {
      screen_gamma = 2.2;  /* A good guess for a PC monitors in a dimly
                              lit room */
//      screen_gamma = 1.7 or 1.0;  /* A good guess for Mac systems */
   }

   /* Tell libpng to handle the gamma conversion for you.  The second call
    * is a good guess for PC generated images, but it should be configurable
    * by the user at run time by the user.  It is strongly suggested that
    * your application support gamma correction.
    */
#ifdef HUGHGH
   int intent;

   if (png_get_sRGB(png_ptr, info_ptr, &intent))
      png_set_sRGB(png_ptr, intent, 0);
   else
   {
      double image_gamma;
      if (png_get_gAMA(png_ptr, info_ptr, &image_gamma))
         png_set_gamma(png_ptr, screen_gamma, image_gamma);
      else
         png_set_gamma(png_ptr, screen_gamma, 0.45455);
   }
#endif
   /* Dither RGB files down to 8 bit palette or reduce palettes
    * to the number of colors available on your screen.
    */
#ifdef DITHERDITHER

   if (color_type & PNG_COLOR_MASK_COLOR)
   {
      png_uint_32 num_palette;
      png_colorp palette;

      /* This reduces the image to the application supplied palette */
      if ( 0 ) // /* we have our own palette */)
      {
         /* An array of colors to which the image should be dithered */
//         png_color std_color_cube[MAX_SCREEN_COLORS];

//         png_set_dither(png_ptr, std_color_cube, MAX_SCREEN_COLORS,
//            MAX_SCREEN_COLORS, NULL, 0);
      }
      /* This reduces the image to the palette supplied in the file */
      else if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette))
      {
         png_color16p histogram;

         png_get_hIST(png_ptr, info_ptr, &histogram);

         png_set_dither(png_ptr, palette, num_palette,
                        max_screen_colors, histogram, 0);
      }
   }
#endif
   /* invert monocrome files to have 0 as white and 1 as black */
//   png_set_invert_mono(png_ptr);

   /* If you want to shift the pixel values from the range [0,255] or
    * [0,65535] to the original [0,7] or [0,31], or whatever range the
    * colors were originally in:
    */
#ifdef SHIFTSHIFT
   if (png_get_valid(png_ptr, info_ptr, PNG_INFO_sBIT))
   {
      png_color8p sig_bit;

      png_get_sBIT(png_ptr, info_ptr, &sig_bit);
      png_set_shift(png_ptr, sig_bit);
   }
#endif

   /* flip the RGB pixels to BGR (or RGBA to BGRA) */
//   png_set_bgr(png_ptr);

   /* swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR) */
//   png_set_swap_alpha(png_ptr);

   /* swap bytes of 16 bit files to least significant byte first */
//   png_set_swap(png_ptr);

   /* Add filler (or alpha) byte (before/after each RGB triplet) */
//   png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

   /* Turn on interlace handling.  REQUIRED if you are not using
    * png_read_image().  To see how to handle interlacing passes,
    * see the png_read_row() method below:
    */
//   number_passes = png_set_interlace_handling(png_ptr);

{
   unsigned int nBytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
   unsigned int nBytesPerPixel = nBytesPerRow / width;
int iii=0;
}




   if (png_get_valid(png_ptr, info_ptr,PNG_INFO_tRNS))
   {
      png_set_tRNS_to_alpha(png_ptr);
   }



   /* Optional call to gamma correct and add the background to the palette
    * and update info structure.  REQUIRED if you are expecting libpng to
    * update the palette for you (ie you selected such a transform above).
    */
   png_read_update_info(png_ptr, info_ptr);

   /* Allocate the memory to hold the image using the fields of info_ptr. */

   /* The easiest way to read the image: */


   unsigned int nBytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
   unsigned int nBytesPerPixel = nBytesPerRow / width;
















   unsigned int isize, ogl_width = 0, ogl_height = 0;
   int ibit;

   int extra_width = 0;
   int extra_height = 0;

   if( bpreview )
   {
      ogl_width = width;
   }
   else
   {
      for( int ibit=1; ibit<12; ibit++ )
      {
         isize = 1<<ibit;
         if( width <= isize )
         {
             ogl_width = isize;
             break;
         }
      }
      extra_width = ogl_width - width;
   }


   if( bpreview )
   {
      ogl_height = height;
   }
   else
   {
      for( ibit=1; ibit<12; ibit++ )
      {
         isize = 1<<ibit;
         if( height <= isize )
         {
             ogl_height = isize;
             break;
         }
      }
      extra_height = ogl_height - height;
   }



   png_bytep* row_pointers = new png_bytep[height];
   unsigned char* pMyData = new unsigned char[nBytesPerPixel*height*width];

   for ( unsigned int row = 0; row < height; row++)
   {
      row_pointers[row] = &pMyData[( height - row - 1 ) * width * nBytesPerPixel];
   }




   /* Now it's time to read the image.  One of these methods is REQUIRED */

   png_read_image(png_ptr, row_pointers);


   /* read rest of file, and get additional chunks in info_ptr - REQUIRED */
   png_read_end(png_ptr, info_ptr);

   /* clean up after the read, and free any memory allocated - REQUIRED */
   png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

   /* close the file */
   fclose(fp);



   if( extra_width || extra_height )
   {
      unsigned int i, iy_ogl, ix_ogl, ix, iy;
      unsigned char* pData0 = new unsigned char[nBytesPerPixel*ogl_height*ogl_width];

      unsigned char* pData = pData0;

      double yscl = 0.999 * (( double ) height ) / (( double ) ogl_height );
      double xscl = 0.999 * (( double ) width  ) / (( double ) ogl_width  );


      for( iy_ogl=0; iy_ogl<ogl_height; iy_ogl++ )
      {
         iy = ( int )( ((double)iy_ogl ) * yscl );
         for( ix_ogl=0; ix_ogl<ogl_width; ix_ogl++ )
         {
            ix = ( int )( ((double)ix_ogl ) * xscl );
            if( ix >= 0 && ix < width &&
                iy >= 0 && iy < height )
            {
               for( i=0; i<nBytesPerPixel; i++ )
               {
                  pData[i] = pMyData[ i + nBytesPerPixel * ( ix + iy*width )];
               }
               pData+=nBytesPerPixel;
            }
            else
            {
               ASSERT( 0 );
            }
         }
      }

      delete [] pMyData;
      pMyData = pData0;
   }

	*pwidth  = ogl_width;
	*pheight = ogl_height;
	*pnBytes = nBytesPerPixel;

	delete [] row_pointers;

	/* that's it */
	return pMyData;
}

/* progressively read a file */


#ifdef asdfasfdasfdasfdasdfasdf

int
initialize_png_reader(png_structp *png_ptr, png_infop *info_ptr)
{
   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also check that
    * the library version is compatible in case we are using dynamically
    * linked libraries.
    */
   *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
       png_voidp user_error_ptr, user_error_fn, user_warning_fn);

   if (*png_ptr == NULL)
   {
      *info_ptr = NULL;
      return ERROR;
   }

   *info_ptr = png_create_info_struct(png_ptr);

   if (*info_ptr == NULL)
   {
      png_destroy_read_struct(png_ptr, info_ptr, (png_infopp)NULL);
      return ERROR;
   }

   if (setjmp((*png_ptr)->jmpbuf))
   {
      png_destroy_read_struct(png_ptr, info_ptr, (png_infopp)NULL);
      return ERROR;
   }

   /* This one's new.  You will need to provide all three
    * function callbacks, even if you aren't using them all.
    * If you aren't using all functions, you can specify NULL
    * parameters.  Even when all three functions are NULL,
    * you need to call png_set_progressive_read_fn().
    * These functions shouldn't be dependent on global or
    * static variables if you are decoding several images
    * simultaneously.  You should store stream specific data
    * in a separate struct, given as the second parameter,
    * and retrieve the pointer from inside the callbacks using
    * the function png_get_progressive_ptr(png_ptr).
    */
   png_set_progressive_read_fn(*png_ptr, (void *)stream_data,
      info_callback, row_callback, end_callback);

   return OK;
}

int
process_data(png_structp *png_ptr, png_infop *info_ptr,
   png_bytep buffer, png_uint_32 length)
{
   if (setjmp((*png_ptr)->jmpbuf))
   {
      /* Free the png_ptr and info_ptr memory on error */
      png_destroy_read_struct(png_ptr, info_ptr, (png_infopp)NULL);
      return ERROR;
   }

   /* This one's new also.  Simply give it chunks of data as
    * they arrive from the data stream (in order, of course).
    * On Segmented machines, don't give it any more than 64K.
    * The library seems to run fine with sizes of 4K, although
    * you can give it much less if necessary (I assume you can
    * give it chunks of 1 byte, but I haven't tried with less
    * than 256 bytes yet).  When this function returns, you may
    * want to display any rows that were generated in the row
    * callback, if you aren't already displaying them there.
    */
   png_process_data(*png_ptr, *info_ptr, buffer, length);
   return OK;
}

info_callback(png_structp png_ptr, png_infop info)
{
/* do any setup here, including setting any of the transformations
 * mentioned in the Reading PNG files section.  For now, you _must_
 * call either png_start_read_image() or png_read_update_info()
 * after all the transformations are set (even if you don't set
 * any).  You may start getting rows before png_process_data()
 * returns, so this is your last chance to prepare for that.
 */
}

row_callback(png_structp png_ptr, png_bytep new_row,
   png_uint_32 row_num, int pass)
{
/* this function is called for every row in the image.  If the
 * image is interlacing, and you turned on the interlace handler,
 * this function will be called for every row in every pass.
 * Some of these rows will not be changed from the previous pass.
 * When the row is not changed, the new_row variable will be NULL.
 * The rows and passes are called in order, so you don't really
 * need the row_num and pass, but I'm supplying them because it
 * may make your life easier.
 *
 * For the non-NULL rows of interlaced images, you must call
 * png_progressive_combine_row() passing in the row and the
 * old row.  You can call this function for NULL rows (it will
 * just return) and for non-interlaced images (it just does the
 * memcpy for you) if it will make the code easier.  Thus, you
 * can just do this for all cases:
 */

   png_progressive_combine_row(png_ptr, old_row, new_row);

/* where old_row is what was displayed for previous rows.  Note
 * that the first pass (pass == 0 really) will completely cover
 * the old row, so the rows do not have to be initialized.  After
 * the first pass (and only for interlaced images), you will have
 * to pass the current row, and the function will combine the
 * old row and the new row.
 */
}

end_callback(png_structp png_ptr, png_infop info)
{
/* this function is called when the whole image has been read,
 * including any chunks after the image (up to and including
 * the IEND).  You will usually have the same info chunk as you
 * had in the header, although some data may have been added
 * to the comments and time fields.
 *
 * Most people won't do much here, perhaps setting a flag that
 * marks the image as finished.
 */
}



#endif











#ifdef asdfasdfasdfsdf

	bool bVerbose=false;
	bool wrote_question=false;

   static FILE *fpout;  /* "static" prevents setjmp corruption */
   png_structp write_ptr;
   png_infop write_info_ptr;
   png_bytep row_buf;
   png_uint_32 y;
   png_uint_32 width, height;
   int num_pass, pass;
   int bit_depth, color_type;
#ifdef USE_FAR_KEYWORD
   jmp_buf jmpbuf;
#endif   

   row_buf = (png_bytep)NULL;

   if ((fpout = fopen(outname, "wb")) == NULL)
   {
//      fprintf(STDERR, "Could not open output file %s\n", outname);
      return (1);
   }

   png_debug(0, "Allocating read and write structures\n");
#if defined(PNG_NO_STDIO)
//   png_set_error_fn(read_ptr, (png_voidp)inname, png_default_error,
  //     png_default_warning);
#endif
#ifdef PNG_USER_MEM_SUPPORTED
   write_ptr = png_create_write_struct_2(PNG_LIBPNG_VER_STRING, (png_voidp)NULL,
      (png_error_ptr)NULL, (png_error_ptr)NULL, (png_voidp)NULL,
      (png_malloc_ptr)png_debug_malloc, (png_free_ptr)png_debug_free);
#else
   write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp)NULL,
      (png_error_ptr)NULL, (png_error_ptr)NULL);
#endif
#if defined(PNG_NO_STDIO)
   png_set_error_fn(write_ptr, (png_voidp)inname, png_default_error,
       png_default_warning);
#endif
   png_debug(0, "Allocating read_info, write_info and end_info structures\n");
//   read_info_ptr = png_create_info_struct(read_ptr);
   write_info_ptr = png_create_info_struct(write_ptr);
//   end_info_ptr = png_create_info_struct(read_ptr);
#ifdef PNG_USER_MEM_SUPPORTED
#endif

   png_debug(0, "Setting jmpbuf for read struct\n");
#ifdef USE_FAR_KEYWORD
   if (setjmp(jmpbuf))
#else
//   if (setjmp(read_ptr->jmpbuf))
#endif
   {
//      fprintf(STDERR, "%s -> %s: libpng read error\n", inname, outname);
//      png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
      png_destroy_write_struct(&write_ptr, &write_info_ptr);
      fclose(fpout);
      return (1);
   }
#ifdef USE_FAR_KEYWORD
//   png_memcpy(read_ptr->jmpbuf,jmpbuf,sizeof(jmp_buf));
#endif

   png_debug(0, "Setting jmpbuf for write struct\n");
#ifdef USE_FAR_KEYWORD
   if (setjmp(jmpbuf))
#else
   if (setjmp(write_ptr->jmpbuf))
#endif
   {
//      fprintf(STDERR, "%s -> %s: libpng write error\n", inname, outname);
//      png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
      png_destroy_write_struct(&write_ptr, &write_info_ptr);
      fclose(fpout);
      return (1);
   }
#ifdef USE_FAR_KEYWORD
   png_memcpy(write_ptr->jmpbuf,jmpbuf,sizeof(jmp_buf));
#endif

   png_debug(0, "Initializing input and output streams\n");
#if !defined(PNG_NO_STDIO)
   png_init_io(write_ptr, fpout);
#else
   png_set_write_fn(write_ptr, (png_voidp)fpout,  png_default_write_data,
#if defined(PNG_WRITE_FLUSH_SUPPORTED)
      png_default_flush);
#else
      NULL);
#endif
#endif
  png_set_write_status_fn(write_ptr, NULL);
//  png_set_read_status_fn(read_ptr, NULL);


//   png_debug(0, "Reading info struct\n");
//   png_read_info(read_ptr, read_info_ptr);

//   png_debug(0, "Transferring info struct\n");
   {
      int interlace_type, compression_type, filter_type;

//      if (png_get_IHDR(read_ptr, read_info_ptr, &width, &height, &bit_depth,
//          &color_type, &interlace_type, &compression_type, &filter_type))
      {
			width = widthIn;
			height = heightIn;
			bit_depth = 8*nBytesIn;
			color_type = 
			compression_type = PNG_COMPRESSION_TYPE_BASE;
			filter_type = PNG_FILTER_TYPE_BASE;
			png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth,
				color_type, PNG_INTERLACE_NONE, compression_type, filter_type);
      }
   }


#ifdef SUPPORT_ADVANCED_PNG_OPTIONS

#if defined(PNG_READ_bKGD_SUPPORTED) && defined(PNG_WRITE_bKGD_SUPPORTED)
   {
      png_color_16p background;

      if (png_get_bKGD(read_ptr, read_info_ptr, &background))
      {
         png_set_bKGD(write_ptr, write_info_ptr, background);
      }
   }
#endif
#if defined(PNG_READ_cHRM_SUPPORTED) && defined(PNG_WRITE_cHRM_SUPPORTED)
   {
      double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;

      if (png_get_cHRM(read_ptr, read_info_ptr, &white_x, &white_y, &red_x,
         &red_y, &green_x, &green_y, &blue_x, &blue_y))
      {
         png_set_cHRM(write_ptr, write_info_ptr, white_x, white_y, red_x,
            red_y, green_x, green_y, blue_x, blue_y);
      }
   }
#endif
#if defined(PNG_READ_gAMA_SUPPORTED) && defined(PNG_WRITE_gAMA_SUPPORTED)
   {
      double gamma;

      if (png_get_gAMA(read_ptr, read_info_ptr, &gamma))
      {
         png_set_gAMA(write_ptr, write_info_ptr, gamma);
      }
   }
#endif
#if defined(PNG_READ_sRGB_SUPPORTED) && defined(PNG_WRITE_sRGB_SUPPORTED)
   {
      int intent;

      if (png_get_sRGB(read_ptr, read_info_ptr, &intent))
      {
         png_set_sRGB(write_ptr, write_info_ptr, intent);
      }
   }
#endif
#if defined(PNG_READ_hIST_SUPPORTED) && defined(PNG_WRITE_hIST_SUPPORTED)
   {
      png_uint_16p hist;

      if (png_get_hIST(read_ptr, read_info_ptr, &hist))
      {
         png_set_hIST(write_ptr, write_info_ptr, hist);
      }
   }
#endif
#if defined(PNG_READ_oFFs_SUPPORTED) && defined(PNG_WRITE_oFFs_SUPPORTED)
   {
      png_uint_32 offset_x, offset_y;
      int unit_type;

      if (png_get_oFFs(read_ptr, read_info_ptr,&offset_x,&offset_y,&unit_type))
      {
         png_set_oFFs(write_ptr, write_info_ptr, offset_x, offset_y, unit_type);
      }
   }
#endif
#if defined(PNG_READ_pCAL_SUPPORTED) && defined(PNG_WRITE_pCAL_SUPPORTED)
   {
      png_charp purpose, units;
      png_charpp params;
      png_int_32 X0, X1;
      int type, nparams;

      if (png_get_pCAL(read_ptr, read_info_ptr, &purpose, &X0, &X1, &type,
         &nparams, &units, &params))
      {
         png_set_pCAL(write_ptr, write_info_ptr, purpose, X0, X1, type,
            nparams, units, params);
      }
   }
#endif
#if defined(PNG_READ_pHYs_SUPPORTED) && defined(PNG_WRITE_pHYs_SUPPORTED)
   {
      png_uint_32 res_x, res_y;
      int unit_type;

      if (png_get_pHYs(read_ptr, read_info_ptr, &res_x, &res_y, &unit_type))
      {
         png_set_pHYs(write_ptr, write_info_ptr, res_x, res_y, unit_type);
      }
   }
#endif
   {
      png_colorp palette;
      int num_palette;

      if (png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette))
      {
         png_set_PLTE(write_ptr, write_info_ptr, palette, num_palette);
      }
   }
#if defined(PNG_READ_sBIT_SUPPORTED) && defined(PNG_WRITE_sBIT_SUPPORTED)
   {
      png_color_8p sig_bit;

      if (png_get_sBIT(read_ptr, read_info_ptr, &sig_bit))
      {
         png_set_sBIT(write_ptr, write_info_ptr, sig_bit);
      }
   }
#endif
#if (defined(PNG_READ_tEXt_SUPPORTED) && defined(PNG_WRITE_tEXt_SUPPORTED)) || \
    (defined(PNG_READ_zTXt_SUPPORTED) && defined(PNG_WRITE_zTXt_SUPPORTED))
   {
      png_textp text_ptr;
      int num_text;

      if (png_get_text(read_ptr, read_info_ptr, &text_ptr, &num_text) > 0)
      {
         png_debug1(0, "Handling %d tEXt/zTXt chunks\n", num_text);
         png_set_text(write_ptr, write_info_ptr, text_ptr, num_text);
      }
   }
#endif


#if defined(PNG_READ_tIME_SUPPORTED_NOT_) && defined(PNG_WRITE_tIME_SUPPORTED)
   {
      png_timep mod_time;

      if (png_get_tIME(read_ptr, read_info_ptr, &mod_time))
      {
         png_set_tIME(write_ptr, write_info_ptr, mod_time);
#if defined(PNG_TIME_RFC1123_SUPPORTED)
         /* we have to use png_strcpy instead of "=" because the string
            pointed to by png_convert_to_rfc1123() gets free'ed before
            we use it */
         png_strcpy(tIME_string,png_convert_to_rfc1123(read_ptr, mod_time));
         tIME_chunk_present++;
#endif /* PNG_TIME_RFC1123_SUPPORTED */
      }
   }
#endif


#if defined(PNG_READ_tRNS_SUPPORTED_NOT_) && defined(PNG_WRITE_tRNS_SUPPORTED)
   {
      png_bytep trans;
      int num_trans;
      png_color_16p trans_values;

      if (png_get_tRNS(read_ptr, read_info_ptr, &trans, &num_trans,
         &trans_values))
      {
         png_set_tRNS(write_ptr, write_info_ptr, trans, num_trans,
            trans_values);
      }
   }
#endif

#endif

   png_debug(0, "\nWriting info struct\n");
   png_write_info(write_ptr, write_info_ptr);

   png_debug(0, "\nAllocating row buffer \n");

//   row_buf = (png_bytep)png_malloc(read_ptr,
//      png_get_rowbytes(read_ptr, read_info_ptr));


   if (row_buf == NULL)
   {
//      fprintf(STDERR, "No memory to allocate row buffer\n");
//      png_destroy_read_struct(&read_ptr, &read_info_ptr, (png_infopp)NULL);
      png_destroy_write_struct(&write_ptr, &write_info_ptr);
      fclose(fpout);
      return (1);
   }
   png_debug(0, "Writing row data\n");

   num_pass=1;

#ifdef PNGTEST_TIMING
   t_stop = (float)clock();
   t_misc += (t_stop - t_start);
   t_start = t_stop;
#endif
   for (pass = 0; pass < num_pass; pass++)
   {
      png_debug1(0, "Writing row data for pass %d\n",pass);
      for (y = 0; y < height; y++)
      {
//         png_read_rows(read_ptr, (png_bytepp)&row_buf, (png_bytepp)NULL, 1);
#ifdef PNGTEST_TIMING
         t_stop = (float)clock();
         t_decode += (t_stop - t_start);
         t_start = t_stop;
#endif
         png_write_rows(write_ptr, (png_bytepp)&row_buf, 1);
#ifdef PNGTEST_TIMING
         t_stop = (float)clock();
         t_encode += (t_stop - t_start);
         t_start = t_stop;
#endif
      }
   }

   png_debug(0, "Reading and writing end_info data\n");
//   png_read_end(read_ptr, end_info_ptr);
//   png_write_end(write_ptr, end_info_ptr);

#ifdef PNG_EASY_ACCESS_SUPPORTED
   if(bVerbose)
   {
      png_uint_32 iwidth, iheight;
      iwidth = png_get_image_width(write_ptr, write_info_ptr);
      iheight = png_get_image_height(write_ptr, write_info_ptr);
//      fprintf(STDERR, "Image width = %lu, height = %lu\n",
//         iwidth, iheight);
   }
#endif

   png_debug(0, "Destroying data structs\n");
//   png_free(read_ptr, row_buf);
//   png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
   png_destroy_write_struct(&write_ptr, &write_info_ptr);

   fclose(fpout);

   return (0);
}


#endif


























