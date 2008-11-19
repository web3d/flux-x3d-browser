// giftobmp->cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "gifio.h"



/**
* Fetch the next code from the raster data stream.  The codes can be
* any length from 3 to 12 bits, packed into 8-bit bytes, so we have to
* maintain our location in the raster array as a BIT Offset.  We compute
* the byte Offset into the raster array by dividing this by 8, pick up
* three bytes, compute the bit Offset into our 24-bit chunk, shift to
* bring the desired code to the bottom, then mask it off and return it. 
*/
static int32 BmpClass_ReadCode (byte * raster, int32 rasterStart, int32 rasterLength, int32 codeSize, int32 readMask, int32 *bitOffset)
{
	int32 rawCode;
	int32 byteOffset;
    byteOffset = (*bitOffset) >>3;
    // v2.2   
    // Alan Dix modification to fix raster over-run errors
    // int rawCode =   (raster [byteOffset] & 0xFF)
    //              + ((raster [byteOffset + 1] & 0xFF) << 8);
    rawCode =   (raster [byteOffset+rasterStart] & 0xFF);
        
    if (byteOffset + 1 < rasterLength){
        rawCode += ((raster [byteOffset+rasterStart + 1] & 0xFF) << 8);
    }
    if (codeSize >= 8 && byteOffset + 2 < rasterLength){
        rawCode += (raster [byteOffset+rasterStart + 2] & 0xFF) << 16;
    }
    rawCode >>= ((*bitOffset) % 8);
    (*bitOffset) += codeSize;
        
    return rawCode & readMask;
}

static int32 BmpClass_LoadGIF ( const char * filename, BmpClass *bmp )
{

	bmp->width = 0;
	bmp->height = 0;
	bmp->NumColors = 0;
	bmp->bmpData = NULL;

	bmp->bGotTransparency = false;
	bmp->iTranparentIndex = 0;

	FILE *pIFile;

	boolean gif89;
	int32 gifptr = 0;
	boolean hasColormap;
	int32 bitsPerPixel;
	int32 colorMapSize;
	int32 bitMask;
	int32 localPaletteSize = 0;
	int32 paletteSize = 0;

	int32 palettePos = -1;
	int32 block;
	int32 blockSize;

    int32  npixels ;
    int32  maxpixels ;
	int32 misc;

    int32 codeSize ;
    int32 clearCode ; // GIF clear code
    int32 EOFCode   ;   // GIF end-of-information code
    int32 firstFree ;   // First free code, generated per GIF spec
    int32 freeCode ;       // Decompressor,next free slot in hash table
    int32 initCodeSize ;     // Starting code size, used during Clear
    int32 maxCode ;   // limiting value for current code size
    int32 readMask;      // Code AND mask for current code size

	byte * srcData = NULL;
	int32 dataPtr;
	int32 fileSize = 0;
    int32 rasterStart;
    int32 rasterPos;
    int32 rasterLength;
    // The hash table used by the decompressor 
    int32 prefix [4096];
    int32 suffix [4096];
    // An output array used by the decompressor 
    int32 outCode [4097];

    int32 outCount;   // Decompressor output 'stack count'
    int32 currentCode;    // Decompressor variables
    int32 oldCode ;
    int32 inCode;
    int32 finChar ;    
    int32 x ;
    int32 xbmp ;
    int32 y;
    int32 code ;
	int32 i;
	int32 n;
	int32 numBits;
    
	int32 numColors   ;
	int32 numBitsExport ;
	int32 numColorsExport ;

	int32 padding;
	int32 scanLength;
 	int32 imageSize  ;
	
	int32 bfOffBits  ;
	int32 palin ;
	int32 ptr ;

	int32 bitOffset = 0;  // Bit Offset of next code


	if ((pIFile = fopen(filename, "rb")) == NULL){		
		return FALSE;
	}
	//if (fseek(pIFile, sizeof(byte), SEEK_END)!=0){
	if (fseek(pIFile, 0L, 2)!=0){
		fclose(pIFile);
		return FALSE;
	}
	fileSize = ftell(pIFile);

	//printf("fileSize %d\n",fileSize);

	fseek(pIFile, 0, 0);
	ftell(pIFile);

	//srcData = (byte*)calloc((size_t)fileSize, (size_t)1);//malloc(sizeof(byte)*fileSize);
	//for (i=0;i<fileSize;i++){
	//	srcData[i] = (byte)0;
	//}
//	srcData = (byte*)malloc(sizeof(byte)*fileSize);//malloc(sizeof(byte)*fileSize);

	srcData = new byte[fileSize];
	for (i=0;i<fileSize;i++){
		srcData[i] = (byte)0;
	}


	fread(srcData, (size_t)fileSize, (size_t)1, pIFile);

	dataPtr = 0;

    // check file type
    if (!(srcData[0]=='G' && srcData[1]=='I' && srcData[2]=='F' && srcData[3]=='8')){
		//printf("Not a GIF file!\n");
		fclose(pIFile);
		delete [] srcData;
		return FALSE;
    }
    gif89 = srcData[4]=='9';

	// the 'a' - as in GIF89a GIF87a
    //dataInput.readByte()
    
    // Get variables from the GIF screen descriptor 
    // Some stuff is ignored...

    // Screen width - ignored
    //dataInput.readByte ();dataInput.readByte ();
    // Screen height - ignored
    //dataInput.readByte ();dataInput.readByte ();
    
    //dataInput.skipBytes(5);
	//gifptr+=5;
 
    //byte aByte = dataInput.readByte ();
    hasColormap = (srcData[10] & 0xffffff80)!=0;//((byte)0x80))!=0;    // 0x80 == COLORMAPMASK;
    // Bits per pixel, read from GIF header
    bitsPerPixel = (srcData[10] & 7) + 1;
    // Number of colors
    colorMapSize = 1 << bitsPerPixel;
    // AND mask for data size
    bitMask = colorMapSize - 1;

	unsigned char tranparent = srcData[6];
	unsigned char bgColorBit = srcData[11];
    // Ignore background color...
    //dataInput.readByte(); 

    if ((srcData[12] & 0xFF) != 0 && !gif89){ // do not aspect ratio
		fclose(pIFile);
		delete [] srcData;
		return FALSE;
	}
	
	dataPtr+=13;

    // Read in global colormap. 
    if (hasColormap){
		paletteSize = colorMapSize;
		palettePos = dataPtr;
		dataPtr += paletteSize + (paletteSize<<1);
    }





    for (;;)
	{
		block = srcData[dataPtr];dataPtr++;
		if (block == 0x3b) //TRAILER
			break;

		if (block == 0x21) //EXTENSION)
        { 
            // possible things at this point are:
            //   an application extension block
            //   a comment extension block
            //   an (optional) graphic control extension block
            //       followed by either an image
            //     or a plaintext extension
            
            // ignore all extension blocks
            //dataInput.readByte();




/****
Offset   Length   Contents
  0      1 byte   Extension Introducer (0x21)
  1      1 byte   Graphic Control Label (0xf9)
  2      1 byte   Block Size (0x04)
  3      1 byte   bit 0..2: Reserved
                  bit 3..5: Disposal Method
                  bit 6:    User Input Flag
                  bit 7:    Transparent Color Flag
  4      2 bytes  Delay Time (1/100ths of a second)
  6      1 byte   Transparent Color Index
  7      1 byte   Block Terminator(0x00)

****/


			if( srcData[dataPtr] == 0xf9 ) {
				// Found Graphic Control Extension Block
				// Look for transparency!
				bmp->bGotTransparency = srcData[dataPtr+2] & 0x01;
				bmp->iTranparentIndex = srcData[dataPtr+5];
			}



			dataPtr++;
            for (;;){
				blockSize = srcData[dataPtr++] & 0xFF;
				if (blockSize == 0) 
					break;
				dataPtr += blockSize;
			}

        }
        else if (block == 0x2c)//IMAGESEP)
        {
            // takes the first image and ignores any others...
             npixels = 0;
             maxpixels = 0;

            // left offset - ignored
            //dataInput.readByte ();dataInput.readByte ();
            // top offset - ignored
            //dataInput.readByte ();dataInput.readByte ();
            
			 dataPtr+=4;

			bmp->width	= (srcData[dataPtr] & 0xFF) + ((srcData[dataPtr+1] & 0xFF)<<8); dataPtr+=2;
			bmp->height	= (srcData[dataPtr] & 0xFF) + ((srcData[dataPtr+1] & 0xFF)<<8); dataPtr+=2;

            misc = srcData[dataPtr++] & 0xFF;// miscellaneous bits (interlace, local cmap)

            if ((misc & 0x40)>0){    // INTERLACEMASK
    			fclose(pIFile);
				delete [] srcData;
				return FALSE;       // do not support interlaced gifs
			}
            // local colormap
            if ((misc & 0x80)>0){
				localPaletteSize = (1 << ((misc & 7) + 1));
				if (localPaletteSize>0){
					paletteSize = localPaletteSize;
					palettePos = dataPtr;					
					dataPtr+=localPaletteSize*3;
				}				
            }
     
            // Start reading the raster data. First we get the intial code size
            // and compute decompressor constant values, based on this code size.

            // Code size, read from GIF header 
			//fread((void *)aByte, sizeof(byte), 1, pIFile);
            codeSize = srcData[dataPtr++] & 0xFF;
            clearCode = (1 << codeSize); // GIF clear code
            EOFCode   = clearCode + 1;   // GIF end-of-information code
            firstFree = clearCode + 2;   // First free code, generated per GIF spec
            freeCode  = firstFree;       // Decompressor,next free slot in hash table

			//printf("codeSize %d\n",codeSize);
            // The GIF spec has it that the code size is the code size used to 
            // compute the above values is the code size given in the file, but the
            // code size used in compression/decompression is the code size given in
            // the file plus one. (thus the ++).
            codeSize++;
            initCodeSize = codeSize;     // Starting code size, used during Clear
            maxCode = (1 << codeSize);   // limiting value for current code size
            readMask = maxCode - 1;      // Code AND mask for current code size

            // Read the raster data. Turn it from a series of blocks into one long
            // data stream, which makes life much easier for readCode ().
			// Optimized to just store the one long stream back in the original src array
			// to keep memory usage down....

            rasterStart = dataPtr;
            rasterPos = rasterStart;
            rasterLength = 0;
            for (;;)
            {   
				blockSize = srcData[dataPtr++] & 0xFF;
				if (blockSize==0)
					break;
                while (blockSize-- > 0){
                    srcData[rasterPos++] = srcData[dataPtr++];
					rasterLength++;
                }
            }

            // Allocate the 'pixels' 
            // actually creates enough space for the bitmap structure
            maxpixels = bmp->width * bmp->height;
  
            //{{ BMP Stuff
	        n = paletteSize;
	        numBits = 1;
	        if (n-- == 0)
	            numBits = 0;
	        else
	            while ((n >>= 1) != 0)
	                ++numBits;
        	
	        numColors       = 1 << numBits;
	        numBitsExport   = numColors<3? 1 : (numColors<17? 4 : 8);
	        numColorsExport = 1 << numBitsExport;
        	//printf("numColors %d\n", numColors);
        	//printf("numBitsExport %d\n", numBitsExport);
        	//printf("numColorsExport %d\n", numColorsExport);

	        // 1 bit
	        if (numBitsExport==1){
	            padding     = (4 - (((bmp->width + 7)>>3) % 4)) % 4;
	            scanLength  = (((bmp->width + 7)>>3) + padding);
				bmp->PixelsPerByte = 8;
	        }
	        // 4 bit
	        else if (numBitsExport==4){
	            padding     = (4 - (((bmp->width + 1)>>1) % 4)) % 4;
	            scanLength  = (((bmp->width + 1)>>1) + padding);
				bmp->PixelsPerByte = 2;
	        }
	        // 8 bit
	        else {
	            padding     = (4 - (bmp->width % 4)) % 4;
	            scanLength  = (bmp->width + padding);
				bmp->PixelsPerByte = 1;
	        }
	        imageSize   = scanLength * bmp->height;
			bmp->scanLength = scanLength;
			bmp->padding = padding;


	        bfOffBits   = 54 + (numColorsExport<<2);

			//printf("bfOffBits %d\n", bfOffBits);
			//printf("scanLength %d\n", scanLength);

	        bmp->bfSize      = imageSize + bfOffBits;
			//printf("bmp->bfSize %d\n", bmp->bfSize);
        	//bmp->bmpData = (byte*)calloc(bmp->bfSize, sizeof(byte));//(byte*)malloc(sizeof(byte)*bmp->bfSize);//new byte[bfSize];
//			bmp->bmpData = (byte*)malloc(sizeof(byte)*bmp->bfSize);
			bmp->bmpData = new byte [bmp->bfSize];
			
			for (i=0;i<bmp->bfSize;i++)
				bmp->bmpData[i] = (byte)0;

			//for (i=0;i<bmp->bfSize;i++)
			//	bmp->bmpData[i] = (byte)0;
	        // bfType
	        bmp->bmpData[0] = (byte)66; bmp->bmpData[1] = (byte)77;   
        	
	        // bfSize
	        bmp->bmpData[2]=(byte)(bmp->bfSize&0xff); 
	        bmp->bmpData[3]=(byte)((bmp->bfSize>>8)&0xff); 
	        bmp->bmpData[4]=(byte)((bmp->bfSize>>16)&0xff); 
	        bmp->bmpData[5]=(byte)((bmp->bfSize>>24)&0xff);
        	
	        // bfOffBits
	        bmp->bmpData[10]=(byte)(bfOffBits&0xff); 
	        bmp->bmpData[11]=(byte)((bfOffBits>>8)&0xff); 
	        bmp->bmpData[12]=(byte)((bfOffBits>>16)&0xff); 
	        bmp->bmpData[13]=(byte)((bfOffBits>>24)&0xff);
        	
	        // biSize
	        bmp->bmpData[14]=(byte)(40);
        	
	        // biWidth
	        bmp->bmpData[18]=(byte)(bmp->width&0xff); 
	        bmp->bmpData[19]=(byte)((bmp->width>>8)&0xff); 
	        bmp->bmpData[20]=(byte)((bmp->width>>16)&0xff); 
	        bmp->bmpData[21]=(byte)((bmp->width>>24)&0xff);
        	
	        // biHeight
	        bmp->bmpData[22]=(byte)(bmp->height&0xff); 
	        bmp->bmpData[23]=(byte)((bmp->height>>8)&0xff); 
	        bmp->bmpData[24]=(byte)((bmp->height>>16)&0xff); 
	        bmp->bmpData[25]=(byte)((bmp->height>>24)&0xff);
        	
	        // biPlanes
	        bmp->bmpData[26]=(byte)1;
        	
	        // biBitCount
	        bmp->bmpData[28]=(byte)numBitsExport;
        	
	        // biSizeImage
	        bmp->bmpData[34]=(byte)(imageSize&0xff); 
	        bmp->bmpData[35]=(byte)((imageSize>>8)&0xff); 
	        bmp->bmpData[36]=(byte)((imageSize>>16)&0xff); 
	        bmp->bmpData[37]=(byte)((imageSize>>24)&0xff);
        	
	        // biXPelsPerMeter (72 dpi)
	        bmp->bmpData[38]=(byte)numBitsExport;
        	
	        // biYPelsPerMeter (72 dpi)
	        bmp->bmpData[42]=(byte)numBitsExport;
         
	        // biClrUsed
	        bmp->bmpData[46]=(byte)numColors;
        	
	        // biClrImportant
	        bmp->bmpData[50]=(byte)numColors;
        	
	        // Write palette.
	        palin = 0;
	        ptr = 54;

			bmp->Palette = &bmp->bmpData[54];
			bmp->NumColors = numColors;
			bmp->PixelData = &bmp->bmpData[bfOffBits];

	        for (i=0;i<numColorsExport;i++){
		        bmp->bmpData[ptr++] = i<numColors ? (srcData[palettePos+palin+2]) : 0;    // B
		        bmp->bmpData[ptr++] = i<numColors ? (srcData[palettePos+palin+1]) : 0;    // G
		        bmp->bmpData[ptr++] = i<numColors ? (srcData[palettePos+palin]) : 0;      // R
		        bmp->bmpData[ptr++] = 0;
		        palin+=3;
	        }
            //}} EndOf BMP Stuff
 			//printf("imageSize %d\n", imageSize);
        	//printf("padding %d\n", padding);

            outCount = 0;   // Decompressor output 'stack count'
            oldCode = 0;
            finChar = 0;
            
            x = 0;
            xbmp = 0;
            y = (bmp->height-1)*scanLength;
            
            // Decompress the file, continuing until you see the GIF EOF code.
            // One obvious enhancement is to add checking for corrupt files here.
            code = BmpClass_ReadCode (srcData, rasterStart, rasterLength, codeSize, readMask, &bitOffset);
            while (code != EOFCode) 
            {
				// Clear code sets everything back to its initial value, then reads the
                // immediately subsequent code as uncompressed data.
                if (code == clearCode)
                {
                    codeSize = initCodeSize;
                    maxCode = (1 << codeSize);
                    readMask = maxCode - 1;
                    freeCode = firstFree;
                    code = BmpClass_ReadCode (srcData, rasterStart, rasterLength, codeSize, readMask, &bitOffset);
                    currentCode = oldCode = code;
                    finChar = currentCode & bitMask;

                    //{{ BMP Stuff
	                if (numBitsExport==1){          // 1 bit
	                    // 8 pixels per byte.
		                bmp->bmpData[y + xbmp + bfOffBits] |= ((byte)finChar) << (7 - (x % 8));
		                if (((x % 8) == 7) || (x == bmp->width - 1))
		                    xbmp++;
		            }
		            else if (numBitsExport==4){     // 4 bit
	                    // 2 pixels per byte.
		                bmp->bmpData[y + xbmp + bfOffBits] |= ((byte)finChar) << (x%2==0?4:0);
		                if (((x % 2) == 1) || (x == bmp->width - 1))
		                    xbmp++;
		            }
		            else {                          // 8 bit
	                    // 1 pixel per byte.
		                bmp->bmpData[y + xbmp + bfOffBits] |= ((byte)finChar);
		                xbmp++;
		            }
		            x++;
		            if (x == bmp->width){
		                y-=scanLength;
		                x = 0;
		                xbmp = 0;
		            }
                    //}} EndOf BMP Stuff
                    
                    npixels++;
                }
                else 
                {
                    // If not a clear code, must be data: save same as currentCode and inCode 

                    // if we're at maxcode and didn't get a clear, stop loading 
                    if (freeCode >= 4096) 
                        break;

                    currentCode = inCode = code;
                  
                    // If greater or equal to freeCode, not in the hash table yet;
                    // repeat the last character decoded
                    if (currentCode >= freeCode) 
                    {
                        currentCode = oldCode;
                        if (outCount > 4096) 
                            break;
                        outCode [outCount++] = finChar;
                    }
      
                    // Unless this code is raw data, pursue the chain pointed to by currentCode
                    // through the hash table to its end; each code in the chain puts its
                    // associated output code on the output queue.
                    while (currentCode > bitMask) 
                    {
                        if (outCount > 4096)
                            break;   // corrupt file 
                            
                        outCode [outCount++] = suffix [currentCode];
                        currentCode = prefix[currentCode];
                    }
                    if (outCount > 4096) 
                        break;
      
                    // The last code in the chain is treated as raw data. 
                    finChar = currentCode & bitMask;
                    outCode [outCount++] = finChar;
                  
                    // Now we put the data out to the Output routine.
                    // It's been stacked LIFO, so deal with it that way...

                    // safety thing:  prevent exceeding range of 'bytePixels' 
                    if (npixels + outCount > maxpixels) 
                        outCount = maxpixels - npixels;
  
                    npixels += outCount;

                    for (i = outCount - 1; i >= 0; i--){
						
                        //{{ BMP Stuff
	                    if (numBitsExport==1){          // 1 bit
	                        // 8 pixels per byte.
		                    bmp->bmpData[y + xbmp + bfOffBits] |= ((byte)outCode[i]) << (7 - (x % 8));
		                    if (((x % 8) == 7) || (x == bmp->width - 1))
		                        xbmp++;
		                }
		                else if (numBitsExport==4){     // 4 bit
	                        // 2 pixels per byte.
		                    bmp->bmpData[y + xbmp + bfOffBits] |= ((byte)outCode[i]) << (x%2==0?4:0);
		                    if (((x % 2) == 1) || (x == bmp->width - 1))
		                        xbmp++;
		                }
		                else {                          // 8 bit
	                        // 1 pixel per byte.
		                    bmp->bmpData[y + xbmp + bfOffBits] |= ((byte)outCode[i]);
		                    xbmp++;
		                }
		                x++;
		                if (x == bmp->width){
		                    y-=scanLength;
		                    x = 0;
		                    xbmp = 0;
		                }
                        //}} EndOf BMP Stuff
                    }
                    outCount = 0;

                    // Build the hash table on-the-fly. No table is stored in the file.       
                    prefix [freeCode] = oldCode;
                    suffix [freeCode] = finChar;
                    oldCode = inCode;
      
                    // Point to the next slot in the table.  If we exceed the current
                    // maxCode value, increment the code size unless it's already 12.  If it
                    // is, do nothing: the next code decompressed better be CLEAR
                  
                    freeCode++;
                    if (freeCode >= maxCode && codeSize < 12)
                    {
                        codeSize++;
                        maxCode <<= 1;
                        readMask = (1 << codeSize) - 1;
                    }
                }
  
				code = BmpClass_ReadCode (srcData, rasterStart, rasterLength, codeSize, readMask, &bitOffset);
				if (npixels >= maxpixels) 
					break;
					
			}
			fclose(pIFile);
			delete [] srcData;
			return TRUE;
		}
		else 
		{      
			// unknown block type 
			// don't mention bad block if file was trunc'd, as it's all bogus 
			//fclose(pIFile);
			fclose(pIFile);
			delete [] srcData;
			return FALSE;
		}

    }
	//fclose(pIFile);

	delete [] srcData;
	return 0;
}




unsigned char* read_gif(const char *file_name, int* pwidth, int* pheight, int* pnBytes )
{
	unsigned char* pDataOut = NULL;

	if( file_name ) {
		BmpClass bitmapData;

		if( BmpClass_LoadGIF (file_name, &bitmapData) ) {

			unsigned int c1, c2, c3;
			unsigned int icolor;
			

//			bitmapData.bGotTransparency = srcData[dataPtr+2] & 0x01;
//			bitmapData.iTranparentIndex = srcData[dataPtr+5];

			int nBytes = ( bitmapData.bGotTransparency ) ? 4 : 3;
			bool bPixelIsTran = false;

			*pwidth = bitmapData.width;
			*pheight = bitmapData.height;
			*pnBytes = nBytes;

			int size = bitmapData.width*bitmapData.height;
			pDataOut = new unsigned char[nBytes * size];
			unsigned char* pData = pDataOut;
			unsigned int PixelsPerByte = bitmapData.PixelsPerByte;
			unsigned int scanLength = bitmapData.scanLength;

			for( int iy=0; iy<*pheight; iy++  ) {
				for( int ix=0; ix<*pwidth; ix++  ) {


					icolor = bitmapData.PixelData[( ix / PixelsPerByte + iy*scanLength ) ];
					if( PixelsPerByte == 2 ) {
						if( ix%2 == 0 ) {
							icolor = ( icolor >> 4 ) & 0x0F;
						}
						else {
							icolor = ( icolor ) & 0x0F;
						}
					}
					else if( PixelsPerByte == 8 ) {
						icolor = ( icolor & ( 1 << ( ix%8 ) ) ) ? 1 : 0;
					}

					bPixelIsTran = false;
					if( bitmapData.bGotTransparency ) {
						if( icolor == bitmapData.iTranparentIndex ) {
							bPixelIsTran = true;
							pData[2] = 0;
							pData[1] = 0;
							pData[0] = 0;
							pData[3] = 0;
						}
						else {
							pData[3] = 255;
						}
					}
					if( !bPixelIsTran &&
								icolor >=0 && 
								icolor < bitmapData.NumColors ) {
						uint8* pPaletteTmp = &bitmapData.Palette[icolor*4];
						pData[2] = pPaletteTmp[0]; // B
						pData[1] = pPaletteTmp[1]; // G
						pData[0] = pPaletteTmp[2]; // R
					}
					pData+=nBytes;
				}
			}
			delete [] bitmapData.bmpData;
		}
	}
	return pDataOut;
}


/************* usage
void readgif( char* name ) {
	BmpClass *bmp = (BmpClass*)malloc(sizeof(BmpClass));
	int loaded = BmpClass_LoadGIF (name, bmp);
	int jjj=0;
	
	unsigned int c1, c2, c3;

	unsigned int theColor = 0;

	FILE* fp = fopen( "texture.txt", "w" );
	for( int icolor=0; icolor<bmp->NumColors; icolor++ ) {
		c1 = bmp->Palette[icolor*4];   // B
		c2 = bmp->Palette[icolor*4+1]; // G
		c3 = bmp->Palette[icolor*4+2]; // R
		theColor = c1 + ( c2<<8 ) + ( c3<<16 );
		fprintf( fp, "Palet %d  %d\n", icolor, theColor );

	}

	int size = bmp->width*bmp->height;
	for( int ipixel=0; ipixel<size; ipixel++ ) {
		c1 = bmp->bmpData[ipixel];
		fprintf( fp, "Pixel %d  %d\n", ipixel, c1 );

	}

	fclose( fp );
//	printf("loaded %d %d %d\n", loaded, bmp->width, bmp->height);

}
**************************/

