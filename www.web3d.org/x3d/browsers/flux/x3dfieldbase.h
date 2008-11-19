/********************************************************************************
 * Flux
 *
 * File: x3dfieldbase.h
 * Description: Base class for COM field interfaces
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


#ifndef _x3dfieldbase_h
#define _x3dfieldbase_h

#include "anmfields.h"
#include "anmnode.h"
#include "anmsainode.h"

struct sAnmObserverCallback {

	IDispatch *m_observer;
	IDispatch *m_userData;

	sAnmObserverCallback(IDispatch *pObserver, IDispatch *userData)
	{
		m_observer = pObserver;
		m_userData = userData;
	}
};

struct sAnmThreadedCallback {
	class X3DFieldBase *pFieldBase;
	X3DField *pField;
	double timestamp;
};

enum eAnmFieldBaseSetValue {
	ANMFIELDBASE_CANTDO,
	ANMFIELDBASE_DID,
	ANMFIELDBASE_DIY,
};


#define ANMMINFIELDCALLBACKDELTA .2f	// /secs

class X3DFieldBase
{
protected :

	std::list<sAnmObserverCallback> m_observers;
	X3DExecutionContext *m_context;
	class CAnmSAINode *m_node;
	class CAnmField *m_field;
	class CAnmCallback *m_callback;
	CLASSMEMBERID m_mid;
	eValueType m_fieldtype;
	Time m_lastcbtime; 
	
	static void fieldBaseCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData, void *userData);
	virtual void handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData);
	void callCallbacks(X3DField *pField);
	void callThreadedCallbacks(X3DField *pField, double timestamp);

public :

	X3DFieldBase()
	{
		m_context = NULL;
		m_node = NULL;
		m_field = NULL;
		m_callback = NULL;
		m_mid = NULL;
		m_lastcbtime = ANMTIME_BADTIMEVALUE;
	}

	~X3DFieldBase();

	// Accessors
	void setType(eValueType fieldType)
	{
		m_fieldtype = fieldType;
	}

	eValueType getType()
	{
		return m_fieldtype;
	}

	class CAnmSAINode *getNode()
	{
		return m_node;
	}

	class CAnmField *getField()
	{
		return m_field;
	}

	// Methods
	void attach(X3DExecutionContext *context, CAnmSAINode *pNode, CAnmField *pField,
		CLASSMEMBERID mid, eValueType fieldType);

	eAnmFieldBaseSetValue getValue(void **pVal);
	eAnmFieldBaseSetValue setValue(void *pVal);

	void addFieldEventListener(IDispatch *listener);
	void removeFieldEventListener(IDispatch *listener);

	STDMETHOD(getAccessType)(/*[out, retval]*/ X3DFieldAccess *accesstype);
	STDMETHOD(getType)(/*[out, retval]*/ X3DFieldType *fieldtype);
	STDMETHOD(getName)(/*[out, retval]*/ BSTR *name);
	STDMETHOD(dispose)();

	static void CallX3DCallback(sAnmThreadedCallback *pTCB);

	static X3DFieldAccess getAccessType(eAnmClassMemberType memberType);
	static X3DFieldType getFieldType(eValueType valueType);
};


#endif // _x3dfieldbase_h
