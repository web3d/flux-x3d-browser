/********************************************************************************
 * Flux
 *
 * File: keyboard.h
 * Description: Keyboard manager
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


#ifndef _KEYBOARD_H
#define _KEYBOARD_H

/**
 * Any object that implements this interface can receive input
 * from the keyboard.
 */
struct iKeyboardReceiver
{
	virtual void KeyUp( int key ){};
	virtual void KeyDown( int key ){};
};

#define ANMKEY_NOKEY	-1
#define ANMKEY_HOME		0
#define ANMKEY_END		1
#define ANMKEY_PGUP		2
#define ANMKEY_PGDN		3
#define ANMKEY_UP		4
#define ANMKEY_DOWN		5
#define ANMKEY_LEFT		6
#define ANMKEY_RIGHT	7
#define ANMKEY_F1		8
#define ANMKEY_F2		9
#define ANMKEY_F3		10
#define ANMKEY_F4		11
#define ANMKEY_F5		12
#define ANMKEY_F6		13
#define ANMKEY_F7		14
#define ANMKEY_F8		15
#define ANMKEY_F9		16
#define ANMKEY_F10		17
#define ANMKEY_F11		18
#define ANMKEY_F12		19
#define ANMKEY_LSHIFT	20
#define ANMKEY_RSHIFT	21
#define ANMKEY_LCONTROL	22
#define ANMKEY_RCONTROL	23
#define ANMKEY_LALT		24
#define ANMKEY_RALT		25
#define ANMKEY_DELETE	26
#define ANMKEY_BACK		27
#define ANMKEY_ESCAPE	28


#define ANMKEY_0	'0'
#define ANMKEY_1	'1'
#define ANMKEY_2	'2'
#define ANMKEY_3	'3'
#define ANMKEY_4	'4'
#define ANMKEY_5	'5'
#define ANMKEY_6	'6'
#define ANMKEY_7	'7'
#define ANMKEY_8	'8'
#define ANMKEY_9	'9'
#define ANMKEY_A	'A'
#define ANMKEY_B	'B'
#define ANMKEY_C	'C'
#define ANMKEY_D	'D'
#define ANMKEY_E	'E'
#define ANMKEY_F	'F'
#define ANMKEY_G	'G'
#define ANMKEY_H	'H'
#define ANMKEY_I	'I'
#define ANMKEY_J	'J'
#define ANMKEY_K	'K'
#define ANMKEY_L	'L'
#define ANMKEY_M	'M'
#define ANMKEY_N	'N'
#define ANMKEY_O	'O'
#define ANMKEY_P	'P'
#define ANMKEY_Q	'Q'
#define ANMKEY_R	'R'
#define ANMKEY_S	'S'
#define ANMKEY_T	'T'
#define ANMKEY_U	'U'
#define ANMKEY_V	'V'
#define ANMKEY_W	'W'
#define ANMKEY_X	'X'
#define ANMKEY_Y	'Y'
#define ANMKEY_Z	'Z'

#define ANMKEY_SPACE	' '
#define ANMKEY_ASTERISK	'*'
#define ANMKEY_MINUS	'-'
#define ANMKEY_PLUS		'+'
#define ANMKEY_EQUALS	'='
#define ANMKEY_SEMICOLON	';'
#define ANMKEY_APOSTROPHE	'\''
#define ANMKEY_BACKSLASH	'\\'
#define ANMKEY_COMMA	','
#define ANMKEY_PERIOD	'.'
#define ANMKEY_SLASH	'/'	// on main keyboard 
#define ANMKEY_LBRACKET	'['
#define ANMKEY_RBRACKET	']'
#define ANMKEY_RETURN	0x1C    /* Enter on main keyboard  */

class EXPORT cKeyboard  
{
protected :

	std::list<iKeyboardReceiver*>		m_receivers;

	class cApplication		*m_app;
	cKeyboard( class cApplication *pApp, HWND hWnd );

public:

	virtual ~cKeyboard();

	// Overridables - subclass MUST supply
	// Poll to see if a certain key is down
	virtual void ClearTable() PURE;
	virtual bool Poll( int key ) PURE;
	virtual eResult Update() PURE;
	// Virtual key help
	virtual char TranslateVirtualKey(int key) PURE;

	// Other methods
	// Use this to establish a KeyboardReceiver as the current input focus
	void AddReceiver( iKeyboardReceiver* pTarget );
	void RemoveReceiver( iKeyboardReceiver* pTarget );


};

#endif //_KEYBOARD_H
