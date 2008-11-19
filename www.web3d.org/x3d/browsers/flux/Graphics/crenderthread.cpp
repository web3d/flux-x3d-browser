/********************************************************************************
 * Flux
 *
 * File: cRenderThread.cpp
 * Description: Worker thread to drive rendering/simulation
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

#include "cRenderThread.h"
#include "..\input\InputLayer.h"

cRenderThread::cRenderThread(cApplication *app) : cThread()
{
	m_app = app;
	m_simState = eAnmSimGotFocus;
	m_sleepTime  = CalcSleepTime(m_simState);
}


cRenderThread::~cRenderThread()
{
}

DWORD cRenderThread::ThreadProc()
{
	int isrunning;
	DWORD sleepTime;

	while (TRUE)
	{
		
		Lock();
		isrunning = IsRunning();
		Unlock();

		if (!isrunning)
		{
			break;
		}

		try
		{
			m_app->Simulate();
		}
		catch (CAnmError &err)
		{
			eAnmFailureModeOptions failuremode = (eAnmFailureModeOptions)
				m_app->GetOptions(eAnmOptionsFailureMode);

			if (failuremode == eAnmFailureModeVerbose)
			{
				::MessageBox(NULL, err.GetText(), "Flux - Fatal Error", 
					MB_OK|MB_ICONEXCLAMATION);
			}

			throw err;
		}
#ifndef DEBUG
		catch (...)
		{
			eAnmFailureModeOptions failuremode = (eAnmFailureModeOptions)
				m_app->GetOptions(eAnmOptionsFailureMode);

			if (failuremode == eAnmFailureModeVerbose)
			{
				::MessageBox(NULL, "Unknown error in rendering system thread", "Flux - Fatal Error", 
					MB_OK|MB_ICONEXCLAMATION);
			}

			throw 0L;
		}
#endif

		Lock();
		isrunning = IsRunning();
		Unlock();

		if (!isrunning)
		{
			break;
		}

		// Poll here for iconic. Annoying but it's one way to avoid hooking IE
		// or other container window
		bool iconic = false;

		HWND hTopLevel = m_app->GetTopLevelHWnd();

		if (hTopLevel)
			iconic = ::IsIconic(hTopLevel);

		m_sleepTime = CalcSleepTime(iconic ? eAnmSimMinimized : m_simState);

		sleepTime = m_sleepTime;

		Lock();
		isrunning = IsRunning();
		Unlock();

		if (!isrunning)
		{
			break;
		}

		Sleep(sleepTime);
	}

	return 0;
}

void cRenderThread::SetSimState(eAnmSimState state)
{
	Lock();

	m_simState = state;
	m_sleepTime = CalcSleepTime(state);

	Unlock();
}
