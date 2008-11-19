/********************************************************************************
 * Flux
 *
 * File: anmgunzip.h
 * Description: Gzip decompresion class
 *						Invocation and use of gzip is Windows-specific
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

#ifndef _anmgunzip_h
#define _anmgunzip_h

#define ANMGZIPMAGIC 0x8B1F		// got this from sample gzip header; where is it really spec'ed?
#define ANMGZIPEXE "gzip.exe"
#define ANMGZIPFLAGS "-dqf"


#define MAXBUFLEN 2048

// N.B.: length
class CAnmGunzip
{
private :

	static char m_gzipexebuf[ANMMAXSTRBUF];
	static char *m_gzipexe;

	// finds path to the GZIP exe to spawn
	static void FindGzipExecutable();

public :

	// returns TRUE if file either has .gz extension or contains gzip magic no
	static BOOL TestForGzip(const char *filename);
	// returns OK if gzipping infilename successful; outfilename is where the output goes
	static eAnmReturnStatus Gunzip(const char *infilename, char *outfilename, int maxbuf);

};

#endif // _anmgunzip_h
