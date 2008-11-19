/********************************************************************************
 * Flux
 *
 * File: anmlayout.cpp
 * Description: Layout node
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
#include "anmlayout.h"



CAnmLayout::CAnmLayout() : CAnmNode(),
	m_offset( ANMLAYOUT_DEFAULT_OFFSET ),
	m_size( ANMLAYOUT_DEFAULT_SIZE )
{
	// Init the fields
	//
	m_offsetUnits = new StringArray;
	m_sizeUnits = new StringArray;

	m_offsetUnits->push_back(new CAnmString(ANMLAYOUT_DEFAULT_OFFSETUNITS));
	m_offsetUnits->push_back(new CAnmString(ANMLAYOUT_DEFAULT_OFFSETUNITS));

	m_sizeUnits->push_back(new CAnmString(ANMLAYOUT_DEFAULT_SIZEUNITS));
	m_sizeUnits->push_back(new CAnmString(ANMLAYOUT_DEFAULT_SIZEUNITS));



	m_align =		new StringArray;
	m_scaleMode =	new StringArray;

	m_align->push_back(new CAnmString(ANMLAYOUT_DEFAULT_UALIGNMENT));
	m_align->push_back(new CAnmString(ANMLAYOUT_DEFAULT_VALIGNMENT));

	m_scaleMode->push_back(new CAnmString(ANMLAYOUT_DEFAULT_SCALE_MODE));
	m_scaleMode->push_back(new CAnmString(ANMLAYOUT_DEFAULT_SCALE_MODE));

}

CAnmLayout::~CAnmLayout()
{
	SafeUnRef(m_offsetUnits);
	SafeUnRef(m_sizeUnits);

	SafeUnRef(m_align);
	SafeUnRef(m_scaleMode);
}


void CAnmLayout::Realize()
{
	CAnmNode::Realize();
}

eScaleMode CAnmLayout::GetScaleMode( String str )
{
	if( str ) {
		switch( str->GetBuf()[0] ) {
			case 'P':
			case 'p':
				return eScaleModePixel;

			case 'F':
			case 'f':
				return eScaleModeFraction;

			case 's':
			case 'S':
				return eScaleModeStretch;
		}
	}
	return eScaleModeNone;
}



euAlignmentMode CAnmLayout::GetuAlignmentMode( String str )
{
	if( str ) {
		switch( str->GetBuf()[0] ) {
			case 'L':
			case 'l':
				return euAlignmentLeft;

			case 'C':
			case 'c':
			case 'M':
			case 'm':
				return euAlignmentCenter;

			case 'R':
			case 'r':
				return euAlignmentRight;
		}
	}
	return euAlignmentLeft;
}

evAlignmentMode CAnmLayout::GetvAlignmentMode( String str )
{
	if( str ) {
		switch( str->GetBuf()[0] ) {
			case 'B':
			case 'b':
				return evAlignmentBottom;

			case 'C':
			case 'c':
			case 'M':
			case 'm':
				return evAlignmentMiddle;

			case 'T':
			case 't':
				return evAlignmentTop;
		}
	}
	return evAlignmentBottom;
}

eUnitsMode CAnmLayout::GetUnitsMode( String str )
{
	if( str ) {
		switch( str->GetBuf()[0] ) {
			case 'W':
			case 'w':
				return eUnitsWorld;

			case 'F':
			case 'f':
				return eUnitsFraction;

			case 'P':
			case 'p':
				return eUnitsPixel;
		}
	}
	return eUnitsWorld;

}



// Access:

void CAnmLayout::SetOffset(Point2 p)
{
	m_offset = p;
	SetStateDirty();
	NotifyParentsOfChange();

	CallCallbacks(CALLBACKID(CAnmLayout, offset_changed), &p);
}

void CAnmLayout::SetSize(Point2 p)
{
	m_size = p;
	SetStateDirty();
	NotifyParentsOfChange();

	CallCallbacks(CALLBACKID(CAnmLayout, size_changed), &p);
}


void CAnmLayout::SetOffsetUnits(StringArray *pOffsetUnits)
{
	assert(pOffsetUnits != NULL);

	SafeUnRef(m_offsetUnits);
	m_offsetUnits = pOffsetUnits;
	m_offsetUnits->Ref();
	SetStateDirty();
	NotifyParentsOfChange();

	CallCallbacks(CALLBACKID(CAnmLayout, offsetUnits_changed), pOffsetUnits);
}

void CAnmLayout::SetSizeUnits(StringArray *pSizeUnits)
{
	assert(pSizeUnits != NULL);

	SafeUnRef(m_sizeUnits);
	m_sizeUnits = pSizeUnits;
	m_sizeUnits->Ref();
	SetStateDirty();
	NotifyParentsOfChange();

	CallCallbacks(CALLBACKID(CAnmLayout, sizeUnits_changed), pSizeUnits);
}



void CAnmLayout::SetAlign(StringArray * align)
{
	SafeUnRef(m_align);
	m_align = align;
	m_align->Ref();
	SetStateDirty();
	NotifyParentsOfChange();

	CallCallbacks(CALLBACKID(CAnmLayout, align_changed), align);
}

void CAnmLayout::SetScaleMode(StringArray * scaleMode)
{
	SafeUnRef(m_scaleMode);
	m_scaleMode = scaleMode;
	m_scaleMode->Ref();
	SetStateDirty();
	NotifyParentsOfChange();

	CallCallbacks(CALLBACKID(CAnmLayout, scaleMode_changed), scaleMode);
}



DEFINE_VALUE(CAnmLayout, offset, eValuePoint2, Point2, GetOffset, SetOffset);
DEFINE_VALUE(CAnmLayout, size,   eValuePoint2, Point2, GetSize,   SetSize);
DEFINE_VALUE(CAnmLayout, offsetUnits, eValueStringArray, StringArray*, GetOffsetUnits, SetOffsetUnits);
DEFINE_VALUE(CAnmLayout, sizeUnits, eValueStringArray, StringArray*, GetSizeUnits, SetSizeUnits);
DEFINE_VALUE(CAnmLayout, align, eValueStringArray, StringArray*, GetAlign, SetAlign);
DEFINE_VALUE(CAnmLayout, scaleMode, eValueStringArray, StringArray*, GetScaleMode, SetScaleMode);



DEFINE_METHOD(CAnmLayout, set_offset, eValuePoint2, Point2, SetOffset);
DEFINE_CALLBACK(CAnmLayout, offset_changed, eValuePoint2);

DEFINE_METHOD(CAnmLayout, set_size, eValuePoint2, Point2, SetSize);
DEFINE_CALLBACK(CAnmLayout, size_changed, eValuePoint2);



DEFINE_METHOD(CAnmLayout, set_offsetUnits, eValueStringArray, StringArray*, SetOffsetUnits);
DEFINE_CALLBACK(CAnmLayout, offsetUnits_changed, eValueStringArray);

DEFINE_METHOD(CAnmLayout, set_sizeUnits, eValueStringArray, StringArray*, SetSizeUnits);
DEFINE_CALLBACK(CAnmLayout, sizeUnits_changed, eValueStringArray);





DEFINE_METHOD(CAnmLayout, set_align, eValueStringArray, StringArray*, SetAlign);
DEFINE_CALLBACK(CAnmLayout, align_changed, eValueStringArray);


DEFINE_METHOD(CAnmLayout, set_scaleMode, eValueStringArray, StringArray*, SetScaleMode);
DEFINE_CALLBACK(CAnmLayout, scaleMode_changed, eValueStringArray);







BEGIN_DEFINE_NODE_MEMBERS(CAnmLayout)

VALUEID(CAnmLayout, offset),
VALUEID(CAnmLayout, size),
VALUEID(CAnmLayout, offsetUnits),
VALUEID(CAnmLayout, sizeUnits),
VALUEID(CAnmLayout, align),
VALUEID(CAnmLayout, scaleMode),

END_DEFINE_MEMBERS

DEFINE_CLASS(CAnmLayout, CAnmNode);


