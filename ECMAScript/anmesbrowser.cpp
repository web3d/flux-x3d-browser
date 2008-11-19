/********************************************************************************
 * Flux
 *
 * File: anmesbrowser.cpp
 * Description: ECMAScript browser object
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
#include "anmesbrowser.h"
#include "anmescontext.h"
#include "anmesfields.h"
#include "anmesnode.h"
#include "anmesstring.h"
#include "anmbrowser.h"
#include "anmesmessage.h"
#include "anmsaiscene.h"
#include "anmscript.h"

// Browser object
CAnmESBrowserObject::CAnmESBrowserObject(CAnmBrowser *pBrowser, CAnmECMAScriptLanguageBinding *pOwner)
: CAnmESBuiltinObject(pBrowser, pOwner)
{
}

CAnmESBrowserObject::~CAnmESBrowserObject()
{
}

AnmESVar CAnmESBrowserObject::createVrmlFromURL(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	if (nArgs != 3)
	{
		// runtime warning here
	}
	else
	{
		StringArray *pUrl = NULL;
		CAnmNode *pNode = NULL;
		
		pUrl = (StringArray *) args[0];
		if (pUrl == NULL)
		{
			// ANMMSG_ESCREATEVRMLNOTSTRING : "Browser::createVrmlFromUrl: Can't convert parameter 1 to string array value"
			throw CAnmError(ANMMSG_ESCREATEVRMLNOTSTRING);
		}

		pNode = (CAnmNode *) args[1];
		if (pNode == NULL)
		{
			// ANMMSG_ESCREATEVRMLNOTNODE : "Browser::createVrmlFromUrl: Expected node object in parameter 2"
			throw CAnmError(ANMMSG_ESCREATEVRMLNOTNODE);
		}

		CAnmESString *eventname = args[2];
		if (eventname == NULL)
			eventname = args[2].toString(m_owner);

		String str = eventname->GetString();

		if (str->length() == 0)
		{
			// ANMMSG_ESCREATEVRMLNOTEVENT : "Browser::createVrmlFromUrl: Expected event name in parameter 3"
			throw CAnmError(ANMMSG_ESCREATEVRMLNOTEVENT);
		}

		int sz;
		if (((sz = pUrl->size()) > 0) && str->length())
		{
			pBrowser->createVrmlFromUrl(pUrl, pNode, str->GetBuf(), NULL, NULL);
		}
	}

	return AnmESVar(0L);
}


AnmESVar CAnmESBrowserObject::createVrmlFromString(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	AnmESVar v;

	if (nArgs > 0)
	{
		CAnmESString *pESStr = args[0];

		if (pESStr)
		{
			String s = pESStr->GetString();

			CAnmSAIScene *pScene = pBrowser->createVrmlFromString(s, pBrowser->GetContainingNode());

			if (pScene)
			{
				std::vector<CAnmSAINode *> *pRootNodes = pScene->GetRootNodes();

				if (pRootNodes)
				{
					int sz = pRootNodes->size();

					AnmESVar *va = new AnmESVar[sz];

					for (int i = 0; i < sz; i++)
					{
						CAnmSAINode *pSAINode = (*pRootNodes)[i];

						CAnmESNodeObject *pNodeObj = new CAnmESNodeObject(pSAINode, m_owner);
						va[i] = pNodeObj;

						pScene->RemoveRootNode(pSAINode);

					}

					CAnmESObject *pMFNode = CAnmESMFNode::Create(m_owner, sz, va);

					delete[] va;


					return pMFNode;
				}
			}
		}

	}

	return v;
}

AnmESVar CAnmESBrowserObject::createX3DFromString(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	AnmESVar v;

	if (nArgs > 0)
	{
		CAnmESString *pESStr = args[0];

		if (pESStr)
		{
			String s = pESStr->GetString();

			CAnmSAIScene *pScene = pBrowser->createVrmlFromString(s, pBrowser->GetContainingNode());

			if (pScene)
				v = new CAnmESScene(pScene, m_owner);
		}

	}

	return v;
}


AnmESVar CAnmESBrowserObject::name()
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;
	AnmESVar v = (AnmESVar) new CAnmESString(new CAnmString(pBrowser->getName()),
		m_owner);
	return v;
}

AnmESVar CAnmESBrowserObject::version()
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;
	AnmESVar v = (AnmESVar) new CAnmESString(new CAnmString(pBrowser->getVersion()),
		m_owner);
	return v;
}

AnmESVar CAnmESBrowserObject::currentFrameRate()
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;
	AnmESVar v = (AnmESVar) (double) pBrowser->getCurrentFrameRate();
	return v;
}

AnmESVar CAnmESBrowserObject::currentSpeed()
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;
	AnmESVar v = (AnmESVar) (double) pBrowser->getCurrentSpeed();
	return v;
}

AnmESVar CAnmESBrowserObject::currentScene()
{
	AnmESVar v = (CAnmESObject *) NULL;

	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	CAnmExecutionContext *pContext = pBrowser->getExecutionContext();
	if (pContext->GetClass()->IsA(GETCLASS(CAnmSAIScene)))
	{
		CAnmESScene *pESScene = new CAnmESScene((CAnmSAIScene *)pContext, m_owner);
		v = pESScene;
	}
	else
	{
		CAnmESExecutionContext *pESContext = new CAnmESExecutionContext(pContext, m_owner);
		v = pESContext;
	}

	return v;
}

AnmESVar CAnmESBrowserObject::getCurrentFrameRate(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;
	AnmESVar v = (AnmESVar) (double) pBrowser->getCurrentFrameRate();
	return v;
}

AnmESVar CAnmESBrowserObject::getCurrentSpeed(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;
	AnmESVar v = (AnmESVar) (double) pBrowser->getCurrentSpeed();
	return v;
}

AnmESVar CAnmESBrowserObject::getName(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;
	AnmESVar v = (AnmESVar) new CAnmESString(new CAnmString(pBrowser->getName()),
		m_owner);
	return v;
}

AnmESVar CAnmESBrowserObject::getVersion(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;
	AnmESVar v = (AnmESVar) new CAnmESString(new CAnmString(pBrowser->getVersion()),
		m_owner);
	return v;
}


AnmESVar CAnmESBrowserObject::print(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	if (nArgs < 1)
	{
		// runtime warning here
	}
	else
	{
		CAnmESString *pStr = args[0];
		if (pStr == NULL)
			pStr = args[0].toString(m_owner);

		String s = pStr->GetString();
		if (s->length())
			pBrowser->print(s->GetBuf());
	}

	return AnmESVar(0L);
}

AnmESVar CAnmESBrowserObject::addRoute(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	if (nArgs < 4)
	{
		// runtime warning here
	}
	else
	{
		CAnmNode *pFromNode = (CAnmNode *) args[0];
		CAnmESString *fromFieldStr = (CAnmESString *) args[1];
		if (fromFieldStr == NULL)
			fromFieldStr = args[1].toString(m_owner);
		String fromField = fromFieldStr->GetString();

		CAnmNode *pToNode = (CAnmNode *) args[2];
		CAnmESString *toFieldStr = (CAnmESString *) args[3];
		if (toFieldStr == NULL)
			toFieldStr = args[3].toString(m_owner);
		String toField = toFieldStr->GetString();

		if (pFromNode == NULL)
		{
			// runtime error here
		}
		else if (pToNode == NULL)
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
			pBrowser->addRoute(pFromNode, fromField->GetBuf(), pToNode, toField->GetBuf());
		}
	}

	return AnmESVar(0L);
}

AnmESVar CAnmESBrowserObject::deleteRoute(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	if (nArgs < 4)
	{
		// runtime warning here
	}
	else
	{
		CAnmNode *pFromNode = (CAnmNode *) args[0];
		CAnmESString *fromFieldStr = (CAnmESString *) args[1];
		if (fromFieldStr == NULL)
			fromFieldStr = args[1].toString(m_owner);
		String fromField = fromFieldStr->GetString();

		CAnmNode *pToNode = (CAnmNode *) args[2];
		CAnmESString *toFieldStr = (CAnmESString *) args[3];
		if (toFieldStr == NULL)
			toFieldStr = args[3].toString(m_owner);
		String toField = toFieldStr->GetString();

		if (pFromNode == NULL)
		{
			// runtime error here
		}
		else if (pToNode == NULL)
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
			pBrowser->deleteRoute(pFromNode, fromField->GetBuf(), pToNode, toField->GetBuf());
		}
	}

	return AnmESVar(0L);
}

AnmESVar CAnmESBrowserObject::setDescription(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	if (nArgs < 1)
	{
		// runtime warning here
	}
	else
	{
		CAnmESString *pStr = args[0];
		if (pStr == NULL)
			pStr = args[0].toString(m_owner);

		String s = pStr->GetString();
		if (s->length())
			pBrowser->setDescription(s->GetBuf());
	}

	return AnmESVar(0L);
}

AnmESVar CAnmESBrowserObject::moveView(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	if (nArgs < 1)
	{
		// runtime warning here
	}
	else
	{
		if (nArgs > 1)
		{
			int axis = args[0];
			int direction = args[1];
			pBrowser->moveView((eAnmBrowserNavAxis) axis, direction);
		}
	}

	return AnmESVar(0L);
}

AnmESVar CAnmESBrowserObject::rotateView(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	if (nArgs < 1)
	{
		// runtime warning here
	}
	else
	{
		if (nArgs > 1)
		{
			int axis = args[0];
			int direction = args[1];
			pBrowser->rotateView((eAnmBrowserNavAxis) axis, direction);
		}
	}

	return AnmESVar(0L);
}

AnmESVar CAnmESBrowserObject::orbitView(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	if (nArgs < 1)
	{
		// runtime warning here
	}
	else
	{
		if (nArgs > 1)
		{
			int axis = args[0];
			int direction = args[1];
			pBrowser->orbitView((eAnmBrowserNavAxis) axis, direction);
		}
	}

	return AnmESVar(0L);
}

AnmESVar CAnmESBrowserObject::firstViewpoint(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	pBrowser->firstViewpoint();

	return AnmESVar(0L);
}

AnmESVar CAnmESBrowserObject::lastViewpoint(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	pBrowser->lastViewpoint();

	return AnmESVar(0L);
}


AnmESVar CAnmESBrowserObject::nextViewpoint(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	pBrowser->nextViewpoint();

	return AnmESVar(0L);
}

AnmESVar CAnmESBrowserObject::previousViewpoint(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	pBrowser->previousViewpoint();

	return AnmESVar(0L);
}

AnmESVar CAnmESBrowserObject::getViewpoints()
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	std::vector<class CAnmSAINode *> *pViewpoints =
		pBrowser->getViewpoints();

	if (pViewpoints)
	{
		int sz = pViewpoints->size();

		if (sz <= 0)
			return (CAnmESObject *) NULL;

		CAnmESObject **ppESNodes = new CAnmESObject *[sz];

		for (int i = 0; i < sz; i++)
		{
			CAnmSAINode *pSAINode = (*pViewpoints)[i];

			CAnmESNodeObject *pNodeObj = new CAnmESNodeObject(pSAINode, m_owner);
			ppESNodes[i] = pNodeObj;
		}
		
		CAnmESUserObject *pUserObj = new CAnmESUserObject(m_owner, sz, ppESNodes);

		delete[] ppESNodes;

		return pUserObj;
	}

	return (CAnmESObject *) NULL;
}

AnmESVar CAnmESBrowserObject::showConsole(int nArgs, AnmESVar *args)
{
	CAnmBrowser *pBrowser = (CAnmBrowser *) m_object;

	pBrowser->showConsole();

	return AnmESVar(0L);
}

// Tables for Callbacks, Methods and Put/GetProperties

// Browser object
DEFINE_ESMETHOD(CAnmESBrowserObject, createVrmlFromURL);
DEFINE_ESMETHOD(CAnmESBrowserObject, createVrmlFromString);
DEFINE_ESMETHOD(CAnmESBrowserObject, createX3DFromString);
DEFINE_ESMETHOD(CAnmESBrowserObject, setDescription);
DEFINE_ESMETHOD(CAnmESBrowserObject, print);
DEFINE_ESMETHOD(CAnmESBrowserObject, addRoute);
DEFINE_ESMETHOD(CAnmESBrowserObject, deleteRoute);
DEFINE_ESMETHOD(CAnmESBrowserObject, getCurrentFrameRate);
DEFINE_ESMETHOD(CAnmESBrowserObject, getCurrentSpeed);
DEFINE_ESMETHOD(CAnmESBrowserObject, getName);
DEFINE_ESMETHOD(CAnmESBrowserObject, getVersion);
DEFINE_ESMETHOD(CAnmESBrowserObject, moveView);
DEFINE_ESMETHOD(CAnmESBrowserObject, rotateView);
DEFINE_ESMETHOD(CAnmESBrowserObject, orbitView);
DEFINE_ESMETHOD(CAnmESBrowserObject, firstViewpoint);
DEFINE_ESMETHOD(CAnmESBrowserObject, lastViewpoint);
DEFINE_ESMETHOD(CAnmESBrowserObject, nextViewpoint);
DEFINE_ESMETHOD(CAnmESBrowserObject, previousViewpoint);
DEFINE_ESMETHOD(CAnmESBrowserObject, showConsole);
DEFINE_ESREADONLYPROPERTY(CAnmESBrowserObject, name, name);
DEFINE_ESREADONLYPROPERTY(CAnmESBrowserObject, version, version);
DEFINE_ESREADONLYPROPERTY(CAnmESBrowserObject, currentFrameRate, currentFrameRate);
DEFINE_ESREADONLYPROPERTY(CAnmESBrowserObject, currentSpeed, currentSpeed);
DEFINE_ESREADONLYPROPERTY(CAnmESBrowserObject, currentScene, currentScene);
DEFINE_ESREADONLYPROPERTY(CAnmESBrowserObject, viewpoints, getViewpoints);

// member table
BEGIN_DEFINE_MEMBERS(CAnmESBrowserObject)
ESMETHODID(CAnmESBrowserObject, createVrmlFromURL),
ESMETHODID(CAnmESBrowserObject, createVrmlFromString),
ESMETHODID(CAnmESBrowserObject, createX3DFromString),
ESMETHODID(CAnmESBrowserObject, setDescription),
ESMETHODID(CAnmESBrowserObject, print),
ESMETHODID(CAnmESBrowserObject, addRoute),
ESMETHODID(CAnmESBrowserObject, deleteRoute),
ESMETHODID(CAnmESBrowserObject, getCurrentFrameRate),
ESMETHODID(CAnmESBrowserObject, getCurrentSpeed),
ESMETHODID(CAnmESBrowserObject, getName),
ESMETHODID(CAnmESBrowserObject, getVersion),
ESMETHODID(CAnmESBrowserObject, moveView),
ESMETHODID(CAnmESBrowserObject, rotateView),
ESMETHODID(CAnmESBrowserObject, orbitView),
ESMETHODID(CAnmESBrowserObject, firstViewpoint),
ESMETHODID(CAnmESBrowserObject, lastViewpoint),
ESMETHODID(CAnmESBrowserObject, nextViewpoint),
ESMETHODID(CAnmESBrowserObject, previousViewpoint),
ESMETHODID(CAnmESBrowserObject, showConsole),
ESPROPID(CAnmESBrowserObject, name),
ESPROPID(CAnmESBrowserObject, version),
ESPROPID(CAnmESBrowserObject, currentFrameRate),
ESPROPID(CAnmESBrowserObject, currentSpeed),
ESPROPID(CAnmESBrowserObject, currentScene),
ESPROPID(CAnmESBrowserObject, viewpoints),
END_DEFINE_MEMBERS

DEFINE_ABSTRACT_CLASS(CAnmESBrowserObject, CAnmESBuiltinObject);
