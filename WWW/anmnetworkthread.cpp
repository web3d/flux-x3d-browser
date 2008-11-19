/********************************************************************************
 * Flux
 *
 * File: anmnetworkthread.cpp
 * Description: Worker thread to load files, urls etc.
 *						Uses file manager class to do loading
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

#include "anmnetworkthread.h"
#include "anmfilemanager.h"

CAnmNetworkThread::CAnmNetworkThread(cApplication *app) : cThread()
{
	m_app = app;
}


CAnmNetworkThread::~CAnmNetworkThread()
{
}

void CAnmNetworkThread::Begin()
{
	cThread::Begin(eAnmThreadPriorityNormal, TRUE);
}


DWORD CAnmNetworkThread::ThreadProc()
{
	int isrunning;

	while (TRUE)
	{
		
		Lock();
		isrunning = IsRunning();
		Unlock();

		if (!isrunning)
			break;

		try {

			// m_app->GetFileManager()->Lock();
			if (m_app->GetFileManager()->LoadRequestsPending())
			{
				m_app->GetFileManager()->ProcessLoadRequests();
			}
			// m_app->GetFileManager()->Unlock();
		}
		catch (CAnmError &err)
		{
			//::MessageBox(NULL, err.GetText(), "Flux - Fatal Error", 
			//	MB_OK|MB_ICONEXCLAMATION);

			// throw err;
			break;
		}
#ifndef DEBUG
		catch (...)
		{
			//::MessageBox(NULL, "Unknown error in network system thread", "Flux - Fatal Error", 
			//	MB_OK|MB_ICONEXCLAMATION);

			throw 0L;
			break;
		}
#endif // DEBUG

		Lock();
		isrunning = IsRunning();
		Unlock();

		if (!isrunning)
			break;

		Suspend();
	}

	return 0;
}
