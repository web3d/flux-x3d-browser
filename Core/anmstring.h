/********************************************************************************
 * Flux
 *
 * File: anmstring.h
 * Description: String handling
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

#ifndef _anmstring_h
#define _anmstring_h

#include "anmobject.h"


extern EXPORT char *AnmCopyString(const char *str);
extern EXPORT char *AnmAllocString(size_t len);
inline char *AnmSkipLeadingSpace(const char *cp)
{
	if (!cp)
		return NULL;

	char *skipcp = (char *) cp;
	while (isspace(*skipcp))
		skipcp++;

	return skipcp;
}

class EXPORT CAnmString : public CAnmObject
{

protected :

	char						*m_cstr;

public :

	CAnmString()
	{
		m_cstr = NULL;
	}

	CAnmString(char *cp)
	{
		m_cstr = AnmCopyString((const char *) cp);
	}

	CAnmString(const char *cp)
	{
		m_cstr = AnmCopyString(cp);
	}

	~CAnmString()
	{
		SafeDelete(m_cstr);
	}

	CAnmString *Clone()
	{
		return new CAnmString(m_cstr);
	}

	int length()
	{
		if (!m_cstr)
			return 0L;
		else
			return strlen(m_cstr);
	}

	void alloc(size_t sz)
	{
		m_cstr = AnmAllocString(sz);
	}

	char *GetBuf()
	{
		return m_cstr;
	}

	BOOL operator == (const char *cp)
	{
		if (!m_cstr && !cp)
			return TRUE;
		else if (!m_cstr)
			return FALSE;
		else if (!cp)
			return FALSE;
		else
			return (!strcmp(m_cstr, cp));
	}
};


#endif //_anmstring_h