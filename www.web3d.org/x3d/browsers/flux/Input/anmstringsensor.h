/********************************************************************************
 * Flux
 *
 * File: anmstringsensor.h
 * Description: StringSensor class
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

#ifndef _anmstringsensor_h
#define _anmstringsensor_h

#include "anmkeyboardsensor.h"
#include "..\animaapp.h"

#define ANMSTRINGSENSOR_DEFAULT_ENABLED				TRUE
#define ANMSTRINGSENSOR_DEFAULT_DELETIONALLOWED		TRUE

#define ANMSTRINGSENSOR_ALLOCSIZE	1024

#define ANMSTRINGSENSOR_TERMINATORCHAR	ANMWIN_ENTERCHAR
#define ANMSTRINGSENSOR_DELETECHAR		ANMWIN_DELETECHAR

class CAnmStringSensor : public CAnmKeyboardSensor, public iCharacterReceiver
{
protected:

	Boolean					 m_deletionAllowed;
	
	
	class cAnimaApp			*m_anmapp;
	char					*m_stringBuffer;
	int						 m_curstringbuffersize;
	int						 m_curcharindex;
	bool					 m_newdata;
	bool					 m_terminatorReceived;

	// iCharacterReceiver overrides
	virtual void NewCharacter( int nchar );

	// Helpers

	// String allocation helpers
	void ExpandBuffer();
	void ClearBuffer();

	void EnsureBufferSize()
	{
		if (m_curcharindex >= m_curstringbuffersize - 1)
			ExpandBuffer();
	}

public:

	// constructor/destructor
	CAnmStringSensor();
	virtual ~CAnmStringSensor();

	// CAnmKeyboardSensor overrides
	virtual void Realize();
	virtual void Update();
	virtual void Poll();

	// New methods
	
	// Accessors
	virtual void SetDeletionAllowed(Boolean deletionAllowed);
	virtual Boolean GetDeletionAllowed() { return m_deletionAllowed; }
	virtual void GetDeletionAllowed(Boolean *pVal)
	{
		if (pVal)
			*pVal = m_deletionAllowed;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmStringSensor);

	DECLARE_VALUE(deletionAllowed);
	DECLARE_VALUE(enabled);

	DECLARE_METHOD(set_deletionAllowed);
	DECLARE_METHOD(set_enabled);

	DECLARE_CALLBACK(deletionAllowed_changed);
	DECLARE_CALLBACK(enabled_changed);
	DECLARE_CALLBACK(enteredText);
	DECLARE_CALLBACK(finalText);
	DECLARE_CALLBACK(isActive);
};

#endif // _anmstringsensor_h

