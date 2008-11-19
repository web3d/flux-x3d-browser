/********************************************************************************
 * Flux
 *
 * File: anmexecutioncontext.h
 * Description: Execution Context object
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

#ifndef _anmexecutioncontext_h
#define _anmexecutioncontext_h

#include "anmobject.h"

class EXPORT CAnmExecutionContext : public CAnmObject
{
protected :

	class cApplication				*m_app;
	class CAnmProto					*m_proto;
	class CAnmSAINodeEventSink		*m_sink;

	class CAnmSAINode *CreateSAINode(class CAnmNode *pNode);

public :

	// Constructor/destructor
	CAnmExecutionContext(class cApplication *pApp, class CAnmProto *pProto,
		class CAnmSAINodeEventSink *pSink);
	virtual ~CAnmExecutionContext();

	// New methods
	virtual char *GetSpecificationVersion();
	virtual eAnmX3DEncoding GetEncoding();
	virtual class CAnmProfileInfo *GetProfile();
	virtual std::vector<class CAnmComponentInfo *> *GetComponents();
	virtual const char *GetWorldURL();

	virtual class CAnmSAINode *GetNode(const char *nodename);
	virtual class CAnmSAINode *GetImportedNode(const char *nodename);

	virtual class CAnmSAINode *CreateNode(const char *nodetypename);
	virtual class CAnmSAINode *CreateProto(const char *prototypename);
	
	virtual class CAnmProtoSymbol *GetProtoDeclaration(const char *prototypename);
	virtual void GetProtoDeclarations(int *pNProtoDecls, CAnmProtoSymbol ***pppClassSymbols);

	virtual std::vector<class CAnmSAINode *> *GetRootNodes();

	virtual class CAnmSAIRoute *AddRoute(class CAnmSAINode *pFromSAINode,
		const char *fromfield,
		class CAnmSAINode *pToSAINode,
		const char *tofield);

	virtual void RemoveRoute(class CAnmSAIRoute *pSAIRoute);

	virtual std::vector<class CAnmSAIRoute *> *GetRoutes();

	// Accessors
	virtual void SetApp(class cApplication *pApp)
	{
		m_app = pApp;
	}

	virtual class cApplication *GetApp()
	{
		return m_app;
	}

	virtual void SetProto(class CAnmProto *pProto)
	{
		m_proto = pProto;
	}

	virtual class CAnmProto *GetProto()
	{
		return m_proto;
	}

	virtual class CAnmSAINodeEventSink *GetSink()
	{
		return m_sink;
	}

	DECLARE_CLASS(CAnmExecutionContext);
};

#endif // _anmexecutioncontext_h
