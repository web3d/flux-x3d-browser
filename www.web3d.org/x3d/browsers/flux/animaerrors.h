/********************************************************************************
 * Flux
 *
 * File: animaerrors.h
 * Description: Error handling definitions
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


#ifndef _animaerrors_h
#define _animaerrors_h

class CAnmError
{
	std::string m_errorText;

public:

	CAnmError( const char* errorText )
	{
		DP1("\n[***ERROR***] CAnmError thrown! text: [%s]\n", errorText );
		m_errorText = std::string( errorText );
	}

	const char* GetText()
	{
		return m_errorText.c_str();
	}
};

// N.B. still have Perez/Royer atavisms; do we need to keep these?

enum eResult
{
	resAllGood		= 0, // function passed with flying colors
	resFalse		= 1, // function worked and returns value 'false'
	resFailed		= -1, // function failed miserably
	resNotImpl		= -2, // function has not been implemented
	resForceDWord = 0x7FFFFFFF
};

inline bool Succeeded( eResult in )
{
	if( in >= 0 )
		return true;
	return false;
}

inline bool Failed( eResult in )
{
	if( in < 0 )
		return true;
	return false;
}

#endif // _animaerrors_h
