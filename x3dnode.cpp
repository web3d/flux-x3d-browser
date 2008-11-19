/********************************************************************************
 * Flux
 *
 * File: X3DNode.cpp
 * Description: Implementation of CX3DNode
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

// X3DNode.cpp : Implementation of CX3DNode
#include "stdafx.h"
#include "Anima.h"
#include "X3DNode.h"
#include "anmnode.h"
#include "anmsainode.h"

#include "x3dfield.h"
#include "x3dfieldbase.h"
#include "x3dfielddefinition.h"
#include "x3dfielddefinitionarray.h"
#include "x3dsfbool.h"
#include "x3dsfcolor.h"
#include "x3dsffloat.h"
#include "x3dsfint32.h"
#include "x3dsfnode.h"
#include "x3dsfrotation.h"
#include "x3dsfstring.h"
#include "x3dsfvec2f.h"
#include "x3dsfvec3f.h"
#include "x3dsftime.h"
#include "x3dmfcolor.h"
#include "x3dmffloat.h"
#include "x3dmfint32.h"
#include "x3dmfnode.h"
#include "x3dmfrotation.h"
#include "x3dmfstring.h"
#include "x3dmfvec2f.h"
#include "x3dmfvec3f.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DNode


STDMETHODIMP CX3DNode::getTypeName(BSTR *typenm)
{
	// TODO: Add your implementation code here

	CComBSTR bstr;

	if (m_node == NULL)
		bstr = "UNKNOWN NODE TYPE";
	else
	{
		CAnmNode *pNode = m_node->GetNode();
		bstr = pNode->GetClass()->m_className;
	}

	*typenm = bstr.Copy();

	return S_OK;
}

static X3DField *createMember(eValueType valueType)
{
	HRESULT hr;
	X3DSFBool *psfbool = NULL;
	X3DSFColor *psfcolor = NULL;
	X3DSFFloat *psffloat = NULL;
	X3DSFInt32 *psfint32 = NULL;
	X3DSFNode *psfnode = NULL;
	X3DSFRotation *psfrotation = NULL;
	X3DSFString *psfstring = NULL;
	X3DSFTime *psftime = NULL;
	X3DSFVec2f *psfvec2f = NULL;
	X3DSFVec3f *psfvec3f = NULL;
	X3DMFColor *pmfcolor = NULL;
	X3DMFFloat *pmffloat = NULL;
	X3DMFInt32 *pmfint32 = NULL;
	X3DMFNode *pmfnode = NULL;
	X3DMFRotation *pmfrotation = NULL;
	X3DMFString *pmfstring = NULL;
	X3DMFVec2f *pmfvec2f = NULL;
	X3DMFVec3f *pmfvec3f = NULL;
	X3DField *retval = NULL;

	switch (valueType)
	{
		case eValueBoolean :
			hr = CoCreateInstance(CLSID_CoX3DSFBool, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFBool, (LPVOID*)&psfbool);
			retval = psfbool;
			break;

		case eValueColor :
			hr = CoCreateInstance(CLSID_CoX3DSFColor, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFColor, (LPVOID*)&psfcolor);
			retval = psfcolor;
			break;

		case eValueFloat :
			hr = CoCreateInstance(CLSID_CoX3DSFFloat, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFFloat, (LPVOID*)&psffloat);
			retval = psffloat;
			break;

		case eValueInteger :
			hr = CoCreateInstance(CLSID_CoX3DSFInt32, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFInt32, (LPVOID*)&psfint32);
			retval = psfint32;
			break;

		case eValueNode :
			hr = CoCreateInstance(CLSID_CoX3DSFNode, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFNode, (LPVOID*)&psfnode);
			retval = psfnode;
			break;

		case eValuePoint2 :
			hr = CoCreateInstance(CLSID_CoX3DSFVec2f, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFVec2f, (LPVOID*)&psfvec2f);
			retval = psfvec2f;
			break;

		case eValuePoint3 :
			hr = CoCreateInstance(CLSID_CoX3DSFVec3f, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFVec3f, (LPVOID*)&psfvec3f);
			retval = psfvec3f;
			break;

		case eValueRotation :
			hr = CoCreateInstance(CLSID_CoX3DSFRotation, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFRotation, (LPVOID*)&psfrotation);
			retval = psfrotation;
			break;

		case eValueString :
			hr = CoCreateInstance(CLSID_CoX3DSFString, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFString, (LPVOID*)&psfstring);
			retval = psfstring;
			break;

		case eValueTime :
			hr = CoCreateInstance(CLSID_CoX3DSFTime, NULL, CLSCTX_INPROC_SERVER, IID_X3DSFTime, (LPVOID*)&psftime);
			retval = psftime;
			break;

		case eValueColorArray :
			hr = CoCreateInstance(CLSID_CoX3DMFColor, NULL, CLSCTX_INPROC_SERVER, IID_X3DMFColor, (LPVOID*)&pmfcolor);
			retval = pmfcolor;
			break;

		case eValueFloatArray :
			hr = CoCreateInstance(CLSID_CoX3DMFFloat, NULL, CLSCTX_INPROC_SERVER, IID_X3DMFFloat, (LPVOID*)&pmffloat);
			retval = pmffloat;
			break;

		case eValueIntegerArray :
			hr = CoCreateInstance(CLSID_CoX3DMFInt32, NULL, CLSCTX_INPROC_SERVER, IID_X3DMFInt32, (LPVOID*)&pmfint32);
			retval = pmfint32;
			break;

		case eValueNodeArray :
			hr = CoCreateInstance(CLSID_CoX3DMFNode, NULL, CLSCTX_INPROC_SERVER, IID_X3DMFNode, (LPVOID*)&pmfnode);
			retval = pmfnode;
			break;

		case eValueRotationArray :
			hr = CoCreateInstance(CLSID_CoX3DMFRotation, NULL, CLSCTX_INPROC_SERVER, IID_X3DMFRotation, (LPVOID*)&pmfrotation);
			retval = pmfrotation;
			break;

		case eValueStringArray :
			hr = CoCreateInstance(CLSID_CoX3DMFString, NULL, CLSCTX_INPROC_SERVER, IID_X3DMFString, (LPVOID*)&pmfstring);
			retval = pmfstring;
			break;

		case eValuePoint2Array :
			hr = CoCreateInstance(CLSID_CoX3DMFVec2f, NULL, CLSCTX_INPROC_SERVER, IID_X3DMFVec2f, (LPVOID*)&pmfvec2f);
			retval = pmfvec2f;
			break;
		
		case eValuePoint3Array :
			hr = CoCreateInstance(CLSID_CoX3DMFVec3f, NULL, CLSCTX_INPROC_SERVER, IID_X3DMFVec3f, (LPVOID*)&pmfvec3f);
			retval = pmfvec3f;
			break;

		default :
			assert(FALSE);
			retval = NULL;
			break;
	}

	return retval;
}

static CLASSMEMBERID findMember(CAnmNode *pNode, BSTR name)
{
	assert(pNode);

}

STDMETHODIMP CX3DNode::getField(BSTR name, X3DField **result)
{
	if (m_node == NULL)
		return S_FALSE;

	char fieldname[ANMMAXSTRBUF];

	wcstombs(fieldname, name, ANMMAXSTRBUF);

	return getField(fieldname, result);

}

STDMETHODIMP CX3DNode::getField(char *fieldname, X3DField **result)
{
	CAnmField *pField = m_node->GetField(fieldname);

	if (pField == NULL)
		return S_FALSE;
	else
	{
		*result = createMember(pField->GetType());

		if (*result == NULL)
			return S_FALSE;

		CAnmNode *pNode = m_node->GetNode();
		assert(pNode);

		CAnmClass *pClass = pNode->GetClass();
		assert(pClass);

		CLASSMEMBERID mid = pClass->FindClassMember(fieldname);

		(*result)->attach(m_context, m_node, pField, mid, mid->valueType);
	}

	return S_OK;
}

STDMETHODIMP CX3DNode::getFieldAsVariant(char *fieldname, VARIANT *result)
{
	if (!fieldname || !strlen(fieldname) || !result)
		return S_FALSE;

	CAnmField *pField = m_node->GetField(fieldname);

	if (pField)
	{
		HRESULT hr = S_OK;

		switch (pField->GetType())
		{
			case eValueBoolean :
				{
					bool *pB = ((CAnmSFBool *)pField)->GetValue();
					assert(pB);

					result->vt = VT_BOOL;
					if (*pB)
						result->boolVal = VARIANT_TRUE;
					else
						result->boolVal = VARIANT_FALSE;
				}
				break;

			case eValueDouble :
				{
					double *pD = ((CAnmSFDouble *)pField)->GetValue();
					assert(pD);

					result->vt = VT_R8;
					result->dblVal = *pD;
				}
				break;

			case eValueFloat :
				{
					float *pF = ((CAnmSFFloat *)pField)->GetValue();
					assert(pF);

					double d = (double) *pF;

					result->vt = VT_R8;
					result->dblVal = d;
				}
				break;

			case eValueInteger :
				{
					Integer *pI = ((CAnmSFInt32 *)pField)->GetValue();
					assert(pI);

					result->vt = VT_I4;
					result->intVal = *pI;
				}
				break;

			case eValueString :
				{
					String *pS = ((CAnmSFString *)pField)->GetValue();
					assert(pS);

					char *cp = (*pS)->GetBuf();

					CComBSTR bstr = cp;

					result->vt = VT_BSTR;
					result->bstrVal = bstr.Copy();
				}
				break;

			case eValueTime :
				{
					Time *pT = ((CAnmSFTime *)pField)->GetValue();
					assert(pT);

					double d = *pT;

					result->vt = VT_R8;
					result->dblVal = d;
				}
				break;

			default :
				{
					X3DField *pX3DField = NULL;
					hr = getField(fieldname, &pX3DField);

					if (SUCCEEDED(hr))
					{
						result->vt = VT_DISPATCH;
						result->pdispVal = pX3DField;
					}
				}
				break;
		}
		return hr;
	}
	else
		return S_FALSE;
}

STDMETHODIMP CX3DNode::setFieldFromVariant(char *fieldname, const VARIANT *value)
{
	if (!fieldname || !strlen(fieldname) || !value)
		return S_FALSE;

	CAnmField *pField = m_node->GetField(fieldname);

	if (pField)
	{
		HRESULT hr = S_OK;

		eValueType valueType = pField->GetType();

		switch (valueType)
		{
			case eValueBoolean :
				{
					if (value->vt == VT_BOOL)
					{						
						bool b;
						
						if (value->boolVal == VARIANT_TRUE)
							b = true;
						else
							b = false;

						((CAnmSFBool *)pField)->SetValue(b);
					}
				}
				break;

			case eValueDouble :
				{
					if (value->vt == VT_R4)
						((CAnmSFDouble *)pField)->SetValue((double) value->fltVal);
					else if (value->vt == VT_R8)
						((CAnmSFDouble *)pField)->SetValue(value->dblVal);
					else if (value->vt == VT_I4)
						((CAnmSFDouble *)pField)->SetValue((double) value->intVal);
				}
				break;

			case eValueFloat :
				{
					if (value->vt == VT_R4)
						((CAnmSFFloat *)pField)->SetValue(value->fltVal);
					else if (value->vt == VT_R8)
						((CAnmSFFloat *)pField)->SetValue((float) value->dblVal);
					else if (value->vt == VT_I4)
						((CAnmSFFloat *)pField)->SetValue((float) value->intVal);
				}
				break;

			case eValueInteger :
				{
					if (value->vt == VT_R4)
						((CAnmSFInt32 *)pField)->SetValue((int) value->fltVal);
					else if (value->vt == VT_R8)
						((CAnmSFInt32 *)pField)->SetValue((int) value->dblVal);
					else if (value->vt == VT_I4)
						((CAnmSFInt32 *)pField)->SetValue(value->intVal);
				}
				break;

			case eValueString :
				{
					if (value->vt == VT_BSTR)
					{
						int len = wcslen(value->bstrVal);
						String s = new CAnmString;
						s->alloc(len + 1);
						wcstombs(s->GetBuf(), value->bstrVal, len + 1);

						((CAnmSFString *)pField)->SetValue(s);

						s->UnRef();
					}
				}
				break;

			case eValueTime :
				{
					if (value->vt == VT_R4)
						((CAnmSFTime *)pField)->SetValue((Time) value->fltVal);
					else if (value->vt == VT_R8)
						((CAnmSFTime *)pField)->SetValue(value->dblVal);
					else if (value->vt == VT_I4)
						((CAnmSFTime *)pField)->SetValue((Time) value->intVal);
				}
				break;

			default :
				hr = S_FALSE;
				break;
		}

		return hr;
	}
	else
		return S_FALSE;
}

STDMETHODIMP CX3DNode::getContext(X3DExecutionContext **result)
{

	*result = m_context;
	return S_OK;
}

STDMETHODIMP CX3DNode::setContext(X3DExecutionContext *context)
{
	m_context = context;
	return S_OK;
}

STDMETHODIMP CX3DNode::getName(BSTR *name)
{
	if (m_node == NULL)
		return S_FALSE;

	CAnmNode *pNode = m_node->GetNode();
	const char *nm = pNode->GetName()->GetBuf();

	CComBSTR str(nm);
	*name = str.Copy();

	return S_OK;
}


STDMETHODIMP CX3DNode::getNumFields(int *result)
{
	if (result == NULL)
		return S_FALSE;

	if (m_node == NULL)
	{
		*result = 0;
		return S_FALSE;
	}

	CAnmNode *pNode = m_node->GetNode();
	if (pNode == NULL)
	{
		*result = 0;
		return S_FALSE;
	}

	CAnmClass *pClass = pNode->GetClass();
	assert(pClass);

	*result = pClass->GetNumMembers();

	return S_OK;
}


STDMETHODIMP CX3DNode::getFieldInfo(int fieldIndex, X3DFieldAccess *accessType, X3DFieldType *fieldType, BSTR *fieldName)
{
	if (accessType == NULL || fieldType == NULL || fieldName == NULL)
		return S_FALSE;

	if (m_node == NULL)
		return S_FALSE;

	CAnmNode *pNode = m_node->GetNode();
	if (pNode == NULL)
		return S_FALSE;

	CAnmClass *pClass = pNode->GetClass();
	assert(pClass);

	int numClassMembers = pClass->GetNumMembers();

	if (fieldIndex < 0 || fieldIndex >= numClassMembers)
		return S_FALSE;

	CAnmClassMember **members = pClass->GetMemberTable();
	CAnmClassMember *pMember = members[fieldIndex];

	*accessType = X3DFieldBase::getAccessType(pMember->memberType);
	*fieldType = X3DFieldBase::getFieldType(pMember->valueType);
	CComBSTR str(pMember->name);
	*fieldName = str.Copy();

	return S_OK;
}

STDMETHODIMP CX3DNode::get_Text(BSTR *pVal)
{
	return getTypeName(pVal);
}

X3DNode *CX3DNode::CreateX3DNode(CAnmSAINode *pSAINode, X3DExecutionContext *pX3DContext)
{
	assert(pSAINode);

	X3DNode *pX3DNode = NULL;

	HRESULT hr = CoCreateInstance(CLSID_CoX3DNode, NULL, CLSCTX_INPROC_SERVER, IID_X3DNode, (LPVOID*)&pX3DNode);

	if (FAILED(hr))
		return NULL;

	pX3DNode->setNativeNode(pSAINode);
	pX3DNode->setContext(pX3DContext);

	return pX3DNode;
}

STDMETHODIMP CX3DNode::getFieldDefinitions(X3DFieldDefinitionArray **fieldDefinitions)
{
	int nfields;

	HRESULT hr = getNumFields(&nfields);

	if (FAILED(hr))
		return E_FAIL;

	X3DFieldDefinitionArray *pFieldDefs = NULL;

	hr = CoCreateInstance(CLSID_CoX3DFieldDefinitionArray, NULL, CLSCTX_INPROC_SERVER, 
		IID_X3DFieldDefinitionArray, (LPVOID*)&pFieldDefs);

	if (FAILED(hr))
		return E_FAIL;

	pFieldDefs->put_Count(nfields);

	for (int i = 0; i < nfields; i++)
	{
		X3DFieldType fieldType;
		X3DFieldAccess accessType; 
		BSTR fieldName;

		hr = getFieldInfo(i, &accessType, &fieldType, &fieldName);

		if (SUCCEEDED(hr))
		{
			X3DFieldDefinition *pFieldDef = CX3DFieldDefinition::Create(fieldType, accessType, fieldName);

			pFieldDefs->set1Value(i, pFieldDef);
		}
	}

	*fieldDefinitions = pFieldDefs;

	return S_OK;
}

STDMETHODIMP CX3DNode::GetTypeInfoCount( 
            /* [out] */ UINT __RPC_FAR *pctinfo)
{
	HRESULT hr = IDispatchImpl<X3DNode, &IID_X3DNode, &LIBID_ANIMALib>::GetTypeInfoCount(pctinfo);

	return hr;
}
        
STDMETHODIMP CX3DNode::GetTypeInfo( 
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
{
	HRESULT hr = IDispatchImpl<X3DNode, &IID_X3DNode, &LIBID_ANIMALib>::GetTypeInfo(iTInfo, lcid, ppTInfo);

	return hr;
}

#define IDOFFSET 32
        
STDMETHODIMP CX3DNode::GetIDsOfNames( 
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
{
	HRESULT hr = IDispatchImpl<X3DNode, &IID_X3DNode, &LIBID_ANIMALib>::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);

	if (!SUCCEEDED(hr))
	{
		char fieldname[ANMMAXSTRBUF];

		for (int nameindex = 0; nameindex < cNames; nameindex++)
		{
			wcstombs(fieldname, rgszNames[nameindex], ANMMAXSTRBUF);

			CAnmNode *pNode = m_node->GetNode();
			CAnmClass *pClass = pNode->GetClass();
			assert(pClass);

			CLASSMEMBERID mid = pClass->FindClassMember((char *) fieldname);

			if (mid)
			{
				CLASSMEMBERID *pMembers = pClass->GetMemberTable();
				int nMembers = pClass->GetNumMembers();

				for (int mindex = 0; mindex < nMembers; mindex++)
				{
					if (pMembers[mindex] == mid)
						break;
				}
				assert(mindex < nMembers);

				rgDispId[nameindex] = mindex + IDOFFSET;
			}
			else
				return E_FAIL;
		}
		return S_OK;

	}

	return hr;
}
        
STDMETHODIMP CX3DNode::Invoke( 
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr)
{
	HRESULT hr = IDispatchImpl<X3DNode, &IID_X3DNode, &LIBID_ANIMALib>::Invoke(dispIdMember, riid, lcid,
		wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	if (!SUCCEEDED(hr) && (wFlags == DISPATCH_PROPERTYGET || wFlags == DISPATCH_PROPERTYPUT))
	{
		int index = dispIdMember - IDOFFSET;
		
		if (index >= 0)
		{
			CAnmNode *pNode = m_node->GetNode();
			CAnmClass *pClass = pNode->GetClass();
			assert(pClass);

			CLASSMEMBERID *pMembers = pClass->GetMemberTable();
			int nMembers = pClass->GetNumMembers();

			if (index <= nMembers)
			{
				CLASSMEMBERID mid = pMembers[index];
				assert(mid);

				if (wFlags == DISPATCH_PROPERTYGET)
				{
					hr = getFieldAsVariant(mid->name, pVarResult);
						
					return hr;
				}
				else
				{
					assert(pDispParams);

					if (pDispParams->cArgs > 0)
						hr = setFieldFromVariant(mid->name, &(pDispParams->rgvarg[0]));
					else
						hr = E_FAIL;

					return hr;
				}
			}
		}

		return E_FAIL;
	}

	return hr;
}