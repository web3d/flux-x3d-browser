/********************************************************************************
 * Flux
 *
 * File: anmbindstatuscallback.cpp
 * Description: Bind status callback class
 *						Here's where we handle async URL loading
 *                      N.B.: completely Windows-specific
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
#include "anmbindstatuscallback.h"
#include "anmfilemanager.h"
 
#define TRACE(_t_) 

#define MAXBUFLEN 2048

 //-----------------------------------------------------------------------------
CAnmBindStatusCallback::CAnmBindStatusCallback(cApplication *pApp,
											   CAnmFileManager *pFileManager,
											   void *closure)
{
	m_app = pApp;
	m_fileManager = pFileManager;
	m_closure = closure;
	m_ulObjRefCount = 1;
}
//-----------------------------------------------------------------------------
// IUnknown
STDMETHODIMP CAnmBindStatusCallback::QueryInterface(REFIID riid, void **ppvObject)
{
	TRACE(_T("IUnknown::QueryInterface\n"));

	*ppvObject = NULL;
	
	// IUnknown
	if (::IsEqualIID(riid, __uuidof(IUnknown)))
	{
		TRACE(_T("IUnknown::QueryInterface(IUnknown)\n"));

		*ppvObject = this;
	}
	// IBindStatusCallback
	else if (::IsEqualIID(riid, __uuidof(IBindStatusCallback)))
	{
		TRACE(_T("IUnknown::QueryInterface(IBindStatusCallback)\n"));

		*ppvObject = static_cast<IBindStatusCallback *>(this);
	}

	if (*ppvObject)
	{
		(*reinterpret_cast<LPUNKNOWN *>(ppvObject))->AddRef();

		return S_OK;
	}
	
	return E_NOINTERFACE;
}                                             
//-----------------------------------------------------------------------------
STDMETHODIMP_(ULONG) CAnmBindStatusCallback::AddRef()
{
	TRACE(_T("IUnknown::AddRef\n"));

	return ++m_ulObjRefCount;
}
//-----------------------------------------------------------------------------
STDMETHODIMP_(ULONG) CAnmBindStatusCallback::Release()
{
	TRACE(_T("IUnknown::Release\n"));

	return --m_ulObjRefCount;
}
//-----------------------------------------------------------------------------
// IBindStatusCallback
STDMETHODIMP CAnmBindStatusCallback::OnStartBinding(DWORD, IBinding *pBinding)
{
	TRACE(_T("IBindStatusCallback::OnStartBinding\n"));

	return S_OK;
}
//-----------------------------------------------------------------------------
STDMETHODIMP CAnmBindStatusCallback::GetPriority(LONG *)
{
	TRACE(_T("IBindStatusCallback::GetPriority\n"));

	return E_NOTIMPL;
}
//-----------------------------------------------------------------------------
STDMETHODIMP CAnmBindStatusCallback::OnLowResource(DWORD)
{
	TRACE(_T("IBindStatusCallback::OnLowResource\n"));

	return S_OK;
}
//-----------------------------------------------------------------------------
STDMETHODIMP CAnmBindStatusCallback::OnProgress(ULONG ulProgress,
										 ULONG ulProgressMax,
										 ULONG ulStatusCode,
										 LPCWSTR szStatusText)
{
	static const LPCTSTR plpszStatus[] = 
	{
		_T("BINDSTATUS_FINDINGRESOURCE"),  // 1
		_T("BINDSTATUS_CONNECTING"),
		_T("BINDSTATUS_REDIRECTING"),
		_T("BINDSTATUS_BEGINDOWNLOADDATA"),
		_T("BINDSTATUS_DOWNLOADINGDATA"),
		_T("BINDSTATUS_ENDDOWNLOADDATA"),
		_T("BINDSTATUS_BEGINDOWNLOADCOMPONENTS"),
		_T("BINDSTATUS_INSTALLINGCOMPONENTS"),
		_T("BINDSTATUS_ENDDOWNLOADCOMPONENTS"),
		_T("BINDSTATUS_USINGCACHEDCOPY"),
		_T("BINDSTATUS_SENDINGREQUEST"),
		_T("BINDSTATUS_CLASSIDAVAILABLE"),
		_T("BINDSTATUS_MIMETYPEAVAILABLE"),
		_T("BINDSTATUS_CACHEFILENAMEAVAILABLE"),
		_T("BINDSTATUS_BEGINSYNCOPERATION"),
		_T("BINDSTATUS_ENDSYNCOPERATION"),
		_T("BINDSTATUS_BEGINUPLOADDATA"),
		_T("BINDSTATUS_UPLOADINGDATA"),
		_T("BINDSTATUS_ENDUPLOADINGDATA"),
		_T("BINDSTATUS_PROTOCOLCLASSID"),
		_T("BINDSTATUS_ENCODING"),
		_T("BINDSTATUS_VERFIEDMIMETYPEAVAILABLE"),
		_T("BINDSTATUS_CLASSINSTALLLOCATION"),
		_T("BINDSTATUS_DECODING"),
		_T("???")  // unexpected
	};

	TRACE(_T("IBindStatusCallback::OnProgress\n"));

	TRACE(_T("ulProgress: %lu, ulProgressMax: %lu\n"),
		  ulProgress, ulProgressMax);
	
	TRACE(_T("ulStatusCode: %lu "), ulStatusCode);

	if (ulStatusCode < BINDSTATUS_FINDINGRESOURCE ||
		ulStatusCode > BINDSTATUS_DECODING)
		ulStatusCode = BINDSTATUS_DECODING + 1;
	
	TRACE(_T("(%s), szStatusText: %ls\n"),
		  plpszStatus[ulStatusCode - BINDSTATUS_FINDINGRESOURCE],
		  szStatusText);

	char statbuf[MAXBUFLEN];
	char cbuf[MAXBUFLEN];

	if (szStatusText == NULL)
		return S_OK;

	int len = wcslen(szStatusText);
	if (len > MAXBUFLEN - 1)
		len = MAXBUFLEN -1;

	if (len <= 0)
		return S_OK;

	wcstombs(cbuf, szStatusText, len);
	cbuf[len] = '\0';

	BOOL printsomething = TRUE;
	int percent = 0L;
	switch(ulStatusCode)
	{
		case BINDSTATUS_FINDINGRESOURCE	:
			sprintf(statbuf, "Finding server %s...", cbuf);
			break;

		case BINDSTATUS_CONNECTING	:
			sprintf(statbuf, "Connecting to %s...", cbuf);
			break;

		case BINDSTATUS_BEGINDOWNLOADDATA	:
			sprintf(statbuf, "Downloading data...", cbuf);
			break;

		case BINDSTATUS_DOWNLOADINGDATA	:
			percent = (int) ((float) ulProgress/(float)ulProgressMax * 100.f);
			sprintf(statbuf, "Loading %s (%d%%)...", cbuf, percent);
			if (m_fileManager)
				m_fileManager->NotifyProgress((float) percent / 100.f, m_closure);
			break;

		case BINDSTATUS_ENDDOWNLOADDATA	:
			sprintf(statbuf, "Done", cbuf);
			break;

		case BINDSTATUS_REDIRECTING	:
		case BINDSTATUS_BEGINDOWNLOADCOMPONENTS	:
		case BINDSTATUS_INSTALLINGCOMPONENTS	:
		case BINDSTATUS_ENDDOWNLOADCOMPONENTS	:
		case BINDSTATUS_USINGCACHEDCOPY	:
		case BINDSTATUS_SENDINGREQUEST	:
		case BINDSTATUS_CLASSIDAVAILABLE	:
		case BINDSTATUS_MIMETYPEAVAILABLE	:
		case BINDSTATUS_CACHEFILENAMEAVAILABLE	:
		case BINDSTATUS_BEGINSYNCOPERATION	:
		case BINDSTATUS_ENDSYNCOPERATION	:
		case BINDSTATUS_BEGINUPLOADDATA	:
		case BINDSTATUS_UPLOADINGDATA	:
		case BINDSTATUS_ENDUPLOADDATA	:
		case BINDSTATUS_PROTOCOLCLASSID	:
		case BINDSTATUS_ENCODING	:
		case BINDSTATUS_VERIFIEDMIMETYPEAVAILABLE	:
		case BINDSTATUS_CLASSINSTALLLOCATION	:
		case BINDSTATUS_DECODING	:
		case BINDSTATUS_LOADINGMIMEHANDLER	:
		default :
			printsomething = FALSE;
			break;

	}

	if (printsomething)
		m_app->SetStatusText(statbuf);

	return S_OK;
}
//-----------------------------------------------------------------------------
STDMETHODIMP CAnmBindStatusCallback::OnStopBinding(HRESULT, LPCWSTR)
{
	TRACE(_T("IBindStatusCallback::OnStopBinding\n"));

	return S_OK;
}
//-----------------------------------------------------------------------------
STDMETHODIMP CAnmBindStatusCallback::GetBindInfo(DWORD *, BINDINFO *)
{
	TRACE(_T("IBindStatusCallback::GetBindInfo\n"));

	return S_OK;
}
//-----------------------------------------------------------------------------
STDMETHODIMP CAnmBindStatusCallback::OnDataAvailable(DWORD, DWORD,
											  FORMATETC *, STGMEDIUM *)
{
	TRACE(_T("IBindStatusCallback::OnDataAvailable\n"));

	return S_OK;
}
//-----------------------------------------------------------------------------
STDMETHODIMP CAnmBindStatusCallback::OnObjectAvailable(REFIID, IUnknown *)
{
	TRACE(_T("IBindStatusCallback::OnObjectAvailable\n"));

	return S_OK;
}
//-----------------------------------------------------------------------------
