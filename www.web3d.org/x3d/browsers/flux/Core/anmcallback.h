/********************************************************************************
 * Flux
 *
 * File: anmcallback.h
 * Description: Callback classes
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

#ifndef _anmcallback_h
#define _anmcallback_h

#include <list>

#include "anmclass.h"

typedef void (*CAnmCallbackFunction) (class CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData, void *userData);

class CAnmCallback
{

protected:

	CAnmCallbackFunction			 m_cbfunc;
	class CAnmObject				*m_cbobject;
	CLASSMEMBERID					 m_cbreason;
	void							*m_cbclosure;

public:

	// constructor/destructor
	CAnmCallback()
	{
		m_cbfunc = NULL;
		m_cbobject = NULL;
		m_cbreason = NULL;
		m_cbclosure = NULL;
	}

	CAnmCallback(CAnmCallbackFunction userFunction, class CAnmObject *sourceObject, 
			CLASSMEMBERID reason, void *userData)
	{
		m_cbfunc = userFunction;
		m_cbobject = sourceObject;
		m_cbreason = reason;
		m_cbclosure = userData;
	}

	virtual ~CAnmCallback()
	{
	}

	// Methods
	void Call(void *callData)
	{
		(*m_cbfunc)(m_cbobject, m_cbreason, callData, m_cbclosure);
	}

	// Accessors
	void SetFunction(CAnmCallbackFunction userFunction)
	{
		m_cbfunc = userFunction;
	}

	CAnmCallbackFunction GetFunction()
	{
		return m_cbfunc;
	}

	void SetObject(class CAnmObject *sourceObject)
	{
		m_cbobject = sourceObject;
	}

	class CAnmObject *GetObject()
	{
		return m_cbobject;
	}

	void SetReason(CLASSMEMBERID reason)
	{
		m_cbreason = reason;
	}

	CLASSMEMBERID GetReason()
	{
		return m_cbreason;
	}

	void SetClosure(void *userData)
	{
		m_cbclosure = userData;
	}

	void *GetClosure()
	{
		return m_cbclosure;
	}
};

class CAnmCallbackList
{
protected:

	std::list<CAnmCallback *>		m_callbacks;

public:

	// constructor/destructor
	CAnmCallbackList(int nCallbacks = 1)
	{
		// pre-allocate the list?
	}

	virtual ~CAnmCallbackList()
	{
		std::list<CAnmCallback *>::iterator iter;
		for( 
			iter = m_callbacks.begin(); 
			iter != m_callbacks.end();
			iter++ )
		{
			CAnmCallback *pCB = (*iter);
			delete pCB;
		}
	}

	// Methods
	virtual void AddCallback(CAnmCallback *pCB)
	{
		m_callbacks.push_back(pCB);
	}

	virtual void RemoveCallback(CAnmCallback *pCB)
	{
		m_callbacks.remove(pCB);
	}

	virtual void CallCallbacks(void *callData)
	{
		std::list<CAnmCallback *>::iterator iter;
		for( 
			iter = m_callbacks.begin(); 
			iter != m_callbacks.end();
			iter++ )
		{
			(*iter)->Call(callData);
		}
	}
};

#endif // _anmcallback_h
