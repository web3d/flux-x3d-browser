/********************************************************************************
 * Flux
 *
 * File: anmscene.h
 * Description: Scene objects; for top-level
 *							scenes only (worlds and inlines)
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

#ifndef _anmscene_h
#define _anmscene_h

#include "anmproto.h"

class CAnmScene : public CAnmProto
{
protected:

	class cApplication			*m_app;
	char						*m_specificationVersion;
	eAnmX3DEncoding				 m_encoding;
	class CAnmProfileInfo		*m_profileInfo;
	std::vector<class CAnmComponentInfo *>
								 m_componentInfo;
	char						*m_worldUrl;
	class CAnmGroup				*m_containingGroup;
	class CAnmMetadataTable		*m_metadatatable;

	virtual void SetApp(class cApplication *pApp)
	{
		m_app = pApp;
	}

	virtual void SetSpecificationVersion(char *specificationVersion)
	{
		m_specificationVersion = specificationVersion;
	}

	virtual void SetEncoding(eAnmX3DEncoding encoding)
	{
		m_encoding = encoding;
	}

	virtual void SetProfile(class CAnmProfileInfo *pProfile)
	{
		m_profileInfo = pProfile;
	}

	virtual void SetComponentInfo(std::vector<class CAnmComponentInfo *> *pComponentInfo)
	{
		m_componentInfo.clear();

		int sz = (*pComponentInfo).size();
		for (int i = 0; i < sz; i++)
			(*pComponentInfo).push_back((*pComponentInfo)[i]);

	}

	virtual void SetWorldURL(const char *worldUrl)
	{
		m_worldUrl = AnmCopyString(worldUrl);
	}

	static CAnmObject *Create();

	friend class CAnmSceneSymbol;
	friend class CAnmFileLoader;
	friend class cAnimaApp;

public:

	// constructor/destructor
	CAnmScene();
	virtual ~CAnmScene();

	// CAnmProto overrides
	virtual char *GetSpecificationVersion()
	{
		return m_specificationVersion;
	}

	virtual eAnmX3DEncoding GetEncoding()
	{
		return m_encoding;
	}

	virtual class CAnmProfileInfo *GetProfile()
	{
		return m_profileInfo;
	}

	virtual std::vector<class CAnmComponentInfo *> *GetComponents()
	{
		return &m_componentInfo;
	}

	virtual const char *GetWorldURL()
	{
		return m_worldUrl;
	}

	virtual NodeArray *GetRootNodes();

	// New methods
	virtual String GetMetaData(const char *key);
	virtual void SetMetaData(String key, String value);

	virtual class CAnmNode *GetExportedNode(const char *nodename);

	virtual void AddRootNode(class CAnmNode *pRootNode);
	virtual void RemoveRootNode(class CAnmNode *pRootNode);

	// Accessors
	virtual void SetContainingGroup(class CAnmGroup *pGroup)
	{
		m_containingGroup = pGroup;
	}

	virtual class CAnmGroup *GetContainingGroup()
	{
		return m_containingGroup;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmScene);

};


inline CAnmClass *CAnmScene::GetClass()
{
	if (m_protosymbol)
		return m_protosymbol->GetClass();
	else
		return CAnmScene::m_classdata;
}

#endif // _anmscene_h
