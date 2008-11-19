//------------------------------------------------------------------------------
// File: anmAudioSensorRenderer.h
//
// Desc: DirectShow sample code - header file for anmaudioSensorRenderer renderer.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


// {954862FC-BB7C-49b5-A8B7-7D85FE0A365C}
DEFINE_GUID(CLSID_AudioSensorRenderer, 
0x954862fc, 0xbb7c, 0x49b5, 0xa8, 0xb7, 0x7d, 0x85, 0xfe, 0xa, 0x36, 0x5d);


// These are our video window styles



#define  AUDIO_BUFFER_SIZE  (1024*200)
#define  BEATBUFFER_SIZE   ( 1024 )


class CanmAudioSensorRenderer : public CBaseRenderer
{

public:
	CanmAudioSensorRenderer( class CAnmAudioSensor *pRenererObject );


    ~CanmAudioSensorRenderer();

//     STDMETHODIMP NonDelegatingQueryInterface(REFIID, void **);

    STDMETHODIMP Pause();
    STDMETHODIMP Stop();
	STDMETHODIMP Run(REFERENCE_TIME tStart);



    HRESULT BreakConnect();
	HRESULT SetMediaType(const CMediaType *pmt);
    HRESULT CheckMediaType(const CMediaType *pmt);
    HRESULT DoRenderSample(IMediaSample *pMediaSample);
    void OnReceiveFirstSample(IMediaSample *pMediaSample);



	void InitBuffer();
//	DWORD GetBufferSize(){ return m_bufferSize; }

	bool SetCurrentPosition( DWORD pos = 0 );
	bool SetCurrentPosInSeconds( double dTime );

	bool IsSoundBufferPlaying();

	bool IsFlushingInputPin();
	void CanmAudioSensorRenderer::ComputeAudioOutput();

	bool GetAudioData( 
				Integer frequencyDataSize,
				Float frequencyStart,
				Float frequencyEnd,
				Boolean &bBeat, 
				Float& fVal, 
				FloatArray& MyFloatArrayOut );


	void CanmAudioSensorRenderer::BufferAudioData( PBYTE pbData,
				int iPos, int len );
	void CanmAudioSensorRenderer::ProcessAudioData( PBYTE pbData, int iDstOffset, int len );
	void CanmAudioSensorRenderer::MapFrequencyData( float* fFreqDataFull, int nFrewDataFull );
	void SetRenderObject( class CAnmAudioSensor	*pRenererObject ){ m_pRenererObject = pRenererObject; }



protected:

	WAVEFORMATEX			 m_Format;
	bool					 m_bBufferPlayPending;
	REFERENCE_TIME			 m_tLastStart;
	REFERENCE_TIME			 m_tLastStop;

	DWORD					 m_iPos;
	bool					 m_bPlaying;
	DWORD					 m_LastComputePos;


//// ANM_AUDIOANIM
	Boolean					 m_bAudioBeat;
	Float					 m_AudioDataOut;
	int						 m_nHistorySize;
	Float					 m_HistoryMag;
	unsigned char			 m_AudioDataBuffer[AUDIO_BUFFER_SIZE];

	float					 m_AudioAnimDataBuffer[AUDIO_BUFFER_SIZE];


	int						 m_nBitsPerSample;
	int						 m_nBytesPerSample;
	int						 m_iBytesPerAnimSample;
	int						m_iSamplePerAnimSample;
	int						 m_iAnimBufferSize;
	int						m_nSamplesPerSec;


	int						m_BeatBuffer[BEATBUFFER_SIZE];
	int						m_iBeatBufferWritePos;
	int						m_iBeatBufferReadPos;


	float					m_MinAudioThreshold;
	float					m_FallOffTime;
	float					m_PeakOffset;

	float					m_ReplacementLookBackTime;
	float					m_ReplacementOffset;
	float					m_ReplacementDelta;


	float*					m_pFreqData;
	int						m_nFrewData;

	int						m_nFrewDataOut;

	float*					m_pFreqData0;
	int						m_nFreqData0;

	float					m_fract0;
	float					m_fractf;
	
	bool					m_bLockedWhileReading;
	bool					m_bLockedWhileWriting;

	class CAnmAudioSensor	*m_pRenererObject;



FILE* m_FP;

};

