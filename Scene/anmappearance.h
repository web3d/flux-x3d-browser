/********************************************************************************
 * Flux
 *
 * File: anmappearance.h
 * Description: Appearance node
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

#ifndef _anmappearance_h
#define _anmappearance_h

#include "anmgroup.h"

class CAnmAppearance : public CAnmGroup
{

protected:

	CAnmNode						*m_material;
	CAnmNode						*m_shader;
	CAnmNode						*m_texture;
	CAnmNode						*m_textureTransform;

public:

	// constructor/destructor
	CAnmAppearance();
	virtual ~CAnmAppearance();

	// CAnmGroup overrides
	virtual void Realize();				// build lower-level rendering structures
	virtual void Update();				// re-render/reset rendering structures

	virtual eAnmReturnStatus AddChild(CAnmNode *pChild);
	virtual eAnmReturnStatus RemoveChild(CAnmNode *pChild);

	virtual void Traverse(class CAnmUpdateVisitor *pVisitor);

	// Accessors
	void SetMaterial(CAnmNode *pMaterial);
	CAnmNode *GetMaterial()
	{
		return m_material;
	}
	void GetMaterial(CAnmNode **ppMat)
	{
		assert(ppMat);
		*ppMat = m_material;
	}

	void SetShader(CAnmNode *pShader);
	CAnmNode *GetShader()
	{
		return m_shader;
	}
	void GetShader(CAnmNode **ppShader)
	{
		assert(ppShader);
		*ppShader = m_shader;
	}

	void SetTexture(CAnmNode *pTexture);
	CAnmNode *GetTexture()
	{
		return m_texture;
	}
	void GetTexture(CAnmNode **ppTxt)
	{
		assert(ppTxt);
		*ppTxt = m_texture;
	}
	
	void SetTextureTransform(CAnmNode *pTextureTransform);
	CAnmNode *GetTextureTransform()
	{
		return m_textureTransform;
	}
	void GetTextureTransform(CAnmNode **ppTxtfm)
	{
		assert(ppTxtfm);
		*ppTxtfm = m_textureTransform;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_NODECLASS(CAnmAppearance);

	DECLARE_VALUE(material);
	DECLARE_VALUE(shader);
	DECLARE_VALUE(texture);
	DECLARE_VALUE(textureTransform);

	DECLARE_METHOD(set_material);
	DECLARE_METHOD(set_shader);
	DECLARE_METHOD(set_texture);
	DECLARE_METHOD(set_textureTransform);

	DECLARE_CALLBACK(material_changed);
	DECLARE_CALLBACK(shader_changed);
	DECLARE_CALLBACK(texture_changed);
	DECLARE_CALLBACK(textureTransform_changed);

};

DECLARE_GETIMPLEMENTATION(CAnmAppearance);

#endif // _anmappearance_h
