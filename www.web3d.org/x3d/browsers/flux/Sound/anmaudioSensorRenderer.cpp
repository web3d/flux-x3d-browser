//------------------------------------------------------------------------------
// File: CanmAudioSensorRenderer.cpp
//
// Desc: A Audio Sensor Renederer that fits into a DIrectSHow filter Graph
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <streams.h>
#include <initguid.h>
#include "anmaudioSensorRenderer.h"
#include "SoundLayer.h"
#include "anmAudioSensor.h"

#include "Fourier.h"

// Setup data

const AMOVIESETUP_MEDIATYPE sudIpPinTypes =
{
    &MEDIATYPE_Audio,              // MajorType
    &MEDIASUBTYPE_NULL            // MinorType
};

const AMOVIESETUP_PIN sudIpPin =
{
    L"Input",                     // The Pins name
    FALSE,                        // Is rendered
    FALSE,                        // Is an output pin
    FALSE,                        // Allowed none
    FALSE,                        // Allowed many
    &CLSID_NULL,                  // Connects to filter
    NULL,                         // Connects to pin
    1,                            // Number of types
    &sudIpPinTypes                // Pin details
};




const AMOVIESETUP_FILTER sudAudiooutAx =
{
    &CLSID_AudioSensorRenderer,   // Filter CLSID
    L"Audio Sensor Renderer",     // String name
    MERIT_NORMAL,                 // Filter merit
    1,                            // Number of pins
    &sudIpPin                     // Pin details
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance
// function when it is asked to create a CLSID_VideoRenderer object
/*************

CFactoryTemplate g_Templates[] = {
    { L"Audio Display"
    , &CLSID_AudioSensorRenderer
    , CanmAudioSensorRenderer::CreateInstance
    , NULL
    , &sudAudiooutAx },
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

//
// DllRegisterServer
//
// Used to register and unregister the filter
//
STDAPI DllRegisterServer()
{
    return AMovieDllRegisterServer2( TRUE );

} // DllRegisterServer


//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
    return AMovieDllRegisterServer2( FALSE );

} // DllUnregisterServer

****************/

//
// Constructor
//
//
#pragma warning(disable:4355)
//

static HRESULT statichr;


CanmAudioSensorRenderer::CanmAudioSensorRenderer( class CAnmAudioSensor *pRenererObject ) :
                          CBaseRenderer(CLSID_AudioSensorRenderer, 
                                   NAME("Audio Sensor Renderer"), NULL, &statichr),
	m_tLastStart( 1 ),
	m_tLastStop( 1 ),
	m_iPos( 0 ),
	m_bPlaying( false ),
	m_pFreqData( NULL ),
	m_nFrewData( 0 ),
	m_nFrewDataOut( 0 ),
	m_pFreqData0( NULL ),
	m_nFreqData0( 0 ),
	m_bLockedWhileWriting( false ),
	m_bLockedWhileReading( false ),
	m_LastComputePos( 42 ),
	m_pRenererObject( pRenererObject )
{

//  ANM_AUDIOANIM


	m_MinAudioThreshold = 50.0;
	m_FallOffTime = 0.4;
	m_PeakOffset = 40.0;

	m_ReplacementLookBackTime = .000001;
	m_ReplacementOffset = -0.0;
	m_ReplacementDelta = 60.0;



	m_iBeatBufferWritePos = -1;
	m_iBeatBufferReadPos = 0;

	m_FP = NULL;
							  
if( !m_FP ) {
//	m_FP = fopen( "c:/keith/db.txt", "w" );
}


	m_AudioDataOut = 0.0;
	m_bAudioBeat = FALSE;
	m_nHistorySize = 0;
	m_HistoryMag = 0.0;

	m_nBitsPerSample = 0;
	m_nBytesPerSample = 1;
	m_iBytesPerAnimSample = 0;


	for( int i=0; i<AUDIO_BUFFER_SIZE; i++ ) {
		m_AudioAnimDataBuffer[i] = 0.0;
	}
	for( i=0; i<BEATBUFFER_SIZE; i++ ) {
		m_BeatBuffer[i] = 0;
	}

	if( m_pRenererObject ) {
		m_pRenererObject->SetRendererFilter( this );
	}
	// Add us to the app's external resource list so that we'll get shut down properly later
}



//
// Destructor
//
CanmAudioSensorRenderer::~CanmAudioSensorRenderer()
{
	if( m_pRenererObject ) {
		m_pRenererObject->SetRendererFilter( NULL );
	}

	if( m_FP ) {
		fclose( m_FP );
	}
	delete [] m_pFreqData;
	delete [] m_pFreqData0;
}

//
// BreakConnect
//
// Deactivate the Audio out rendering window
//
HRESULT CanmAudioSensorRenderer::BreakConnect()
{

//	if( m_FP ) {
//		fprintf( m_FP, "XXXXXXXXXXXX  BreakConnect  XXXXXXXXXX\n" );
//	}

    return NOERROR;
} // BreakConnect


//
// CheckMediaType
//
// Check that we can support a given proposed type
//

HRESULT CanmAudioSensorRenderer::CheckMediaType(const CMediaType * pmt)
{
    WAVEFORMATEX *pwfx = (WAVEFORMATEX *) pmt->Format();

    if (pwfx == NULL)
        return E_INVALIDARG;

    // Reject non-PCM Audio type

    if (pmt->majortype != MEDIATYPE_Audio) {
        return E_INVALIDARG;
    }

    if (pmt->formattype != FORMAT_WaveFormatEx) {
        return E_INVALIDARG;
    }

    if (pwfx->wFormatTag != WAVE_FORMAT_PCM) {
        return E_INVALIDARG;
    }
    return NOERROR;
}


 
//-----------------------------------------------------------------------------
// SetMediaType: Graph connection has been made. 
//-----------------------------------------------------------------------------
HRESULT CanmAudioSensorRenderer::SetMediaType(const CMediaType *pmt)
{

	// The FIlter has recieved a Media Type.
	// Confirm the Type, set the Format, and Init the Buffer.
	//
	const GUID * pFT = pmt->FormatType();
	if( *pFT == FORMAT_WaveFormatEx ) {
		m_Format = (* ( ( WAVEFORMATEX* )pmt->Format() ) );

		m_nSamplesPerSec = m_Format.nSamplesPerSec;
		m_nBitsPerSample = m_Format.wBitsPerSample;
		m_nBytesPerSample = m_Format.wBitsPerSample / 8;
		m_iBytesPerAnimSample = 500 / m_nBytesPerSample;
		m_iSamplePerAnimSample = m_iBytesPerAnimSample / m_nBytesPerSample;
		m_iAnimBufferSize = AUDIO_BUFFER_SIZE / m_iBytesPerAnimSample;

		InitBuffer();
	}

    return S_OK;
}







//
// DoRenderSample
//
// This is called when a sample is ready for rendering
//
HRESULT CanmAudioSensorRenderer::DoRenderSample(IMediaSample *pSample)
{
    PBYTE pbData;

    REFERENCE_TIME tStart, tStop;
    pSample->GetTime(&tStart, &tStop);

	int len = pSample->GetActualDataLength();
/*
    CString str;
	str.Format( "tStart(%d), tStop(%d), Diff(%d ms), Bytes(%d)",
           tStart,
           tStop,
           (LONG)((tStart - m_tLast) / 10000),
           len );
*/
//    m_tLast = tStart;

    // Copy the data to the file

    HRESULT hr = pSample->GetPointer(&pbData);
    if (FAILED(hr)) {
        return hr;
    }
    
    void*   pbDataDst  = NULL;
    void*   pbDataDst2 = NULL;
    ulong   dwLength = 0;
    ulong   dwLength2 = 0;
	
//	double dStart = ( ( double ) tStart ) * ( ( double ) m_Format.nAvgBytesPerSec ) / 10000000.0;
//	DWORD iPos = ( ( DWORD ) dStart ) % AUDIO_BUFFER_SIZE;

	DWORD iPos = m_iPos % AUDIO_BUFFER_SIZE;


//	for( int iSleep=0; m_bLocked && iSleep<10; iSleep++ ) {
//		Sleep( 50 );
//	}

	if( !m_bLockedWhileReading ) {
		m_bLockedWhileWriting = true;
		BufferAudioData( pbData, iPos, len );
		m_bLockedWhileWriting = false;

		m_iPos += len;
		m_tLastStart = tStart;
		m_tLastStop = tStop;

	}


	return S_OK; 
	
	

} // DoRenderSample




//
// OnReceiveFirstSample
//
// Display an image if not streaming
//
void CanmAudioSensorRenderer::OnReceiveFirstSample(IMediaSample *pMediaSample)
{
    if(IsStreaming() == FALSE)
    {
        ASSERT(pMediaSample);
    }

} // OnReceiveFirstSample

bool CanmAudioSensorRenderer::IsFlushingInputPin()
{
	return ( m_pInputPin && m_pInputPin->IsFlushing() );
}


bool CanmAudioSensorRenderer::SetCurrentPosition( DWORD iPos )
{
	m_iPos = iPos;
	return false;
}

bool CanmAudioSensorRenderer::SetCurrentPosInSeconds( double dTime )
{
	
	DWORD iPos = ( DWORD ) ( dTime * ( double ) m_Format.nAvgBytesPerSec );
	return SetCurrentPosition( iPos );
}


//
// Stop
//
// Overriden to close the AudioRenderer file
//
STDMETHODIMP CanmAudioSensorRenderer::Stop()
{
	m_bPlaying = false;
    return CBaseRenderer::Stop();
}

// 
// Run
//
// Overriden to open the AudioRenderer file
//
STDMETHODIMP CanmAudioSensorRenderer::Run(REFERENCE_TIME tStart)
{
	// Start the Audio Render, but dont start the DirectSound buffer to play yet.
	//  Need to wait till we got some data to play with.
	//


	m_bPlaying = true;
    return CBaseRenderer::Run(tStart);
}

/*
static void logit( char* str )
{
	FILE* fp = fopen( "g:/keith/dbstream.txt", "a" );
	if( fp ) {
		fprintf( fp, "%s\n", str );
		fclose( fp );
	}
}
*/

//
// Pause
//
//
STDMETHODIMP CanmAudioSensorRenderer::Pause()
{
    HRESULT hr = CBaseRenderer::Pause();
	return hr;
} // Pause





void CanmAudioSensorRenderer::InitBuffer( )
{
    /**
	 * Remember how big the buffer is
	 */
}

void CanmAudioSensorRenderer::ComputeAudioOutput()
{


	if( m_iBytesPerAnimSample <= 0 ) {
		return;
	}

	// if there is no fresh audio data, do NOT recompute.
	//
	DWORD lastlast = m_LastComputePos;
	if( m_LastComputePos == m_iPos ) {
		return;
	}
	m_LastComputePos = m_iPos;


	DWORD PlayPos = m_iPos;
	DWORD WritePos = 0;

//		LONGLONG llTotal = 0;
//		LONGLONG llCurrent = 0;
//		m_pMTR->QueryProgress( &llTotal, &llCurrent );


	int iBitsPerSample = m_Format.wBitsPerSample;

	int nSamplePerSecond = m_Format.nSamplesPerSec;
	int nChannels = m_Format.nChannels;
	int nAveBytsPerSec = m_Format.nAvgBytesPerSec;

	int nBytesPerStep = iBitsPerSample / 8;



#ifdef sfdsadf
	float fVal;
	m_AudioDataOut = 0.0;
	int nAdd = 500;
	for( int iAdd=0; iAdd<nAdd; iAdd++ ) {

		int iPos = ( PlayPos - iAdd*nBytesPerStep ) % AUDIO_BUFFER_SIZE;
		// modify iPos so it is aligned with SOund Sample SIze.

/***
		int iBitPos = iPos*8;
		iBitPos -= ( ( iBitPos ) % iBitsPerSample  );
		iPos = iBitPos/8;
		unsigned int theVal = ( ( (*( ( unsigned int* )( m_AudioDataBuffer+iPos ) ) ) ) & ( ( unsigned int )( ( 1<< ( iBitsPerSample ) ) - 1 ) ) );
		fVal = ( float ) theVal;
		fVal = fabs( fVal -   (        ( float ) ( ( unsigned int )( ( 1<< ( iBitsPerSample-1 ) ) ) )   ) );
***/				

//				fVal = 128.0 - fabs( ( float ) ( m_AudioDataBuffer[iPos+nBytesPerStep-1] - 128 ) );
		fVal = 128.0 - fabs( ( ( float ) ( m_AudioDataBuffer[iPos+nBytesPerStep-1] ) ) - 127.0 );
//				fVal = 128.0 - fabs( ( ( float ) ( m_AudioDataBuffer[iPos] ) ) - 127.0 );

		m_AudioDataOut +=  fVal;
	}


//			m_AudioDataOut *= 0.1;
	m_AudioDataOut /= ( float )nAdd;


#endif


	int iAnimPos = ( PlayPos - 1 / m_iBytesPerAnimSample ) % AUDIO_BUFFER_SIZE;
	iAnimPos = max( 0.0, iAnimPos );

	float fTry = m_AudioAnimDataBuffer[iAnimPos];
// fTry *= 0.1;

/*
if( !m_FP ) {
m_FP = fopen( "c:/keith/db.txt", "w" );
}
if( m_FP ) {
fprintf( m_FP, "BOTTOM Good Data       : %8d     %f, %f\n",PlayPos, m_AudioDataOut, fTry );
}
*/


//	m_AudioDataOut = fTry;

	bool bKeepTrying = true;
	while( bKeepTrying ) {
		bKeepTrying = false;
		if( m_iBeatBufferWritePos >= 0 &&
			m_iBeatBufferWritePos >= m_iBeatBufferReadPos ) {

			float fAnimPerSec = ( ( float ) m_nSamplesPerSec ) / ( ( float ) m_iSamplePerAnimSample );

			int iBeatPos = m_BeatBuffer[m_iBeatBufferReadPos%BEATBUFFER_SIZE];
			float CurrTime = ( ( float ) PlayPos ) / ( ( float ) ( m_nSamplesPerSec * m_nBytesPerSample ) );
			float BeatTime = ( ( float ) iBeatPos ) / fAnimPerSec;
			if( BeatTime<=CurrTime ) {
				bKeepTrying = true;
				m_bAudioBeat = TRUE;
				m_iBeatBufferReadPos++;
			}
		}
	}

	if( m_nFrewDataOut > 0 ) {

		int nFFT = 1024 ;

		int sample_rate = m_nSamplesPerSec;

		float* pFFTDataIn = new float[nFFT];

			
		int SampleRateMode = 2;
		for( int iBit=1; iBit<30; iBit++ ) {
			if( ( 1<<iBit ) <= sample_rate ) {
				SampleRateMode = ( 1<<iBit );
			}
		}

/**********		
		if( m_nFreqData0 != SampleRateMode ) {
			delete [] m_pFreqData0;
			m_nFreqData0 = SampleRateMode;
			m_pFreqData0 = new float[m_nFreqData0];

			for( int iSamp=0; iSamp<nFFT; iSamp++ ) {
				pFFTDataIn[iSamp] = 0.0;
			}
			CFourier MyFFT;
			MyFFT.ComplexFFT( pFFTDataIn, nFFT, SampleRateMode, 1 );
			for( int i=0; i<SampleRateMode; i++ ) {
				m_pFreqData0[i] = MyFFT.vector[i];
			}

		}
************/


		for( int iSamp=0; iSamp<nFFT; iSamp++ ) {
			int iPos = ( PlayPos + ( iSamp - 1 - nFFT )*nBytesPerStep ) % AUDIO_BUFFER_SIZE;
			pFFTDataIn[iSamp+0] = ( float ) m_AudioDataBuffer[iPos+nBytesPerStep-1];
		}

		CFourier MyFFT;
		MyFFT.ComplexFFT( pFFTDataIn, nFFT, SampleRateMode, 1 );

		MapFrequencyData( MyFFT.vector, SampleRateMode );

		delete [] pFFTDataIn;
	
	}


}


bool CanmAudioSensorRenderer::GetAudioData( 
				Integer frequencyDataSize,
				Float frequencyStart,
				Float frequencyEnd,
				Boolean &bBeat, 
				Float& fVal, 
				FloatArray& MyFloatArrayOut )
{
	m_fract0 = frequencyStart;
	m_fractf = frequencyEnd;
	m_nFrewDataOut = frequencyDataSize;

//	for( int iSleep=0; m_bLocked && iSleep<10; iSleep++ ) {
//		Sleep( 50 );
//	}

	if( !m_bLockedWhileWriting ) {
		m_bLockedWhileReading = true;
		ComputeAudioOutput();
		m_bLockedWhileReading = false;
	}

	fVal = m_AudioDataOut;
	bBeat = m_bAudioBeat;
	m_bAudioBeat = FALSE;

	int OutputSize = MyFloatArrayOut.size();
	if( OutputSize == m_nFrewData ) {
		for( int iOut=0; iOut<OutputSize; iOut++ ) {
			MyFloatArrayOut[iOut] = m_pFreqData[iOut];
		}
	}
	else {
		for( int iOut=0; iOut<OutputSize; iOut++ ) {
			MyFloatArrayOut[iOut] = 0.0;
		}
	}

	return true;
}


void CanmAudioSensorRenderer::MapFrequencyData( float* fFreqDataFull, int nFrewDataFull )
{
	if( m_nFrewDataOut > 0 ) {

		if( m_nFrewData != m_nFrewDataOut ) {
			delete [] m_pFreqData;
			m_nFrewData = m_nFrewDataOut;
			m_pFreqData = new float[m_nFrewData];
		}

		int iFreqData = ( int ) ( m_fract0 * ( float )nFrewDataFull  );
		int iFreq0 = iFreqData;


		float nFull = ( ( float ) nFrewDataFull ) * ( m_fractf - m_fract0 );

		for( int iOut=0; iOut<m_nFrewData; iOut++ ) {

			float tmp = 0.0;

			float fNext = ( ( float ) ( iOut+1) )/( ( float ) m_nFrewData );

			for( int nVals=0; iFreqData<nFrewDataFull &&
				( ( float ) ( iFreqData - iFreq0 ) ) / nFull < fNext; iFreqData++, nVals++ ) {
				tmp += fabs( fFreqDataFull[iFreqData] );
			}
			if( nVals > 0 ) {
				m_pFreqData[iOut] = tmp / ( ( float ) nVals );
			}
			else {
				m_pFreqData[iOut] = 0.0;
			}
		}
	}
}


void CanmAudioSensorRenderer::ProcessAudioData( PBYTE pbData, int iDstOffset, int len )
{
	memcpy( m_AudioDataBuffer+iDstOffset, pbData, len );



	int iStartProcess = iDstOffset - ( iDstOffset % m_iBytesPerAnimSample );
	int iStopProcess = iDstOffset + len;
	iStopProcess = iStopProcess - ( iStopProcess % m_iBytesPerAnimSample );

	int iProc = 0;
	int iByteInAnimSample;

	float fVal;
	int iPos;

	int iPosBest = -1;
	float fValMax = -1.0;


	
	for( iProc=iStartProcess; iProc<iStopProcess; iProc+=m_iBytesPerAnimSample ) {
		float fTot = 0.0;
		int iCount=0;
		for( iByteInAnimSample=0; iByteInAnimSample<m_iBytesPerAnimSample; iByteInAnimSample+=m_nBytesPerSample	) {

			iPos = ( iProc + iByteInAnimSample ) % AUDIO_BUFFER_SIZE;
			fVal = 128.0 - fabs( ( ( float ) ( m_AudioDataBuffer[iPos+m_nBytesPerSample-1] ) ) - 127.0 );

			fTot += fVal;

			iCount++;
		}


		iPos = ( iProc ) % AUDIO_BUFFER_SIZE;
		int iPosAnim = ( iPos / m_iBytesPerAnimSample ) % AUDIO_BUFFER_SIZE;


		assert( iCount == ( m_iSamplePerAnimSample ) );

		float fff = ( fTot / ( float )( m_iSamplePerAnimSample ) );
		assert( fff < 129.0 );

		fVal = 5.0 * fff;
		m_AudioAnimDataBuffer[iPosAnim] = fVal;
		m_AudioDataOut = fVal;

		if( fVal > fValMax ) {
			fValMax = fVal;
			iPosBest = iPos;
		}
	}

	int iStartAnim = iStartProcess / m_iBytesPerAnimSample;
	int iStopAnim = iStopProcess / m_iBytesPerAnimSample;

	int iPrev = iStartAnim-1;
	if( iPrev < 0 ) {
		iPrev = m_iAnimBufferSize-1;
	}
	int iNext,iThis; 
	float fThis;

	float fAnimPerSec = ( ( float ) m_nSamplesPerSec ) / ( ( float ) m_iSamplePerAnimSample );
	int iLastPeak;
	float fLastPeak, deltaVal, dValdT, fThresh, fDeltaTime;
	int iDeltaAnim;

	for( int iAnim=iStartAnim; iAnim<iStopAnim; iAnim++ ) {
		iNext = ( iAnim+1 ) % m_iAnimBufferSize;
		iThis = ( iAnim ) % m_iAnimBufferSize;
		fThis = m_AudioAnimDataBuffer[iThis];
		if( fThis >= m_AudioAnimDataBuffer[iNext] &&
			fThis >= m_AudioAnimDataBuffer[iPrev] &&
			fThis > m_MinAudioThreshold ) {

			bool bPeak = false;

			if( m_iBeatBufferWritePos >= 0 ) {
				iLastPeak = m_BeatBuffer[m_iBeatBufferWritePos%BEATBUFFER_SIZE];
				int iDeltaAnim = iAnim - iLastPeak;
				if( iDeltaAnim < 0 ) {
					iDeltaAnim += m_iAnimBufferSize;
				}
				fDeltaTime = ( ( float )iDeltaAnim ) / fAnimPerSec;
				if( fDeltaTime > m_FallOffTime ) {
					fThresh = m_MinAudioThreshold;
				}
				else {
					fLastPeak = m_AudioAnimDataBuffer[iLastPeak];
					deltaVal = fLastPeak + m_PeakOffset - m_MinAudioThreshold;
					dValdT = deltaVal / m_FallOffTime;
					fThresh = fLastPeak + m_PeakOffset - fDeltaTime*dValdT;
				}
				if( fThis > fThresh ) {
					bPeak = true;
				}
			}
			else {
				fLastPeak = m_MinAudioThreshold;
				if( fThis > m_MinAudioThreshold ) {
					bPeak = true;
				}
			}
			if( bPeak ) {
				bool bReplacePreviousBeat = false;

/********
				if( m_iBeatBufferWritePos >= 0 &&
					m_iBeatBufferWritePos > m_iBeatBufferReadPos &&
					fDeltaTime < m_ReplacementLookBackTime ) {

					dValdT = m_ReplacementDelta / m_ReplacementLookBackTime;
					fThresh = fThis + m_ReplacementOffset + fDeltaTime*dValdT;
					if( fLastPeak < fThresh ) {
						bReplacePreviousBeat = true;
					}
				}
				if( !bReplacePreviousBeat ) {
					m_iBeatBufferWritePos++;
				}

****************/

				m_iBeatBufferWritePos++;
				m_BeatBuffer[m_iBeatBufferWritePos%BEATBUFFER_SIZE] = iThis;
			}
		}
		iPrev = iThis;
	}


}

void CanmAudioSensorRenderer::BufferAudioData( PBYTE pbData,
				int iPos, int len )
{
	if( len <= 0 || len > AUDIO_BUFFER_SIZE ||
		iPos < 0 ) {
		// this should never happen
		return;
	}
	// this should never happen
	iPos = iPos % AUDIO_BUFFER_SIZE;

	if( iPos + len > AUDIO_BUFFER_SIZE ) {
		int nSpill = ( iPos + len ) - AUDIO_BUFFER_SIZE;
		int nTail = len - nSpill;
		ProcessAudioData( pbData,		iPos,	nTail );
		ProcessAudioData( pbData+nTail, 0,		nSpill );
	}
	else {
		ProcessAudioData( pbData,		iPos,	len );
	}
}



