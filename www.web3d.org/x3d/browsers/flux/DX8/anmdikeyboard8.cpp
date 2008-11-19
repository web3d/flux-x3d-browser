/********************************************************************************
 * Flux
 *
 * File: anmdikeyboard9.cpp
 * Description: Keyboard class for DirectX Version 9
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
#include "anmdikeyboard8.h"
#include "anmdinput8.h"
#include "anmdx8message.h"

// N.B.: SHOULD TRANSLATE EVERY ONE OF THESE KEYS to a virtualKey field
#define ANMKEY_ 0 /* until we xlat them all */
static unsigned char xlatDIKtoANMkey[256] =
{
	ANMKEY_,	// no key
	ANMKEY_ESCAPE,	// DIK_ESCAPE      0x01
	ANMKEY_1,	// DIK_1               0x02
	ANMKEY_2,	// DIK_2               0x03
	ANMKEY_3,	// DIK_3               0x04
	ANMKEY_4,	// DIK_4               0x05
	ANMKEY_5,	// DIK_5               0x06
	ANMKEY_6,	// DIK_6               0x07
	ANMKEY_7,	// DIK_7               0x08
	ANMKEY_8,	// DIK_8               0x09
	ANMKEY_9,	// DIK_9               0x0A
	ANMKEY_0,	// DIK_0               0x0B
	ANMKEY_MINUS,	// DIK_MINUS           0x0C    /* - on main keyboard 
	ANMKEY_EQUALS,	// DIK_EQUALS          0x0D
	ANMKEY_BACK,	// DIK_BACK            0x0E    /* backspace 
	ANMKEY_,	// DIK_TAB             0x0F
	ANMKEY_Q,	// DIK_Q               0x10
	ANMKEY_W,	// DIK_W               0x11
	ANMKEY_E,	// DIK_E               0x12
	ANMKEY_R,	// DIK_R               0x13
	ANMKEY_T,	// DIK_T               0x14
	ANMKEY_Y,	// DIK_Y               0x15
	ANMKEY_U,	// DIK_U               0x16
	ANMKEY_I,	// DIK_I               0x17
	ANMKEY_O,	// DIK_O               0x18
	ANMKEY_P,	// DIK_P               0x19
	ANMKEY_LBRACKET,	// DIK_LBRACKET        0x1A
	ANMKEY_RBRACKET,	// DIK_RBRACKET        0x1B
	ANMKEY_RETURN,	// DIK_RETURN          0x1C    /* Enter on main keyboard 
	ANMKEY_LCONTROL,	// DIK_LCONTROL        0x1D
	ANMKEY_A,	// DIK_A               0x1E
	ANMKEY_S,	// DIK_S               0x1F
	ANMKEY_D,	// DIK_D               0x20
	ANMKEY_F,	// DIK_F               0x21
	ANMKEY_G,	// DIK_G               0x22
	ANMKEY_H,	// DIK_H               0x23
	ANMKEY_J,	// DIK_J               0x24
	ANMKEY_K,	// DIK_K               0x25
	ANMKEY_L,	// DIK_L               0x26
	ANMKEY_SEMICOLON,	// DIK_SEMICOLON       0x27
	ANMKEY_APOSTROPHE,	// DIK_APOSTROPHE      0x28
	ANMKEY_,	// DIK_GRAVE           0x29    /* accent grave 
	ANMKEY_LSHIFT,	// DIK_LSHIFT          0x2A
	ANMKEY_BACKSLASH,	// DIK_BACKSLASH       0x2B
	ANMKEY_Z,	// DIK_Z               0x2C
	ANMKEY_X,	// DIK_X               0x2D
	ANMKEY_C,	// DIK_C               0x2E
	ANMKEY_V,	// DIK_V               0x2F
	ANMKEY_B,	// DIK_B               0x30
	ANMKEY_N,	// DIK_N               0x31
	ANMKEY_M,	// DIK_M               0x32
	ANMKEY_COMMA,	// DIK_COMMA           0x33
	ANMKEY_PERIOD,	// DIK_PERIOD          0x34    /* . on main keyboard 
	ANMKEY_SLASH,	// DIK_SLASH           0x35    /* / on main keyboard 
	ANMKEY_RSHIFT,	// DIK_RSHIFT          0x36
	ANMKEY_ASTERISK,	// DIK_MULTIPLY        0x37    /* * on numeric keypad 
	ANMKEY_LALT,	// DIK_LMENU           0x38    /* left Alt 
	ANMKEY_SPACE,	// DIK_SPACE           0x39
	ANMKEY_,	// DIK_CAPITAL         0x3A
	ANMKEY_F1,	// DIK_F1              0x3B
	ANMKEY_F2,	// DIK_F2              0x3C
	ANMKEY_F3,	// DIK_F3              0x3D
	ANMKEY_F4,	// DIK_F4              0x3E
	ANMKEY_F5,	// DIK_F5              0x3F
	ANMKEY_F6,	// DIK_F6              0x40
	ANMKEY_F7,	// DIK_F7              0x41
	ANMKEY_F8,	// DIK_F8              0x42
	ANMKEY_F9,	// DIK_F9              0x43
	ANMKEY_F10,	// DIK_F10             0x44
	ANMKEY_,	// DIK_NUMLOCK         0x45
	ANMKEY_,	// DIK_SCROLL          0x46    /* Scroll Lock 
	ANMKEY_7,	// DIK_NUMPAD7         0x47
	ANMKEY_8,	// DIK_NUMPAD8         0x48
	ANMKEY_9,	// DIK_NUMPAD9         0x49
	ANMKEY_MINUS,	// DIK_SUBTRACT        0x4A    /* - on numeric keypad 
	ANMKEY_4,	// DIK_NUMPAD4         0x4B
	ANMKEY_5,	// DIK_NUMPAD5         0x4C
	ANMKEY_6,	// DIK_NUMPAD6         0x4D
	ANMKEY_PLUS,	// DIK_ADD             0x4E    /* + on numeric keypad 
	ANMKEY_1,	// DIK_NUMPAD1         0x4F
	ANMKEY_2,	// DIK_NUMPAD2         0x50
	ANMKEY_3,	// DIK_NUMPAD3         0x51
	ANMKEY_0,	// DIK_NUMPAD0         0x52
	ANMKEY_PERIOD,	// DIK_DECIMAL         0x53    /* . on numeric keypad 
	ANMKEY_,		// ?				0x54
	ANMKEY_,		// ?				0x55
	ANMKEY_,	// DIK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) 
	ANMKEY_F11,	// DIK_F11             0x57
	ANMKEY_F12,	// DIK_F12             0x58
	ANMKEY_,		// ?				0x59
	ANMKEY_,		// ?				0x5a
	ANMKEY_,		// ?				0x5b
	ANMKEY_,		// ?				0x5c
	ANMKEY_,		// ?				0x5d
	ANMKEY_,		// ?				0x5e
	ANMKEY_,		// ?				0x5f
	ANMKEY_,		// ?				0x60
	ANMKEY_,		// ?				0x61
	ANMKEY_,		// ?				0x62
	ANMKEY_,		// ?				0x63
	ANMKEY_,	// DIK_F13             0x64    /*                     (NEC PC98) 
	ANMKEY_,	// DIK_F14             0x65    /*                     (NEC PC98) 
	ANMKEY_,	// DIK_F15             0x66    /*                     (NEC PC98) 
	ANMKEY_,		// ?				0x67
	ANMKEY_,		// ?				0x68
	ANMKEY_,		// ?				0x69
	ANMKEY_,		// ?				0x6a
	ANMKEY_,		// ?				0x6b
	ANMKEY_,		// ?				0x6c
	ANMKEY_,		// ?				0x6d
	ANMKEY_,		// ?				0x6e
	ANMKEY_,		// ?				0x6f
	ANMKEY_,	// DIK_KANA            0x70    /* (Japanese keyboard)            
	ANMKEY_,		// ?				0x71
	ANMKEY_,		// ?				0x72
	ANMKEY_,	// DIK_ABNT_C1         0x73    /* /? on Brazilian keyboard 
	ANMKEY_,		// ?				0x74
	ANMKEY_,		// ?				0x75
	ANMKEY_,		// ?				0x76
	ANMKEY_,		// ?				0x77
	ANMKEY_,		// ?				0x78
	ANMKEY_,	// DIK_CONVERT         0x79    /* (Japanese keyboard)            
	ANMKEY_,		// ?				0x7a
	ANMKEY_,	// DIK_NOCONVERT       0x7B    /* (Japanese keyboard)            
	ANMKEY_,		// ?				0x7c
	ANMKEY_,	// DIK_YEN             0x7D    /* (Japanese keyboard)            
	ANMKEY_,	// DIK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard 
	ANMKEY_,		// ?				0x7f
	ANMKEY_,		// ?				0x80
	ANMKEY_,		// ?				0x81
	ANMKEY_,		// ?				0x82
	ANMKEY_,		// ?				0x83
	ANMKEY_,		// ?				0x84
	ANMKEY_,		// ?				0x85
	ANMKEY_,		// ?				0x86
	ANMKEY_,		// ?				0x87
	ANMKEY_,		// ?				0x88
	ANMKEY_,		// ?				0x89
	ANMKEY_,		// ?				0x8a
	ANMKEY_,		// ?				0x8b
	ANMKEY_,		// ?				0x8c
	ANMKEY_,	// DIK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) 
	ANMKEY_,		// ?				0x8e
	ANMKEY_,		// ?				0x8f
	ANMKEY_,	// DIK_PREVTRACK       0x90    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) 
	ANMKEY_,	// DIK_AT              0x91    /*                     (NEC PC98) 
	ANMKEY_,	// DIK_COLON           0x92    /*                     (NEC PC98) 
	ANMKEY_,	// DIK_UNDERLINE       0x93    /*                     (NEC PC98) 
	ANMKEY_,	// DIK_KANJI           0x94    /* (Japanese keyboard)            
	ANMKEY_,	// DIK_STOP            0x95    /*                     (NEC PC98) 
	ANMKEY_,	// DIK_AX              0x96    /*                     (Japan AX) 
	ANMKEY_,	// DIK_UNLABELED       0x97    /*                        (J3100) 
	ANMKEY_,		// ?				0x98
	ANMKEY_,	// DIK_NEXTTRACK       0x99    /* Next Track 
	ANMKEY_,		// ?				0x9a
	ANMKEY_,		// ?				0x9b
	ANMKEY_RETURN,	// DIK_NUMPADENTER     0x9C    /* Enter on numeric keypad 
	ANMKEY_RCONTROL,	// DIK_RCONTROL        0x9D
	ANMKEY_,		// ?				0x9e
	ANMKEY_,		// ?				0x9f
	ANMKEY_,	// DIK_MUTE            0xA0    /* Mute 
	ANMKEY_,	// DIK_CALCULATOR      0xA1    /* Calculator 
	ANMKEY_,	// DIK_PLAYPAUSE       0xA2    /* Play / Pause 
	ANMKEY_,		// ?				0xa3
	ANMKEY_,	// DIK_MEDIASTOP       0xA4    /* Media Stop 
	ANMKEY_,		// ?				0xa5
	ANMKEY_,		// ?				0xa6
	ANMKEY_,		// ?				0xa7
	ANMKEY_,		// ?				0xa8
	ANMKEY_,		// ?				0xa9
	ANMKEY_,		// ?				0xaa
	ANMKEY_,		// ?				0xab
	ANMKEY_,		// ?				0xac
	ANMKEY_,		// ?				0xad
	ANMKEY_,	// DIK_VOLUMEDOWN      0xAE    /* Volume - 
	ANMKEY_,		// ?				0xaf
	ANMKEY_,	// DIK_VOLUMEUP        0xB0    /* Volume + 
	ANMKEY_,		// ?				0xb1
	ANMKEY_,	// DIK_WEBHOME         0xB2    /* Web home 
	ANMKEY_COMMA,	// DIK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) 
	ANMKEY_,		// ?				0xb4
	ANMKEY_SLASH,	// DIK_DIVIDE          0xB5    /* / on numeric keypad 
	ANMKEY_,		// ?				0xb6
	ANMKEY_,	// DIK_SYSRQ           0xB7
	ANMKEY_RALT,	// DIK_RMENU           0xB8    /* right Alt 
	ANMKEY_,		// ?				0xb9
	ANMKEY_,		// ?				0xba
	ANMKEY_,		// ?				0xbb
	ANMKEY_,		// ?				0xbc
	ANMKEY_,		// ?				0xbd
	ANMKEY_,		// ?				0xbe
	ANMKEY_,		// ?				0xbf
	ANMKEY_,		// ?				0xc0
	ANMKEY_,		// ?				0xc1
	ANMKEY_,		// ?				0xc2
	ANMKEY_,		// ?				0xc3
	ANMKEY_,		// ?				0xc4
	ANMKEY_,	// DIK_PAUSE           0xC5    /* Pause 
	ANMKEY_,		// ?				0xc6
	ANMKEY_HOME,	// DIK_HOME            0xC7    /* Home on arrow keypad 
	ANMKEY_UP,		// DIK_UP              0xC8    /* UpArrow on arrow keypad 
	ANMKEY_PGUP,	// DIK_PRIOR           0xC9    /* PgUp on arrow keypad 
	ANMKEY_,		// ?				0xca
	ANMKEY_LEFT,	// DIK_LEFT            0xCB    /* LeftArrow on arrow keypad 
	ANMKEY_,		// ?				0xcc
	ANMKEY_RIGHT,	// DIK_RIGHT           0xCD    /* RightArrow on arrow keypad 
	ANMKEY_,		// ?				0xce
	ANMKEY_END,		// DIK_END             0xCF    /* End on arrow keypad 
	ANMKEY_DOWN,	// DIK_DOWN            0xD0    /* DownArrow on arrow keypad 
	ANMKEY_PGDN,	// DIK_NEXT            0xD1    /* PgDn on arrow keypad 
	ANMKEY_,	// DIK_INSERT          0xD2    /* Insert on arrow keypad 
	ANMKEY_DELETE,	// DIK_DELETE          0xD3    /* Delete on arrow keypad 
	// N.B.: the remainder of this list needs to be filled out
	ANMKEY_,	// DIK_LWIN            0xDB    /* Left Windows key 
	ANMKEY_,	// DIK_RWIN            0xDC    /* Right Windows key 
	ANMKEY_,	// DIK_APPS            0xDD    /* AppMenu key 
	ANMKEY_,	// DIK_POWER           0xDE    /* System Power 
	ANMKEY_,	// DIK_SLEEP           0xDF    /* System Sleep 
	ANMKEY_,	// DIK_WAKE            0xE3    /* System Wake 
	ANMKEY_,	// DIK_WEBSEARCH       0xE5    /* Web Search 
	ANMKEY_,	// DIK_WEBFAVORITES    0xE6    /* Web Favorites 
	ANMKEY_,	// DIK_WEBREFRESH      0xE7    /* Web Refresh 
	ANMKEY_,	// DIK_WEBSTOP         0xE8    /* Web Stop 
	ANMKEY_,	// DIK_WEBFORWARD      0xE9    /* Web Forward 
	ANMKEY_,	// DIK_WEBBACK         0xEA    /* Web Back 
	ANMKEY_,	// DIK_MYCOMPUTER      0xEB    /* My Computer 
	ANMKEY_,	// DIK_MAIL            0xEC    /* Mail 
	ANMKEY_		// DIK_MEDIASELECT     0xED    /* Media Select 
};

inline int CAnmDIKeyboard8::AnmKey(int virtKey)
{
	return (int)xlatDIKtoANMkey[virtKey];
/* original code
	switch (virtKey)
	{
		case DIK_HOME :
			return ANMKEY_HOME		;

		case DIK_END :
			return ANMKEY_END		;

		case DIK_PRIOR :
			return ANMKEY_PGUP		;

		case DIK_NEXT :
			return ANMKEY_PGDN		;

		case DIK_UP :
			return ANMKEY_UP		;

		case DIK_DOWN :
			return ANMKEY_DOWN		;

		case DIK_LEFT :
			return ANMKEY_LEFT		;

		case DIK_RIGHT :
			return ANMKEY_RIGHT	;

		case DIK_F1 :
			return ANMKEY_F1		;

		case DIK_F2 :
			return ANMKEY_F2		;

		case DIK_F3 :
			return ANMKEY_F3		;

		case DIK_F4 :
			return ANMKEY_F4		;

		case DIK_F5 :
			return ANMKEY_F5		;

		case DIK_F6 :
			return ANMKEY_F6		;

		case DIK_F7 :
			return ANMKEY_F7		;

		case DIK_F8 :
			return ANMKEY_F8		;

		case DIK_F9 :
			return ANMKEY_F9		;

		case DIK_F10 :
			return ANMKEY_F10		;

		case DIK_F11 :
			return ANMKEY_F11		;

		case DIK_F12 :
			return ANMKEY_F12		;

		case DIK_LSHIFT :
			return ANMKEY_LSHIFT	;

		case DIK_RSHIFT :
			return ANMKEY_RSHIFT	;

		case DIK_LCONTROL :
			return ANMKEY_LCONTROL	;

		case DIK_RCONTROL :
			return ANMKEY_RCONTROL	;

		case DIK_LMENU :
			return ANMKEY_LALT		;

		case DIK_RMENU :
			return ANMKEY_RALT		;

		case DIK_DELETE :
			return ANMKEY_DELETE	;

		case DIK_BACK :
			return ANMKEY_BACK		;

		default :
			return virtKey; // ANMKEY_NOKEY ;
	}
original code	*/
}


CAnmDIKeyboard8::CAnmDIKeyboard8( cApplication *pApp, HWND hWnd )
: cKeyboard(pApp, hWnd)
{
	HRESULT hr;

	/**
	 * Get the DInput interface pointer
	 */
	CAnmDInput8 *pDInput8 = (CAnmDInput8 *) m_app->GetInput();
	LPDIRECTINPUT8 pDI = pDInput8->GetDInput();

	/**
	 * Create the keyboard device
	 */
    hr = pDI->CreateDevice(
		GUID_SysKeyboard,		
		&m_pDevice, 
		NULL); 
    if( FAILED(hr) )
    { 
		// ANMMSG_COULDNTCREATEKEYBOARD : "Keyboard could not be created"
        throw CAnmError(ANMMSG_COULDNTCREATEKEYBOARD);
    }
 
	/**
	 * Set the keyboard data format
	 */
    hr = m_pDevice->SetDataFormat(&c_dfDIKeyboard); 
    if( FAILED(hr) )
    { 
		SafeRelease( m_pDevice );
		// ANMMSG_COULDNTCREATEKEYBOARD : "Keyboard could not be created"
        throw CAnmError(ANMMSG_COULDNTCREATEKEYBOARD);
    } 
 
    /**
	 * Set the cooperative level 
	 */

	
    hr = m_pDevice->SetCooperativeLevel(
		hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
    if( FAILED(hr) )
    { 
		SafeRelease( m_pDevice );
		// ANMMSG_COULDNTSETKBCOOPLEVEL : "Keyboard coop level could not be changed"
        throw CAnmError(ANMMSG_COULDNTSETKBCOOPLEVEL);
    } 
	

	// CHANGE: we used to grab the keyboard here, but this would cause
	// a crash if the app didn't have focus when it started up.  This is,
	// of course, an undesirable trait :)


	memset( m_keyState, 0, 256*sizeof(bool) );	
}

CAnmDIKeyboard8::~CAnmDIKeyboard8()
{
	if( m_pDevice )
	{
		m_pDevice->Unacquire();
		SafeRelease( m_pDevice );
	}
}

void CAnmDIKeyboard8::ClearTable()
{
	memset( m_keyState, 0, sizeof(char)*256 );
}


bool CAnmDIKeyboard8::Poll( int key )
{
	// stuff goes in here.
	if( m_keyState[key] & 0x80 )
		return true;
	return false;
}


eResult CAnmDIKeyboard8::Update()
{
	char     newState[256]; 
    HRESULT  hr; 
 
	// CHANGE: Polling can't hurt, but it can help.
    hr = m_pDevice->Poll(); 
    hr = m_pDevice->GetDeviceState(sizeof(newState),(LPVOID)&newState); 

    if( FAILED(hr) )
    { 
		hr = m_pDevice->Acquire();
		if( FAILED( hr ) )
		{
			return resFailed;
		}

	    hr = m_pDevice->Poll(); 
	    hr = m_pDevice->GetDeviceState(sizeof(newState),(LPVOID)&newState); 
		if( FAILED( hr ) )
		{
			return resFailed;
		}
    } 

	int sz = m_receivers.size();
	if( sz )
	{
		int i;
		for( i=0; i< 256; i++ )
		{
			if( m_keyState[i] != newState[i] )
			{
				// Something happened to this key since the last time we checked
				if( (newState[i] & 0x80) )
				{
					// It was Pressed
					std::list<iKeyboardReceiver*>::iterator iter;
					for (iter = m_receivers.begin(); 
						iter != m_receivers.end(); iter ++)
					{
						(*iter)->KeyDown( AnmKey(i) );
					}
				}
				else
				{
					// It was Released
					std::list<iKeyboardReceiver*>::iterator iter;
					for (iter = m_receivers.begin(); 
						iter != m_receivers.end(); iter ++)
					{
						(*iter)->KeyUp( AnmKey(i) );
					}
				}
			}

			// copy the state over (we could do a memcpy at the end, but this
			// will have better cache performance)
			m_keyState[i] = newState[i];
		}
	}
	else
	{
		// copy the new states over.
		memcpy( m_keyState, newState, 256 );
	}
 
	return resAllGood;
}

char CAnmDIKeyboard8::TranslateVirtualKey(int key)
{
	switch (key)
	{
		case DIK_0  : // On main keyboard 
			return '0';
			break;

		case DIK_1  : // On main keyboard 
			return '1';
			break;

		case DIK_2  : // On main keyboard 
			return '2';
			break;

		case DIK_3  : // On main keyboard 
			return '3';
			break;

		case DIK_4  : // On main keyboard 
			return '4';
			break;

		case DIK_5  : // On main keyboard 
			return '5';
			break;

		case DIK_6  : // On main keyboard 
			return '6';
			break;

		case DIK_7  : // On main keyboard 
			return '7';
			break;

		case DIK_8  : // On main keyboard 
			return '8';
			break;

		case DIK_9  : // On main keyboard 
			return '9';
			break;

		case DIK_A   : // 
			return 'A';
			break;

		case DIK_ABNT_C1 : // On numeric pad of Brazilian keyboards 
			break;

		case DIK_ABNT_C2 : // On numeric pad of Brazilian keyboards 
			break;

		case DIK_ADD  : // PLUS SIGN (+) on numeric keypad  
			return '+';
			break;

		case DIK_APOSTROPHE   : // 
			return '\'';
			break;

		case DIK_APPS  : // Application key  
			break;

		case DIK_AT : // On Japanese keyboard 
			break;

		case DIK_AX : // On Japanese keyboard 
			break;

		case DIK_B   : // 
			return 'B';
			break;

		case DIK_BACK  : // BACKSPACE  
			break;

		case DIK_BACKSLASH   : // 
			return '\\';
			break;

		case DIK_C   : // 
			return 'C';
			break;

		case DIK_CALCULATOR  : // 
			break;

		case DIK_CAPITAL  : // CAPS LOCK  
			break;

		case DIK_COLON : // On Japanese keyboard 
			return ':';
			break;

		case DIK_COMMA   : // 
			return ',';
			break;

		case DIK_CONVERT : // On Japanese keyboard 
			break;

		case DIK_D   : // 
			return 'D';
			break;

		case DIK_DECIMAL  : // PERIOD (decimal point) on numeric keypad  
			return '.';
			break;

		case DIK_DELETE   : // 
			break;

		case DIK_DIVIDE  : // Forward slash (/) on numeric keypad  
			return '/';
			break;

		case DIK_DOWN  : // DOWN ARROW  
			break;

		case DIK_E   : // 
			return 'E';
			break;

		case DIK_END   : // 
			break;

		case DIK_EQUALS  : // On main keyboard 
			return '=';
			break;

		case DIK_ESCAPE   : // 
			break;

		case DIK_F   : // 
			return 'F';
			break;

		case DIK_F1   : // 
			break;

		case DIK_F2   : // 
			break;

		case DIK_F3   : // 
			break;

		case DIK_F4   : // 
			break;

		case DIK_F5  : // 
			break;

		case DIK_F6  : // 
			break;

		case DIK_F7  : // 
			break;

		case DIK_F8  : // 
			break;

		case DIK_F9   : // 
			break;

		case DIK_F10   : // 
			break;

		case DIK_F11   : // 
			break;

		case DIK_F12   : // 
			break;

		case DIK_F13  : // On NEC PC-98 Japanese keyboard 
			break;

		case DIK_F14  : // On NEC PC-98 Japanese keyboard 
			break;

		case DIK_F15  : // On NEC PC-98 Japanese keyboard 
			break;

		case DIK_G   : // 
			return 'G';
			break;

		case DIK_GRAVE  : // Grave accent (`)  
			break;

		case DIK_H   : // 
			return 'H';
			break;

		case DIK_HOME    : // 
			break;

		case DIK_I   : // 
			return 'I';
			break;

		case DIK_INSERT   : // 
			break;

		case DIK_J   : // 
			return 'J';
			break;

		case DIK_K   : // 
			return 'K';
			break;

		case DIK_KANA : // On Japanese keyboard 
			break;

		case DIK_KANJI : // On Japanese keyboard 
			break;

		case DIK_L   : // 
			return 'L';
			break;

		case DIK_LBRACKET  : // Left square bracket [ 
			return '[';
			break;

		case DIK_LCONTROL  : // Left CTRL  
			break;

		case DIK_LEFT  : // LEFT ARROW  
			break;

		case DIK_LMENU  : // Left ALT  
			break;

		case DIK_LSHIFT  : // Left SHIFT  
			break;

		case DIK_LWIN  : // Left Microsoft® Windows® logo key  
			break;

		case DIK_M   : // 
			return 'M';
			break;

		case DIK_MAIL  : // 
			break;

		case DIK_MEDIASELECT : // Media Select key, which displays a selection of supported media players on the system  
			break;

		case DIK_MEDIASTOP  : // 
			break;

		case DIK_MINUS  : // On main keyboard 
			return '-';
			break;

		case DIK_MULTIPLY  : // Asterisk (*) on numeric keypad  
			return '*';
			break;

		case DIK_MUTE  : // 
			break;

		case DIK_MYCOMPUTER  : // 
			break;

		case DIK_N   : // 
			return 'N';
			break;

		case DIK_NEXT  : // PAGE DOWN  
			break;

		case DIK_NEXTTRACK : // Next track 
			break;

		case DIK_NOCONVERT : // On Japanese keyboard 
			break;

		case DIK_NUMLOCK   : // 
			break;

		case DIK_NUMPAD0   : // 
			return '0';
			break;

		case DIK_NUMPAD1   : // 
			return '1';
			break;

		case DIK_NUMPAD2   : // 
			return '2';
			break;

		case DIK_NUMPAD3   : // 
			return '3';
			break;

		case DIK_NUMPAD4   : // 
			return '4';
			break;

		case DIK_NUMPAD5   : // 
			return '5';
			break;

		case DIK_NUMPAD6   : // 
			return '6';
			break;

		case DIK_NUMPAD7   : // 
			return '7';
			break;

		case DIK_NUMPAD8   : // 
			return '8';
			break;

		case DIK_NUMPAD9   : // 
			return '9';
			break;

		case DIK_NUMPADCOMMA : // On numeric keypad of NEC PC-98 Japanese keyboard 
			return ',';
			break;

		case DIK_NUMPADENTER   : // 
			break;

		case DIK_NUMPADEQUALS : // On numeric keypad of NEC PC-98 Japanese keyboard 
			return '=';
			break;

		case DIK_O   : // 
			return 'O';
			break;

		case DIK_OEM_102 : // On British and German keyboards 
			break;

		case DIK_P   : // 
			return 'P';
			break;

		case DIK_PAUSE   : // 
			break;

		case DIK_PERIOD  : // On main keyboard  
			return '.';
			break;

		case DIK_PLAYPAUSE  : // 
			break;

		case DIK_POWER  : // 
			break;

		case DIK_PREVTRACK : // Previous track; circumflex on Japanese keyboard 
			break;

		case DIK_PRIOR  : // PAGE UP  
			break;

		case DIK_Q   : // 
			return 'Q';
			break;

		case DIK_R   : // 
			return 'R';
			break;

		case DIK_RBRACKET  : // Right square bracket ] 
			return ']';
			break;

		case DIK_RCONTROL  : // Right CTRL  
			break;

		case DIK_RETURN  : // ENTER on main keyboard  
			break;

		case DIK_RIGHT  : // RIGHT ARROW  
			break;

		case DIK_RMENU  : // Right ALT  
			break;

		case DIK_RSHIFT  : // Right SHIFT  
			break;

		case DIK_RWIN  : // Right Windows logo key  
			break;

		case DIK_S   : // 
			return 'S';
			break;

		case DIK_SCROLL  : // SCROLL LOCK 
			break;

		case DIK_SEMICOLON   : // 
			return ';';
			break;

		case DIK_SLASH  : // Forward slash (/) on main keyboard 
			return '/';
			break;

		case DIK_SLEEP  : // 
			break;

		case DIK_SPACE  : // SPACEBAR  
			return ' ';
			break;

		case DIK_STOP : // On NEC PC-98 Japanese keyboard 
			break;

		case DIK_SUBTRACT  : // MINUS SIGN (-) on numeric keypad  
			return '-';
			break;

		case DIK_SYSRQ   : // 
			break;

		case DIK_T   : // 
			return 'T';
			break;

		case DIK_TAB   : // 
			break;

		case DIK_U   : // 
			return 'U';
			break;

		case DIK_UNDERLINE : // On NEC PC-98 Japanese keyboard 
			break;

		case DIK_UNLABELED : // On Japanese keyboard 
			break;

		case DIK_UP  : // UP ARROW  
			break;

		case DIK_V   : // 
			return 'V';
			break;

		case DIK_VOLUMEDOWN  : // 
			break;

		case DIK_VOLUMEUP  : // 
			break;

		case DIK_W   : // 
			return 'W';
			break;

		case DIK_WAKE  : // 
			break;

		case DIK_WEBBACK  : // 
			break;

		case DIK_WEBFAVORITES : // Displays the Microsoft Internet Explorer Favorites list, the Windows Favorites folder, or the Netscape Bookmarks list.  
			break;

		case DIK_WEBFORWARD  : // 
			break;

		case DIK_WEBHOME  : // 
			break;

		case DIK_WEBREFRESH  : // 
			break;

		case DIK_WEBSEARCH  : // 
			break;

		case DIK_WEBSTOP  : // 
			break;

		case DIK_X   : // 
			return 'X';
			break;

		case DIK_Y   : // 
			return 'Y';
			break;

		case DIK_YEN : // On Japanese keyboard 
			break;

		case DIK_Z : // 
			return 'Z';
			break;

		default :
			return 0;
			break;

	}

	return 0;

}
