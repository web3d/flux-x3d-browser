/********************************************************************************
 * Flux
 *
 * File: cRenderThread.h
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

#include "cThread.h"

#define ANMSIMTHREAD_GOTFOCUSFRAMERATE	60
#define ANMSIMTHREAD_LOSTFOCUSFRAMERATE 30
#define ANMSIMTHREAD_MINIMIZEDFRAMERATE  1

typedef enum eAnmSimState
{
	eAnmSimGotFocus,
	eAnmSimLostFocus,
	eAnmSimMinimized,
} eAnmSimState ;

class EXPORT cRenderThread : public cThread 
{
protected:

	class cApplication	*m_app;
	eAnmSimState		 m_simState;
	DWORD				 m_sleepTime;

	DWORD CalcSleepTime(eAnmSimState state)
	{
		switch (state)
		{
			case eAnmSimGotFocus :
				return 1000 / ANMSIMTHREAD_GOTFOCUSFRAMERATE;
				break;

			case eAnmSimLostFocus :
				return 1000 / ANMSIMTHREAD_LOSTFOCUSFRAMERATE;
				break;

			case eAnmSimMinimized :
				return 1000 / ANMSIMTHREAD_MINIMIZEDFRAMERATE;
				break;

			default :
				assert(FALSE);
				break;
		}

		return 1000 / ANMSIMTHREAD_GOTFOCUSFRAMERATE;
	}

public:
	cRenderThread(class cApplication *app);
	virtual ~cRenderThread();

	virtual DWORD ThreadProc();
	virtual void cRenderThread::SetSimState(eAnmSimState state);

	DWORD GetSleepTime()
	{
		return m_sleepTime;
	}
};
