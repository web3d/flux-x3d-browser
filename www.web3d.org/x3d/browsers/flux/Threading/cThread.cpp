/********************************************************************************
 * Flux
 *
 * File: cThread.cpp
 * Description: implementation of the cThread class
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

// cThread.cpp: implementation of the cThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cThread.h"
// #include "cNetError.h"

//////////////////////////////////////////////////////////////////////

static DWORD WINAPI cThreadProc( cThread *pThis )
{
	CoInitialize(NULL);

	DWORD retval = pThis->ThreadProc();

	SetEvent(pThis->GetSyncHandle());

	delete pThis;

	CoUninitialize();

	return retval;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cThread::cThread()
{
  d_threadID = 0;
  d_threadHandle = NULL;
  m_syncHandle = NULL;

  m_winthread = NULL;

  d_bIsRunning = FALSE;
}


cThread::~cThread()
{
  End();

  CloseHandle(m_syncHandle);
}


void cThread::Begin(eAnmThreadPriority priority, BOOL suspendFlag)
{
  d_bIsRunning = TRUE;

  DWORD createFlags = 0L;
  if (suspendFlag)
	  createFlags |= CREATE_SUSPENDED;

  m_winthread = AfxBeginThread((AFX_THREADPROC) cThreadProc,
	  this, priority, 0, createFlags);

  d_threadHandle = m_winthread;

  m_syncHandle = CreateEvent(0, FALSE, FALSE, NULL);
}


void cThread::End(bool waitflag)
{
	Lock();
    d_bIsRunning = FALSE;
	Unlock();

	Resume();

#if 1
	DWORD stat = WaitForSingleObject(m_syncHandle, REASONABLE_THREAD_TIMEOUT);
#else
	DWORD stat = WaitForSingleObject(m_syncHandle, INFINITE);
#endif

	if (stat == WAIT_OBJECT_0)
	{
		int foo = 1;
	}
	else if (stat == WAIT_FAILED)
	{
		int foo = 1;
	}

}


DWORD cThread::ThreadProc()
{
	return 0;
}
////////////////////////////////////////////////////////////////////////////////

void cThread::SetError( const char *pBuffer )
{
  if( pBuffer == NULL )
    strcpy( d_errorBuffer, "Unspecified." );
  else
  {
    if( sizeof( d_errorBuffer ) > strlen( pBuffer ) )
      strcpy( d_errorBuffer, pBuffer );
    else
    {
      strncpy( d_errorBuffer, pBuffer, sizeof( d_errorBuffer ) );
      d_errorBuffer[ sizeof( d_errorBuffer ) - 1 ] = '\0';
    }
  }
}


const char *cThread::Error()
{
  return (const char *)d_errorBuffer;
}


bool cThread::IsRunning()
{
  return d_bIsRunning;
}

void cThread::Suspend()
{
	DWORD suspendcount = m_winthread->SuspendThread();
}

void cThread::Resume()
{
	DWORD suspendcount = m_winthread->ResumeThread();
}

void cThread::SetPriority(eAnmThreadPriority prio)
{
	BOOL stat = m_winthread->SetThreadPriority(prio);
	if (!stat)
	{
		// N.B.: eep, what do we do?
	}
}

eAnmThreadPriority cThread::GetPriority()
{
	return (eAnmThreadPriority) m_winthread->GetThreadPriority();
}