/*******************************************************************
 *		Anima
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		File:			anmmovietexturerenderer.cpp
 *		Description:	Anima movie texture renderer for Direct Show
 *
 * (C) 2001-2004 by Media Machines
 *
 *		Revision history:
 *			$Log: anmmovietexturerenderer.cpp,v $
 *			Revision 1.4  2005/02/22 01:04:25  Tony
 *			Added console warnings for DShow errors
 *			
 *			Revision 1.3  2004/06/09 18:01:40  Tony
 *			MovieTexture resource cleanups
 *			
 *			Revision 1.2  2004/06/08 05:35:56  Tony
 *			More MovieTexture
 *			
 *			Revision 1.1  2004/06/07 03:34:10  Tony
 *			MovieTexture, improved Asset Cache
 *			

 *			
 *
 *******************************************************************/

#include "stdafx.h"
#include "anmmovietexturerenderer.h"
#include "anmdevice.h"
#include "anmmediasourceobject.h"
#include "anmplatform.h"
#include "anmworld.h"
#include "dsound.h"
#include "SoundLayer.h"
#include "anmaudioSensorRenderer.h"



#if 1
CAnmMovieTextureBlitter *CAnmMovieTextureBlitter::Create(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface,
														 LONG width, LONG height, LONG pitch, LONG byteDepth)
{
	cApplication *pApp = pDevice->GetApp();
	assert(pApp);

	CAnmPlatform *pPlatform = pApp->GetPlatform();
	assert(pPlatform);

	pTextureSurface->SetHasAlpha( byteDepth > 3 );
	return pPlatform->CreateMovieTextureBlitter(pDevice, pTextureSurface, width, height, pitch, byteDepth );
}

#else

#ifdef DX9
#include "anmdshowmovietextureblitter9.h"

// Factory
CAnmMovieTextureBlitter *CAnmMovieTextureBlitter::Create(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface,
														 LONG width, LONG height, LONG pitch, LONG byteDepth)
{
	pTextureSurface->SetHasAlpha( byteDepth > 3 );
	return new CAnmDShowMovieTextureBlitter9(pDevice, pTextureSurface, width, height, pitch, byteDepth);
}


#else

#ifdef DX8
#include "anmdshowmovietextureblitter8.h"

// Factory
CAnmMovieTextureBlitter *CAnmMovieTextureBlitter::Create(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface,
														 LONG width, LONG height, LONG pitch, LONG byteDepth )
{
	pTextureSurface->SetHasAlpha( byteDepth > 3 );
	return new CAnmDShowMovieTextureBlitter8(pDevice, pTextureSurface, width, height, pitch, byteDepth);
}

#else

#include "anmdshowmovietextureblitter7.h"

// Factory
CAnmMovieTextureBlitter *CAnmMovieTextureBlitter::Create(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface,
														 LONG width, LONG height, LONG pitch, LONG byteDepth )
{
	pTextureSurface->SetHasAlpha( byteDepth > 3 );
	return new CAnmDShowMovieTextureBlitter7(pDevice, pTextureSurface, width, height, pitch, byteDepth );
}

#endif // DX8
#endif // DX9

#endif // 0

//-----------------------------------------------------------------------------
// Msg: Display an error message box if needed
//-----------------------------------------------------------------------------
void Msg(cApplication *pApp, TCHAR *szFormat, ...)
{
    TCHAR szBuffer[512];

    va_list pArgs;
    va_start(pArgs, szFormat);
    _vstprintf(szBuffer, szFormat, pArgs);
    va_end(pArgs);

	pApp->WriteToConsole(szBuffer);
}



static HRESULT CountFilterPins(IBaseFilter *pFilter, ULONG *pulInPins, ULONG *pulOutPins)
{
    HRESULT hr=S_OK;
    IEnumPins *pEnum=0;
    ULONG ulFound;
    IPin *pPin;

    // Verify input
    if (!pFilter || !pulInPins || !pulOutPins)
        return E_POINTER;

    // Clear number of pins found
    *pulInPins = 0;
    *pulOutPins = 0;

    // Get pin enumerator
    hr = pFilter->EnumPins(&pEnum);
    if(FAILED(hr)) 
        return hr;

    pEnum->Reset();

    // Count every pin on the filter
    while(S_OK == pEnum->Next(1, &pPin, &ulFound))
    {
        PIN_DIRECTION pindir = (PIN_DIRECTION)3;

        hr = pPin->QueryDirection(&pindir);

        if(pindir == PINDIR_INPUT)
            (*pulInPins)++;
        else
            (*pulOutPins)++;

        pPin->Release();
    } 

    pEnum->Release();
    return hr;
}

static HRESULT GetPin( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin)
{
    CComPtr< IEnumPins > pEnum;
    *ppPin = NULL;
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if(FAILED(hr)) 
        return hr;

    ULONG ulFound;
    IPin *pPin;
    hr = E_FAIL;

    while(S_OK == pEnum->Next(1, &pPin, &ulFound))
    {
        PIN_DIRECTION pindir = (PIN_DIRECTION)3;
        pPin->QueryDirection(&pindir);
        if(pindir == dirrequired)
        {
            if(iNum == 0)
            {
                *ppPin = pPin;
                // Found requested pin, so clear error
                hr = S_OK;
                break;
            }
            iNum--;
        } 

        pPin->Release();
    } 

    return hr;
}

static IPin * GetInPin( IBaseFilter * pFilter, int Num )
{
    CComPtr< IPin > pComPin;
    GetPin(pFilter, PINDIR_INPUT, Num, &pComPin);
    return pComPin;
}


static IPin * GetOutPin( IBaseFilter * pFilter, int Num )
{
    CComPtr< IPin > pComPin;
    GetPin(pFilter, PINDIR_OUTPUT, Num, &pComPin);
    return pComPin;
}


static bool FindInputPin( IBaseFilter* pFilter, CComPtr<IPin> & pPinIn )
{

	bool bFound = false;

	IEnumPins  *pEnum;
	IPin       *pPin;

	pFilter->EnumPins(&pEnum);
	while( !bFound && pEnum->Next(1, &pPin, 0) == S_OK ) {
		PIN_DIRECTION PinDirThis;

		// first test for output pin
		//
		if( pPin->QueryDirection(&PinDirThis) == S_OK &&
			 PinDirThis == PINDIR_INPUT ) {

			pPinIn = pPin;
			bFound = true;
		}
		pPin->Release();
	}
	pEnum->Release();

	return bFound;
}



static bool FindDisconnectedOutputPin( IBaseFilter* pFilter, CComPtr<IPin> & pPinOut )
{

	bool bFound = false;

	IEnumPins  *pEnum;
	IPin       *pPin;

	pFilter->EnumPins(&pEnum);
	while( !bFound && pEnum->Next(1, &pPin, 0) == S_OK ) {
		PIN_DIRECTION PinDirThis;

		// first test for output pin
		//
		if( pPin->QueryDirection(&PinDirThis) == S_OK &&
			 PinDirThis == PINDIR_OUTPUT ) {
			IPin *pTestPin = NULL;
			if( SUCCEEDED( pPin->ConnectedTo( &pTestPin ) ) ) {
				pTestPin->Release();
			}
			else {
				pPinOut = pPin;
				bFound = true;
			}
		}
		pPin->Release();
	}
	pEnum->Release();

	return bFound;
}


// Look through all the Filters in the Graph to a
// Transform Filter with an Audio Output Pin,
//
static bool FindOutputAudioPinInGraph( IGraphBuilder* pGB, CComPtr<IPin> &pAudioPinOut )
{

	pAudioPinOut = NULL;
    HRESULT hr;
    IEnumFilters *pEnum = NULL;
    IBaseFilter *pFilter = NULL;
    ULONG ulFetched, ulInPins, ulOutPins;
    BOOL bFound=FALSE;

    // Verify graph builder interface
    if (!pGB)
        return false;



    // Get filter enumerator
    hr = pGB->EnumFilters(&pEnum);
    if (FAILED(hr))
        return false;

    pEnum->Reset();

    // Enumerate all filters in the graph
    while(!bFound && (pEnum->Next(1, &pFilter, &ulFetched) == S_OK))
    {
#ifdef DEBUG
        // Read filter name for debugging purposes
        FILTER_INFO FilterInfo;
        TCHAR szName[256];
    
        hr = pFilter->QueryFilterInfo(&FilterInfo);
        if (SUCCEEDED(hr))
        {
            // Show filter name in debugger
#ifdef UNICODE
            lstrcpy(szName, FilterInfo.achName);
#else
            WideCharToMultiByte(CP_ACP, 0, FilterInfo.achName, -1, szName, 256, 0, 0);
#endif
            FilterInfo.pGraph->Release();
        }       
#endif

        // Find a filter with one input and no output pins
        hr = CountFilterPins(pFilter, &ulInPins, &ulOutPins);
        if (FAILED(hr))
            break;

        if ((ulInPins >= 1) && (ulOutPins >= 1))
        {
			IEnumPins  *pEnum;
			IPin       *pPin;

			pFilter->EnumPins(&pEnum);
			while( pEnum->Next(1, &pPin, 0) == S_OK ) {
				PIN_DIRECTION PinDirThis;

				// first test for output pin
				//
				if( pPin->QueryDirection(&PinDirThis) == S_OK &&
					 PinDirThis == PINDIR_OUTPUT ) {
					// Next test for Audio pin
					// Enum the Types, and test for video.
					IEnumMediaTypes*     pEnumMT;
					ULONG						nFetched = 0;
					AM_MEDIA_TYPE*			pMediaTypes = NULL;
					if( pPin->EnumMediaTypes(&pEnumMT) == S_OK ) {
						while( pEnumMT->Next(1, &pMediaTypes, &nFetched) == S_OK ) {
							if( !pAudioPinOut &&
								pMediaTypes->formattype == FORMAT_WaveFormatEx ) {
								pAudioPinOut = pPin;
								bFound = true;
							}

							DeleteMediaType( pMediaTypes );
						}
						pEnumMT->Release();
					}
				}
				pPin->Release();
			}
			pEnum->Release();
            pFilter->Release();

		}
        else
        {
            // No match, so release the interface
            pFilter->Release();
        }
    }

    pEnum->Release();
    return ( pAudioPinOut != NULL );
}


// static factory method.
//
CAnmMovieTextureRenderer *CAnmMovieTextureRenderer::Create( class CAnmMediaSourceObject *pMediaSource,
		CAnmDevice *pDevice, CAnmWorld *pWorld,
		String filename, bool bDoAudio, bool bDoVideo, std::list<class CAnmAudioSensor *>& rendererList )
{

	CAnmMovieTextureRenderer * pNewMovieTextureRenderer = NULL;
	// Create our Movie Renderer.
	// If it does not have any AUdio or Video, then we kill it now, and return NULL,
	// so that we can try other urls in the url list.
	//
	CAnmMovieTextureRenderer* pMovieTest = new CAnmMovieTextureRenderer(pMediaSource, 
			pDevice, pWorld, filename, bDoAudio, bDoVideo, rendererList );
	if( pMovieTest ) {
		if( pMovieTest->HasAudio() || pMovieTest->HasVideo() ) {
			pNewMovieTextureRenderer = pMovieTest;
		}
		else {
			delete pMovieTest;
		}
	}

	return pNewMovieTextureRenderer;
}





//-----------------------------------------------------------------------------
// CAnmMovieTextureRenderer constructor
//-----------------------------------------------------------------------------
static HRESULT statichr;
CAnmMovieTextureRenderer::CAnmMovieTextureRenderer(class CAnmMediaSourceObject *pMediaSource, 
												   CAnmDevice *pDevice, CAnmWorld *pWorld, 
												   String filename, bool bTryAudio, bool bTryVideo, 
												   std::list<class CAnmAudioSensor *>& rendererList )
                                   : CBaseVideoRenderer(__uuidof(CLSID_FluxMovieTextureRenderer), 
                                   NAME("Flux Texture Renderer"), NULL, &statichr)
{
	AddRef();

	m_p3DBuffer = NULL;
	m_pListener = NULL;
	m_pBasicAudio = NULL;

	bool bAudioSensor = rendererList.size();

	m_InitPos = Point3::Zero;
	m_InitDir = Point3::Zero;
	m_InitListenerPos = Point3::Zero;
	m_InitMin = -1.0;
	m_InitMax = -1.0;


	m_bFilterGraphHasBeenRun = false;

	m_mediasource = pMediaSource;

	m_bHasVideo = false;
	m_bHasAudio = false;

	m_device = pDevice;
	assert(m_device);

	m_world = pWorld;
	assert(m_world);
	
	m_app = m_world->GetApp();
	assert(m_app);

	m_textureSurface = NULL;
	m_blitter = NULL;
	m_loop = false;
	m_active = false;

	// Add us to the app's external resource list so that we'll get shut down properly later
	m_app->AddExternalResource(this);

	if (FAILED(statichr))
	{
		return;
	}










	
    HRESULT hr = S_OK;
    CComPtr<IBaseFilter>    pFTR;           // Texture Renderer Filter
    CComPtr<IPin>           pFTRPinIn;      // Texture Renderer Input Pin
    CComPtr<IBaseFilter>    pFSrc;          // Source Filter
    CComPtr<IPin>           pVideoPinOut;    // Source Filter Output Pin   
    CComPtr<IPin>           pAnyPinOut;    // Source Filter Output Pin   


    CComPtr<IPin>           pAudioPinIn;      // Audio Renderer Input Pin
    CComPtr<IPin>           pAudioPinOut;    // Source Filter Output Pin  for audio






    // Create the filter graph
    if (FAILED(m_pGB.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC)))
        return;

    // Determine the file to load based on DirectX Media path (from SDK)
    // Use a helper function included in DXUtils.cpp
    WCHAR wFileName[MAX_PATH];

    #ifndef UNICODE
        MultiByteToWideChar(CP_ACP, 0, filename->GetBuf(), -1, wFileName, MAX_PATH);
    #else
        lstrcpy(wFileName, filename->GetBuf());
    #endif

	if (FAILED(hr = m_pGB->AddSourceFilter (wFileName, L"SOURCE", &pFSrc)))
    {
        Msg(m_app, TEXT("Warning: MovieTexture %s: Could not add source filter to graph!  hr=0x%x"), filename->GetBuf(), hr);
        return;
    }


	// Traverse the Pins, and Look for Audio, or Video if above call failed.
	//
	IEnumPins  *pEnum;
	IPin       *pPin;

	int			nUnknowOutputPins = 0;
	IPin		*pUnknowOutputPins[10];


	pFSrc->EnumPins(&pEnum);
	while( pEnum->Next(1, &pPin, 0) == S_OK ) {
		PIN_DIRECTION PinDirThis;

		// first test for output pin
		//
		if( pPin->QueryDirection(&PinDirThis) == S_OK &&
			 PinDirThis == PINDIR_OUTPUT ) {

			bool bIdentifiedPin = false;
			pAnyPinOut = pPin;

			// Next test for Video pin
			// Enum the Types, and test for video, and audio
			IEnumMediaTypes*     pEnumMT;
			ULONG						nFetched = 0;
			AM_MEDIA_TYPE*			pMediaTypes = NULL;
			if( pPin->EnumMediaTypes(&pEnumMT) == S_OK ) {
				while( pEnumMT->Next(1, &pMediaTypes, &nFetched) == S_OK ) {

					if( !pVideoPinOut && 
						( pMediaTypes->formattype == FORMAT_VideoInfo ||
						  pMediaTypes->formattype == FORMAT_VideoInfo2 ||
						  pMediaTypes->majortype == MEDIATYPE_Video ) ) {
						pVideoPinOut = pPin;
						bIdentifiedPin = true;
					}
					else if( !pAudioPinOut &&
						pMediaTypes->formattype == FORMAT_WaveFormatEx ||
 					    pMediaTypes->majortype == MEDIATYPE_Audio ) {
						pAudioPinOut = pPin;
						bIdentifiedPin = true;
					}

					DeleteMediaType( pMediaTypes );
				}
				pEnumMT->Release();
			}
			if( !bIdentifiedPin && nUnknowOutputPins < 10 ) {
				pUnknowOutputPins[nUnknowOutputPins++] = pPin;
			}



		}
		pPin->Release();
	}
	pEnum->Release();


	if( !pAnyPinOut ) {
		Msg(m_app, TEXT("Warning: MovieTexture %s: Could not find output pin!  hr=0x%x"), filename->GetBuf(), hr);
		return;
	}



	bool bGotVideoConnection = false;
	bool bGotAudioConnection = false;


	// First setup Video.
	//
	if( bTryVideo ) {

		// Setup the Video Renderer, if we want Video.
		//
		// Get a pointer to the IBaseFilter on the TextureRenderer, add it to graph
		pFTR = this;
		if (FAILED(hr = m_pGB->AddFilter(pFTR, L"TEXTURERENDERER")))
		{
			Msg(m_app, TEXT("Warning: MovieTexture %s: Could not add renderer filter to graph!  hr=0x%x"), filename->GetBuf(), hr);
		}
		else {
    		// Find the renderer's input pin
			if( !FindInputPin( pFTR, pFTRPinIn ) ) {
				m_pGB->RemoveFilter(this);
				Msg(m_app, TEXT("Warning: MovieTexture %s: Could not find input pin!  hr=0x%x"), filename->GetBuf(), hr);
			}
			else {
				 // Connect these two Video filters
				if( pVideoPinOut ) {
					// If we know which pin is the Video pin, use it.
					//
					hr = m_pGB->Connect(pVideoPinOut, pFTRPinIn);
					bGotVideoConnection = ( !FAILED(hr) );
				}
				else {
					// Try all the output pins.
					//
					for( int iTry=0; iTry<nUnknowOutputPins && !bGotVideoConnection; iTry++ ) {
						hr = m_pGB->Connect(pUnknowOutputPins[iTry], pFTRPinIn);
						bGotVideoConnection = (!FAILED(hr));
						if( bGotVideoConnection ) {
							pUnknowOutputPins[iTry] = NULL;
						}
					}
				}

				if( !bGotVideoConnection )
				{
					m_pGB->RemoveFilter(this);
					Msg(m_app, TEXT("Warning: MovieTexture %s: Could not connect Video pins!  hr=0x%x"), filename->GetBuf(), hr);
				}
			}
		}
	}


	// if we the Source filter did not have a audio output pin, then lets
	// serach the Filters in the GraphBuilder for a Transform Filter that contains 
	// some Audio Output
	// When we called Connect above, it added any required Transform filters.


	if( bTryAudio ) {
		if( bGotVideoConnection && ( pAudioPinOut == NULL ) ) {
			// If the graph already has the Video Pins connected, look anywhere in the
			// graph for an Audio Output Pine.
			FindOutputAudioPinInGraph( m_pGB, pAudioPinOut );
		}

		if( pAudioPinOut || nUnknowOutputPins ) {


			// Create a Default Audio Renderer
			//
			CComPtr<IBaseFilter>    pAudio;   
			pAudio.CoCreateInstance(CLSID_DSoundRender, NULL, CLSCTX_INPROC);
			if( pAudio ) {

				// Get the 3DSound Interfaces from our Default Renderer.
				//
				pAudio->QueryInterface(IID_IDirectSound3DBuffer, 
							(LPVOID *)&m_p3DBuffer); 
				pAudio->QueryInterface(IID_IDirectSound3DListener, 
							(LPVOID *)&m_pListener); 
				pAudio->QueryInterface(IID_IBasicAudio, 
							(LPVOID *)&m_pBasicAudio); 

				
				// Find the input pin to the Audio Renderer.
				//
				if( !FindInputPin( pAudio, pAudioPinIn ) ) {
					Msg(m_app, TEXT("Warning: AudioRenderer: Could not find input pin!  hr=0x%x"), hr);
				}
				else {

					// before we connect, add the AudioRenderer to the graph Builder
					if (FAILED(hr = m_pGB->AddFilter(pAudio, L"AUDIORENDERER")))
					{
						Msg(m_app, TEXT("Warning: AudioRenderer: Could not Add FIlter to Graph Builder!  hr=0x%x"), hr);
					}
					else {
						// Connect these two Audio filters


						if( pAudioPinOut ) {
							// If we know which pin is the Audio pin, use it.
							//
							hr = m_pGB->Connect(pAudioPinOut, pAudioPinIn);
							bGotAudioConnection = (!FAILED(hr));
						}
						else {
							// Try all the output pins.
							//
							for( int iTry=0; iTry<nUnknowOutputPins && !bGotAudioConnection; iTry++ ) {
								if( pUnknowOutputPins[iTry] != NULL ) {
									hr = m_pGB->Connect(pUnknowOutputPins[iTry], pAudioPinIn);
									bGotAudioConnection = ( !FAILED(hr) );
								}
							}
						}

						if( !bGotAudioConnection ) {
							Msg(m_app, TEXT("Warning: AudioRenderer: Could not connect pins!  hr=0x%x"), hr);
							m_pGB->RemoveFilter(pAudio);
						}

						if( bAudioSensor && bGotAudioConnection ) {
							// Find the pin into the audio Renderer, and insert a TEE in there.
							//


							std::list< CAnmAudioSensor * >::iterator iter;
							for( 
								iter = rendererList.begin(); 
								iter != rendererList.end();
								iter++ )
							{
								CAnmAudioSensor *pRendererNode = (*iter);
								InsertAudioSensor( pAudioPinIn, pRendererNode );
							}
						}
					}
				}
			}
		}
	}

	m_bHasVideo = bGotVideoConnection;
	m_bHasAudio = bGotAudioConnection;


	if( !bGotVideoConnection && !bGotAudioConnection ) {
		return;
	}


   // ot connect pins!  hr=0x8004025f
    // Get the graph's media control, event & position interfaces
    m_pGB.QueryInterface(&m_pMC);
    m_pGB.QueryInterface(&m_pMP);
    m_pGB.QueryInterface(&m_pME);

	m_active = true;
}

//-----------------------------------------------------------------------------
// CAnmMovieTextureRenderer destructor
//-----------------------------------------------------------------------------
CAnmMovieTextureRenderer::~CAnmMovieTextureRenderer()
{
    // Now shut down the graph
	if (m_active)
		Deactivate();

	// N.B.: duh, don't do this because the owner MovieTexture's destructor does
	// SafeUnRef(m_textureSurface);

	if (m_mediasource)
		m_mediasource->NotifyRendererDestroyed();

	// Take us off the app's resource list
	m_app->RemoveExternalResource(this);
}

void CAnmMovieTextureRenderer::Deactivate()
{
	// Stop the movie
    if (m_pMC) {
		m_pMC->Stop();
	}

	// Wait this thing out so that the DShow thread synchronizes
	OAFilterState state;

	while (true)
	{
		HRESULT hr = m_pMC->GetState(INFINITE, &state);

		if (SUCCEEDED(hr))
		{
			if (state == State_Stopped)
				break;
		}
		else
		{
			if (hr != VFW_S_STATE_INTERMEDIATE) // if still in transition, keep going
				break;
		}
	}

	// Unplug Texture Renderer Input Pin to avoid more callbacks
	if( HasVideo() ) {
		CComPtr<IPin>           pFTRPinIn;
		if( FindInputPin( this, pFTRPinIn ) && pFTRPinIn ) {
			m_pGB->Disconnect(pFTRPinIn);
		}
	}

	// Clean up interfaces
    if (m_pMC) m_pMC.Release();
    if (m_pME) m_pME.Release();
    if (m_pMP) m_pMP.Release();
    if (m_pGB) m_pGB.Release();

	// Chuck the temp texture surface
	SafeDelete(m_blitter);
	
	m_textureSurface = NULL;
	m_blitter = NULL;

	m_active = false;
}

void CAnmMovieTextureRenderer::FreeResource()
{
	delete this;
}

//-----------------------------------------------------------------------------
// CheckMediaType: This method forces the graph to give us an R8G8B8 video
// type, making our copy to texture memory trivial.
//-----------------------------------------------------------------------------
HRESULT CAnmMovieTextureRenderer::CheckMediaType(const CMediaType *pmt)
{

    HRESULT   hr = E_FAIL;
    VIDEOINFO *pvi;
    
    // Reject the connection if this is not a video type
    if( *pmt->FormatType() != FORMAT_VideoInfo ) {
        return E_INVALIDARG;
    }
    
    // Only accept RGB24
    pvi = (VIDEOINFO *)pmt->Format();
    if(IsEqualGUID( *pmt->Type(),    MEDIATYPE_Video)  &&
       IsEqualGUID( *pmt->Subtype(), MEDIASUBTYPE_RGB24))
    {
        hr = S_OK;
    }
    if(IsEqualGUID( *pmt->Type(),    MEDIATYPE_Video)  &&
       IsEqualGUID( *pmt->Subtype(), MEDIASUBTYPE_ARGB32))
    {
        hr = S_OK;
    }
    


    return hr;
}


void CAnmMovieTextureRenderer::PauseStream()
{
	if( !IsStreamFlushing() &&
		IsStreamPlaying() ) {
		m_pMC->Pause();
	}
}


 
void CAnmMovieTextureRenderer::ResumeStream()
{
	if( !IsStreamFlushing() &&
		IsStreamPaused() ) {
		HRESULT   hr;
		// Start the graph running;
		if (FAILED(hr = m_pMC->Run()))
		{
			Msg(m_app, TEXT("Warning: MovieTexture: Could not run the DirectShow graph!  hr=0x%x"), hr);
		}
	}
}





void CAnmMovieTextureRenderer::PlayStream(bool loop, Time startTime)
{
	m_loop = loop;

	if( !IsStreamFlushing() &&
		IsStreamPaused() ) {


		Time duration = GetDuration();

		if( startTime > duration ||
			startTime < -0.01) {
			startTime = 0.0;
		}
		HRESULT hr;

		if( !FAILED( m_pMP->put_CurrentPosition(startTime) ) ) {
			// Start the graph running;
			if (FAILED(hr = m_pMC->Run()))
			{
				Msg(m_app, TEXT("Warning: MovieTexture: Could not run the DirectShow graph!  hr=0x%x"), hr);
			}
			if( !m_bFilterGraphHasBeenRun ) {
				m_bFilterGraphHasBeenRun = true;
				ApplyInitSoundParams();
			}
		}
	}
}

 

void CAnmMovieTextureRenderer::InitStream()
{
	m_pMP->put_CurrentPosition(0);
	m_pMC->Pause();
}


bool CAnmMovieTextureRenderer::IsStreamFlushing()
{

	if( m_bHasVideo &&
		m_pInputPin &&
		m_pInputPin->IsFlushing() ) {
		return true;
	}
	return false;
}



bool CAnmMovieTextureRenderer::IsStreamPaused()
{
	OAFilterState state;
	if( !FAILED( m_pMC->GetState( 0, &state ) ) ) {
		return ( state == State_Paused || state == State_Stopped );
	}
	return false;
}

bool CAnmMovieTextureRenderer::IsStreamPlaying()
{
	OAFilterState state;
	if( !FAILED( m_pMC->GetState( 0, &state ) ) ) {
		return ( state == State_Running );
	}
	return false;
}



bool CAnmMovieTextureRenderer::Continue()
{
	bool bDeactivate = false;
    long lEventCode;
    long lParam1;
    long lParam2;
    HRESULT hr; 

    // Check for completion events
    hr = m_pME->GetEvent(&lEventCode, (LONG_PTR *) &lParam1, (LONG_PTR *) &lParam2, 0);
    if (SUCCEEDED(hr))
    {
        if(m_active && EC_COMPLETE == lEventCode) {
			if( m_loop ) {
				hr = m_pMP->put_CurrentPosition(0);
			}
			else {
				bDeactivate = true;
			}
		}

        // Free any memory associated with this event
        hr = m_pME->FreeEventParams(lEventCode, lParam1, lParam2);
    }
	return bDeactivate;
}

Time CAnmMovieTextureRenderer::GetDuration()
{
	REFTIME plength;

	if( m_pMP ) {
		HRESULT hr = m_pMP->get_Duration(&plength);

		if (SUCCEEDED(hr)) {
			return plength;
		}
	}
	return ANMTIME_BADTIMEVALUE;
}

 
//-----------------------------------------------------------------------------
// SetMediaType: Graph connection has been made. 
//-----------------------------------------------------------------------------
HRESULT CAnmMovieTextureRenderer::SetMediaType(const CMediaType *pmt)
{
    // Retrive the size of this media type
    VIDEOINFO *pviBmp;                      // Bitmap info header
    pviBmp = (VIDEOINFO *)pmt->Format();
    m_lVidWidth  = pviBmp->bmiHeader.biWidth;
    m_lVidHeight = abs(pviBmp->bmiHeader.biHeight);
    m_lVidByteDepth = ( pviBmp->bmiHeader.biBitCount == 32 ) ? 4 : 3;
    m_lVidPitch = (m_lVidWidth * m_lVidByteDepth + 3) & ~(3); // We are forcing RGB24 or ARGB32

    // Create the texture that maps to this media type
	CAnmGraphics *pGraphics = m_app->GetGraphics();
	assert(pGraphics);

	m_textureSurface = pGraphics->CreateTexture(m_device);

	m_blitter = CAnmMovieTextureBlitter::Create(m_device, m_textureSurface, m_lVidWidth, m_lVidHeight, m_lVidPitch, m_lVidByteDepth);

    return S_OK;
}

//-----------------------------------------------------------------------------
// DoRenderSample: A sample has been delivered. Copy it to the texture.
//-----------------------------------------------------------------------------
HRESULT CAnmMovieTextureRenderer::DoRenderSample( IMediaSample * pSample )
{
	return DoRenderBitsToSurface( pSample );
}


// krv:  We need to render these bits so we get the texture on initialization.
//
void CAnmMovieTextureRenderer::OnReceiveFirstSample(IMediaSample *pSample)
{
	DoRenderBitsToSurface( pSample );
}


HRESULT CAnmMovieTextureRenderer::DoRenderBitsToSurface( IMediaSample * pSample )
{


	if (m_textureSurface == NULL)
		return E_FAIL;

	if (m_blitter == NULL)
		return E_FAIL;

	// Copy the bitmap image to the surface
	BYTE  *pBmpBuffer;     // Bitmap buffer, texture buffer
	
	HRESULT hr;

	// Get the video bitmap buffer
	hr = pSample->GetPointer( &pBmpBuffer );

	if (!SUCCEEDED(hr))
		return E_FAIL;

	// Get a lock on the world since we're getting this from another thread
	m_world->Lock();
	hr = m_blitter->CopyBits(pBmpBuffer);
	m_world->Unlock();

	return hr;
}
  

LPDIRECTSOUND3DBUFFER CAnmMovieTextureRenderer::GetSound3DBuffer()
{
	if( 1 ||  m_bFilterGraphHasBeenRun ) {
		return m_p3DBuffer;
	}
	return NULL;	
}




void CAnmMovieTextureRenderer::ApplyInitSoundParams()
{
	if( m_InitMax > 0.0 ) {
		SetPosition( m_InitPos );
		SetOrientation( m_InitDir );
		SetMinDistance( m_InitMin );
		SetMaxDistance( m_InitMax );
		SetListenerPosition( m_InitListenerPos );
		CommitSound3DChanges();
	}
}




void CAnmMovieTextureRenderer::SetPosition(const Point3 &pos)
{
	m_InitPos = pos;
	if( m_bHasAudio ) {
		if ( GetSound3DBuffer() )
		{
			HRESULT hr = GetSound3DBuffer()->SetPosition(pos.x, pos.y, pos.z, DS3D_DEFERRED );
		}
	}
}

void CAnmMovieTextureRenderer::SetOrientation(const Point3 &dir)
{
	m_InitDir = dir;
	if( m_bHasAudio ) {
		if (GetSound3DBuffer())
		{
			HRESULT hr = GetSound3DBuffer()->SetConeOrientation(dir.x, dir.y, dir.z, DS3D_DEFERRED );
		}
	}
}

void CAnmMovieTextureRenderer::SetMinDistance(float minDistance)
{
	m_InitMin = minDistance;
	if( m_bHasAudio ) {
		if (GetSound3DBuffer())
		{
			HRESULT hr = GetSound3DBuffer()->SetMinDistance(minDistance, DS3D_DEFERRED );
		}
	}
}

void CAnmMovieTextureRenderer::SetMaxDistance(float maxDistance)
{
	m_InitMax = maxDistance;
	if( m_bHasAudio ) {
		if (GetSound3DBuffer())
		{
			HRESULT hr = GetSound3DBuffer()->SetMaxDistance(maxDistance, DS3D_DEFERRED );
		}
	}
}


void CAnmMovieTextureRenderer::CommitSound3DChanges()
{
	if( m_bHasAudio ) {
		if ( m_pListener )
		{
			HRESULT hr = m_pListener->CommitDeferredSettings();
		}
	}
}


void CAnmMovieTextureRenderer::SetListenerPosition(const Point3 &pos)
{
	m_InitListenerPos = pos;
	if( m_bHasAudio ) {
		if ( m_pListener )
		{
			HRESULT hr = m_pListener->SetPosition(pos.x, pos.y, pos.z, DS3D_DEFERRED  );
		}
	}
}

void CAnmMovieTextureRenderer::SetListenerOrientation(const Point3 &dir, const Point3 &up)
{
//	m_InitListenerPos = pos;
	if( m_bHasAudio ) {
		if ( m_pListener )
		{
			HRESULT hr = m_pListener->SetOrientation(
				dir.x, dir.y, dir.z, 
				up.x, up.y, up.z, 
				DS3D_DEFERRED  );
		}
	}
}




#define SOUND_VOLUME_MAX 0
#define SOUND_VOLUME_MIN -2000			// DSound units are dB/100; -20B is pretty quiet


void CAnmMovieTextureRenderer::SetIntensity(float intensity)
{
	if( m_bHasAudio ) {
		if( m_pBasicAudio )
		{
			// Input is intensity on a range from 0 to 1; transform to DSound values
			LONG range = SOUND_VOLUME_MAX - SOUND_VOLUME_MIN;
			LONG lIntensity = (LONG) (float) range * intensity;
			LONG lVolume = SOUND_VOLUME_MIN + lIntensity;

			m_pBasicAudio->put_Volume(lVolume);
		}
	}
}



CBaseRenderer * CAnmMovieTextureRenderer::InsertAudioSensor( IPin* pAudioRendererPinIn, CAnmAudioSensor * pRendererNode )
{

    HRESULT hr=S_OK;

	CBaseRenderer * pAudioSensorRendererOut = NULL;
	IPin* pAudioRendererPinOut;
	// Get the Pin that provides Input to the Audio Renerer.
	//
	if( SUCCEEDED( pAudioRendererPinIn->ConnectedTo( &pAudioRendererPinOut ) ) ) {
		// Disconnect the pins
		//


		// Get the Media Type
		//

		IEnumMediaTypes*     pEnumMT;
		ULONG						nFetched = 0;
		AM_MEDIA_TYPE*			pMediaTypes = NULL;
		AM_MEDIA_TYPE*			pMediaTypeText = NULL;
		if( pAudioRendererPinOut->EnumMediaTypes(&pEnumMT) == S_OK ) {
			if( pEnumMT->Next(1, &pMediaTypeText, &nFetched) == S_OK ) {
				pMediaTypes = pMediaTypeText;
			}
			pEnumMT->Release();
		}



		if( pMediaTypes && 
			SUCCEEDED( pAudioRendererPinIn->Disconnect () ) &&
			SUCCEEDED( pAudioRendererPinOut->Disconnect () ) 
			) {

			// Create the Tee filter.
			//
			CComPtr<IBaseFilter>    pAudioTee;   
			pAudioTee.CoCreateInstance(CLSID_InfTee, NULL, CLSCTX_INPROC);
			if( pAudioTee ) {
				if (FAILED(hr = m_pGB->AddFilter(pAudioTee, L"AUDIOTEE")))
				{
					Msg(m_app, TEXT("Warning: AudioTee: Could not Add FIlter to Graph Builder!  hr=0x%x"), hr);
				}
				else {

					CComPtr<IPin>           pTeePinIn;
					CComPtr<IPin>           pTeePinOut;
					if( FindInputPin( pAudioTee, pTeePinIn ) && 
						FindDisconnectedOutputPin( pAudioTee, pTeePinOut ) ) {


						if (FAILED(hr = m_pGB->ConnectDirect(pAudioRendererPinOut, pTeePinIn, pMediaTypes))) {
							Msg(m_app, TEXT("Warning: AudioRenderer: Could not connect Audio to Tee!  hr=0x%x"), hr);
						}
						else {

							if (FAILED(hr = m_pGB->ConnectDirect(pTeePinOut, pAudioRendererPinIn, pMediaTypes))) {
								Msg(m_app, TEXT("Warning: AudioRenderer: Could not connect Tee to Audio Renerer!  hr=0x%x"), hr);
							}
							else {
								CanmAudioSensorRenderer* pAudioSensorRenderer = new CanmAudioSensorRenderer( pRendererNode );

								// Find a second pin in the TEE.
								// Must be done after first pin is connected.
								CComPtr<IPin>           pTeePin2Out;
								CComPtr<IPin>           pAudioRendererPinIn;
								if( pAudioSensorRenderer &&
									FindInputPin( pAudioSensorRenderer, pAudioRendererPinIn ) && 
									FindDisconnectedOutputPin( pAudioTee, pTeePin2Out ) ) {


									if (FAILED(hr = m_pGB->AddFilter(pAudioSensorRenderer, L"AUDIOSENSORRENDERER")))
									{
										Msg(m_app, TEXT("Warning: AudioTee: Could not Add Audio Sensor Renderer to Graph Builder!  hr=0x%x"), hr);
									}
									else {

										if (FAILED(hr = m_pGB->ConnectDirect(pTeePin2Out, pAudioRendererPinIn, pMediaTypes))) {
											Msg(m_app, TEXT("Warning: AudioRenderer: Could not connect Audio to Tee!  hr=0x%x"), hr);
										}
										else {

											pAudioSensorRendererOut = pAudioSensorRenderer;
										}
									}
								}


							}
						}
		
					}
				}
			}
			DeleteMediaType( pMediaTypes );
		}
	}

	return pAudioSensorRendererOut;
}
