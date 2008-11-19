/********************************************************************************
 * Flux
 *
 * File: anmmetadatatable.h
 * Description: Metadata key/value pair storage and lookup
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

#ifndef _anmmetadatatable_h
#define _anmmetadatatable_h

#include "anmsymbol.h"

class CAnmMetadataEntry : public CAnmSymbol
{
protected :

	String					m_value;

public :

	CAnmMetadataEntry(String key, String value) : CAnmSymbol(key)
	{
		m_value = NULL;
		SetValue(value);
	}

	~CAnmMetadataEntry()
	{
		SafeUnRef(m_value);
	}

	// Accessors
	virtual const char *GetKey()
	{
		assert(m_name);
		return m_name->GetBuf();
	}

	virtual void SetValue(String value)
	{
		SafeUnRef(m_value);
		m_value = value;
		m_value->Ref();
	}

	virtual String GetValue()
	{
		return m_value;
	}
};

class CAnmMetadataTable
{
protected :

	CAnmSymbolTable				*m_symbolTable;

public :

	CAnmMetadataTable()
	{
		m_symbolTable = new CAnmSymbolTable();
	}

	~CAnmMetadataTable()
	{
		SafeDelete(m_symbolTable);
	}

	// Methods
	virtual void AddEntry(CAnmMetadataEntry *pEntry)
	{
		m_symbolTable->AddSymbol(pEntry);
	}

	virtual CAnmMetadataEntry *FindEntry(const char * key)
	{
		return (CAnmMetadataEntry *) m_symbolTable->FindSymbol(key);
	}
};

#endif // _anmmetadatatable_h
