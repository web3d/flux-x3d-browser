/********************************************************************************
 * Flux
 *
 * File: anmnormal.cpp
 * Description: Normal node
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

#include "stdafx.h"
#include "anmnormal.h"
#include "anmindexedfaceset.h"

CAnmNormal::CAnmNormal() : CAnmNode () // Parent IFS will add me
{
	m_vector = new Point3Array;
}

CAnmNormal::~CAnmNormal()
{
	SafeUnRef(m_vector);
}

void CAnmNormal::Realize()
{
	CAnmNode::Realize();
}

void CAnmNormal::Update()
{
	ClearStateDirty();
}


void CAnmNormal::SetVector(Point3Array *pVector)
{
	assert(pVector != NULL);

	SafeUnRef(m_vector);
	m_vector = pVector;
	m_vector->Ref();

	SetStateDirty();
	NotifyParentsOfChange();

	CallCallbacks(CALLBACKID(CAnmNormal, vector_changed), &pVector);
}

DEFINE_VALUE(CAnmNormal, vector, eValuePoint3Array, Point3Array*, 
			 GetVector, SetVector);
DEFINE_METHOD(CAnmNormal, set_vector, eValuePoint3Array, Point3Array*, SetVector);
DEFINE_CALLBACK(CAnmNormal, vector_changed, eValuePoint3Array);

BEGIN_DEFINE_NODE_MEMBERS(CAnmNormal)
VALUEID(CAnmNormal, vector), 
METHODID(CAnmNormal, set_vector), 
CALLBACKID(CAnmNormal, vector_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmNormal, CAnmNode);
