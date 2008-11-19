/********************************************************************************
 * Flux
 *
 * File: anmprotobuilder.cpp
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

#include "stdafx.h"
#include "anmprotobuilder.h"
#include "anmexecutioncontext.h"
#include "anmfields.h"
#include "anmmessage.h"
#include "anmsymbol.h"
#include "anmproto.h"
#include "anmroute.h"

#ifdef DUMP_PROTO_BUILD
inline void dump(char *format, ... )
{
	char buf[1024];

	va_list arglist;
	va_start (arglist, format);
	vsprintf (buf, format, arglist);
	va_end (arglist);


	DP(buf);
}
#else
inline void dump(char *format, ... )
{
}
#endif

CAnmProtoBuilder::CAnmProtoBuilder(cApplication *pApp, CAnmProtoSymbol *pProtoSym)
{
	assert(pApp);
	assert(pProtoSym);

	m_app = pApp;
	m_protosym = pProtoSym;
	m_protosymForExtern = NULL;
	m_code.reserve(ANMPROTOBUILDER_CODEBLOCKSIZE);
	m_nodearray = NULL;
	m_numnodelocals = 0;
	m_containers = NULL;
	m_clonedvalue = NULL;
}

CAnmProtoBuilder::~CAnmProtoBuilder()
{
	SafeUnRef(m_nodearray);
}

void CAnmProtoBuilder::SetProtoSymForExtern(class CAnmProtoSymbol *pProtoSymForExtern)
{
	m_protosymForExtern = pProtoSymForExtern;
}

// Run-time build routines
// This is where it all happens, i.e. the scene graph gets built

void CAnmProtoBuilder::Build(CAnmProto *pProto, NodeArray **ppNodesReturn)
{
#ifdef DUMP_PROTO_BUILD
	dump("-------------- Begin build -------------\n");
#endif

	// Init
	m_proto = pProto;
	SafeUnRef(m_nodearray);
	m_nodeaccum.reserve(ANMPROTONODEARRAY_ALLOC);
	m_nodeaccum.resize(0);
	m_nodeindex = 0;
	m_defname = NULL;
	m_defaddr = ANMPROTOBADLOCALADDR;
	m_codeptr = 0;
	m_codesize = m_code.size();

	if (pProto && m_numnodelocals > 0)
		pProto->AllocLocals(m_numnodelocals);

	// start up accumulation stack
	m_nodeindexstack.push(0);

	// start up container field stack
	EnterContainerScope();

	// now build it
	while (m_codeptr < m_codesize)
	{
		eAnmProtoOps opcode = (eAnmProtoOps) ReadCode();

		switch (opcode)
		{
			case eDefNode :
				DefNode();
				break;

			case eUseNode :
				UseNode();
				break;

			case eNullNode :
				NullNode();
				break;

			case eBeginMakeNode	:
				MakeNode();
				break;

			case eMakeRoute :
				MakeRoute();
				break;

			case eSetSFNodeValue :
				SetSFNodeValue(m_proto);
				ClearNodeAccums();
				break;

			case eSetMFNodeValue :
				SetMFNodeValue(m_proto);
				ClearNodeAccums();
				break;

			case eAppendNode :
				AppendNode();
				ClearNodeAccums();
				break;

			case eSetSFNodeDefault :
				SetSFNodeDefault(m_proto);
				ClearNodeAccums();
				break;

			case eSetMFNodeDefault :
				SetMFNodeDefault(m_proto);
				ClearNodeAccums();
				break;

			default :

				// ANMMSG_PROTOBUILDERBADOPTOPLEVEL : "[ProtoBuilder] bad opcode at top PROTO level"
				throw CAnmError(ANMMSG_PROTOBUILDERBADOPTOPLEVEL);
				break;
		}
	}

	// Make sure all lingering MF SetValues are done (XML encoding only)
	LeaveContainerScope(m_proto);

#ifdef DUMP_PROTO_BUILD
	dump("-------------- End build -------------\n");
#endif

	NodeArrayFromNodes();
	if (m_nodearray->size() > 0)
	{
		*ppNodesReturn = m_nodearray;
	}

}

void CAnmProtoBuilder::MakeNode()
{
	// signature: BeginMakeNode pClass
	CAnmClass *pClass = (CAnmClass *) ReadCode();

#ifdef DUMP_PROTO_BUILD
	dump("Begin make node %s\n", pClass->m_className);
#endif

	assert (pClass != NULL);
	CAnmNode *pNewNode = (CAnmNode *) pClass->Create();

	m_app->AddGlobalObject(pNewNode);

	// get the def name and local address, if any, for a DEF
	char *defname = m_defname;
	NODELOCALOFFSET defaddr = m_defaddr;
	if (defname)
	{
		assert(defaddr != ANMPROTOBADLOCALADDR);
		StoreNode(pNewNode, defaddr);

		if (pNewNode)
		{
			String s = new CAnmString(defname);
			pNewNode->SetName(s);
			s->UnRef();
		}

		// clean up the defname and addr; don't want anybody getting confused
		m_defname = NULL;
		m_defaddr = ANMPROTOBADLOCALADDR;
	
	}

	// Ok, we've got a new node; accumulate it
	m_nodeindexstack.push(m_nodeindex + 1);			// we'll get back to here eventually
	AccumNode(pNewNode);

	// Start new scope for this node's containerFields
	EnterContainerScope();

	BOOL done = FALSE;
	while (!done)
	{
		eAnmProtoOps opcode = (eAnmProtoOps) ReadCode();

		switch (opcode)
		{
			case eDefNode :
				DefNode();
				break;

			case eUseNode :
				UseNode();
				break;

			case eNullNode :
				NullNode();
				break;

			case eBeginMakeNode	:
				MakeNode();
				break;

			case eEndMakeNode	:
				// signature: EndMakeNode
				done = TRUE;
				break;

			case eMakeRoute :
				MakeRoute();
				break;

			case eAddISField:
				AddISField(pNewNode);
				break;

			case eSetValue :
				SetValue(pNewNode);
				break;

			case eSetSFNodeValue :
				SetSFNodeValue(pNewNode);
				break;

			case eSetMFNodeValue :
				SetMFNodeValue(pNewNode);
				break;

			case eAppendNode :
				AppendNode();
				break;

			default :

				// ANMMSG_PROTOBUILDERBADOPMAKENODE : "[ProtoBuilder] bad opcode in MakeNode"
				throw CAnmError(ANMMSG_PROTOBUILDERBADOPMAKENODE);
				break;
		}

		if (!done && m_codeptr >= m_codesize)
		{
			// ANMMSG_PROTOBUILDERENDOFCODE : "[ProtoBuilder] unexpected end of code stream in MakeNode"
			throw CAnmError(ANMMSG_PROTOBUILDERENDOFCODE);
		}
	}

	// Make sure all lingering MF SetValues are done (XML encoding only)
	LeaveContainerScope(pNewNode);

	// pop the accums
	PopNodeAccums();

#ifdef DUMP_PROTO_BUILD
	dump("End make node %s\n", pClass->m_className);
#endif
}


void CAnmProtoBuilder::DefNode()
{
	// signature: DEF name, localOffset
	m_defname = (char *) ReadCode();
	m_defaddr = (NODELOCALOFFSET) ReadCode();

#ifdef DUMP_PROTO_BUILD
	dump("DefNode %d\t%s\n", m_defaddr, m_defname);
#endif
}

void CAnmProtoBuilder::UseNode()
{
	// signature: USE localOffset

	NODELOCALOFFSET localOffset = (NODELOCALOFFSET) ReadCode();

	CAnmNode *pNode = LoadNode(localOffset);

	AccumNode(pNode);

#ifdef DUMP_PROTO_BUILD
	dump("UseNode %d\n", localOffset);
#endif
}

void CAnmProtoBuilder::NullNode()
{
	// signature: NullNode
	AccumNode(NULL);

#ifdef DUMP_PROTO_BUILD
	dump("NullNode\n");
#endif
}

void CAnmProtoBuilder::MakeRoute()
{
	NODELOCALOFFSET fromAddr, toAddr;
	CLASSMEMBERID fromMid, toMid;

	// signature:: MakeRoute fromAddr, fromMid, toAddr, toMid
	fromAddr = (NODELOCALOFFSET) ReadCode();
	fromMid = (CLASSMEMBERID) ReadCode();
	toAddr = (NODELOCALOFFSET) ReadCode();
	toMid = (CLASSMEMBERID) ReadCode();

	CAnmNode *pFromNode = LoadNode(fromAddr);
	assert(pFromNode != NULL);

	CAnmNode *pToNode = LoadNode(toAddr);
	assert(pToNode != NULL);

	m_proto->AddRoute(m_app, pFromNode, fromMid, pToNode, toMid);

#ifdef DUMP_PROTO_BUILD
	dump("MakeRoute %d\t%x\t%d\t%x\n", fromAddr, fromMid, toAddr, toMid);
#endif
}

void CAnmProtoBuilder::AddISField(CAnmNode *pChildNode)
{
	CLASSMEMBERID protoMid, childMid;

	// signature: AddISField protoMid, childMid
	protoMid = (CLASSMEMBERID) ReadCode();
	childMid = (CLASSMEMBERID) ReadCode();

	char *protoMidName = protoMid->name;

	if (m_protosymForExtern)
		protoMid = FixupExternMid(protoMid);

	if (protoMid == NULL)
	{
		// N.B.: runtime warning
		char wbuf[ANMMAXSTRBUF];

		String protoname = m_protosym->GetName();
		assert(protoname);

		const char *protonamebuf = protoname->GetBuf();
		assert(protonamebuf);

		sprintf(wbuf, "%s EXTERNPROTO declaration missing field '%s'",
			protonamebuf, protoMidName ? protoMidName : "unknown");

		m_app->Warn(wbuf);

		return;
	}

	assert(m_proto != NULL);
	assert(pChildNode != NULL);

	m_proto->AddISMember(protoMid, pChildNode, childMid);

}

void CAnmProtoBuilder::SetValue(CAnmNode *pNode)
{
	// signature: SetValue mid, value, cloneflag
	CLASSMEMBERID mid = (CLASSMEMBERID) ReadCode();
	void *pValue = (void *) ReadValue(mid->valueType);
	bool cloneflag = (bool) ReadCode();

	assert(pNode != NULL);
	if (cloneflag)
		pValue = CloneValue(mid->valueType, pValue);

	pNode->SetValue(mid, pValue);

#ifdef DUMP_PROTO_BUILD
	dump("SetValue %x %x %s\n", mid, pValue, mid->name);
#endif
}

void CAnmProtoBuilder::SetSFNodeValue(CAnmNode *pNode)
{
	// signature: SetSFNodeValue mid
	CLASSMEMBERID mid = (CLASSMEMBERID) ReadCode();

	void *pValue = (void *) NodeFromNodes();

	assert(pNode != NULL);
	pNode->SetValue(mid, pValue);

#ifdef DUMP_PROTO_BUILD
	dump("SetSFNodeValue %x\n", mid);
#endif
}

void CAnmProtoBuilder::SetMFNodeValue(CAnmNode *pNode)
{
	// signature: SetMFNodeValue mid
	CLASSMEMBERID mid = (CLASSMEMBERID) ReadCode();

	void *pValue = (void *) NodeArrayFromNodes();

	assert(pNode != NULL);
	pNode->SetValue(mid, pValue);

#ifdef DUMP_PROTO_BUILD
	dump("SetMFNodeValue %x\n", mid);
#endif
}

void CAnmProtoBuilder::AppendNode()
{
	// signature: AppendNode mid
	CLASSMEMBERID mid = (CLASSMEMBERID) ReadCode();

	CAnmNode **ppNodeToAppend = NodeFromNodes();

	assert(ppNodeToAppend != NULL);
	assert(*ppNodeToAppend != NULL);

	NodeArray *pNA = GetContainerField(mid);
	assert(pNA != NULL);

	pNA->push_back(*ppNodeToAppend);

#ifdef DUMP_PROTO_BUILD
	dump("SetSFNodeValue %x\n", mid);
#endif
}

void CAnmProtoBuilder::SetSFNodeDefault(CAnmProto *pProto)
{
	// signature: SetSFNodeDefault mid
	CLASSMEMBERID mid = (CLASSMEMBERID) ReadCode();

	void *pValue = (void *) NodeFromNodes();

	assert(pProto != NULL);
	CAnmProtoSymbol *pProtoSym = pProto->GetProtoSymbol();
	assert(pProtoSym);
	CAnmAttributeSymbol *pAttrSym = pProtoSym->FindAttribute(mid);
	if (pAttrSym)
		pAttrSym->SetDefaultValue(CAnmField::CreateField(mid->valueType, pValue));

#ifdef DUMP_PROTO_BUILD
	dump("SetSFNodeDefault %x\n", mid);
#endif
}

void CAnmProtoBuilder::SetMFNodeDefault(CAnmProto *pProto)
{
	// signature: SetMFNodeDefault mid
	CLASSMEMBERID mid = (CLASSMEMBERID) ReadCode();

	void *pValue = (void *) NodeArrayFromNodes();

	assert(pProto != NULL);
	
	assert(pProto != NULL);
	CAnmProtoSymbol *pProtoSym = pProto->GetProtoSymbol();
	assert(pProtoSym);
	CAnmAttributeSymbol *pAttrSym = pProtoSym->FindAttribute(mid);
	if (pAttrSym)
		pAttrSym->SetDefaultValue(CAnmField::CreateField(mid->valueType, pValue));

#ifdef DUMP_PROTO_BUILD
	dump("SetMFNodeDefault %x\n", mid);
#endif
}

CLASSMEMBERID CAnmProtoBuilder::FixupExternMid(CLASSMEMBERID mid)
{
	// N.B.: this is for EXTERNPROTO. The compiled mid's of any of my
	// IS fields might be off because the interface declaration in the
	// refering file and that in the file in which this PROTO is actually
	// defined might be different

	CAnmClass *pClass = m_protosym->GetClass();
	assert(pClass);

	const char *memberName = NULL;
	for (int i = 0; i < pClass->m_numMembers; i++)
	{
		if (mid == pClass->m_memberTable[i])
		{
			memberName = mid->name;
			break;
		}
	}

	if (memberName)
	{
		assert(m_protosymForExtern);
		CAnmAttributeSymbol *pAttrSym = m_protosymForExtern->FindAttribute(memberName);
		if (pAttrSym)
		{
			return pAttrSym->GetClassMember();
		}
	}

	return NULL;
}

// Helpers to manage run-time node stack
void CAnmProtoBuilder::AccumNode(class CAnmNode *pNode)
{
	m_nodeaccum.push_back(pNode);
	m_nodeindex++;
}

void CAnmProtoBuilder::PopNodeAccums()
{
	m_nodeindex = m_nodeindexstack.top();
	m_nodeindexstack.pop();
	m_nodeaccum.resize(m_nodeindex);
}

void CAnmProtoBuilder::ClearNodeAccums()
{
	m_nodeindex = 0;
	m_nodeaccum.clear();
}

Node *CAnmProtoBuilder::NodeFromNodes()
{
	m_node = m_nodeaccum[m_nodeindex-1];
	return &m_node;
}

NodeArray **CAnmProtoBuilder::NodeArrayFromNodes()
{
	int firstindex = m_nodeindexstack.top();
	int lastindex = m_nodeindex;

	SafeUnRef(m_nodearray);
	m_nodearray = new NodeArray;
	m_nodearray->reserve(lastindex - firstindex);

	for (int i = firstindex; i < lastindex; i++)
	{
		CAnmNode *pNode = m_nodeaccum[i];
		m_nodearray->push_back(m_nodeaccum[i]);
	}

	m_nodeindex = firstindex;	// reset current MFNode list
	m_nodeaccum.resize(m_nodeindex);
	return &m_nodearray;
}

// Routines to manage containers stack
void CAnmProtoBuilder::EnterContainerScope()
{
	// Push our current list of container fields, and create a new one
	m_containerstack.push(m_containers);
	m_containers = new sAnmContainers;
}

void CAnmProtoBuilder::LeaveContainerScope(CAnmNode *pNode)
{
	assert(m_containers != NULL);

	// Do SetValue on all containerFields in current scope
	int sz = m_containers->containerFields.size();

	for (int i = 0; i < sz; i++)
	{
		CLASSMEMBERID mid = m_containers->containerFields[i].mid;
		NodeArray *pNA = m_containers->containerFields[i].pNodeArray;
		assert(pNA != NULL);

		void *pValue = (void *) &pNA;
		pNode->SetValue(mid, pValue);
	}

	// Restore current list of container fields to previous scope
	assert(!m_containerstack.empty());
	sAnmContainers *pContainers = m_containerstack.top();
	m_containerstack.pop();
	m_containers = pContainers;
}

NodeArray *CAnmProtoBuilder::GetContainerField(CLASSMEMBERID mid)
{
	assert(m_containers != NULL);

	int sz = m_containers->containerFields.size();

	// Linear list search; it's ok because most nodes will only have 1 or 2 of these, erp
	for (int i = 0; i < sz; i++)
	{
		if (mid == m_containers->containerFields[i].mid)
			return m_containers->containerFields[i].pNodeArray;
	}

	// Not found? Make a new one
	NodeArray *pNA = new NodeArray;
	sAnmContainerField cf(mid, pNA);
	m_containers->containerFields.push_back(cf);

	return pNA;
}


// Local node storage (for def/use)
void CAnmProtoBuilder::StoreNode(class CAnmNode *pNode, NODELOCALOFFSET localOffset)
{
	if (m_proto)
		m_proto->StoreNode(pNode, localOffset);
}

class CAnmNode *CAnmProtoBuilder::LoadNode(NODELOCALOFFSET localOffset)
{
	if (m_proto)
		return m_proto->LoadNode(localOffset);
	else
		return NULL;
}


// ProtoCode generation routines
NODELOCALOFFSET CAnmProtoBuilder::GenDefNode(const char *name)
{
	// signature: DEF name, localOffset
	Gen(eDefNode);
	Gen((ProtoCode) name);
	Gen((ProtoCode) m_numnodelocals);

	return m_numnodelocals++;
}

void CAnmProtoBuilder::GenUseNode(NODELOCALOFFSET nodeLocalOffset)
{
	assert(nodeLocalOffset >= 0 && nodeLocalOffset < m_numnodelocals);

	// signature: USE localOffset
	Gen(eUseNode);
	Gen((ProtoCode) nodeLocalOffset);
}


void CAnmProtoBuilder::GenNullNode()
{
	// signature: NullNode
	Gen((ProtoCode) eNullNode);
}


void CAnmProtoBuilder::GenBeginMakeNode(class CAnmClass *pClass)
{
	// signature: BeginMakeNode pClass
	Gen(eBeginMakeNode);
	Gen((ProtoCode) pClass);
}

void CAnmProtoBuilder::GenEndMakeNode()
{
	// signature: EndMakeNode
	Gen(eEndMakeNode);
}


void CAnmProtoBuilder::GenSetValue(CLASSMEMBERID mid,void *value, bool clonevalue)
{
	// signature: SetValue mid, value, cloneflag
	Gen(eSetValue);
	Gen((ProtoCode) mid);
	GenValue(mid->valueType, value);
	Gen((ProtoCode) clonevalue);
}

void CAnmProtoBuilder::GenSetSFNodeValue(CLASSMEMBERID mid)
{
	// signature: SetSFNodeValue mid
	Gen(eSetSFNodeValue);
	Gen((ProtoCode) mid);
}

void CAnmProtoBuilder::GenSetMFNodeValue(CLASSMEMBERID mid)
{
	// signature: SetMFNodeValue mid
	Gen(eSetMFNodeValue);
	Gen((ProtoCode) mid);
}

void CAnmProtoBuilder::GenAppendNode(CLASSMEMBERID mid)
{
	// signature: AppendNode mid
	Gen(eAppendNode);
	Gen((ProtoCode) mid);
}

void CAnmProtoBuilder::GenSetSFNodeDefault(CLASSMEMBERID mid)
{
	// signature: SetSFNodeDefault mid
	Gen(eSetSFNodeDefault);
	Gen((ProtoCode) mid);
}

void CAnmProtoBuilder::GenSetMFNodeDefault(CLASSMEMBERID mid)
{
	// signature: SetMFNodeDefault mid
	Gen(eSetMFNodeDefault);
	Gen((ProtoCode) mid);
}

void CAnmProtoBuilder::GenMakeRoute(NODELOCALOFFSET fromLocalOffset, CLASSMEMBERID fromMid,
	NODELOCALOFFSET toLocalOffset, CLASSMEMBERID toMid)
{
	// signature:: MakeRoute fromAddr, fromMid, toAddr, toMid
	Gen(eMakeRoute);
	Gen((ProtoCode) fromLocalOffset);
	Gen((ProtoCode) fromMid);
	Gen((ProtoCode) toLocalOffset);
	Gen((ProtoCode) toMid);
}

void CAnmProtoBuilder::GenAddISField(CLASSMEMBERID protoMid, CLASSMEMBERID isMid)
{
	// signature: AddISField protoMid, childMid
	Gen(eAddISField);
	Gen((ProtoCode) protoMid);
	Gen((ProtoCode) isMid);
}

void *CAnmProtoBuilder::ReadValue(eValueType valueType)
{

	int sz;

	switch (valueType)
	{
		// simple (four-byte) types
		case eValueBoolean :
		case eValueBooleanArray :
		case eValueColorArray :
		case eValueDoubleArray :
		case eValueFloat :
		case eValueFloatArray :
		case eValueInteger :
		case eValueImage :
		case eValueIntegerArray :
		case eValueRotationArray :
		case eValueString :
		case eValueStringArray :
		case eValueTimeArray :
		case eValuePoint2Array :
		case eValuePoint3Array :
			sz = 1;
			break;

		// complex (more than four-byte) data
		case eValueColor :
			sz = sizeof(Color) / sizeof(ProtoCode);
			break;

		case eValueDouble :
			sz = sizeof(Double) / sizeof(ProtoCode);
			break;

		case eValueRotation :
			sz = sizeof(Rotation) / sizeof(ProtoCode);
			break;

		case eValueTime :
			sz = sizeof(Time) / sizeof(ProtoCode);
			break;

		case eValuePoint2 :
			sz = sizeof(Point2) / sizeof(ProtoCode);
			break;

		case eValuePoint3 :
			sz = sizeof(Point3) / sizeof(ProtoCode);
			break;

		case eValueNode :
		case eValueNodeArray :
			// falls through; Nodes and NodeArrays are handled elsewhere
		default :

			assert(FALSE);
			break;
	}

	// the value is actually inline in the code stream
	void *retval = (void *) & m_code[m_codeptr];

	// move code pointer past the value
	m_codeptr += sz;

	return retval;
}

void CAnmProtoBuilder::GenValue(eValueType valueType, void *value)
{
	int i;
	int sz;
	ProtoCode *pp = NULL;
	Color c;
	Double d;
	Rotation r;
	String s;
	Time t;
	Point2 p2;
	Point3 p3;
	void *pv = NULL;

	// save the value into the code pointer; it might get overwritten
	pp = (ProtoCode *) value;
	sz = 1;
	switch (valueType)
	{
		// simple (four-byte) types; value is in the void *
		case eValueBoolean :
		case eValueFloat :
		case eValueInteger :
			break;

		// simple (pointer) types; void * is 
		case eValueBooleanArray :
			(*(BooleanArray **)value)->Ref();
			break;

		case eValueColorArray :
			(*(ColorArray **)value)->Ref();
			break;

		case eValueDoubleArray :
			(*(DoubleArray **)value)->Ref();
			break;

		case eValueFloatArray :
			(*(FloatArray **)value)->Ref();
			break;

		case eValueImage :
			(*(Image **)value)->Ref();
			break;

		case eValueIntegerArray :
			(*(IntegerArray **)value)->Ref();
			break;

		case eValueRotationArray :
			(*(RotationArray **)value)->Ref();
			break;

		case eValueString :
			(* (String *) value)->Ref();
			break;

		case eValueStringArray :
			(*(StringArray **)value)->Ref();
			break;

		case eValueTimeArray :
			(*(TimeArray **)value)->Ref();
			break;

		case eValuePoint2Array :
			(*(Point2Array **)value)->Ref();
			break;

		case eValuePoint3Array :
			(*(Point3Array **)value)->Ref();
			break;

		// complex (more than four-byte) data
		case eValueColor :
			c = * (Color *) value;
			sz = sizeof(Color) / sizeof(ProtoCode);
			pp = (ProtoCode *) &c;
			break;

		case eValueDouble :
			d = * (Double *) value;
			sz = sizeof(Double) / sizeof(ProtoCode);
			pp = (ProtoCode *) &d;
			break;

		case eValueRotation :
			r = * (Rotation *) value;
			sz = sizeof(Rotation) / sizeof(ProtoCode);
			pp = (ProtoCode *) &r;
			break;

		case eValueTime :
			t = * (Time *) value;
			sz = sizeof(Time) / sizeof(ProtoCode);
			pp = (ProtoCode *) &t;
			break;

		case eValuePoint2 :
			p2 = * (Point2 *) value;
			sz = sizeof(Point2) / sizeof(ProtoCode);
			pp = (ProtoCode *) &p2;
			break;

		case eValuePoint3 :
			p3 = * (Point3 *) value;
			sz = sizeof(Point3) / sizeof(ProtoCode);
			pp = (ProtoCode *) &p3;
			break;

		case eValueNode :
		case eValueNodeArray :
			// falls through; Nodes and NodeArrays are handled elsewhere
		default :

			assert(FALSE);
			break;
	}

	assert(pp != NULL);
	for (i = 0; i < sz; i++)
	{
		Gen(*pp++);
	}
}

// Clone a pointer-based value; for non-pointer types this returns the original value
// It's kind of a hack because we need a transient pointer-to-pointer, so the value
// gets stored in m_clonedvalue.
// N.B.: DON'T use that except as a transient value
// within ::SetValue - we only did this to get the levels of indirection right

void *CAnmProtoBuilder::CloneValue(eValueType valueType, void *pValue)
{
	void *pNewValue = pValue;

	switch (valueType)
	{
		case eValueBooleanArray :
			m_clonedvalue = (*(BooleanArray **)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

		case eValueColorArray :
			m_clonedvalue = (*(ColorArray **)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

		case eValueDoubleArray :
			m_clonedvalue = (*(DoubleArray **)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

		case eValueFloatArray :
			m_clonedvalue = (*(FloatArray **)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

		case eValueImage :
			m_clonedvalue = (*(Image **)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

		case eValueIntegerArray :
			m_clonedvalue = (*(IntegerArray **)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

		case eValueRotationArray :
			m_clonedvalue = (*(RotationArray **)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

		case eValueString :
			m_clonedvalue = (*(String *)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

		case eValueStringArray :
			m_clonedvalue = (*(StringArray **)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

		case eValueTimeArray :
			m_clonedvalue = (*(TimeArray **)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

		case eValuePoint2Array :
			m_clonedvalue = (*(Point2Array **)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

		case eValuePoint3Array :
			m_clonedvalue = (*(Point3Array **)pValue)->Clone();
			pNewValue = &m_clonedvalue;
			break;

	}


	return pNewValue;
}