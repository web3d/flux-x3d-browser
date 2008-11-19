/********************************************************************************
 * Flux
 *
 * File: anmeaseineaseout.cpp
 * Description: EaseInEaseOut Interpolator class
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
#include "anmeaseineaseout.h"


CAnmEaseInEaseOut::CAnmEaseInEaseOut()
: CAnmNode()
{
	m_key = new FloatArray;
	m_easeInEaseOut = new Point2Array;
}

CAnmEaseInEaseOut::~CAnmEaseInEaseOut()
{
	SafeUnRef(m_key);
	SafeUnRef(m_easeInEaseOut);
}

// CAnmNode overrides
void CAnmEaseInEaseOut::Realize()
{
	CAnmNode::Realize();
}



void CAnmEaseInEaseOut::SetKey(FloatArray *pKey)
{
	SafeUnRef(m_key);
	m_key = pKey;
	m_key->Ref();
}

void CAnmEaseInEaseOut::SetEaseInEaseOut(Point2Array *pEaseInEaseOut)
{
	SafeUnRef(m_easeInEaseOut);
	m_easeInEaseOut = pEaseInEaseOut;
	m_easeInEaseOut->Ref();
}




static Float EaseInEaseOut( Float u, Float easeOut, Float easeIn )
{
	// u is the LOCAL fraction of the span between keyA and KeyB.
	// u is Zero at keyA, and One at keyB.
	// easeOut is Ease out from Previous Key, keyA
	// easeIn is Ease in from Next Key, keyB

	// This returns the modified local fraction between the two keys.

	Float sum = easeOut + easeIn;
	if( sum < 0.000001f ) {
		return u;
	}
	if( sum > 1.0 ) {
		easeOut /= sum;
		easeIn /= sum;
	}

	Float tmp = 1.0 / ( 2.0 - easeOut - easeIn );
	if( u < easeOut ) {
		return ( ( tmp/easeOut ) *u*u );
	}
	else if( u < 1.0 - easeIn ) {
		return ( tmp*( 2.0*u - easeOut ));
	}
	else {
		u = 1.0 - u;
		return  ( 1.0 - ( tmp/easeIn )*u*u );
	}
}


void CAnmEaseInEaseOut::SetFraction(Float fraction)
{
	// this is where the interpolation gets done
	Float modifiedFraction = 0.0f;

	int i;
	Float delta, ratio;

	int numKeys = m_key->size();
	if (numKeys > m_easeInEaseOut->size()) {
		numKeys = m_easeInEaseOut->size();
	}

	if (numKeys)
	{
		for(i = 0; i < numKeys; i++)
		{
			if( fraction < (*m_key)[i])
				break;
		}

		if( i==0 )
		{
			modifiedFraction = 0.0f;		
		}
		else if ( i == numKeys )
		{
			modifiedFraction = 1.0f;		
		}
		else
		{
			delta = (*m_key)[i] - (*m_key)[i-1];
			if( delta == 0.f) {
				ratio = 0.f;
			}
			else {
				ratio = (fraction - (*m_key)[i-1]) / delta;
			}


			float localFraction = EaseInEaseOut( ratio, (*m_easeInEaseOut)[i-1].y, (*m_easeInEaseOut)[i].x );
			modifiedFraction = (*m_key)[i-1] + localFraction * delta;
		}
	}




	CallCallbacks(CALLBACKID(CAnmEaseInEaseOut, modifiedFraction_changed), &modifiedFraction);
}


// Tables for Callbacks, Methods and Set/GetValues

DEFINE_VALUE(CAnmEaseInEaseOut, key, eValueFloatArray, FloatArray *, GetKey, SetKey);
DEFINE_VALUE(CAnmEaseInEaseOut, easeInEaseOut, eValuePoint2Array, Point2Array *, GetEaseInEaseOut, SetEaseInEaseOut);

DEFINE_METHOD(CAnmEaseInEaseOut, set_fraction, eValueFloat, Float, SetFraction);
DEFINE_METHOD(CAnmEaseInEaseOut, set_key, eValueFloatArray, FloatArray *, SetKey);
DEFINE_METHOD(CAnmEaseInEaseOut, set_easeInEaseOut, eValuePoint2Array, Point2Array *, SetEaseInEaseOut);

DEFINE_CALLBACK(CAnmEaseInEaseOut, key_changed, eValueFloatArray);
DEFINE_CALLBACK(CAnmEaseInEaseOut, easeInEaseOut_changed, eValuePoint2Array);
DEFINE_CALLBACK(CAnmEaseInEaseOut, modifiedFraction_changed, eValueFloat);


BEGIN_DEFINE_NODE_MEMBERS(CAnmEaseInEaseOut)
VALUEID(CAnmEaseInEaseOut, key),
VALUEID(CAnmEaseInEaseOut, easeInEaseOut),
METHODID(CAnmEaseInEaseOut, set_fraction),
METHODID(CAnmEaseInEaseOut, set_key),
METHODID(CAnmEaseInEaseOut, set_easeInEaseOut),
CALLBACKID(CAnmEaseInEaseOut, key_changed),
CALLBACKID(CAnmEaseInEaseOut, easeInEaseOut_changed),
CALLBACKID(CAnmEaseInEaseOut, modifiedFraction_changed),
END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmEaseInEaseOut, CAnmNode);
