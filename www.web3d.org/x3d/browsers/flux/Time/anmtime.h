/********************************************************************************
 * Flux
 *
 * File: anmtime.h
 * Description: Time class
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

#ifndef _anmtime_h
#define _anmtime_h

#include <time.h>

typedef double ANMTIME;
#define ANMTIME_BADTIMEVALUE ((ANMTIME) -1.0)		// N.B.: this should be NaN
#define ANMTIME_MAXTIME	DBL_MAX

class CAnmTime
{

protected:

	// Timer variables
	bool m_performanceFlag;
	double m_timeFactor;
	time_t m_cStartTime;
	ANMTIME m_cpuStartTime;
	ANMTIME m_currentTime;

	ANMTIME GetCpuTime();

public:

	// constructor/destructor
	CAnmTime();
	virtual ~CAnmTime()
	{
	}

	// Methods
	ANMTIME GetNewTime();
	
	// Accessors
	virtual void SetCurrentTime(ANMTIME curTime)
	{
		m_currentTime = curTime;
	}

	virtual ANMTIME GetCurrentTime()
	{
		return m_currentTime;
	}
};

#endif // _anmtime_h
