/********************************************************************************
 * Flux
 *
 * File: SAXErrorHandlerImpl.cpp
 * Description: XML SAX error handler class
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
#include "SAXErrorHandlerImpl.h"

HRESULT STDMETHODCALLTYPE SAXErrorHandlerImpl::error(
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE SAXErrorHandlerImpl::fatalError(
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE SAXErrorHandlerImpl::ignorableWarning(
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode )
{
	return S_OK;
}

long __stdcall SAXErrorHandlerImpl::QueryInterface(const struct _GUID &riid,void ** ppvObject)
{
    // hack-hack-hack!
	if (riid == __uuidof(ISAXLexicalHandler))
	{
		*ppvObject = this;
		return S_OK;
	}

    return 0;
}

unsigned long __stdcall SAXErrorHandlerImpl::AddRef()
{
    // hack-hack-hack!
    return 0;
}

unsigned long __stdcall SAXErrorHandlerImpl::Release()
{
    // hack-hack-hack!
    return 0;
}

