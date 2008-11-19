/********************************************************************************
 * Flux
 *
 * File: anmobject.cpp
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

#include "stdafx.h"

#include "anmobject.h"
#include "anmcallback.h"

CAnmObject::CAnmObject()
{
}

CAnmObject::~CAnmObject()
{
}

// Callback, Method and Set/GetValue support

DEFINE_EMPTY_ROOT_CLASS(CAnmObject)

CAnmCallbackList *CAnmObject::getCallbackList(CLASSMEMBERID mid)
{
	
	CAnmCallbackList *pCBList;
	MEMBEROFFSET offset = mid->memberOffset;

	int pointerAsInt;

	pointerAsInt = (int) this;
	pointerAsInt += offset;

	pCBList = (CAnmCallbackList *) pointerAsInt;

	return pCBList;
}

CAnmCallback *CAnmObject::AddCallback(CLASSMEMBERID mid, CAnmCallbackFunction userFunction, void *userData)
{
	CAnmCallbackList *pCBList;

	pCBList = getCallbackList(mid);
	assert(pCBList != NULL);

	CAnmCallback *pCB = new CAnmCallback(userFunction, this, mid, userData);

	pCBList->AddCallback(pCB);

	return pCB;
}

void CAnmObject::RemoveCallback(CLASSMEMBERID mid, CAnmCallback *pCB)
{
	assert(pCB != NULL);

	CAnmCallbackList *pCBList;

	pCBList = getCallbackList(mid);
	assert(pCBList != NULL);

	pCBList->RemoveCallback(pCB);

	delete pCB;
}

void CAnmObject::CallCallbacks(CLASSMEMBERID mid, void *cbdata)
{
	CAnmCallbackList *pCBList;

	pCBList = getCallbackList(mid);
	assert(pCBList != NULL);

	pCBList->CallCallbacks(cbdata);
}

void CAnmObject::SetValue(CLASSMEMBERID mid, void *value)
{
	if (mid)
	{
		assert(mid->memberType == eAnmValueMember || mid->memberType == eAnmInitMember );

		(*(mid->pSetValue)) (this, value);
	}
	// N.B.: should we have an else-case here? assert, exception, warning, what?
}

void CAnmObject::GetValue(CLASSMEMBERID mid, void **ppValue)
{
	if (mid)
	{
		 if (mid->memberType == eAnmValueMember)
			(*(mid->pGetValue)) (this, ppValue);
		 else
		 {
			 assert(mid->memberType == eAnmInitMember);
		 }
	}
	// N.B.: should we have an else-case here? assert, exception, warning, what?
}

void CAnmObject::CallMethod(CLASSMEMBERID mid, void *value)
{
	if (mid)
	{
		assert(mid->memberType == eAnmMethodMember);

		(*(mid->pCallMethod)) (this, value);
	}
	// N.B.: should we have an else-case here? assert, exception, warning, what?
}

