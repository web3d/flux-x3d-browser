/********************************************************************************
 * Flux
 *
 * File: anmloadsensor.h
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

#ifndef _anmloadsensor_h
#define _anmloadsensor_h


#include "anmsensor.h"

/*
LoadSensor : X3DNetworkSensorNode {
  SFBool  [in,out] enabled   TRUE
  SFTime  [in,out] timeOut   0
  MFNode  [in,out] watchList []   [X3DUrlNode]
  SFBool  [out]    isActive
  SFBool  [out]    isLoaded
  SFTime  [out]    loadTime
  SFFloat [out]    progress
}
*/

#define ANMLOADSENSOR_DEFAULT_TIMEOUT	0.f
#define ANMLOADSENSOR_DEFAULT_ENABLED	TRUE

class CAnmLoadSensor : public CAnmSensor
{
protected:

	Float					 m_timeOut;
	NodeArray				*m_watchList;

	BooleanArray			*m_activeValues;
	FloatArray				*m_progressValues;

	Boolean					 m_isActive;
	Boolean					 m_prevActive;
	Boolean					 m_isLoaded;
	Boolean					 m_prevLoaded;
	Boolean					 m_timedOut;
	Boolean					 m_failed;
	Float					 m_prevProgress;

	virtual void HandleActive(Boolean isActive);
	virtual void HandleLoaded(Boolean isLoaded);
	virtual void HandleProgress(Float progress);
	virtual void HandleFailure();

	virtual void NotifyActive(class CAnmUrlObject *pUrlObject);
	virtual void NotifyLoaded(class CAnmUrlObject *pUrlObject);
	virtual void NotifyProgress(class CAnmUrlObject *pUrlObject, Float progress);
	virtual void NotifyTimeOut(class CAnmUrlObject *pUrlObject);
	virtual void NotifyFailure(class CAnmUrlObject *pUrlObject);

	virtual int WatchChildIndex(class CAnmUrlObject *watchChild);
	virtual class CAnmUrlObject *GetUrlObject(CAnmNode *pChild); 

	void AddToWatchChildren();
	void RemoveFromWatchChildren();

	friend class CAnmUrlObject;

public:

	// constructor/destructor
	CAnmLoadSensor();
	virtual ~CAnmLoadSensor();

	// CAnmSensor overrides
	virtual void Realize();
	virtual void Update();
	virtual void Poll();

	// New methods
	
	// Accessors
	virtual void SetTimeOut(Float timeOut);
	virtual Float GetTimeOut() { return m_timeOut; }
	virtual void GetTimeOut(Float *pVal)
	{
		if (pVal)
			*pVal = m_timeOut;
	}

	virtual void SetWatchList(NodeArray *pWatchlist);
	virtual NodeArray *GetWatchList() { return m_watchList; }
	virtual void GetWatchList(NodeArray **pVal)
	{
		if (pVal)
			*pVal = m_watchList;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmLoadSensor);

	DECLARE_VALUE(enabled);
	DECLARE_VALUE(timeOut);
	DECLARE_VALUE(watchList);

	DECLARE_METHOD(set_enabled);
	DECLARE_METHOD(set_timeOut);
	DECLARE_METHOD(set_watchList);

	DECLARE_CALLBACK(enabled_changed);
	DECLARE_CALLBACK(timeOut_changed);
	DECLARE_CALLBACK(watchList_changed);
	DECLARE_CALLBACK(isActive);
	DECLARE_CALLBACK(isLoaded);
	DECLARE_CALLBACK(loadTime);
	DECLARE_CALLBACK(progress);
};

#endif // _anmloadsensor_h

