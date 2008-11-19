/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: SoundLayer.h
 *    Desc: Wrapper for DirectSound
 *          Based off of the SDK samples
 * copyright (c) 1999 by Adrian Perez
 * See license.txt for modification and distribution information
 ******************************************************************/

#ifndef _SOUNDLAYER_H
#define _SOUNDLAYER_H


#include <dsound.h>


class cSoundLayer  
{

	class cApplication	*m_pApp;
	LPDIRECTSOUND		m_pDSound;
	LPDIRECTSOUNDBUFFER	m_pPrimary; // primary mixer
	LPDIRECTSOUND3DLISTENER	 m_pListener;		// listener location etc.

	cSoundLayer( class cApplication *pApp, HWND hWnd );

public:
	virtual ~cSoundLayer();


	LPDIRECTSOUND GetDSound()
	{
		return m_pDSound;
	}

	static void Create( class cApplication *pApp, HWND hWnd )
	{
		new cSoundLayer( pApp, hWnd );
	}

	void CommitChanges();

	void SetListenerPosition(const Point3 &pos);
	void SetListenerOrientation(const Point3 &dir, const Point3 &up );

};

#endif //_SOUNDLAYER_H
