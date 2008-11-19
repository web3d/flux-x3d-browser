/********************************************************************************
 * Flux
 *
 * File: anmmediasourceobject.h
 * Description: MediaSourceObject base class
 *							Inherited by any sound, video or other
 *							media playback type
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

#ifndef _anmmediasourceobject_h
#define _anmmediasourceobject_h

#include "anmtimedependentobject.h"

class CAnmMediaSourceObject : public CAnmTimeDependentObject
{
protected:

	class CAnmMediaSensor		*m_mediaSensor;

	std::list<class CAnmAudioSensor *>			m_RendererList;

	virtual void ConnectSensor(class CAnmNode *pNode);

public:

	// constructor/destructor
	CAnmMediaSourceObject();
	~CAnmMediaSourceObject();

	virtual void NotifyRendererDestroyed() PURE;

	// krv should be abstract class MediaRenderer
	//
	void AddRenderer( class CAnmAudioSensor* pAudioRenderer );
	void RemoveRenderer( class CAnmAudioSensor* pAudioRenderer );

	// Interface glue
	ANMDECLARE_INTERFACE(CAnmMediaSourceObject);

};

#endif // _anmmediasourceobject_h

