/********************************************************************************
 * Flux
 *
 * File: anmmediasourceobject.cpp
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


#include "stdafx.h"
#include "anmmediasourceobject.h"
#include "anmmediasensor.h"

CAnmMediaSourceObject::CAnmMediaSourceObject()
	: CAnmTimeDependentObject()
{
	m_mediaSensor = NULL;
} 

CAnmMediaSourceObject::~CAnmMediaSourceObject()
{
	SafeUnRef(m_mediaSensor);
}

void CAnmMediaSourceObject::ConnectSensor(CAnmNode *pNode)
{
	SafeUnRef(m_mediaSensor);

	m_mediaSensor = new CAnmMediaSensor(this);

	m_mediaSensor->SetOwner(pNode);
	m_mediaSensor->Realize();
}


void CAnmMediaSourceObject::AddRenderer( class CAnmAudioSensor* pAudioRenderer )
{
	// krv:: 
	// NEED to set a dirty bit to rebuild direct show filter graph renderer, or insert new renderer.
	//
	m_RendererList.push_back( pAudioRenderer );
}

void CAnmMediaSourceObject::RemoveRenderer( class CAnmAudioSensor* pAudioRenderer )
{
	// krv:: 
	// NEED to set a dirty bit to rebuild direct show filter graph renderer, or insert new renderer.
	//
	m_RendererList.remove( pAudioRenderer );
}




// Interface glue
ANMDEFINE_INTERFACE(CAnmMediaSourceObject)