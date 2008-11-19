/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: SoundLayer.cpp
 *    Desc: DirectSound wrapper, based off of the SDK samples
 *          
 * (C) 1999 by Adrian Perez
 *
 *		Revision history:
 *			$Log: SoundLayer.cpp,v $
 *			Revision 1.2  2004/03/22 07:40:50  Tony
 *			Spatialized audio-- first attempt
 *			
 *			Revision 1.1.1.1  2003/09/10 02:18:12  turk
 *			build466
 *			
 * See license.txt for modification and distribution information
 ******************************************************************/

#include "stdafx.h"

#include "SoundLayer.h"
#include "Sound.h"
#include "application.h"
#include "anmmessage.h"

cSoundLayer::cSoundLayer( cApplication *pApp, HWND hWnd )
{
	m_pApp = pApp;
	m_pDSound = NULL;
	m_pPrimary = NULL;
	m_pListener = NULL;

	pApp->SetSound(this);

    HRESULT             hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    // Create IDirectSound using the primary sound device
	hr = DirectSoundCreate( NULL, &m_pDSound, NULL );
    if( FAILED( hr ) )
	{
		pApp->WriteToConsole(ANMMSG_DSOUND_FAILED);
		// ANMMSG_DSOUNDCREATEFAILED : "DirectSoundCreate failed" 
//        throw CAnmError(ANMMSG_DSOUNDCREATEFAILED);
		return;
	}

    // Set coop level to DSSCL_PRIORITY
	hr = m_pDSound->SetCooperativeLevel( hWnd, DSSCL_PRIORITY );
    if( FAILED( hr ) )
	{
		// ANMMSG_DSOUNDSCLFAILED : "SetCooperativeLevel (DS) failed"
		return;
//        throw CAnmError(ANMMSG_DSOUNDSCLFAILED);
	}

    // Get the primary buffer 
	sAutoZero<DSBUFFERDESC> dsbd;
    dsbd.dwFlags       = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
       
	hr = m_pDSound->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL );
    if( FAILED( hr ) )
	{
		// ANMMSG_DSOUNDCSBFAILED : "CreateSoundBuffer (DS) failed"
//        throw CAnmError(ANMMSG_DSOUNDCSBFAILED);
		return;
	}

    // Set primary buffer format to 22kHz and 16-bit output.
    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = 2; 
    wfx.nSamplesPerSec  = 22050; 
    wfx.wBitsPerSample  = 16; 
    wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    if( FAILED( hr = pDSBPrimary->SetFormat(&wfx) ) )
	{
		// ANMMSG_DSOUNDSETFORMATFAILED : "SetFormat (DS) failed"
//       throw CAnmError(ANMMSG_DSOUNDSETFORMATFAILED);
		return;
	}

	// Get listener interface.
	hr = pDSBPrimary->QueryInterface(IID_IDirectSound3DListener, (LPVOID *)&m_pListener);

    SafeRelease( pDSBPrimary );
}

cSoundLayer::~cSoundLayer()
{
	SafeRelease(m_pListener);
	SafeRelease( m_pPrimary );
	ExtraSafeRelease( m_pDSound );
}

void cSoundLayer::CommitChanges()
{
	if (m_pListener)
	{
		HRESULT hr = m_pListener->CommitDeferredSettings();
	}
}


void cSoundLayer::SetListenerPosition(const Point3 &pos)
{
	if (m_pListener)
	{
		HRESULT hr = m_pListener->SetPosition(pos.x, pos.y, pos.z, DS3D_DEFERRED );
	}
}

void cSoundLayer::SetListenerOrientation(const Point3 &dir, const Point3 &up )
{
	if (m_pListener)
	{
		HRESULT hr = m_pListener->SetOrientation(
			dir.x, dir.y, dir.z, 
			up.x, up.y, up.z, 
			DS3D_DEFERRED );
	}
}