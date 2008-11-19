/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			ImageIO.h
 *		Description:	Header file for Texture Reading code.
 *		Author:			Keith Victor
 *          Supports    Png, Jpg, and Bmp
 *						
 *
 * (C) 2003 by Media Machines
 *
 *		Revision history:
 *			$Log: ImageIO.h,v $
 *			Revision 1.1  2004/06/21 06:47:59  Tony
 *			Replaced Image Library with IJG
 *			
 *			Revision 1.1.1.1  2003/09/10 02:18:11  turk
 *			build466
 *			
 *
 *******************************************************************/





unsigned char* read_jpg_IJG(const char * filename, int* pwidth, int* pheight );

unsigned char* read_png_png(const char *file_name, int* pwidth, int* pheight, int* pnBytes );

unsigned char* read_gif(const char *file_name, int* pwidth, int* pheight, int* pnBytes );

unsigned char* read_bmp(const char *file_name, int* pwidth, int* pheight );
