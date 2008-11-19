/********************************************************************************
 * Flux
 *
 * File: anmtime.cpp
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

#include "stdafx.h"

#include "anmtime.h"

/**
* Excerpted from Perez/Royer--
	* Timer code based off of an article at:
	* http://www.peccavi.com/developers/
	* by Robert Dunlop
*/

CAnmTime::CAnmTime()
{
	// Check for QPC support
	LONGLONG perfCnt;
	if( QueryPerformanceFrequency( (LARGE_INTEGER*) &perfCnt) ) 
	{ 
		// Yes, we got QPC
		m_performanceFlag = true; 

		m_timeFactor = 1.0 / perfCnt;			// perf counter returns ticks per second
												// reciprocate
	} 	
	else 
	{ 
		m_performanceFlag = false; 
		m_timeFactor = 0.001;					// timeGetTime returns milliseconds
												// reciprocate
	}
	
	time(&m_cStartTime);
	m_cpuStartTime = GetCpuTime();
}

ANMTIME CAnmTime::GetCpuTime()
{
	LONGLONG sysTime;

	// Read the timer
	if( m_performanceFlag )
	{
		QueryPerformanceCounter( (LARGE_INTEGER*) &sysTime );
	}
	else
	{
		sysTime = timeGetTime(); 
	}
		
	// scale it
	return sysTime * m_timeFactor;
}

ANMTIME CAnmTime::GetNewTime()
{
	ANMTIME cpuTime, curTime;

	// get current cpu time
	cpuTime = GetCpuTime();

	// current time (UTC) is starting ctime plus elapsed Msecs
	curTime = cpuTime - m_cpuStartTime + m_cStartTime;

	// save it; it's the new time
	SetCurrentTime(curTime);

	// return it
	return curTime;
}
