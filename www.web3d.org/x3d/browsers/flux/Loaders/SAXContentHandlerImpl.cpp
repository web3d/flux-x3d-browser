/********************************************************************************
 * Flux
 *
 * File: SAXContentHandlerImpl.cpp
 * Description: implementation of the SAXContentHandlerImpl class
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

// SAXContentHandlerImpl.cpp: implementation of the SAXContentHandlerImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SAXContentHandlerImpl.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


SAXContentHandlerImpl::SAXContentHandlerImpl()
{
}

SAXContentHandlerImpl::~SAXContentHandlerImpl()
{
}



HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::putDocumentLocator( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator
            )
{
    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::startDocument()
{
    return S_OK;
}
        

        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::endDocument( void)
{
    return S_OK;
}
        
        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::startPrefixMapping( 
            /* [in] */ wchar_t __RPC_FAR *pwchPrefix,
            /* [in] */ int cchPrefix,
            /* [in] */ wchar_t __RPC_FAR *pwchUri,
            /* [in] */ int cchUri)
{
    return S_OK;
}
        
        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::endPrefixMapping( 
            /* [in] */ wchar_t __RPC_FAR *pwchPrefix,
            /* [in] */ int cchPrefix)
{
    return S_OK;
}
        

        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::startElement( 
            /* [in] */ wchar_t __RPC_FAR *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ wchar_t __RPC_FAR *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ wchar_t __RPC_FAR *pwchRawName,
            /* [in] */ int cchRawName,
            /* [in] */ ISAXAttributes __RPC_FAR *pAttributes)
{
    return S_OK;
}
        
       
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::endElement( 
            /* [in] */ wchar_t __RPC_FAR *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ wchar_t __RPC_FAR *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ wchar_t __RPC_FAR *pwchRawName,
            /* [in] */ int cchRawName)
{
    return S_OK;
}
        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::characters( 
            /* [in] */ wchar_t __RPC_FAR *pwchChars,
            /* [in] */ int cchChars)
{
    return S_OK;
}
        

HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::ignorableWhitespace( 
            /* [in] */ wchar_t __RPC_FAR *pwchChars,
            /* [in] */ int cchChars)
{
    return S_OK;
}
        

HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::processingInstruction( 
            /* [in] */ wchar_t __RPC_FAR *pwchTarget,
            /* [in] */ int cchTarget,
            /* [in] */ wchar_t __RPC_FAR *pwchData,
            /* [in] */ int cchData)
{
    return S_OK;
}
        
        
HRESULT STDMETHODCALLTYPE SAXContentHandlerImpl::skippedEntity( 
            /* [in] */ wchar_t __RPC_FAR *pwchVal,
            /* [in] */ int cchVal)
{
    return S_OK;
}


long __stdcall SAXContentHandlerImpl::QueryInterface(const struct _GUID &riid,void ** ppvObject)
{
    // hack-hack-hack!
    return 0;
}

unsigned long __stdcall SAXContentHandlerImpl::AddRef()
{
    // hack-hack-hack!
    return 0;
}

unsigned long __stdcall SAXContentHandlerImpl::Release()
{
    // hack-hack-hack!
    return 0;
}

