/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			anmupdatecheck.h
 *		Description:	Staticx class to do update test for Windows.
 *
 *
 * (C) 2001 by Tony Parisi
 *
 *		Revision history:
 *			
 *
 *******************************************************************/

#ifndef _anmupdatecheck_h
#define _anmupdatecheck_h

#include "anmurlobject.h"


// N.B.: length
class CAnmUpdateCheck : public CAnmUrlObject
{
private :

	bool CheckForUpdate( char* updateFile );

	class cApplication *m_pApp;

public :
	CAnmUpdateCheck( class cApplication *pApp );
	virtual class CAnmAsset *CAnmUpdateCheck::handleUrlLoaded(String pRequestUrl, String pCacheFileName);
	virtual void CAnmUpdateCheck::handleUrlLoaded(String pRequestUrl, class CAnmAsset *pAsset);

	// returns TRUE if the update was triggered.
};

#endif // _anmupdatecheck_h
