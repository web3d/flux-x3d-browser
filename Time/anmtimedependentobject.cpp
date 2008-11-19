/********************************************************************************
 * Flux
 *
 * File: anmtimedependentobject.cpp
 * Description: TimeDependentObject base class
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
#include "anmtimedependentobject.h"
#include "anmtime.h"

CAnmTimeDependentObject::CAnmTimeDependentObject()
{
	m_loop = false;
	m_isActive = false;
	m_startTime = 0;
	m_stopTime = 0;
	m_duration = m_newDuration = ANMTIME_BADTIMEVALUE;
	m_prevTime = ANMTIME_BADTIMEVALUE;
	m_clock = NULL;

	m_resumeTime = 0;
	m_pauseTime = 0;
	m_isPaused = FALSE;
	m_elapsedTime = 0;

	m_isPausedTime = 0;
	m_isActiveTime = 0;
	m_totalPausedTimeSinceActivation = 0;
	m_bBeenInited = false;

} 

void CAnmTimeDependentObject::Update()
{
	if (m_stopTime < m_startTime)
	{
		m_stopTime = ANMTIME_MAXTIME;
	}
	if (m_resumeTime < m_pauseTime)
	{
		m_resumeTime = ANMTIME_MAXTIME;
	}

}

void CAnmTimeDependentObject::Poll()
{
	assert(m_clock);

	Time newTime = m_clock->GetCurrentTime();

	if (m_newDuration != m_duration)
	{
		m_duration = m_newDuration;
		HandleDurationChanged(m_newDuration);
	}

	Time stopTime  = CalcValidStopTime();

	// try paused / resume
	if(m_isPaused) 
	{
		if (newTime >= m_resumeTime && 
			m_resumeTime > m_pauseTime )
		{
			HandleResume();
			HandlePaused(false);

			m_isPaused = false;
			m_totalPausedTimeSinceActivation += ( newTime - m_isPausedTime );
		}
	}
	// try activate/deactivate
	else if (m_isActive)
	{
		// Test Stop
		if (newTime >= stopTime)
		{
			HandleStop();
			HandleActive(false);

			m_isActive = false;
		}
		// Test Pause
		else if (newTime >= m_pauseTime && 
					m_pauseTime > m_resumeTime  )
		{
			HandlePause();
			HandlePaused(true);

			m_isPaused = true;
			m_isPausedTime = newTime;
		}
	}
	else
	{
		// Test Start Condition.
		if ( newTime >= m_startTime && newTime < stopTime )
		{
			HandleStart(newTime-m_startTime);
			HandleActive(true);

			m_isActive = true;
			m_isActiveTime = newTime;
			m_totalPausedTimeSinceActivation = 0;
		}
	}


	if (m_isActive)
	{
		if (m_prevTime == ANMTIME_BADTIMEVALUE ||
			m_prevTime != newTime)
		{
			HandleTimeChanged(newTime);
		}

		// If we are active, and not paused, the calc the elapsedTime to send event.
		//
		if( !m_isPaused ) {
			Time elapsedTime = ( newTime - m_isActiveTime ) - m_totalPausedTimeSinceActivation;
			if( elapsedTime >= 0.0 &&
				elapsedTime != m_elapsedTime ) {
				m_elapsedTime = elapsedTime;
				HandleElapsedTimeChanged(m_elapsedTime);
			}
		}	    
	}
	else if( !m_bBeenInited ) {
		// If on the first poll, we did not init activate the stream,
		// then we need to init the stream so that the first video frame will
		// display, and start the steam streaming.
		HandleInit();
	}
	m_bBeenInited = true;



	m_prevTime = newTime;
}
