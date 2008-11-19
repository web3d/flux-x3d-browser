/********************************************************************************
 * Flux
 *
 * File: anmproto.h
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

#ifndef _anmproto_h
#define _anmproto_h

#include "anmgroup.h"
#include "anmsymbol.h"
#include "anmprotobuilder.h"

#include <vector>

// callback handling-- have to do it differently from core objects
typedef struct sAnmProtoCallbackList {
	int memberOffset;
	CAnmCallbackList callbackList;
} sAnmProtoCallbackList ;

typedef std::vector<sAnmProtoCallbackList> tAnmProtoCallbackLists;

// IS support
typedef struct sAnmISMember {
	CAnmNode		*pNode;
	CLASSMEMBERID	 mid;
} sAnmISMember;

typedef std::vector<sAnmISMember> tAnmISMap;

typedef struct sAnmISCallbackData {
	class CAnmProto		*pProto;
	CLASSMEMBERID	mid;
} sAnmISCallbackData;

typedef std::vector<sAnmISCallbackData *> tAnmISCallbackDataList;

class CAnmProto : public CAnmGroup
{

protected:

	class CAnmProtoSymbol		*m_protosymbol;		// type info for this proto
	CAnmNode					*m_impl;			// what I'm implemented as; either a built-in
													// or another PROTO
	BOOL						 m_finalized;
	std::vector<class CAnmField *>	 m_deferredValues;
	std::vector<class CAnmField *>	 m_deferredDefaultValues;

	tAnmProtoCallbackLists		m_callbackLists;

	std::vector<tAnmISMap *>	m_ISmaps;			// maps from external interface to implementation
													// fields
	tAnmISCallbackDataList		m_IScallbackDataList;

	NodeArray					 m_nodelocals;
	int							 m_numnodelocals;

	std::list<class CAnmRoute *>
								 m_routes;
	String						 m_baseUrl;

	virtual void AllocMembers(int nMembers);
	virtual void AllocLocals(int nMembers);

	virtual void InitCallbackList(CLASSMEMBERID mid);
	virtual sAnmProtoCallbackList *LookupCallbackList(CLASSMEMBERID mid);

	virtual tAnmISMap *LookupISMap(CLASSMEMBERID mid);
	virtual void AddISMember(CLASSMEMBERID mid, CAnmNode *ISNode, CLASSMEMBERID ISMid);
	virtual void AddISCallback(CLASSMEMBERID mid, CAnmNode *ISNode, CLASSMEMBERID ISMid);

		
	static void HandleISCallback(CAnmObject *sourceObject, CLASSMEMBERID reason,
									  void *callData, void *userData);
	virtual void HandleISCallback(CLASSMEMBERID mid, void *callData);

	static CAnmObject *Create();

	virtual void SetSceneGraph(NodeArray *pChildren);
	virtual void SetImplementationNode(CAnmNode *pImp);

	virtual void SetValueFromDefault(CLASSMEMBERID mid, void *value);

	virtual class CAnmRoute *FindRoute(class CAnmNode *pFromNode, CLASSMEMBERID frommid,
		class CAnmNode *pToNode, CLASSMEMBERID tomid);

public:

	// constructor/destructor
	CAnmProto();
	virtual ~CAnmProto();

	// CAnmGroup overrides
	virtual void SetValue(CLASSMEMBERID mid, void *value);
	virtual void GetValue(CLASSMEMBERID mid, void **pValue);

	virtual void CallMethod(CLASSMEMBERID mid, void *value);

	virtual void Realize();				// build lower-level rendering structures
	virtual void Render(class CAnmDevice *pDevice);			// draw it

	virtual void SetChildren(NodeArray *pChildren);

	virtual CAnmCallback *AddCallback(CLASSMEMBERID mid, CAnmCallbackFunction userFunction, void *userData);
	virtual void RemoveCallback(CLASSMEMBERID mid, CAnmCallback *pCB);
	virtual void CallCallbacks(CLASSMEMBERID mid, void *cbdata);
	
	virtual class CAnmProto *GetProtoContext()
	{
		return this;
	}

	virtual void SetBaseUrl(String baseUrl);
	virtual String GetBaseUrl()
	{
		return m_baseUrl;
	}

	// New methods
	// N.B.: these should be protected methods
	virtual void StoreNode(class CAnmNode *pNode, NODELOCALOFFSET localOffset);
	virtual class CAnmNode *LoadNode(NODELOCALOFFSET localOffset);
	virtual CAnmNode *GetNode(const char *nodename);
	virtual NodeArray *GetRootNodes();
	virtual CAnmNode *CreateNode(const char *nodetypename);
	virtual CAnmProto *CreateProto(const char *prototypename);

	virtual void Finalize();

	virtual class CAnmRoute *AddRoute(class cApplication *pApp, class CAnmNode *pFromNode, CLASSMEMBERID frommid,
		class CAnmNode *pToNode, CLASSMEMBERID tomid);
	virtual class CAnmRoute *AddRoute(class cApplication *pApp, class CAnmNode *pFromNode, const char *fromfield,
		class CAnmNode *pToNode, const char *tofield);

	virtual void RemoveRoute(class CAnmNode *pFromNode, CLASSMEMBERID frommid,
		class CAnmNode *pToNode, CLASSMEMBERID tomid);
	virtual void RemoveRoute(class CAnmNode *pFromNode, const char *fromfield,
		class CAnmNode *pToNode, const char *tofield);
	virtual void RemoveRoute(class CAnmRoute *pRoute);

	virtual char *GetSpecificationVersion();
	virtual eAnmX3DEncoding GetEncoding();
	virtual class CAnmProfileInfo *GetProfile();
	virtual std::vector<class CAnmComponentInfo *> *GetComponents();
	virtual const char *GetWorldURL();


	// Accessors
	virtual CAnmNode *GetImplementationNode();
	virtual void GetSceneGraph(NodeArray **pChildren);

	virtual void SetProtoSymbol(class CAnmProtoSymbol *pProtoSym)
	{
		m_protosymbol = pProtoSym;
	}
	virtual class CAnmProtoSymbol *GetProtoSymbol()
	{
		return m_protosymbol;
	}

	// Linkage for callbacks, methods, values. include in all classes derived from this one
	DECLARE_CLASS(CAnmProto);

	// ... and friends
	friend class CAnmProtoSymbol;
	friend class CAnmProtoBuilder;
};

inline CAnmClass *CAnmProto::GetClass()
{
	if (m_protosymbol)
		return m_protosymbol->GetClass();
	else
		return CAnmProto::m_classdata;
}

#endif // _anmproto_h
