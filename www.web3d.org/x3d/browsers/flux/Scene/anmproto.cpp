/********************************************************************************
 * Flux
 *
 * File: anmproto.cpp
 * Description: Proto instance
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
#include "anmproto.h"
#include "anmexecutioncontext.h"
#include "anmfields.h"
#include "anmroute.h"
#include "anmscene.h"
#include "anmsymbol.h"

// constructor/destructor
CAnmProto::CAnmProto() : CAnmGroup()
{
	m_impl = NULL;
	m_protosymbol = NULL;
	m_finalized = FALSE;
	m_baseUrl = NULL;
}

CAnmObject *CAnmProto::Create()
{
	return new CAnmProto;
}

CAnmProto::~CAnmProto()
{


	SafeUnRef(m_impl);
	SafeUnRef(m_baseUrl);

	std::list<CAnmRoute *>::iterator iter;

	for( 
		iter = m_routes.begin(); 
		iter != m_routes.end();
		iter++ )
	{
		SafeUnRef((*iter));
	}


	// krv:
	// Remove all Children here, before we destroy the IS callbacks,
	// which get used in cases where we call SetBind on VP Destructor.
	//
	RemoveAllChildren();



	// krv;
	// Must do this last, after we delete the Routes.

	int i;
	for (i = 0; i < m_ISmaps.size(); i++)
	{
		delete m_ISmaps[i];
	}

	for (i = 0; i < m_IScallbackDataList.size(); i++)
	{
		delete m_IScallbackDataList[i];
	}

}

void CAnmProto::AllocMembers(int nMembers)
{
	for (int i = 0; i < nMembers; i++)
	{
		tAnmISMap *pISMap = new tAnmISMap;
		m_ISmaps.push_back(pISMap);
	}
}

void CAnmProto::AllocLocals(int nLocals)
{
	m_nodelocals.reserve(nLocals);
	m_numnodelocals = nLocals;
}

void CAnmProto::StoreNode(class CAnmNode *pNode, NODELOCALOFFSET localOffset)
{
	assert(localOffset >= 0 && localOffset < m_numnodelocals);
	m_nodelocals[localOffset] = pNode;
}

CAnmNode *CAnmProto::LoadNode(NODELOCALOFFSET localOffset)
{
	assert(localOffset >= 0 && localOffset < m_numnodelocals);
	return m_nodelocals[localOffset];
}

void CAnmProto::InitCallbackList(CLASSMEMBERID mid)
{
	sAnmProtoCallbackList cblist;
	cblist.memberOffset = (int) mid->memberOffset;
	m_callbackLists.push_back(cblist);
}

sAnmProtoCallbackList *CAnmProto::LookupCallbackList(CLASSMEMBERID mid)
{
	for (int i = 0; i < m_callbackLists.size(); i++)
	{
		if (m_callbackLists[i].memberOffset == mid->memberOffset)
			return &m_callbackLists[i];
	}

	return NULL;
}

CAnmCallback *CAnmProto::AddCallback(CLASSMEMBERID mid, CAnmCallbackFunction userFunction, void *userData)
{
	sAnmProtoCallbackList *pCBList = LookupCallbackList(mid);
	assert(pCBList != NULL);

	CAnmCallback *pCB = new CAnmCallback(userFunction, this, mid, userData);

	pCBList->callbackList.AddCallback(pCB);

	return pCB;
}

void CAnmProto::RemoveCallback(CLASSMEMBERID mid, CAnmCallback *pCB)
{
	sAnmProtoCallbackList *pCBList = LookupCallbackList(mid);
	assert(pCBList != NULL);

	pCBList->callbackList.RemoveCallback(pCB);

	delete pCB;
}

void CAnmProto::CallCallbacks(CLASSMEMBERID mid, void *cbdata)
{
	sAnmProtoCallbackList *pCBList = LookupCallbackList(mid);

	// N.B.: the callback list might actually be NULL because
	// of an EXTERNPROTO field mismatch
	// assert(pCBList != NULL);

	if (pCBList)
		pCBList->callbackList.CallCallbacks(cbdata);
}

tAnmISMap *CAnmProto::LookupISMap(CLASSMEMBERID mid)
{
	int index = (int) mid->memberOffset;
	if (index < 0 || index >= m_ISmaps.size())
		return NULL;


	return m_ISmaps[index];
}

void CAnmProto::AddISMember(CLASSMEMBERID mid, CAnmNode *ISNode, CLASSMEMBERID ISMid)
{
	assert(ISNode != NULL);
	// N.B.: no longer do this because we need to be able to coerce
	// initializeOnly to inputOutput for SetValue operations
	// assert(ISMid->memberType == mid->memberType);
	
	tAnmISMap *pMap = LookupISMap(mid);

	sAnmISMember mem;
	mem.pNode = ISNode;
	mem.mid = ISMid;

	pMap->push_back(mem);

	// Also add the callback here
	if (mid->memberType == eAnmCallbackMember)
	{
		AddISCallback(mid, ISNode, ISMid);
	}
}

void CAnmProto::AddISCallback(CLASSMEMBERID mid, CAnmNode *ISNode, CLASSMEMBERID ISMid)
{
	sAnmISCallbackData *pCBdata = new sAnmISCallbackData;
	pCBdata->pProto = this;
	pCBdata->mid = mid;
	m_IScallbackDataList.push_back(pCBdata);

	ISNode->AddCallback(ISMid, HandleISCallback, pCBdata);
}

void CAnmProto::HandleISCallback(CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData, void *userData)
{
	sAnmISCallbackData *pCBData = (sAnmISCallbackData *) userData;

	assert(pCBData != NULL);
	pCBData->pProto->HandleISCallback(pCBData->mid, callData);
}

void CAnmProto::HandleISCallback(CLASSMEMBERID mid, void *callData)
{
	if (!m_realized)
		return;
	
	CallCallbacks(mid, callData);
}


void CAnmProto::SetValue(CLASSMEMBERID mid, void *value)
{
	if (!m_finalized)
	{
		CAnmField *pField = CAnmField::CreateField(mid->valueType, value);
		pField->SetMemberID(mid);
		m_deferredValues.push_back(pField);
		return;
	}

	// look up the ISmid in the IS table
	tAnmISMap *pISMap = LookupISMap(mid);

	if (pISMap == NULL)
		return;

	for (int i = 0; i < pISMap->size(); i++)
	{
		sAnmISMember *pIS = &(*pISMap)[i];

		CLASSMEMBERID ISmid = pIS->mid;
		CAnmNode *pISNode = pIS->pNode;

		if (ISmid->memberType == eAnmMethodMember)
			pISNode->CallMethod(ISmid, value);
		else if (ISmid->memberType == eAnmCallbackMember)
			pISNode->CallCallbacks(ISmid, value);
		else
			pISNode->SetValue(ISmid, value);
	}
}

void CAnmProto::SetValueFromDefault(CLASSMEMBERID mid, void *value)
{
	// defer this if we're not real, otherwise just set it
	if (!m_finalized)
	{
		CAnmField *pField = CAnmField::CreateField(mid->valueType, value);
		pField->SetMemberID(mid);
		m_deferredDefaultValues.push_back(pField);
		return;
	}

	SetValue(mid, value);
}

void CAnmProto::GetValue(CLASSMEMBERID mid, void **pValue)
{
	if (!m_finalized)
		return;

	// look up the ISmid in the IS table
	tAnmISMap *pISMap = LookupISMap(mid);

	if (pISMap == NULL)
		return;

	for (int i = 0; i < pISMap->size(); i++)
	{
		sAnmISMember *pIS = &(*pISMap)[i];
		CLASSMEMBERID ISmid = pIS->mid;
		CAnmNode *pISNode = pIS->pNode;

		pISNode->GetValue(ISmid, pValue);
	}
}

void CAnmProto::CallMethod(CLASSMEMBERID mid, void *value)
{
	if (!m_finalized)
	{
		CAnmField *pField = CAnmField::CreateField(mid->valueType, value);
		pField->SetMemberID(mid);
		m_deferredValues.push_back(pField);
		return;
	}

	// look up the ISmid in the IS table
	tAnmISMap *pISMap = LookupISMap(mid);

	if (pISMap == NULL)
		return;

	for (int i = 0; i < pISMap->size(); i++)
	{
		sAnmISMember *pIS = &(*pISMap)[i];
		CLASSMEMBERID ISmid = pIS->mid;
		CAnmNode *pISNode = pIS->pNode;

		pISNode->CallMethod(ISmid, value);
	}
}

void CAnmProto::Finalize()
{
	m_finalized = TRUE;

	// do the default values first
	for (int i = 0; i < m_deferredDefaultValues.size(); i++)
	{
		CAnmField *pField = m_deferredDefaultValues[i];
		CLASSMEMBERID mid = pField->GetMemberID();
		// Fix up mid's for extern proto's - these were saved with
		// in-body values that now need to get translated to the outer interface
		// for this particular proto declaration
		if (m_protosymbol->IsExternProto())
		{
			CAnmProtoSymbol *pExternProtoSym = m_protosymbol->GetProtoSymbolForExtern();
			if (pExternProtoSym)
			{
				// N.B.: note 'm_protosymbol' here, not 'pExternProtoSym'

				CAnmAttributeSymbol *pAttrSym = m_protosymbol->FindAttribute(mid->name);
				if (pAttrSym)
					mid = pAttrSym->GetClassMember();
			}
		}

		if (mid->memberType == eAnmValueMember || mid->memberType == eAnmInitMember)
			SetValue(mid, pField->GetData());
		else if (mid->memberType == eAnmMethodMember)
			CallMethod(mid, pField->GetData());
		pField->UnRef();
	}

	m_deferredDefaultValues.clear();

	// now any explicitly set ones
	for (i = 0; i < m_deferredValues.size(); i++)
	{
		CAnmField *pField = m_deferredValues[i];
		CLASSMEMBERID mid = pField->GetMemberID();
		if (mid->memberType == eAnmValueMember || mid->memberType == eAnmInitMember)
			SetValue(mid, pField->GetData());
		else if (mid->memberType == eAnmMethodMember)
			CallMethod(mid, pField->GetData());
		pField->UnRef();
	}

	m_deferredValues.clear();
}

void CAnmProto::Realize()
{
	if (!m_finalized)
		return;

	// Realize all my contained children
	CAnmGroup::Realize();
}

void CAnmProto::Render(CAnmDevice *pDevice)
{
	// this is only for sanity
	assert(FALSE);
}

void CAnmProto::SetChildren(NodeArray *pChildren)
{
	// this is only for sanity
	assert(FALSE);
}

void CAnmProto::SetSceneGraph(NodeArray *pChildren)
{
	assert((*pChildren).size() > 0);

	SetImplementationNode((*pChildren)[0]);

	CAnmGroup::SetChildren(pChildren);
}
	
void CAnmProto::SetImplementationNode(CAnmNode *pImp)
{
	SafeUnRef(m_impl);
	m_impl = pImp;

	if (m_impl)
		m_impl->Ref();
}

CAnmNode *CAnmProto::GetImplementationNode()
{
	if (m_impl == NULL)
		return NULL;

	return m_impl->GetImplementationNode();
}

void CAnmProto::GetSceneGraph(NodeArray **ppChildren)
{
	CAnmGroup::GetChildren(ppChildren);
}

CAnmNode *CAnmProto::GetNode(const char *nodename)
{
	CAnmNodeSymbol *pNodeSymbol = 
		(CAnmNodeSymbol *) m_protosymbol->FindNodeSymbol((char *) nodename);
	if (pNodeSymbol)
	{
		// N.B.: restructure this
		CAnmNode *pNode = LoadNode(pNodeSymbol->GetLocalOffset());

		return pNode;
	}
	else
		return NULL;
}


NodeArray *CAnmProto::GetRootNodes()
{
	NodeArray *pRootNodes = NULL;
	GetChildren(&pRootNodes);
	return pRootNodes;
}

CAnmRoute *CAnmProto::AddRoute(class cApplication *pApp, CAnmNode *pFromNode, CLASSMEMBERID frommid,
		CAnmNode *pToNode, CLASSMEMBERID tomid)
{
	CAnmRoute *pRoute = new CAnmRoute(pApp, pFromNode, frommid, pToNode, tomid, TRUE);
	m_routes.push_back(pRoute);

	return pRoute;
}

CAnmRoute *CAnmProto::AddRoute(class cApplication *pApp, CAnmNode *pFromNode, const char *fromfield,
		CAnmNode *pToNode, const char *tofield)
{
	CAnmRoute *pRoute = NULL;
	CLASSMEMBERID from_mid, to_mid;
	// N.B.: length
	char buf[ANMMAXSTRBUF];

	if (pFromNode == NULL || pToNode == NULL)
		// N.B.: should warn, return error code or gen exception
		return NULL;

	from_mid = pFromNode->GetClass()->FindCallback((char *)fromfield);
	if (from_mid == NULL)
	{
		// try the automatic _changed thing
		sprintf(buf, "%s_changed", fromfield);
		from_mid = pFromNode->GetClass()->FindCallback(buf);
	}

	if (from_mid == NULL)
		// N.B.: should warn, return error code or gen exception
		return NULL;

	to_mid = pToNode->GetClass()->FindMethod((char *)tofield);
	if (to_mid == NULL)
	{
		// try the automatic set_ thing
		sprintf(buf, "set_%s", tofield);
		to_mid = pToNode->GetClass()->FindMethod(buf);
	}

	if (to_mid == NULL)
		// N.B.: should warn, return error code or gen exception
		return NULL;

	// All good; we found the nodes and their fields. Wire them up
	return AddRoute(pApp, pFromNode, from_mid, pToNode, to_mid);
}


void CAnmProto::RemoveRoute(CAnmNode *pFromNode, CLASSMEMBERID frommid,
		CAnmNode *pToNode, CLASSMEMBERID tomid)
{
	CAnmRoute *pRoute = FindRoute(pFromNode, frommid, pToNode, tomid);
	if (pRoute)
		RemoveRoute(pRoute);
}

void CAnmProto::RemoveRoute(CAnmNode *pFromNode, const char *fromfield,
		CAnmNode *pToNode, const char *tofield)
{
	CAnmRoute *pRoute = NULL;
	CLASSMEMBERID from_mid, to_mid;
	// N.B.: length
	char buf[ANMMAXSTRBUF];

	if (pFromNode == NULL || pToNode == NULL)
		// N.B.: should warn, return error code or gen exception
		return;

	from_mid = pFromNode->GetClass()->FindCallback((char *)fromfield);
	if (from_mid == NULL)
	{
		// try the automatic _changed thing
		sprintf(buf, "%s_changed", fromfield);
		from_mid = pFromNode->GetClass()->FindCallback(buf);
	}

	if (from_mid == NULL)
		// N.B.: should warn, return error code or gen exception
		return;

	to_mid = pToNode->GetClass()->FindMethod((char *)tofield);
	if (to_mid == NULL)
	{
		// try the automatic set_ thing
		sprintf(buf, "set_%s", tofield);
		to_mid = pToNode->GetClass()->FindMethod(buf);
	}

	if (to_mid == NULL)
		// N.B.: should warn, return error code or gen exception
		return;

	RemoveRoute(pFromNode, from_mid, pToNode, to_mid);
}

void CAnmProto::RemoveRoute(CAnmRoute *pRoute)
{
	assert(pRoute);

	m_routes.remove(pRoute);
}

CAnmRoute *CAnmProto::FindRoute(CAnmNode *pFromNode, CLASSMEMBERID frommid,
		CAnmNode *pToNode, CLASSMEMBERID tomid)
{
	CAnmRoute *pFoundRoute = NULL;

	std::list<CAnmRoute *>::iterator iter;

	for( 
		iter = m_routes.begin(); 
		iter != m_routes.end();
		iter++ )
	{
		CAnmRoute *pRoute = *iter;

		if (frommid == pRoute->GetFromCallback() &&
			tomid == pRoute->GetToMethod() &&
			pFromNode == pRoute->GetFromNode() &&
			pToNode == pRoute->GetToNode())
		{
			*pFoundRoute = *pRoute;
			break;
		}
	}

	return pFoundRoute;
}

char *CAnmProto::GetSpecificationVersion()
{
	CAnmScene *pScene = GetContainingScene();
	assert(pScene);

	return pScene->GetSpecificationVersion();
}

eAnmX3DEncoding CAnmProto::GetEncoding()
{
	CAnmScene *pScene = GetContainingScene();
	assert(pScene);

	return pScene->GetEncoding();
}

class CAnmProfileInfo *CAnmProto::GetProfile()
{
	CAnmScene *pScene = GetContainingScene();
	assert(pScene);

	return pScene->GetProfile();
}

std::vector<class CAnmComponentInfo *> *CAnmProto::GetComponents()
{
	CAnmScene *pScene = GetContainingScene();
	assert(pScene);

	return pScene->GetComponents();
}

const char *CAnmProto::GetWorldURL()
{
	CAnmScene *pScene = GetContainingScene();
	assert(pScene);

	return pScene->GetWorldURL();
}

CAnmNode *CAnmProto::CreateNode(const char *nodetypename)
{
	CAnmClassSymbol *pClassSym = m_protosymbol->FindClassSymbol((char *) nodetypename);

	CAnmNode *pNode = NULL;
	if (pClassSym)
	{
		pNode = (CAnmNode *) pClassSym->GetClass()->Create();
		pNode->SetBaseUrl(m_baseUrl);
	}

	return pNode;
}

CAnmProto *CAnmProto::CreateProto(const char *prototypename)
{
#if IMPLEMENT_LATER
	CAnmProto *pProto = m_proto->CreateProto(prototypename);

	if (pProto)
	{
		CAnmSAINode *pSAINode = CreateSAINode(pProto);
		return pSAINode;
	}
	else
		return NULL;
#endif

	return NULL;
}

void CAnmProto::SetBaseUrl(String baseUrl)
{
	SafeUnRef(m_baseUrl);
	m_baseUrl = baseUrl;
	m_baseUrl->Ref();

	CAnmGroup::SetBaseUrl(baseUrl);
}

// Linkage for callbacks, methods, values. include in all classes derived from this one
// Special version of gunk for Proto classes... cf. anmclass.h in core for basis of these
// macros... TP

#define DEFINE_PROTO_CLASS(_cls_, _super_) \
static CAnmObject *create##_cls_() \
{ \
	return new _cls_(); \
} \
\
static CAnmClass classdata = \
{ \
	#_cls_, \
	create##_cls_, \
	NULL, \
	0, \
	_super_::m_classdata, \
	NULL, \
}; \
CAnmClass *_cls_::m_classdata = &classdata; \
CAnmClass *_cls_::GetClassData() { return _cls_::m_classdata; }

DEFINE_PROTO_CLASS(CAnmProto, CAnmGroup)

