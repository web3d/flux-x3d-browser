/********************************************************************************
 * Flux
 *
 * File: SaxErrorHandlerImpl.h
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

#ifndef _saxerrorhandlerimpl_h
#define _saxerrorhandlerimpl_h

class SAXErrorHandlerImpl : public ISAXErrorHandler
{
    //
    // Raw methods provided by interface
    //
protected :

    // This must be correctly implemented, if your handler must be a COM Object (in this example it does not)
    long __stdcall QueryInterface(const struct _GUID &,void ** );
    unsigned long __stdcall AddRef(void);
    unsigned long __stdcall Release(void);

    virtual HRESULT __stdcall error (
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode );
    virtual HRESULT __stdcall fatalError (
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode );
    virtual HRESULT __stdcall ignorableWarning (
        struct ISAXLocator * pLocator,
        unsigned short * pwchErrorMessage,
        HRESULT hrErrorCode );
};

#endif // _saxerrorhandlerimpl_h