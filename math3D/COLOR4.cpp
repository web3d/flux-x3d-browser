/*******************************************************************
 *         Advanced 3D Game Programming using DirectX 7.0
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Title: color4.h
 *    Desc: RGBA color primitive using floats
 *          
 * copyright (c) 1999 by Adrian Perez
 * See license.txt for modification and distribution information
 ******************************************************************/

#include "stdafx.h"
#include "color4.h"

const color4 color4::Black( 0.f, 0.f, 0.f, 1.f );
const color4 color4::Gray( 0.5f, 0.5f, 0.5f, 1.f );
const color4 color4::White( 1.f, 1.f, 1.f, 1.f );
const color4 color4::Red( 1.f, 0.f, 0.f, 1.f );
const color4 color4::Green( 0.f, 1.f, 0.f, 1.f );
const color4 color4::Blue( 0.f, 0.f, 1.f, 1.f );
const color4 color4::Magenta( 1.f, 0.f, 1.f, 1.f );
const color4 color4::Cyan( 0.f, 1.f, 1.f, 1.f );
const color4 color4::Yellow( 1.f, 1.f, 0.f, 1.f );

const color4 &color4::GetBlack()
{
	return Black;
}
