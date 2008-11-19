/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: Sound.h
 *    Desc: Class wrapper for a WAV sound.  Uses the supplied
 *          DirectX code to read WAV files
 * copyright (c) 1999 by Adrian Perez
 * See license.txt for modification and distribution information
 ******************************************************************/

#ifndef _SOUND_H
#define _SOUND_H




#include <map>

#include "SoundLayer.h"
#include "Wavread.h"



class cSound  
{
	class cApplication		*m_pApp;
	CWaveSoundRead*			 m_pWaveSoundRead;
	LPDIRECTSOUNDBUFFER		 m_pBuffer;
	LPDIRECTSOUND3DBUFFER	 m_p3DBuffer;
	int						 m_bufferSize;

	/**
	 * Multiple sounds that use the same
	 * file shouldn't re-read it, they should
	 * share the CWSR object.  This map
	 * implements rudimentary reference counting.
	 * I would have just changed CWaveSoundRead,
	 * but I wanted to keep it unchanged from the
	 * samples.
	 */
	static std::map< CWaveSoundRead*, int > m_waveMap;

	void Init();

public:
	cSound( cApplication *pApp, char* filename );
	cSound( cSound& in );
	cSound& operator=( const cSound &in );

	virtual ~cSound();

	void Restore();
	void Fill();
	void Play( bool bLoop, Time startTime );
	void Resume( bool bLoop );

	bool IsPlaying();

	void Stop();

	Time GetDuration();
	void SetIntensity(float intensity);

	void SetPosition(const Point3 &pos);
	void SetOrientation(const Point3 &dir);
	void SetMinDistance(float minDistance);
	void SetMaxDistance(float maxDistance);
	void SetListenerPosition(const Point3 &pos);
	void SetListenerOrientation(const Point3 &dir, const Point3 &up );


	static void CommitChanges();
};

#endif //_SOUND_H
