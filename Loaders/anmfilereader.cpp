/********************************************************************************
 * Flux
 *
 * File: anmfilereader.cpp
 * Description: Abstract file reader class
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
#include "anmfilereader.h"
#include "anmfileloader.h"

CAnmFileReader::CAnmFileReader(CAnmFileLoader *pLoader)
{
	assert(pLoader != NULL);

	m_loader = pLoader;

	m_colorarray = NULL;
	m_doublearray = NULL;
	m_point2array = NULL;
	m_point3array = NULL;
	m_rotationarray = NULL;
	m_image = NULL;
	m_intarray = NULL;
	m_floatarray = NULL;
	m_stringarray = NULL;
	m_timearray = NULL;
	m_booleanarray = NULL;

	m_booleanvalues.reserve(VALUEARRAY_ALLOC);
	m_doublevalues.reserve(VALUEARRAY_ALLOC);
	m_colorvalues.reserve(VALUEARRAY_ALLOC);
	m_floatvalues.reserve(VALUEARRAY_ALLOC);
	m_integervalues.reserve(VALUEARRAY_ALLOC);
	m_rotationvalues.reserve(VALUEARRAY_ALLOC);
	m_stringvalues.reserve(VALUEARRAY_ALLOC);
	m_timevalues.reserve(VALUEARRAY_ALLOC);
	m_point2values.reserve(VALUEARRAY_ALLOC);
	m_point3values.reserve(VALUEARRAY_ALLOC);

}

CAnmFileReader::~CAnmFileReader()
{
	SafeUnRef(m_colorarray);
	SafeUnRef(m_point2array);
	SafeUnRef(m_point3array);
	SafeUnRef(m_rotationarray);
	SafeUnRef(m_image);
	SafeUnRef(m_intarray);
	SafeUnRef(m_floatarray);
	SafeUnRef(m_stringarray);
	SafeUnRef(m_timearray);
	SafeUnRef(m_booleanarray);
}

void CAnmFileReader::WriteToConsole(char *str)
{
	if (m_loader)
		m_loader->WriteToConsole(str);
}
