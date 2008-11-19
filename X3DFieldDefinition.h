/********************************************************************************
 * Flux
 *
 * File: X3DFieldDefinition.h
 * Description: Declaration of the CX3DFieldDefinition
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

// X3DFieldDefinition.h : Declaration of the CX3DFieldDefinition

#ifndef __X3DFIELDDEFINITION_H_
#define __X3DFIELDDEFINITION_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CX3DFieldDefinition
class ATL_NO_VTABLE CX3DFieldDefinition : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DFieldDefinition, &CLSID_CoX3DFieldDefinition>,
	public IDispatchImpl<X3DFieldDefinition, &IID_X3DFieldDefinition, &LIBID_ANIMALib>
{
	X3DFieldType			 m_fieldType;
	X3DFieldAccess			 m_fieldAccessType;
	CComBSTR				 m_name;

public:
	CX3DFieldDefinition()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DFIELDDEFINITION)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DFieldDefinition)
	COM_INTERFACE_ENTRY(X3DFieldDefinition)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// X3DFieldDefinition
public:
	STDMETHOD(get_accessType)(/*[out, retval]*/ X3DFieldAccess *pVal);
	STDMETHOD(put_accessType)(/*[in]*/ X3DFieldAccess newVal);
	STDMETHOD(get_dataType)(/*[out, retval]*/ X3DFieldType *pVal);
	STDMETHOD(put_dataType)(/*[in]*/ X3DFieldType newVal);
	STDMETHOD(get_name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_name)(/*[in]*/ BSTR newVal);

	static X3DFieldDefinition *Create(X3DFieldType fieldType, X3DFieldAccess accessType,
		BSTR name);
	
};

#endif //__X3DFIELDDEFINITION_H_
