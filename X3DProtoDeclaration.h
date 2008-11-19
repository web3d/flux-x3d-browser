/********************************************************************************
 * Flux
 *
 * File: X3DProtoDeclaration.h
 * Description: Declaration of the CX3DProtoDeclaration
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

// X3DProtoDeclaration.h : Declaration of the CX3DProtoDeclaration

#ifndef __X3DPROTODECLARATION_H_
#define __X3DPROTODECLARATION_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CX3DProtoDeclaration
class ATL_NO_VTABLE CX3DProtoDeclaration : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DProtoDeclaration, &CLSID_CoX3DProtoDeclaration>,
	public IDispatchImpl<X3DProtoDeclaration, &IID_X3DProtoDeclaration, &LIBID_ANIMALib>
{
protected :

	class CAnmProtoSymbol *m_protosym;

public:
	CX3DProtoDeclaration()
	{
		m_protosym = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DPROTODECLARATION)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DProtoDeclaration)
	COM_INTERFACE_ENTRY(X3DProtoDeclaration)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// X3DProtoDeclaration
public:

	STDMETHOD(get_name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_isExternProto)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_fields)(/*[out, retval]*/ X3DFieldDefinitionArray **pVal);

	STDMETHOD(setNativeProtoDeclaration)(/*[in]*/ void *pNativeProtoDeclaration)
	{
		m_protosym = (class CAnmProtoSymbol *) pNativeProtoDeclaration;
		return S_OK;
	}

	static X3DProtoDeclaration *Create(class CAnmProtoSymbol *pProtoSym);

};

#endif //__X3DPROTODECLARATION_H_
