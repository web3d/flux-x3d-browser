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

#ifndef _anmtimesensor_h
#define _anmtimesensor_h

#define TIMESENSOR_DEFAULT_CYCLEINTERVAL	1.0f
#define TIMESENSOR_DEFAULT_ENABLED			TRUE
#define TIMESENSOR_DEFAULT_LOOP				FALSE
#define TIMESENSOR_DEFAULT_PAUSETIME		0.f
#define TIMESENSOR_DEFAULT_RESUMETIME		0.f
#define TIMESENSOR_DEFAULT_STARTTIME		0.f
#define TIMESENSOR_DEFAULT_STOPTIME			0.f


#include "anmsensor.h"

class CAnmTimeSensor : public CAnmSensor
{

protected:

	Time			m_prevTime;
	Boolean			m_isActive;
	Boolean			m_isPaused;

	Time			m_cycleInterval;
	Boolean			m_loop;
	Time			m_pauseTime;
	Time			m_resumeTime;
	Time			m_startTime;
	Time			m_stopTime;

	Time			m_totalPausedTime;
	Time			m_stopLoopTime;

	// Helpers
	class CAnmTime	*GetClock();
	Time CalcElapsedTime(Time t);
	Time CalcStopLoopTime(Time t);
	Time CalcValidStopTime(Time t);
	Float CalcFraction(Time delta);

	void Activate(Time t);
	void Deactivate(Time t);
	void HandleActive(Boolean activeFlag);
	void HandlePaused(Boolean pausedFlag);
	void HandleTimeChanged(Time t);
	void HandleCycleTime(Time t);

public:

	// constructor/destructor
	CAnmTimeSensor();
	virtual ~CAnmTimeSensor();

	// CAnmSensor overrides
	virtual void Realize();
	virtual void Update();
	virtual void Poll();
	virtual void SetEnabled(Boolean enabled);

	// New methods
	
	// Accessors
	virtual void SetCycleInterval(Time cycleInterval);
	virtual Time GetCycleInterval() { return m_cycleInterval; }
	virtual void GetCycleInterval(Time *pTime)
	{
		if (pTime)
			*pTime = m_cycleInterval;
	}

	virtual void SetLoop(Boolean loop);
	virtual Boolean GetLoop() { return m_loop; }
	virtual void GetLoop(Boolean *pLoop)
	{
		if (pLoop)
			*pLoop = m_loop;
	}

	virtual void SetPauseTime(Time pauseTime);
	virtual Time GetPauseTime() { return m_pauseTime; }
	virtual void GetPauseTime(Time *pTime)
	{
		if (pTime)
			*pTime = m_pauseTime;
	}

	virtual void SetResumeTime(Time resumeTime);
	virtual Time GetResumeTime() { return m_resumeTime; }
	virtual void GetResumeTime(Time *pTime)
	{
		if (pTime)
			*pTime = m_resumeTime;
	}

	virtual void SetStartTime(Time startTime);
	virtual Time GetStartTime() { return m_startTime; }
	virtual void GetStartTime(Time *pTime)
	{
		if (pTime)
			*pTime = m_startTime;
	}

	virtual void SetStopTime(Time stopTime);
	virtual Time GetStopTime() { return m_stopTime; }
	virtual void GetStopTime(Time *pTime)
	{
		if (pTime)
			*pTime = m_stopTime;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmTimeSensor);

	DECLARE_VALUE(cycleInterval);
	DECLARE_VALUE(enabled);
	DECLARE_VALUE(loop);
	DECLARE_VALUE(pauseTime);
	DECLARE_VALUE(resumeTime);
	DECLARE_VALUE(startTime);
	DECLARE_VALUE(stopTime);

	DECLARE_METHOD(set_cycleInterval);
	DECLARE_METHOD(set_enabled);
	DECLARE_METHOD(set_loop);
	DECLARE_METHOD(set_pauseTime);
	DECLARE_METHOD(set_resumeTime);
	DECLARE_METHOD(set_startTime);
	DECLARE_METHOD(set_stopTime);

	DECLARE_CALLBACK(cycleInterval_changed);
	DECLARE_CALLBACK(enabled_changed);
	DECLARE_CALLBACK(loop_changed);
	DECLARE_CALLBACK(pauseTime_changed);
	DECLARE_CALLBACK(resumeTime_changed);
	DECLARE_CALLBACK(startTime_changed);
	DECLARE_CALLBACK(stopTime_changed);
	DECLARE_CALLBACK(cycleTime);
	DECLARE_CALLBACK(elapsedTime);
	DECLARE_CALLBACK(fraction_changed);
	DECLARE_CALLBACK(isActive);
	DECLARE_CALLBACK(isPaused);
	DECLARE_CALLBACK(time);

};

inline Time CAnmTimeSensor::CalcElapsedTime(Time t)
{
	return t - m_startTime - m_totalPausedTime;
}

inline Time CAnmTimeSensor::CalcStopLoopTime(Time t)
{
	Time elapsedTime = CalcElapsedTime(t);

	int nCyclesPassed = elapsedTime / m_cycleInterval;
	Time stopLoopTime = m_startTime + m_totalPausedTime + 
		((Time) (nCyclesPassed + 1) * m_cycleInterval);

	return stopLoopTime;
}

inline Time CAnmTimeSensor::CalcValidStopTime(Time t)
{
	// we'll start with forever as the stop time
	Time stopTime = ANMTIME_MAXTIME;

	// if t is valid, use that
	if (m_stopTime > m_startTime && m_stopTime < ANMTIME_MAXTIME)
		stopTime = m_stopTime;
	else
	{
		// if we have a good cycle interval, and we're not looping, use that
		if (!m_loop && m_cycleInterval > 0.0)
		{
			if (m_stopLoopTime != ANMTIME_BADTIMEVALUE)
			{
				stopTime = m_stopLoopTime;
			}
			else
			{
				stopTime = m_startTime + m_totalPausedTime + m_cycleInterval;
			}
		}
	}

	return stopTime;
}

inline Float CAnmTimeSensor::CalcFraction(Time delta)
{
	Float fraction;

	if (m_cycleInterval <= 0)
		fraction = 1.f;				// guard against bogus cycleInterval values
	else
	{
		Time mdelta = fmod(delta, m_cycleInterval);

		// at the boundary, use 1 fractional values after the initial 0
		
		// krv: 
		// The final frame is getting set back to fraction=0, because there is a tiny
		// mdelta.  Seems that something is setting the mdelta eqaul to the cycletime plus
		// a very small residual significance on the final frame of an interpoaltor.
//		if (mdelta == 0.f && delta > 0.f)
		if (mdelta < 0.00001 && delta > 0.f)
			fraction = 1.f;
		else
			fraction = (Float) mdelta / m_cycleInterval;
	}

	return fraction;
}
	
#endif // _anmtimesensor_h

