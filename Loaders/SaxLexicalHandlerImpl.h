/********************************************************************************
 * Flux
 *
 * File: SaxLexicalHandlerImpl.h
 * Description: XML SAX lexical handler class
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

#ifndef _saxlexicalhandlerimpl_h
#define _saxlexicalhandlerimpl_h

class SAXLexicalHandlerImpl : public ISAXLexicalHandler
{
    //
    // Raw methods provided by interface
    //
protected :

    // This must be correctly implemented, if your handler must be a COM Object (in this example it does not)
    long __stdcall QueryInterface(const struct _GUID &,void ** );
    unsigned long __stdcall AddRef(void);
    unsigned long __stdcall Release(void);

	virtual HRESULT STDMETHODCALLTYPE startDTD (
        unsigned short * pwchName,
        int cchName,
        unsigned short * pwchPublicId,
        int cchPublicId,
        unsigned short * pwchSystemId,
        int cchSystemId );
    virtual HRESULT STDMETHODCALLTYPE endDTD ( );
    virtual HRESULT STDMETHODCALLTYPE startEntity (
        unsigned short * pwchName,
        int cchName );
    virtual HRESULT STDMETHODCALLTYPE endEntity (
        unsigned short * pwchName,
        int cchName );
    virtual HRESULT STDMETHODCALLTYPE startCDATA ( );
    virtual HRESULT STDMETHODCALLTYPE endCDATA ( );
    virtual HRESULT STDMETHODCALLTYPE comment (
        unsigned short * pwchChars,
        int cchChars );
};

#endif // _saxlexicalhandlerimpl_h
