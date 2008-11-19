/********************************************************************************
 * Flux
 *
 * File: X3DSFNode.cpp
 * Description: Implementation of CX3DSFNode
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

// X3DSFNode.cpp : Implementation of CX3DSFNode
#include "stdafx.h"
#include "Anima.h"
#include "X3DSFNode.h"
#include "x3dnode.h"

/////////////////////////////////////////////////////////////////////////////
// CX3DSFNode


STDMETHODIMP CX3DSFNode::getValue(X3DNode **value)
{
	eAnmFieldBaseSetValue setval;

	if (value == NULL)
		return S_FALSE;
	
	CAnmNode *nodeval;

	nodeval = m_nodevalue; // covers ANIMFIELDBASE_DIY case
	setval = X3DFieldBase::getValue((void **) &nodeval);

	if (setval == ANMFIELDBASE_CANTDO)
		return S_FALSE;
	
	if (nodeval == NULL)
	{
		*value = NULL;
		return S_OK;
	}


	CAnmSAINode *pSAINode = new CAnmSAINode(nodeval);
	pSAINode->SetSink(m_node->GetSink());

	X3DNode *pX3DNode = CX3DNode::CreateX3DNode(pSAINode, m_context);
	if (pX3DNode)
	{
		*value = pX3DNode;
		return S_OK;
	}
	else
		return E_FAIL;
}

STDMETHODIMP CX3DSFNode::setValue(X3DNode *value)
{
	// krv:  QUESTION:  why can't we set the Node to NULL in the SAI??????
	//
	if (value == NULL)
		return S_FALSE;



	// krv:  Get SAI Node first, then get native node.
	//  It crashes much less that way.

	CAnmSAINode *pSAINode = NULL;

	HRESULT hr = value->getNativeNode(&pSAINode);
	if ( FAILED(hr) || pSAINode == NULL ) {
		return S_FALSE;
	}

	// Now get the anmNode
	//
	CAnmNode* pAnmNode = pSAINode->GetNode();
	if ( pAnmNode == NULL ) {
		return S_FALSE;
	}

	m_nodevalue = pAnmNode;

	if (X3DFieldBase::setValue((void *) &pAnmNode) != ANMFIELDBASE_CANTDO) {
		return S_OK;
	}
	else {
		return S_FALSE;
	}
}

void CX3DSFNode::handleCallback(class CAnmObject *sourceObject, CLASSMEMBERID reason,
								  void *callData)

{
	assert(sourceObject == m_node->GetNode());
	assert(callData != NULL);
	
	m_nodevalue = *(Node *) callData;

	X3DFieldBase::callCallbacks(this);
}
