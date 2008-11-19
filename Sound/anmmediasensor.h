/********************************************************************************
 * Flux
 *
 * File: anmmediasensor.h
 * Description: MediaSensor class
 *							Utility class to help with integrating
 *							Internal time base with external media
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

#ifndef _anmmediasensor_h
#define _anmmediasensor_h

#include "anmsensor.h"
#include "anmmediasourceobject.h"

class CAnmMediaSensor : public CAnmSensor
{

protected:

	class CAnmMediaSourceObject		*m_mediaSource;

public:

	// constructor/destructor
	CAnmMediaSensor(class CAnmMediaSourceObject *pMediaSource)
		: CAnmSensor()
	{
		m_mediaSource = pMediaSource;
	}

	virtual ~CAnmMediaSensor()
	{
	}

	// CAnmSensor overrides
	virtual void Poll()
	{
		assert(m_mediaSource);
		m_mediaSource->Poll();
	}
	
	// Accessors
	virtual void SetMediaSource(class CAnmMediaSourceObject *pMediaSource)
	{
		m_mediaSource = pMediaSource;
	}

	virtual class CAnmMediaSourceObject *GetMediaSource()
	{
		return m_mediaSource;
	}
};

	
#endif // _anmmediasensor_h

