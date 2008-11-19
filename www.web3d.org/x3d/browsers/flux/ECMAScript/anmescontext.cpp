/********************************************************************************
 * Flux
 *
 * File: anmescontext.cpp
 * Description: ECMAScript execution context (and scene) object
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
#include "anmescontext.h"
#include "anmeslanguagebinding.h"
#include "anmesnode.h"
#include "anmesstring.h"
#include "anmexecutioncontext.h"
#include "anmnode.h"
#include "anmsainode.h"
#include "anmsaiscene.h"
#include "anmscript.h"

// ExecutionContext object
CAnmESExecutionContext::CAnmESExecutionContext(CAnmExecutionContext *pContext, CAnmECMAScriptLanguageBinding *pOwner)
: CAnmESBuiltinObject(pContext, pOwner)
{
}

CAnmESExecutionContext::~CAnmESExecutionContext()
{
}

AnmESVar CAnmESExecutionContext::getSpecificationVersion()
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	AnmESVar v;

	String s = new CAnmString(pContext->GetSpecificationVersion());

	v = new CAnmESString(s, m_owner);

	// Drop one or this never goes away
	s->UnRef();

	return v;
}

AnmESVar CAnmESExecutionContext::getEncoding()
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	return (int) pContext->GetEncoding();
}


AnmESVar CAnmESExecutionContext::getProfile()
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	// N.B.: warn NIY?

	return (CAnmESObject *) NULL;
}

AnmESVar CAnmESExecutionContext::getComponents()
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	// N.B.: warn NIY?

	return (CAnmESObject *) NULL;
}

AnmESVar CAnmESExecutionContext::getWorldURL()
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	AnmESVar v;

	String s = new CAnmString(pContext->GetWorldURL());

	v = new CAnmESString(s, m_owner);

	// Drop one or this never goes away
	s->UnRef();

	return v;
}


AnmESVar CAnmESExecutionContext::getNode(int nArgs, AnmESVar *args)
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	AnmESVar v;

	if (nArgs > 0)
	{
		CAnmESString *pESStr = args[0];

		if (pESStr)
		{
			String s = pESStr->GetString();

			const char *nodename = s->GetBuf();

			if (nodename && strlen(nodename))
			{
				CAnmSAINode *pSAINode = pContext->GetNode(nodename);

				if (pSAINode)
				{
					pSAINode->SetSink(m_owner->GetScriptNode());

					CAnmESNodeObject *pNodeObj = new CAnmESNodeObject(pSAINode, m_owner);

					// drop one ref otherwise this thing will live forever
					pSAINode->UnRef();

					v = pNodeObj;
				}
			}
		}
	}

	return v;
}

AnmESVar CAnmESExecutionContext::getImportedNode(int nArgs, AnmESVar *args)
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	AnmESVar v;

	if (nArgs > 0)
	{
		CAnmESString *pESStr = args[0];

		if (pESStr)
		{
			String s = pESStr->GetString();

			const char *nodename = s->GetBuf();

			if (nodename && strlen(nodename))
			{
				CAnmSAINode *pSAINode = pContext->GetImportedNode(nodename);

				if (pSAINode)
				{
					pSAINode->SetSink(m_owner->GetScriptNode());

					CAnmESNodeObject *pNodeObj = new CAnmESNodeObject(pSAINode, m_owner);

					// drop one ref otherwise this thing will live forever
					pSAINode->UnRef();

					v = pNodeObj;
				}
			}
		}
	}

	return v;
}


AnmESVar CAnmESExecutionContext::createNode(int nArgs, AnmESVar *args)
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	AnmESVar v;

	if (nArgs > 0)
	{
		CAnmESString *pESStr = args[0];

		if (pESStr)
		{
			String s = pESStr->GetString();

			const char *nodetypename = s->GetBuf();

			if (nodetypename && strlen(nodetypename))
			{
				CAnmSAINode *pSAINode = pContext->CreateNode(nodetypename);

				if (pSAINode)
				{
					pSAINode->SetSink(m_owner->GetScriptNode());

					CAnmESNodeObject *pNodeObj = new CAnmESNodeObject(pSAINode, m_owner);

					// drop one ref otherwise this thing will live forever
					pSAINode->UnRef();

					v = pNodeObj;
				}
			}
		}
	}

	return v;
}

AnmESVar CAnmESExecutionContext::createProto(int nArgs, AnmESVar *args)
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	AnmESVar v;

	if (nArgs > 0)
	{
		CAnmESString *pESStr = args[0];

		if (pESStr)
		{
			String s = pESStr->GetString();

			const char *prototypename = s->GetBuf();

			if (prototypename && strlen(prototypename))
			{
				CAnmSAINode *pSAINode = pContext->GetImportedNode(prototypename);

				if (pSAINode)
				{
					pSAINode->SetSink(m_owner->GetScriptNode());

					CAnmESNodeObject *pNodeObj = new CAnmESNodeObject(pSAINode, m_owner);

					// drop one ref otherwise this thing will live forever
					pSAINode->UnRef();

					v = pNodeObj;
				}
			}
		}
	}

	return v;
}

AnmESVar CAnmESExecutionContext::getProtoDeclaration(int nArgs, AnmESVar *args)
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	// N.B.: warn NIY?

	return (CAnmESObject *) NULL;
}

AnmESVar CAnmESExecutionContext::getRootNodes()
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	std::vector<class CAnmSAINode *> *pRootNodes =
		pContext->GetRootNodes();

	if (pRootNodes)
	{
		int sz = pRootNodes->size();

		if (sz <= 0)
			return (CAnmESObject *) NULL;

		CAnmESObject **ppESNodes = new CAnmESObject *[sz];

		for (int i = 0; i < sz; i++)
		{
			CAnmSAINode *pSAINode = (*pRootNodes)[i];

			CAnmESNodeObject *pNodeObj = new CAnmESNodeObject(pSAINode, m_owner);
			ppESNodes[i] = pNodeObj;
		}
		
		CAnmESUserObject *pUserObj = new CAnmESUserObject(m_owner, sz, ppESNodes);

		delete[] ppESNodes;

		return pUserObj;
	}

	return (CAnmESObject *) NULL;
}

AnmESVar CAnmESExecutionContext::addRoute(int nArgs, AnmESVar *args)
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	AnmESVar v;

	if (nArgs < 4)
	{
		// runtime warning here
	}
	else
	{
		CAnmESNodeObject *pFromESNode = (CAnmESNodeObject *) args[0];
		CAnmSAINode *pFromSAINode = NULL;
		if (pFromESNode)
			pFromSAINode = pFromESNode->GetNode();

		CAnmESString *fromFieldStr = (CAnmESString *) args[1];
		if (fromFieldStr == NULL)
			fromFieldStr = args[1].toString(m_owner);
		String fromField = fromFieldStr->GetString();

		CAnmESNodeObject *pToESNode = (CAnmESNodeObject *) args[2];
		CAnmSAINode *pToSAINode = NULL;
		if (pToESNode)
			pToSAINode = pToESNode->GetNode();

		CAnmESString *toFieldStr = (CAnmESString *) args[3];
		if (toFieldStr == NULL)
			toFieldStr = args[3].toString(m_owner);
		String toField = toFieldStr->GetString();

		if (pFromSAINode == NULL)
		{
			// runtime error here
		}
		else if (pToSAINode == NULL)
		{
			// runtime error here
		}
		else if (!fromField->length())
		{
			// runtime error here
		}
		else if (!toField->length())
		{
			// runtime error here
		}
		else
		{
			pContext->AddRoute(pFromSAINode, fromField->GetBuf(), pToSAINode, toField->GetBuf());
		}
	}

	return v;
}

AnmESVar CAnmESExecutionContext::deleteRoute(int nArgs, AnmESVar *args)
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	// N.B.: warn NIY?

	return (CAnmESObject *) NULL;
}

AnmESVar CAnmESExecutionContext::getRoutes()
{
	CAnmExecutionContext *pContext = (CAnmExecutionContext *) m_object;

	// N.B.: warn NIY?

	return (CAnmESObject *) NULL;
}

// Scene object
CAnmESScene::CAnmESScene(CAnmSAIScene *pScene, CAnmECMAScriptLanguageBinding *pOwner)
: CAnmESExecutionContext(pScene, pOwner)
{
}

CAnmESScene::~CAnmESScene()
{
}

AnmESVar CAnmESScene::getMetaData(int nArgs, AnmESVar *args)
{
	CAnmSAIScene *pScene = (CAnmSAIScene *) m_object;

	AnmESVar v;

	if (nArgs > 0)
	{
		CAnmESString *pESStr = args[0];

		if (pESStr)
		{
			String skey = pESStr->GetString();

			const char *key = skey->GetBuf();

			if (key && strlen(key))
			{
				String sval = pScene->GetMetaData(key);

				String s2 = sval->Clone();

				v = new CAnmESString(s2, m_owner);

				// Drop one or this never goes away
				s2->UnRef();

			}
		}
	}

	return v;
}

AnmESVar CAnmESScene::setMetaData(int nArgs, AnmESVar *args)
{
	CAnmSAIScene *pScene = (CAnmSAIScene *) m_object;

	if (nArgs > 1)
	{
		CAnmESString *pESStrKey = args[0];

		if (pESStrKey)
		{
			String skey = pESStrKey->GetString();

			const char *key = skey->GetBuf();

			if (key && strlen(key))
			{
				CAnmESString *pESStrVal = args[1];
				if (pESStrVal)
				{
					String sval = pESStrVal->GetString();

					pScene->SetMetaData(skey, sval);
				}
			}
		}
	}

	return 0L;
}

AnmESVar CAnmESScene::getExportedNode(int nArgs, AnmESVar *args)
{
	CAnmSAIScene *pScene = (CAnmSAIScene *) m_object;

	AnmESVar v;

	if (nArgs > 0)
	{
		CAnmESString *pESStr = args[0];

		if (pESStr)
		{
			String s = pESStr->GetString();

			const char *nodename = s->GetBuf();

			if (nodename && strlen(nodename))
			{
				CAnmSAINode *pSAINode = pScene->GetImportedNode(nodename);

				if (pSAINode)
				{
					pSAINode->SetSink(m_owner->GetScriptNode());

					CAnmESNodeObject *pNodeObj = new CAnmESNodeObject(pSAINode, m_owner);

					// drop one ref otherwise this thing will live forever
					pSAINode->UnRef();

					v = pNodeObj;
				}
			}
		}
	}

	return v;
}

AnmESVar CAnmESScene::addRootNode(int nArgs, AnmESVar *args)
{
	CAnmSAIScene *pScene = (CAnmSAIScene *) m_object;

	AnmESVar v;

	if (nArgs > 0)
	{
		CAnmESNodeObject *pESNode = (CAnmESNodeObject *) args[0];
		CAnmSAINode *pSAINode = NULL;
		if (pESNode)
		{
			pSAINode = pESNode->GetNode();

			if (pSAINode)
				pScene->AddRootNode(pSAINode);
		}
	}

	return v;
}

AnmESVar CAnmESScene::removeRootNode(int nArgs, AnmESVar *args)
{
	CAnmSAIScene *pScene = (CAnmSAIScene *) m_object;

	AnmESVar v;

	if (nArgs > 0)
	{
		CAnmESNodeObject *pESNode = (CAnmESNodeObject *) args[0];
		CAnmSAINode *pSAINode = NULL;
		if (pESNode)
		{
			pSAINode = pESNode->GetNode();

			if (pSAINode)
				pScene->RemoveRootNode(pSAINode);
		}
	}

	return v;
}

// Tables for Callbacks, Methods and Put/GetProperties

// ExecutionContext object

DEFINE_ESMETHOD(CAnmESExecutionContext, getNode);
DEFINE_ESMETHOD(CAnmESExecutionContext, getImportedNode);
DEFINE_ESMETHOD(CAnmESExecutionContext, createNode);
DEFINE_ESMETHOD(CAnmESExecutionContext, createProto);
DEFINE_ESMETHOD(CAnmESExecutionContext, getProtoDeclaration);
DEFINE_ESMETHOD(CAnmESExecutionContext, addRoute);
DEFINE_ESMETHOD(CAnmESExecutionContext, deleteRoute);
DEFINE_ESREADONLYPROPERTY(CAnmESExecutionContext, specificationVersion, getSpecificationVersion);
DEFINE_ESREADONLYPROPERTY(CAnmESExecutionContext, encoding, getEncoding);
DEFINE_ESREADONLYPROPERTY(CAnmESExecutionContext, profile, getProfile);
DEFINE_ESREADONLYPROPERTY(CAnmESExecutionContext, components, getComponents);
DEFINE_ESREADONLYPROPERTY(CAnmESExecutionContext, worldURL, getWorldURL);
DEFINE_ESREADONLYPROPERTY(CAnmESExecutionContext, rootNodes, getRootNodes);
DEFINE_ESREADONLYPROPERTY(CAnmESExecutionContext, routes, getRoutes);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESExecutionContext)
ESMETHODID(CAnmESExecutionContext, getNode),
ESMETHODID(CAnmESExecutionContext, getImportedNode),
ESMETHODID(CAnmESExecutionContext, createNode),
ESMETHODID(CAnmESExecutionContext, createProto),
ESMETHODID(CAnmESExecutionContext, getProtoDeclaration),
ESMETHODID(CAnmESExecutionContext, addRoute),
ESMETHODID(CAnmESExecutionContext, deleteRoute),
ESPROPID(CAnmESExecutionContext, specificationVersion),
ESPROPID(CAnmESExecutionContext, encoding),
ESPROPID(CAnmESExecutionContext, profile),
ESPROPID(CAnmESExecutionContext, components),
ESPROPID(CAnmESExecutionContext, worldURL),
ESPROPID(CAnmESExecutionContext, rootNodes),
ESPROPID(CAnmESExecutionContext, routes),

END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESExecutionContext, CAnmESBuiltinObject);

// Scene object
DEFINE_ESINHERITED_METHOD(CAnmESScene, getNode, CAnmESExecutionContext);
DEFINE_ESINHERITED_METHOD(CAnmESScene, getImportedNode, CAnmESExecutionContext);
DEFINE_ESINHERITED_METHOD(CAnmESScene, createNode, CAnmESExecutionContext);
DEFINE_ESINHERITED_METHOD(CAnmESScene, createProto, CAnmESExecutionContext);
DEFINE_ESINHERITED_METHOD(CAnmESScene, getProtoDeclaration, CAnmESExecutionContext);
DEFINE_ESINHERITED_METHOD(CAnmESScene, addRoute, CAnmESExecutionContext);
DEFINE_ESINHERITED_METHOD(CAnmESScene, deleteRoute, CAnmESExecutionContext);
DEFINE_ESMETHOD(CAnmESScene, getMetaData);
DEFINE_ESMETHOD(CAnmESScene, setMetaData);
DEFINE_ESMETHOD(CAnmESScene, getExportedNode);
DEFINE_ESMETHOD(CAnmESScene, addRootNode);
DEFINE_ESMETHOD(CAnmESScene, removeRootNode);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESScene)
ESMETHODID(CAnmESScene, getNode),
ESMETHODID(CAnmESScene, getImportedNode),
ESMETHODID(CAnmESScene, createNode),
ESMETHODID(CAnmESScene, createProto),
ESMETHODID(CAnmESScene, getProtoDeclaration),
ESMETHODID(CAnmESScene, addRoute),
ESMETHODID(CAnmESScene, deleteRoute),
ESMETHODID(CAnmESScene, getMetaData),
ESMETHODID(CAnmESScene, setMetaData),
ESMETHODID(CAnmESScene, getExportedNode),
ESMETHODID(CAnmESScene, addRootNode),
ESMETHODID(CAnmESScene, removeRootNode),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESScene, CAnmESExecutionContext);
