/********************************************************************************
 * Flux
 *
 * File: anmprotobuilder.h
 * Description: ProtoBuilder class -  creates node from
 *							PROTO definition
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

#ifndef _anmprotobuilder_h
#define _anmprotobuilder_h

#include <stack>
#include <vector>

typedef ULONG ProtoCode;
typedef std::vector<ProtoCode> tAnmProtoCode;

typedef enum eAnmProtoOps {
	eDefNode,
	eUseNode,
	eBeginMakeNode,
	eEndMakeNode,
	eNullNode,
	eSetValue,
	eSetSFNodeValue,
	eSetMFNodeValue,
	eAppendNode,
	eMakeMFNode,
	eMakeRoute,
	eAddISField,
	eSetSFNodeDefault,
	eSetMFNodeDefault,
} eAnmProtoOps;

// Support for silly XML-encoded containerField attribute
typedef struct sAnmContainerField {
	CLASSMEMBERID mid;
	NodeArray *pNodeArray;

	sAnmContainerField()
	{
		mid = NULL;
		pNodeArray = NULL;
	}

	sAnmContainerField(CLASSMEMBERID memberid, NodeArray *pNA)
	{
		mid = memberid;
		pNodeArray = pNA;
	}

} sAnmContainerField ;

typedef struct sAnmContainers {
	std::vector<sAnmContainerField> containerFields;
} sAnmContainers ;

typedef ULONG NODELOCALOFFSET;

#define ANMPROTOBUILDER_CODEBLOCKSIZE 256
#define ANMPROTONODEARRAY_ALLOC 128
#define ANMPROTOBADLOCALADDR ((NODELOCALOFFSET) -1)

class CAnmProtoBuilder
{

protected:

	class cApplication			*m_app;
	class CAnmProtoSymbol		*m_protosym;
	class CAnmProtoSymbol		*m_protosymForExtern;
	class CAnmProto				*m_proto;
	tAnmProtoCode				 m_code;
	int							 m_codeptr;
	int							 m_codesize;
	Node						 m_node;
	NodeArray					 m_nodeaccum;
	int							 m_nodeindex;
	std::stack<int>				 m_nodeindexstack;
	sAnmContainers				*m_containers;
	std::stack<sAnmContainers *> m_containerstack;
	NodeArray					*m_nodearray;
	char						*m_defname;
	NODELOCALOFFSET				 m_defaddr;
	int							 m_numnodelocals;
	void						*m_clonedvalue;

	// Run-time build helpers
	virtual void MakeNode();
	virtual void DefNode();
	virtual void UseNode();
	virtual void NullNode();
	virtual void MakeRoute();
	virtual void AddISField(class CAnmNode *pNode);
	virtual void SetValue(class CAnmNode *pNode);
	virtual void SetSFNodeValue(class CAnmNode *pNode);
	virtual void SetMFNodeValue(class CAnmNode *pNode);
	virtual void AppendNode();
	virtual void SetSFNodeDefault(class CAnmProto *pProto);
	virtual void SetMFNodeDefault(class CAnmProto *pProto);
	virtual CLASSMEMBERID FixupExternMid(CLASSMEMBERID mid);

	virtual ProtoCode ReadCode()
	{
		return m_code[m_codeptr++];
	}

	virtual void *ReadValue(eValueType valueType);

	// Routines to manage containers stack
	virtual void EnterContainerScope();
	virtual void LeaveContainerScope(class CAnmNode *pNode);
	virtual NodeArray *GetContainerField(CLASSMEMBERID mid);

	// Routines to manage node accumulation stack
	virtual void AccumNode(class CAnmNode *pNode);
	virtual void PopNodeAccums();
	virtual void ClearNodeAccums();

	virtual Node *NodeFromNodes();
	virtual NodeArray **NodeArrayFromNodes();

	// Local node storage (for def/use)
	virtual void StoreNode(class CAnmNode *pNode, NODELOCALOFFSET localOffset);
	virtual class CAnmNode *LoadNode(NODELOCALOFFSET localOffset);

	// Code generation help
	virtual void Gen(ProtoCode code)
	{
		m_code.push_back(code);
	}

	virtual void GenValue(eValueType valueType, void *value);
	virtual void *CloneValue(eValueType valueType, void *pValue);

public:

	// constructor/destructor
	CAnmProtoBuilder(class cApplication *pApp, class CAnmProtoSymbol *pProtoSym);
	virtual ~CAnmProtoBuilder();

	// Run-time build routines
	virtual void Build(class CAnmProto *pProto, NodeArray **ppNodesReturn);
	virtual void SetProtoSymForExtern(class CAnmProtoSymbol *pProtoSymForExtern);

	// ProtoCode generation routines
	virtual void GenBeginMakeNode(class CAnmClass *pClass);
	virtual void GenEndMakeNode();
	virtual NODELOCALOFFSET GenDefNode(const char *name);
	virtual void GenUseNode(NODELOCALOFFSET localOffset);
	virtual void GenNullNode();
	virtual void GenSetValue(CLASSMEMBERID mid, void *value, bool clonevalue);
	virtual void GenSetSFNodeValue(CLASSMEMBERID mid);
	virtual void GenSetMFNodeValue(CLASSMEMBERID mid);
	virtual void GenAppendNode(CLASSMEMBERID mid);
	virtual void GenSetSFNodeDefault(CLASSMEMBERID mid);
	virtual void GenSetMFNodeDefault(CLASSMEMBERID mid);
	virtual void GenMakeRoute(NODELOCALOFFSET fromLocalOffset, CLASSMEMBERID fromMid,
		NODELOCALOFFSET toLocalOffset, CLASSMEMBERID toMid);
	virtual void GenAddISField(CLASSMEMBERID protoMid, CLASSMEMBERID isMid);


	// Accessors
	class CAnmProtoSymbol *GetProtoSym()
	{
		return m_protosym;
	}

	class CAnmProtoSymbol *GetProtoSymForExtern()
	{
		return m_protosymForExtern;
	}
};


#endif // _anmprotobuilder_h
