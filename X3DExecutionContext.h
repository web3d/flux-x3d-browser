/********************************************************************************
 * Flux
 *
 * File: X3DExecutionContext.h
 * Description: Declaration of the CX3DExecutionContext
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

// X3DExecutionContext.h : Declaration of the CX3DExecutionContext

#ifndef __X3DEXECUTIONCONTEXT_H_
#define __X3DEXECUTIONCONTEXT_H_

#include "resource.h"       // main symbols
#include "x3dexecbase.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DExecutionContext
class ATL_NO_VTABLE CX3DExecutionContext : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CX3DExecutionContext, &CLSID_CoX3DExecutionContext>,
	public IDispatchImpl<X3DExecutionContext, &IID_X3DExecutionContext, &LIBID_ANIMALib>,
	public X3DExecBase
{
public:
	CX3DExecutionContext()
	{
		SetX3DContext(this);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_X3DEXECUTIONCONTEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CX3DExecutionContext)
	COM_INTERFACE_ENTRY(X3DExecutionContext)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// X3DExecutionContext
public:
	STDMETHOD(setClosure)(/*[in]*/ void *closure) 
		{ SetContext((CAnmExecutionContext *) closure); return S_OK; }
	STDMETHOD(getClosure)(/*[in]*/ void **closure) 
		{ GetContext((CAnmExecutionContext **) closure); return S_OK; }
	STDMETHOD(getSpecificationVersion)(/*[out,retval]*/ BSTR *value)
		{  return X3DExecBase::getSpecificationVersion(value); }
	STDMETHOD(getEncoding)(/*[out,retval]*/ X3DEncodingType *encodingType)
		{  return X3DExecBase::getEncoding(encodingType); }
	STDMETHOD(getNode)(/*[in]*/ BSTR name, /*[out, retval]*/ X3DNode **value)
		{ return X3DExecBase::getNode(name, value); }
	STDMETHOD(createNode)(BSTR type, /*[out, retval]*/ X3DNode **node)
		{ return X3DExecBase::createNode(type, node); }
	STDMETHOD(addRoute)(/*[in]*/ X3DNode * fromNode, /*[in]*/ BSTR fromEventOut, /*[in]*/ X3DNode *toNode, /*[in]*/ BSTR toEventIn, X3DRoute **ppRoute)
		{ return X3DExecBase::addRoute(fromNode, fromEventOut, toNode, toEventIn, ppRoute); }
	STDMETHOD(deleteRoute)(/*[in]*/ X3DRoute *pX3DRoute)
		{ return X3DExecBase::deleteRoute(pX3DRoute); }
	STDMETHOD(getRootNodes)(/*[out,retval]*/ X3DNodeArray **rootnodes)
		{ return X3DExecBase::getRootNodes(rootnodes); }
	STDMETHOD(getProtoDeclarations)(/*[out,retval]*/ X3DProtoDeclarationArray **protodeclarations)
		{ return X3DExecBase::getProtoDeclarations(protodeclarations); }
};

#endif //__X3DEXECUTIONCONTEXT_H_
