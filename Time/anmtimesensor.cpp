/********************************************************************************
 * Flux
 *
 * File: anmtimesensor.h
 * Description: Time Sensor class
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
#include "anmtimesensor.h"
#include "anmtime.h"
#include "anmworld.h"

CAnmTimeSensor::CAnmTimeSensor() : 
CAnmSensor(TRUE),
m_pauseTime(TIMESENSOR_DEFAULT_PAUSETIME),
m_resumeTime(TIMESENSOR_DEFAULT_RESUMETIME),
m_startTime(TIMESENSOR_DEFAULT_STARTTIME),
m_stopTime(TIMESENSOR_DEFAULT_STOPTIME),
m_cycleInterval(TIMESENSOR_DEFAULT_CYCLEINTERVAL),
m_loop(TIMESENSOR_DEFAULT_LOOP)
{
	m_prevTime = ANMTIME_BADTIMEVALUE;
	m_isActive = false;
	m_isPaused = false;
	m_totalPausedTime = 0.f;
	m_stopLoopTime = ANMTIME_BADTIMEVALUE;
}

CAnmTimeSensor::~CAnmTimeSensor()
{
}


void CAnmTimeSensor::Realize()
{
	CAnmSensor::Realize();		// just chain down for now
}

void CAnmTimeSensor::Update()
{
	ClearStateDirty();
}

void CAnmTimeSensor::Poll()
{
	assert(m_enabled);

	if (StateDirty())
		Update();

	CAnmTime *pClock = GetClock();
	if (pClock == NULL)
		return;

	Time now = pClock->GetCurrentTime();

	Time stopTime = CalcValidStopTime(now);

	// try activate/deactivate
	if (m_isActive)
	{
		if (m_isPaused)
		{
			if (now >= m_resumeTime && m_resumeTime > m_pauseTime)
			{
				m_totalPausedTime += (now - m_pauseTime);
				HandlePaused(false);
			}
		}
		else
		{
			if (now >= stopTime)
			{
				Deactivate(stopTime);
			}
			else if (now >= m_pauseTime && m_pauseTime > m_resumeTime)
			{
				HandlePaused(true);
			}
		}
	}
	else
	{

//		krv: Get rid of "Or Equal" stoptime
		// If stopTIme is routed in from the CUrrent Time, then we do NOT activate.
//		if (m_startTime <= now && now <= stopTime)
		if (m_startTime <= now && now < stopTime)
		{
			Activate(now);
		}
	}

	// still active? fire time/fraction events
	if (m_isActive && !m_isPaused)
	{	
		if (now != m_prevTime)
		{
			if (now >= stopTime)
			{
				Deactivate(now);
			}
			else
			{
				HandleTimeChanged(now);

				// try cycle time
				if (m_cycleInterval > 0.0 && m_prevTime != ANMTIME_BADTIMEVALUE)
				{
					Float fraction = CalcFraction(CalcElapsedTime(now));
					Float prevfraction = CalcFraction(CalcElapsedTime(m_prevTime));

					if (fraction < prevfraction)	// we've cycled
					{
						HandleCycleTime(now);
					}
				}
			}
		}
	}

	m_prevTime = now;
}

CAnmTime *CAnmTimeSensor::GetClock()
{
	CAnmWorld *pWorld = GetWorld();
	if (pWorld == NULL)
		return NULL;

	cApplication *pApp = pWorld->GetApp();

	if (pApp == NULL)
		return NULL;

	return pApp->GetClock();
}

void CAnmTimeSensor::Activate(Time t)
{
	// send active event
	HandleActive(true);
	
	// send initial outputs
	HandleCycleTime(t);

	// reset our total paused time
	m_totalPausedTime = 0.f;

	// reset our funky start/stop loop stop time
	m_stopLoopTime = ANMTIME_BADTIMEVALUE;
}

void CAnmTimeSensor::Deactivate(Time t)
{
	// send inactive event
	HandleActive(false);
	
	// send final outputs
	HandleTimeChanged(t);

	// reset our total paused time
	m_totalPausedTime = 0.f;

	// reset our funky start/stop loop stop time
	m_stopLoopTime = ANMTIME_BADTIMEVALUE;
}

void CAnmTimeSensor::HandleActive(Boolean activeFlag)
{
	if (m_isActive != activeFlag)
	{
		m_isActive = activeFlag;
		CallCallbacks(CALLBACKID(CAnmTimeSensor, isActive), &m_isActive);
	}
}

void CAnmTimeSensor::HandlePaused(Boolean pausedFlag)
{
	if (m_isPaused != pausedFlag)
	{
		m_isPaused = pausedFlag;
		CallCallbacks(CALLBACKID(CAnmTimeSensor, isPaused), &m_isPaused);
	}
}

void CAnmTimeSensor::HandleTimeChanged(Time t)
{
	Time elapsedTime = CalcElapsedTime(t);
	float fraction = CalcFraction(elapsedTime);

	CallCallbacks(CALLBACKID(CAnmTimeSensor, time), (void *) &t);
	CallCallbacks(CALLBACKID(CAnmTimeSensor, fraction_changed), (void *) &fraction);
	CallCallbacks(CALLBACKID(CAnmTimeSensor, elapsedTime), (void *) &elapsedTime);
}

void CAnmTimeSensor::HandleCycleTime(Time t)
{
	CallCallbacks(CALLBACKID(CAnmTimeSensor, cycleTime), (void *) &t);
}

void CAnmTimeSensor::SetEnabled(Boolean enabled)
{
	// If we're disabling the sensor and it's active, deactivate it
	if (!enabled && m_enabled && m_isActive)
	{
		CAnmTime *pClock = GetClock();
		if (pClock != NULL)
		{
			Time now = pClock->GetCurrentTime();
			Deactivate(now);
		}
	}

	// Get all kinds of good stuff from the base class
	CAnmSensor::SetEnabled(enabled);
}

void CAnmTimeSensor::SetCycleInterval(Time cycleInterval)
{
	// N.B.: can't set the cycleInterval of an already-active timesensor
	if (m_isActive)
		return;

	m_cycleInterval = cycleInterval;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmTimeSensor, cycleInterval_changed), &cycleInterval);
}

void CAnmTimeSensor::SetLoop(Boolean loop)
{
	if (loop != m_loop)
	{
		m_loop = loop;

		CAnmTime *pClock = GetClock();
		if (pClock != NULL)
		{
			Time now = pClock->GetCurrentTime();
			if (loop)
				m_stopLoopTime = ANMTIME_BADTIMEVALUE;
			else
				m_stopLoopTime = CalcStopLoopTime(now);
		}

		SetStateDirty();

		CallCallbacks(CALLBACKID(CAnmTimeSensor, loop_changed), &loop);
	}
}

void CAnmTimeSensor::SetPauseTime(Time pauseTime)
{
	// N.B.: can't set the pauseTime of an inactive timesensor
	if (!m_isActive)
		return;

	// N.B.: can't set the pauseTime of an already-paused timesensor
	if (m_isPaused)
		return;

	m_pauseTime = pauseTime;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmTimeSensor, pauseTime_changed), &pauseTime);
}

void CAnmTimeSensor::SetResumeTime(Time resumeTime)
{
	// N.B.: can't set the resumeTime of an inactive timesensor
	if (!m_isActive)
		return;

	m_resumeTime = resumeTime;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmTimeSensor, resumeTime_changed), &resumeTime);
}

void CAnmTimeSensor::SetStartTime(Time startTime)
{
	// N.B.: can't set the startTime of an already-active timesensor
	if (m_isActive)
		return;

	m_startTime = startTime;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmTimeSensor, startTime_changed), &startTime);
}

void CAnmTimeSensor::SetStopTime(Time stopTime)
{
	// N.B.: can't set the stopTime of an already-active timesensor to less than current start time
	if (m_isActive && (stopTime <= m_startTime))
		return;

	m_stopTime = stopTime;

	// This handles the case where setStopTime and setStartTime are called in the same tick
	if (m_isActive)
	{
		CAnmTime *pClock = GetClock();
		if (pClock != NULL)
		{
			Time now = pClock->GetCurrentTime();
			if (now >= stopTime)
			{
				Deactivate(stopTime);
			}
		}
	}

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmTimeSensor, stopTime_changed), &stopTime);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmTimeSensor, cycleInterval, eValueTime, Time, GetCycleInterval, SetCycleInterval);
DEFINE_INHERITED_VALUE(CAnmTimeSensor, enabled, CAnmSensor);
DEFINE_VALUE(CAnmTimeSensor, loop, eValueBoolean, Boolean, GetLoop, SetLoop);
DEFINE_VALUE(CAnmTimeSensor, pauseTime, eValueTime, Time, GetPauseTime, SetPauseTime);
DEFINE_VALUE(CAnmTimeSensor, resumeTime, eValueTime, Time, GetResumeTime, SetResumeTime);
DEFINE_VALUE(CAnmTimeSensor, startTime, eValueTime, Time, GetStartTime, SetStartTime);
DEFINE_VALUE(CAnmTimeSensor, stopTime, eValueTime, Time, GetStopTime, SetStopTime);

DEFINE_METHOD(CAnmTimeSensor, set_cycleInterval, eValueTime, Time, SetCycleInterval);
DEFINE_INHERITED_METHOD(CAnmTimeSensor, set_enabled, CAnmSensor);
DEFINE_METHOD(CAnmTimeSensor, set_loop, eValueBoolean, Boolean, SetLoop);
DEFINE_METHOD(CAnmTimeSensor, set_pauseTime, eValueTime, Time, SetPauseTime);
DEFINE_METHOD(CAnmTimeSensor, set_resumeTime, eValueTime, Time, SetResumeTime);
DEFINE_METHOD(CAnmTimeSensor, set_startTime, eValueTime, Time, SetStartTime);
DEFINE_METHOD(CAnmTimeSensor, set_stopTime, eValueTime, Time, SetStopTime);

DEFINE_CALLBACK(CAnmTimeSensor, cycleInterval_changed, eValueTime);
DEFINE_INHERITED_CALLBACK(CAnmTimeSensor, enabled_changed, CAnmSensor);
DEFINE_CALLBACK(CAnmTimeSensor, loop_changed, eValueBoolean);
DEFINE_CALLBACK(CAnmTimeSensor, pauseTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmTimeSensor, resumeTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmTimeSensor, startTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmTimeSensor, stopTime_changed, eValueTime);
DEFINE_CALLBACK(CAnmTimeSensor, cycleTime, eValueTime);
DEFINE_CALLBACK(CAnmTimeSensor, elapsedTime, eValueTime);
DEFINE_CALLBACK(CAnmTimeSensor, fraction_changed, eValueFloat);
DEFINE_CALLBACK(CAnmTimeSensor, isActive, eValueBoolean);
DEFINE_CALLBACK(CAnmTimeSensor, isPaused, eValueBoolean);
DEFINE_CALLBACK(CAnmTimeSensor, time, eValueTime);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmTimeSensor)
VALUEID(CAnmTimeSensor, cycleInterval),
VALUEID(CAnmTimeSensor, enabled),
VALUEID(CAnmTimeSensor, loop),
VALUEID(CAnmTimeSensor, pauseTime),
VALUEID(CAnmTimeSensor, resumeTime),
VALUEID(CAnmTimeSensor, startTime),
VALUEID(CAnmTimeSensor, stopTime),

METHODID(CAnmTimeSensor, set_cycleInterval),
METHODID(CAnmTimeSensor, set_enabled),
METHODID(CAnmTimeSensor, set_loop),
METHODID(CAnmTimeSensor, set_pauseTime),
METHODID(CAnmTimeSensor, set_resumeTime),
METHODID(CAnmTimeSensor, set_startTime),
METHODID(CAnmTimeSensor, set_stopTime),

CALLBACKID(CAnmTimeSensor, enabled_changed),
CALLBACKID(CAnmTimeSensor, cycleInterval_changed),
CALLBACKID(CAnmTimeSensor, loop_changed),
CALLBACKID(CAnmTimeSensor, pauseTime_changed),
CALLBACKID(CAnmTimeSensor, resumeTime_changed),
CALLBACKID(CAnmTimeSensor, startTime_changed),
CALLBACKID(CAnmTimeSensor, stopTime_changed),
CALLBACKID(CAnmTimeSensor, cycleTime),
CALLBACKID(CAnmTimeSensor, elapsedTime),
CALLBACKID(CAnmTimeSensor, fraction_changed),
CALLBACKID(CAnmTimeSensor, isActive),
CALLBACKID(CAnmTimeSensor, isPaused),
CALLBACKID(CAnmTimeSensor, time),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmTimeSensor, CAnmSensor);
