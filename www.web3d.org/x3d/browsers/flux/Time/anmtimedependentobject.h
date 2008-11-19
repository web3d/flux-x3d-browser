/********************************************************************************
 * Flux
 *
 * File: anmtimedependentobject.h
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

#ifndef _anmtimedependentobject_h
#define _anmtimedependentobject_h

class CAnmTimeDependentObject
{
protected:

	Boolean				 m_loop;
	Time				 m_startTime;
	Time				 m_stopTime;
	Boolean				 m_isActive;

	Time				 m_resumeTime;
	Time				 m_pauseTime;
	Boolean				 m_isPaused;

	Time				 m_elapsedTime;

	Time				 m_duration;
	Time				 m_newDuration;


	Time				 m_isPausedTime;
	Time				 m_isActiveTime;
	Time				 m_totalPausedTimeSinceActivation;
	Time				 m_prevTime;


	bool				 m_bBeenInited;

	class CAnmTime		*m_clock;

	// Handlers - subclass *must* override
	virtual void HandleStart(Time startTime) PURE;
	virtual void HandleStop() PURE;
	virtual void HandleActive(Boolean isActive) PURE;
	virtual void HandleResume() PURE;
	virtual void HandlePause() PURE;
	virtual void HandlePaused(Boolean isPaused) PURE;
	virtual void HandleDurationChanged(Time newDuration) PURE;
	virtual void HandleTimeChanged(Time newtime) PURE;
	virtual void HandleElapsedTimeChanged(Time elapsedTime) PURE;
	virtual void HandleInit() PURE;

	// Setup methods - sublass *must* call these for setup
	virtual void SetClock(class CAnmTime *pClock)
	{
		m_clock = pClock;
	}

	virtual void SetDuration(Time duration)
	{
		m_newDuration = duration;
	}

	// Helpers
	Boolean IsValidStopTime();
	Time CalcValidStopTime();
	Boolean IsValidResumeTime();
	Time CalcValidResumeTime();

public:

	// constructor/destructor
	CAnmTimeDependentObject();

	// New methods
	// N.B.: these are NOT sensor node class overrides, but rather helpers
	virtual void Update();
	virtual void Poll();

	// Accessors
	virtual void SetLoop(Boolean loop)
	{
		m_loop = loop;
	}
	virtual Boolean GetLoop()
	{ 
		return m_loop; 
	}
	virtual void GetLoop(Boolean *pLoop)
	{
		if (pLoop)
			*pLoop = m_loop;
	}

	virtual void SetStartTime(Time startTime)
	{
		m_startTime = startTime;
	}
	virtual Time GetStartTime() 
	{ 
		return m_startTime; 
	}
	virtual void GetStartTime(Time *pTime)
	{
		if (pTime)
			*pTime = m_startTime;
	}

	virtual void SetStopTime(Time stopTime)
	{
		m_stopTime = stopTime;
	}	
	virtual Time GetStopTime()
	{
		return m_stopTime;
	}
	virtual void GetStopTime(Time *pTime)
	{
		if (pTime)
			*pTime = m_stopTime;
	}


	virtual void SetResumeTime(Time resumeTime)
	{
		m_resumeTime = resumeTime;
	}
	virtual Time GetResumeTime() 
	{ 
		return m_resumeTime; 
	}
	virtual void GetResumeTime(Time *pTime)
	{
		if (pTime)
			*pTime = m_resumeTime;
	}

	virtual void SetPauseTime(Time pauseTime)
	{
		m_pauseTime = pauseTime;
	}
	virtual Time GetPauseTime() 
	{ 
		return m_pauseTime; 
	}
	virtual void GetPauseTime(Time *pTime)
	{
		if (pTime)
			*pTime = m_pauseTime;
	}



};

inline Boolean CAnmTimeDependentObject::IsValidStopTime()
{
	return (m_stopTime > m_startTime && m_stopTime < ANMTIME_MAXTIME);
}

inline Time CAnmTimeDependentObject::CalcValidStopTime()
{
	// we'll start with forever as the stop time
	Time stopTime = ANMTIME_MAXTIME;

	// if stopTime is greater than start, use that
	if (IsValidStopTime()) {
		stopTime = m_stopTime;
	}


	// if we have a good duration, and we're not looping, use that
	if (!m_loop && 
			m_duration != ANMTIME_BADTIMEVALUE &&
			stopTime > m_startTime + m_duration ) {
		stopTime = m_startTime + m_duration;
	}

	return stopTime;
}






inline Boolean CAnmTimeDependentObject::IsValidResumeTime()
{
	return (m_resumeTime > m_pauseTime && m_resumeTime < ANMTIME_MAXTIME);
}

inline Time CAnmTimeDependentObject::CalcValidResumeTime()
{
	// we'll resume with forever as the pause time
	Time resumeTime = ANMTIME_MAXTIME;

	// if pauseTime is greater than resume, use that
	if (IsValidResumeTime())
		resumeTime = m_resumeTime;

	return resumeTime;
}


#endif // _anmtimedependentobject_h

