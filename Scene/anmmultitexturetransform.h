/********************************************************************************
 * Flux
 *
 * File: anmmultitexturetransform.h
 * Description: MultiTextureTransform node
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

#ifndef _anmmultitexturetransform_h
#define _anmmultitexturetransform_h

#include "anmtexturetransform.h"

class CAnmMultiTextureTransform : public CAnmTextureTransform
{

protected:
	
	NodeArray					*m_textureTransform;

	class CAnmTextureTransform *GetTextureTransformChild(int index);
	void SetStageToDevice(CAnmDevice *pDevice, CAnmTextureTransform *pTextureTransform, 
												 int stage);
public:

	// constructor/destructor
	CAnmMultiTextureTransform();
	virtual ~CAnmMultiTextureTransform();

	// CAnmTextureTransform overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual void SetToDevice(class CAnmDevice *pDevice, int startLevel, int nLevels);
	virtual int NumLevels();


	// Accessors
	void SetTextureTransform(NodeArray *pTextureTransform);
	NodeArray *GetTextureTransform() { return m_textureTransform; }
	void GetTextureTransform(NodeArray **pVal)
	{
		assert(pVal);
		*pVal = m_textureTransform;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmMultiTextureTransform);

	DECLARE_VALUE(textureTransform);

	DECLARE_METHOD(set_textureTransform);

	DECLARE_CALLBACK(textureTransform_changed);
};


#endif // _anmmultitexturetransform_h
