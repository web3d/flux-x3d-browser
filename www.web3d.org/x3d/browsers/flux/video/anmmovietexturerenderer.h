/********************************************************************************
 * Flux
 *
 * File: anmmovietexturerenderer.h
 * Description: Movie texture renderer for Direct Show
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

#ifndef _anmmovietexturerenderer_h
#define _anmmovietexturerenderer_h

#include <streams.h>
#include <dsound.h>

#include "anmobject.h"
#include "anmexternalresource.h"

// N.B.: windows-specific

class CAnmMovieTextureBlitter
{
protected:

	class CAnmDevice					*m_device;
	CAnmGraphics::Texture				*m_textureSurface;
	LONG								 m_width;
	LONG								 m_height;
	LONG								 m_pitch;
	LONG								 m_byteDepth;

	// Constructor - protected
    CAnmMovieTextureBlitter(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface, 
		LONG width, LONG height, LONG pitch, LONG byteDepth)
	{
		m_device = pDevice;
		m_textureSurface = pTextureSurface;
		m_width = width;
		m_height = height;
		m_pitch = pitch;
		m_byteDepth = byteDepth;
	}

public:

	// Factory
	static CAnmMovieTextureBlitter *Create(class CAnmDevice *pDevice, CAnmGraphics::Texture *pTextureSurface,
		LONG width, LONG height, LONG pitch, LONG byteDepth );

	// Destructor
    virtual ~CAnmMovieTextureBlitter()
	{
	}

	// These next are abstract and MUST be overriden by a subclass
	virtual HRESULT CopyBits(BYTE  *pBmpBuffer) PURE;
};

//-----------------------------------------------------------------------------
// Define GUID for Texture Renderer
// {71771540-2017-11cf-AE26-0020AFD79767}
//-----------------------------------------------------------------------------
struct __declspec(uuid("{B8979AC3-42C0-4f4e-9453-41E1FACB17C3}")) CLSID_FluxMovieTextureRenderer;

class CAnmMovieTextureRenderer : public CBaseVideoRenderer, public CAnmObject,
	public CAnmExternalResource
{
protected:

	LONG								 m_lVidWidth;	// Video width
	LONG								 m_lVidHeight;	// Video Height
	LONG								 m_lVidPitch;	// Video Pitch
	LONG								 m_lVidByteDepth;	// Bytes Per Pixel.  Either 3 or 4

	CComPtr<IGraphBuilder>				 m_pGB;          // GraphBuilder
	CComPtr<IMediaControl>				 m_pMC;          // Media Control
	CComPtr<IMediaPosition>				 m_pMP;          // Media Postion
	CComPtr<IMediaEvent>				 m_pME;          // Media Event


	class CAnmDevice					*m_device;
	class CAnmWorld						*m_world;
	class cApplication					*m_app;
	class CAnmGraphics::Texture			*m_textureSurface;
	class CAnmMediaSourceObject			*m_mediasource;
	CAnmMovieTextureBlitter				*m_blitter;


	bool								m_bHasVideo;
	bool								m_bHasAudio;

	bool								m_loop;
	bool								m_active;

	LPDIRECTSOUND3DBUFFER				m_p3DBuffer;
	LPDIRECTSOUND3DLISTENER				m_pListener;
	IBasicAudio*						m_pBasicAudio;

	bool								m_bFilterGraphHasBeenRun;


	Point3								m_InitPos;
	Point3								m_InitDir;
	Point3								m_InitListenerPos;
	float								m_InitMin;
	float								m_InitMax;


	// Helpers
	void Deactivate();
	HRESULT DoRenderBitsToSurface( IMediaSample * pSample );
	bool IsStreamFlushing();

	void ApplyInitSoundParams();

	CBaseRenderer * CAnmMovieTextureRenderer::InsertAudioSensor( IPin* pAudioRendererPinIn, class CAnmAudioSensor * pRendererNode );

public:

	// Constructor/destructor
    CAnmMovieTextureRenderer(class CAnmMediaSourceObject *pMediaSource,
		class CAnmDevice *pDevice, class CAnmWorld *pWorld, 
		String filename, bool bTryAudio, bool bTryVideo,
			 std::list<class CAnmAudioSensor *>& rendererList);
    virtual ~CAnmMovieTextureRenderer();

	// Methods
    HRESULT CheckMediaType(const CMediaType *pmt );     // Format acceptable?
    HRESULT SetMediaType(const CMediaType *pmt );       // Video format notification
    HRESULT DoRenderSample(IMediaSample *pMediaSample); // New video sample
    void OnReceiveFirstSample(IMediaSample *pMediaSample);
	bool IsStreamPlaying();
	bool IsStreamPaused();

   	bool Continue();
	Time GetDuration();

	void PlayStream( bool loop, Time startTime );
	void PauseStream();
	void ResumeStream();
	void InitStream( );

	// Factory
	static CAnmMovieTextureRenderer *Create(class CAnmMediaSourceObject *pMediaSource,
		class CAnmDevice *pDevice, class CAnmWorld *pWorld,
		String filename, bool bDoAudio, bool bDoVideo, 
		 std::list<class CAnmAudioSensor *>& rendererList );

	// CAnmExternalResource overrides
	virtual void FreeResource();

	// Accessors
	class CAnmGraphics::Texture *GetTextureSurface()
	{
		return m_textureSurface;
	}

	bool HasAudio(){ return m_bHasAudio; }
	bool HasVideo(){ return m_bHasVideo; }
	bool HasVideoOrAudio(){ return ( m_bHasAudio || m_bHasVideo ); }
	LPDIRECTSOUND3DBUFFER GetSound3DBuffer();
	LPDIRECTSOUNDBUFFER GetSoundBuffer();

	void CAnmMovieTextureRenderer::CommitSound3DChanges();

	void SetIntensity(float intensity);

	void SetPosition(const Point3 &pos);
	void SetOrientation(const Point3 &dir);
	void SetMinDistance(float minDistance);
	void SetMaxDistance(float maxDistance);
	void SetListenerPosition(const Point3 &pos);
	void SetListenerOrientation(const Point3 &dir, const Point3 &up);

	



};

#endif // _anmmovietexturerenderer_h
