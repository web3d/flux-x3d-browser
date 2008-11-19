/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			canmupdatecheck.cpp
 *		Description:	Checks for Updates available
 *						Windows Only.
 *
 * (C) 2001 by Keith Victor
 *
 *		Revision history:
 *			
 *
 *******************************************************************/

#include "stdafx.h"
#include <afxinet.h>
#include "anmupdatecheck.h"
#include "FileVersionInfo.h"
#include "anmFileManager.h"


#include "anmscene.h"
#include "anmworld.h"




#define ANM_VERSION_NUMBER_URL   ( "http://www.mediamachines.com/autoupdate/fluxversion.txt" )


static bool CompareVersionNumbers( const char* VersionOfLatest, const char* VersionThis )
{
	char SingleVersionLatest[80];
	char SingleVersionThis[80];

	if( strlen( VersionOfLatest ) < 8 ||
		strlen( VersionThis ) < 8 ) {
		return false;
	}

	const char* pLatest = VersionOfLatest;
	const char* pThis = VersionThis;

	for( int iVer=0; iVer<4; iVer++ ) {
		const char* pLook;

		for( pLook = pLatest; pLook; ) {
			if( *pLook == ',' ||
				*pLook < 32 ) {

				strncpy( SingleVersionLatest, pLatest, pLook - pLatest );
				SingleVersionLatest[pLook - pLatest] = '\0';

				if( iVer < 3 ) {
					if( *pLook == ',' ) {
						pLatest = pLook+1;
					}
					else {
						return false;
					}
				}
				pLook = NULL;
			}
			else {
				pLook++;
			}
		}


		for( pLook = pThis; pLook; ) {
			if( *pLook == ',' ||
				*pLook < 32 ) {
				strncpy( SingleVersionThis, pThis, pLook - pThis );
				SingleVersionThis[pLook - pThis] = '\0';
				if( iVer < 3 ) {
					if( *pLook == ',' ) {
						pThis = pLook+1;
					}
					else {
						return false;
					}
				}
				pLook = NULL;
			}
			else {
				pLook++;
			}
		}

		// Now comapre the strings
		//
		if( strcmp( SingleVersionLatest, SingleVersionThis ) > 0 ) {
			return true;
		}


	}
	return false;
}


static GetVersionOfThisOcx( char* versionString )
{
	versionString[0] = '\0';
	char thisexePath[500];
	GetModuleFileName( AfxGetInstanceHandle(), thisexePath,  500 );

	CFileVersionInfo  fvi;
	if (fvi.Create( thisexePath ) ) {
		int iThisVersion[4];
		iThisVersion[3] = fvi.GetFileVersion( 0 );
		iThisVersion[2] = fvi.GetFileVersion( 1 );
		iThisVersion[1] = fvi.GetFileVersion( 2 );
		iThisVersion[0] = fvi.GetFileVersion( 3 );
		sprintf( versionString, 
			"%d,%d,%d,%d",
			iThisVersion[0],
			iThisVersion[1],
			iThisVersion[2],
			iThisVersion[3] );
	}


}


static bool TestForUpdateNeeded( char* versionFileName, char* UpdateUrl )
{
	bool bNeedUpdate = false;
	char buffer[1024];
	FILE* fin = fopen( versionFileName, "r" );
	if( fin ) {
		fread( buffer, 1024, 1, fin );
		buffer[1000] = '\0';
		char* pFind = NULL;
		char ThisVersionString[100];
		GetVersionOfThisOcx( ThisVersionString );

		if( CompareVersionNumbers( buffer, ThisVersionString ) ) {
			char* pUpdateUrl = strstr( buffer, "http" );
			if( pUpdateUrl ) {
				strcpy( UpdateUrl, pUpdateUrl );
				if( ( pFind = strstr( UpdateUrl, ".html" ) ) ) {
					*( pFind+5 ) = '\0';
					bNeedUpdate = true;
				}
			}
		}
		fclose( fin );
	}
	return bNeedUpdate;
}



static BOOL SetRegStr( const char* cpath, const char* cname, unsigned char* nameset )
{
	int istat = FALSE;
	HKEY hkey;

	LONG iOpenStat = 0;

	if( ( iOpenStat = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
					cpath,
					0, KEY_SET_VALUE, &hkey) ) == ERROR_SUCCESS) 
	{
		LONG iSetStat = RegSetValueEx( hkey,
							cname,
							NULL, REG_SZ,
							(const unsigned char *)nameset,
							strlen( (const  char *)nameset )+1 );
		RegCloseKey(hkey);
		istat = TRUE;
	}
	
	return istat;
}


#define LAST_UPDATE_DATE_REG_PATH			( "SOFTWARE\\Media Machines, Inc.\\Flux" )
#define LAST_UPDATE_DATE_REG_NAME			( "LastUpdateTest" )



static BOOL SetRegStrWithCreate( const char* cpath, 
								    const char* cname, char* nameset )
{
	int istat = FALSE;
	if( !SetRegStr( cpath, cname, ( unsigned char* )nameset ) ) { 


		HKEY hkey = NULL;
		LONG iCreateStat = RegCreateKey( HKEY_LOCAL_MACHINE,
			cpath,
			&hkey );

		istat = SetRegStr( cpath, cname, ( unsigned char* )nameset );

	}
	else {
		istat = TRUE;
	}

	return istat;
}


static BOOL GetRegStr( const char* cpath, const char* cname, char* pathout, int iSizeOut0 )
{
	DWORD	iSizeOut = iSizeOut0;
	int istat = FALSE;
	DWORD type;
	DWORD size = iSizeOut;
	HKEY hkey;

	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
					cpath,
					0, KEY_READ, &hkey) == ERROR_SUCCESS)  
	{
		if (RegQueryValueEx(hkey, cname, NULL, &type, NULL, &size) == ERROR_SUCCESS &&
			 size < iSizeOut ) {
			if (RegQueryValueEx(hkey, cname, NULL, &type, ( unsigned char* )pathout, &size) == ERROR_SUCCESS) {
				istat = TRUE;
			}
		}
		RegCloseKey(hkey);
	}
	return istat;
}



CAnmUpdateCheck::CAnmUpdateCheck( cApplication *pApp ) :
	m_pApp( pApp )
{


	// Do the test for updates only once per app instance.
	static bool bDoneTest = false;

	if( !bDoneTest ) {
		bDoneTest = true;

		// check if we have done a Update Text today.
		//
		bool bDoUpdateCheck = true;

		time_t secs = time( (long*) 0 );
		tm* ptime = localtime( &secs );
		char ThisUpdateDateString[300];
		sprintf( ThisUpdateDateString, "%d", ptime->tm_yday );

		char LastUpdateDateString[400];
		if( GetRegStr( LAST_UPDATE_DATE_REG_PATH, LAST_UPDATE_DATE_REG_NAME, LastUpdateDateString, 380 ) ) {
			if( strcmp( LastUpdateDateString, ThisUpdateDateString ) == 0 ) {
				bDoUpdateCheck = false;
			}
		}


		if( bDoUpdateCheck ) {
			SetRegStrWithCreate( LAST_UPDATE_DATE_REG_PATH, LAST_UPDATE_DATE_REG_NAME, ThisUpdateDateString );

			StringArray *pUrl = new StringArray;
			pUrl->push_back(new CAnmString(ANM_VERSION_NUMBER_URL));
			CAnmUrlObject::SetUrl(pUrl);

			Load( m_pApp );
		}
	}
}

bool CAnmUpdateCheck::CheckForUpdate( char* updateFile )
{
	bool bInformIfOK = false;

	bool bDidUpdate = false;
	// first test if we need to  update.
	//
	char UpdateUrl[1024];
	if( TestForUpdateNeeded( updateFile, UpdateUrl ) ) {
		// we need to update.
		// Launch Update App.


		if( 
			MessageBox(
			  m_pApp->GetTopLevelHWnd(),          // handle of owner window
			  "Updates are currently available for Flux Player.\nDo you want to Update Now?", 
			  "Flux Player Update Check",
			MB_YESNO ) == IDYES ) {

			StringArray *pUrl = new StringArray;
			StringArray *pParams = new StringArray;
			pUrl->push_back(new CAnmString(UpdateUrl));

			m_pApp->GetFileManager()->RequestLinkToUrl( pUrl, "", pParams);
			
//			ShellExecute( NULL, NULL, UpdateUrl, NULL, NULL, SW_SHOWNORMAL );
			bDidUpdate = true;
		}

	}
	else if( bInformIfOK ) {
		AfxMessageBox( "Your Player is up to date!", MB_ICONINFORMATION );
	}
	return bDidUpdate;
}


void CAnmUpdateCheck::handleUrlLoaded(String pRequestUrl, CAnmAsset *pAsset)
{
}

class CAnmAsset *CAnmUpdateCheck::handleUrlLoaded(String pRequestUrl, String pCacheFileName)
{
	bool bUpdate = CheckForUpdate( pCacheFileName->GetBuf() );
	return NULL;
}

