#ifndef GIFIO_H
#define GIFIO_H





#ifndef boolean
typedef  unsigned char      boolean;     /* Boolean value type. */
#endif

typedef  unsigned long int  uint32;      /* Unsigned 32 bit value */
typedef  unsigned short     uint16;      /* Unsigned 16 bit value */
typedef  unsigned char      uint8;       /* Unsigned 8  bit value */

typedef  signed long int    int32;       /* Signed 32 bit value */
typedef  signed short       int16;       /* Signed 16 bit value */
typedef  signed char        int8;        /* Signed 8  bit value */

/* This group are the deprecated types.  Their use should be
** discontinued and new code should use the types above
*/
typedef  unsigned char     byte;         /* Unsigned 8  bit value type. */
typedef  unsigned short    word;         /* Unsinged 16 bit value type. */
typedef  unsigned long     dword;        /* Unsigned 32 bit value type. */

typedef  unsigned char     uint1;        /* Unsigned 8  bit value type. */
typedef  unsigned short    uint2;        /* Unsigned 16 bit value type. */
typedef  unsigned long     uint4;        /* Unsigned 32 bit value type. */

typedef  signed char       int1;         /* Signed 8  bit value type. */
typedef  signed short      int2;         /* Signed 16 bit value type. */
typedef  long int          int4;         /* Signed 32 bit value type. */

typedef  signed long       sint31;       /* Signed 32 bit value */
typedef  signed short      sint15;       /* Signed 16 bit value */
typedef  signed char       sint7;        /* Signed 8  bit value */

  typedef  __int64            int64;       /* Signed 64 bit value */
  typedef  unsigned __int64   uint64;      /* Unsigned 64 bit value */


//typedef struct _BmpClass BmpClass;
typedef struct _BmpClass 
{
    int32			bfSize;            
    int32			width;            
    int32			height;
    uint8 *			bmpData;  // image stored in bitmap format - header / palette / data


	uint8*          Palette;
	int             NumColors;
	uint8*          PixelData;


	int32			scanLength;
	int32			padding;
	int32			PixelsPerByte;


	bool  bGotTransparency;
	uint8 iTranparentIndex;


} BmpClass;




#endif GIFIO_H