/********************************************************************************
 * Flux
 *
 * File: anmgunzip.cpp
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

#include "stdafx.h"
#include "anmgunzip.h"
#include "anmmessage.h"
#include <io.h>


// if the zlib_dll is defined, we will do our gunzipping internally, without the external exe.
// if it is not set, we use the external exe.
#ifdef ZLIB_DLL

#include "zlib.h" 
#define ANM_DO_GZIP_INTERNAL

#endif


char *CAnmGunzip::m_gzipexe = NULL;
char CAnmGunzip::m_gzipexebuf[ANMMAXSTRBUF];



#ifdef ANM_DO_GZIP_INTERNAL

static int DoGunZipInternal( const char* filein, const char* fileout )
{

	int status = FALSE;


	gzFile in  = gzopen(filein,"rb");
	FILE * out = fopen(fileout,"wb");

	if( in && out ) {
		status = TRUE;
	   char buf[16384];	// buffer per gzip
	   int len = 0;
	   while(1)
	   {
		   len = gzread( in, buf,sizeof(buf));
		   if (len == 0) break;
		   fwrite( buf, (unsigned)len, 1, out );
	   }
	}
	if( in ) {
	   gzclose(in);
	}
	if( out ) {
	   fclose(out);
	}

	return status;
}



// returns OK if infilename successful; outfilename is where the output goes
static eAnmReturnStatus GunZipInternal(const char *infilename, char *outfilename, int maxbuf)
{
	// create a tmp filename
	//
	int len = strlen( infilename );
	if( len < 1 || len > maxbuf-5 ) {
		return eAnmReturnAllBad;
	}

	strcpy( outfilename, infilename );
	char *ext = strrchr(outfilename, '.');
	if (!ext )
	{
		strcat(outfilename, ".gunz");
	}
	else
	{
		const char* extInfile = infilename + ( ext - outfilename );
		strcpy( ext, ".gunzip" );
		strcat( outfilename, extInfile );
	}


	// Just do it:
	bool success = ( DoGunZipInternal( infilename, outfilename ) );
	return success ? eAnmReturnAllGood : eAnmReturnAllBad;
}



#endif  // ANM_DO_GZIP_INTERNAL



#ifndef ANM_DO_GZIP_INTERNAL

// finds path to the GZIP exe to spawn
void CAnmGunzip::FindGzipExecutable()
{
	GetModuleFileName(AfxGetInstanceHandle(), m_gzipexebuf, ANMMAXSTRBUF - strlen(ANMGZIPEXE) - 2);

	char *dir = strrchr(m_gzipexebuf, '\\');

	if (dir == NULL)
		return;

	strcpy(dir + 1, ANMGZIPEXE);

	if (_access(m_gzipexebuf, 0) == -1)
		// ANMMSG_COULDNTFINDGZIP : "Couldn't find gzip executable"
		throw CAnmError(ANMMSG_COULDNTFINDGZIP);

	m_gzipexe = &m_gzipexebuf[0];
}

#endif  // NOT ANM_DO_GZIP_INTERNAL



// returns TRUE if file either has .gz extension or contains gzip magic number
BOOL CAnmGunzip::TestForGzip(const char *filename)
{
	FILE *fp = NULL;
	
	if(filename == NULL)
		return FALSE;

	// try extension first; it's cheaper
	const char *ext = strrchr(filename, '.');

	if (!strcmp(ext, ".gz"))
		return TRUE;

	// otherwise, sniff at the first 2 bytes for GZIP magic number
	BOOL retval = TRUE;
	unsigned short magic;

	fp = fopen(filename, "rb");
	if(fp == NULL)
		return FALSE;

	if (fread(&magic, sizeof(unsigned short), 1, fp) < 1)
		retval = FALSE;
	else if (magic != ANMGZIPMAGIC)
		retval = FALSE;

	fclose(fp);

	return retval;
}

// returns OK if gzipping infilename successful; outfilename is where the output goes
eAnmReturnStatus CAnmGunzip::Gunzip(const char *infilename, char *outfilename, int maxbuf)
{

#ifdef ANM_DO_GZIP_INTERNAL

	return GunZipInternal( infilename, outfilename, maxbuf );

#else

	if (m_gzipexe == NULL)
	{
		FindGzipExecutable();
		if (m_gzipexe == NULL)
			return eAnmReturnAllBad;
	}

	// first up: copy the input file to the windows temp directory
	char tmpnamebuf[ANMMAXSTRBUF];
	if (GetTempPath(ANMMAXSTRBUF - 1, tmpnamebuf) > ANMMAXSTRBUF - 1)
	{
		// what to do?
		assert(FALSE);
	}

	// convert backslashes to forward
	char nobackslashes[ANMMAXSTRBUF];
	for (int i = 0; i < strlen(infilename) + 1; i++)
		if (infilename[i] == '\\')
			nobackslashes[i] = '/';
		else
			nobackslashes[i] = infilename[i];

	char *filename = strrchr(nobackslashes, '/');
	if (filename == NULL)
		filename = nobackslashes;
	else
		filename++;

	strcat(tmpnamebuf, filename);

	// copy the file
	// N.B.: eventually we should unlink these temp files...
	CopyFile(infilename, tmpnamebuf, FALSE);

	infilename = tmpnamebuf;

	char tmpinfilename[ANMMAXSTRBUF];
	char commandLine[ANMMAXSTRBUF * 2];

	// N.B.: need to check string lengths against maxbuf
	BOOL renamed = FALSE;
	strcpy(tmpinfilename, infilename);
	char *ext = strrchr(infilename, '.');
	if (!ext || strcmp(ext, ".gz"))
	{
		// if it doesn't have .gz extension, append one and rename; outfile is original file name
		strcat(tmpinfilename, ".gz");
		rename(infilename, tmpinfilename);

		strcpy(outfilename, infilename);

		renamed = TRUE;
	}
	else
	{
		// if it does have .gz extension, outfile is original file name minus .gz
		// because gzip strips the .gz extension
		strcpy(outfilename, infilename);
		outfilename[strlen(infilename) - 3] = '\0';
	}

	// quote the file name to account for spaces
	sprintf(commandLine, "%s %s \"%s\"", m_gzipexe, ANMGZIPFLAGS, tmpinfilename);

	// Spawn a gzip process
	BOOL stat;
	STARTUPINFO			startinfo;
	PROCESS_INFORMATION	procinfo;
	BOOL success = TRUE;

	memset((void *)&startinfo, 0, sizeof(STARTUPINFO));

	startinfo.cb				= sizeof(STARTUPINFO);
	startinfo.dwFlags 			= STARTF_USESHOWWINDOW;
	startinfo.wShowWindow		= SW_HIDE;				// don't want a DOS window coming up :-p

	stat = CreateProcess(NULL, commandLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS,
							NULL, NULL, &startinfo, &procinfo);

	// couldn't launch for some reason
	if (!stat)
	{
		DWORD err = GetLastError();
		success = FALSE;
	}
	else
	{
		DWORD waitStat = WaitForSingleObject(procinfo.hProcess, INFINITE);

		if (waitStat == WAIT_ABANDONED)
		{
			success = FALSE;
		}
		else
		{
			if (_access(outfilename, 0) == -1)
				success = FALSE;
		}
	}
	return success ? eAnmReturnAllGood : eAnmReturnAllBad;
#endif  // ELSE ANM_DO_GZIP_INTERNAL
}
