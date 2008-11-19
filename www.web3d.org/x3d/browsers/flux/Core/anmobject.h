/********************************************************************************
 * Flux
 *
 * File: anmobject.h
 * Description: Base class for all objects
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

#ifndef _anmobject_h
#define _anmobject_h

#include "anmcriticalsection.h"
#include "anmcallback.h"
#include "anmclass.h"
#include "anmrefobject.h"

class EXPORT CAnmObject : public CAnmRefObject
{
protected:

	class CAnmCallbackList	*getCallbackList(CLASSMEMBERID mid);

	CAnmCriticalSection		 m_criticalSection;

public:

	// constructor/destructor
	CAnmObject();
	virtual ~CAnmObject();

	// New methods

	// Thread safety
	void Lock()
	{
		m_criticalSection.Lock();
	}

	void Unlock()
	{
		m_criticalSection.Unlock();
	}

	// Callbacks, methods, values
	virtual CAnmCallback *AddCallback(CLASSMEMBERID mid, CAnmCallbackFunction userFunction, void *userData);
	virtual void RemoveCallback(CLASSMEMBERID mid, CAnmCallback *pCB);
	virtual void CallCallbacks(CLASSMEMBERID mid, void *cbdata);

	virtual void SetValue(CLASSMEMBERID mid, void *value);
	virtual void GetValue(CLASSMEMBERID mid, void **pValue);

	virtual void CallMethod(CLASSMEMBERID mid, void *value);

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmObject);

};


#endif // _anmobject_h
