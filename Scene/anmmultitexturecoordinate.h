/********************************************************************************
 * Flux
 *
 * File: anmmultitexturecoordinate.h
 * Description: MultiTextureCoordinate node
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

#ifndef _anmmultitexturecoordinate_h
#define _anmmultitexturecoordinate_h

#include "anmgroup.h"

#define ANMMTC_TEXCOORDDIRTY				ANMNODE_NDIRTYBITS

typedef enum eAnmMTCDirtyFlags {
eAnmMTCTexcoordDirty				= ANMDIRTYBIT(ANMMTC_TEXCOORDDIRTY),
} eAnmMTCDirtyFlags;


class CAnmMultiTextureCoordinate : public CAnmGroup
{
protected :

	NodeArray					*m_texCoord;

	virtual class CAnmTextureCoordinate *GetTexCoordChild(int index);
	virtual class CAnmTextureCoordinateGenerator *GetTexCoordGenChild(int index);

public:

	// constructor/destructor
	CAnmMultiTextureCoordinate();
	virtual ~CAnmMultiTextureCoordinate();

	// CAnmGroup overrides
	virtual void Realize();								// build lower-level rendering structures
	virtual void Update();								// re-render/reset rendering structures

	// New methods
	virtual bool SetToDevice(class CAnmDevice *pDevice, int startLevel, int nLevels);
	virtual int NumLevels();
	
	// Accessors
	void SetTexCoord(NodeArray *pTexCoord);
	NodeArray *GetTexCoord() { return m_texCoord; }
	void GetTexCoord(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_texCoord;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmMultiTextureCoordinate);

	DECLARE_CALLBACK(texCoord_changed);

	DECLARE_VALUE(texCoord);

	DECLARE_METHOD(set_texCoord);

};

DECLARE_GETIMPLEMENTATION(CAnmMultiTextureCoordinate);

#endif _anmmultitexturecoordinate_h
