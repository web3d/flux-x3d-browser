/********************************************************************************
 * Flux
 *
 * File: anmdecrypt.cpp
 * Description: Decryption module
 *
 *				NOTE: Relies on vendor-supplied decryption DLL
 *						not included with Flux distribution
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
#include "anmdecrypt.h"
#include <io.h>
 

#define DECRYPTLIBNAME "fluxcryp.dll"

#define TESTENCRYPTPROCNAME "AnmTestForEncrypt"
#define DECRYPTPROCNAME "AnmDecrypt"

typedef bool (*TESTENCRYPTPROC) (const char *filename);
typedef bool (*DECRYPTPROC) (const char *infilename, char *outfilename, int maxbuf);

static bool intialized = false;
static HMODULE hLibraryModule = NULL;
static TESTENCRYPTPROC testProc = NULL;
static DECRYPTPROC decryptProc = NULL;

static void Init()
{
	char modpath[ANMMAXSTRBUF];

	GetModuleFileName(AfxGetInstanceHandle(), modpath, ANMMAXSTRBUF - strlen(DECRYPTLIBNAME) - 2);

	char *dir = strrchr(modpath, '\\');

	if (dir == NULL)
		return;

	strcpy(dir + 1, DECRYPTLIBNAME);

	hLibraryModule = LoadLibrary(modpath);

	if (hLibraryModule != NULL)
	{
		testProc = (TESTENCRYPTPROC) GetProcAddress(hLibraryModule, TESTENCRYPTPROCNAME);

		decryptProc = (DECRYPTPROC) GetProcAddress(hLibraryModule, DECRYPTPROCNAME);
	}

	intialized = true;
}


// krv:
// Decryption section
// returns TRUE if file either has required decryption
bool CAnmDecrypt::TestForEncrypt(const char *filename)
{
	if (!intialized)
		Init();

	if (testProc)
		return (*testProc)(filename);

	return false;
}

// returns OK if decryption infilename successful; outfilename is where the output goes
bool CAnmDecrypt::Decrypt(const char *infilename, char *outfilename, int maxbuf)
{
	if (!intialized)
		Init();

	if (decryptProc)
		return (*decryptProc)(infilename, outfilename, maxbuf);

	return false;
}
