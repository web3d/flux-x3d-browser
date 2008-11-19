/********************************************************************************
 * Flux
 *
 * File: cThread.h
 * Description: interface for the cThread class
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

// cThread.h: interface for the cThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTHREAD_H__4A115C4B_D1D4_11D3_AE4F_00E029031C67__INCLUDED_)
#define AFX_CTHREAD_H__4A115C4B_D1D4_11D3_AE4F_00E029031C67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "anmcriticalsection.h"

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>

typedef enum eAnmThreadPriority
{
	eAnmThreadPriorityNormal = THREAD_PRIORITY_NORMAL,
	eAnmThreadPriorityAboveNormal = THREAD_PRIORITY_ABOVE_NORMAL,
	eAnmThreadPriorityBelowNormal = THREAD_PRIORITY_BELOW_NORMAL,
	eAnmThreadPriorityHighest = THREAD_PRIORITY_HIGHEST,
	eAnmThreadPriorityLowest = THREAD_PRIORITY_LOWEST,
} eAnmThreadPriority;

#define REASONABLE_THREAD_TIMEOUT 60000 // msec

class EXPORT cThread
{
protected:
  HANDLE  d_threadHandle;
	DWORD   d_threadID;

	HANDLE m_syncHandle;

  Boolean    d_bIsRunning;
  CWinThread *m_winthread;

  char d_errorBuffer[ 1000 ];

  CAnmCriticalSection	m_criticalSection;

public:
	cThread();
	virtual ~cThread();

  virtual void Begin(eAnmThreadPriority priority = eAnmThreadPriorityNormal,
	  BOOL suspendFlag = FALSE);
  virtual void End(bool waitflag = false);
  virtual bool IsRunning();

  virtual DWORD ThreadProc();

	// Thread safety
	void Lock()
	{
		m_criticalSection.Lock();
	}

	void Unlock()
	{
		m_criticalSection.Unlock();
	}

	void Suspend();
	void Resume();

	void SetPriority(eAnmThreadPriority prio);
	eAnmThreadPriority GetPriority();

	HANDLE GetSyncHandle()
	{
		return m_syncHandle;
	}

  void        SetError( const char *pBuffer );
  const char  *Error();
};

#endif // !defined(AFX_CTHREAD_H__4A115C4B_D1D4_11D3_AE4F_00E029031C67__INCLUDED_)
