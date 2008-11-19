/********************************************************************************
 * Flux
 *
 * File: anmdecrypt.h
 * Description: Decryption module
 *
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

#ifndef _anmdecrypt_h
#define _anmdecrypt_h


#define MAXBUFLEN 2048

// N.B.: length
class CAnmDecrypt
{
private :

public :

	// returns TRUE if file either has required decryption
	static bool TestForEncrypt(const char *filename);
	// returns OK if decryption infilename successful; outfilename is where the output goes
	static bool Decrypt(const char *infilename, char *outfilename, int maxbuf);
};

#endif // _anmdecrypt_h
