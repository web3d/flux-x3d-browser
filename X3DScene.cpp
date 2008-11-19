/********************************************************************************
 * Flux
 *
 * File: X3DScene.cpp
 * Description: Implementation of CX3DScene
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

// X3DScene.cpp : Implementation of CX3DScene
#include "stdafx.h"
#include "Anima.h"
#include "X3DScene.h"

#include "anmsaiscene.h"
#include "anmsainode.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DScene


STDMETHODIMP CX3DScene::getMetaData(BSTR key,  BSTR *value)
{
	// TODO: Add your implementation code here
	CComBSTR bstr;

	CAnmSAIScene *pSAIScene = (CAnmSAIScene *) m_context;
	assert(pSAIScene);

	int keylen = wcslen(key);
	char *cpkey = AnmAllocString(keylen + 1);
	bstr2cstr(cpkey, key, keylen);
	String s = pSAIScene->GetMetaData(cpkey);

	if (s == NULL)
		return S_FALSE;

	bstr = s->GetBuf();

	*value = bstr.Copy();

	return S_OK;
}


STDMETHODIMP CX3DScene::setMetaData(BSTR key, BSTR value)
{
	// TODO: Add your implementation code here

	String skey, svalue;
	skey = new CAnmString();
	svalue = new CAnmString();

	int keylen = wcslen(key);
	int valuelen = wcslen(value);

	skey->alloc(keylen + 1);
	bstr2cstr(skey->GetBuf(), key, keylen);
	svalue->alloc(valuelen + 1);
	bstr2cstr(svalue->GetBuf(), value, valuelen);

	return S_OK;
}

STDMETHODIMP CX3DScene::addRootNode(X3DNode *node)
{
	if (node == NULL)
		return E_INVALIDARG;

	CAnmSAINode *pSAINode = NULL;
	HRESULT hr = node->getNativeNode(&pSAINode);
	if (SUCCEEDED(hr))
	{
		CAnmSAIScene *pSAIScene = (CAnmSAIScene *) m_context;
		assert(pSAIScene);

		pSAIScene->AddRootNode(pSAINode);

		node->setContext(m_x3dcontext);

		return S_OK;
	}
	else
		return E_INVALIDARG;
}

STDMETHODIMP CX3DScene::removeRootNode(X3DNode *node)
{
	if (node == NULL)
		return E_INVALIDARG;

	CAnmSAINode *pSAINode = NULL;
	HRESULT hr = node->getNativeNode(&pSAINode);
	if (SUCCEEDED(hr))
	{
		CAnmSAIScene *pSAIScene = (CAnmSAIScene *) m_context;
		assert(pSAIScene);

		pSAIScene->RemoveRootNode(pSAINode);

		return S_OK;
	}
	else
		return E_INVALIDARG;
}