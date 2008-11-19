/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: Sound.cpp
 *    Desc: Class wrapper for a WAV sound.  Uses the supplied
 *          DirectX code to read WAV files
 * copyright (c) 1999 by Adrian Perez
 * See license.txt for modification and distribution information
 ******************************************************************/

#include "stdafx.h"

#include "WavRead.h"
#include "Sound.h"
#include "..\windowing\Application.h"

#include "anmmessage.h"

using namespace std;
using std::map;

map< CWaveSoundRead*, int > cSound::m_waveMap;

cSound::cSound(cApplication *pApp, char* filename )
{
	m_pApp = pApp;
	m_pWaveSoundRead = NULL;
	m_pBuffer = NULL;
	m_p3DBuffer = NULL;

	// If we dont have a Diurect SOund Device, dont bother doing this.
	//
	if( m_pApp->GetSound() == NULL ||
		m_pApp->GetSound()->GetDSound() == NULL ) {
		return;
	}

    // Create a new wave file class
    m_pWaveSoundRead = new CWaveSoundRead();
	m_waveMap[ m_pWaveSoundRead ] = 1;

    // Load the wave file
    if( FAILED( m_pWaveSoundRead->Open( filename ) ) )
    {
		// ANMMSG_WAVCOULDNTOPENFILE : "Error: couldn't open WAV file"
        // throw CAnmError(ANMMSG_WAVCOULDNTOPENFILE);
		return;
    }

	Init();
	Fill();
}

cSound::cSound( cSound& in )
{
	m_pWaveSoundRead = in.m_pWaveSoundRead;
	m_waveMap[ m_pWaveSoundRead ]++;
	Init();
	Fill();
}

cSound& cSound::operator=( const cSound &in )
{
	/**
	 * Destroy the old object
	 */
	int count = --m_waveMap[ m_pWaveSoundRead ];
	if( !count )
	{
		delete m_pWaveSoundRead;
	}
	SafeRelease( m_pBuffer );
	SafeRelease( m_p3DBuffer );
	
	/**
	 * Clone the incoming one
	 */
	m_pWaveSoundRead = in.m_pWaveSoundRead;
	m_waveMap[ m_pWaveSoundRead ]++;

	Init();
	Fill();
	
	return *this;
}


cSound::~cSound()
{
	int count = m_waveMap[ m_pWaveSoundRead ];
	if( count == 1 )
	{
		delete m_pWaveSoundRead;
	}
	else
	{
		m_waveMap[ m_pWaveSoundRead ] = count - 1;
	}
		
	SafeRelease( m_pBuffer );
	SafeRelease( m_p3DBuffer );
}

void cSound::Init()
{
    /**
	 * Set up the DirectSound surface. the size of the sound file 
	 * and the format of the data can be retrieved from the wave 
	 * sound object.  Besides that, we only set the STATIC flag,
	 * so that the driver isn't restricted in setting up the 
	 * buffer.
	 */
	sAutoZero<DSBUFFERDESC> dsbd;
    dsbd.dwFlags       = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS
		| DSBCAPS_CTRL3D | DSBCAPS_CTRLFREQUENCY | DSBCAPS_MUTE3DATMAXDISTANCE ;
    dsbd.dwBufferBytes = m_pWaveSoundRead->m_ckIn.cksize;
    dsbd.lpwfxFormat   = m_pWaveSoundRead->m_pwfx;

    HRESULT hr;
	if( m_pApp->GetSound() == NULL ||
		m_pApp->GetSound()->GetDSound() == NULL ) {
		return;
	}

	hr = m_pApp->GetSound()->GetDSound()->CreateSoundBuffer( &dsbd, &m_pBuffer, NULL );
    if( FAILED( hr ) )
    {
		m_pApp->WriteToConsole(ANMMSG_DSOUND_BUFFER_FAILED);
		return;
		// ANMMSG_WAVCOULDNTCREATEBUFFER : "DirectSound::CreateSoundBuffer failed"
        // throw CAnmError(ANMMSG_WAVCOULDNTCREATEBUFFER);
    }

	hr = m_pBuffer->QueryInterface(IID_IDirectSound3DBuffer, 
                (LPVOID *)&m_p3DBuffer); 


    /**
	 * Remember how big the buffer is
	 */
    m_bufferSize = dsbd.dwBufferBytes;
}


void cSound::Restore()
{
    HRESULT hr;

    if( NULL == m_pBuffer )
	{
		return;
	}

    DWORD dwStatus;
    if( FAILED( hr = m_pBuffer->GetStatus( &dwStatus ) ) )
	{
		return;
		// ANMMSG_WAVCOULDNTGETBUFFERSTATUS : "Couldn't get buffer status" 
		// throw CAnmError( ANMMSG_WAVCOULDNTGETBUFFERSTATUS);
	}

    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
		/**
		 * Chances are, we got here because the app /just/
		 * started, and DirectSound hasn't given us any 
		 * control yet.  Just spin until we can restore
		 * the buffer
		 */
        do 
        {
            hr = m_pBuffer->Restore();
            if( hr == DSERR_BUFFERLOST )
                Sleep( 10 );
        }
        while( hr = m_pBuffer->Restore() );

		/**
		 * The buffer was restored.  Fill 'er up.
		 */
        Fill();
    }
}


void cSound::Fill()
{
    if( NULL == m_pBuffer )
	{
		return;
	}

    HRESULT hr; 
    uchar*   pbWavData; // Pointer to actual wav data 
    uint    cbWavSize; // Size of data
    void*   pbData  = NULL;
    void*   pbData2 = NULL;
    ulong   dwLength;
    ulong   dwLength2;

	/**
	 * How big the wav file is
	 */
    uint nWaveFileSize = m_pWaveSoundRead->m_ckIn.cksize;

    /**
	 * Allocate enough data to hold the wav file data
	 */
    pbWavData = new uchar[ nWaveFileSize ];
    if( NULL == pbWavData )
	{
	    delete [] pbWavData;
		// ANMMSG_WAVOUTOFMEMORY : "Out of memory"
        // throw CAnmError(ANMMSG_WAVOUTOFMEMORY);
		return;
	}

	hr = m_pWaveSoundRead->Read( 
		nWaveFileSize, 
        pbWavData, 
        &cbWavSize );
    if( FAILED( hr ) )           
	{
	    delete [] pbWavData;
		// ANMMSG_WAVREADFAILED : "m_pWaveSoundRead->Read failed"
        // throw CAnmError(ANMMSG_WAVREADFAILED);
		return;
	}

    /**
	 * Reset the file to the beginning 
	 */
    m_pWaveSoundRead->Reset();

    /**
	 * Lock the buffer so we can copy the data over
	 */
	hr = m_pBuffer->Lock( 
		0, m_bufferSize, &pbData, &dwLength, 
        &pbData2, &dwLength2, 0L );
    if( FAILED( hr ) )
	{
	    delete [] pbWavData;
		// ANMMSG_WAVBUFLOCKFAILED : "m_pBuffer->Lock failed"
        // throw CAnmError(ANMMSG_WAVBUFLOCKFAILED);
		return;
	}

    /**
	 * Copy said data over, unlocking afterwards
	 */
    memcpy( pbData, pbWavData, m_bufferSize );
    m_pBuffer->Unlock( pbData, m_bufferSize, NULL, 0 );

    /**
	 * We're done with the wav data memory.
	 */
    delete [] pbWavData;
}


bool cSound::IsPlaying()
{
    DWORD dwStatus = 0;
    if( NULL == m_pBuffer )
	{
		return false;
	}

    m_pBuffer->GetStatus( &dwStatus );

    if( dwStatus & DSBSTATUS_PLAYING )
        return true;
    else 
        return false;
}


void cSound::Resume( bool bLoop )
{
	Play( bLoop, -1.0 );
}

void cSound::Play( bool bLoop, Time startTime )
{
	HRESULT hr;
    if( NULL == m_pBuffer ) {
        return;
	}

    // Restore the buffers if they are lost
    Restore();

    // Play buffer 

	// Set Start Pos.
	if( startTime > 0.0 ) {
		uint nSamplesPerSec = m_pWaveSoundRead->m_pwfx->nSamplesPerSec;
		WORD wBitsPerSample = m_pWaveSoundRead->m_pwfx->wBitsPerSample;
		WORD wBytesPerSample = wBitsPerSample / 8;
		int posInBytes = ( int ) ( startTime * ( ( double ) nSamplesPerSec ) * wBytesPerSample );

		m_pBuffer->SetCurrentPosition( posInBytes );
	}


    DWORD dwLooped = bLoop ? DSBPLAY_LOOPING : 0L;
    if( FAILED( hr = m_pBuffer->Play( 0, 0, dwLooped ) ) )
	{
		// ANMMSG_WAVPLAYFAILED : "m_pBuffer->Play failed"
		// throw CAnmError(ANMMSG_WAVPLAYFAILED);
		return;
	}
}

void cSound::Stop()
{
	HRESULT hr;
    if( NULL == m_pBuffer )
        return;

    // Restore the buffers if they are lost
    Restore();

    // Play buffer 
    if( FAILED( hr = m_pBuffer->Stop() ) )
	{
		// ANMMSG_WAVSTOPFAILED : "m_pBuffer->Stop failed"
		// throw CAnmError(ANMMSG_WAVSTOPFAILED);
		return;
	}
}


Time cSound::GetDuration()
{
	if (m_pBuffer == NULL || m_pWaveSoundRead == NULL)
		return 0.f;

	uint nFileSize = m_pWaveSoundRead->m_ckIn.cksize;
	uint nSamplesPerSec = m_pWaveSoundRead->m_pwfx->nSamplesPerSec;
	WORD wBitsPerSample = m_pWaveSoundRead->m_pwfx->wBitsPerSample;
	WORD wBytesPerSample = wBitsPerSample / 8;
	if (nSamplesPerSec == 0.f || wBytesPerSample == 0.f)
		return 0.f;
	float duration = (Time) nFileSize / (Time) (nSamplesPerSec * wBytesPerSample);
	return duration;
}

#define SOUND_VOLUME_MAX 0
#define SOUND_VOLUME_MIN -2000			// DSound units are dB/100; -20B is pretty quiet


void cSound::SetIntensity(float intensity)
{
	if (m_pBuffer == NULL)
		return;

	// Input is intensity on a range from 0 to 1; transform to DSound values
	LONG range = SOUND_VOLUME_MAX - SOUND_VOLUME_MIN;
	LONG lIntensity = (LONG) (float) range * intensity;
	LONG lVolume = SOUND_VOLUME_MIN + lIntensity;

	HRESULT hr = m_pBuffer->SetVolume(lVolume);

	// just to make sure?
	hr = m_pBuffer->SetPan(DSBPAN_CENTER);
}

void cSound::SetPosition(const Point3 &pos)
{
    if( NULL == m_pBuffer )
	{
		return;
	}

	if (m_p3DBuffer)
	{
		HRESULT hr = m_p3DBuffer->SetPosition(pos.x, pos.y, pos.z, DS3D_DEFERRED);
	}
}

void cSound::SetOrientation(const Point3 &dir)
{
	// return;

	if (m_p3DBuffer)
	{
		HRESULT hr = m_p3DBuffer->SetConeOrientation(dir.x, dir.y, dir.z, DS3D_DEFERRED);
	}
}

void cSound::SetMinDistance(float minDistance)
{
	// return;

	if (m_p3DBuffer)
	{
		HRESULT hr = m_p3DBuffer->SetMinDistance(minDistance, DS3D_DEFERRED);
	}
}

void cSound::SetMaxDistance(float maxDistance)
{
	// return;

	if (m_p3DBuffer)
	{
		HRESULT hr = m_p3DBuffer->SetMaxDistance(maxDistance, DS3D_DEFERRED);
	}
}

void cSound::SetListenerPosition(const Point3 &pos)
{
	// return;

	m_pApp->GetSound()->SetListenerPosition(pos);
}



void cSound::SetListenerOrientation(const Point3 &dir, const Point3 &up)
{
	m_pApp->GetSound()->SetListenerOrientation(dir, up );
}


