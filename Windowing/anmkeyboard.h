/********************************************************************************
 * Flux
 *
 * File: anmkeyboard.h
 * Description: Keyboard support (Windows)
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

typedef SHORT ANMKEYSTATE;
typedef int ANMKEY;

#ifndef _anmkeyboard_h
#define _anmkeyboard_h

inline ANMKEYSTATE AnmGetKeyState(ANMKEY k)
{
	return GetAsyncKeyState(k);
}
 
#define ANMKEYDOWN(ks) (ks & 0x8000)

#define ANMKEY_SHIFT VK_SHIFT

/*
	unsigned				 m_shiftKeyDown:1;
	unsigned				 m_ctrlKeyDown:1;
	unsigned				 m_altKeyDown:1;
	unsigned				 m_pgDnKey:1;
	unsigned				 m_pgUpKey:1;
	unsigned				 m_homeKey:1;
	unsigned				 m_endKey:1;
	unsigned				 m_spaceKey:1;
	unsigned				 m_upArrowKey:1;
	unsigned				 m_downArrowKey:1;
	unsigned				 m_leftArrowKey:1;
	unsigned				 m_rightArrowKey:1;
*/

#endif // _anmkeyboard_h