/********************************************************************************
 * Flux
 *
 * File: anmloadsensor.cpp
 * Description: LoadSensor class
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
#include "anmloadsensor.h"
#include "anmurlobject.h"
#include "anmworld.h"

CAnmLoadSensor::CAnmLoadSensor() : 
CAnmSensor(ANMLOADSENSOR_DEFAULT_ENABLED),
m_timeOut(ANMLOADSENSOR_DEFAULT_TIMEOUT)
{
	m_watchList = new NodeArray;
	m_activeValues = new BooleanArray;
	m_progressValues = new FloatArray;

	m_isActive = false;
	m_prevActive = false;
	m_isLoaded = false;
	m_prevLoaded = false;
	m_timedOut = false;
	m_failed = false;
	m_prevProgress = 0.f;
} 

CAnmLoadSensor::~CAnmLoadSensor()
{
	SafeUnRef(m_watchList);
	SafeUnRef(m_activeValues);
	SafeUnRef(m_progressValues);
}

void CAnmLoadSensor::Realize()
{
	CAnmSensor::Realize();		// just chain down for now

	SetStateDirty();
}

void CAnmLoadSensor::Update()
{
	Lock();

	if (!StateDirty()) {
		Unlock();
		return;
	}

	m_activeValues->clear();
	m_progressValues->clear();

	int sz = m_watchList->size();
	for (int i = 0; i < sz; i++)
	{
		m_activeValues->push_back(false);
		m_progressValues->push_back(0.f);
	}

	m_isActive = false;
	m_prevActive = false;
	m_isLoaded = false;
	m_prevLoaded = false;
	m_timedOut = false;
	m_failed = false;
	m_prevProgress = 0.f;

	ClearStateDirty();

	Unlock();
}

void CAnmLoadSensor::Poll()
{
	Lock();

	if (StateDirty())
		Update();

	// Nothing to report if we're already loaded
	if (m_isLoaded)
	{
		Unlock();
		return;
	}

	// Check for timeOut or failure
	if (m_timedOut || m_failed)
	{
		HandleFailure();
	}

	int sz = m_watchList->size();
	if (sz <= 0)
	{

		Unlock();
		return;
	}

	// Test for active. Was the flag changed since last time?
	if (m_isActive != m_prevActive)
	{
		HandleActive(m_isActive);
		m_prevActive = m_isActive;
	}

	// Test progress by averaging file progress values
	int psz = m_progressValues->size();


	if( psz != sz ) {
		Unlock();
		return;
	}
	assert(psz == sz);


	float progress = 0;
	for (int i = 0; i < psz; i++)
	{
		progress += (*m_progressValues)[i];
	}

	assert(psz >= 0);
	progress /= psz;

	if (m_prevProgress != progress)
	{
		HandleProgress(progress);
		m_prevProgress = progress;
	}

	// Test for fully loaded: progress >= 1
	if (progress >= 0.999f) {
		m_isLoaded = true;
	}

	if (m_isLoaded != m_prevLoaded)
	{
		HandleLoaded(m_isLoaded);
		m_prevLoaded = m_isLoaded;
	}

	Unlock();
}

// Helper methods
void CAnmLoadSensor::HandleActive(Boolean isActive)
{
	CallCallbacks(CALLBACKID(CAnmLoadSensor, isActive), &isActive);
}

void CAnmLoadSensor::HandleLoaded(Boolean isLoaded)
{
	CallCallbacks(CALLBACKID(CAnmLoadSensor, isLoaded), &isLoaded);

	if (isLoaded)
	{
		CAnmWorld *pWorld = GetWorld();
		ABORT_IF_WORLD_IS_NULL

		cApplication *pApp = pWorld->GetApp();

		if (pApp == NULL)
			return;

		CAnmTime *pClock = pApp->GetClock();
		Time loadTime = pClock->GetCurrentTime();

		CallCallbacks(CALLBACKID(CAnmLoadSensor, loadTime), &loadTime);
	}
}

void CAnmLoadSensor::HandleProgress(Float progress)
{
	CallCallbacks(CALLBACKID(CAnmLoadSensor, progress), &progress);
}

void CAnmLoadSensor::HandleFailure()
{
	HandleActive(false);
	HandleLoaded(false);

	m_isActive = false;
	m_isLoaded = false;
}

int CAnmLoadSensor::WatchChildIndex(class CAnmUrlObject *watchChild)
{
	int sz = m_watchList->size();

	for (int i = 0; i < sz; i++)
	{
		// eep
		if (watchChild == GetUrlObject((*m_watchList)[i]))
			return i;
	}

	return -1;
}

CAnmUrlObject *CAnmLoadSensor::GetUrlObject(CAnmNode *pChild)
{
	assert(pChild != NULL);

	// get past any PROTOs
	CAnmNode *pImp = pChild->GetImplementationNode();

	CAnmUrlObject *pUrlObject = NULL;
	if (pImp->GetInterface(ANMINTERFACEID(CAnmUrlObject), &pUrlObject))
		return pUrlObject;

	return NULL;
}

void CAnmLoadSensor::AddToWatchChildren()
{
	CAnmUrlObject *pWatchChild;

	int sz = m_watchList->size();

	for (int i = 0; i < sz; i++)
	{
		if (pWatchChild = GetUrlObject((*m_watchList)[i]))
		{
			pWatchChild->AddLoadSensor(this);
		}
	}
}

void CAnmLoadSensor::RemoveFromWatchChildren()
{
	CAnmUrlObject *pWatchChild;

	int sz = m_watchList->size();

	for (int i = 0; i < sz; i++)
	{
		if (pWatchChild = GetUrlObject((*m_watchList)[i]))
		{
			pWatchChild->RemoveLoadSensor(this);
		}
	}
}

// Notifications from UrlObject friends

void CAnmLoadSensor::NotifyActive(class CAnmUrlObject *pUrlObject)
{
	Lock();
	// We go active on first load attemp
	m_isActive = true;

	Unlock();
}

void CAnmLoadSensor::NotifyLoaded(class CAnmUrlObject *pUrlObject)
{
	Lock();

	// Set progress value for this child to 1 (= fully loaded)
	int childIndex = WatchChildIndex(pUrlObject);

	if (childIndex < 0) {
		Unlock();
		return;
	}

	int psz = m_progressValues->size();

	if (childIndex >= psz) {
		Unlock();
		return;
	}

	(*m_progressValues)[childIndex] = 1.f;

	Unlock();
}

void CAnmLoadSensor::NotifyProgress(class CAnmUrlObject *pUrlObject, Float progress)
{
	Lock();

	// Set progress value for this child to 1 (= fully loaded)
	int childIndex = WatchChildIndex(pUrlObject);

	if (childIndex < 0) {
		Unlock();
		return;
	}

	int psz = m_progressValues->size();

	if (childIndex >= psz) {
		Unlock();
		return;
	}

	(*m_progressValues)[childIndex] = progress;

	Unlock();
}

void CAnmLoadSensor::NotifyTimeOut(class CAnmUrlObject *pUrlObject)
{
	Lock();

	m_timedOut = true;

	Unlock();
}

void CAnmLoadSensor::NotifyFailure(class CAnmUrlObject *pUrlObject)
{
	Lock();

	m_failed = true;

	Unlock();
}

// Accessors
void CAnmLoadSensor::SetTimeOut(Float timeOut)
{
	m_timeOut = timeOut;

	SetStateDirty();

	CallCallbacks(CALLBACKID(CAnmLoadSensor, timeOut_changed), &timeOut);
}

void CAnmLoadSensor::SetWatchList(NodeArray *pWatchList)
{
	assert(pWatchList != NULL);
	RemoveFromWatchChildren();

	SafeUnRef(m_watchList);
	m_watchList = pWatchList;
	m_watchList->Ref();

	SetStateDirty();

	// Check for non-UrlObject children
	int sz = m_watchList->size();

	for (int i = 0; i < sz; i++)
	{
		if (GetUrlObject((*m_watchList)[i]) == NULL)
			// warn or something
			;

	}

	AddToWatchChildren();

	CallCallbacks(CALLBACKID(CAnmLoadSensor, watchList_changed), &pWatchList);
}

// Tables for Callbacks, Methods and Set/GetValues

DEFINE_INHERITED_VALUE(CAnmLoadSensor, enabled, CAnmSensor);
DEFINE_VALUE(CAnmLoadSensor, timeOut, eValueFloat, Float, GetTimeOut, SetTimeOut);
DEFINE_VALUE(CAnmLoadSensor, watchList, eValueNodeArray, NodeArray*, GetWatchList, SetWatchList);

DEFINE_INHERITED_METHOD(CAnmLoadSensor, set_enabled, CAnmSensor);
DEFINE_METHOD(CAnmLoadSensor, set_timeOut, eValueFloat, Float, SetTimeOut);
DEFINE_METHOD(CAnmLoadSensor, set_watchList, eValueNodeArray, NodeArray*, SetWatchList);

DEFINE_INHERITED_CALLBACK(CAnmLoadSensor, enabled_changed, CAnmSensor);
DEFINE_CALLBACK(CAnmLoadSensor, timeOut_changed, eValueFloat);
DEFINE_CALLBACK(CAnmLoadSensor, watchList_changed, eValueNodeArray);
DEFINE_INHERITED_CALLBACK(CAnmLoadSensor, isActive, CAnmSensor);
DEFINE_CALLBACK(CAnmLoadSensor, isLoaded, eValueBoolean);
DEFINE_CALLBACK(CAnmLoadSensor, loadTime, eValueTime);
DEFINE_CALLBACK(CAnmLoadSensor, progress, eValueFloat);

// member table
BEGIN_DEFINE_NODE_MEMBERS(CAnmLoadSensor)
VALUEID(CAnmLoadSensor, enabled),
VALUEID(CAnmLoadSensor, timeOut),
VALUEID(CAnmLoadSensor, watchList),

METHODID(CAnmLoadSensor, set_enabled),
METHODID(CAnmLoadSensor, set_timeOut),
METHODID(CAnmLoadSensor, set_watchList),

CALLBACKID(CAnmLoadSensor, enabled_changed),
CALLBACKID(CAnmLoadSensor, timeOut_changed),
CALLBACKID(CAnmLoadSensor, watchList_changed),
CALLBACKID(CAnmLoadSensor, isActive),
CALLBACKID(CAnmLoadSensor, isLoaded),
CALLBACKID(CAnmLoadSensor, loadTime),
CALLBACKID(CAnmLoadSensor, progress),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmLoadSensor, CAnmSensor);
