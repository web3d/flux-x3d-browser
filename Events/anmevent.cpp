/********************************************************************************
 * Flux
 *
 * File: anmevent.cpp
 * Description: Event class
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
#include "anmevent.h"
#include "anmnodedefs.h"
#include "anmfields.h"

CAnmEvent::CAnmEvent(class CAnmObject *pObject, CLASSMEMBERID reason, void *pEventData)
{
	m_sourceObject = pObject;
	m_reason = reason;
	m_fieldValue = CAnmField::CreateField(reason->valueType, pEventData);
}

CAnmEvent::CAnmEvent(class CAnmObject *pObject, CLASSMEMBERID reason)
{
	m_sourceObject = pObject;
	m_reason = reason;
	m_fieldValue = CAnmField::CreateField(reason->valueType, NULL);
}

void CAnmEvent::SetData(void *pData)
{
	m_fieldValue->SetData(pData);
}

void *CAnmEvent::GetData()
{
	return m_fieldValue->GetData();
}

CAnmEvent::~CAnmEvent()
{
	int debug = 1;

	SafeUnRef(m_fieldValue);
}
