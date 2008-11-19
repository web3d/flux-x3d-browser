/********************************************************************************
 * Flux
 *
 * File: anmmetadatanode.h
 * Description: Metadata base class
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

#ifndef _anmmetadatanode_h
#define _anmmetadatanode_h

#define ANMMETADATA_DEFAULT_NAME			""
#define ANMMETADATA_DEFAULT_REFERENCE		""

#include "anmnode.h"

class CAnmMetadataNode : public CAnmNode
{
protected:


	String				m_metadataname;	// not to be confused with the node name
	String				m_reference;	// not to be confused with the node name

public:

	// constructor/destructor
	CAnmMetadataNode();

	virtual ~CAnmMetadataNode();

	// CAnmNode overrides
	virtual void Realize();


	// Accessors
	void SetMetadataName(String description);
	String GetMetadataName()
	{
		return m_metadataname;
	}
	void GetMetadataName(String *pStr)
	{
		assert(pStr);
		*pStr = m_metadataname;
	}

	void SetReference(String reference);
	String GetReference()
	{
		return m_reference;
	}
	void GetReference(String *pStr)
	{
		assert(pStr);
		*pStr = m_reference;
	}


	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmMetadataNode);

	DECLARE_VALUE(name);
	DECLARE_VALUE(reference);

	DECLARE_METHOD(set_name);
	DECLARE_METHOD(set_reference);

	DECLARE_CALLBACK(name_changed);
	DECLARE_CALLBACK(reference_changed);
};

#endif // _anmmetadatanode_h

