/********************************************************************************
 * Flux
 *
 * File: x3dexecbase.h
 * Description: Base class for COM execution context
 *								 interfaces
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


#ifndef _x3dexecbase_h
#define _x3dexecbase_h

class X3DExecBase
{
protected :

	class CAnmExecutionContext *m_context;
	interface X3DExecutionContext *m_x3dcontext;

	void SetX3DContext(interface X3DExecutionContext *pX3DContext)
	{
		m_x3dcontext = pX3DContext;
	}

	void SetContext(class CAnmExecutionContext *pContext);
	void GetContext(class CAnmExecutionContext **ppContext);
	STDMETHOD(getSpecificationVersion)(/*[out,retval]*/ BSTR *value);
	STDMETHOD(getEncoding)(/*[out,retval]*/ X3DEncodingType *encodingType);
	STDMETHOD(getNode)(/*[in]*/ BSTR name, /*[out, retval]*/ X3DNode **value);
	STDMETHOD(createNode)(BSTR type, /*[out, retval]*/ X3DNode **node);
	STDMETHOD(addRoute)(/*[in]*/ X3DNode * fromNode, /*[in]*/ BSTR fromEventOut, /*[in]*/ X3DNode *toNode, /*[in]*/ BSTR toEventIn, X3DRoute **ppRoute);
	STDMETHOD(deleteRoute)(/*[in]*/ X3DRoute *pX3DRoute);
	STDMETHOD(getRootNodes)(/*[out,retval]*/ X3DNodeArray **rootnodes);
	STDMETHOD(getProtoDeclarations)(/*[out,retval]*/ X3DProtoDeclarationArray **protodeclarations);
	
public :

	X3DExecBase()
	{
		m_context = NULL;
	}
	~X3DExecBase();

};


#endif // _x3dexecbase_h
