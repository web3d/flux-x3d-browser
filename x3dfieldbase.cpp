/********************************************************************************
 * Flux
 *
 * File: x3dfieldbase.cpp
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



#include "stdafx.h"
#include "Anima.h"

#include "x3dfieldbase.h"

#include "animaapp.h"
#include "anmexecutioncontext.h"
#include "anmbrowser.h"
#include "anmtime.h"
#include "anmworld.h"

X3DFieldBase::~X3DFieldBase()
{
	if (m_mid && m_mid->memberType == eAnmCallbackMember && m_callback
		&& m_node)
	{
		CAnmNode *pNode = m_node->GetNode();
		pNode->RemoveCallback(m_mid, m_callback);
	}

	SafeRelease(m_context);

	std::list< sAnmObserverCallback >::iterator iter;
	for( 
		iter = m_observers.begin(); 
		iter != m_observers.end();
		iter++ )
	{
		IDispatch *pListener = (*iter).m_observer;
		SafeRelease(pListener);
	}
}

void X3DFieldBase::attach(X3DExecutionContext *context, CAnmSAINode *pNode, 
						  CAnmField *pField, CLASSMEMBERID mid, eValueType valueType)
{
	m_context = context;
	m_context->AddRef();

	m_node = pNode;
	m_field = pField;
	m_fieldtype = valueType;
	m_mid = mid;

	if (pNode && mid && (mid->memberType == eAnmCallbackMember))
	{
		CAnmNode *pNode = m_node->GetNode();
		m_callback = pNode->AddCallback(mid, fieldBaseCallback, this);
	}
}

void X3DFieldBase::fieldBaseCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData, void *userData)
{
	X3DFieldBase *pfb = (X3DFieldBase *) userData;
	pfb->handleCallback(sourceObject, reason, callData);
}

void X3DFieldBase::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData)
{
}


void X3DFieldBase::callCallbacks(X3DField *pField)
{
	CAnmExecutionContext *pContext = NULL;
	m_context->getClosure((void **) &pContext);

	if (pContext)
	{
		cApplication *pApp = pContext->GetApp();

		assert(pApp);

		CAnmTime *pClock = pApp->GetClock();
		assert(pClock);

		Time timestamp = pClock->GetCurrentTime();
		sAnmThreadedCallback *pTCB = new sAnmThreadedCallback;
		pTCB->pFieldBase = this;
		pTCB->pField = pField;
		pTCB->timestamp = timestamp;
		pApp->PostMessage(eAppMsgCallFunction, pTCB);
	}
}

void X3DFieldBase::callThreadedCallbacks(X3DField *pField, double timestamp)
{
	int sz = m_observers.size();

	std::list< sAnmObserverCallback >::iterator iter;
	for( 
		iter = m_observers.begin(); 
		iter != m_observers.end();
		iter++ )
	{

		HRESULT hr;

		IDispatch *pObserver = (*iter).m_observer;

		// try to use the raw COM interface if it's there
		X3DFieldEventListener *pRawListener = NULL;
		hr = pObserver->QueryInterface(IID_X3DFieldEventListener, (void **) &pRawListener);

		if (SUCCEEDED(hr))
		{
			pRawListener->readableFieldChanged(pField, timestamp);

			int refcount = pRawListener->Release();
		}
		else
		{

			// Otherwise, try Dispatch method

			DISPID callbackdisp = -1;
			BSTR callbackname = SysAllocString(L"readableFieldChanged");

			ITypeInfo *pTInfo = NULL;

			hr = pObserver->GetTypeInfo( 
				0, GetUserDefaultLCID(), &pTInfo);

			if (SUCCEEDED(hr))
			{
				// got type info? let's try to dispatch that way
				FUNCDESC *pFD;
				hr = pTInfo->GetFuncDesc(0, &pFD);

				hr = pObserver->GetIDsOfNames(IID_NULL, &callbackname, 1, GetUserDefaultLCID(), &callbackdisp);

				if (SUCCEEDED(hr))
				{
					// N.B.: load VARIANT args in reverse order for Invoke?
					VARIANT args[2];
					VariantInit(&args[1]);
					args[1].vt = VT_DISPATCH;
					args[1].pdispVal = pField;
					VariantInit(&args[0]);
					args[0].vt = VT_R8;
					args[0].dblVal = timestamp;

					VARIANT retval = { VT_EMPTY, 0 };

					DISPPARAMS params = {args, 0, 2, 0};

					hr = pObserver->Invoke(callbackdisp,
						IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &params, NULL,
						NULL, NULL);
				}
			}
		}
	}
}

eAnmFieldBaseSetValue X3DFieldBase::getValue(void **pVal)
{
	if (m_mid == NULL || m_node == NULL)
		return ANMFIELDBASE_DIY;

	if (m_mid->memberType == eAnmValueMember || m_mid->memberType == eAnmInitMember)
	{
		CAnmNode *pNode = m_node->GetNode();
		assert(pNode);

		// If the node is in the scene graph already, we need to
		// make sure we're synced up with the latest simulation state
		CAnmWorld *pWorld = pNode->GetWorld();
		if (pWorld)
		{
			cAnimaApp *pApp = (cAnimaApp *) pWorld->GetApp();
			assert(pApp);

			pApp->WaitForExternalEvents();

			pApp->Lock();

			pNode->GetValue(m_mid, pVal);

			pApp->Unlock();
		}
		else
		{
			pNode->GetValue(m_mid, pVal);
		}
	}
	else if (m_mid->memberType == eAnmCallbackMember)
		return ANMFIELDBASE_DIY;
	else
		return ANMFIELDBASE_CANTDO;

	return ANMFIELDBASE_DID;
}

eAnmFieldBaseSetValue X3DFieldBase::setValue(void *pVal)
{
	if (m_field)
	{
		CAnmNode *pNode = m_node->GetNode();
		assert(pNode);

		// If the node is in the scene graph already, we need to
		// make sure we're synced up with the latest simulation state
		CAnmWorld *pWorld = pNode->GetWorld();
		if (pWorld)
		{
			cAnimaApp *pApp = (cAnimaApp *) pWorld->GetApp();
			assert(pApp);

			pApp->Lock();

			m_field->SetData(pVal);

			pApp->Unlock();
		}
		else
		{
			m_field->SetData(pVal);
		}

		return ANMFIELDBASE_DID;
	}
	else
	{
		return ANMFIELDBASE_DIY;
	}
}

void X3DFieldBase::addFieldEventListener(IDispatch *listener)
{
	if (listener)
	{
		m_observers.push_back(sAnmObserverCallback(listener, NULL));
		listener->AddRef();
	}
}

void X3DFieldBase::removeFieldEventListener(IDispatch *listener)
{
	if (listener)
	{
		BOOL found = FALSE;

		std::list< sAnmObserverCallback >::iterator iter;
		for( 
			iter = m_observers.begin(); 
			iter != m_observers.end();
			iter++ )
		{
			if ((*iter).m_observer == listener)
			{
				found = TRUE;
				break;
			}

		}

		if (found)
		{
			m_observers.erase(iter);
			listener->Release();
		}
	}
}

#if 0
// These are left around for historical reasons, a reference in case
// we need to go back to taking VARIANTs as args...
void X3DFieldBase::adviseV(VARIANT pObserver, VARIANT userData)
{
	if (pObserver.vt == VT_DISPATCH)
	{
		IDispatch *pDisp = pObserver.pdispVal;

		IDispatch *pUd = NULL;
		if (userData.vt == VT_DISPATCH)
			pUd = userData.pdispVal;

		if (pDisp)
		{
			advise(pDisp, pUd);
		}
	}
}

void X3DFieldBase::unAdviseV(VARIANT pObserver)
{
	if (pObserver.vt == VT_DISPATCH)
	{
		IDispatch *pDisp = pObserver.pdispVal;
		if (pDisp)
		{
			unAdvise(pDisp);
		}
	}
}
#endif

void X3DFieldBase::CallX3DCallback(sAnmThreadedCallback *pTCB)
{
	if (pTCB)
	{
		pTCB->pFieldBase->callThreadedCallbacks(pTCB->pField, pTCB->timestamp);
		delete pTCB;
	}
}


STDMETHODIMP X3DFieldBase::getAccessType(X3DFieldAccess *accesstype)
{
	if (m_mid)
		*accesstype = getAccessType(m_mid->memberType);
	else
		*accesstype = X3DInputOutput;

	return S_OK;
}

STDMETHODIMP X3DFieldBase::getType(X3DFieldType *fieldtype)
{
	*fieldtype = getFieldType(m_fieldtype);

	return S_OK;
}

STDMETHODIMP X3DFieldBase::getName(BSTR *name)
{
	CComBSTR bstr;

	if (m_mid)
		bstr = m_mid->name;
	else
		bstr = "";

	*name = bstr.Copy();

	return S_OK;
}

STDMETHODIMP X3DFieldBase::dispose()
{
	// N.B.: later
	return S_OK;
}

X3DFieldAccess X3DFieldBase::getAccessType(eAnmClassMemberType memberType)
{
	X3DFieldAccess val;

	switch (memberType)
	{
		case eAnmCallbackMember :
			val = X3DOutputOnly;
			break;

		case eAnmMethodMember :
			val = X3DInputOnly;
			break;

		case eAnmValueMember :
			val = X3DInputOutput;
			break;

		case eAnmInitMember :
			val = X3DInitializeOnly;
			break;

		default :
			assert(FALSE);	// what the hell?
			break;
	}

	return val;
}

X3DFieldType X3DFieldBase::getFieldType(eValueType valueType)
{
	X3DFieldType val = X3DUnknownFieldType;

	switch (valueType)
	{
		case eValueBoolean :
			val = X3DSFBoolType;
			break;

		case eValueBooleanArray :
			val = X3DMFBoolType;
			break;

		case eValueColor :
			val = X3DSFColorType;
			break;

		case eValueColorArray :
			val = X3DMFColorType;
			break;

		case eValueDouble :
			val = X3DSFDoubleType;
			break;

		case eValueDoubleArray :
			val = X3DMFDoubleType;
			break;

		case eValueInteger :
			val = X3DSFInt32Type;
			break;

		case eValueIntegerArray :
			val = X3DMFInt32Type;
			break;

		case eValueFloat :
			val = X3DSFFloatType;
			break;

		case eValueFloatArray :
			val = X3DMFFloatType;
			break;

		case eValueNode :
			val = X3DSFNodeType;
			break;

		case eValueNodeArray :
			val = X3DMFNodeType;
			break;

		case eValuePoint2 :
			val = X3DSFVec2fType;
			break;

		case eValuePoint2Array :
			val = X3DMFVec2fType;
			break;

		case eValuePoint3 :
			val = X3DSFVec3fType;
			break;

		case eValuePoint3Array :
			val = X3DMFVec3fType;
			break;

		case eValueRotation :
			val = X3DSFRotationType;
			break;

		case eValueRotationArray :
			val = X3DMFRotationType;
			break;

		case eValueString :
			val = X3DSFStringType;
			break;

		case eValueStringArray :
			val = X3DMFStringType;
			break;

		case eValueTime :
			val = X3DSFTimeType;
			break;

		case eValueTimeArray :
			val = X3DMFTimeType;
			break;

		default :
			assert(FALSE);	// what the hell?
			break;
	}

	return val;

}

